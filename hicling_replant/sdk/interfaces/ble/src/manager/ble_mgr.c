/**
 ****************************************************************************************
 *
 * @file ble_mgr.c
 *
 * @brief BLE manager
 *
 * Copyright (C) 2015. Dialog Semiconductor Ltd, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor Ltd. All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 * <black.orca.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

#include <string.h>

#include "ble_config.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "ble_common.h"
#include "ble_gap.h"
#include "ble_irb_helper.h"
#include "ble_mgr.h"
#include "ble_mgr_ad_msg.h"
#include "ble_mgr_gtl.h"
#include "ble_mgr_irb.h"
#include "ble_mgr_irb_common.h"
#include "ble_mgr_config.h"
#include "storage.h"
#include "ad_ble.h"
#include "hw_gpio.h"

#include "gapm_task.h"
#include "gapc_task.h"
#include "gattm_task.h"

#include "sys_watchdog.h"
#include "interrupts.h"

/*------------------------------------- Local definitions ----------------------------------------*/


/* Task stack size */
#ifdef CONFIG_BLE_STORAGE
#define mainBLE_MGR_STACK_SIZE 1024       // with NVMS adapter we need more stack space
#else
#define mainBLE_MGR_STACK_SIZE 512
#endif

/* Task priorities */
#define mainBLE_MGR_PRIORITY              ( OS_TASK_PRIORITY_HIGHEST - 4 )

/* Event group bits */
#define mainBIT_MANAGER_COMMAND_QUEUE     (1 << 0)
#define mainBIT_MANAGER_EVENT_QUEUE       (1 << 0)
#define mainBIT_ADAPTER_EVENT_QUEUE       (1 << 1)
#define mainBIT_COMMIT_STORAGE            (1 << 2)
#define mainBIT_ADAPTER_BLOCKED           (1 << 3)

/*------------------------------------- Local variables ------------------------------------------*/

PRIVILEGED_DATA static const ad_ble_interface_t *adapter_if;    // adapter interface
PRIVILEGED_DATA static ble_mgr_interface_t mgr_if;              // manager interface
PRIVILEGED_DATA static OS_TASK app_task;
PRIVILEGED_DATA static OS_MUTEX ble_dev_params_lock;
PRIVILEGED_DATA static bool reset;      // BLE stack reset in progress
static const ble_dev_params_t default_ble_dev_params = {
        /** Device information */
        .dev_name              = defaultBLE_DEVICE_NAME,
        .appearance            = defaultBLE_APPEARANCE,
        /** BLE state */
        .status                = BLE_IS_DISABLED,
        /** Air operations in progress */
        .advertising           = false,
        .connecting            = false,
        .scanning              = false,
        .updating              = false,
        .role                  = defaultBLE_GAP_ROLE,
        /** Privacy  parameters */
        .addr_renew_duration   = defaultBLE_ADDRESS_RENEW_DURATION,
        .own_addr = {
                .addr_type     = PUBLIC_STATIC_ADDRESS,
                .addr          = defaultBLE_STATIC_ADDRESS
        },
        .irk.key               = defaultBLE_IRK,
        /** Attribute database configuration */
        .att_db_cfg            = defaultBLE_ATT_DB_CONFIGURATION,
        .mtu_size              = defaultBLE_MTU_SIZE,
        /** Channel map (central only) */
        .channel_map = {
                .map           = defaultBLE_CHANNEL_MAP
        },
        /** Advertising mode configuration */
        .adv_mode              = defaultBLE_ADVERTISE_MODE,
        .adv_channel_map       = defaultBLE_ADVERTISE_CHANNEL_MAP,
        .adv_intv_min          = defaultBLE_ADVERTISE_INTERVAL_MIN,
        .adv_intv_max          = defaultBLE_ADVERTISE_INTERVAL_MAX,
        .adv_filter_policy     = defaultBLE_ADVERTISE_FILTER_POLICY,
        .adv_data_length       = defaultBLE_ADVERTISE_DATA_LENGTH,
        .adv_data              = defaultBLE_ADVERTISE_DATA,
        .scan_rsp_data_length  = defaultBLE_SCAN_RESPONSE_DATA_LENGTH,
        .scan_rsp_data         = defaultBLE_SCAN_RESPONSE_DATA,
        /** Scan parameters used for connection procedures */
        .scan_params = {
                .interval = defaultBLE_SCAN_INTERVAL,
                .window   = defaultBLE_SCAN_WINDOW
        },
        /** Peripheral preferred connection parameters */
        .gap_ppcp = {
                .interval_min  = defaultBLE_PPCP_INTERVAL_MIN,
                .interval_max  = defaultBLE_PPCP_INTERVAL_MAX,
                .slave_latency = defaultBLE_PPCP_SLAVE_LATENCY,
                .sup_timeout   = defaultBLE_PPCP_SUP_TIMEOUT
        },
        /** IO Capabilities configuration */
        .io_capabilities       = defaultBLE_GAP_IO_CAP,
};

/* Structure to maintain BLE device parameters. */
PRIVILEGED_DATA static ble_dev_params_t ble_dev_params;

/* Variable used to track BLE adapter status. */
PRIVILEGED_DATA volatile static bool ad_ble_blocked;

/* Flag to hold BLE manager task status. */
PRIVILEGED_DATA volatile static bool ble_mgr_blocked;

/*------------------------------------- Prototypes -----------------------------------------------*/

/*------------------------------------- Global variables -----------------------------------------*/

/**
 * \brief BLE manager task
 */
static void ble_mgr_task(void *pvParameters)
{
        ad_ble_hdr_t *msg_rx;
        uint32_t ulNotifiedValue;
        OS_BASE_TYPE xResult;
        int8_t wdog_id;

        /* Register task to be monitored by watch dog. */
        wdog_id = sys_watchdog_register(false);

        for (;;) {
                /* Notify watch dog on each loop since there's no other trigger for this. */
                sys_watchdog_notify(wdog_id);

                /* Suspend monitoring while task is blocked on OS_TASK_NOTIFY_WAIT(). */
                sys_watchdog_suspend(wdog_id);

                /*
                 * Wait on any of the event group bits, then clear them all.
                 */
                xResult = OS_TASK_NOTIFY_WAIT(0x0, OS_TASK_NOTIFY_ALL_BITS, &ulNotifiedValue,
                                                                            OS_TASK_NOTIFY_FOREVER);
                OS_ASSERT(xResult == OS_OK);

                /* Resume watch dog monitoring. */
                sys_watchdog_notify_and_resume(wdog_id);

                if (ulNotifiedValue & mainBIT_ADAPTER_EVENT_QUEUE) {
                        /* Make sure there are messages waiting on the queue. */
                        if (!uxQueueMessagesWaiting(adapter_if->evt_q)) {
                                goto no_event;
                        }

                        /* Check if there is free space on BLE manager's event queue. */
                        if (uxQueueSpacesAvailable(mgr_if.evt_q)) {
                                /* Get message from queue. */
                                OS_QUEUE_GET(adapter_if->evt_q, &msg_rx, 0);
                                OS_ASSERT(msg_rx->op_code < AD_BLE_OP_CODE_LAST);

#ifdef BLE_STACK_PASSTHROUGH_MODE
                                {
                                        OS_IRB new_irb;

                                        /* Fill-in new IRB fields. */
                                        new_irb.status   = IRB_PENDING;
                                        new_irb.class_id = IRB_BLE;
                                        new_irb.ptr_buf  = msg_rx;

                                        /* Send directly to BLE manager's event queue. */
                                        ble_mgr_event_queue_send(&new_irb, OS_QUEUE_FOREVER);
                                }
#else

                                if (msg_rx->op_code == AD_BLE_OP_CODE_STACK_MSG) {
                                        irb_ble_stack_msg_t *stack_msg =
                                                                      (irb_ble_stack_msg_t*) msg_rx;

                                        /* In non-passthrough we only expect GTL messages. */
                                        OS_ASSERT(stack_msg->msg_type == GTL_MSG);

                                        /*
                                         * during reset we ignore messages other than GAPM_CMP_EVT
                                         * and GAPM_RESET operation
                                         */
                                        if (reset) {
                                                struct gapm_cmp_evt *evt;

                                                if (stack_msg->msg.gtl.msg_id != GAPM_CMP_EVT) {
                                                        goto rx_done;
                                                }

                                                evt = (void *) stack_msg->msg.gtl.param;

                                                if (evt->operation != GAPM_RESET) {
                                                        goto rx_done;
                                                }
                                        }

                                        /*
                                         * Check if someone is waiting for this message.
                                         * if not, try to handle message as an event.
                                         */
                                        if (!ble_gtl_waitqueue_match(&stack_msg->msg.gtl)) {
                                                if (!ble_gtl_handle_event(&stack_msg->msg.gtl)) {
                                                        /* Stack message is not handled by the manager. */
#ifdef DEBUG
                                                        configASSERT(0);
#endif
                                                }
                                        }
                                }
                                else if (msg_rx->op_code == AD_BLE_OP_CODE_ADAPTER_MSG) {
                                        ad_ble_msg_t *ad_msg = (ad_ble_msg_t *) msg_rx;

                                        /* In non-passthrough we only expect GTL messages. */
                                        OS_ASSERT(ad_msg->operation < AD_BLE_OP_LAST);

                                        /* Check if someone is waiting for this message. */
                                        ble_ad_msg_waitqueue_match(ad_msg);

                                }

rx_done:
                                OS_FREE(msg_rx);
#endif
                                /*
                                 * Check if there are more messages waiting in the BLE adapter's
                                 * event queue.
                                 */
                                if (uxQueueMessagesWaiting(adapter_if->evt_q)) {
                                        OS_TASK_NOTIFY(mgr_if.task,
                                                mainBIT_ADAPTER_EVENT_QUEUE, OS_NOTIFY_SET_BITS);
                                }
                        }
                        else {
                                /* Set blocked flag to true. */
                                ble_mgr_blocked = true;
                        }
                }
no_event:
                if (ulNotifiedValue & mainBIT_MANAGER_COMMAND_QUEUE) {
                        if (uxQueueMessagesWaiting(mgr_if.cmd_q)) {
                                OS_IRB irb_rx;

                                /* Get IRB from the queue. */
                                OS_QUEUE_GET(mgr_if.cmd_q, &irb_rx, 0);

                                if (irb_rx.status == IRB_COMPLETED) {
                                        /* Free message buffer if it was not freed by application. */
                                        irb_ble_free_msg(&irb_rx);
                                }
                                else if (irb_rx.status == IRB_ERROR) {
                                        ble_mgr_event_queue_send(&irb_rx, OS_QUEUE_FOREVER);
                                }
                                else if (irb_rx.status == IRB_PENDING) {
                                        /* New IRB from application. */
                                        if (!ble_irb_handle_msg(&irb_rx)) {
                                                /*
                                                 * No handler found for IRB - free command buffer
                                                 * because nothing else will free it.
                                                 */
                                                OS_FREE(irb_rx.ptr_buf);
                                                irb_rx.ptr_buf = NULL;
                                        }
                                }

                                /* Check if blocked and if there is space on the event queue. */
                                if (ble_mgr_blocked && uxQueueSpacesAvailable(mgr_if.evt_q)) {
                                        /* Set flag to false. */
                                        ble_mgr_blocked = false;

                                        /* Notify task to resume getting BLE adapter events. */
                                        OS_TASK_NOTIFY(mgr_if.task, mainBIT_ADAPTER_EVENT_QUEUE,
                                                OS_NOTIFY_SET_BITS);
                                }

                                /* Check if there are messages waiting in the command queue. */
                                if (uxQueueMessagesWaiting(mgr_if.cmd_q)) {
                                        OS_TASK_NOTIFY(mgr_if.task,
                                                mainBIT_MANAGER_COMMAND_QUEUE, OS_NOTIFY_SET_BITS);
                                }
                        }
                }

                /*
                 * Check this bit as last one since previous commands may also update storage. In
                 * such case changes will be written to flash already and there's no need to execute
                 * this twice in a row.
                 */
                if (ulNotifiedValue & mainBIT_COMMIT_STORAGE) {
                        /*
                         * To commit anything modified in storage it's enough to acquire and then
                         * immediately release lock - if dirty flag was set, contents of storage
                         * will be written to flash automatically.
                         */
                        storage_acquire();
                        storage_release();
                }

                /* Check if BLE adapter is blocked and if there is free space on its event queue. */
                if (ble_mgr_adapter_is_blocked() && uxQueueSpacesAvailable(adapter_if->evt_q)) {
                        /* Notify BLE adapter that there is free space on its event queue. */
                        ad_ble_notify_event_queue_avail();
                }
        }
}

/**
 * \brief BLE manager initialization function
 */
void ble_mgr_init(void)
{
        /* Create BLE manager queues. */
        OS_QUEUE_CREATE(mgr_if.cmd_q, sizeof(OS_IRB), BLE_MGR_COMMAND_QUEUE_LENGTH);
        OS_ASSERT(mgr_if.cmd_q);
        OS_QUEUE_CREATE(mgr_if.evt_q, sizeof(OS_IRB), BLE_MGR_EVENT_QUEUE_LENGTH);
        OS_ASSERT(mgr_if.evt_q);
        OS_QUEUE_CREATE(mgr_if.rsp_q, sizeof(OS_IRB), BLE_MGR_RESPONSE_QUEUE_LENGTH);
        OS_ASSERT(mgr_if.rsp_q);
        
        /* Create needed mutexes. */
        (void) OS_MUTEX_CREATE(ble_dev_params_lock);
        configASSERT(ble_dev_params_lock);

        /* Set default BLE device parameters. */
        ble_mgr_dev_params_set_default();

        /* Get BLE adapter interface. */
        adapter_if = ad_ble_get_interface();

        /* Create task. */
        OS_TASK_CREATE("bleM",                 // Text name assigned to the task
                       ble_mgr_task,           // Function implementing the task
                       NULL,                   // No parameter passed
                       mainBLE_MGR_STACK_SIZE, // Size of the stack to allocate to task
                       mainBLE_MGR_PRIORITY,   // Priority of the task
                       mgr_if.task);           // No task handle

        OS_ASSERT(mgr_if.task);

        /* Register to BLE adapter event notifications. */
        ad_ble_event_queue_register(mgr_if.task);
}

const ble_mgr_interface_t *ble_mgr_get_interface()
{
        return &mgr_if;
}

void ble_mgr_register_application(OS_TASK task)
{
        OS_ASSERT(app_task == 0); // single app supported!

        app_task = task;
}

OS_BASE_TYPE ble_mgr_command_queue_send(const void *item, OS_TICK_TIME wait_ticks)
{
        if (OS_QUEUE_PUT(mgr_if.cmd_q, item, wait_ticks) != OS_OK) {
                return OS_FAIL;
        }
        OS_TASK_NOTIFY(mgr_if.task, mainBIT_MANAGER_COMMAND_QUEUE, OS_NOTIFY_SET_BITS);

        return OS_OK;
}

OS_BASE_TYPE ble_mgr_event_queue_send(const void *item, OS_TICK_TIME wait_ticks)
{
        if (in_interrupt()) {
                if (OS_QUEUE_PUT_FROM_ISR(mgr_if.evt_q, item) != OS_OK) {
                        return OS_FAIL;
                }

                if (app_task) {
                        OS_TASK_NOTIFY_FROM_ISR(app_task, mainBIT_MANAGER_EVENT_QUEUE, OS_NOTIFY_SET_BITS);
                }
        } else {
                if (OS_QUEUE_PUT(mgr_if.evt_q, item, wait_ticks) != OS_OK) {
                        return OS_FAIL;
                }

                if (app_task) {
                        OS_TASK_NOTIFY(app_task, mainBIT_MANAGER_EVENT_QUEUE, OS_NOTIFY_SET_BITS);
                }
        }

        return OS_OK;
}

OS_BASE_TYPE ble_mgr_event_queue_get(OS_IRB *irb, OS_TICK_TIME wait_ticks)
{
        return OS_QUEUE_GET(mgr_if.evt_q, irb, wait_ticks);
}

OS_BASE_TYPE ble_mgr_event_queue_peek(OS_IRB *irb, OS_TICK_TIME wait_ticks)
{
        return OS_QUEUE_PEEK(mgr_if.evt_q, irb, wait_ticks);
}

OS_BASE_TYPE ble_mgr_command_queue_send_from_isr(const void *item)
{
        if (OS_QUEUE_PUT_FROM_ISR(mgr_if.cmd_q, item) != OS_OK) {
                return OS_FAIL;
        }
        OS_TASK_NOTIFY(mgr_if.task, mainBIT_MANAGER_COMMAND_QUEUE, OS_NOTIFY_SET_BITS);

        return OS_OK;
}

OS_BASE_TYPE ble_mgr_response_queue_send(const OS_IRB *irb, OS_TICK_TIME wait_ticks)
{
        return OS_QUEUE_PUT(mgr_if.rsp_q, irb, wait_ticks);
}

OS_BASE_TYPE ble_mgr_response_queue_get(OS_IRB *irb, OS_TICK_TIME wait_ticks)
{
        return OS_QUEUE_GET(mgr_if.rsp_q, irb, wait_ticks);
}

ble_dev_params_t *ble_mgr_dev_params_acquire(void)
{
        OS_MUTEX_GET(ble_dev_params_lock, OS_MUTEX_FOREVER);

        return &ble_dev_params;
}

void ble_mgr_dev_params_release(void)
{
        OS_MUTEX_PUT(ble_dev_params_lock);
}

bool ble_mgr_is_own_task(void)
{
        return OS_GET_CURRENT_TASK() == mgr_if.task;
}

void ble_mgr_notify_commit_storage(void)
{
        OS_TASK_NOTIFY(mgr_if.task, mainBIT_COMMIT_STORAGE, OS_NOTIFY_SET_BITS);
}

__INLINE bool ble_mgr_adapter_is_blocked(void)
{
        return ad_ble_blocked;
}

void ble_mgr_notify_adapter_blocked(bool status)
{
        ad_ble_blocked = status;

        if (status) {
                OS_TASK_NOTIFY(mgr_if.task, mainBIT_ADAPTER_BLOCKED, OS_NOTIFY_SET_BITS);
        }
}

void ble_mgr_dev_params_set_default(void)
{
        memcpy(&ble_dev_params, &default_ble_dev_params, sizeof(ble_dev_params_t));
}

void ble_mgr_set_reset(bool enable)
{
        reset = enable;
}
