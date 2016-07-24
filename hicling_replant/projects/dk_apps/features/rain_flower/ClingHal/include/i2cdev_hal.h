/********************************************************************************

 **** Copyright (C), 2015, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : wrist_detect.h
 * Author        : MikeWang
 * Date          : 2016-03-28
 * Description   : __I2C_DEV_HAL_H__ header file
 * Version       : 1.0
 * Function List :
 *
 * Record        :
 * 1.Date        : 2016-03-28
 *   Author      : MikeWang
 *   Modification: Created file

 *************************************************************************************************************/

#ifndef __I2C_DEV_HAL_H__
#define __I2C_DEV_HAL_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include "stdint.h"
#include "oop_hal.h"
#include "ad_i2c.h"
/*================================type def===============================================*/

typedef void* i2c_dev_handle_t;
#define  INVALID_SPI_DEV_HANDLE NULL
#define  I2C_BUS_DEVICE_NAME    i2c_device_id

/*================================CONFIGRATION===============================================*/
DEF_CLASS(I2CDevHal)
        i2c_dev_handle_t (*open)(CLASS(I2CDevHal) *arg, const I2C_BUS_DEVICE_NAME dev_name);
        int (*write)(CLASS(I2CDevHal) *arg, i2c_dev_handle_t hdl, uint8_t *p_buf,  size_t size);
        int (*read)(CLASS(I2CDevHal) *arg, i2c_dev_handle_t hdl, uint8_t *p_buf,  size_t size);
        int (*write_read)(CLASS(I2CDevHal) *arg, i2c_dev_handle_t hdl, uint8_t *p_txbuf,  size_t tx_size, uint8_t *p_rxbuf,  size_t rx_size);
        int (*close)(CLASS(I2CDevHal) *arg, i2c_dev_handle_t hdl);
END_DEF_CLASS(I2CDevHal)


CLASS(I2CDevHal)* I2CDevHal_get_instance(void);


/*================================CONFIGRATION===============================================*/
#define  IS_I2C_ACC_SENSOR_SUPPORTED (1)
#define  I2C_ACC_SENSOR_NAME        LIS3DH
#define  I2C_ACC_SENSOR_ADDR        0X00

#define  IS_I2C_OLED_SUPPORTED      (0)
#define  I2C_OLED_NAME              OLED
#define  I2C_OLED_ADDR              0X01

#define  IS_I2C_PPG_SENSOR_SUPPORTED       (1)
#define  I2C_PPG_SENSOR_NAME        PPG
#define  I2C_PPG_SENSOR_ADDR        0X02

/*WRAP IT TP A INITIZIZED ONE */
#define CLING_I2C_DEV_HAL_INIT()  do{\
        I2CDevHal_get_instance();\
while(0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __I2C_DEV_HAL_H__ */
