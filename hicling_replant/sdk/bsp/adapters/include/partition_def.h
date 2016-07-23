/**
 *****************************************************************************************
 *
 * @file partition_def.h
 *
 * @brief Partition table entry definition
 *
 * Copyright (C) 2016. Dialog Semiconductor Ltd, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor Ltd. All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 * <black.orca.support@diasemi.com> and contributors.
 *
 *****************************************************************************************
 */

#ifndef PARTITION_DEF_H_
#define PARTITION_DEF_H_

/**
 * \brief NVMS Partition IDs
 */
typedef enum {
        NVMS_FIRMWARE_PART              = 1,
        NVMS_PARAM_PART                 = 2,
        NVMS_BIN_PART                   = 3,
        NVMS_LOG_PART                   = 4,
        NVMS_GENERIC_PART               = 5,
        NVMS_PLATFORM_PARAMS_PART       = 15,
        NVMS_PARTITION_TABLE            = 16,
        NVMS_FW_EXEC_PART               = 17,
        NVMS_FW_UPDATE_PART             = 18,
        NVMS_PRODUCT_HEADER_PART        = 19,
        NVMS_IMAGE_HEADER_PART          = 20,
} nvms_partition_id_t;

/**
 * \brief Partition entry.
 */
typedef struct partition_entry_t {
        uint8_t magic;          /**< Partition magic number 0xEA */
        uint8_t type;           /**< Partition ID */
        uint8_t valid;          /**< Valid marker 0xFF */
        uint8_t flags;          /**< */
        uint16_t start_sector;  /**< Partition start sector */
        uint16_t sector_count;  /**< Number of sectors in partition */
        uint8_t reserved2[8];   /**< Reserved for future use */
} partition_entry_t;

#define PARTITION_FLAG_READ_ONLY        1
#define PARTITION_FLAG_VES              2

#endif /* PARTITION_DEF_H_ */
