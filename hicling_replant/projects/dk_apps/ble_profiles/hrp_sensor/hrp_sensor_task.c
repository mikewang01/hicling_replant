/**
 ****************************************************************************************
 *
 * @file profile_task.c
 *
 * @brief Profiles demo task
 *
 * Copyright (C) 2015. Dialog Semiconductor Ltd, unpublished work. This computer
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
#include <string.h>
#include "osal.h"
#include "sys_watchdog.h"
#include "ble_common.h"
#include "ble_service.h"
#include "hrs.h"
#include "dis.h"
#include "hw_gpio.h"
#include "hrp_sensor_config.h"

/*
 * Support for multiple concurrent connections
 *
 * If set to zero (default), application will restart advertising upon disconnection of existing
 * client making possible for only one client being connected at given time.
 * If set to non-zero, application will restart advertising immediately after it was stopped due
 * to incoming connected. This allows for multiple clients to be connected at the same time.
 */
#define CFG_MULTIPLE_CLIENTS (0)

/*
 * Notification bits reservation
 *
 * Bit #0 is always assigned to BLE event queue notification.
 */
#define HRS_TIMER_NOTIF (1 << 1)

/*
 * HRP advertising and scan response data
 *
 * As per HRP specification, sensor device should include HRS UUID in advertising data and local name
 * in either advertising data or scan response.
 */
static const uint8_t adv_data[] = {
        0x03, GAP_DATA_TYPE_UUID16_LIST_INC,
        0x0D, 0x18, // = 0x180D (HRS UUID)
};

static const uint8_t scan_rsp[] = {
        0x11, GAP_DATA_TYPE_LOCAL_NAME,
        'D', 'i', 'a', 'l', 'o', 'g', ' ', 'H', 'R', ' ', 'S', 'e', 'n', 's', 'o', 'r'
};

/*
 * Device Information Service data
 *
 * Manufacturer Name String is mandatory for devices supporting HRP.
 */
static const dis_device_info_t dis_info = {
        .manufacturer = "Dialog Semiconductor",
        .model_number = "Dialog BLE",
};

/* Callbacks from HRS implementation */
static void hrs_notif_changed_cb(uint16_t conn_idx, bool enabled);
static void hrs_ee_reset_cb(uint16_t conn_idx);

static const hrs_callbacks_t hrs_cb = {
        .notif_changed = hrs_notif_changed_cb,
        .ee_reset      = hrs_ee_reset_cb,
};

/* Timer used to send measurements */
PRIVILEGED_DATA static OS_TIMER hrs_timer;

/* Measurement data */
INITIALISED_PRIVILEGED_DATA static hrs_measurement_t hrs_meas = {
        .contact_supported = true,
        .contact_detected = true,
        .has_energy_expended = false,
        .energy_expended = 0,
        .rr_num = 0,
        .rr = { 0, 0, 0, 0 }, // placeholder for up to 4 RR-Interval values
};

/* HRS timer callback */
static void hrp_timer_cb(OS_TIMER timer)
{
        OS_TASK task = (OS_TASK) OS_TIMER_GET_TIMER_ID(timer);

        OS_TASK_NOTIFY(task, HRS_TIMER_NOTIF, eSetBits);
}

/* Notification state for Heart Rate Measurement changed by peer */
static void hrs_notif_changed_cb(uint16_t conn_idx, bool enabled)
{
        if (enabled) {
                OS_TIMER_START(hrs_timer, 1000 / OS_PERIOD_MS);
        } else {
                OS_TIMER_STOP(hrs_timer, 0);
        }
}

/* Reset Energy Expended requested by peer */
static void hrs_ee_reset_cb(uint16_t conn_idx)
{
        hrs_meas.energy_expended = 0;
}

#if !CFG_MULTIPLE_CLIENTS
static void handle_evt_gap_disconnected(ble_evt_gap_disconnected_t *evt)
{
        ble_gap_adv_start(GAP_CONN_MODE_UNDIRECTED);
}
#else
static void handle_evt_gap_adv_completed(ble_evt_gap_adv_completed_t *evt)
{
        ble_gap_adv_start(GAP_CONN_MODE_UNDIRECTED);
}
#endif

static void handle_evt_gap_pair_req(ble_evt_gap_pair_req_t *evt)
{
        ble_gap_pair_reply(evt->conn_idx, true, evt->bond);
}

void hrp_sensor_task(void *params)
{
        ble_service_t *hrs;
        int8_t wdog_id;

        /* register hrp_sensor task to be monitored by watchdog */
        wdog_id = sys_watchdog_register(false);

        ble_peripheral_start();
        ble_register_app();

        /*
         * Set device name
         */
        ble_gap_device_name_set("Dialog HR Sensor", ATT_PERM_READ);

        /*
         * Register HRS
         *
         * HRS instance should be registered in ble_service framework in order for events inside
         * service to be processed properly.
         */
        hrs = hrs_init(HRS_SENSOR_LOC_CHEST, &hrs_cb);
        ble_service_add(hrs);

        /*
         * Register DIS
         *
         * DIS doesn't contain any dynamic data thus it doesn't need to be registered in ble_service
         * framework (but it's not an error to do so).
         */
        dis_init(NULL, &dis_info);

        /*
         * Create timer for HRS which will be used to send measurement every 1 second once client
         * is connected.
         */
        hrs_timer = OS_TIMER_CREATE("hrs", 1000 / OS_PERIOD_MS, OS_TIMER_SUCCESS,
                                                (void *) OS_GET_CURRENT_TASK(), hrp_timer_cb);
        OS_ASSERT(hrs_timer);

        /*
         * Set advertising data and scan response, then start advertising.
         */
        ble_gap_adv_data_set(sizeof(adv_data), adv_data, sizeof(scan_rsp), scan_rsp);
        ble_gap_adv_start(GAP_CONN_MODE_UNDIRECTED);

        for (;;) {
                OS_BASE_TYPE ret;
                uint32_t notif;

                /* notify watchdog on each loop */
                sys_watchdog_notify(wdog_id);

                /* suspend watchdog while blocking on OS_TASK_NOTIFY_WAIT() */
                sys_watchdog_suspend(wdog_id);

                /*
                 * Wait on any of the notification bits, then clear them all
                 */
                ret = OS_TASK_NOTIFY_WAIT(0, OS_TASK_NOTIFY_ALL_BITS, &notif, OS_TASK_NOTIFY_FOREVER);
                OS_ASSERT(ret == OS_OK);

                /* resume watchdog */
                sys_watchdog_notify_and_resume(wdog_id);

                /* Notified from BLE Manager? */
                if (notif & BLE_APP_NOTIFY_MASK) {
                        ble_evt_hdr_t *hdr;

                        hdr = ble_get_event(false);
                        if (!hdr) {
                                goto no_event;
                        }

                        /*
                         * First, application needs to try pass event through ble_framework.
                         * Then it can handle it itself and finally pass to default event handler.
                         */
                        if (!ble_service_handle_event(hdr)) {
                                switch (hdr->evt_code) {
#if !CFG_MULTIPLE_CLIENTS
                                case BLE_EVT_GAP_DISCONNECTED:
                                        handle_evt_gap_disconnected((ble_evt_gap_disconnected_t *) hdr);
                                        break;
#else
                                case BLE_EVT_GAP_ADV_COMPLETED:
                                        handle_evt_gap_adv_completed((ble_evt_gap_adv_completed_t *) hdr);
                                        break;
#endif
                                case BLE_EVT_GAP_PAIR_REQ:
                                        handle_evt_gap_pair_req((ble_evt_gap_pair_req_t *) hdr);
                                        break;
                                default:
                                        ble_handle_event_default(hdr);
                                        break;
                                }
                        }

                        /* Free event buffer (it's not needed anymore) */
                        OS_FREE(hdr);

no_event:
                        /*
                         * If there are more events waiting in queue, application should process
                         * them now.
                         */
                        if (ble_has_event()) {
                                OS_TASK_NOTIFY(OS_GET_CURRENT_TASK(), BLE_APP_NOTIFY_MASK, eSetBits);
                        }
                }

                /* Notified from HRS timer? */
                if (notif & HRS_TIMER_NOTIF) {
                        static unsigned int hrs_tick = 0;

                        /*
                         * Since this is not real HR sensor, application will produce some fake
                         * data which are similar to what real sensor could send. In addition, these
                         * allow to pass PTS testcases for HRS and HRP.
                         */

                        /* BPM will be 255+/-5 to send both 8- and 16-bit values */
                        hrs_meas.bpm = 80 + rand() % 11;

                        /* Check if trigger pin is set to low (GND) to send 16-bit value */
                        if (!hw_gpio_get_pin_status(CFG_SEND_16_BIT_VALUE_TRIGGER_GPIO_PORT,
                                                         CFG_SEND_16_BIT_VALUE_TRIGGER_GPIO_PIN)) {
                                /* BPM will be (80+/-5) + 200 to send 16-bit values */
                                hrs_meas.bpm += 200;
                        }

                        /* Toggle Contact Detected bit every 4 measurements */
                        if (hrs_tick % 4 == 0) {
                                hrs_meas.contact_detected = !hrs_meas.contact_detected;
                        }

                        /* Energy Expended will be sent every 10 measurements */
                        hrs_meas.has_energy_expended = (hrs_tick % 10 == 0);

                        /* RR-Interval will be sent every time but with different settings */
                        hrs_meas.rr_num = 1 + rand() % 2;
                        hrs_meas.rr[0] = 1000 + rand() % 21;
                        if (hrs_meas.rr_num > 1) {
                                hrs_meas.rr[1] = 1000 + rand() % 21;
                        }

                        /*
                         * There's no need for application to control CCC configurations for each
                         * connected client - just call API as below and service will take care of
                         * it internally and will only send notification to each client which
                         * enabled it.
                         */
                        hrs_notify_measurement_all(hrs, &hrs_meas);

                        /*
                         * Energy Expended is increased every time it was sent in measurement.Also
                         * it should stay at maximum value (65535) once reached.
                         *
                         * Value is increased after measurement is sent in order to be able to send
                         * measurement with EE=0 which allows to check whether EE is reset properly.
                         */
                        if (hrs_meas.has_energy_expended && hrs_meas.energy_expended < 0xffff) {
                                hrs_meas.energy_expended++;
                        }

                        hrs_tick++;
                }

        }
}
