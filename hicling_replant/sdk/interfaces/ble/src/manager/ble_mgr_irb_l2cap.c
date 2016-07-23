/**
 ****************************************************************************************
 *
 * @file ble_mgr_irb_l2cap.c
 *
 * @brief BLE IRB handlers (for L2CAP)
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

#include <stdlib.h>
#include "ble_mgr.h"
#include "ble_mgr_gtl.h"
#include "ble_mgr_irb.h"
#include "ble_mgr_irb_l2cap.h"
#include "ble_irb_helper.h"
#include "ble_common.h"
#include "ble_l2cap.h"
#include "ble_storage.h"
#include "storage.h"

#include "l2cc_task.h"
#include "l2cc_pdu.h"

void irb_ble_handler_l2cap_conn_param_update_cmd(OS_IRB *irb)
{
        const irb_ble_l2cap_conn_param_update_cmd_t *cmd =
                (irb_ble_l2cap_conn_param_update_cmd_t *) irb->ptr_buf;
        irb_ble_l2cap_conn_param_update_rsp_t *rsp;
        static irb_ble_stack_msg_t *gmsg;
        struct l2cc_pdu_send_req *gcmd;
        ble_error_t ret = BLE_ERROR_FAILED;
        device_t *dev;

        storage_acquire();
        dev = find_device_by_conn_idx(cmd->conn_idx);
        storage_release();

        if (!dev) {
                /* no active connection corresponds to provided index */
                ret = BLE_ERROR_NOT_CONNECTED;
                goto done;
        }

        gmsg = ble_gtl_alloc_with_conn(L2CC_PDU_SEND_REQ, TASK_ID_L2CC, dev->conn_idx,
                                                                                     sizeof(*gcmd));
        gcmd = (struct l2cc_pdu_send_req *) gmsg->msg.gtl.param;

        // setup GTL message
        gcmd->pdu.chan_id = L2C_CID_LE_SIGNALING;
        gcmd->pdu.data.update_req.code     = L2C_CODE_CONN_PARAM_UPD_REQ;
        gcmd->pdu.data.update_req.intv_max = cmd->conn_params->interval_max;
        gcmd->pdu.data.update_req.intv_min = cmd->conn_params->interval_min;
        gcmd->pdu.data.update_req.latency  = cmd->conn_params->slave_latency;
        gcmd->pdu.data.update_req.timeout  = cmd->conn_params->sup_timeout;
        // generate packet identifier
        gcmd->pdu.data.update_req.pkt_id = ((uint32_t) rand()) % 256;
        if(gcmd->pdu.data.update_req.pkt_id == 0)
        {
                gcmd->pdu.data.update_req.pkt_id = 1;
        }

        ble_gtl_send(gmsg);
        ret = BLE_STATUS_OK;
done:
        irb_ble_mark_completed(irb, true);
        rsp = irb_ble_replace_msg(irb, IRB_BLE_L2CAP_CONN_PARAM_UPDATE_CMD, sizeof(*rsp));
        rsp->status = ret;
        ble_mgr_response_queue_send(irb, OS_QUEUE_FOREVER);
}
