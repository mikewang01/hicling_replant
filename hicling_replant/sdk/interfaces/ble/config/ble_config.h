/**
 \addtogroup INTERFACES
 \{
 \addtogroup BLE
 \{
 \addtogroup BLE_CONFIG
 \{
 */

/**
 ****************************************************************************************
 *
 * @file ble_config.h
 *
 * @brief BLE configuration options
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
#ifndef _BLE_CONFIG_H_
#define _BLE_CONFIG_H_

#include <stdint.h>

/**
 * BLE device default configuration parameters
 */

/**
 * \brief Default device name (GAP service attribute only)
 *
 * Device Name used for GAP service attribute.
 *
 * \note This is not reflected on advertising data, which have to be changed either using
 * ble_gap_adv_data_set() or by changing #defaultBLE_ADVERTISE_DATA.
 */
#ifndef defaultBLE_DEVICE_NAME
#define defaultBLE_DEVICE_NAME               "Dialog BLE"
#endif

/**
 * \brief Default appearance (GAP service attribute only)
 *
 * Appearance used for GAP service attribute.
 *
 * \note This is not reflected on advertising data, which have to be changed either using
 * ble_gap_adv_data_set() or by changing #defaultBLE_ADVERTISE_DATA.
 */
#ifndef defaultBLE_APPEARANCE
#define defaultBLE_APPEARANCE                (0)
#endif

/**
 * \brief Default maximum number of simultaneous connections
 *
 * Maximum number of connections that can maintained simultaneously.
 */
#ifndef defaultBLE_MAX_CONNECTIONS
#define defaultBLE_MAX_CONNECTIONS           (8)
#endif

/**
 * \brief Default maximum number of bonded devices
 *
 * Maximum number of bonded devices for which bonding data can be maintained in persistent storage.
 */
#ifndef defaultBLE_MAX_BONDED
#define defaultBLE_MAX_BONDED                (8)
#endif

/**
 * \brief Default GAP role
 *
 * Default GAP role set at start-up, if not set otherwise using ble_gap_role_set(),
 * ble_peripheral_start() or ble_central_start()).
 */
#ifndef defaultBLE_GAP_ROLE
#define defaultBLE_GAP_ROLE                  GAP_NO_ROLE
#endif

/**
 * \brief Default random address renew duration
 *
 * Default duration for random address generation when a random resolvable or a random
 * non-resolvable address has been set using ble_gap_address_set().
 *
 * \note Value is in steps of 10ms and minimum renew duration that can be set is 150s (values lower
 * than 15000 will be ignored).
 */
#ifndef defaultBLE_ADDRESS_RENEW_DURATION
#define defaultBLE_ADDRESS_RENEW_DURATION    (15000)    // 150s (15000 * 10ms).Stack minimum is 150s
#endif

/**
 * \brief Default static BD address
 *
 * Default static BD address set if one is not retrieved from the non-volatile storage.
 */
#ifndef defaultBLE_STATIC_ADDRESS
#define defaultBLE_STATIC_ADDRESS            { 0x01, 0x00, 0x80, 0xCA, 0xEA, 0x80 }
#endif

/**
 * \brief Default BD address type
 *
 * Default BD address type used if one is not set using ble_gap_address_set().
 */
#ifndef defaultBLE_ADDRESS_TYPE
#define defaultBLE_ADDRESS_TYPE              PUBLIC_ADDRESS
#endif

/**
 * \brief Default Identity Resolution Key
 *
 * Default Identity Resolution Key to be used upon IRK echange.
 */
#ifndef defaultBLE_IRK
#define defaultBLE_IRK                       { 0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01, \
                                               0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01 }
#endif

/**
 * \brief Default attribute database configuration
 *
 * Default attribute database configuration:
 *     7     6    5     4     3    2    1    0
 * +-----+-----+----+-----+-----+----+----+----+
 * | DBG | RFU | SC | PCP | APP_PERM |NAME_PERM|
 * +-----+-----+----+-----+-----+----+----+----+
 * - Bit [0-1]: Device Name write permission requirements for peer device (\see gapm_write_att_perm)
 * - Bit [2-3]: Device Appearance write permission requirements for peer device (\see gapm_write_att_perm)
 * - Bit [4]  : Slave Preferred Connection Parameters present
 * - Bit [5]  : Service change feature present in GATT attribute database.
 * - Bit [6]  : Reserved
 * - Bit [7]  : Enable Debug Mode
 */
#ifndef defaultBLE_ATT_DB_CONFIGURATION
#define defaultBLE_ATT_DB_CONFIGURATION      (0x10)     // Peripheral Pref. Conn. Parameters present
#endif

/**
 * \brief Default MTU size
 *
 * Default MTU size used on MTU exchange negotiations if one is not set using ble_gap_mtu_size_set().
 */
#ifndef defaultBLE_MTU_SIZE
#define defaultBLE_MTU_SIZE                  (23)
#endif

/**
 * \brief Default channel map (for central role only)
 *
 * Default channel map used when device is configured with the central role if one is not set using
 * ble_gap_channel_map_set().
 */
#ifndef defaultBLE_CHANNEL_MAP
#define defaultBLE_CHANNEL_MAP               {0xFF, 0xFF, 0xFF, 0xFF, 0x1F}  // All channels enabled
#endif

/**
 * \brief Default advertising mode
 *
 * Default mode used for advertising if one is not set using ble_gap_adv_mode_set().
 */
#ifndef defaultBLE_ADVERTISE_MODE
#define defaultBLE_ADVERTISE_MODE            GAP_DISC_MODE_GEN_DISCOVERABLE
#endif

/**
 * \brief Default channels used for advertising
 *
 * Default channel used for advertising if they are not set using ble_gap_adv_chnl_map_set().
 */
#ifndef defaultBLE_ADVERTISE_CHANNEL_MAP
#define defaultBLE_ADVERTISE_CHANNEL_MAP     (GAP_ADV_CHANNEL_37 | GAP_ADV_CHANNEL_38 | GAP_ADV_CHANNEL_39)
#endif

/**
 * \brief Default minimum interval used for advertising
 *
 * Default minimum interval used for advertising in steps of 0.625ms if one is not set using
 * ble_gap_adv_intv_set().
 */
#ifndef defaultBLE_ADVERTISE_INTERVAL_MIN
#define defaultBLE_ADVERTISE_INTERVAL_MIN    (1100)     // = 687.5ms (1100 * 0.625ms)
#endif

/**
 * \brief Default maximum interval used for advertising
 *
 * Default maximum interval used for advertising in steps of 0.625ms if one is not set using
 * ble_gap_adv_intv_set().
 */
#ifndef defaultBLE_ADVERTISE_INTERVAL_MAX
#define defaultBLE_ADVERTISE_INTERVAL_MAX    (1100)     // = 687.5ms (1100 * 0.625ms)
#endif

/**
 * \brief Default filtering policy used for advertising
 *
 * Default filtering policy used for advertising if one is not set using ble_gap_adv_filt_policy_set().
 *
 * \note Whitelist management API is not present in this release, so setting a filtering policy for
 * advertising is not possible.
 */
#ifndef defaultBLE_ADVERTISE_FILTER_POLICY
#define defaultBLE_ADVERTISE_FILTER_POLICY   ADV_ALLOW_SCAN_ANY_CONN_ANY
#endif

/**
 * \brief Default advertising data length
 *
 * Default length of advertising data. This is set to the maximum value allowed by the stack, which
 * is 28 bytes.
 */
#ifndef defaultBLE_ADVERTISE_DATA_LENGTH
#define defaultBLE_ADVERTISE_DATA_LENGTH     (28)
#endif

/**
 * \brief Default advertising Data
 *
 * Default advertising data are set to advertise the device name. If the application
 * should have specific advertising data, these should be set using the ble_gap_adv_data_set().
 *
 * \note Changing #defaultBLE_DEVICE_NAME won't change the device name included by default in the
 * advertising data.
 */
#ifndef defaultBLE_ADVERTISE_DATA
#define defaultBLE_ADVERTISE_DATA            { 0x0C, GAP_DATA_TYPE_LOCAL_NAME, \
                                               'D', 'i', 'a', 'l', 'o', 'g', ' ', 'B', 'L', 'E' }
#endif

/**
 * \brief Default scan response data length
 *
 * Default length of scan response data. This is set to the maximum value allowed by the stack,
 * which is 31 bytes.
 */
#ifndef defaultBLE_SCAN_RESPONSE_DATA_LENGTH
#define defaultBLE_SCAN_RESPONSE_DATA_LENGTH (31)
#endif

/**
 * \brief Default scan response Data
 *
 * Default scan response data are set to zero. If the application should have specific scan response
 * data, these should be set using the ble_gap_adv_data_set().
 *
 * \note Changing #defaultBLE_DEVICE_NAME won't change the device name included by default in the
 * scan response data.
 */
#ifndef defaultBLE_SCAN_RESPONSE_DATA
#define defaultBLE_SCAN_RESPONSE_DATA        { }
#endif

/**
 * \brief Default scan interval
 *
 * Default interval used for scanning in steps of 0.625ms if one is not set using
 * ble_gap_scan_params_set().
  */
#ifndef defaultBLE_SCAN_INTERVAL
#define defaultBLE_SCAN_INTERVAL             (160)        // 100ms (160 * 0.625ms)
#endif

/**
 * \brief Default scan window
 *
 * Default window used for scanning in steps of 0.625ms if one is not set using
 * ble_gap_scan_params_set().
  */
#ifndef defaultBLE_SCAN_WINDOW
#define defaultBLE_SCAN_WINDOW               (80)         // 50ms  (80  * 0.625ms)
#endif

/**
 * \brief Default peripheral preferred minimum connection interval
 *
 * Default minimum connection interval set in the peripheral preferred connection parameters
 * attribute in steps of 1.25ms if one is not set using ble_gap_per_pref_conn_params_set().
  */
#ifndef defaultBLE_PPCP_INTERVAL_MIN
#define defaultBLE_PPCP_INTERVAL_MIN         (8)          // 10ms  (8   * 1.25ms)
#endif

/**
 * \brief Default peripheral preferred maximum connection interval
 *
 * Default maximum connection interval set in the peripheral preferred connection parameters
 * attribute in steps of 1.25ms if one is not set using ble_gap_per_pref_conn_params_set().
  */
#ifndef defaultBLE_PPCP_INTERVAL_MAX
#define defaultBLE_PPCP_INTERVAL_MAX         (16)         // 20ms  (16  * 1.25ms)
#endif

/**
 * \brief Default peripheral preferred slave latency
 *
 * Default slave latency set in the peripheral preferred connection parameters attribute in number
 * of events if one is not set using ble_gap_per_pref_conn_params_set().
  */
#ifndef defaultBLE_PPCP_SLAVE_LATENCY
#define defaultBLE_PPCP_SLAVE_LATENCY        (0)          // number of events
#endif

/**
 * \brief Default peripheral preferred supervision timeout
 *
 * Default supervision timeout set in the peripheral preferred connection parameters attribute in
 * steps of 10ms if one is not set using ble_gap_per_pref_conn_params_set().
  */
#ifndef defaultBLE_PPCP_SUP_TIMEOUT
#define defaultBLE_PPCP_SUP_TIMEOUT          (100)        // 1s    (100 * 10ms)
#endif

/**
 * \brief Default Input/Output capabilities
 *
 * Default input/output capabilities set for pairing procedures if they are not set using the
 * ble_gap_set_io_cap().
 */
#ifndef defaultBLE_GAP_IO_CAP
#define defaultBLE_GAP_IO_CAP                GAP_IO_CAP_NO_INPUT_OUTPUT
#endif

/**
 * \brief Disable Connection Parameter Request procedure
 *
 * If the application has not defined #dg_configBLE_CONN_PARAM_REQ_DISABLE in its custom_config
 * file, this is defined to the default value of 0 to enable the Connection Parameter Request
 * procedure introduced in Bluetooth Core v4.1.
 */
#ifndef dg_configBLE_CONN_PARAM_REQ_DISABLE
#define dg_configBLE_CONN_PARAM_REQ_DISABLE  (0)
#endif


/**
 * \brief Enable Event Counters in BLE ISR
 *
 * If the application has not defined #dg_configBLE_EVENT_COUNTER_ENABLE in its custom_config
 * file, this is defined to the default value of 0 to disable the Event Counters in BLE stack ISR.
 */
#ifndef dg_configBLE_EVENT_COUNTER_ENABLE
#define dg_configBLE_EVENT_COUNTER_ENABLE  (0)
#endif

/**
 * \brief Enable ADV_UNDERRUN workaround
 *
 * If the application has not defined #dg_configBLE_ADV_STOP_DELAY_ENABLE in its custom_config
 * file, this is defined to the default value of 0 to disable the ADV_UNDERRUN workaround in the
 * BLE adapter.
 */
#ifndef dg_configBLE_ADV_STOP_DELAY_ENABLE
#define dg_configBLE_ADV_STOP_DELAY_ENABLE  (0)
#endif

/**
 * \brief Enable use of crypto block from BLE stack code
 *
 * If the application has not defined #dg_configUSE_CRYPTO in its custom_config file, this is
 * defined to the default value of 1 to disable direct access to the crypto block from the BLE stack
 * code.
 */
#ifndef dg_configUSE_CRYPTO
#define dg_configUSE_CRYPTO  (1)
#endif

/**
 * \brief Maximum Receive Data Channel PDU Payload Length
 *
 * If the application has not defined #dg_configBLE_DATA_LENGTH_RX_MAX in its custom_config file,
 * this is defined to the default value for Bluetooth 4.0/4.1, which is 27 octets. In this case, the
 * Data Length Update procedure will not be initiated upon a connection establishment
 * (the controller will not send an LL_LENGTH_REQ PDU to the peer device).
 *
 * \note The controller will still respond to received LL_LENGTH_REQ PDUs with LL_LENGTH_RSP PDUs.
 */
#ifndef dg_configBLE_DATA_LENGTH_RX_MAX
#define dg_configBLE_DATA_LENGTH_RX_MAX  (27)
#endif

/**
 * \brief Maximum Transmit Data Channel PDU Payload Length
 *
 * If the application has not defined #dg_configBLE_DATA_LENGTH_TX_MAX in its custom_config file,
 * this is defined to the default value for Bluetooth 4.0/4.1, which is 27 octets. In this case, the
 * Data Length Update procedure will not be initiated upon a connection establishment
 * (the controller will not send an LL_LENGTH_REQ PDU to the peer device).
 *
 * \note The controller will still respond to received LL_LENGTH_REQ PDUs with LL_LENGTH_RSP PDUs.
 */
#ifndef dg_configBLE_DATA_LENGTH_TX_MAX
#define dg_configBLE_DATA_LENGTH_TX_MAX  (27)
#endif

/**
 * \brief Use BLE sleep mode
 *
 * Macro #USE_BLE_SLEEP controls whether BLE will be set to sleep when it is not needed to be
 * active.
 *
 * \note If the application has enabled the external host configuration by defining
 * BLE_EXTERNAL_HOST, #USE_BLE_SLEEP will be set to 0 because sleep mode is not yet supported in
 * that configuration.
 */
#ifndef BLE_EXTERNAL_HOST
#define USE_BLE_SLEEP           1
#else
#define BLE_STACK_PASSTHROUGH_MODE
#define USE_BLE_SLEEP           0
#endif

/**
 * \brief Wake Up Latency
 *
 * Defines the Wake Up Latency expressed in Low Power clock cycles, that is
 * the number of LP clock cycles needed for the BLE to be fully operational
 * (calculations and BLE timer synchronization)
 *
 */
#ifndef BLE_WUP_LATENCY
#if (dg_configUSE_LP_CLK == LP_CLK_32768) || (dg_configUSE_LP_CLK == LP_CLK_32000)
#ifdef RELEASE_BUILD
        #if (dg_configCODE_LOCATION != NON_VOLATILE_IS_FLASH)   // RAM, OTP
                #define BLE_WUP_LATENCY        8
        #else                                                   // Flash
                #define BLE_WUP_LATENCY        9
        #endif
#else                                                           // !RELEASE_BUILD
        #if (dg_configCODE_LOCATION != NON_VOLATILE_IS_FLASH)
                #define BLE_WUP_LATENCY        16
        #else
                #define BLE_WUP_LATENCY        32
        #endif
#endif // RELEASE_BUILD

#else // RCX

#ifdef RELEASE_BUILD
        #if (dg_configCODE_LOCATION != NON_VOLATILE_IS_FLASH)
                #define BLE_WUP_LATENCY        cm_rcx_us_2_lpcycles(300)
        #else
                #define BLE_WUP_LATENCY        cm_rcx_us_2_lpcycles(300)
        #endif
#else // !RELEASE_BUILD
        #if (dg_configCODE_LOCATION != NON_VOLATILE_IS_FLASH)
                #define BLE_WUP_LATENCY        cm_rcx_us_2_lpcycles(500)
        #else
                #define BLE_WUP_LATENCY        cm_rcx_us_2_lpcycles(1000)
        #endif
#endif // RELEASE_BUILD
#endif
#endif

#endif /* _BLE_CONFIG_H_ */
/**
 \}
 \}
 \}
 */

