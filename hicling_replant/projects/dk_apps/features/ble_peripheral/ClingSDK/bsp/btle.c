/***************************************************************************//**
 * File: btle.c
 * 
 * Description: BLUETOOTH low energy protocol and state machine handle
 *
 ******************************************************************************/

#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include "main.h"
#include "Aes.h"
#define BLE_PAYLOAD_LEN 20
#include "ble_cus.h"
//#include "user_peripheral.h"
///////////////////////////////
// SPEED 
//   Low: 
//       Min connection interval -- 300 * 1.25ms = 375ms
//       Man connection interval -- 320 * 1.25ms = 400ms
//       Slave Latency -- 4
//       Connection supervision timeout-- 600 * 10ms = 6000ms
//       Enable Update Request: ON
// 
//   Medium:
//       Min connection interval -- 72 * 1.25ms = 90ms
//       Man connection interval -- 92 * 1.25ms = 115ms
//       Slave Latency -- 0
//       Connection supervision timeout-- 600 * 10ms = 6000ms
//       Enable Update Request: ON
// 
//   High:
//      Min connection interval -- 16 * 1.25ms = 20ms
//      Man connection interval -- 36 * 1.25ms = 45ms
//      Slave Latency -- 0
//      Connection supervision timeout-- 600 * 10ms = 6000ms
//      Enable Update Request: default ON
//
//    off:
//      Min connection interval -- 300 * 1.25ms = 375ms
//      Man connection interval -- 320 * 1.25ms = 400ms
//      Slave Latency -- 4
//      Connection supervision timeout-- 600 * 10ms = 6000ms
//      Enable Update Request: OFF
//
struct cling_ble_status {
        uint8_t conn_id;
        uint8_t start_hdr;
};

static struct cling_ble_status ble_status;

static int ble_notify_callback(uint16_t uuid_num, uint8_t status)
{
        BLE_CTX *r = &cling.ble;
        Y_SPRINTF("[BTLE]: notification complete status = %d", status);
        if (status == BLE_STATUS_OK) {
                r->tx_buf_available += 20;
        }
        return BLE_STATUS_OK;
}

static int ble_rev_process(uint16_t uuid_num, uint8_t *s, size_t size)
{

#if 1
        BLE_CTX *r = &cling.ble;
        uint8_t *rec_buf = cling.gcp.pkt_buff;
        uint8_t i = 0;
#if 0
        if (size != 20)
                return PRF_APP_ERROR;
#endif
        // Set index for correct characteristics
        rec_buf[i++] = (uint8_t)(uuid_num >> 8);

        rec_buf[i++] = (uint8_t)uuid_num;

        memcpy(rec_buf + i, s, size);

        cling.gcp.b_new_pkt = TRUE;

       // N_SPRINTF("[BTLE] a new packet");

       // r->packet_received_ts = CLK_get_system_time();
#endif
        printf("[BTLE]: ble rev process uuid = 0x%04x size = %d timestamp = %d", uuid_num , size, r->packet_received_ts);
        return BLE_STATUS_OK;
}

void btle_init()
{
        printf("[btle]: btle init\r\n");
        /*register callback funtion*/
        //app_cust1_val_wr_callback_reg(ble_rev_process);
        cling_service_reg_write_callback(ble_rev_process);
        //app_notify_complete_callback_reg(ble_notify_callback);
}

void _radio_state_cleanup()
{
        // iOS should release MUTEX whatsoever
        SYSTEM_release_mutex(MUTEX_IOS_LOCK_VALUE);

        // if we are in middle of a file write, we then close out this file
        FILE_fclose_force();
}

void BTLE_reset_streaming()
{
        BLE_CTX *r = &cling.ble;
        // Reset minute streaming amount;
        r->streaming_minute_file_amount = 0;
        r->streaming_minute_file_index = 0;
        r->streaming_minute_scratch_amount = 0;
        r->streaming_minute_scratch_index = 0;
        r->streaming_second_count = 0;

        cling.system.mcu_reg[REGISTER_MCU_CTL] &= ~CTL_IM;

        N_SPRINTF("[BTLE] Reset BTLE streaming");
}

void ble_on_disconnect_dispatch()
{
        ble_status.conn_id = 0;
}

void ble_on_connect_dispatch(uint8_t connection_idx, struct gapc_connection_req_ind const *param)
{
        ble_status.conn_id = connection_idx;
}
void BTLE_execute_adv(BOOLEAN b_fast)
{

}

void BTLE_on_ble_evt()
{

}

BOOLEAN BTLE_tx_buffer_available()
{
        return FALSE;
}

BOOLEAN BTLE_Send_Packet(I8U *data, I8U len)
{
        uint16_t uuid_num = (data[0] << 8) | data[1];
#ifndef _CLING_PC_SIMULATION_
        BLE_CTX *r = &cling.ble;
        uint16_t hvx_len;
        I8U ch_idx = 0;
        uint32_t err_code;

        // Input parameter check
        if (data == NULL || (len != CP_UUID_PKT_SIZE))
                return FALSE;

//	N_SPRINTF("[BTLE] %02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,",
//	data[2], data[3], data[4],data[5], data[6], data[7], data[8], data[9],
//	data[10], data[11], data[12], data[13], data[14],data[15], data[16], data[17], data[18], data[19],data[20], data[21]);

        hvx_len = len - 2;

        app_custs1_send_data(uuid_num, data + sizeof(uuid_num), hvx_len);

        if (r->tx_buf_available > 1) {
                // Decrease the buffer
                r->tx_buf_available--;
        }

        N_SPRINTF("[BTLE] packet sent OK (%d)!", r->tx_buf_available);

        return TRUE;
#else
        return TRUE;
#endif
        return TRUE;
}

// AES has to go through API call as ECB is restricted from SoftDevice
void BTLE_aes_encrypt(I8U *key, I8U *in, I8U *out)
{
#ifdef _AES_ENABLED_
        // Set up hal_aes using new key and init vector
        nrf_ecb_hal_data_t ecb_ctx;

        // Set ECB key
        memcpy(ecb_ctx.key, key, SOC_ECB_KEY_LENGTH);
        // Set ECB text input
        memcpy(ecb_ctx.cleartext, in, SOC_ECB_CLEARTEXT_LENGTH);
        // Encryption
        sd_ecb_block_encrypt(&ecb_ctx);

        // get encrypted text
        memcpy(out, ecb_ctx.ciphertext, SOC_ECB_CLEARTEXT_LENGTH);
#endif
}

//
// The decryption does NOT work at the point as Nordic
// AES ECB hardware block only supports encrypt mode
//
void BTLE_aes_decrypt(I8U *key, I8U *in, I8U *out)
{
        AES_CTX ctx;

        // Generate sbox code table
        AES_generateSBox();

        // Set keys
        AES_set_key(&ctx, key, NULL, AES_MODE_128);

        AES_convert_key(&ctx);

        AES_cbc_decrypt(&ctx, in, out, AES_BLOCKSIZE * 4);
}

// 
// -------------- OLD ---------------
//

BOOLEAN BTLE_is_connected()
{
        return FALSE;
}

BOOLEAN BTLE_is_advertising()
{
        return TRUE;
}

BOOLEAN BTLE_is_idle()
{
        return TRUE;
}

BOOLEAN BTLE_is_streaming_enabled(void)
{
        return TRUE;
}

/*
 retrive the amount of minute activity need to be uploaded
 */
uint32_t get_flash_minute_activity_offset()
{

        return 0;
}

static void _disconnect_clean_up()
{
}

BOOLEAN BTLE_streaming_authorized()
{
        return TRUE;
}

void BTLE_update_streaming_minute_entries()
{

}

void BTLE_state_machine_update()
{
}

/*
 * Input: pointer of radio software version (minor release build number)
 *        the initial version number is 0, maximum version number is 127
 *
 * Nordic BLE SDK:  current version - 4.4.2.33551
 * Return: TRUE -- radio software version number is accessed
 *         FALSE -- radio software version number isn't accessed
 */
BOOLEAN BTLE_get_radio_software_version(I8U *radio_sw_ver)
{
        return TRUE;
}

