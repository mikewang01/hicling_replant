#include <battmgr_hal.h>
#include <btle_api.h>
/******************************************************************************
 #include <spidev_hal.h>
 #include <spidev_hal.h>
 * Copyright 2013-2016 hicling Systems (MikeWang)
 *
 * FileName: BattMgrHal.c
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
#include "pin_names.h"
#include "platform_devices.h"
#include "ad_battery.h"
#include "battmgr_hal.h"
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
static CLASS(BattMgrHal) *p_instance = NULL;
static void (*user_layer_callback)(int value) = NULL;
static void (*usb_charger_mgr_event_cb_p)(usb_charger_event_t evt) = NULL;
static usb_charger_event_t latest_event_record = USB_CHARGER_NO_EVENT;
/*********************************************************************
 * LOCAL DECLARATION
 */
static batt_handle_t open(CLASS(BattMgrHal) *arg);
static int read(CLASS(BattMgrHal) *arg, const batt_handle_t hdl);
static void batt_async_read_callback(void *user_data, int value);
static int read_async(CLASS(BattMgrHal) *arg, const batt_handle_t hdl, void (*cb)(int value));
static int close(CLASS(BattMgrHal) *arg, const batt_handle_t hdl);
static int reg_usb_charger_event(CLASS(BattMgrHal) *arg, void (*cb)(usb_charger_event_t evt));

/******************************************************************************
 * FunctionName : BattMgrHal_init
 * Description  : BattMgrHal object constuction function
 * Parameters   : CLASS(BattMgrHal) *arg: object pointer
 * Returns          : 0: sucess
 *                   -1: error
 *******************************************************************************/

static int BattMgrHal_init(CLASS(BattMgrHal) *arg)
{
        if (arg == NULL) {
                return -1;
        }
        arg->open = open;
        arg->read = read;
        arg->read_async = read_async;
        arg->reg_usb_charger_event_cb = reg_usb_charger_event;
        arg->close = close;
        GPADC_INIT();
        return 0;
}

/******************************************************************************
 * FunctionName : BattMgrHal_get_instance
 * Description  : for user to get single instance object pointer
 * Parameters   : none
 * Returns          : 0: sucess
 *                   -1: error
 *******************************************************************************/

CLASS(BattMgrHal)* BattMgrHal_get_instance(void)
{
        static CLASS(BattMgrHal) p;
        if (p_instance == NULL) {
                memset(&p, 0, sizeof(p));
                p_instance = &p;
                BattMgrHal_init(p_instance);
        }
        return p_instance;
}

/******************************************************************************
 * FunctionName :open
 * Description  : get batt handle
 * Parameters   : arg: obj pointer
 * Returns          : 0: sucess
 -1: error
 *******************************************************************************/
static batt_handle_t open(CLASS(BattMgrHal) *arg)
{
        N_SPRINTF("[batt hal] batt opened");
        return ad_battery_open();
}

static int read(CLASS(BattMgrHal) *arg, const batt_handle_t hdl)
{
        return ad_battery_raw_to_mvolt(hdl, ad_battery_read(hdl));
}

static void batt_async_read_callback(void *user_data, int value)
{

        if (user_layer_callback != NULL) {
                const batt_handle_t hdl = (const batt_handle_t)user_data;
                user_layer_callback(ad_battery_raw_to_mvolt(hdl, value));
        }
        return;
}

static int read_async(CLASS(BattMgrHal) *arg, const batt_handle_t hdl, void (*cb)(int value))
{
        if (cb != NULL) {
                user_layer_callback = cb;
        }
        ad_battery_read_async(hdl, batt_async_read_callback, hdl);
        return 0;
}

static int close(CLASS(BattMgrHal) *arg, const batt_handle_t hdl)
{
        ad_battery_close(hdl);
        return 0;
}

static int reg_usb_charger_event(CLASS(BattMgrHal) *arg, void (*cb)(usb_charger_event_t evt))
{
        if (cb != NULL) {
                usb_charger_mgr_event_cb_p = cb;
                usb_charger_mgr_event_cb_p(latest_event_record);
        }
        return 0;
}

void usb_attach_cb()
{
        latest_event_record = USB_ATTACH_EVENT;
        N_SPRINTF("[batt mgr] usb  event %d arrived ", latest_event_record);
        if (usb_charger_mgr_event_cb_p != NULL) {
                usb_charger_mgr_event_cb_p(latest_event_record);
        }
        return;
}
void usb_detach_cb(void)
{
        latest_event_record = USB_DETACH_EVENT;
        N_SPRINTF("[batt mgr] usb  event %d arrived ", latest_event_record);
        if (usb_charger_mgr_event_cb_p != NULL) {
                usb_charger_mgr_event_cb_p(latest_event_record);
        }
        return;
}
void usb_start_enumeration_cb(void)
{
        latest_event_record = USB_START_ENUMMERATION_EVENT;
        N_SPRINTF("[batt mgr] usb  event %d arrived ", latest_event_record);
        if (usb_charger_mgr_event_cb_p != NULL) {
                usb_charger_mgr_event_cb_p(latest_event_record);
        }
        return;
}
void usb_charging(void)
{
        latest_event_record = USB_CHARGING_EVENT;
        N_SPRINTF("[batt mgr] usb  event %d arrived ", latest_event_record);
        if (usb_charger_mgr_event_cb_p != NULL) {
                usb_charger_mgr_event_cb_p(latest_event_record);
        }
        return;
}
void usb_precharging(void)
{
        latest_event_record = USB_PRECHARGING_EVENT;
        N_SPRINTF("[batt mgr] usb  event %d arrived ", latest_event_record);
        if (usb_charger_mgr_event_cb_p != NULL) {
                usb_charger_mgr_event_cb_p(latest_event_record);
        }
        return;
}
void usb_precharging_aborted(void)
{
        latest_event_record = USB_PRECHARGING_ABORTED_EVENT;
        N_SPRINTF("[batt mgr] usb  event %d arrived ", latest_event_record);
        if (usb_charger_mgr_event_cb_p != NULL) {
                usb_charger_mgr_event_cb_p(latest_event_record);
        }
        return;
}
void usb_charging_stopped(void)
{
        latest_event_record = USB_CHARGING_STOPPED_EVENT;
        N_SPRINTF("[batt mgr] usb  event %d arrived ", latest_event_record);
        if (usb_charger_mgr_event_cb_p != NULL) {
                usb_charger_mgr_event_cb_p(latest_event_record);
        }
        return;
}
void usb_charging_aborted(void)
{
        latest_event_record = USB_CHARGING_ABORTED_EVENT;
        N_SPRINTF("[batt mgr] usb  event %d arrived ", latest_event_record);
        if (usb_charger_mgr_event_cb_p != NULL) {
                usb_charger_mgr_event_cb_p(latest_event_record);
        }
        return;
}
void usb_charged(void)
{
        latest_event_record = USB_CHARGED_EVENT;
        N_SPRINTF("[batt mgr] usb  event %d arrived ", latest_event_record);
        if (usb_charger_mgr_event_cb_p != NULL) {
                usb_charger_mgr_event_cb_p(latest_event_record);
        }
        return;
}
void usb_charger_battery_full(void)
{
        latest_event_record = USB_CHARGER_BATT_FULL_EVENT;
        N_SPRINTF("[batt mgr] usb  event %d arrived ", latest_event_record);
        if (usb_charger_mgr_event_cb_p != NULL) {
                usb_charger_mgr_event_cb_p(latest_event_record);
        }
        return;
}
void usb_charger_bad_battery(void)
{
        latest_event_record = USB_CHARGER_BAD_BATT_EVENT;
        N_SPRINTF("[batt mgr] usb  event %d arrived ", latest_event_record);
        if (usb_charger_mgr_event_cb_p != NULL) {
                usb_charger_mgr_event_cb_p(latest_event_record);
        }
        return;
}
void usb_charger_temp_low(void)
{
        latest_event_record = USB_CHARGER_BATT_TMP_LOW_EVENT;
        N_SPRINTF("[batt mgr] usb  event %d arrived ", latest_event_record);
        if (usb_charger_mgr_event_cb_p != NULL) {
                usb_charger_mgr_event_cb_p(latest_event_record);
        }
        return;
}
void usb_charger_temp_high(void)
{
        latest_event_record = USB_CHARGER_BATT_TMP_HIGH_EVENT;
        N_SPRINTF("[batt mgr] usb  event %d arrived ", latest_event_record);
        if (usb_charger_mgr_event_cb_p != NULL) {
                usb_charger_mgr_event_cb_p(latest_event_record);
        }
        return;
}

#ifdef __cplusplus
}
#endif
