/**
 \addtogroup INTERFACES
 \{
 \addtogroup BLE
 \{
 \addtogroup API
 \{
 */

/**
 ****************************************************************************************
 *
 * @file ble_l2cap.h
 *
 * @brief BLE L2CAP API
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

#ifndef BLE_L2CAP_H_
#define BLE_L2CAP_H_

#include <stdint.h>
#include "ble_common.h"
#include "ble_gap.h"

/**
 * \brief Initiate a connection parameter update over L2CAP
 *
 * This call can be used only by the slave of a connection to initiate a connection parameter
 * request to the master over L2CAP, instead of using ble_gap_conn_param_update() which triggers the
 * connection parameter request procedure. Successful connection parameters update will result in a
 * ::BLE_EVT_GAP_CONN_PARAM_UPDATED event message. Unsuccessful connection parameters update
 * (connection parameters rejected by the master) will not result in any event message to the
 * application.
 *
 * \param [in]  conn_idx       Connection index
 * \param [in]  conn_params    Pointer to the connection parameters
 *
 * \return result code
 */
ble_error_t ble_l2cap_conn_param_update(uint16_t conn_idx, const gap_conn_params_t *conn_params);

#endif /* BLE_L2CAP_H_ */

/**
 \}
 \}
 \}
 */
