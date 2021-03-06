/**
\addtogroup BSP
\{
\addtogroup CONFIG
\{
\addtogroup CUSTOM
\{
*/

/**
****************************************************************************************
*
* @file custom_config_ram.h
*
* @brief Board Support Package. User Configuration file for execution from RAM.
*
* Copyright (C) 2015. Dialog Semiconductor Ltd, unpublished work. This computer
* program includes Confidential, Proprietary Information and is a Trade Secret of
* Dialog Semiconductor Ltd. All use, disclosure, and/or reproduction is prohibited
* unless authorized in writing. All Rights Reserved.
*
* <black.orca.support@diasemi.com> and contributors.
*
****************************************************************************************
*/

#ifndef CUSTOM_CONFIG_RAM_H_
#define CUSTOM_CONFIG_RAM_H_

#include "bsp_definitions.h"

#define CONFIG_USE_BLE


#define __HEAP_SIZE  0x800
#define __STACK_SIZE 0x800

#define dg_configGPADC_ADAPTER                  1
#define dg_configUART_BLE_ADAPTER               1

#define dg_configPOWER_CONFIG                   (POWER_CONFIGURATION_2)

#define dg_configUSE_LP_CLK                     LP_CLK_32768
#define dg_configCODE_LOCATION                  NON_VOLATILE_IS_NONE
#define dg_configEXT_CRYSTAL_FREQ               EXT_CRYSTAL_IS_16M

#define dg_configIMAGE_SETUP                    DEVELOPMENT_MODE
#define dg_configEMULATE_OTP_COPY               (0)

#define dg_configUSER_CAN_USE_TIMER1            (0)

#define dg_configMEM_RETENTION_MODE             (0x1F)
#define dg_configMEM_RETENTION_MODE_PRESERVE_IMAGE       (0x1F)
#define dg_configSHUFFLING_MODE                 (0x3)

#define dg_configUSE_WDOG                       (0)

#define dg_configUSE_DCDC                       (1)

#define dg_configPOWER_FLASH                    (1)
#define dg_configFLASH_POWER_DOWN               (0)
#define dg_configFLASH_POWER_OFF                (0)

#define dg_configBATTERY_TYPE                   (BATTERY_TYPE_LIMN2O4)
#define dg_configBATTERY_CHARGE_CURRENT         2       // 30mA
#define dg_configBATTERY_PRECHARGE_CURRENT      20      // 2.1mA
#define dg_configBATTERY_CHARGE_NTC             1       // disabled

#define dg_configUSE_USB_CHARGER                0
#define dg_configALLOW_CHARGING_NOT_ENUM        1

#define dg_configUSE_ProDK                      (1)
#define dg_configUSE_HW_TRNG                    (1)

#define dg_configUSE_SW_CURSOR                  (1)

#define dg_configCONFIG_HEADER_IN_FLASH         0

/*************************************************************************************************\
 * FreeRTOS specific config
 */
#define OS_FREERTOS                              /* Define this to use FreeRTOS */
#define configTOTAL_HEAP_SIZE                    12000   /* This is the FreeRTOS Total Heap Size */

/*************************************************************************************************\
 * BLE specific config
 */
#define BLE_EXTERNAL_HOST
#define BLE_PROD_TEST
#define APP_SPECIFIC_HCI_ENABLE 1
#define APP_SPECIFIC_HCI_SEND ad_uart_ble_app_send

/*************************************************************************************************\
 * Peripheral specific config
 */


/* Include bsp default values */
#include "bsp_defaults.h"

#endif /* CUSTOM_CONFIG_RAM_H_ */

/**
\}
\}
\}
*/
