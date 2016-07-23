/**
 ****************************************************************************************
 *
 * @file ble_cling_service.c
 *
 * @brief ble_cling_service protocol file
 *
 * Copyright (C) 2015. hicling elec Ltd, unpublished work. This computer
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
#include "osal.h"
#include "sys_watchdog.h"
#include "ble_att.h"
#include "ble_common.h"
#include "ble_gap.h"
#include "ble_gatts.h"
#include "ble_service.h"
#include "ble_uuid.h"
#include "ble_peripheral_config.h"
#include "dlg_debug.h"
#include "bas.h"
#include "cts.h"
#include "dis.h"
#include "scps.h"
#include "stdio.h"
#include "main.h"
#include "ble_storage.h"
#include "ble_cus.h"
#include "ad_flash.h"
#include "ble_hal.h"
#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
 * MACROS
 */
#define DEF_HICLING_NOTIFY1_POINT_USER_DESC     "NOTIFY1_POINT"
#define DEF_HICLING_NOTIFY2_POINT_USER_DESC     "NOTIFY2_POINT"
#define DEF_HICLING_NOTIFY3_POINT_USER_DESC     "NOTIFY3_POINT"
#define DEF_HICLING_NOTIFY4_POINT_USER_DESC     "NOTIFY4_POINT"

#define DEF_HICLING_WRITE1_POINT_USER_DESC     "WRITE1_POINT"
#define DEF_HICLING_WRITE2_POINT_USER_DESC     "WRITE2_POINT"
#define DEF_HICLING_WRITE3_POINT_USER_DESC     "WRITE3_POINT"
#define DEF_HICLING_WRITE4_POINT_USER_DESC     "WRITE4_POINT"

#define CONFIG_BLE_DLGDEBUG_MAX_CP_LEN 64

#define HANDLE_STEPS_EVEY_VALUE_WITH_NTFPRO ble_gatts_get_num_attr(0,1,1)
#define HANDLE_STEPS_EVEY_VALUE             ble_gatts_get_num_attr(0,1,0)

/*********************************************************************
 * TYPEDEFS
 */

enum {
        HICLING_IDX_SVC = 0,

        HICLING_IDX_NOTIFY1_POINT_CHAR,
        HICLING_IDX_NOTIFY1_POINT_VAL,
        HICLING_IDX_NOTIFY1_POINT_CFG,
        HICLING_IDX_NOTIFY1_POINT_USER_DESC,

        HICLING_IDX_NOTIFY2_POINT_CHAR,
        HICLING_IDX_NOTIFY2_POINT_VAL,
        HICLING_IDX_NOTIFY2_POINT_CFG,
        HICLING_IDX_NOTIFY2_POINT_USER_DESC,

        HICLING_IDX_NOTIFY3_POINT_CHAR,
        HICLING_IDX_NOTIFY3_POINT_VAL,
        HICLING_IDX_NOTIFY3_POINT_CFG,
        HICLING_IDX_NOTIFY3_POINT_USER_DESC,

        HICLING_IDX_NOTIFY4_POINT_CHAR,
        HICLING_IDX_NOTIFY4_POINT_VAL,
        HICLING_IDX_NOTIFY4_POINT_CFG,
        HICLING_IDX_NOTIFY4_POINT_USER_DESC,

        HICLING_IDX_WRITE1_POINT_CHAR,
        HICLING_IDX_WRITE1_POINT_VAL,
        HICLING_IDX_WRITE1_POINT_USER_DESC,

        HICLING_IDX_WRITE2_POINT_CHAR,
        HICLING_IDX_WRITE2_POINT_VAL,
        HICLING_IDX_WRITE2_POINT_USER_DESC,

        HICLING_IDX_WRITE3_POINT_CHAR,
        HICLING_IDX_WRITE3_POINT_VAL,
        HICLING_IDX_WRITE3_POINT_USER_DESC,

        HICLING_IDX_WRITE4_POINT_CHAR,
        HICLING_IDX_WRITE4_POINT_VAL,
        HICLING_IDX_WRITE4_POINT_USER_DESC,

        HICLING_IDX_NB
};

enum {
        HICLING_SERVICE_UUID_16 = 0XFFE0,
        HICLING_NOTIFY1_POINT_UUID_16 = 0xFFE1,
        HICLING_NOTIFY2_POINT_UUID_16,
        HICLING_NOTIFY3_POINT_UUID_16,
        HICLING_NOTIFY4_POINT_UUID_16,
        HICLING_NOTIFY_END_POINT_UUID_16,

        HICLING_WRITE1_POINT_UUID_16 = 0xFFE5,
        HICLING_WRITE2_POINT_UUID_16,
        HICLING_WRITE3_POINT_UUID_16,
        HICLING_WRITE4_POINT_UUID_16,
        HICLING_WRITE_END_POINT_UUID_16,

};

typedef struct {
        ble_service_t svc;

        // handles
        uint16_t cp_val_h[HICLING_WRITE_END_POINT_UUID_16 - HICLING_NOTIFY1_POINT_UUID_16];
        uint16_t cp_ccc_h[HICLING_NOTIFY_END_POINT_UUID_16 - HICLING_NOTIFY1_POINT_UUID_16];

        void *handlers;
} cling_service_t;

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static struct task_handle_sets task_hanles;
static int (*ble_write_callback_p)(ble_service_t *svc, uint16_t conn_idx, uint16_t uuid_num,
        uint8_t *s, size_t size) = NULL;
static int (*ble_on_conn_callback_p)(ble_service_t *svc, uint16_t conn_idx) = NULL;
static int (*ble_on_disconn_callback_p)() = NULL;

static const uint16 uuid_check_table[] = {
        HICLING_SERVICE_UUID_16,
        0,
        /*0*3 OFFSET HANDLE*/
        HICLING_NOTIFY1_POINT_UUID_16,
        0,
        0,
        /*183 OFFSET HANDLE*/
        HICLING_NOTIFY2_POINT_UUID_16,
        0,
        0,

        /*2*3 OFFSET HANDLE*/
        HICLING_NOTIFY3_POINT_UUID_16,
        0,
        0,

        /*3*3 OFFSET HANDLE*/
        HICLING_NOTIFY4_POINT_UUID_16,
        0,
        0,

        /*4*2 OFFSET HANDLE*/
        HICLING_WRITE1_POINT_UUID_16,
        0,
        /*5*2 OFFSET HANDLE*/
        HICLING_WRITE2_POINT_UUID_16,
        0,
        /*6*2 OFFSET HANDLE*/
        HICLING_WRITE3_POINT_UUID_16,
        0,
        /*7*2 OFFSET HANDLE*/
        HICLING_WRITE4_POINT_UUID_16,
        0,

};
/*********************************************************************
 * LOCAL DECLARATION
 *
 */
static void handle_read_req(ble_service_t *svc, const ble_evt_gatts_read_req_t *evt);
static void handle_write_req(ble_service_t *svc, const ble_evt_gatts_write_req_t *evt);
static void handle_prepare_write_req(ble_service_t *svc,
        const ble_evt_gatts_prepare_write_req_t *evt);

static void handle_conection_req(ble_service_t *svc, const ble_evt_gap_connected_t *evt);
static void hanlde_disconnected_evt_t(ble_service_t *svc, const ble_evt_gap_disconnected_t *evt);

int cling_service_reg_write_callback(
        int (*p)(ble_service_t *svc, uint16_t conn_idx, uint16_t uuid_num, uint8_t *s, size_t size))
{
        if (p != NULL) {
                ble_write_callback_p = p;
        }
        return BLE_STATUS_OK;

}

int cling_service_reg_conn_callback(int (*p)(ble_service_t *svc, uint16_t conn_idx))
{
        if (p != NULL) {
                ble_on_conn_callback_p = p;
        }
        return BLE_STATUS_OK;

}

int cling_service_reg_disconn_callback(int (*p)(void))
{
        if (p != NULL) {
                ble_on_disconn_callback_p = p;
        }
        return BLE_STATUS_OK;

}

ble_service_t *cling_service_init(const ble_service_config_t *cfg, struct task_handle_sets sets)
{

        task_hanles = sets;
        /*initilate btle interface*/
        cling_service_t *dbgs;
        uint16_t num_attr;
        att_uuid_t uuid;
        uint16_t cp_cpf_h;
        uint8_t cp_cpf_val[7];
        uint8_t *p = cp_cpf_val;
        dbgs = OS_MALLOC(sizeof(*dbgs));
        memset(dbgs, 0, sizeof(*dbgs));
        /*add callback function*/

        dbgs->svc.connected_evt = handle_conection_req;
        dbgs->svc.disconnected_evt = hanlde_disconnected_evt_t;
        dbgs->svc.read_req = handle_read_req;
        dbgs->svc.write_req = handle_write_req;
        dbgs->svc.prepare_write_req = handle_prepare_write_req;
        /*add service and charactor*/
        num_attr = ble_gatts_get_num_attr(0,
                HICLING_WRITE_END_POINT_UUID_16 - HICLING_NOTIFY1_POINT_UUID_16,
                HICLING_NOTIFY_END_POINT_UUID_16 - HICLING_NOTIFY1_POINT_UUID_16);

        ble_uuid_create16(HICLING_SERVICE_UUID_16, &uuid);
        int ret = ble_gatts_add_service(&uuid, GATT_SERVICE_PRIMARY, num_attr);

        printf("[ble cust]: ble_gatts_add_service ret = 0x%04x\r\n", ret);
        fflush(stdout);

        for (int i = HICLING_NOTIFY1_POINT_UUID_16; i < HICLING_NOTIFY_END_POINT_UUID_16; i++) {
                ble_uuid_create16(i, &uuid);
                ble_gatts_add_characteristic(&uuid, GATT_PROP_WRITE | GATT_PROP_NOTIFY,
                        ATT_PERM_WRITE, CONFIG_BLE_DLGDEBUG_MAX_CP_LEN, 0, NULL,
                        &(dbgs->cp_val_h[i - HICLING_NOTIFY1_POINT_UUID_16]));
                ble_uuid_create16(UUID_GATT_CLIENT_CHAR_CONFIGURATION, &uuid);
                ble_gatts_add_descriptor(&uuid, ATT_PERM_RW, sizeof(uint16_t), 0,
                        &(dbgs->cp_ccc_h[i - HICLING_NOTIFY1_POINT_UUID_16]));

        }

        for (int i = HICLING_WRITE1_POINT_UUID_16; i < HICLING_WRITE_END_POINT_UUID_16; i++) {
                ble_uuid_create16(i, &uuid);
                ble_gatts_add_characteristic(&uuid, GATT_PROP_WRITE,
                        ATT_PERM_WRITE, CONFIG_BLE_DLGDEBUG_MAX_CP_LEN, 0, NULL,
                        &dbgs->cp_val_h[i - HICLING_NOTIFY1_POINT_UUID_16]);

        }
        ble_gatts_register_service(&dbgs->svc.start_h, &dbgs->cp_val_h[0], &dbgs->cp_ccc_h[0],
                &dbgs->cp_val_h[1], &dbgs->cp_ccc_h[1],
                &dbgs->cp_val_h[2], &dbgs->cp_ccc_h[2],
                &dbgs->cp_val_h[3], &dbgs->cp_ccc_h[3],
                &dbgs->cp_val_h[4],
                &dbgs->cp_val_h[5],
                &dbgs->cp_val_h[6],
                &dbgs->cp_val_h[7], &cp_cpf_h, 0);

        dbgs->svc.end_h = dbgs->svc.start_h + num_attr;
#if 1
        /* Content Presentation Format for CP */
        put_u8_inc(&p, 25);      // Format=UTF-8 string
        put_u8_inc(&p, 0);      // Exponent=n/a
        put_u16_inc(&p, 0x2700);      // Unit=unitless
        put_u8_inc(&p, 1);      // namespace
        put_u16_inc(&p, 0);      // descriptor
        ble_gatts_set_value(cp_cpf_h, sizeof(cp_cpf_val), cp_cpf_val);
#endif

        return &dbgs->svc;
}

static void hanlde_disconnected_evt_t(ble_service_t *svc, const ble_evt_gap_disconnected_t *evt)
{
        if (ble_on_disconn_callback_p != NULL) {
                ble_on_disconn_callback_p();
        }
}

static void handle_conection_req(ble_service_t *svc, const ble_evt_gap_connected_t *evt)
{
        if (ble_on_conn_callback_p != NULL) {
                ble_on_conn_callback_p(svc, evt->conn_idx);
        }
        return;

}

static void handle_read_req(ble_service_t *svc, const ble_evt_gatts_read_req_t *evt)
{
        cling_service_t *dbgs = (cling_service_t *)svc;

        printf("[ble cust]: read callback request\r\n");
        fflush(stdout);
        for (int i = 0; i < sizeof(dbgs->cp_ccc_h) / sizeof(dbgs->cp_ccc_h[0]); i++) {
                if (evt->handle == dbgs->cp_ccc_h[i]) {
                        uint16_t ccc_val = 0;
                        ble_storage_get_u16(evt->conn_idx, dbgs->cp_ccc_h[i], &ccc_val);
                        // we're little-endian, ok to use value as-is
                        ble_gatts_read_cfm(evt->conn_idx, evt->handle, ATT_ERROR_OK,
                                sizeof(uint16_t), &ccc_val);

                }

        }

}

static void handle_write_req(ble_service_t *svc, const ble_evt_gatts_write_req_t *evt)
{
        cling_service_t *dbgs = (cling_service_t *)svc;
        att_error_t status = ATT_ERROR_WRITE_NOT_PERMITTED;
        N_SPRINTF("[ble cust]: handle write handle = %d number = %d\r\n", evt->handle, sizeof(dbgs->cp_ccc_h) / sizeof(dbgs->cp_ccc_h[0]));

        /*check for notification enable event*/
        for (int i = 0; i < sizeof(dbgs->cp_ccc_h) / sizeof(dbgs->cp_ccc_h[0]); i++) {
                N_SPRINTF("[ble cust]: current ccc hanle value = %d\r\n", dbgs->cp_ccc_h[i]);
                if (evt->handle == dbgs->cp_ccc_h[i]) {
                        uint16_t ccc_val = 0;
                        if (evt->offset) {
                                status = ATT_ERROR_ATTRIBUTE_NOT_LONG;
                                goto done;
                        }

                        if (evt->length != sizeof(uint16_t)) {
                                status = ATT_ERROR_APPLICATION_ERROR;
                                goto done;
                        }

                        ccc_val = get_u16(evt->value);

                        ble_storage_put_u32(evt->conn_idx, dbgs->cp_ccc_h[i], ccc_val, true);

                        status = ATT_ERROR_OK;

                        goto done;
                }

        }
        /*check for value write event*/
        for (int i = 0; i < sizeof(dbgs->cp_val_h) / sizeof(dbgs->cp_val_h[0]); i++) {
                N_SPRINTF("[ble cust]: current  cp_val_h value = %d\r\n", dbgs->cp_val_h[i]);
                if (evt->handle == dbgs->cp_val_h[i]) {
                        /*pass data to user space*/
                        if (ble_write_callback_p != NULL) {
                                ble_write_callback_p(svc, evt->conn_idx,
                                        uuid_check_table[evt->handle - dbgs->svc.start_h],
                                        (uint8_t*)evt->value, (size_t)evt->length);
                        }
                        N_SPRINTF("[ble cust]: handle write data = %d lenth %d\r\n",
                                evt->value[0], evt->length);
                }
        }
        done:
        ble_gatts_write_cfm(evt->conn_idx, evt->handle, status);
}

static void handle_prepare_write_req(ble_service_t *svc,
        const ble_evt_gatts_prepare_write_req_t *evt)
{
        cling_service_t *dbgs = (cling_service_t *)svc;
        N_SPRINTF("[ble cust]: handle_prepare_write_req\r\n");
#if 0
        if (evt->handle == dbgs->cp_val_h) {
                ble_gatts_prepare_write_cfm(evt->conn_idx, evt->handle,
                        CONFIG_BLE_DLGDEBUG_MAX_CP_LEN, ATT_ERROR_OK);
        }
        else {
                ble_gatts_prepare_write_cfm(evt->conn_idx, evt->handle, 0,
                        ATT_ERROR_REQUEST_NOT_SUPPORTED);
        }
#endif
}

ble_error_t cus_notify_send(ble_service_t *svc, uint16_t conn_idx, uint16_t uuid_num, uint8_t *t,
        size_t size)
{
        cling_service_t *dbgs = (cling_service_t *)svc;
        uint16_t ccc_val = 0;
        ble_storage_get_u16(conn_idx, dbgs->cp_ccc_h[uuid_num - HICLING_NOTIFY1_POINT_UUID_16],
                &ccc_val);

        if (!(ccc_val & GATT_CCC_NOTIFICATIONS)) {
                Y_SPRINTF("ccd not enabled\r\n");
                return BLE_ERROR_FAILED;
        }
        //value_len = pack_notify_value(meas, sizeof(value), value);
        ble_gatts_set_value(dbgs->cp_val_h[uuid_num - HICLING_NOTIFY1_POINT_UUID_16], size, t);
        return ble_gatts_send_event(conn_idx,
                dbgs->cp_val_h[uuid_num - HICLING_NOTIFY1_POINT_UUID_16],
                GATT_EVENT_NOTIFICATION, size, t);
}

void notify_cling_task_a_package_has_arrived()
{
        OS_TASK_NOTIFY(task_hanles.cling_handle, 0x02, OS_NOTIFY_SET_BITS);
}

#ifdef __cplusplus
}
#endif
