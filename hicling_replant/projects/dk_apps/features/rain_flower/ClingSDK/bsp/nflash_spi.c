//#include "main.h"
//#include "spi_flash.h"
//
//#ifndef _CLING_PC_SIMULATION_
//extern I8U g_spi_tx_buf[];
//extern I8U g_spi_rx_buf[];
//#else
//uint8_t g_spi_tx_buf[10];
//uint8_t g_spi_rx_buf[10];
//#endif
//BOOLEAN b_flash_PD_flag;
///*----------------------------------------------------------------------------------
//*  Function:	void NOR_init(void)
//*
//*  Description:
//*	Init the SPI interface
//*
//*----------------------------------------------------------------------------------*/
//#define SPI_FLASH_SIZE (256*1024)              // SPI Flash memory size in bytes
//#define SPI_FLASH_PAGE 256                 // SPI Flash memory page size in bytes
//
//void _wait_for_operation_completed()
//{
//#ifndef _CLING_PC_SIMULATION_
//	I32U count = 0;
//	I8U ret;
//
//	if (b_flash_PD_flag) {
//		b_flash_PD_flag = FALSE;
//		NOR_releasePowerDown();
//	}
//	// Maximum delay 500 milliseconds
//	for (count = 0; count < 800; count ++) {
//		ret = NOR_readStatusRegister();
//
//		if (ret & SPI_FLASH_WIP) {
//			BASE_delay_msec(1);
//		} else {
//			break;
//		}
//	}
//#endif
//}
//
//void NOR_init(void)
//{
//#ifndef _CLING_PC_SIMULATION_
//	I8U buf[2];
//	spi_flash_auto_detect();
//  spi_flash_init(SPI_FLASH_SIZE, SPI_FLASH_PAGE);
//
//	// Power up the chip
//	b_flash_PD_flag = FALSE;
//	NOR_releasePowerDown();
//
//	// Read ID
//	NOR_readID((I8U *)buf);
//
//	cling.whoami.nor[0] = buf[0];
//	cling.whoami.nor[1] = buf[1];
//
//	_wait_for_operation_completed();
//
//	NOR_powerDown();
//	b_flash_PD_flag = TRUE;
//#endif
//}
//
///*----------------------------------------------------------------------------------
//*  Function:	void NOR_writeEnable(void)
//*
//*  Description:
//*	send the write enable command (0x06), all write or erase operation must send
//*  the command firstly
//*
//*----------------------------------------------------------------------------------*/
//void NOR_writeEnable(void)
//{
//	int8_t err_code;
//
//	if (b_flash_PD_flag) {
//		b_flash_PD_flag = FALSE;
//		NOR_releasePowerDown();
//	}
//
//	_wait_for_operation_completed();
//
//	err_code = spi_flash_set_write_enable();
//
//	if (err_code != ERR_OK) {
//		Y_SPRINTF("[NFLASH] flash write enable error :%d" ,err_code);
//	}
//}
//
//void NOR_writeDisable(void)
//{
//	int32_t err_code;
//
//	if (b_flash_PD_flag) {
//		b_flash_PD_flag = FALSE;
//		NOR_releasePowerDown();
//	}
//
//  err_code = spi_flash_set_write_disable();
//
//	if (err_code != ERR_OK) {
//		Y_SPRINTF("[NFLASH] flash write disable error :%d" ,err_code);
//	}
//
//  _wait_for_operation_completed();
//}
//
///*----------------------------------------------------------------------------------
//*  Function:	I8U NOR_readStatusRegister(void)
//*
//*  Description:
//*	read status register (0x04), return the status register value
//*
//*----------------------------------------------------------------------------------*/
//I8U NOR_readStatusRegister(void)
//{
//	I8U status;
//
//  status = spi_flash_read_status_reg();
//
//	return status;
//}
//
///*----------------------------------------------------------------------------------
//*  Function:	void NOR_readData(I32U addr, I16U len, I8U *dataBuf)
//*
//*  Description:
//*	read the len's data to dataBuf
//*  addr: the 24 bit address
//*  len: read length
//*  dataBuf: the out data buf
//*
//*----------------------------------------------------------------------------------*/
//void NOR_readData(I32U addr, I16U len, I8U *dataBuf)
//{
//	int32_t err_code;
//
//	if (b_flash_PD_flag) {
//		b_flash_PD_flag = FALSE;
//		NOR_releasePowerDown();
//	}
//
//	err_code = spi_flash_read_data(dataBuf, addr, len);
//
//	if (err_code != ERR_OK) {
//		if (err_code != len) {
//		  Y_SPRINTF("[NFLASH] flash read data error :%d" ,err_code);
//		}
//	}
//
//	if (!OTA_if_enabled()){
//	  if (!b_flash_PD_flag) {
//		  b_flash_PD_flag = TRUE;
//		  NOR_powerDown();
//	  }
//  }
//}
///*----------------------------------------------------------------------------------
//*  Function:	void NOR_pageProgram(I32U addr, I16U len, I8U *data)
//*
//*  Description:
//*	read the len's data to dataBuf
//*  addr: the 24 bit address
//*  len: write length, must <=256
//*  data: the write data buf
//*
//*----------------------------------------------------------------------------------*/
//static void _page_program_core(I32U addr, I16U len, I8U *data)
//{
//	int32_t err_code;
//
//	if (b_flash_PD_flag) {
//		b_flash_PD_flag = FALSE;
//		NOR_releasePowerDown();
//	}
//
//	NOR_writeEnable();
//
//  err_code = spi_flash_page_program(data, addr, len);
//
//	if (err_code != ERR_OK) {
//		Y_SPRINTF("[NFLASH] flash page program error :%d" ,err_code);
//	}
//
//	_wait_for_operation_completed();
//
//	if (!OTA_if_enabled()){
//	  if (!b_flash_PD_flag) {
//	  	b_flash_PD_flag = TRUE;
//		  NOR_powerDown();
//	  }
//  }
//}
//
//void NOR_pageProgram(I32U addr, I16U len, I8U *data)
//{
//	I32U start_addr = addr;
//	I32U end_addr = addr + len - 1;
//	I32U length_1, length_2;
//
//	if (b_flash_PD_flag) {
//		b_flash_PD_flag = FALSE;
//		NOR_releasePowerDown();
//	}
//
//	//
//	// Note, page size is 256 bytes, and it can only be programmed within the page.
//	//
//	// if we cross the page boundary, we have to seperate it into two parts.
//	//
//	start_addr &= 0xffffff00;
//	end_addr   &= 0xffffff00;
//
//	if (start_addr == end_addr) {
//		_page_program_core(addr, len, data);
//	} else {
//		// First page
//		length_1 = 256 - (addr - start_addr);
//		_page_program_core(addr, length_1, data);
//
//		// Second page
//		addr += length_1;
//		length_2 = len - length_1;
//		_page_program_core(addr, length_2, data+length_1);
//	}
//
//	if (!OTA_if_enabled()){
//	  if (!b_flash_PD_flag) {
//		  b_flash_PD_flag = TRUE;
//		  NOR_powerDown();
//	  }
//  }
//}
///*----------------------------------------------------------------------------------
//*  Function:	NOR_erase_block_4k(I32U addr)
//*
//*  Description:
//*	erase the sector(4K Bytes)
//*  addr: the 24 bit address
//*
//*
//*----------------------------------------------------------------------------------*/
//void NOR_erase_block_4k(I32U addr)
//{
//	int8_t err_code;
//
//	if (b_flash_PD_flag) {
//		b_flash_PD_flag = FALSE;
//		NOR_releasePowerDown();
//	}
//
//	NOR_writeEnable();
//
//  err_code = spi_flash_block_erase(addr, SECTOR_ERASE);
//
//	if (err_code != ERR_OK) {
//		Y_SPRINTF("[NFLASH] flash erase block 4k error :%d" ,err_code);
//	}
//
//	//wait for the operation finished
//	_wait_for_operation_completed();
//
//	if (!OTA_if_enabled()){
//	  if (!b_flash_PD_flag) {
//	  	b_flash_PD_flag = TRUE;
//		  NOR_powerDown();
//	  }
//  }
//}
//
///*----------------------------------------------------------------------------------
//*  Function:	NOR_erase_block_32k(I32U addr)
//*
//*  Description:
//*	erase the 32K block
//*  addr: the 24 bit address
//*
//*
//*----------------------------------------------------------------------------------*/
//void NOR_erase_block_32k(I32U addr)
//{
//  int8_t err_code;
//
//	if (b_flash_PD_flag) {
//		b_flash_PD_flag = FALSE;
//		NOR_releasePowerDown();
//	}
//
//	NOR_writeEnable();
//
//	err_code = spi_flash_block_erase(addr, BLOCK_ERASE_32);
//
//	if (err_code != ERR_OK) {
//		Y_SPRINTF("[NFLASH] flash erase block 32k error :%d" ,err_code);
//	}
//
//	//wait for the operation finished
//	_wait_for_operation_completed();
//
//	if (!OTA_if_enabled()){
//	  if (!b_flash_PD_flag) {
//		  b_flash_PD_flag = TRUE;
//		  NOR_powerDown();
//	  }
//  }
//}
///*----------------------------------------------------------------------------------
//*  Function:	NOR_erase_block_64k(I32U addr)
//*
//*  Description:
//*	erase the 64K block
//*  addr: the 24 bit address
//*
//*----------------------------------------------------------------------------------*/
//void NOR_erase_block_64k(I32U addr)
//{
//	int8_t err_code;
//
//	if (b_flash_PD_flag) {
//		b_flash_PD_flag = FALSE;
//		NOR_releasePowerDown();
//	}
//
//	NOR_writeEnable();
//
//	err_code = spi_flash_block_erase(addr, BLOCK_ERASE_64);
//
//	if (err_code != ERR_OK) {
//		Y_SPRINTF("[NFLASH] flash erase block 64k error :%d" ,err_code);
//	}
//
//	//wait for the operation finished
//	_wait_for_operation_completed();
//	N_SPRINTF("[NFLASH] completed!");
//
//	if (!OTA_if_enabled()){
//	  if (!b_flash_PD_flag) {
//		  b_flash_PD_flag = TRUE;
//		  NOR_powerDown();
//	  }
//  }
//}
///*----------------------------------------------------------------------------------
//*  Function:	NOR_ChipErase
//*
//*  Description:
//*	erase the 64K block
//*
//*----------------------------------------------------------------------------------*/
//void NOR_ChipErase()
//{
//	int8_t err_code;
//
//	if (b_flash_PD_flag) {
//		b_flash_PD_flag = FALSE;
//		NOR_releasePowerDown();
//	}
//
//	NOR_writeEnable();
//
//  err_code = spi_flash_chip_erase();
//
//	if (err_code != ERR_OK) {
//		Y_SPRINTF("[NFLASH] flash erase block 64k error :%d" ,err_code);
//	}
//	//wait for the operation finished
//	_wait_for_operation_completed();
//
//	if (!OTA_if_enabled()){
//	  if (!b_flash_PD_flag) {
//	  	b_flash_PD_flag = TRUE;
//		  NOR_powerDown();
//	  }
//  }
//}
///*----------------------------------------------------------------------------------
//*  Function:	void NOR_powerDown()
//*
//*  Description:
//*  power down the nor flash to save the power consumption
//*
//*----------------------------------------------------------------------------------*/
//void NOR_powerDown()
//{
//  int32_t err_code;
//
//	err_code = spi_flash_power_down();
//
//	if (err_code != ERR_OK) {
//		Y_SPRINTF("[NFLASH] flash power down error :%d" ,err_code);
//	}
//}
//
///*----------------------------------------------------------------------------------
//*  Function:	void NOR_releasePowerDown()
//*
//*  Description:
//*  exit the power down mode for normal operation
//*
//*----------------------------------------------------------------------------------*/
//void NOR_releasePowerDown()
//{
//	int32_t err_code;
//
//  err_code = spi_flash_release_from_power_down();
//
//	if (err_code != ERR_OK) {
//		Y_SPRINTF("[NFLASH] flash release power down error :%d" ,err_code);
//	}
//}
///*----------------------------------------------------------------------------------
//*  Function:	void NOR_readID(I8U *id)
//*
//*  Description:
//*  read the manufacture ID and device ID
//*	id: the array of I8U, the length >2
//*
//*----------------------------------------------------------------------------------*/
//void NOR_readID(I8U *id)
//{
//	uint16_t dev_id;
//
//	if (b_flash_PD_flag) {
//		b_flash_PD_flag = FALSE;
//		NOR_releasePowerDown();
//	}
//
//  dev_id = spi_read_flash_memory_man_and_dev_id();
//
//	*id = (I8U)dev_id;
//	*(id+1) = (I8U)(dev_id >> 8);
//
//	Y_SPRINTF("[NFLASH] read id :%02x :%02x",*id, *(id+1));
//	if (!OTA_if_enabled()){
//	  if (!b_flash_PD_flag) {
//		  b_flash_PD_flag = TRUE;
//		  NOR_powerDown();
//	  }
//  }
//}
///*----------------------------------------------------------------------------------
//*  Function:	void NOR_readUID(I8U *id)
//*
//*  Description:
//*  read the unique ID
//*	id: the array of I8U, the length >8
//*
//*----------------------------------------------------------------------------------*/
//void NOR_readUID(I8U *id)
//{
//	uint64_t unique_id;
//
//	if (b_flash_PD_flag) {
//		b_flash_PD_flag = FALSE;
//		NOR_releasePowerDown();
//	}
//
//	unique_id = spi_read_flash_unique_id();
//
//	*id = (I8U)unique_id;
//	*(id+1) = (I8U)(unique_id >> 8);
//	*(id+1) = (I8U)(unique_id >> 16);
//	*(id+1) = (I8U)(unique_id >> 24);
//	*(id+1) = (I8U)(unique_id >> 32);
//	*(id+1) = (I8U)(unique_id >> 40);
//	*(id+1) = (I8U)(unique_id >> 48);
//	*(id+1) = (I8U)(unique_id >> 56);
//
//	if (!OTA_if_enabled()){
//	  if (!b_flash_PD_flag) {
//		  b_flash_PD_flag = TRUE;
//		  NOR_powerDown();
//	  }
//  }
//}
