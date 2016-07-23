/**
 * \addtogroup BSP
 * \{
 * \addtogroup BSP_CONFIG
 * \{
 * \addtogroup RF_FEM
 *
 * \brief RF Front-End module confguration
 *
 *\{
 */

/**
****************************************************************************************
*
* @file bsp_fem.h
*
* @brief Board Support Package. RF Front-End Module definitions.
*
* Copyright (C) 2016. Dialog Semiconductor Ltd, unpublished work. This computer
* program includes Confidential, Proprietary Information and is a Trade Secret of
* Dialog Semiconductor Ltd. All use, disclosure, and/or reproduction is prohibited
* unless authorized in writing. All Rights Reserved.
*
* <black.orca.support@diasemi.com> and contributors.
*
****************************************************************************************
*/

#ifndef BSP_FEM_H_
#define BSP_FEM_H_

/* ------------------------------- RF FEM driver (SKY66112-11) configuration settings ----------- */

#ifdef dg_configFEM_DLG_REF_BOARD

#undef dg_configBLACK_ORCA_IC_STEP
#define dg_configBLACK_ORCA_IC_STEP             BLACK_ORCA_IC_STEP_D

#define dg_configFEM FEM_SKY66112_11

#if dg_configPOWER_EXT_1V8_PERIPHERALS == 0
#warning Setting dg_configPOWER_EXT_1V8_PERIPHERALS to 1 for FEM to work
#define dg_configPOWER_EXT_1V8_PERIPHERALS      1
#endif

#define dg_configFEM_SKY66112_11_FEM_BIAS_V18P
#define dg_configFEM_SKY66112_11_FEM_BIAS2_V18

#ifndef dg_configFEM_SKY66112_11_CSD_PORT
#define dg_configFEM_SKY66112_11_CSD_PORT HW_GPIO_PORT_4
#endif

#ifndef dg_configFEM_SKY66112_11_CSD_PIN
#define dg_configFEM_SKY66112_11_CSD_PIN HW_GPIO_PIN_2
#endif

#ifndef dg_configFEM_SKY66112_11_CPS_PORT
#define dg_configFEM_SKY66112_11_CPS_PORT HW_GPIO_PORT_4
#endif

#ifndef dg_configFEM_SKY66112_11_CPS_PIN
#define dg_configFEM_SKY66112_11_CPS_PIN HW_GPIO_PIN_7
#endif

#ifndef dg_configFEM_SKY66112_11_CRX_PORT
#define dg_configFEM_SKY66112_11_CRX_PORT HW_GPIO_PORT_4
#endif

#ifndef dg_configFEM_SKY66112_11_CRX_PIN
#define dg_configFEM_SKY66112_11_CRX_PIN HW_GPIO_PIN_4
#endif

#ifndef dg_configFEM_SKY66112_11_CTX_PORT
#define dg_configFEM_SKY66112_11_CTX_PORT HW_GPIO_PORT_4
#endif

#ifndef dg_configFEM_SKY66112_11_CTX_PIN
#define dg_configFEM_SKY66112_11_CTX_PIN HW_GPIO_PIN_6
#endif

#ifndef dg_configFEM_SKY66112_11_CHL_PORT
#define dg_configFEM_SKY66112_11_CHL_PORT HW_GPIO_PORT_4
#endif

#ifndef dg_configFEM_SKY66112_11_CHL_PIN
#define dg_configFEM_SKY66112_11_CHL_PIN HW_GPIO_PIN_5
#endif

#ifndef dg_configFEM_SKY66112_11_ANTSEL_PORT
#define dg_configFEM_SKY66112_11_ANTSEL_PORT HW_GPIO_PORT_4
#endif

#ifndef dg_configFEM_SKY66112_11_ANTSEL_PIN
#define dg_configFEM_SKY66112_11_ANTSEL_PIN HW_GPIO_PIN_3
#endif

#endif /* dg_configFEM_DLG_REF_BOARD */

#ifndef dg_configFEM
#define dg_configFEM FEM_NOFEM
#endif

#if dg_configFEM == FEM_SKY66112_11
#ifndef dg_configFEM_SKY66112_11_CSD_USE_DCF
#define dg_configFEM_SKY66112_11_CSD_USE_DCF 1
#endif

#ifndef dg_configFEM_SKY66112_11_TXSET_DCF
#define dg_configFEM_SKY66112_11_TXSET_DCF 47
#endif

#ifndef dg_configFEM_SKY66112_11_TXRESET_DCF
#define dg_configFEM_SKY66112_11_TXRESET_DCF 0
#endif

#ifndef dg_configFEM_SKY66112_11_RXSET_DCF
#define dg_configFEM_SKY66112_11_RXSET_DCF 1
#endif

#ifndef dg_configFEM_SKY66112_11_RXRESET_DCF
#define dg_configFEM_SKY66112_11_RXRESET_DCF 20
#endif
#endif /* dg_configFEM == FEM_SKY66112_11 */

#endif /* BSP_FEM_H_ */

/**
\}
\}
\}
*/

