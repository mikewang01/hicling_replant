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
#include "ad_spi.h"
#include "platform_devices.h"
#include "spidev_hal.h"
#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************
 * MACROS
 */
#define  ATTACHED_BUS_NAME      SPI1 //SPI2

#if IS_SPI_ACC_SENSOR_SUPPORTED
SPI_SLAVE_DEVICE(SPI1, ACC_SENSOR_NAME, GET_PORT_NUM(LIS3DH_SPI_CS), GET_PIN_NUM(LIS3DH_SPI_CS),
        HW_SPI_WORD_8BIT, HW_SPI_POL_LOW, HW_SPI_PHA_MODE_0,
        HW_SPI_FREQ_DIV_4, HW_DMA_CHANNEL_4);
#endif

#if IS_SPI_OLED_SUPPORTED
SPI_SLAVE_DEVICE(SPI1, OLED_NAME, GET_PORT_NUM(OLED_SPI_CS), GET_PIN_NUM(OLED_SPI_CS),
        HW_SPI_WORD_8BIT, HW_SPI_POL_LOW, HW_SPI_PHA_MODE_0,
        HW_SPI_FREQ_DIV_4, HW_DMA_CHANNEL_4);
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
static CLASS(SpiDevHal) *p_instance = NULL;
/*********************************************************************
 * LOCAL DECLARATION
 */
static int write_read(CLASS(SpiDevHal) *arg, spi_dev_handle_t hdl, uint8_t *p_txbuf, size_t tx_size,
        uint8_t *p_rxbuf, size_t rx_size);
static int read(CLASS(SpiDevHal) *arg, spi_dev_handle_t hdl, uint8_t *p_buf, size_t size);
static int write(CLASS(SpiDevHal) *arg, spi_dev_handle_t hdl, uint8_t *p_buf, size_t size);
static spi_dev_handle_t open(CLASS(SpiDevHal) *arg, const SPI_BUS_DEVICE_NAME dev_id);
static int close(CLASS(SpiDevHal) *arg, spi_dev_handle_t hdl);

/******************************************************************************
 * FunctionName : SpiDevHal_init
 * Description  : SpiDevHal object constuction function
 * Parameters   : CLASS(SpiDevHal) *arg: object pointer
 * Returns          : 0: sucess
 *                   -1: error
 *******************************************************************************/

static int SpiDevHal_init(CLASS(SpiDevHal) *arg)
{
        if (arg == NULL) {
                return -1;
        }
        arg->open = open;
        arg->read = read;
        arg->write = write;
        arg->write_read = write_read;
        arg->close = close;

        ad_spi_init();
        SPI_BUS_INIT(SPI1);
#if IS_SPI_ACC_SENSOR_SUPPORTED
        SPI_DEVICE_INIT(ACC_SENSOR_NAME);
#endif

#if IS_SPI_OLED_SUPPORTED
        SPI_DEVICE_INIT(OLED_NAME);
#endif
        return 0;
}

/******************************************************************************
 * FunctionName : SpiDevHal_get_instance
 * Description  : for user to get single instance object pointer
 * Parameters   : none
 * Returns          : 0: sucess
 *                   -1: error
 *******************************************************************************/

CLASS(SpiDevHal)* SpiDevHal_get_instance(void)
{
        static CLASS(SpiDevHal) p;
        if(p_instance  == NULL){
                p_instance  = &p;
                memset(&p, 0, sizeof(p));
                SpiDevHal_init(p_instance);
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

static spi_dev_handle_t open(CLASS(SpiDevHal) *arg, const SPI_BUS_DEVICE_NAME dev_id)
{
        return ad_spi_open(dev_id);
}

static int write(CLASS(SpiDevHal) *arg, spi_dev_handle_t hdl, uint8_t *p_buf, size_t size)
{
        ad_spi_write(hdl, p_buf, size);
        return 0;
}

static int read(CLASS(SpiDevHal) *arg, spi_dev_handle_t hdl, uint8_t *p_buf, size_t size)
{
        ad_spi_read(hdl, p_buf, size);
        return 0;
}

static int write_read(CLASS(SpiDevHal) *arg, spi_dev_handle_t hdl, uint8_t *p_txbuf, size_t tx_size,
        uint8_t *p_rxbuf, size_t rx_size)
{
        ad_spi_transact(hdl, p_txbuf, tx_size, p_rxbuf, rx_size);
        return 0;
}

static int close(CLASS(SpiDevHal) *arg, spi_dev_handle_t hdl)
{
        ad_spi_close(hdl);
        return 0;
}

#ifdef __cplusplus
}
#endif
