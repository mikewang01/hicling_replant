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
 * @file ble_mgr_irb_l2cap.h
 *
 * @brief BLE IRB definitions (for L2CAP)
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

#ifndef BLE_MGR_IRB_L2CAP_H_
#define BLE_MGR_IRB_L2CAP_H_

#include <stdint.h>
#include "osal.h"
#include "ble_mgr_irb.h"
#include "ble_l2cap.h"

/** OP codes for L2CAP IRBs */
enum irb_ble_opcode_l2cap {
        IRB_BLE_L2CAP_CONN_PARAM_UPDATE_CMD  = IRB_BLE_CAT_EVENT0(IRB_BLE_CAT_L2CAP),
        // dummy event ID, needs to be always defined after all command IRBs
        IRB_BLE_LAST_L2CAP,
};

/** L2CAP connection parameter update command message */
typedef struct {
        irb_ble_hdr_t            hdr;
        uint16_t                 conn_idx;
        const gap_conn_params_t  *conn_params;
} irb_ble_l2cap_conn_param_update_cmd_t;

/** L2CAP connection parameter update response message */
typedef struct {
        irb_ble_hdr_t   hdr;
        ble_error_t     status;
} irb_ble_l2cap_conn_param_update_rsp_t;

void irb_ble_handler_l2cap_conn_param_update_cmd(OS_IRB *irb);

#endif /* BLE_MGR_IRB_L2CAP_H_ */

/**
 \}
 \}
 \}
 */
