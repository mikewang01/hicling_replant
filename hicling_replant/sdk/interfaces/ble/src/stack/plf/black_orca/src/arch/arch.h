/**
 ****************************************************************************************
 *
 * @file arch.h
 *
 * @brief This file contains the definitions of the macros and functions that are
 * architecture dependent.  The implementation of those is implemented in the
 * appropriate architecture directory.
 *
 * Copyright (C) RivieraWaves 2009-2014
 *
 *
 ****************************************************************************************
 */


#ifndef _ARCH_H_
#define _ARCH_H_

/**
 ****************************************************************************************
 * @defgroup REFIP
 * @brief Reference IP Platform
 *
 * This module contains reference platform components - REFIP.
 *
 *
 * @{
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @defgroup DRIVERS
 * @ingroup REFIP
 * @brief Reference IP Platform Drivers
 *
 * This module contains the necessary drivers to run the platform with the
 * RW BT SW protocol stack.
 *
 * This has the declaration of the platform architecture API.
 *
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>             // standard integer definition
#include "compiler.h"           // inline functions
#include "user_config_defs.h"

#include "core_cm0.h"
#include "system_DA14680.h"

/*
 * CPU WORD SIZE
 ****************************************************************************************
 */
/// ARM is a 32-bit CPU
#define CPU_WORD_SIZE           4

/*
 * CPU Endianness
 ****************************************************************************************
 */
/// ARM is little endian
#define CPU_LE                  1

/*
 * DEBUG configuration
 ****************************************************************************************
 */
#if defined(CFG_DBG)
#define PLF_DEBUG               1
#else //CFG_DBG
#define PLF_DEBUG               0
#endif //CFG_DBG

/*
 * NVDS
 ****************************************************************************************
 */

/// NVDS
#ifdef CFG_NVDS
#define PLF_NVDS                1
#else // CFG_NVDS
#define PLF_NVDS                0
#endif // CFG_NVDS

/*
 * LLD ROM defines
 ****************************************************************************************
 */
struct lld_sleep_env_tag
{
        uint32_t irq_mask;
};

/*
 * UART
 ****************************************************************************************
 */

/// UART
#define PLF_UART                1

/*
 * DEFINES
 ****************************************************************************************
 */
/*
 * Deep sleep threshold. Application specific. Control if during deep sleep the system RAM will be powered off and if OTP copy will be required.
 ****************************************************************************************
*/		
//#if (DEEP_SLEEP)
/// Sleep Duration Value in periodic wake-up mode
#define MAX_SLEEP_DURATION_PERIODIC_WAKEUP_DEF          0x0320  // 0.5s
/// Sleep Duration Value in external wake-up mode
#define MAX_SLEEP_DURATION_EXTERNAL_WAKEUP_DEF          0x3E80  // 10s
//#endif //DEEP_SLEEP

/// Possible errors detected by FW
#define RESET_NO_ERROR          0x00000000
#define RESET_MEM_ALLOC_FAIL    0xF2F2F2F2

/// Reset platform and stay in ROM
#define RESET_TO_ROM            0xA5A5A5A5

/// Reset platform and reload FW
#define RESET_AND_LOAD_FW       0xC3C3C3C3

/*
 * EXPORTED FUNCTION DECLARATION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Compute size of SW stack used.
 *
 * This function is compute the maximum size stack used by SW.
 *
 * @return Size of stack used (in bytes)
 ****************************************************************************************
 */
uint16_t get_stack_usage(void);

/**
 ****************************************************************************************
 * @brief Re-boot FW.
 *
 * This function is used to re-boot the FW when error has been detected, it is the end of
 * the current FW execution.
 * After waiting transfers on UART to be finished, and storing the information that
 * FW has re-booted by itself in a non-loaded area, the FW restart by branching at FW
 * entry point.
 *
 * Note: when calling this function, the code after it will not be executed.
 *
 * @param[in] error      Error detected by FW
 ****************************************************************************************
 */
void platform_reset(uint32_t error);

//#if PLF_DEBUG
///**
// ****************************************************************************************
// * @brief Print the assertion error reason and loop forever.
// *
// * @param condition C string containing the condition.
// * @param file C string containing file where the assertion is located.
// * @param line Line number in the file where the assertion is located.
// ****************************************************************************************
// */
//void assert_err(const char *condition, const char * file, int line);
//
///**
// ****************************************************************************************
// * @brief Print the assertion error reason and loop forever.
// * The parameter value that is causing the assertion will also be disclosed.
// *
// * @param param parameter value that is caused the assertion.
// * @param file C string containing file where the assertion is located.
// * @param line Line number in the file where the assertion is located.
// ****************************************************************************************
// */
//void assert_param(int param0, int param1, const char * file, int line);
//
///**
// ****************************************************************************************
// * @brief Print the assertion warning reason.
// *
// * @param condition C string containing the condition.
// * @param file C string containing file where the assertion is located.
// * @param line Line number in the file where the assertion is located.
// ****************************************************************************************
// */
//void assert_warn(const char *condition, const char * file, int line);
//#endif //PLF_DEBUG


/*
 * ASSERTION CHECK
 ****************************************************************************************
 */
/// Assertions showing a critical error that could require a full system reset
#define ASSERT_ERR(cond) 

/// Assertions showing a critical error that could require a full system reset
#define ASSERT_INFO(cond, param0, param1)

/// Assertions showing a non-critical problem that has to be fixed by the SW
#define ASSERT_WARN(cond)

/* Hooks
 *
 * Override modifying the relevant "hook entry" like in the example below:
 *
 * unsigned char my_presleepcheck(void);
 *
 * #define romBLE_HOOK_custom_presleepcheck             1
 * #define romBLE_HOOK_custom_presleepcheck_func        my_presleepcheck
 *
 */
unsigned char (*custom_preinit)(void);                  // function called at the start of arch_main
unsigned char (*custom_postinit)(void);                 // function called after BLE stack init
unsigned char (*custom_appinit)(void);                  // function called after BLE stack init to initialise custom applications
unsigned char (*custom_preloop)(void);                  // function called before main loop
unsigned char (*custom_preschedule)(void);              // function called before BLE stack schedule
unsigned char (*custom_postschedule)(void);             // function called after BLE stack schedule
unsigned char (*custom_postschedule_async)(void);       // function called after BLE stack schedule
unsigned char (*custom_presleepcheck)(void);            // function called before BLE sleep check
unsigned char (*custom_appsleepset)(void *);            // function called to allow app to specify sleep mode (EXT/DEEP). default DEEP
unsigned char (*custom_postsleepcheck)(void *);         // function called after BLE sleep check
unsigned char (*custom_presleepenter)(void *);          // function called before BLE sleep enter
unsigned char (*custom_postsleepexit)(void *);          // function called after BLE sleep exit
unsigned char (*custom_prewakeup)(void);                // function called at the start of wakeup_lp handler
unsigned char (*custom_postwakeup)(void);               // function called at the end of wakeup_lp handler
unsigned char (*custom_preidlecheck)(void);             // function called before WFI in idle more
unsigned char (*custom_pti_set)(void);                  // function called when pti_setf macro is called in order to update our arbiter

#define romBLE_HOOK_custom_preinit                      0
#define romBLE_HOOK_custom_preinit_func                 custom_preinit

#define romBLE_HOOK_custom_postinit                     0
#define romBLE_HOOK_custom_postinit_func                custom_postinit

#define romBLE_HOOK_custom_appinit                      0
#define romBLE_HOOK_custom_appinit_func                 custom_appinit

#define romBLE_HOOK_custom_preloop                      0
#define romBLE_HOOK_custom_preloop_func                 custom_preloop

#define romBLE_HOOK_custom_preschedule                  0
#define romBLE_HOOK_custom_preschedule_func             custom_preschedule

#define romBLE_HOOK_custom_postschedule                 0
#define romBLE_HOOK_custom_postschedule_func            custom_postschedule

#define romBLE_HOOK_custom_postschedule_async           0
#define romBLE_HOOK_custom_postschedule_async_func      custom_postschedule_async

#define romBLE_HOOK_custom_presleepcheck                0
#define romBLE_HOOK_custom_presleepcheck_func           custom_presleepcheck

#define romBLE_HOOK_custom_appsleepset                  0
#define romBLE_HOOK_custom_appsleepset_func             custom_appsleepset

#define romBLE_HOOK_custom_postsleepcheck               0
#define romBLE_HOOK_custom_postsleepcheck_func          custom_postsleepcheck

#define romBLE_HOOK_custom_presleepenter                0
#define romBLE_HOOK_custom_presleepenter_func           custom_presleepenter

#define romBLE_HOOK_custom_postsleepexit                0
#define romBLE_HOOK_custom_postsleepexit_func           custom_postsleepexit

#define romBLE_HOOK_custom_prewakeup                    0
#define romBLE_HOOK_custom_prewakeup_func               custom_prewakeup

#define romBLE_HOOK_custom_postwakeup                   0
#define romBLE_HOOK_custom_postwakeup_func              custom_postwakeup

#define romBLE_HOOK_custom_preidlecheck                 0
#define romBLE_HOOK_custom_preidlecheck_func            custom_preidlecheck

#define romBLE_HOOK_custom_pti_set                      0
#define romBLE_HOOK_custom_pti_set_func                 custom_pti_set

#define ASSIGN_BLE_HOOK(name)                           (romBLE_HOOK_##name != 0 ? romBLE_HOOK_##name##_func : NULL)

extern const uint32_t rom_func_addr_table_var[];

#define CHECK_AND_CALL(func_ptr)                (romBLE_HOOK_##func_ptr != 0 ? romBLE_HOOK_##func_ptr##_func() : 0)
#define CHECK_AND_CALL_WITH_VAR(func_ptr, v)    (romBLE_HOOK_##func_ptr != 0 ? romBLE_HOOK_##func_ptr##_func(v) : 0)
/*
 * Variants of the above macros, which cast away the return value.
 * Convenient to use when you don't care about the return value, to avoid compilation warnings.
 */
#define CHECK_AND_CALL_VOID(func_ptr)                   (void)CHECK_AND_CALL(func_ptr)
#define CHECK_AND_CALL_WITH_VAR_VOID(func_ptr, v)       (void)CHECK_AND_CALL_WITH_VAR(func_ptr, v)

/// Object allocated in shared memory - check linker script
#define __SHARED                __attribute__ ((section("shram")))

/// @} DRIVERS
#endif // _ARCH_H_
