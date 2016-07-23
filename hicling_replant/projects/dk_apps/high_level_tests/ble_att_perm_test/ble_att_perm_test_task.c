/**
 ****************************************************************************************
 *
 * @file ble_att_perm_test_task.c
 *
 * @brief BLE peripheral task
 *
 * Copyright (C) 2016. Dialog Semiconductor Ltd, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 * <black.orca.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>
#include <stdio.h>
#include "osal.h"
#include "sys_watchdog.h"
#include "ble_att.h"
#include "ble_common.h"
#include "ble_gap.h"
#include "ble_gatts.h"
#include "ble_service.h"
#include "ble_uuid.h"
#include "ble_att_perm_test_config.h"
#include "dlg_debug.h"
#include "bas.h"
#include "cts.h"
#include "dis.h"
#include "scps.h"

/*
 * Notification bits reservation
 * bit #0 is always assigned to BLE event queue notification
 */
#define CTS_SET_TIME_NOTIF (1 << 2)
#define BCS_TIMER_NOTIF (1 << 3)

/*
 * BLE peripheral advertising data
 */
static const uint8_t adv_data[] = {
        0x12, GAP_DATA_TYPE_LOCAL_NAME,
        'B', 'L', 'E', ' ', 'A', 'T', 'T', ' ', 'P', 'E', 'R', 'M', ' ', 'T', 'E', 'S', 'T',
};

static OS_TASK ble_att_perm_test_task_handle;

PRIVILEGED_DATA static OS_TIMER event_timer;
/*
 * Custom service data
 */

#define INVALID_HANDLE 0xFFFF
uint16_t active_connection = INVALID_HANDLE;
uint8_t rolling_value = 0;

static void handle_evt_gap_passkey_notify(ble_evt_gap_passkey_notify_t * evt)
{
        printf("conn_idx=%d passkey=%06" PRIu32 "\r\n", evt->conn_idx, evt->passkey);
}

#define MAX_CHARACTERISTICS 8

PRIVILEGED_DATA static uint16_t h[MAX_CHARACTERISTICS];

static void event_timer_cb(OS_TIMER timer)
{
        printf("Tick\r\n");
        rolling_value++;

        if (active_connection != INVALID_HANDLE) {
                ble_gatts_send_event(active_connection, h[7], GATT_EVENT_NOTIFICATION, sizeof(rolling_value), &rolling_value);
        }
}


static void handle_write_cb(const ble_evt_gatts_write_req_t *evt)
{
        att_error_t status = ATT_ERROR_ATTRIBUTE_NOT_FOUND;

        for (uint32_t i = 0 ; i < MAX_CHARACTERISTICS; i++) {
                if (evt->handle == h[i]) {
                        ble_gatts_set_value(h[i], evt->length, evt->value);
                        status = ATT_ERROR_OK;
                        break;
                }
        }

        ble_gatts_write_cfm(evt->conn_idx, evt->handle, status);
}


static void myservice_init(ble_service_t *include_svc)
{
        att_uuid_t uuid;

        /*
         * This service does absolutely nothing, it just checks that it's possible to use 128-bit
         * UUIDs for services, characteristics and descriptors.
         */

        ble_uuid_from_string("91a7608d-4456-479d-b9b1-4706e8711cf8", &uuid);
        ble_gatts_add_service(&uuid, GATT_SERVICE_PRIMARY, ble_gatts_get_num_attr(1, MAX_CHARACTERISTICS, 0));

        ble_gap_set_io_cap(GAP_IO_CAP_DISP_ONLY);

        if (include_svc) {
                ble_gatts_add_include(include_svc->start_h, NULL);
        }

        ble_uuid_from_string("25047e64-657c-4856-afcf-e315048a965c", &uuid);
        uint16_t *p = h;

#define NEW_CHAR(prop, perm) \
        uuid.uuid128[0]++; ble_gatts_add_characteristic(&uuid, (prop), (perm), 1, 0, NULL, p); p++;

        NEW_CHAR(GATT_PROP_READ,                    ATT_PERM_READ                           );
        NEW_CHAR(GATT_PROP_READ,                    ATT_PERM_READ_ENCRYPT                   );
        NEW_CHAR(GATT_PROP_READ,                    ATT_PERM_READ_AUTH                      );
        NEW_CHAR(GATT_PROP_READ | GATT_PROP_WRITE,  ATT_PERM_READ | ATT_PERM_WRITE          );
        NEW_CHAR(GATT_PROP_READ | GATT_PROP_WRITE,  ATT_PERM_READ | ATT_PERM_WRITE_ENCRYPT  );
        NEW_CHAR(GATT_PROP_READ | GATT_PROP_WRITE,  ATT_PERM_READ | ATT_PERM_WRITE_AUTH     );
        NEW_CHAR(GATT_PROP_READ | GATT_PROP_WRITE,  ATT_PERM_READ_AUTH | ATT_PERM_WRITE_AUTH);
        NEW_CHAR(GATT_PROP_READ | GATT_PROP_NOTIFY, ATT_PERM_READ | ATT_PERM_WRITE          );

        ble_gatts_register_service(NULL, h + 0, h + 1, h + 2, h + 3, h + 4, h + 5, h + 6, h + 7, NULL);

        for (uint8_t i = 0; i < MAX_CHARACTERISTICS; i++) {
                uint8_t value = i + 0x80;
                ble_gatts_set_value(h[i], sizeof(value), &value);
        }
}

/*
 * Main code
 */
static void handle_evt_gap_connected(ble_evt_gap_connected_t *evt)
{
        active_connection = evt->conn_idx;
}


static void handle_evt_gap_disconnected(ble_evt_gap_disconnected_t *evt)
{
        active_connection = INVALID_HANDLE;
}


static void handle_evt_gap_adv_completed(ble_evt_gap_adv_completed_t *evt)
{
        // restart advertising so we can connect again
        ble_gap_adv_start(GAP_CONN_MODE_UNDIRECTED);
}

void ble_att_perm_test_task(void *params)
{
        int8_t wdog_id;
        ble_service_t *svc;

        // in case services which do not use svc are all disabled, just surpress -Wunused-variable
        (void) svc;

        /* register ble_att_perm_test task to be monitored by watchdog */
        wdog_id = sys_watchdog_register(false);

        ble_att_perm_test_task_handle = OS_GET_CURRENT_TASK();

        srand(time(NULL));

        ble_peripheral_start();
        ble_register_app();

        ble_gap_device_name_set("BLE ATT PERM TEST", ATT_PERM_READ);

        /* register custom service */
        myservice_init(NULL);

        event_timer = OS_TIMER_CREATE("evt", portCONVERT_MS_2_TICKS(5000), OS_TIMER_SUCCESS,
                                                (void *) OS_GET_CURRENT_TASK(), event_timer_cb);
        OS_ASSERT(event_timer);
        OS_TIMER_START(event_timer, 10);

        ble_gap_adv_data_set(sizeof(adv_data), adv_data, 0, NULL);
        ble_gap_adv_start(GAP_CONN_MODE_UNDIRECTED);

        for (;;) {
                /* notify watchdog on each loop */
                sys_watchdog_notify(wdog_id);

                /* suspend watchdog while blocking on OS_TASK_NOTIFY_WAIT() */
                sys_watchdog_suspend(wdog_id);

                /*
                 * Wait on any of the notification bits, then clear them all
                 */
                uint32_t notif;
                OS_BASE_TYPE ret = OS_TASK_NOTIFY_WAIT(0, OS_TASK_NOTIFY_ALL_BITS, &notif, OS_TASK_NOTIFY_FOREVER);
                /* Blocks forever waiting for task notification. The return value must be OS_OK */
                OS_ASSERT(ret == OS_OK);

                /* resume watchdog */
                sys_watchdog_notify_and_resume(wdog_id);

                /* notified from BLE manager, can get event */
                if (notif & BLE_APP_NOTIFY_MASK) {
                        ble_evt_hdr_t *hdr = ble_get_event(false);
                        if (!hdr) {
                                goto no_event;
                        }

                        if (ble_service_handle_event(hdr)) {
                                goto handled;
                        }

                        switch (hdr->evt_code) {

                        case BLE_EVT_GATTS_WRITE_REQ:
                                handle_write_cb((const ble_evt_gatts_write_req_t *) hdr);
                                break;

                        case BLE_EVT_GAP_CONNECTED:
                                handle_evt_gap_connected((ble_evt_gap_connected_t *) hdr);
                                break;
                        case BLE_EVT_GAP_DISCONNECTED:
                                handle_evt_gap_disconnected((ble_evt_gap_disconnected_t *) hdr);
                                break;
                        case BLE_EVT_GAP_ADV_COMPLETED:
                                handle_evt_gap_adv_completed((ble_evt_gap_adv_completed_t *) hdr);
                                break;
                        case BLE_EVT_GAP_PAIR_REQ:
                        {
                                ble_evt_gap_pair_req_t *evt = (ble_evt_gap_pair_req_t *) hdr;
                                ble_gap_pair_reply(evt->conn_idx, true, evt->bond);
                                break;
                        }
                        case BLE_EVT_GAP_PASSKEY_NOTIFY:
                                handle_evt_gap_passkey_notify((ble_evt_gap_passkey_notify_t *) hdr);
                                break;
                        default:
                                ble_handle_event_default(hdr);
                                break;
                        }
handled:
                        OS_FREE(hdr);

no_event:
                        // notify again if there are more events to process in queue
                        if (ble_has_event()) {
                                OS_TASK_NOTIFY(OS_GET_CURRENT_TASK(), BLE_APP_NOTIFY_MASK, eSetBits);
                        }
                }
        }
}
