/**
 * \addtogroup BSP
 * \{
 * \addtogroup SYSTEM
 * \{
 * \addtogroup SUOTA
 * 
 * \brief SUOTA structure definitions
 *
 * \{
 */

/**
 *****************************************************************************************
 *
 * @file suota.h
 *
 * @brief SUOTA structure definitions
 *
 * Copyright (C) 2015. Dialog Semiconductor Ltd, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor Ltd. All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 * <black.orca.support@diasemi.com> and contributors.
 *
 *****************************************************************************************
 */

#ifndef SUOTA_H_
#define SUOTA_H_

#include <stdint.h>

#define SUOTA_VERSION_1_0       10
#define SUOTA_VERSION_1_1       11

#ifndef SUOTA_VERSION
#define SUOTA_VERSION           SUOTA_VERSION_1_1
#endif

/**
 * \struct suota_1_1_product_header_t;
 *
 * \brief SUOTA 1.1 product header as defined by Dialog SUOTA specification.
 *
 */
typedef struct {
        uint8_t signature[2];
        uint16_t flags;
        uint32_t current_image_location;
        uint32_t update_image_location;
        uint8_t reserved[8];
} suota_1_1_product_header_t;

#define SUOTA_1_1_PRODUCT_HEADER_SIGNATURE_B1   0x70
#define SUOTA_1_1_PRODUCT_HEADER_SIGNATURE_B2   0x62

#define SUOTA_1_0_PRODUCT_HEADER_SIGNATURE_B1   0x70
#define SUOTA_1_0_PRODUCT_HEADER_SIGNATURE_B2   0x52

/**
 * \struct suota_1_0_image_header_t
 *
 * \brief SUOTA 1.0 image header as defined by Dialog SUOTA specification.
 *
 */
typedef struct {
        uint8_t signature[2];
        uint8_t valid_flag;
        uint8_t image_id;
        uint32_t code_size;
        uint32_t crc;
        uint8_t version[16];
        uint32_t timestamp;
        uint8_t encryption;
        uint8_t reserved[31];
} suota_1_0_image_header_t;

/**
 * \struct suota_1_1_image_header_t
 *
 * \brief SUOTA 1.1 image header as defined by Dialog SUOTA specification.
 *
 */
typedef struct {
        uint8_t signature[2];
        uint16_t flags;
        uint32_t code_size;
        uint32_t crc;
        uint8_t version[16];
        uint32_t timestamp;
        uint32_t exec_location;
} suota_1_1_image_header_t;

#define SUOTA_1_1_IMAGE_HEADER_SIGNATURE_B1     0x70
#define SUOTA_1_1_IMAGE_HEADER_SIGNATURE_B2     0x61

#define SUOTA_1_0_IMAGE_HEADER_SIGNATURE_B1     0x70
#define SUOTA_1_0_IMAGE_HEADER_SIGNATURE_B2     0x51

#define SUOTA_1_1_IMAGE_FLAG_FORCE_CRC          0x01
#define SUOTA_1_1_IMAGE_FLAG_VALID              0x02
#define SUOTA_1_1_IMAGE_FLAG_RETRY1             0x04
#define SUOTA_1_1_IMAGE_FLAG_RETRY2             0x08

#if (SUOTA_VERSION == SUOTA_VERSION_1_0)
typedef suota_1_0_image_header_t suota_image_header_t;
#else
typedef suota_1_1_image_header_t suota_image_header_t;
#endif

#endif /* SUOTA_H_ */

/**
 * \}
 * \}
 * \}
 */