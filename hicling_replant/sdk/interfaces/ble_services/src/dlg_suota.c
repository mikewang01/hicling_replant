/**
 ****************************************************************************************
 *
 * @file dlg_suota.c
 *
 * @brief Dialog SUOTA service implementation
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

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "osal.h"
#include "ble_att.h"
#include "ble_bufops.h"
#include "ble_common.h"
#include "ble_gatt.h"
#include "ble_gatts.h"
#include "ble_storage.h"
#include "ble_uuid.h"
#include "svc_defines.h"
#include "dlg_suota.h"
#include "suota.h"
#include "ad_nvms.h"
#include "hw_watchdog.h"
#include "hw_cpm.h"

#define UUID_SUOTA_SERVICE      (0xFEF5)
#define UUID_SUOTA_MEM_DEV      "8082CAA8-41A6-4021-91C6-56F9B954CC34"
#define UUID_SUOTA_GPIO_MAP     "724249F0-5EC3-4B5F-8804-42345AF08651"
#define UUID_SUOTA_MEM_INFO     "6C53DB25-47A1-45FE-A022-7C92FB334FD4"
#define UUID_SUOTA_PATCH_LEN    "9D84B9A3-000C-49D8-9183-855B673FDA31"
#define UUID_SUOTA_PATCH_DATA   "457871E8-D516-4CA1-9116-57D0B17B9CB2"
#define UUID_SUOTA_STATUS       "5F78DF94-798C-46F5-990A-B3EB6A065C88"

#define NUM_OF_CHAR             (6)
#define NUM_OF_DESC             (1)

#define SUOTA_PD_CHAR_SIZE      (120)
#define ACTIVE_IMG_PTR          (0x3000)
#define IMG_ADDR_BASE           (0x8000000)
#define FIRST_IMG_ADDR          (0x4000)
#define SECOND_IMG_ADDR         (0x44000)
#define FLASH_SECTOR_SIZE       (4096)
#define SUOTA_MAX_IMAGE_SIZE    (236 * 1024)

typedef enum {
        SUOTA_CHECK_SIGNATURE,
        SUOTA_MAIN_HEADER,
        SUOTA_IMAGE_HEADER,
        SUOTA_IMAGE_DATA,
        SUOTA_IMAGE_SKIP,
} suota_state_t;

#define SUOTA_CMD_MASK          0xFF000000
#define SUOTA_IMAGE_BANK_MASK   0x0000FFFF
#define SUOTA_BUFFER_SIZE       (512)

/**
 * SUOTA status
 *
 * As defined by Dialog SUOTA specification.
 *
 */
typedef enum {
        /* Value zero must not be used !! Notifications are sent when status changes. */
        SUOTA_SRV_STARTED      = 0x01,     // Valid memory device has been configured by initiator. No sleep state while in this mode
        SUOTA_CMP_OK           = 0x02,     // SUOTA process completed successfully.
        SUOTA_SRV_EXIT         = 0x03,     // Forced exit of SUOTA service.
        SUOTA_CRC_ERR          = 0x04,     // Overall Patch Data CRC failed
        SUOTA_PATCH_LEN_ERR    = 0x05,     // Received patch Length not equal to PATCH_LEN characteristic value
        SUOTA_EXT_MEM_WRITE_ERR= 0x06,     // External Mem Error (Writing to external device failed)
        SUOTA_INT_MEM_ERR      = 0x07,     // Internal Mem Error (not enough space for Patch)
        SUOTA_INVAL_MEM_TYPE   = 0x08,     // Invalid memory device
        SUOTA_APP_ERROR        = 0x09,     // Application error

        /* SUOTA application specific error codes */
        SUOTA_IMG_STARTED      = 0x10,     // SUOTA started for downloading image (SUOTA application)
        SUOTA_INVAL_IMG_BANK   = 0x11,     // Invalid image bank
        SUOTA_INVAL_IMG_HDR    = 0x12,     // Invalid image header
        SUOTA_INVAL_IMG_SIZE   = 0x13,     // Invalid image size
        SUOTA_INVAL_PRODUCT_HDR= 0x14,     // Invalid product header
        SUOTA_SAME_IMG_ERR     = 0x15,     // Same Image Error
        SUOTA_EXT_MEM_READ_ERR = 0x16,     // Failed to read from external memory device

        /* SUOTA extended status for Apple HomeKit */
        SUOTA_LEGACY_MODE      = 0x18,
        SUOTA_HAP_MODE         = 0x19,
        SUOTA_SIGNED_MODE      = 0x1A,
        SUOTA_ENC_SIGNED_MODE  = 0x1B,
} suota_status_t;

/**
 * SUOTA commands
 *
 * As defined by Dialog SUOTA specification.
 *
 */
typedef enum {
        /* SUOTA is used to send entire image */
        SPOTAR_IMG_INT_SYSRAM = 0x10,
        SPOTAR_IMG_INT_RETRAM = 0x11,
        SPOTAR_IMG_I2C_EEPROM = 0x12,
        SPOTAR_IMG_SPI_FLASH  = 0x13,

        /* DO NOT move. Must be before commands */
        SPOTAR_MEM_INVAL_DEV  = 0x14,

        /* SUOTA commands */
        SPOTAR_REBOOT         = 0xFD,
        SPOTAR_IMG_END        = 0xFE,

        /*
         * When initiator selects 0xff, it wants to exit SUOTA service.
         * This is used in case of unexplained failures. If SUOTA process
         * finishes correctly it will exit automatically.
         */
        SPOTAR_MEM_SERVICE_EXIT   = 0xFF,
} suota_commands_t;

/**
 * SUOTA product header
 *
 * As defined by Dialog SUOTA specification.
 *
 */
typedef struct __attribute__ ((packed)) {
        uint8_t signature[2];
        uint8_t version[2];
        uint32_t offset_1;
        uint32_t offset_2;
} suota_product_header_t;

typedef struct {
        ble_service_t svc;

        const suota_callbacks_t *cb;

        uint8_t chunk_len;
        uint32_t received_data_len;
        uint8_t *data_buffer;
        uint16_t patch_len;
        uint16_t buffer_pos;
        #if (SUOTA_VERSION == SUOTA_VERSION_1_0)
        uint32_t image_size;
        #endif
        uint32_t written_img_size;
        #if (SUOTA_VERSION == SUOTA_VERSION_1_0)
        uint32_t image_offset;
        #endif
        uint32_t total_image_size;
        uint8_t current_write_sector;
        suota_state_t state;
        uint32_t addr;
        uint8_t header_fragment;
        uint32_t image_crc;
        uint32_t crc;
        uint16_t conn_idx;

        #if (SUOTA_VERSION == SUOTA_VERSION_1_1)
        suota_image_header_t header;
        #endif

        // handles
        uint16_t suota_mem_dev_val_h;
        uint16_t suota_gpio_map_val_h;
        uint16_t suota_mem_info_val_h;
        uint16_t suota_patch_len_val_h;
        uint16_t suota_patch_data_val_h;
        uint16_t suota_status_val_h;
        uint16_t suota_status_ccc_h;

        suota_active_img_t active_img;

        nvms_t  nvms;
} suota_service_t;

static const uint32_t crc32_tab[] = {
        0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
        0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
        0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
        0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
        0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
        0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
        0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
        0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
        0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
        0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
        0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
        0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
        0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
        0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
        0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
        0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
        0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
        0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
        0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
        0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
        0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
        0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
        0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
        0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
        0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
        0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
        0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
        0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
        0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
        0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
        0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
        0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
        0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
        0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
        0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
        0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
        0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
        0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
        0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
        0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
        0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
        0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
        0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

uint32_t suota_update_crc(uint32_t crc, const uint8_t *data, size_t len)
{
        while (len--) {
                crc = crc32_tab[(crc ^ *data++) & 0xff] ^ (crc >> 8);
        }
        return crc;
}

#if (SUOTA_VERSION == SUOTA_VERSION_1_0)
static uint32_t get_update_addr(suota_service_t *suota)
{
        switch (suota->active_img) {
        case SUOTA_ACTIVE_IMG_FIRST:
                return SECOND_IMG_ADDR;
        case SUOTA_ACTIVE_IMG_SECOND:
                return FIRST_IMG_ADDR;
        default:
                return FIRST_IMG_ADDR;
        }
}
#else
static uint32_t get_update_addr(suota_service_t *suota)
{
        return 0;
}
#endif

static bool ready_for_suota(suota_service_t *suota)
{
        if (suota->cb && suota->cb->suota_ready) {
                return suota->cb->suota_ready();
        }

        return true;
}

static void notify_suota_app_status(suota_service_t *suota, uint8_t status, uint8_t error_code)
{
        if (suota->cb && suota->cb->suota_status) {
                suota->cb->suota_status(status, error_code);
        }
}

static void suota_notify_status(suota_service_t *suota, uint16_t conn_idx, uint8_t status)
{
        uint16_t ccc = 0x0000;

        ble_storage_get_u16(conn_idx, suota->suota_status_ccc_h, &ccc);

        if (!(ccc & GATT_CCC_NOTIFICATIONS)) {
                return;
        }

        ble_gatts_set_value(suota->suota_status_val_h, sizeof(status), &status);
        ble_gatts_send_event(conn_idx, suota->suota_status_val_h, GATT_EVENT_NOTIFICATION,
                                sizeof(status), &status);
}

static att_error_t do_bl_ccc_write(suota_service_t *suota, uint16_t conn_idx, uint16_t offset,
                                                                uint16_t length, const uint8_t *value)
{
        uint16_t ccc;

        if (offset) {
                return ATT_ERROR_ATTRIBUTE_NOT_LONG;
        }

        if (length != sizeof(ccc)) {
                return ATT_ERROR_APPLICATION_ERROR;
        }

        ccc = get_u16(value);

        ble_storage_put_u32(conn_idx, suota->suota_status_ccc_h, ccc, true);

        return ATT_ERROR_OK;
}

#if (SUOTA_VERSION == SUOTA_VERSION_1_0)
static bool set_active_img_ptr(suota_service_t *suota)
{
        int written;
        uint32_t addr = get_update_addr(suota) | IMG_ADDR_BASE;

        ad_nvms_erase_region(suota->nvms, ACTIVE_IMG_PTR, FLASH_SECTOR_SIZE);
        written = ad_nvms_write(suota->nvms, ACTIVE_IMG_PTR, (uint8_t *) &addr, 4);

        return written == 4;
}
#else
static bool set_active_img_ptr(suota_service_t *suota)
{
        int written;

        suota->header.flags |= SUOTA_1_1_IMAGE_FLAG_VALID;
        written = ad_nvms_write(suota->nvms, get_update_addr(suota), (uint8_t *) &suota->header,
                                                                sizeof(suota_image_header_t));
        return written == sizeof(suota_image_header_t);
}
#endif

static void trigger_reboot(void)
{
	hw_cpm_reboot_system();
}

static bool check_num_of_conn(void)
{
        uint8_t num_conn;
        uint16_t *conn_idx;

        ble_gap_get_connected(&num_conn, &conn_idx);

        if (conn_idx) {
                OS_FREE(conn_idx);
        }

        return (num_conn == 1);
}

static att_error_t do_mem_dev_write(suota_service_t *suota, uint16_t conn_idx, uint16_t offset,
                uint16_t length, const uint8_t *value)
{
        uint8_t cmd;

        if (offset) {
                return ATT_ERROR_ATTRIBUTE_NOT_LONG;
        }

        if (length != sizeof(uint32_t)) {
                return ATT_ERROR_APPLICATION_ERROR;
        }

        if (!check_num_of_conn()) {
                suota_notify_status(suota, conn_idx, SUOTA_SRV_EXIT);
                return ATT_ERROR_OK;
        }

        cmd = get_u32(value) >> 24;

        if (cmd < SPOTAR_MEM_INVAL_DEV) {
                suota->addr = get_update_addr(suota);
                suota->written_img_size = 0;
        }

        switch (cmd) {
        case SPOTAR_IMG_SPI_FLASH:
                if (!ready_for_suota(suota)) {
                        suota_notify_status(suota, conn_idx, SUOTA_SRV_EXIT);
                        goto done;
                }

                ble_gap_adv_stop();

                suota_notify_status(suota, conn_idx, SUOTA_IMG_STARTED);
                notify_suota_app_status(suota, SUOTA_START, 0);
                suota->received_data_len = 0;
                suota->header_fragment = 1;
                suota->crc = 0xFFFFFFFF;

                suota->data_buffer = OS_MALLOC(sizeof(uint8_t) * SUOTA_BUFFER_SIZE);

                suota->conn_idx = conn_idx;

                break;

        case SPOTAR_REBOOT:
                /* Reboot on disconnect */
                ble_gap_disconnect(conn_idx, BLE_HCI_ERROR_REMOTE_USER_TERM_CON);
                break;

        case SPOTAR_IMG_END:
                suota->crc ^= 0xFFFFFFFF;

                if (suota->crc != suota->image_crc) {
                        notify_suota_app_status(suota, SUOTA_ERROR, 0);
                        suota_notify_status(suota, conn_idx, SUOTA_CRC_ERR);
                } else {
                        notify_suota_app_status(suota, SUOTA_DONE, 0);
                        suota_notify_status(suota, conn_idx, SUOTA_CMP_OK);
                        if (!set_active_img_ptr(suota)) {
                                return ATT_ERROR_APPLICATION_ERROR;
                        }
                }

                break;

        case SPOTAR_MEM_SERVICE_EXIT:
                suota_notify_status(suota, conn_idx, SUOTA_SRV_EXIT);
                break;
        }

done:
        return ATT_ERROR_OK;
}

static att_error_t do_gpio_map_write(suota_service_t *suota, uint16_t conn_idx, uint16_t offset,
                uint16_t length, const uint8_t *value)
{
        uint32_t gpio;

        if (offset) {
                return ATT_ERROR_ATTRIBUTE_NOT_LONG;
        }

        if (length != sizeof(gpio)) {
                return ATT_ERROR_APPLICATION_ERROR;
        }

        gpio = get_u32(value);

        return ATT_ERROR_OK;
}

static att_error_t do_patch_len_write(suota_service_t *suota, uint16_t conn_idx, uint16_t offset,
                uint16_t length, const uint8_t *value)
{
        if (offset) {
                return ATT_ERROR_ATTRIBUTE_NOT_LONG;
        }

        if (length != sizeof(suota->patch_len)) {
                return ATT_ERROR_APPLICATION_ERROR;
        }

        suota->patch_len = get_u16(value);

        return ATT_ERROR_OK;
}

#if (SUOTA_VERSION == SUOTA_VERSION_1_0)
static bool process_headers(suota_service_t *suota)
{
        suota_image_header_t *img_header;
        suota_product_header_t *prod_header;

        img_header = (suota_image_header_t *) &suota->data_buffer[0];
        prod_header = (suota_product_header_t *) &suota->data_buffer[sizeof(suota_image_header_t)];

        switch (suota->active_img) {
        case SUOTA_ACTIVE_IMG_FIRST:
                suota->image_offset = prod_header->offset_2 + sizeof(suota_image_header_t);
                break;

        case SUOTA_ACTIVE_IMG_SECOND:
                suota->image_offset = prod_header->offset_1 + sizeof(suota_image_header_t);
                break;
        default:
                return false;
        }

        suota->total_image_size = img_header->code_size;
        return true;
}
#else
static void process_headers(suota_service_t *suota)
{
        memcpy(&suota->header, suota->data_buffer, sizeof(suota_image_header_t));

        suota->total_image_size = suota->header.code_size + sizeof(suota_image_header_t);
        suota->image_crc = suota->header.crc;
}
#endif

static void calc_crc(suota_service_t *suota, uint8_t *data, uint16_t len)
{
        suota->crc = suota_update_crc(suota->crc, data, len);
}

static void prepare_flash(suota_service_t *suota, size_t write_size)
{

        uint32_t addr = get_update_addr(suota);
        uint32_t new_addr;

        /* That means what we w to write fits into erased sectors*/
        if (suota->written_img_size + write_size < suota->current_write_sector * FLASH_SECTOR_SIZE) {
                return;
        }

        new_addr = addr + (suota->current_write_sector * FLASH_SECTOR_SIZE);
        ad_nvms_erase_region(suota->nvms, new_addr, FLASH_SECTOR_SIZE);
        suota->current_write_sector++;
}

static att_error_t do_patch_data_write(suota_service_t *suota, uint16_t conn_idx, uint16_t offset,
                uint16_t length, const uint8_t *value)
{
#if (SUOTA_VERSION == SUOTA_VERSION_1_0)
        suota_image_header_t *header;
        int32_t pos;
#endif
        int32_t len;
        int written;
        att_error_t err = ATT_ERROR_OK;

        memcpy(&suota->data_buffer[suota->buffer_pos], value, length);
        suota->buffer_pos += length;
        if (suota->buffer_pos >= SUOTA_BUFFER_SIZE) {
                err = ATT_ERROR_APPLICATION_ERROR;
                goto done;
        }
        suota->received_data_len += length;
        suota->chunk_len += length;

        /* Check if complete block was received */
        if (suota->chunk_len < suota->patch_len && suota->state != SUOTA_CHECK_SIGNATURE) {
                goto done;
        }

        switch (suota->state) {
        case SUOTA_CHECK_SIGNATURE:
                if (suota->received_data_len >= 2) {
                        if (suota->data_buffer[0] == SUOTA_1_0_IMAGE_HEADER_SIGNATURE_B1 &&
                            suota->data_buffer[1] == SUOTA_1_0_IMAGE_HEADER_SIGNATURE_B2) {
                                suota->state = SUOTA_MAIN_HEADER;
                        #if (SUOTA_VERSION == SUOTA_VERSION_1_1)
                        } else if (suota->data_buffer[0] == SUOTA_1_1_IMAGE_HEADER_SIGNATURE_B1 &&
                                   suota->data_buffer[1] == SUOTA_1_1_IMAGE_HEADER_SIGNATURE_B2) {
                                            suota->state = SUOTA_IMAGE_HEADER;
                        #endif
                        } else {
                                /* Invalid image identifier */
                                suota_notify_status(suota, conn_idx, SUOTA_INVAL_IMG_HDR);
                        }
                        goto done;
                }
                break;
        #if (SUOTA_VERSION == SUOTA_VERSION_1_0)
        case SUOTA_MAIN_HEADER:
                /* Main header is always located in first block */
                if (suota->received_data_len >= sizeof(suota_product_header_t) + sizeof(suota_image_header_t)) {
                        /* Read total image size and offsets from headers */
                        if (!process_headers(suota)) {
                                suota_notify_status(suota, conn_idx, SUOTA_INVAL_IMG_BANK);
                                err = ATT_ERROR_APPLICATION_ERROR;
                                goto done;
                        }
                        suota->buffer_pos = 0;
                        suota->state = SUOTA_IMAGE_HEADER;
                }
                break;

        case SUOTA_IMAGE_HEADER:
                /*
                 * When code runs from first images suota should update only the second image,
                 * so the first must be skipped. In this case the skipping is doing in this place.
                 */
                if (suota->received_data_len < suota->image_offset) {
                        suota->buffer_pos = 0;
                        break;
                }

                /* Calculate where the image header begins. */
                pos = (suota->image_offset) - (suota->received_data_len -
                                (suota->patch_len * suota->header_fragment));

                if (pos == suota->patch_len) {
                        /* Header data is in next packet */
                        suota->buffer_pos = 0;
                        break;
                }

                /* Check that the buffer contains all header data */
                if (suota->buffer_pos - pos < sizeof(suota_image_header_t)) {
                        suota->header_fragment++;
                        break;
                }

                /* Read image size from his header */
                header = (suota_image_header_t *) &suota->data_buffer[pos];

                /* Check the image size */
                if (header->code_size > SUOTA_MAX_IMAGE_SIZE) {
                        suota_notify_status(suota, conn_idx, SUOTA_INVAL_IMG_SIZE);
                        goto done;
                }

                suota->image_size = header->code_size;
                suota->image_crc = header->crc;

                /* Remove header data from the block data. The header is not saved in flash memory */
                pos += sizeof(suota_image_header_t);
                memmove(suota->data_buffer, &suota->data_buffer[pos], suota->buffer_pos - pos);
                suota->state = SUOTA_IMAGE_DATA;
                suota->buffer_pos -= pos;

                /* Write the rest of available block data after header removed to the flash memory. */

                prepare_flash(suota, suota->buffer_pos);
                written = ad_nvms_write(suota->nvms, suota->addr, suota->data_buffer,
                                                                                suota->buffer_pos);
                if (written == suota->buffer_pos) {
                        err = ATT_ERROR_APPLICATION_ERROR;
                        goto done;
                }
                calc_crc(suota, suota->data_buffer, suota->buffer_pos);

                suota->addr += suota->buffer_pos;
                suota->written_img_size += suota->buffer_pos;

                suota->buffer_pos = 0;
                break;

        case SUOTA_IMAGE_DATA:
                /* This case is responsible for saving received image data to the flash memory. */
                pos = suota->image_size - suota->written_img_size;

                if (pos >= suota->patch_len) {
                        pos = suota->patch_len;

                } else if (suota->active_img == SUOTA_ACTIVE_IMG_SECOND) {
                        /* All image data is in flash.
                         * Second image is active, suota update first image.
                         * The rest of suota image must be skipped.
                         */
                        suota->state = SUOTA_IMAGE_SKIP;
                }

                prepare_flash(suota, pos);
                written = ad_nvms_write(suota->nvms, suota->addr, suota->data_buffer, pos);
                if (written != pos) {
                        err = ATT_ERROR_APPLICATION_ERROR;
                        goto done;
                }

                calc_crc(suota, suota->data_buffer, pos);

                suota->written_img_size += pos;
                suota->addr += suota->buffer_pos;
                suota->buffer_pos = 0;
                break;

        case SUOTA_IMAGE_SKIP:
                /*
                 * Skip all data of the second image when updating first image.
                 * When suota update second image the first image is skipped in SUOTA_IMAGE_HEADER.
                 */
                suota->buffer_pos = 0;
                break;
        }
        #else
        case SUOTA_IMAGE_SKIP:
        case SUOTA_MAIN_HEADER:
                /* These two cases are not used by SUOTA 1.1 */
                err = ATT_ERROR_APPLICATION_ERROR;
                goto done;
                break;
        case SUOTA_IMAGE_HEADER:
                /* Main header is always located in first block */
                if (suota->received_data_len >= sizeof(suota_image_header_t)) {
                        /* Read image size from header */
                        process_headers(suota);

                        /* Check the image size */
                        if (suota->total_image_size > SUOTA_MAX_IMAGE_SIZE) {
                                suota_notify_status(suota, conn_idx, SUOTA_INVAL_IMG_SIZE);
                                goto done;
                        }

                        prepare_flash(suota, suota->buffer_pos);

                        /* Skip image header, it will be store later */
                        suota->addr += sizeof(suota_image_header_t);

                        if (suota->buffer_pos >= suota->header.exec_location) {
                                len = suota->buffer_pos - suota->header.exec_location;

                                written = ad_nvms_write(suota->nvms, suota->addr,
                                                &suota->data_buffer[suota->header.exec_location],
                                                len);
                                if (written != len) {
                                        err = ATT_ERROR_APPLICATION_ERROR;
                                        goto done;
                                }

                                calc_crc(suota, &suota->data_buffer[suota->header.exec_location],
                                                len);

                                suota->addr += len;
                        }

                        suota->written_img_size += suota->buffer_pos;

                        suota->buffer_pos = 0;
                        suota->state = SUOTA_IMAGE_DATA;
                }
                break;

        case SUOTA_IMAGE_DATA:
                /* This case is responsible for saving received image data to the flash memory. */
                len = suota->total_image_size - suota->written_img_size;

                if (len >= suota->patch_len) {
                        len = suota->patch_len;
                }

                prepare_flash(suota, suota->addr);
                written = ad_nvms_write(suota->nvms, suota->addr, suota->data_buffer, len);
                if (written != len) {
                        err = ATT_ERROR_APPLICATION_ERROR;
                        goto done;
                }

                calc_crc(suota, suota->data_buffer, len);

                suota->written_img_size += len;
                suota->addr += len;
                suota->buffer_pos = 0;
                break;
        }
        #endif

        suota->chunk_len = 0;
        suota_notify_status(suota, conn_idx, SUOTA_CMP_OK);

done:
        return err;
}

static void handle_connected_evt(ble_service_t *svc, const ble_evt_gap_connected_t *evt)
{
        suota_service_t *suota = (suota_service_t *) svc;
        uint16_t disable_val = 0x00;

        ble_gatts_set_value(suota->suota_status_val_h, sizeof(uint8_t), &disable_val);

        suota->state = SUOTA_CHECK_SIGNATURE;
        suota->header_fragment = 1;
        suota->buffer_pos = 0;
        suota->chunk_len = 0;
}

static void handle_disconnected_evt(ble_service_t *svc, const ble_evt_gap_disconnected_t *evt)
{
        suota_service_t *suota = (suota_service_t *) svc;

        if (evt->conn_idx != suota->conn_idx) {
                return;
        }

        /*
         * If device doing suota disconnects, lets reboot.
         * This is to make sure that we always start with latest image
         * even remote device does not send REBOOT command but disconnects
         * instead
         */
        trigger_reboot();
}

/*
 * This callback is called when the GATTS_FLAG_CHAR_READ_REQ flag for characteristic is set
 */
static void handle_read_req(ble_service_t *svc, const ble_evt_gatts_read_req_t *evt)
{
        suota_service_t *suota = (suota_service_t *) svc;

        if (evt->handle == suota->suota_status_ccc_h) {
                uint16_t ccc = 0x0000;

                ble_storage_get_u16(evt->conn_idx, suota->suota_status_ccc_h, &ccc);

                // we're little-endian, ok to write directly from uint16_t
                ble_gatts_read_cfm(evt->conn_idx, evt->handle, ATT_ERROR_OK, sizeof(ccc), &ccc);

        } else if (evt->handle == suota->suota_mem_info_val_h) {
                ble_gatts_read_cfm(evt->conn_idx, evt->handle, ATT_ERROR_OK,
                                sizeof(suota->received_data_len), &suota->received_data_len);
        } else {
                ble_gatts_read_cfm(evt->conn_idx, evt->handle, ATT_ERROR_READ_NOT_PERMITTED, 0, NULL);
        }
}

static void handle_write_req(ble_service_t *svc, const ble_evt_gatts_write_req_t *evt)
{
        suota_service_t *suota = (suota_service_t *) svc;
        att_error_t status = ATT_ERROR_ATTRIBUTE_NOT_FOUND;

        if (evt->handle == suota->suota_status_ccc_h) {
                status = do_bl_ccc_write(suota, evt->conn_idx, evt->offset, evt->length, evt->value);
                goto done;
        }

        if (evt->handle == suota->suota_mem_dev_val_h) {
                status = do_mem_dev_write(suota, evt->conn_idx, evt->offset, evt->length, evt->value);
                goto done;
        }

        if (evt->handle == suota->suota_gpio_map_val_h) {
                status = do_gpio_map_write(suota, evt->conn_idx, evt->offset, evt->length, evt->value);
                goto done;
        }

        if (evt->handle == suota->suota_patch_len_val_h) {
                status = do_patch_len_write(suota, evt->conn_idx, evt->offset, evt->length, evt->value);
                goto done;
        }

        if (evt->handle == suota->suota_patch_data_val_h) {
                status = do_patch_data_write(suota, evt->conn_idx, evt->offset, evt->length, evt->value);
                goto done;
        }

done:
        ble_gatts_write_cfm(evt->conn_idx, evt->handle, status);
}

#if (SUOTA_VERSION == SUOTA_VERSION_1_0)
static suota_active_img_t get_active_img(nvms_t nvms)
{
        uint32_t ptr = 0xFFFF;

        /* Address 0x3000 contains pointer to active img.*/
        ad_nvms_read(nvms, ACTIVE_IMG_PTR, (uint8_t *) &ptr, sizeof(uint32_t));
        if (ptr == 0 || ptr == FIRST_IMG_ADDR || ptr == (IMG_ADDR_BASE | FIRST_IMG_ADDR)) {
                return SUOTA_ACTIVE_IMG_FIRST;
        }

        if (ptr == SECOND_IMG_ADDR || ptr == (IMG_ADDR_BASE | SECOND_IMG_ADDR)){
                return SUOTA_ACTIVE_IMG_SECOND;
        }

        return SUOTA_ACTIVE_IMG_ERROR;
}
#else
static suota_active_img_t get_active_img(nvms_t nvms)
{
        return SUOTA_ACTIVE_IMG_FIRST;
}
#endif

suota_active_img_t suota_get_active_img(ble_service_t *svc) {
        suota_service_t *suota = (suota_service_t *) svc;

        return suota->active_img;
}

ble_service_t *suota_init(const suota_callbacks_t *cb)
{
        uint16_t num_attr;
        uint16_t cpf_h = 0;
        suota_service_t *suota;
        att_uuid_t uuid;
        suota_active_img_t img;
        nvms_t nvms;

        #if (SUOTA_VERSION == SUOTA_VERSION_1_0)
        nvms = ad_nvms_open(NVMS_FIRMWARE_PART);
        #else
        nvms = ad_nvms_open(NVMS_FW_UPDATE_PART);
        #endif
        if (!nvms) {
                return NULL;
        }

        img = get_active_img(nvms);
        if (img == SUOTA_ACTIVE_IMG_ERROR) {
                return NULL;
        }

        suota = OS_MALLOC(sizeof(*suota));
        memset(suota, 0, sizeof(*suota));

        suota->svc.connected_evt = handle_connected_evt;
        suota->svc.disconnected_evt = handle_disconnected_evt;
        suota->svc.read_req = handle_read_req;
        suota->svc.write_req = handle_write_req;
        suota->cb = cb;
        suota->active_img = img;
        suota->conn_idx = BLE_CONN_IDX_INVALID;

        suota->nvms = nvms;

        num_attr = ble_service_get_num_attr(NULL, NUM_OF_CHAR, NUM_OF_DESC);

        ble_uuid_create16(UUID_SUOTA_SERVICE, &uuid);
        ble_gatts_add_service(&uuid, GATT_SERVICE_PRIMARY, num_attr);

        ble_uuid_from_string(UUID_SUOTA_MEM_DEV, &uuid);
        ble_gatts_add_characteristic(&uuid, GATT_PROP_READ | GATT_PROP_WRITE, ATT_PERM_RW,
                                        4, 0, NULL, &suota->suota_mem_dev_val_h);

        ble_uuid_from_string(UUID_SUOTA_GPIO_MAP, &uuid);
        ble_gatts_add_characteristic(&uuid, GATT_PROP_READ | GATT_PROP_WRITE, ATT_PERM_RW,
                                        4, 0, NULL, &suota->suota_gpio_map_val_h);

        ble_uuid_from_string(UUID_SUOTA_MEM_INFO, &uuid);
        ble_gatts_add_characteristic(&uuid, GATT_PROP_READ, ATT_PERM_READ,
                                        4, 0, NULL, &suota->suota_mem_info_val_h);

        ble_uuid_from_string(UUID_SUOTA_PATCH_LEN, &uuid);
        ble_gatts_add_characteristic(&uuid, GATT_PROP_READ | GATT_PROP_WRITE, ATT_PERM_RW,
                                        2, 0, NULL, &suota->suota_patch_len_val_h);

        ble_uuid_from_string(UUID_SUOTA_PATCH_DATA, &uuid);
        ble_gatts_add_characteristic(&uuid, GATT_PROP_READ | GATT_PROP_WRITE |GATT_PROP_WRITE_NO_RESP,
                                        ATT_PERM_RW, SUOTA_PD_CHAR_SIZE, 0, NULL,
                                        &suota->suota_patch_data_val_h);

        ble_uuid_from_string(UUID_SUOTA_STATUS, &uuid);
        ble_gatts_add_characteristic(&uuid, GATT_PROP_READ | GATT_PROP_NOTIFY, ATT_PERM_READ,
                                        1, 0, NULL, &suota->suota_status_val_h);

        ble_uuid_create16(UUID_GATT_CLIENT_CHAR_CONFIGURATION, &uuid);
        ble_gatts_add_descriptor(&uuid, ATT_PERM_RW, 1, 0, &suota->suota_status_ccc_h);

        ble_gatts_register_service(&suota->svc.start_h, &suota->suota_mem_dev_val_h,
                                        &suota->suota_gpio_map_val_h,
                                        &suota->suota_mem_info_val_h,
                                        &suota->suota_patch_len_val_h,
                                        &suota->suota_patch_data_val_h,
                                        &suota->suota_status_val_h,
                                        &suota->suota_status_ccc_h, &cpf_h, 0);

        suota->svc.end_h = suota->svc.start_h + num_attr;

        return &suota->svc;
}
