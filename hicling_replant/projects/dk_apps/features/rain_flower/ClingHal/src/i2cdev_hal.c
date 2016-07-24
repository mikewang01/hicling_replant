#include <btle_api.h>
/******************************************************************************
#include <spidev_hal.h>
#include <spidev_hal.h>
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
#include "pin_names.h"
#include "platform_devices.h"
#include "i2cdev_hal.h"
#include "ad_i2c.h"
#include "hw_i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
 * MACROS
 */
#define  I2C_ATTACHED_BUS_NAME      I2C1 //SPI2

#if IS_I2C_ACC_SENSOR_SUPPORTED
I2C_SLAVE_DEVICE(I2C1, I2C_ACC_SENSOR_NAME, I2C_ACC_SENSOR_ADDR, HW_I2C_ADDRESSING_7B, HW_I2C_SPEED_FAST);
#endif

#if IS_I2C_OLED_SUPPORTED
I2C_SLAVE_DEVICE(I2C1, FM75, 0x4F, HW_I2C_ADDRESSING_7B, HW_I2C_SPEED_STANDARD);
#endif



/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * LOCAL VARIABLES
 */
static CLASS(I2CDevHal) *p_instance = NULL;
/*********************************************************************
 * LOCAL DECLARATION
 */
static int write_read(CLASS(I2CDevHal) *arg, i2c_dev_handle_t hdl, uint8_t *p_txbuf, size_t tx_size,
        uint8_t *p_rxbuf, size_t rx_size);
static int read(CLASS(I2CDevHal) *arg, i2c_dev_handle_t hdl, uint8_t *p_buf, size_t size);
static int write(CLASS(I2CDevHal) *arg, i2c_dev_handle_t hdl, uint8_t *p_buf, size_t size);
static i2c_dev_handle_t open(CLASS(I2CDevHal) *arg, const I2C_BUS_DEVICE_NAME dev_id);
static int close(CLASS(I2CDevHal) *arg, i2c_dev_handle_t hdl);

/******************************************************************************
 * FunctionName : I2CDevHal_init
 * Description  : I2CDevHal object constuction function
 * Parameters   : CLASS(I2CDevHal) *arg: object pointer
 * Returns          : 0: sucess
 *                   -1: error
 *******************************************************************************/

static int I2CDevHal_init(CLASS(I2CDevHal) *arg)
{
        if (arg == NULL) {
                return -1;
        }
        arg->open = open;
        arg->read = read;
        arg->write = write;
        arg->write_read = write_read;
        arg->close = close;

        ad_i2c_init();
        I2C_BUS_INIT(I2C1);
#if IS_I2C_ACC_SENSOR_SUPPORTED
        I2C_DEVICE_INIT(I2C_ACC_SENSOR_NAME);
#endif

#if IS_I2C_OLED_SUPPORTED
        I2C_DEVICE_INIT(I2C_OLED_NAME);
#endif
        return 0;
}

/******************************************************************************
 * FunctionName : I2CDevHal_get_instance
 * Description  : for user to get single instance object pointer
 * Parameters   : none
 * Returns          : 0: sucess
 *                   -1: error
 *******************************************************************************/

CLASS(I2CDevHal)* I2CDevHal_get_instance(void)
{
        static CLASS(I2CDevHal) p;
        if(p_instance  == NULL){
                p_instance  = &p;
                memset(&p, 0, sizeof(p));
                I2CDevHal_init(p_instance);
        }
        return p_instance;
}

/******************************************************************************
 * FunctionName :btle_fsm_process
 * Description  : fsm
 * Parameters   : write: write call back function
 * Returns          : 0: sucess
 -1: error
 *******************************************************************************/
static i2c_dev_handle_t open(CLASS(I2CDevHal) *arg, const I2C_BUS_DEVICE_NAME dev_id)
{
        return ad_i2c_open(dev_id);
}

static int write(CLASS(I2CDevHal) *arg, i2c_dev_handle_t hdl, uint8_t *p_buf, size_t size)
{
        ad_i2c_write(hdl, p_buf, size);
        return 0;
}

static int read(CLASS(I2CDevHal) *arg, i2c_dev_handle_t hdl, uint8_t *p_buf, size_t size)
{
        ad_i2c_read(hdl, p_buf, size);
        return 0;
}

static int write_read(CLASS(I2CDevHal) *arg, i2c_dev_handle_t hdl, uint8_t *p_txbuf, size_t tx_size,
        uint8_t *p_rxbuf, size_t rx_size)
{
        ad_i2c_transact(hdl, p_txbuf, tx_size, p_rxbuf, rx_size);
        return 0;
}

static int close(CLASS(I2CDevHal) *arg, i2c_dev_handle_t hdl)
{
        ad_i2c_close(hdl);
        return 0;
}

#ifdef __cplusplus
}
#endif
