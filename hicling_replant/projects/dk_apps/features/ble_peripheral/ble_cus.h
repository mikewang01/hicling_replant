/**
 ****************************************************************************************
 *
 * @file ble_cus.h
 *
 * @brief hicling service implementation API
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

#ifndef BAS_H_
#define BAS_H_

#include <stdint.h>
#include "ble_service.h"

/**
 * Register cling Service instance
 *
 *
 * \param [in] config   service configuration
 *
 * \return service instance
 *
 */
ble_service_t *cling_service_init(const ble_service_config_t *cfg);

/**
 * Register cling Service write call back function
 *
 *
 * \param [in] p: callback funtion pointer passed from user
 *
 * \return ssucess failed
 *
 */
int cling_service_reg_write_callback(int (*p)(uint16_t, uint8_t*, size_t));

#endif /* BAS_H_ */
