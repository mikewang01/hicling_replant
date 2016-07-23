/**
 ****************************************************************************************
 *
 * @file ble_l2cap.c
 *
 * @brief BLE L2CAP API implementation
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

#include "FreeRTOS.h"
#include "ble_common.h"
#include "osal.h"
#include "ble_gap.h"
#include "ble_irb_helper.h"
#include "ble_l2cap.h"
#include "ble_mgr.h"
#include "ble_mgr_irb_l2cap.h"
#include "storage.h"

ble_error_t ble_l2cap_conn_param_update(uint16_t conn_idx, const gap_conn_params_t *conn_params)
{
        irb_ble_l2cap_conn_param_update_cmd_t *cmd;
        irb_ble_l2cap_conn_param_update_rsp_t *rsp;
        ble_error_t ret = BLE_ERROR_FAILED;

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_L2CAP_CONN_PARAM_UPDATE_CMD, sizeof(*cmd));
        cmd->conn_idx = conn_idx;
        cmd->conn_params = conn_params;

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}
