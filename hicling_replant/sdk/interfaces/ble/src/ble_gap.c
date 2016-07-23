/**
 ****************************************************************************************
 *
 * @file ble_gap.c
 *
 * @brief BLE GAP API implementation
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

#include <string.h>
#include "FreeRTOS.h"
#include "ble_common.h"
#include "osal.h"
#include "ble_gap.h"
#include "ble_irb_helper.h"
#include "ble_mgr.h"
#include "ble_mgr_irb_gap.h"
#include "storage.h"

ble_error_t ble_gap_address_get(own_address_t *address)
{
        ble_dev_params_t *params = ble_mgr_dev_params_acquire();

        address->addr_type = params->own_addr.addr_type;
        memcpy(address->addr, params->own_addr.addr, sizeof(address->addr));

        ble_mgr_dev_params_release();

        return BLE_STATUS_OK;
}

ble_error_t ble_gap_address_set(const own_address_t *address, uint16_t renew_dur)
{
        irb_ble_gap_address_set_cmd_t *cmd;
        irb_ble_gap_address_set_rsp_t *rsp;
        ble_error_t ret = BLE_ERROR_FAILED;

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_GAP_ADDRESS_SET_CMD, sizeof(*cmd));
        cmd->address = address;
        cmd->renew_dur = renew_dur;

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

ble_error_t ble_gap_device_name_set(const char *name, att_perm_t perm)
{
        irb_ble_gap_device_name_set_cmd_t *cmd;
        irb_ble_gap_device_name_set_rsp_t *rsp;
        ble_error_t ret = BLE_ERROR_FAILED;

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_GAP_DEVICE_NAME_SET_CMD, sizeof(*cmd));
        cmd->name = name;
        cmd->perm = perm;

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

ble_error_t ble_gap_device_name_get(char *name, uint8_t *length)
{
        ble_dev_params_t *params = ble_mgr_dev_params_acquire();

        // copy no more than buffer length and make sure it's always null-terminated
        strncpy(name, params->dev_name, *length);
        if (*length > 0) {
                name[*length - 1] = '\0';
        }

        // return proper length to app
        *length = strlen(params->dev_name);

        ble_mgr_dev_params_release();

        return BLE_STATUS_OK;
}

ble_error_t ble_gap_appearance_set(gap_appearance_t appearance, att_perm_t perm)
{
        irb_ble_gap_appearance_set_cmd_t *cmd;
        irb_ble_gap_appearance_set_rsp_t *rsp;
        ble_error_t ret = BLE_ERROR_FAILED;

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_GAP_APPEARANCE_SET_CMD, sizeof(*cmd));
        cmd->appearance = appearance;
        cmd->perm = perm;

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

ble_error_t ble_gap_appearance_get(gap_appearance_t *appearance)
{
        ble_dev_params_t *params = ble_mgr_dev_params_acquire();

        *appearance = params->appearance;

        ble_mgr_dev_params_release();

        return BLE_STATUS_OK;
}

ble_error_t ble_gap_per_pref_conn_params_set(const gap_conn_params_t *conn_params)
{
        irb_ble_gap_ppcp_set_cmd_t *cmd;
        irb_ble_gap_ppcp_set_rsp_t *rsp;
        ble_error_t ret = BLE_ERROR_FAILED;

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_GAP_PPCP_SET_CMD, sizeof(*cmd));
        cmd->gap_ppcp = conn_params;

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

ble_error_t ble_gap_per_pref_conn_params_get(gap_conn_params_t *conn_params)
{
        ble_dev_params_t *params = ble_mgr_dev_params_acquire();

        memcpy(conn_params, &params->gap_ppcp, sizeof(*conn_params));

        ble_mgr_dev_params_release();

        return BLE_STATUS_OK;
}

ble_error_t ble_gap_adv_start(gap_conn_mode_t adv_type)
{
        irb_ble_gap_adv_start_cmd_t *cmd;
        irb_ble_gap_adv_start_rsp_t *rsp;
        ble_error_t ret = BLE_ERROR_FAILED;

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_GAP_ADV_START_CMD, sizeof(*cmd));
        cmd->adv_type = adv_type;

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

ble_error_t ble_gap_adv_stop(void)
{
        irb_ble_gap_adv_stop_cmd_t *cmd;
        irb_ble_gap_adv_stop_rsp_t *rsp;
        ble_error_t ret = BLE_ERROR_FAILED;

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_GAP_ADV_STOP_CMD, sizeof(*cmd));

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

ble_error_t ble_gap_adv_data_set(uint8_t adv_data_len, const uint8_t *adv_data,
                             uint8_t scan_rsp_data_len, const uint8_t *scan_rsp_data)
{
        irb_ble_gap_adv_data_set_cmd_t *cmd;
        irb_ble_gap_adv_data_set_rsp_t *rsp;
        ble_error_t ret = BLE_ERROR_FAILED;

        if (adv_data_len > BLE_ADV_DATA_LEN_MAX || scan_rsp_data_len > BLE_SCAN_RSP_LEN_MAX) {
                return BLE_ERROR_INVALID_PARAM;
        }

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_GAP_ADV_DATA_SET_CMD, sizeof(*cmd));
        cmd->adv_data_len      = adv_data_len;
        cmd->adv_data          = adv_data;
        cmd->scan_rsp_data_len = scan_rsp_data_len;
        cmd->scan_rsp_data     = scan_rsp_data;

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

ble_error_t ble_gap_adv_intv_set(uint16_t adv_intv_min, uint16_t adv_intv_max)
{
        ble_dev_params_t *params = ble_mgr_dev_params_acquire();

        params->adv_intv_min = adv_intv_min;
        params->adv_intv_max = adv_intv_max;

        ble_mgr_dev_params_release();

        return BLE_STATUS_OK;
}

ble_error_t ble_gap_adv_chnl_map_set(uint8_t chnl_map)
{
        ble_dev_params_t *params = ble_mgr_dev_params_acquire();

        params->adv_channel_map = chnl_map;

        ble_mgr_dev_params_release();

        return BLE_STATUS_OK;
}

ble_error_t ble_gap_adv_mode_set(gap_disc_mode_t adv_mode)
{
        ble_dev_params_t *params = ble_mgr_dev_params_acquire();

        params->adv_mode = adv_mode;

        ble_mgr_dev_params_release();

        return BLE_STATUS_OK;
}

ble_error_t ble_gap_adv_filt_policy_set(adv_filt_pol_t filt_policy)
{
        ble_dev_params_t *params = ble_mgr_dev_params_acquire();

        params->adv_filter_policy = filt_policy;

        ble_mgr_dev_params_release();

        return BLE_STATUS_OK;
}

ble_error_t ble_gap_adv_direct_address_set(const bd_address_t *address)
{
        ble_dev_params_t *params = ble_mgr_dev_params_acquire();

        // Copy address used for directed advertising to BLE device parameters
        memcpy(&params->adv_direct_address, address, sizeof(*address));

        ble_mgr_dev_params_release();

        return BLE_STATUS_OK;
}

ble_error_t ble_gap_scan_start(gap_scan_type_t type, gap_scan_mode_t mode, uint16_t interval,
                           uint16_t window, bool filt_wlist, bool filt_dupl)
{
        irb_ble_gap_scan_start_cmd_t *cmd;
        irb_ble_gap_scan_start_rsp_t *rsp;
        ble_error_t ret = BLE_ERROR_FAILED;

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_GAP_SCAN_START_CMD, sizeof(*cmd));
        cmd->type       = type;
        cmd->mode       = mode;
        cmd->interval   = interval;
        cmd->window     = window;
        cmd->filt_wlist = filt_wlist;
        cmd->filt_dupl  = filt_dupl;

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

ble_error_t ble_gap_scan_stop(void)
{
        irb_ble_gap_scan_stop_cmd_t *cmd;
        irb_ble_gap_scan_stop_rsp_t *rsp;
        ble_error_t ret = BLE_ERROR_FAILED;

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_GAP_SCAN_STOP_CMD, sizeof(*cmd));

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

ble_error_t ble_gap_scan_params_get(gap_scan_params_t *scan_params)
{
        ble_dev_params_t *params = ble_mgr_dev_params_acquire();

        memcpy(scan_params, &params->scan_params, sizeof(*scan_params));

        ble_mgr_dev_params_release();

        return BLE_STATUS_OK;
}

ble_error_t ble_gap_scan_params_set(const gap_scan_params_t *scan_params)
{
        ble_dev_params_t *params = ble_mgr_dev_params_acquire();

        memcpy(&params->scan_params, scan_params, sizeof(params->scan_params));

        ble_mgr_dev_params_release();

        return BLE_STATUS_OK;
}

ble_error_t ble_gap_connect(const bd_address_t *peer_addr, const gap_conn_params_t *conn_params)
{
        irb_ble_gap_connect_cmd_t *cmd;
        irb_ble_gap_connect_rsp_t *rsp;
        ble_error_t ret = BLE_ERROR_FAILED;

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_GAP_CONNECT_CMD, sizeof(*cmd));
        cmd->peer_addr = peer_addr;
        cmd->conn_params = conn_params;

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

ble_error_t ble_gap_connect_cancel(void)
{
        irb_ble_gap_connect_cancel_cmd_t *cmd;
        irb_ble_gap_connect_cancel_rsp_t *rsp;
        ble_error_t ret = BLE_ERROR_FAILED;

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_GAP_CONNECT_CANCEL_CMD, sizeof(*cmd));

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

ble_error_t ble_gap_disconnect(uint16_t conn_idx, ble_hci_error_t reason)
{
        irb_ble_gap_disconnect_cmd_t *cmd;
        irb_ble_gap_disconnect_rsp_t *rsp;
        ble_error_t ret = BLE_ERROR_FAILED;

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_GAP_DISCONNECT_CMD, sizeof(*cmd));
        cmd->conn_idx = conn_idx;
        cmd->reason   = reason;

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

ble_error_t ble_gap_conn_rssi_get(uint16_t conn_idx, int8_t *conn_rssi)
{
        irb_ble_gap_conn_rssi_get_cmd_t *cmd;
        irb_ble_gap_conn_rssi_get_rsp_t *rsp;
        ble_error_t ret = BLE_ERROR_FAILED;

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_GAP_CONN_RSSI_GET_CMD, sizeof(*cmd));
        cmd->conn_idx = conn_idx;

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        *conn_rssi = rsp->conn_rssi;

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

ble_error_t ble_gap_role_set(const gap_role_t role)
{
        irb_ble_gap_role_set_cmd_t *cmd;
        irb_ble_gap_role_set_rsp_t *rsp;
        ble_error_t ret = BLE_ERROR_FAILED;

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_GAP_ROLE_SET_CMD, sizeof(*cmd));
        cmd->role = role;

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

ble_error_t ble_gap_mtu_size_get(uint16_t *mtu_size)
{
        ble_dev_params_t *params = ble_mgr_dev_params_acquire();

        *mtu_size = params->mtu_size;

        ble_mgr_dev_params_release();

        return BLE_STATUS_OK;
}

ble_error_t ble_gap_mtu_size_set(uint16_t mtu_size)
{
        irb_ble_gap_mtu_size_set_cmd_t *cmd;
        irb_ble_gap_mtu_size_set_rsp_t *rsp;
        ble_error_t ret = BLE_ERROR_FAILED;

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_GAP_MTU_SIZE_SET_CMD, sizeof(*cmd));
        cmd->mtu_size = mtu_size;

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

ble_error_t ble_gap_channel_map_get(uint64_t *chnl_map)
{
        ble_dev_params_t *params = ble_mgr_dev_params_acquire();

        // copy channel map bit mask
        memcpy(chnl_map, params->channel_map.map, 5);
        // clear unused/reserved bits
        *chnl_map &= 0x1FFFFFFFFF;

        ble_mgr_dev_params_release();

        return BLE_STATUS_OK;
}

ble_error_t ble_gap_channel_map_set(const uint64_t chnl_map)
{
        irb_ble_gap_channel_map_set_cmd_t *cmd;
        irb_ble_gap_channel_map_set_rsp_t *rsp;
        ble_error_t ret = BLE_ERROR_FAILED;

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_GAP_CHANNEL_MAP_SET_CMD, sizeof(*cmd));
        cmd->chnl_map = &chnl_map;

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

ble_error_t ble_gap_conn_param_update(uint16_t conn_idx, const gap_conn_params_t *conn_params)
{
        irb_ble_gap_conn_param_update_cmd_t *cmd;
        irb_ble_gap_conn_param_update_rsp_t *rsp;
        ble_error_t ret = BLE_ERROR_FAILED;

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_GAP_CONN_PARAM_UPDATE_CMD, sizeof(*cmd));
        cmd->conn_idx = conn_idx;
        cmd->conn_params = conn_params;

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

ble_error_t ble_gap_conn_param_update_reply(uint16_t conn_idx, bool accept)
{
        irb_ble_gap_conn_param_update_reply_cmd_t *cmd;
        irb_ble_gap_conn_param_update_reply_rsp_t *rsp;
        ble_error_t ret = BLE_ERROR_FAILED;

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_GAP_CONN_PARAM_UPDATE_REPLY_CMD, sizeof(*cmd));
        cmd->conn_idx = conn_idx;
        cmd->accept = accept;

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

ble_error_t ble_gap_pair(uint16_t conn_idx, bool bond)
{
        irb_ble_gap_pair_cmd_t *cmd;
        irb_ble_gap_pair_rsp_t *rsp;
        ble_error_t ret = BLE_ERROR_FAILED;

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_GAP_PAIR_CMD, sizeof(*cmd));
        cmd->conn_idx = conn_idx;
        cmd->bond = bond;

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

ble_error_t ble_gap_pair_reply(uint16_t conn_idx, bool accept, bool bond)
{
        irb_ble_gap_pair_reply_cmd_t *cmd;
        irb_ble_gap_pair_reply_rsp_t *rsp;
        ble_error_t ret = BLE_ERROR_FAILED;

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_GAP_PAIR_REPLY_CMD, sizeof(*cmd));
        cmd->conn_idx = conn_idx;
        cmd->accept = accept;
        cmd->bond = bond;

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

struct active_conn_idx_data {
        uint8_t  length;
        uint16_t *conn_idx;
};

static void connected_device_cb(const device_t *dev, void *ud)
{
        struct active_conn_idx_data *data = ud;

        if (dev->connected && !dev->resolving) {
                data->conn_idx[data->length++] = dev->conn_idx;
        }
}

ble_error_t ble_gap_get_connected(uint8_t *length, uint16_t **conn_idx)
{
        struct active_conn_idx_data data = {
                .length = 0,
                .conn_idx = OS_MALLOC(defaultBLE_MAX_CONNECTIONS * sizeof(**conn_idx)),
        };

        storage_acquire();
        device_foreach(connected_device_cb, &data);
        storage_release();

        *length = data.length;
        *conn_idx = data.conn_idx;

        return BLE_STATUS_OK;
}

struct active_addr_data {
        uint8_t         length;
        bd_address_t    *addr;
};

static void bonded_device_cb(const device_t *dev, void *ud)
{
        struct active_addr_data *data = ud;

        if (dev->bonded) {
                memcpy(&data->addr[data->length++], &dev->addr, sizeof(bd_address_t));
        }
}

ble_error_t ble_gap_get_bonded(uint8_t *length, bd_address_t **addr)
{
        struct active_addr_data data = {
                .length = 0,
                .addr = OS_MALLOC(defaultBLE_MAX_BONDED * sizeof(**addr)),
        };

        storage_acquire();
        device_foreach(bonded_device_cb, &data);
        storage_release();

        *length = data.length;
        *addr = data.addr;

        return BLE_STATUS_OK;
}

ble_error_t ble_gap_set_io_cap(gap_io_cap_t io_cap)
{
        ble_dev_params_t *ble_dev_params;

        ble_dev_params = ble_mgr_dev_params_acquire();
        ble_dev_params->io_capabilities = io_cap;
        ble_mgr_dev_params_release();

        return BLE_STATUS_OK;
}

ble_error_t ble_gap_passkey_reply(uint16_t conn_idx, bool accept, uint32_t passkey)
{
        irb_ble_gap_passkey_reply_cmd_t *cmd;
        irb_ble_gap_passkey_reply_rsp_t *rsp;
        ble_error_t ret = BLE_ERROR_FAILED;

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_GAP_PASSKEY_REPLY_CMD, sizeof(*cmd));
        cmd->conn_idx = conn_idx;
        cmd->accept = accept;
        cmd->passkey = passkey;

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

ble_error_t ble_gap_get_sec_level(uint16_t conn_idx, gap_sec_level_t *level)
{
        device_t *dev;

        storage_acquire();
        dev = find_device_by_conn_idx(conn_idx);
        if (!dev) {
                storage_release();
                return BLE_ERROR_FAILED;
        }

        *level = GAP_SEC_LEVEL_1;

        if (dev && dev->encrypted) {
                *level = GAP_SEC_LEVEL_2;

                if (dev->mitm) {
                        *level = GAP_SEC_LEVEL_3;
                }
        }

        storage_release();

        return BLE_STATUS_OK;
}

ble_error_t ble_gap_unpair(const bd_address_t *addr)
{
        irb_ble_gap_unpair_cmd_t *cmd;
        irb_ble_gap_unpair_rsp_t *rsp;
        ble_error_t ret;

        /* setup IRB with new message and fill it */
        cmd = alloc_ble_msg(IRB_BLE_GAP_UNPAIR_CMD, sizeof(*cmd));
        memcpy(&cmd->addr, addr, sizeof(bd_address_t));

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

ble_error_t ble_gap_set_sec_level(uint16_t conn_idx, gap_sec_level_t level)
{
        irb_ble_gap_set_sec_level_cmd_t *cmd;
        irb_ble_gap_set_sec_level_rsp_t *rsp;
        gap_sec_level_t current_level;
        ble_error_t ret;

        if (ble_gap_get_sec_level(conn_idx, &current_level) != BLE_STATUS_OK) {
                return BLE_ERROR_FAILED;
        }

        if (current_level >= level) {
                return BLE_ERROR_ALREADY_DONE;
        }

        cmd = alloc_ble_msg(IRB_BLE_GAP_SET_SEC_LEVEL_CMD, sizeof(*cmd));
        cmd->conn_idx = conn_idx;
        cmd->level = level;

        if (!irb_execute(cmd, (void **) &rsp)) {
                return BLE_ERROR_FAILED;
        }

        ret = rsp->status;
        OS_FREE(rsp);

        return ret;
}

static void copy_dev_to_gap_dev(const device_t *dev, gap_device_t *gap_dev)
{
        gap_dev->address = dev->addr;
        gap_dev->conn_idx = dev->connected ? dev->conn_idx : BLE_CONN_IDX_INVALID;
        gap_dev->connected = dev->connected;
        gap_dev->bonded = dev->bonded;
}

struct filter_cb_data {
        uint16_t length;
        uint16_t max_length;
        gap_device_t *gap_devices;
};

static inline void filter_do_copy(const device_t *dev, struct filter_cb_data *fd)
{
        /*
         * if output buffer is not specified, we only return number of devices matching requested
         * filter (initial length values does not matter)
         */
        if (!fd->gap_devices) {
                fd->length++;
                return;
        }

        if (fd->length >= fd->max_length) {
                return;
        }

        copy_dev_to_gap_dev(dev, &fd->gap_devices[fd->length++]);
}

static void filter_copy_all_cb(const device_t *dev, void *ud)
{
        struct filter_cb_data *fd = ud;

        filter_do_copy(dev, fd);
}

static void filter_copy_bonded_cb(const device_t *dev, void *ud)
{
        struct filter_cb_data *fd = ud;

        if (!dev->bonded) {
                return;
        }

        filter_do_copy(dev, fd);
}

static void filter_copy_connected_cb(const device_t *dev, void *ud)
{
        struct filter_cb_data *fd = ud;

        if (!dev->connected || dev->resolving) {
                return;
        }

        filter_do_copy(dev, fd);
}

ble_error_t ble_gap_get_devices(gap_device_filter_t filter, gap_device_filter_data_t *filter_data,
                                                        size_t *length, gap_device_t *gap_devices)
{
        struct filter_cb_data fd;
        device_t *dev;

        if (!length || (*length == 0 && gap_devices)) {
                return BLE_ERROR_INVALID_PARAM;
        }

        fd.length = 0;
        fd.max_length = *length;
        fd.gap_devices = gap_devices;

        storage_acquire();

        switch (filter) {
        case GAP_DEVICE_FILTER_ALL:
                device_foreach(filter_copy_all_cb, &fd);
                break;
        case GAP_DEVICE_FILTER_CONNECTED:
                device_foreach(filter_copy_connected_cb, &fd);
                break;
        case GAP_DEVICE_FILTER_BONDED:
                device_foreach(filter_copy_bonded_cb, &fd);
                break;
        case GAP_DEVICE_FILTER_ADDRESS:
                dev = find_device_by_addr(&filter_data->address, false);
                if (dev) {
                        filter_do_copy(dev, &fd);
                }
                break;
        case GAP_DEVICE_FILTER_CONN_IDX:
                dev = find_device_by_conn_idx(filter_data->conn_idx);
                if (dev) {
                        filter_do_copy(dev, &fd);
                }
                break;
        default:
                storage_release();
                return BLE_ERROR_INVALID_PARAM;
        }

        storage_release();

        *length = fd.length;

        return BLE_STATUS_OK;
}

ble_error_t ble_gap_is_bonded(uint16_t conn_idx, bool *bonded)
{
        device_t *dev;

        storage_acquire();
        dev = find_device_by_conn_idx(conn_idx);
        if (!dev) {
                *bonded = false;
                storage_release();
                return BLE_ERROR_NOT_CONNECTED;
        }

        *bonded = dev->bonded;
        storage_release();

        return BLE_STATUS_OK;
}