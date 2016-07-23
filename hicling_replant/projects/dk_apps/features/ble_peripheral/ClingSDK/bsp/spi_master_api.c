///* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
// *
// * The information contained herein is property of Nordic Semiconductor ASA.
// * Terms and conditions of usage are described in detail in NORDIC
// * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
// *
// * Licensees are granted free, non-transferable use of the information. NO
// * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
// * the file.
// *
// */
//#include "main.h"
////#include "spi.h"
//
//uint8_t rd_data[512];
//uint8_t wr_data[512];
//
//
//uint8_t g_spi_tx_buf[10];
//uint8_t g_spi_rx_buf[10];
//
//static BOOLEAN  m_transfer_completed = TRUE;
//
//void SPI_master_disable()
//{
//	spi_disable();
//	cling.system.b_spi_ON = FALSE;
//
//	N_SPRINTF("[SPI] deinit ...");
//}
//
//static void _spi_master_op_wait_done()
//{
//	I16U i;
//
//	if (m_transfer_completed)
//		return;
//
//	// Wait for up to 200 ms
//	for (i = 0; i < 200; i++) {
//		BASE_delay_msec(1);
//
//		if (m_transfer_completed)
//			break;
//	}
//
//  if(!m_transfer_completed)
//	Y_SPRINTF("[SPI] transfer incompleted, wait for 200ms");
//}
//
//// When spi bus transfer failed,pull up all cs pin.
//static void _spi_master_clear_cs_pin()
//{
//
//}
//
//
///**@brief Function for initializing a SPI master driver.
// */
//void SPI_master_init()
//{
//	SPI_Pad_t spi_CS_Pad;
//
//	// Flag SPI module initialization to indicate initializaiton is completed
//	cling.system.b_spi_ON = TRUE;
//
//	spi_CS_Pad.pin = GPIO_SPI_CS_NFLASH;
//  spi_CS_Pad.port = SPI_GPIO_PORT;
//
//	spi_init(&spi_CS_Pad, SPI_MODE_8BIT, SPI_ROLE_MASTER, SPI_CLK_IDLE_POL_LOW, SPI_PHA_MODE_0, SPI_MINT_DISABLE, SPI_XTAL_DIV_8);
//
//}
//
//
///**@brief Function for sending and receiving data.
//*/
//void SPI_master_tx_rx(I8U * tx_data_buf, I16U tx_data_size,
//										  I8U * rx_data_buf, I16U rx_data_size,
//											I8U pin_port, I8U pin_cs)
//{
//	SPI_Pad_t spi_CS_Pad;
//	I16U i;
//
//  spi_CS_Pad.port = pin_port;
//	spi_CS_Pad.pin = pin_cs;
//
//	if(!cling.system.b_spi_ON){
//		N_SPRINTF("[SPI] re-init: %d", cling.system.b_spi_0_ON);
//	  SPI_master_init();
//	} else {
//		// If SPI module is already ON, just configure it to a different CS pin
//		spi_set_cs(&spi_CS_Pad);
//	}
//
//	spi_set_bitmode(SPI_MODE_8BIT);
//	spi_cs_low();            				  // pull CS low
//
//	// Write all data
//	if (tx_data_size > 0) {
//
//		for (i = 0; i < tx_data_size; i++) {
//			spi_access(tx_data_buf[i]);
//		}
//	}
//
//	// Read out all data
//	if (rx_data_size > 0) {
//		for (i = 0; i < rx_data_size; i++) {
//			rx_data_buf[i] = spi_access(0x0000);
//		}
//	}
//
//	spi_cs_high();
//}
