/**
 ****************************************************************************************
 *
 * @file dlg_suota.h
 *
 * @brief Dialog SUOTA service implementation API
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

#ifndef DLG_SUOTA_H_
#define DLG_SUOTA_H_

#include <stdint.h>
#include "ble_service.h"

/**
 * SUOTA active image enum
 */
typedef enum {
        SUOTA_ACTIVE_IMG_FIRST,
        SUOTA_ACTIVE_IMG_SECOND,
        SUOTA_ACTIVE_IMG_ERROR,
} suota_active_img_t;

/**
 * SUOTA application status
 *
 */
typedef enum {
        SUOTA_START,
        SUOTA_ONGOING,
        SUOTA_DONE,
        SUOTA_ERROR,
} suota_app_status_t;

typedef void (* suota_status_cb_t) (uint8_t status, uint8_t error_code);
typedef bool (* suota_ready_cb_t ) (void);

/**
 * SUOTA application callbacks
 */
typedef struct {
        suota_ready_cb_t suota_ready;
        suota_status_cb_t suota_status;
} suota_callbacks_t;

/**
 * Register SUOTA Service instance
 *
 * \return service instance
 *
 */
ble_service_t *suota_init(const suota_callbacks_t *cb);

/**
 * Get SUOTA active image
 *
 * \return SUOTA active image
 *
 */
suota_active_img_t suota_get_active_img(ble_service_t *svc);

/**
 * Update CRC
 *
 * param [in] crc current value of CRC
 * param [in] data pointer to data to compute CRC over
 * param [in] len number of bytes pointed by data
 *
 * \return new value of CRC
 *
 */
uint32_t suota_update_crc(uint32_t crc, const uint8_t *data, size_t len);

#endif /* DLG_SUOTA_H_ */
