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
#include "ble_service.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * LOCAL DECLARATION
 */
static int btle_on_disconnect();
static void btle_on_connect(ble_service_t *svc, uint16_t conn_idx);
void BTLE_reset_streaming();
/******************************************************************************
 * FunctionName :btle_fsm_init
 * Description  : init fsm state
 * Parameters   : write: write call back function
 * Returns          : 0: sucess
 -1: error
 *******************************************************************************/

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
ble_service_t *btle_svc = NULL;

static int ble_notify_callback(ble_service_t *svc, uint16_t uuid_num, uint8_t status)
{
        BLE_CTX *r = &cling.ble;
        Y_SPRINTF("[BTLE]: notification complete status = %d", status);
        if (status == BLE_STATUS_OK) {
                r->tx_buf_available += 20;
        }
        return BLE_STATUS_OK;
}

static int ble_rev_process(ble_service_t *svc, uint16_t conn_idx, uint16_t uuid_num, uint8_t *s,
        size_t size)
{

        btle_svc = svc;
        ble_status.conn_id = conn_idx;
#if 1
        BLE_CTX *r = &cling.ble;
        uint8_t *rec_buf = cling.gcp.pkt_buff;
        uint8_t i = 0;
#if 1
        if (size != 20)
                return PRF_APP_ERROR;
#endif
        // Set index for correct characteristics
        rec_buf[i++] = (uint8_t)(uuid_num >> 8);

        rec_buf[i++] = (uint8_t)uuid_num;

        memcpy(rec_buf + i, s, size);

        cling.gcp.b_new_pkt = TRUE;

        Y_SPRINTF("[BTLE] a new packet");

        r->packet_received_ts = CLK_get_system_time();
        char *data = rec_buf;
        Y_SPRINTF("[BTLE] rev = %02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,",
                data[2], data[3], data[4],data[5], data[6], data[7], data[8], data[9],
                data[10], data[11], data[12], data[13], data[14],data[15], data[16], data[17], data[18], data[19],data[20], data[21]);

#endif
        N_SPRINTF("[BTLE]: ble rev process uuid = 0x%02x%02x  size = %d timestamp = %d", rec_buf[0], rec_buf[1] , size, r->packet_received_ts);
        notify_cling_task_a_package_has_arrived();
        return BLE_STATUS_OK;
}

void btle_init()
{
        Y_SPRINTF("[btle]: btle init");
        /*register callback funtion*/
        //app_cust1_val_wr_callback_reg(ble_rev_process);
        cling_service_reg_write_callback(ble_rev_process);
        cling_service_reg_conn_callback(btle_on_connect);
        cling_service_reg_disconn_callback(btle_on_disconnect);
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

        Y_SPRINTF("[BTLE] Reset BTLE streaming");
}

void BTLE_execute_adv(BOOLEAN b_fast)
{
        BLE_CTX *r = &cling.ble;

        if (!BATT_is_charging()) {
                if (BATT_is_low_battery() && LINK_is_authorized()) {
                        Y_SPRINTF("[BTLE] BLE low power");
                        r->btle_State = BTLE_STATE_IDLE;
                        return;
                }
        }

        if (!cling.system.b_powered_up)
                return;

        // if UI is ON or battery is in charging state, we should start advertising again
        if (b_fast)
                r->adv_mode = BLE_FAST_ADV;
        else
                r->adv_mode = BLE_SLOW_ADV;

//        HAL_advertising_start();

        r->btle_State = BTLE_STATE_ADVERTISING;

        Y_SPRINTF("[BTLE] start advertising.");
}

BOOLEAN BTLE_tx_buffer_available()
{
        return TRUE;
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

        N_SPRINTF("[BTLE] %02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,%02x,",
                data[2], data[3], data[4],data[5], data[6], data[7], data[8], data[9],
                data[10], data[11], data[12], data[13], data[14],data[15], data[16], data[17], data[18], data[19],data[20], data[21]);

        hvx_len = len - 2;

        cus_notify_send(btle_svc, ble_status.conn_id, uuid_num, data + sizeof(uuid_num), hvx_len);
#if 0
        if (r->tx_buf_available > 1) {
                // Decrease the buffer
                r->tx_buf_available--;
        }
#endif
        Y_SPRINTF("[BTLE] packet sent OK (%d)!", r->tx_buf_available);

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
        BLE_CTX *r = &cling.ble;

        if (r->btle_State == BTLE_STATE_CONNECTED) {
                return TRUE;
        }
        else {
                return FALSE;
        }
}

BOOLEAN BTLE_is_advertising()
{
        BLE_CTX *r = &cling.ble;

        if (r->btle_State == BTLE_STATE_ADVERTISING) {
                return TRUE;
        }
        else {
                return FALSE;
        }
}

BOOLEAN BTLE_is_idle()
{
        BLE_CTX *r = &cling.ble;

        if (r->btle_State == BTLE_STATE_IDLE) {
                return TRUE;
        }
        else {
                return FALSE;
        }
}

BOOLEAN BTLE_is_streaming_enabled(void)
{
        I32U t_curr = CLK_get_system_time();

        // If BLE is disconnected, go return FALSE
        if (cling.ble.btle_State != BTLE_STATE_CONNECTED)
                return FALSE;

        // If streaming is turned off, go return FALSE;
        if (!(cling.system.mcu_reg[REGISTER_MCU_CTL] & CTL_IM)) {
                return FALSE;
        }

        // If Streaming second based activity packets is more than 5, go return FALSE
        if (cling.ble.streaming_second_count > BLE_STREAMING_SECOND_MAX) {
                return FALSE;
        }

        if (t_curr > (cling.ble.packet_received_ts + 30000))
                return FALSE;

        return TRUE;
}

/*

 /*
 retrive the amount of minute activity need to be uploaded
 */
uint32_t get_flash_minute_activity_offset()
{
#define DATA_SENDED_MARK (0x80000000)
        I32U stored_offset = 0;
        /*remember the last position we end our search*/
        static I32U data_sended_offset = 0;
        I32U offset = data_sended_offset;
        I32U dw_buf[4];
        uint32_t epoch_file_count = FILE_exists_with_prefix((I8U *)"epoch", 5);
        Y_SPRINTF("[TRACKING] epoch file number = %d", epoch_file_count);
        /*if a buffered file existed in file system this means this device has not been synced for days*/
        if (epoch_file_count > 0) {
                return ((epoch_file_count * 4096) / sizeof(MINUTE_TRACKING_CTX));
        }
        /*if no buffer file existed, then check out data entried today*/
        MINUTE_TRACKING_CTX *minute = (MINUTE_TRACKING_CTX *)dw_buf;
        I8U *pbuf = (I8U *)dw_buf;
        while (offset < SYSTEM_TRACKING_SPACE_SIZE) {
                FLASH_Read_App(offset + SYSTEM_TRACKING_SPACE_START, pbuf, 16);
                if (minute->epoch == 0xffffffff) {
                        stored_offset = offset;
                        stored_offset -= sizeof(MINUTE_TRACKING_CTX);
                        break;
                }
                else if (((minute->epoch & DATA_SENDED_MARK) == 0)) {
                        /*the first entry point has been found */
                        data_sended_offset = offset;
                }
                offset += sizeof(MINUTE_TRACKING_CTX);
        }
        Y_SPRINTF("[TRACKING] stored_offset = %d : data_sended_offset = %d", stored_offset, data_sended_offset);
        /*if glitch happened*/
        if (data_sended_offset > stored_offset || data_sended_offset >= SYSTEM_TRACKING_SPACE_SIZE) {
                data_sended_offset = 0;
                return 0;
        }
        /*for origin offset take the unint of Bytes, and one package contians 16 bytes, so this is the reason why 4 bytes righ shift
         necessary here*/
        return ((stored_offset - data_sended_offset) >> 4);
}

static void _disconnect_clean_up()
{
        BLE_CTX *r = &cling.ble;
#ifndef _CLING_PC_SIMULATION_
        // Reboot
        if (r->disconnect_evt & BLE_DISCONN_EVT_REBOOT) {
                Y_SPRINTF("[BTLE] disconn event: reboot");

                // clear factory reset flag
                r->disconnect_evt &= ~BLE_DISCONN_EVT_REBOOT;

                // Jumpt to reset vector
                SYSTEM_restart_from_reset_vector();

                return;
        }

        if (r->disconnect_evt & BLE_DISCONN_EVT_BONDMGR_ERROR) {
                Y_SPRINTF("[BTLE] disconn event: bond error");

                // clear factory reset flag
                r->disconnect_evt &= ~BLE_DISCONN_EVT_BONDMGR_ERROR;

                // Clear bond information
//                HAL_device_manager_init(TRUE);

#ifdef _ENABLE_ANCS_
                cling.ancs.bond_flag = ANCS_BOND_STATE_NUKNOW_FLAG;
#endif
                return;
        }

        // Factory reset
        if (r->disconnect_evt & BLE_DISCONN_EVT_FACTORY_RESET) {
                Y_SPRINTF("[BTLE] disconn event: factory reset");

                // De-authorize
                LINK_deauthorize();

                // Add latency before storing messages (Erasure latency: 50 ms)
                BASE_delay_msec(50);

                // System format disk along with all data erasure
                SYSTEM_format_disk(TRUE);

                // Wipe out all user data
                memset(&cling.user_data, 0, sizeof(USER_DATA));

                // clear factory reset flag
                r->disconnect_evt &= ~BLE_DISCONN_EVT_FACTORY_RESET;

                // Jumpt to reset vector
                SYSTEM_restart_from_reset_vector();

                return;
        }

#endif
        if (r->disconnect_evt & BLE_DISCONN_EVT_FAST_CONNECT) {
                r->disconnect_evt &= ~BLE_DISCONN_EVT_FAST_CONNECT;
                return;
        }

        // If BLE disconnects in middle of OTA, restart the device
        if (OTA_if_enabled()) {
                Y_SPRINTF("[BTLE] OTA reboot at end of disconnect clean-up");
                SYSTEM_restart_from_reset_vector();
        }
}

BOOLEAN BTLE_streaming_authorized()
{
        I32U t_curr = CLK_get_system_time();
        BLE_CTX *r = &cling.ble;

        // Nothing gets streamed out if OTA is turned on
        if (OTA_if_enabled()) {
                if (t_curr > (r->packet_received_ts + 30000)) {
                        Y_SPRINTF("[BTLE] OTA no response for 30 seconds -> reboot ");
                        SYSTEM_restart_from_reset_vector();
                }
                return FALSE;
        }

        // Streaming second based activity packets
        if (r->streaming_second_count > BLE_STREAMING_SECOND_MAX) {
                // Switch to slow connection mode if the streaming part is done
//                if (HAL_set_slow_conn_params(SWITCH_SPEED_FOR_POWER_SAVING)) {
//                        N_SPRINTF("Streaming ends: Slow Connection interval(%d)", cling.system.reconfig_cnt);
//                }
                return FALSE;
        }
        else {
                if (t_curr > (r->packet_received_ts + 20000)) {
#ifndef _CLING_PC_SIMULATION_
                        BTLE_reset_streaming();
#endif
                        // if we haven't seen anything in about 20 seconds, switch to slow connection mode
//                        if (HAL_set_slow_conn_params(SWITCH_SPEED_FOR_POWER_SAVING)) {
//                                N_SPRINTF("No packets: Slow Connection interval");
//                        }
                }
                else {
                        // Update amount of data needs to be streamed
                        if (!r->streaming_minute_scratch_amount) {
                                BTLE_update_streaming_minute_entries();
                        }
                }
        }

        // Make sure streaming is allowed
        if (!(cling.system.mcu_reg[REGISTER_MCU_CTL] & CTL_IM)) {
                return FALSE;
        }

        // Minute data has higher priority
        if (r->streaming_minute_scratch_index < r->streaming_minute_scratch_amount) {
#if 0
                // If elapsed time is greater than 1 hour, switch to fast mode.
                if ((t_curr > (r->streaming_ts + 3600000)) || (r->streaming_ts == 0)) {
                        if (HAL_set_conn_params(TRUE)) {
                                Y_SPRINTF("Streaming starts: FAST Connection interval");
                        }
                }
#endif
                if (CP_create_streaming_minute_msg(SYSTEM_TRACKING_SPACE_SIZE)) {
                        return TRUE;
                }
        }

        // All the past activity data in file system
        if (r->streaming_minute_file_index < r->streaming_minute_file_amount) {
                if (r->minute_file_entry_index < MINUTE_STREAMING_FILE_ENTRIES) {
                        if (CP_create_streaming_file_minute_msg(MINUTE_STREAMING_FILE_SIZE)) {
                                return TRUE;
                        }
                }

                // Move onto next file, and upload its content
                r->streaming_minute_file_index++;
                if (r->streaming_minute_file_index < r->streaming_minute_file_amount) {
                        r->minute_file_entry_index = 0;
                        N_SPRINTF("--- get next minute file: %d, %d --", r->streaming_minute_file_index, r->streaming_minute_file_amount);
                        r->streaming_file_addr = FILE_get_next_file_with_prefix((I8U *)"epoch", 5,
                        TRUE);
                        if (r->streaming_file_addr) {
                                CP_create_streaming_file_minute_msg(MINUTE_STREAMING_FILE_SIZE);

                                return TRUE;
                        }
                }
                else {

                        FILE_get_next_file_with_prefix((I8U *)"epoch", 5, TRUE);
                        N_SPRINTF("--- delete minute file --");
                }
        }

        // then come the second data
        if (t_curr > (r->streaming_ts + 1000)) {

                // Streaming seconds means we are done with syncing, so switch to slow connection mode
//                HAL_set_slow_conn_params(SWITCH_SPEED_FOR_POWER_SAVING);

// For background activity streaming, we default media to be BLE.
                CP_create_streaming_daily_msg();
                r->streaming_ts = t_curr;       // Recording streaming time stamp
                r->streaming_second_count++;
                N_SPRINTF("[BLE] activity streaming (%d)", r->streaming_second_count); //streaming one packet");
        }

        return FALSE;
}

void BTLE_update_streaming_minute_entries()
{
        BLE_CTX *r = &cling.ble;

        //
        // 32 KB scratch pad, so that is equivalent to 2048 entries
        r->streaming_minute_scratch_amount = (SYSTEM_TRACKING_SPACE_SIZE >> 4);
        r->streaming_minute_scratch_index = 0;

        // Each file is 4 KB, so that is equivalent to 256 entries
        //
        r->streaming_minute_file_amount = FILE_exists_with_prefix((I8U *)"epoch", 5);
        r->streaming_minute_file_index = 0;
        Y_SPRINTF("Streaming amount, flash: %d, file: %d", r->streaming_minute_scratch_amount, r->streaming_minute_file_amount);
        if (r->streaming_minute_file_amount)
        {
                // Get the start address and reset entry index
                r->streaming_file_addr = FILE_get_next_file_with_prefix((I8U *)"epoch", 5, FALSE);
                r->minute_file_entry_index = 0;
#if 0
                if (HAL_set_conn_params(TRUE)) {
                        N_SPRINTF("Streaming Files: FAST Connection interval");
                }
#endif
        }
        //
        // Reset all the streaming flags
        //
        cling.gcp.streaming.pending = FALSE;
        cling.gcp.streaming.packet_need_ack = FALSE;

}

void BTLE_state_machine_update()
{
#ifndef _CLING_PC_SIMULATION_

        BLE_CTX *r = &cling.ble;

        switch (r->btle_State) {
        case BTLE_STATE_IDLE:
                break;
        case BTLE_STATE_DISCONNECTED:
                // If device is disconnected, check on all the pending events
                if (r->disconnect_evt) {
                        _disconnect_clean_up();
                }
                if (r->adv_mode == BLE_FAST_ADV) {
                        // For whatever reason device is disconnected we should start advertising again
                        BTLE_execute_adv(TRUE);
                }
                else {
                        BTLE_execute_adv(FALSE);
                }
                break;
        case BTLE_STATE_DISCONNECTING:
                // Wait for disconnected
                break;
        case BTLE_STATE_CONNECTING:
                break;
        case BTLE_STATE_CONNECTED:
                if (LINK_is_authorized()) {

                        // Streaming control
                        BTLE_streaming_authorized();
                }
                break;
        case BTLE_STATE_ADVERTISING:
                // Nothing, waiting for either advertising timeout or connected
                break;
        default:
                break;
        }
#endif
}

/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
#ifndef _CLING_PC_SIMULATION_
static void btle_on_connect(ble_service_t *svc, uint16_t conn_idx)
{

        ble_status.conn_id = conn_idx;
        btle_svc = svc;

        BLE_CTX *r = &cling.ble;

//        r->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;

        r->btle_State = BTLE_STATE_CONNECTED;

        r->packet_received_ts = CLK_get_system_time();

        // Just connected, BLE connection is in a default fast mode
        r->b_conn_params_updated = FALSE;
        cling.system.conn_params_update_ts = CLK_get_system_time();
//        sd_ble_tx_buffer_count_get(&r->tx_buf_available);

        Y_SPRINTF("[BTLE] Connected! ");
}
#endif

/*
 **@brief Function for handling the Disconnect event.
 *
 */
#ifndef _CLING_PC_SIMULATION_
static int btle_on_disconnect()
{
#define BLE_CONN_HANDLE_INVALID 0xFFFF  /**< Invalid Connection Handle. */
        ble_status.conn_id = BLE_CONN_HANDLE_INVALID;
        btle_svc = NULL;
        BLE_CTX *r = &cling.ble;
        r->conn_handle = BLE_CONN_HANDLE_INVALID;
        r->btle_State = BTLE_STATE_DISCONNECTED;
        Y_SPRINTF("[BTLE] disconnected! ");

        // Clear all BTLE states
        _radio_state_cleanup();

        // Reset minute streaming amount;
        BTLE_reset_streaming();

        // Stop notifying vibration
        NOTIFIC_stop_notifying();

        if (r->disconnect_evt & BLE_DISCONN_EVT_FAST_CONNECT)
                return -1;

        if (!r->disconnect_evt) {
                NOTIFIC_start_idle_alert();
        }

        if (OTA_if_enabled()) {

                N_SPRINTF("[BTLE] reboot if BLE disconnected in middle of OTA! ");

                SYSTEM_restart_from_reset_vector();
        }
        return 0;
}

#endif

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

