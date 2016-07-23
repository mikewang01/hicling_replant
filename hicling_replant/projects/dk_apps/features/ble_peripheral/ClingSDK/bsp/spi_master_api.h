/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/**@file
 *
 * @defgroup spi_master_example_main spi_master_config
 * @{
 * @ingroup spi_master_example
 *
 * @brief Defintions needed for the SPI master driver.
 */
 
#ifndef SPI_MASTER_API_H__
#define SPI_MASTER_API_H__

#ifndef _CLING_PC_SIMULATION_

#include "standard_types.h"

#define SPI_GPIO_PORT      GPIO_PORT_0
#define SPI_CLK_PIN        GPIO_SPI_SCK          /**< SPI clock GPIO pin number. */
#define SPI_DO_PIN         GPIO_SPI_MOSI         /**< SPI Master Out Slave In GPIO pin number. */
#define SPI_DI_PIN         GPIO_SPI_MISO         /**< SPI Master In Slave Out GPIO pin number. */

void SPI_master_disable(void);

void SPI_master_init(void);

void SPI_master_tx_rx(I8U * tx_data_buf, I16U tx_data_size, 
										  I8U * rx_data_buf, I16U rx_data_size, 
											I8U pin_port, I8U pin_cs);

#endif

#endif // SPI_MASTER_API_H__
