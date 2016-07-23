/**
 \addtogroup INTERFACES
 \{
 \addtogroup BLE
 \{
 \addtogroup MANAGER
 \{
 */

/**
 ****************************************************************************************
 * @file ble_mgr.h
 *
 * @brief BLE Manager API
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

#ifndef BLE_MGR_H_
#define BLE_MGR_H_

#include <stdbool.h>
#include "osal.h"
#include "ad_ble.h"
#include "ble_config.h"
#include "ble_gap.h"

/** BLE device parameters */
typedef struct ble_dev_params_tag {
        /** GAP device information */
        char              dev_name[BLE_GAP_DEVNAME_LEN_MAX+1];/**< GAP device name plus NULL char */
        gap_appearance_t  appearance;                         /**< GAP device appearance */
        /** BLE state */
        ble_status_t      status;                             /**< Status of the BLE stack */
        /** Air operations in progress */
        bool              advertising;                        /**< Advertising operation in progress*/
        bool              connecting;                         /**< Connection operation in progress */
        bool              scanning;                           /**< Scanning operation in progress*/
        bool              updating;                           /**< Connection parameter update operation in progress */
        gap_role_t        role;                               /**< Enabled roles for the device */
        /** Privacy parameters */
        uint16_t          addr_renew_duration;                /**< Random address renew duration */
        own_address_t     own_addr;                           /**< Provided own public address */
        irk_t             irk;                                /**< IRK for device resolvable address */
        /** Attribute database configuration */
        uint8_t           att_db_cfg;                         /**< Attribute DB Configuration bitmask */
        uint16_t          mtu_size;                           /**< MTU size */
        /** Channel map (central only) */
        gap_chnl_map_t    channel_map;                        /**< Channel map */
        /** Advertising mode configuration */
        gap_conn_mode_t   adv_type;                           /**< Advertising type */
        gap_disc_mode_t   adv_mode;                           /**< Discoverability mode for adv. */
        uint8_t           adv_channel_map;                    /**< Channel map used for advertising */
        uint16_t          adv_intv_min;                       /**< Minimum advertising interval */
        uint16_t          adv_intv_max;                       /**< Maximum advertising interval */
        adv_filt_pol_t    adv_filter_policy;                  /**< Advertising filter policy */
        bd_address_t      adv_direct_address;                 /**< Address used for directed advertising */
        uint16_t          adv_data_length;                    /**< Length of advertising data */
        uint8_t           adv_data[BLE_ADV_DATA_LEN_MAX];     /**< Advertising data */
        uint16_t          scan_rsp_data_length;               /**< Length of scan response */
        uint8_t           scan_rsp_data[BLE_SCAN_RSP_LEN_MAX];/**< Scan response */
        /** Scan parameters used for connection procedures */
        gap_scan_params_t scan_params;                        /**< Scan parameters */
        /** Peripheral preferred connection parameters */
        gap_conn_params_t gap_ppcp;                           /**< Connection parameters structure */
        /** IO Capabilities configuration */
        gap_io_cap_t      io_capabilities;                    /**< GAP IO capabilities */
} ble_dev_params_t;

/**  Types of stack API messages */
typedef enum ble_stack_msg_types {
        HCI_CMD_MSG = 0x01,   // HCI Command Packet
        HCI_ACL_MSG = 0x02,   // HCI ACL Data Packet
        HCI_SCO_MSG = 0x03,   // HCI Synchronous Data Packet
        HCI_EVT_MSG = 0x04,   // HCI Event Packet
        GTL_MSG     = 0x05,   // GTL Message Packet
} ble_stack_msg_type_t;

/** Definition of BLE stack IRB message */
typedef struct irb_ble_stack_msg {
        uint16_t              op_code;           /**< IRB OP code */
        uint16_t              msg_size;          /**< Message size in bytes */
        ble_stack_msg_type_t  msg_type;          /**< Stack message type (GTL, HCI CMD, HCI ACL,
                                                      HCI SCO or HCI EVT) */
        ble_stack_msg_t       msg;               /**< Stack message place holder */
} irb_ble_stack_msg_t;

/** Definition of BLE stack IRB message */
typedef struct irb_ble_stack_hdr {
        uint16_t              op_code;           /**< IRB OP code */
        uint16_t              msg_size;          /**< Message size in bytes */
        ble_stack_msg_type_t  msg_type;          /**< Stack message type (GTL, HCI CMD, HCI ACL,
                                                      HCI SCO or HCI EVT) */
} irb_ble_stack_hdr_t;

/** BLE Manager interface */
typedef struct {
        OS_TASK  task;     /**< BLE Manager task handle */
        OS_QUEUE cmd_q;    /**< BLE Manager command queue */
        OS_QUEUE evt_q;    /**< BLE Manager event queue */
        OS_QUEUE rsp_q;    /**< BLE Manager response queue */
} ble_mgr_interface_t;

/**
 *  Function declarations
 */

/**
 * \brief Initialize BLE Manager - create command and event queues
 *
 */
void ble_mgr_init(void);

/**
 * \brief Send a message to the BLE Manager's command queue
 *
 * Sends a message to the BLE Manager's command queue and notifies BLE Manager task
 *
 * \param[in] item       Pointer to the item to be sent to the queue.
 * \param[in] wait_ticks Max time in ticks to wait for space in queue.
 *
 * \return OS_OK if the message was successfully sent to the queue, OS_FAIL otherwise.
 */
OS_BASE_TYPE ble_mgr_command_queue_send(const void *item, OS_TICK_TIME wait_ticks);

/**
 * \brief Send a message to the BLE Manager's event queue
 *
 * Sends a message to the BLE Manager's event queue and notifies application task, if any registered.
 *
 * \param[in] item       Pointer to the item to be sent to the queue.
 * \param[in] wait_ticks Max time in ticks to wait for space in queue.
 *
 * \return OS_OK if the message was successfully sent to the queue, OS_FAIL otherwise.
 */
OS_BASE_TYPE ble_mgr_event_queue_send(const void *item, OS_TICK_TIME wait_ticks);

/**
 * \brief Get message from BLE Manager's event queue
 *
 * \param[out] irb        IRB buffer to receive data
 * \param[in]  wait_ticks time to wait
 *
 * \return OS_OK if the message was successfully received from the queue, OS_FAIL otherwise.
 *
 */
OS_BASE_TYPE ble_mgr_event_queue_get(OS_IRB *irb, OS_TICK_TIME wait_ticks);

/**
 * \brief Peek message from BLE Manager's event queue
 *
 * Message is not removed from queue, it will be returned by subsequent call to ble_mgr_event_queue_get().
 *
 * \param[out] irb        IRB buffer to receive data
 * \param[in]  wait_ticks time to wait
 *
 * \return OS_OK if the message was successfully received from the queue, OS_FAIL otherwise.
 *
 */
OS_BASE_TYPE ble_mgr_event_queue_peek(OS_IRB *irb, OS_TICK_TIME wait_ticks);

/**
 * \brief Send a message to the BLE Manager's command queue from an ISR
 *
 * Sends a message to the BLE Manager's command queue
 *
 * \param[in] item       Pointer to the item to be sent to the queue.
 *
 * \return OS_OK if the message was successfully sent to the queue, OS_FAIL otherwise.
 */
OS_BASE_TYPE ble_mgr_command_queue_send_from_isr(const void *item );

/**
 * \brief Get BLE Manager interface structure
 *
 * \return pointer to interface structure
 *
 */
const ble_mgr_interface_t *ble_mgr_get_interface(void);

/**
 * \brief Register application in BLE Manager
 *
 * This can be called only once as there can be only single application registered right now.
 *
 * \param[in] task task handle of application to register
 *
 */
void ble_mgr_register_application(OS_TASK task);

/**
 * \brief Send message to BLE Manager's response queue
 *
 * \param[in] irb        IRB to send to queue
 * \param[in] wait_ticks time to wait
 *
 * \return OS_OK if the message was successfully sent to the queue, OS_FAIL otherwise.
 *
 */
OS_BASE_TYPE ble_mgr_response_queue_send(const OS_IRB *irb, OS_TICK_TIME wait_ticks);

/**
 * \brief Get message from BLE Manager's response queue
 *
 * \param[out] irb        IRB buffer to receive data
 * \param[in]  wait_ticks time to wait
 *
 * \return OS_OK if the message was successfully received from the queue, OS_FAIL otherwise.
 *
 */
OS_BASE_TYPE ble_mgr_response_queue_get(OS_IRB *irb, OS_TICK_TIME wait_ticks);

/**
 * \brief Get a pointer to the BLE device parameters structure
 *
 * \return Pointer to ble_dev_params
 */
ble_dev_params_t *ble_mgr_dev_params_acquire(void);

void ble_mgr_dev_params_release(void);

/**
 * \brief Checks if current task is BLE Manager task
 *
 * \return true if currect task is BLE Manager task, false otherwise
 */
bool ble_mgr_is_own_task(void);

/**
 * \brief Set default dev_params
 *
 * Function set default dev_params
 */
void ble_mgr_dev_params_set_default(void);

/**
 * \brief Notifies BLE Manager to commit storage changes, if any
 *
 */
void ble_mgr_notify_commit_storage(void);

/**
 * \brief Notifies BLE manager that BLE adapter is blocked or unblocked on its event queue
 *
 * BLE adapter uses this function to indicate it has blocked on a full event queue (if \p status is
 * true) or that it has just been unblocked (if \p status is false).
 *
 * \param [in]  status    true to indicate the adapter has just blocked,
 *                        false to indicate the adapter just unblocked.
 */
void ble_mgr_notify_adapter_blocked(bool status);

/**
 * \brief Returns current adapter status (blocked or not)
 *
 * \return true if adapter is blocked, false otherwise
 */
bool ble_mgr_adapter_is_blocked(void);

/**
 * \brief Notify BLE Manager that software reset is started/finished
 */
void ble_mgr_set_reset(bool enable);

#endif /* BLE_MGR_H_ */
/**
 \}
 \}
 \}
 */