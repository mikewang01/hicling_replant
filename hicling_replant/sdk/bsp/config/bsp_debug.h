/**
 * \addtogroup BSP
 * \{
 * \addtogroup BSP_CONFIG
 * \{
 * \addtogroup BSP_DEBUG
 *
 * \brief Doxygen documentation is not yet available for this module.
 *        Please check the source code file(s)
 *
 * \{
*/

/**
 ****************************************************************************************
 *
 * @file bsp_debug.h
 *
 * @brief Board Support Package. Debug Configuration.
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

#ifndef BSP_DEBUG_H_
#define BSP_DEBUG_H_


/* --------------------------------- DEBUG GPIO handling macros --------------------------------- */

#define DBG_CONFIGURE(flag, name, func)                         \
{                                                               \
        if (flag == 1) {                                        \
                name##_MODE_REG = 0x300 + func;                 \
        }                                                       \
}

#define DBG_CONFIGURE_HIGH(flag, name)                          \
{                                                               \
        if (flag == 1) {                                        \
                name##_SET_REG = name##_PIN;                    \
                name##_MODE_REG = 0x300;                        \
        }                                                       \
}

#define DBG_CONFIGURE_LOW(flag, name)                           \
{                                                               \
        if (flag == 1) {                                        \
                name##_RESET_REG = name##_PIN;                  \
                name##_MODE_REG = 0x300;                        \
        }                                                       \
}

#define DBG_SET_HIGH(flag, name)                                \
{                                                               \
        if (flag == 1) {                                        \
                name##_SET_REG = name##_PIN;                    \
        }                                                       \
}

#define DBG_SET_LOW(flag, name)                                 \
{                                                               \
        if (flag == 1) {                                        \
                name##_RESET_REG = name##_PIN;                  \
        }                                                       \
}

/* ---------------------------------------------------------------------------------------------- */


/* ---------------------------------- HardFault or NMI event ------------------------------------ */

#ifndef EXCEPTION_DEBUG
#define EXCEPTION_DEBUG                         (0)
#endif

/* ---------------------------------------------------------------------------------------------- */


/* --------------------------------- Clock and Power Manager ------------------------------------ */

#ifndef CPM_DEBUG
#define CPM_DEBUG                               (0)
#endif

#ifndef CPM_USE_FUNCTIONAL_DEBUG
#define CPM_USE_FUNCTIONAL_DEBUG                (0)     // Requires GPIO config.
#endif

#ifndef CPM_USE_TIMING_DEBUG
#define CPM_USE_TIMING_DEBUG                    (0)     // Requires GPIO config.
#endif

#ifndef CPM_USE_RCX_DEBUG
#define CPM_USE_RCX_DEBUG                       (0)     // Requires logging.
#endif

/* Controls which RAM blocks will be retained when the MEASURE_SLEEP_CURRENT test mode is used
 * (optional). */
#ifndef dg_configTESTMODE_RETAIN_RAM
#define dg_configTESTMODE_RETAIN_RAM            (0x1F)
#endif

/* Controls whether the Cache will be retained when the MEASURE_SLEEP_CURRENT test mode is used
 * (optional). */
#ifndef dg_configTESTMODE_RETAIN_CACHE
#define dg_configTESTMODE_RETAIN_CACHE          (0)
#endif

/* Controls whether the ECC RAM will be retained when the MEASURE_SLEEP_CURRENT test mode is used
 * (optional). */
#ifndef dg_config_TESTMODE_RETAIN_ECCRAM
#define dg_config_TESTMODE_RETAIN_ECCRAM        (0)
#endif

/* ---------------------------------------------------------------------------------------------- */


/* --------------------------------------- USB Charger ------------------------------------------ */

#ifndef DEBUG_USB_CHARGER
#define DEBUG_USB_CHARGER                       (0)
#endif

#ifndef DEBUG_USB_CHARGER_FSM
#define DEBUG_USB_CHARGER_FSM                   (0)
#endif

#ifndef USB_CHARGER_TIMING_DEBUG
#define USB_CHARGER_TIMING_DEBUG                (0)
#endif

/* ---------------------------------------------------------------------------------------------- */


/* ------------------------------------------- BLE ---------------------------------------------- */

#ifndef BLE_USE_TIMING_DEBUG
#define BLE_USE_TIMING_DEBUG                    (0)     // Requires GPIO config.
#endif

#ifndef BLE_ADAPTER_DEBUG
#define BLE_ADAPTER_DEBUG                       (0)     // Requires GPIO config.
#endif

#ifndef BLE_RX_EN_DEBUG
#define BLE_RX_EN_DEBUG                         (0)     // Requires GPIO config.
#endif

#define BLE_RX_EN_FUNC                          (57)

#ifndef BLE_WINDOW_STATISTICS
#define BLE_WINDOW_STATISTICS                   (0)
#endif

#ifndef BLE_SLEEP_PERIOD_DEBUG
#define BLE_SLEEP_PERIOD_DEBUG                  (0)     // Requires logging and window statistics.
#endif

#ifndef BLE_WAKEUP_MONITOR_PERIOD
#define BLE_WAKEUP_MONITOR_PERIOD               (1024)
#endif

#ifndef BLE_MAX_MISSES_ALLOWED
#define BLE_MAX_MISSES_ALLOWED                  (0)
#endif

#ifndef BLE_MAX_DELAYS_ALLOWED
#define BLE_MAX_DELAYS_ALLOWED                  (0)
#endif

/* ---------------------------------------------------------------------------------------------- */

/* ---------------------------------------------------------------------------------------------- */

/* ------------------------------------------ Common -------------------------------------------- */
#ifndef CMN_TIMING_DEBUG
#define CMN_TIMING_DEBUG                       (0)     // Requires GPIO config.
#endif
/* ---------------------------------------------------------------------------------------------- */


/* ------------------------------------ GPIO configuration -------------------------------------- */

/* Enable/Disable GPIO pin assignment conflict detection
 */
#define DEBUG_GPIO_ALLOC_MONITOR_ENABLED     (0)


/* Exception handling debug configuration
 *
 */
#if (EXCEPTION_DEBUG == 0)
// Exception occurred (initial configuration: low)
#define EXCEPTIONDBG_MODE_REG                   *(volatile int *)0x20000000
#define EXCEPTIONDBG_SET_REG                    *(volatile int *)0x20000000
#define EXCEPTIONDBG_RESET_REG                  *(volatile int *)0x20000000
#define EXCEPTIONDBG_PIN                        (0)

#else

// Tick (P3[0])
#define EXCEPTIONDBG_MODE_REG                   GPIO->P30_MODE_REG
#define EXCEPTIONDBG_SET_REG                    GPIO->P3_SET_DATA_REG
#define EXCEPTIONDBG_RESET_REG                  GPIO->P3_RESET_DATA_REG
#define EXCEPTIONDBG_PIN                        (1 << 0)

#endif

/* Functional debug configuration
 *
 * Note that GPIO overlapping is allowed if the tracked events are discrete and the initial GPIO
 * configuration is the same! No checking is performed for erroneous configuration though!
 *
 */
#if (CPM_USE_FUNCTIONAL_DEBUG == 0)
// Tick (initial configuration: low)
#define CPMDBG_TICK_MODE_REG                    *(volatile int *)0x20000000
#define CPMDBG_TICK_SET_REG                     *(volatile int *)0x20000000
#define CPMDBG_TICK_RESET_REG                   *(volatile int *)0x20000000
#define CPMDBG_TICK_PIN                         (0)

// External Wake-up (initial configuration: low)
#define CPMDBG_EXT_WKUP_MODE_REG                *(volatile int *)0x20000000
#define CPMDBG_EXT_WKUP_SET_REG                 *(volatile int *)0x20000000
#define CPMDBG_EXT_WKUP_RESET_REG               *(volatile int *)0x20000000
#define CPMDBG_EXT_WKUP_PIN                     (0)

// Active / Sleep (initial configuration: high)
#define CPMDBG_POWERUP_MODE_REG                 *(volatile int *)0x20000000
#define CPMDBG_POWERUP_SET_REG                  *(volatile int *)0x20000000
#define CPMDBG_POWERUP_RESET_REG                *(volatile int *)0x20000000
#define CPMDBG_POWERUP_PIN                      (0)

#else

// Tick (P2[3])
#define CPMDBG_TICK_MODE_REG                    GPIO->P23_MODE_REG
#define CPMDBG_TICK_SET_REG                     GPIO->P2_SET_DATA_REG
#define CPMDBG_TICK_RESET_REG                   GPIO->P2_RESET_DATA_REG
#define CPMDBG_TICK_PIN                         (1 << 3)

// External Wake-up (P3[0])
#define CPMDBG_EXT_WKUP_MODE_REG                GPIO->P30_MODE_REG
#define CPMDBG_EXT_WKUP_SET_REG                 GPIO->P3_SET_DATA_REG
#define CPMDBG_EXT_WKUP_RESET_REG               GPIO->P3_RESET_DATA_REG
#define CPMDBG_EXT_WKUP_PIN                     (1 << 0)

// Active / Sleep (P1[4])
#define CPMDBG_POWERUP_MODE_REG                 GPIO->P14_MODE_REG
#define CPMDBG_POWERUP_SET_REG                  GPIO->P1_SET_DATA_REG
#define CPMDBG_POWERUP_RESET_REG                GPIO->P1_RESET_DATA_REG
#define CPMDBG_POWERUP_PIN                      (1 << 4)
#endif

/* Timing debug configuration
 *
 * Note that in this mode the pad latches are removed immediately after the execution resumes from
 * the __WFI(). Because of this, it is not advised to use this feature in projects that use GPIOS.
 * Nevertheless, in case it is used, make sure that the "peripheral initialization" is also done
 * at that point, modifying sys_power_mgr.c accordingly.
 *
 * Note also that GPIO overlapping is allowed if the tracked events are discrete and the initial
 * GPIO configuration is the same! No checking is performed for erroneous configuration though!
 *
 */
#if (CPM_USE_TIMING_DEBUG == 0)
// CPM: sleep or idle entry (until __WFI() is called) (initial configuration: high)
#define CPMDBG_SLEEP_ENTER_MODE_REG             *(volatile int *)0x20000000
#define CPMDBG_SLEEP_ENTER_SET_REG              *(volatile int *)0x20000000
#define CPMDBG_SLEEP_ENTER_RESET_REG            *(volatile int *)0x20000000
#define CPMDBG_SLEEP_ENTER_PIN                  (0)

// CPM: sleep or idle exit (initial configuration: low)
#define CPMDBG_SLEEP_EXIT_MODE_REG              *(volatile int *)0x20000000
#define CPMDBG_SLEEP_EXIT_SET_REG               *(volatile int *)0x20000000
#define CPMDBG_SLEEP_EXIT_RESET_REG             *(volatile int *)0x20000000
#define CPMDBG_SLEEP_EXIT_PIN                   (0)

// Low clocks (initial configuration: low)
#define CPMDBG_LOWER_CLOCKS_MODE_REG            *(volatile int *)0x20000000
#define CPMDBG_LOWER_CLOCKS_SET_REG             *(volatile int *)0x20000000
#define CPMDBG_LOWER_CLOCKS_RESET_REG           *(volatile int *)0x20000000
#define CPMDBG_LOWER_CLOCKS_PIN                 (0)

// XTAL16M settling (initial configuration: low)
#define CPMDBG_XTAL16M_SETTLED_MODE_REG         *(volatile int *)0x20000000
#define CPMDBG_XTAL16M_SETTLED_SET_REG          *(volatile int *)0x20000000
#define CPMDBG_XTAL16M_SETTLED_RESET_REG        *(volatile int *)0x20000000
#define CPMDBG_XTAL16M_SETTLED_PIN              (0)

#else

// CPM: sleep or idle entry (until __WFI() is called) (P1[7])
#define CPMDBG_SLEEP_ENTER_MODE_REG             GPIO->P17_MODE_REG
#define CPMDBG_SLEEP_ENTER_SET_REG              GPIO->P1_SET_DATA_REG
#define CPMDBG_SLEEP_ENTER_RESET_REG            GPIO->P1_RESET_DATA_REG
#define CPMDBG_SLEEP_ENTER_PIN                  (1 << 7)

// CPM: sleep or idle exit (P1[6])
#define CPMDBG_SLEEP_EXIT_MODE_REG              GPIO->P16_MODE_REG
#define CPMDBG_SLEEP_EXIT_SET_REG               GPIO->P1_SET_DATA_REG
#define CPMDBG_SLEEP_EXIT_RESET_REG             GPIO->P1_RESET_DATA_REG
#define CPMDBG_SLEEP_EXIT_PIN                   (1 << 6)

// Low clocks (P1[5])
#define CPMDBG_LOWER_CLOCKS_MODE_REG            GPIO->P15_MODE_REG
#define CPMDBG_LOWER_CLOCKS_SET_REG             GPIO->P1_SET_DATA_REG
#define CPMDBG_LOWER_CLOCKS_RESET_REG           GPIO->P1_RESET_DATA_REG
#define CPMDBG_LOWER_CLOCKS_PIN                 (1 << 5)

// XTAL16M settling (P1[4])
#define CPMDBG_XTAL16M_SETTLED_MODE_REG         GPIO->P14_MODE_REG
#define CPMDBG_XTAL16M_SETTLED_SET_REG          GPIO->P1_SET_DATA_REG
#define CPMDBG_XTAL16M_SETTLED_RESET_REG        GPIO->P1_RESET_DATA_REG
#define CPMDBG_XTAL16M_SETTLED_PIN              (1 << 4)
#endif

#if (BLE_USE_TIMING_DEBUG == 0)
// BLE LP & GEN irqs (initial configuration: low)
#define CPMDBG_BLE_IRQ_MODE_REG                 *(volatile int *)0x20000000
#define CPMDBG_BLE_IRQ_SET_REG                  *(volatile int *)0x20000000
#define CPMDBG_BLE_IRQ_RESET_REG                *(volatile int *)0x20000000
#define CPMDBG_BLE_IRQ_PIN                      (0)

// BLE sleep prepare (final stage) (initial configuration: low)
#define CPMDBG_BLE_SLEEP_ENTRY_MODE_REG         *(volatile int *)0x20000000
#define CPMDBG_BLE_SLEEP_ENTRY_SET_REG          *(volatile int *)0x20000000
#define CPMDBG_BLE_SLEEP_ENTRY_RESET_REG        *(volatile int *)0x20000000
#define CPMDBG_BLE_SLEEP_ENTRY_PIN              (0)

// BLE LP irq signal (diagnostics) (initial configuration: unknown)
#define CPMDBG_BLE_LP_IRQ_MODE_REG              *(volatile int *)0x20000000

#else

// BLE LP & GEN ISRs (P1[4])
#define CPMDBG_BLE_IRQ_MODE_REG                 GPIO->P14_MODE_REG
#define CPMDBG_BLE_IRQ_SET_REG                  GPIO->P1_SET_DATA_REG
#define CPMDBG_BLE_IRQ_RESET_REG                GPIO->P1_RESET_DATA_REG
#define CPMDBG_BLE_IRQ_PIN                      (1 << 4)

// BLE sleep prepare (final stage) (P1[4])
#define CPMDBG_BLE_SLEEP_ENTRY_MODE_REG         GPIO->P14_MODE_REG
#define CPMDBG_BLE_SLEEP_ENTRY_SET_REG          GPIO->P1_SET_DATA_REG
#define CPMDBG_BLE_SLEEP_ENTRY_RESET_REG        GPIO->P1_RESET_DATA_REG
#define CPMDBG_BLE_SLEEP_ENTRY_PIN              (1 << 4)

// BLE LP irq signal (diagnostics) (P2[3])
#define CPMDBG_BLE_LP_IRQ_MODE_REG              GPIO->P23_MODE_REG
#endif

#if (BLE_ADAPTER_DEBUG == 0)
#define BLEBDG_ADAPTER_MODE_REG                 *(volatile int *)0x20000000
#define BLEBDG_ADAPTER_SET_REG                  *(volatile int *)0x20000000
#define BLEBDG_ADAPTER_RESET_REG                *(volatile int *)0x20000000
#define BLEBDG_ADAPTER_PIN                      (0)

#else

#define BLEBDG_ADAPTER_MODE_REG                 GPIO->P30_MODE_REG
#define BLEBDG_ADAPTER_SET_REG                  GPIO->P3_SET_DATA_REG
#define BLEBDG_ADAPTER_RESET_REG                GPIO->P3_RESET_DATA_REG
#define BLEBDG_ADAPTER_PIN                      (1 << 0)
#endif

#if (BLE_RX_EN_DEBUG == 0)
#define BLEBDG_RXEN_MODE_REG                    *(volatile int *)0x20000000
#define BLEBDG_RXEN_SET_REG                     *(volatile int *)0x20000000
#define BLEBDG_RXEN_RESET_REG                   *(volatile int *)0x20000000
#define BLEBDG_RXEN_PIN                         (0)

#else

#define BLEBDG_RXEN_MODE_REG                    GPIO->P12_MODE_REG
#define BLEBDG_RXEN_SET_REG                     GPIO->P1_SET_DATA_REG
#define BLEBDG_RXEN_RESET_REG                   GPIO->P1_RESET_DATA_REG
#define BLEBDG_RXEN_PIN                         (1 << 2)
#endif

#if (USB_CHARGER_TIMING_DEBUG == 0)
// CHRG: Inside critical section (initial configuration: low)
#define CHRGDBG_CRITICAL_SECTION_MODE_REG       *(volatile int *)0x20000000
#define CHRGDBG_CRITICAL_SECTION_SET_REG        *(volatile int *)0x20000000
#define CHRGDBG_CRITICAL_SECTION_RESET_REG      *(volatile int *)0x20000000
#define CHRGDBG_CRITICAL_SECTION_PIN            (0)

// CHRG: FSM task (initial configuration: low)
#define CHRGDBG_FSM_TASK_MODE_REG               *(volatile int *)0x20000000
#define CHRGDBG_FSM_TASK_SET_REG                *(volatile int *)0x20000000
#define CHRGDBG_FSM_TASK_RESET_REG              *(volatile int *)0x20000000
#define CHRGDBG_FSM_TASK_PIN                    (0)

// CHRG: Control task (initial configuration: low)
#define CPMDBG_CONTROL_TASK_MODE_REG            *(volatile int *)0x20000000
#define CPMDBG_CONTROL_TASK_SET_REG             *(volatile int *)0x20000000
#define CPMDBG_CONTROL_TASK_RESET_REG           *(volatile int *)0x20000000
#define CPMDBG_CONTROL_TASK_PIN                 (0)

#else

// CHRG: Inside critical section (P3[2])
#define CHRGDBG_CRITICAL_SECTION_MODE_REG       GPIO->P32_MODE_REG
#define CHRGDBG_CRITICAL_SECTION_SET_REG        GPIO->P3_SET_DATA_REG
#define CHRGDBG_CRITICAL_SECTION_RESET_REG      GPIO->P3_RESET_DATA_REG
#define CHRGDBG_CRITICAL_SECTION_PIN            (1 << 2)

// CHRG: FSM task (P3[3])
#define CHRGDBG_FSM_TASK_MODE_REG               GPIO->P33_MODE_REG
#define CHRGDBG_FSM_TASK_SET_REG                GPIO->P3_SET_DATA_REG
#define CHRGDBG_FSM_TASK_RESET_REG              GPIO->P3_RESET_DATA_REG
#define CHRGDBG_FSM_TASK_PIN                    (1 << 3)

// CHRG: Control task (P3[4])
#define CPMDBG_CONTROL_TASK_MODE_REG            GPIO->P34_MODE_REG
#define CPMDBG_CONTROL_TASK_SET_REG             GPIO->P3_SET_DATA_REG
#define CPMDBG_CONTROL_TASK_RESET_REG           GPIO->P3_RESET_DATA_REG
#define CPMDBG_CONTROL_TASK_PIN                 (1 << 4)

#endif

#if (CMN_TIMING_DEBUG == 0)
// Common: Inside critical section (initial configuration: low)
#define CMNDBG_CRITICAL_SECTION_MODE_REG       *(volatile int *)0x20000000
#define CMNDBG_CRITICAL_SECTION_SET_REG        *(volatile int *)0x20000000
#define CMNDBG_CRITICAL_SECTION_RESET_REG      *(volatile int *)0x20000000
#define CMNDBG_CRITICAL_SECTION_PIN            (0)

#else

// Common: Inside critical section (P4[6])
#define CMNDBG_CRITICAL_SECTION_MODE_REG        GPIO->P40_MODE_REG
#define CMNDBG_CRITICAL_SECTION_SET_REG         GPIO->P4_SET_DATA_REG
#define CMNDBG_CRITICAL_SECTION_RESET_REG       GPIO->P4_RESET_DATA_REG
#define CMNDBG_CRITICAL_SECTION_PIN             (1 << 0)

#endif


/* Enables the logging of stack (RW) heap memories usage.
 *
 * The feature shall only be enabled in development/debug mode
 */
#ifndef dg_configLOG_BLE_STACK_MEM_USAGE
#define dg_configLOG_BLE_STACK_MEM_USAGE                (0)
#endif

#endif /* BSP_DEBUG_H_ */

/**
\}
\}
\}
*/
