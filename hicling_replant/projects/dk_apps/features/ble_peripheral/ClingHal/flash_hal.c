/**
 ****************************************************************************************
 *
 * @file spi_flash_hal.c
 *
 * @brief abstract a layer between rtos and cling sdk
 *
 * Copyright (C) 2015. hicling elec Ltd, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 * <black.orca.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */
#include "oop_hal.h"
#include "ad_flash.h"
#include "flash_hal.h"
#define USER_FLASH_OFFSET_BASE  512*1024//(512K)

static int HalFlash_init(CLASS(HalFlash) *arg);
static int write_enable(CLASS(HalFlash) *arg);
static int write_disable(CLASS(HalFlash) *arg);
static int get_status_register(CLASS(HalFlash) *arg);
static int read_data(CLASS(HalFlash) *arg, uint32_t addr, uint16_t len, uint8_t *data_buf);
static int page_program(CLASS(HalFlash) *arg, uint32_t addr, uint16_t len, uint8_t *data_buf);
static int erase_one_block_4k(CLASS(HalFlash) *arg, uint32_t addr);
static int erase_one_block_32k(CLASS(HalFlash) *arg, uint32_t addr);
static int erase_one_block_64k(CLASS(HalFlash) *arg, uint32_t addr);
static int erase_all(CLASS(HalFlash) *arg);
static size_t get_erase_size(CLASS(HalFlash) *arg);
static size_t get_page_size(CLASS(HalFlash) *arg);

static CLASS(HalFlash) *flash_instance = NULL;
CLASS(HalFlash)* HalFlash_get_instance(void)
{
        static CLASS(HalFlash) p;
        if (flash_instance == NULL) {
                flash_instance = &p;
                HalFlash_init(flash_instance);
        }
        return flash_instance;
}

static int HalFlash_init(CLASS(HalFlash) *arg)
{
        if(arg == NULL){
                return -1;
        }
        arg->write_enable = write_enable;
        arg->write_disable = write_disable;
        arg->get_status_register = get_status_register;
        arg->read_data = read_data;
        arg->page_program = page_program;
        arg->erase_one_block_4k = erase_one_block_4k;
        arg->erase_one_block_64k = erase_one_block_64k;
        arg->erase_one_block_32k = erase_one_block_32k;
        arg->get_erase_size = get_erase_size;
        ad_flash_init();
        return 0;
}

static int write_enable(CLASS(HalFlash) *arg)
{
        return 0;
}
static int write_disable(CLASS(HalFlash) *arg)
{
        return 0;
}

static int get_status_register(CLASS(HalFlash) *arg)
{
        return 0;
}
static int read_data(CLASS(HalFlash) *arg, uint32_t addr, uint16_t len, uint8_t *data_buf)
{
        if (arg == NULL) {
                return -1;
        }
        return ad_flash_read(USER_FLASH_OFFSET_BASE + addr, data_buf, len);
}

static int page_program(CLASS(HalFlash) *arg, uint32_t addr, uint16_t len, uint8_t *data_buf)
{
        ad_flash_lock();
        ad_flash_write(USER_FLASH_OFFSET_BASE + addr, data_buf, len);
        ad_flash_unlock();
        return 0;
}
static int erase_one_block_4k(CLASS(HalFlash) *arg, uint32_t addr)
{
        ad_flash_lock();
        addr/=(arg->get_erase_size(arg));
        addr*=(arg->get_erase_size(arg));
        ad_flash_erase_region(USER_FLASH_OFFSET_BASE + addr,  arg->get_erase_size(arg));
        ad_flash_unlock();
        return 0;
}
static int erase_one_block_32k(CLASS(HalFlash) *arg, uint32_t addr)
{
        ad_flash_lock();
        addr/=(arg->get_erase_size(arg));
        addr*=(arg->get_erase_size(arg));
        ad_flash_erase_region(USER_FLASH_OFFSET_BASE + addr,  8*arg->get_erase_size(arg));
        ad_flash_unlock();
        return 0;
}
static int erase_one_block_64k(CLASS(HalFlash) *arg, uint32_t addr)
{
        printf("[hal_flash]:erase 64k start \r\n");
        ad_flash_lock();
        ad_flash_erase_region(USER_FLASH_OFFSET_BASE + addr,  8*arg->get_erase_size(arg));
        ad_flash_unlock();
        return 0;
}
static int erase_all(CLASS(HalFlash) *arg)
{
        ad_flash_lock();

        ad_flash_unlock();
        return 0;
}

static size_t get_erase_size(CLASS(HalFlash) *arg)
{
        return ad_flash_erase_size();
}
static size_t get_page_size(CLASS(HalFlash) *arg)
{
        return 256;
}


#define _NOR_FLASH_SPI_TEST_
#ifdef _NOR_FLASH_SPI_TEST_
void _nor_flash_spi_test()
{
    uint32_t  i, j, err_cnt;
    uint32_t add = 0;
    uint32_t buf[32];
    uint8_t *temp;
    uint32_t index, len, cnt;

    // Nor Flash initialize
    CLASS(HalFlash) *p = HalFlash_get_instance();

    // Read ID
    //NOR_readID((I8U *)buf);
   // temp = (I8U *)buf;
   // Y_SPRINTF("ID: %02x, %02x", temp[0], temp[1]);

    // Erase the 256 KB
#if 1
    add = 0;
    for (i = 0; i < 4; i ++) {
        printf("[FS] erase: %d, 64K\r\n", add);
        p->erase_one_block_64k(p, add);
//        NOR_erase_block_64k(add);
        add += 65536;
        //Watchdog_Feed();
    }
#else
    NOR_ChipErase();

#endif
    temp = (uint8_t *)buf;

    // Read out one byte just to confirm
    p->read_data(p, 0x0fffff, 1, temp);
    printf("Value at 0x00ffff: 0x%02x\r\n", temp[0]);

    cnt = 1000;

    // Program Flash, 1000x128 = 125 K
    add = 160;
    len = 128;
    for (i = 0; i < cnt; i++) {
        index = 0;
        for (j = 0; j < len; j++) {
            temp[j] = index ++;
        }
        p->page_program(p, add, len, temp);
//        NOR_pageProgram(add, len, temp);
        add += len;
//        Watchdog_Feed();
    }

    // Read out the data
    add = 160;
    len = 128;
    err_cnt = 0;
    for (i = 0; i < cnt; i++) {
        temp = (uint8_t *)buf;
        p->page_program(p, add, len, temp);
//        NOR_readData(add, len, temp);
        for (j = 0; j < len; j++) {
            if (j != temp[j]) {
                err_cnt ++;
                printf("[MAIN] %d, %d\r\n", j, temp[j]);
            }
        }
        /*
        temp = (I8U *)&buf[16];
                N_SPRINTF("%02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x,",
                        temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]);
                */
        add += len;
//        Watchdog_Feed();
    }

    printf("----  test nor flash: %d errors\r\n", err_cnt);

#if 0
    // Erase the whole chip
    add = 0;
    NOR_ChipErase();

    // Write the data
    add = 0;
    for (i = 0; i < 1; i++) {
        index = 0;
        for (j = 0; j < 128; j++) {
            temp[j] = index ++;
        }
        NOR_pageProgram(add, 32, (I8U *)buf);
        add += 128;
    }

    // Read out the data
    add = 0;
    for (i = 0; i < 30; i++) {
        NOR_readData(add, 8, (I8U *)buf);
        N_SPRINTF("%02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x,",
                  temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]);
        add += 8;
    }
#endif
    return;

    // Read out the data
    add = 0;
    err_cnt = 0;
    for (i = 0; i < 4096; i++) {
        index = 0;
        for (j = 0; j < 32; j++) {
            buf[j] = 0;
        }
        p->page_program(p, add, 128, buf);
//        NOR_readData(add, 128, (I8U *)buf);

        for (j = 0; j < 128; j++) {
            if (j != temp[j]) {
                err_cnt ++;
                //N_SPRINTF("%02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x,",
                //temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]);
            }
        }

        add += 128;
    }
    printf("---- first test: %d errors", err_cnt);

#if 0

    N_SPRINTF("---- second test ---------");
    // Erase 32 K block
    add = 0;
    NOR_ChipErase();

    // Write the data
    add = 0;
    index = 0x11;
    for (i = 0; i < 30; i++) {
        for (j = 0; j < 8; j++) {
            temp[j] = index ++;
        }
        NOR_pageProgram(add, 8, (I8U *)buf);
        add += 8;
    }

    // Read out the data
    add = 0;
    for (i = 0; i < 30; i++) {
        NOR_readData(add, 8, (I8U *)buf);
        N_SPRINTF("%02x, %02x, %02x, %02x, %02x, %02x, %02x, %02x,",
                  temp[0], temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], temp[7]);
        add += 8;
    }
#endif
}
#endif

