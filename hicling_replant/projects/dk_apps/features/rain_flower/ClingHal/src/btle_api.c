#include <battmgr_hal.h>
#include <btle_api.h>
/******************************************************************************
 * Copyright 2013-2016 hicling Systems (MikeWang)
 *
 * FileName: rtc_hal.c
 *
 * Description: abstract layer between cling sdk and freertos.
 *
 * Modification history:
 *     2016/7/22, v1.0 create this file mike.
 *******************************************************************************/
#include "oop_hal.h"
#include "ad_flash.h"
#include "rtc_hal.h"
#include "osal.h"
#include "main.h"
#include "stdio.h"
#include "string.h"
#include "main.h"
#include "btle_api.h"
#ifdef __cplusplus
extern "C" {
#endif

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
/******************************************************************************
 * FunctionName :btle_fsm_init
 * Description  : init fsm state
 * Parameters   : write: write call back function
 * Returns          : 0: sucess
 -1: error
 *******************************************************************************/

void usb_charger_event_observer(usb_charger_event_t evt)
{
        N_SPRINTF("[batt mgr] test event %d arrived", (uint8_t) evt);
}
int btle_fsm_init(void)
{
        RTC_Init();
        /*start rtc*/
        RTC_Start();
        SYSTEM_CLING_init();
        CLING_BATT_MGR_DEV_HAL_INIT(usb_charger_event_observer);
        CLASS(BattMgrHal)*p = BattMgrHal_get_instance();
        Y_SPRINTF("[ble] batt get instance");
        batt_handle_t t = p->open(p);
        Y_SPRINTF("[ble] batt voltage = %d mv", p->read(p, t));
        btle_init();
        return 0;
}

/******************************************************************************
 * FunctionName :btle_fsm_process
 * Description  : fsm
 * Parameters   : write: write call back function
 * Returns          : 0: sucess
 -1: error
 *******************************************************************************/
int btle_fsm_process()
{
        CP_state_machine_update();
        LINK_state_machine();
        BTLE_state_machine_update();
        return 0;
}

#ifdef __cplusplus
}
#endif
