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
/**************************************************************************************************/
static void handle_read_req(ble_service_t *svc, const ble_evt_gatts_read_req_t *evt);
static void handle_write_req(ble_service_t *svc, const ble_evt_gatts_write_req_t *evt);
static void handle_prepare_write_req(ble_service_t *svc,
        const ble_evt_gatts_prepare_write_req_t *evt);
static int (*ble_write_callback_p)(uint16_t uuid, uint8_t *s, size_t size) = NULL;

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

enum {
        HICLING_SERVICE_UUID_16 = 0XFEE7,
        HICLING_NOTIFY1_POINT_UUID_16 = 0xFEE1,
        HICLING_NOTIFY2_POINT_UUID_16,
        HICLING_NOTIFY3_POINT_UUID_16,
        HICLING_NOTIFY4_POINT_UUID_16,
        HICLING_NOTIFY_END_POINT_UUID_16,

        HICLING_WRITE1_POINT_UUID_16 = 0xFEE5,
        HICLING_WRITE2_POINT_UUID_16,
        HICLING_WRITE3_POINT_UUID_16,
        HICLING_WRITE4_POINT_UUID_16,
        HICLING_WRITE_END_POINT_UUID_16,

};
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
typedef struct {
        ble_service_t svc;

        // handles
        uint16_t cp_val_h[HICLING_WRITE_END_POINT_UUID_16 - HICLING_NOTIFY1_POINT_UUID_16];
        uint16_t cp_ccc_h[HICLING_NOTIFY_END_POINT_UUID_16 - HICLING_NOTIFY1_POINT_UUID_16];

        void *handlers;
} cling_service_t;
CLING_MAIN_CTX cling;

int cling_service_reg_write_callback(int (*p)(uint16_t, uint8_t*, size_t))
{
        if (p != NULL) {
                ble_write_callback_p = p;
        }
        return BLE_STATUS_OK;

}
void btle_init(void);
void LINK_state_machine(void);
void _nor_flash_spi_test();
ble_service_t *cling_service_init(const ble_service_config_t *cfg)
{
//        LINK_state_machine();
        cling_service_t *dbgs;
        uint16_t num_attr;
        att_uuid_t uuid;
        uint16_t cp_cpf_h;
        uint8_t cp_cpf_val[7];
        uint8_t *p = cp_cpf_val;
        _nor_flash_spi_test();
        dbgs = OS_MALLOC(sizeof(*dbgs));
        memset(dbgs, 0, sizeof(*dbgs));

        dbgs->svc.read_req = handle_read_req;
        dbgs->svc.write_req = handle_write_req;
        dbgs->svc.prepare_write_req = handle_prepare_write_req;

        printf("[ble cust]: add hicling service\r\n");
        fflush(stdout);

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
        btle_init();
        return &dbgs->svc;
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
        printf("[ble cust]: handle write handle = %d number = %d\r\n", evt->handle,
                sizeof(dbgs->cp_ccc_h) / sizeof(dbgs->cp_ccc_h[0]));

        /*check for notification enable event*/
        for (int i = 0; i < sizeof(dbgs->cp_ccc_h) / sizeof(dbgs->cp_ccc_h[0]); i++) {
                printf("[ble cust]: current ccc hanle value = %d\r\n", dbgs->cp_ccc_h[i]);
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
                printf("[ble cust]: current  cp_val_h value = %d\r\n", dbgs->cp_val_h[i]);
                if (evt->handle == dbgs->cp_val_h[i]) {
                        //status = handle_cp_val_write(dbgs, evt);
                        /*pass data to user space*/
                        if (ble_write_callback_p != NULL) {
                                ble_write_callback_p(
                                        uuid_check_table[evt->handle - dbgs->svc.start_h],
                                        (uint8_t*)evt->value, (size_t)evt->length);
                        }
                        printf("[ble cust]: handle write data = %d %d start handle = %d\r\n",
                                evt->value[0], evt->length, dbgs->svc.start_h);
                }
        }
        done:
        fflush(stdout);
        ble_gatts_write_cfm(evt->conn_idx, evt->handle, status);
}

static void handle_prepare_write_req(ble_service_t *svc,
        const ble_evt_gatts_prepare_write_req_t *evt)
{
        cling_service_t *dbgs = (cling_service_t *)svc;
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

