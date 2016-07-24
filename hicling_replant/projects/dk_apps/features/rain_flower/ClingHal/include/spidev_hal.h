/********************************************************************************

 **** Copyright (C), 2015, xx xx xx xx info&tech Co., Ltd.                ****

 ********************************************************************************
 * File Name     : wrist_detect.h
 * Author        : MikeWang
 * Date          : 2016-03-28
 * Description   : __SPI_HAL_H__ header file
 * Version       : 1.0
 * Function List :
 *
 * Record        :
 * 1.Date        : 2016-03-28
 *   Author      : MikeWang
 *   Modification: Created file

 *************************************************************************************************************/

#ifndef __SPI_HAL_H__
#define __SPI_HAL_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#include "stdint.h"
#include "oop_hal.h"
#include "ad_spi.h"
/*================================type def===============================================*/

typedef void* spi_dev_handle_t;
#define  INVALID_SPI_DEV_HANDLE NULL
#define  SPI_BUS_DEVICE_NAME    spi_device_id

/*================================CONFIGRATION===============================================*/
DEF_CLASS(SpiDevHal)
        spi_dev_handle_t (*open)(CLASS(SpiDevHal) *arg, const SPI_BUS_DEVICE_NAME dev_name);
        int (*write)(CLASS(SpiDevHal) *arg, spi_dev_handle_t hdl, uint8_t *p_buf,  size_t size);
        int (*read)(CLASS(SpiDevHal) *arg, spi_dev_handle_t hdl, uint8_t *p_buf,  size_t size);
        int (*write_read)(CLASS(SpiDevHal) *arg, spi_dev_handle_t hdl, uint8_t *p_txbuf,  size_t tx_size, uint8_t *p_rxbuf,  size_t rx_size);
        int (*close)(CLASS(SpiDevHal) *arg, spi_dev_handle_t hdl);
END_DEF_CLASS(SpiDevHal)


CLASS(SpiDevHal)* SpiDevHal_get_instance(void);


/*================================CONFIGRATION===============================================*/
#define  IS_SPI_ACC_SENSOR_SUPPORTED (1)
#define  ACC_SENSOR_NAME        LIS3DH

#define  IS_SPI_OLED_SUPPORTED       (0)
#define  OLED_NAME              OLED

#define  IS_SPI_PPG_SENSOR_SUPPORTED       (1)
#define  PPG_SENSOR_NAME        PPG

/*WRAP IT TP A INITIZIZED ONE */
#define CLING_SPI_DEV_HAL_INIT()  do{\
        SpiDevHal_get_instance()\
while(0)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __SPI_HAL_H__ */
