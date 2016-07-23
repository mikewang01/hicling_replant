/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_PINNAMES_H
#define MBED_PINNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

#define _PIN_NAME_PORT_OFFSET_          8
#define GENERATE_PIN_NAME(PORT, PINNUM)  (uint16_t)((PORT<<8) | (PINNUM))
#define GET_PORT_NUM(PINNAME)            (uint8_t)(((uint16_t)(PINNAME>>8))&0X00FF)
#define GET_PIN_NUM(PINNAME)             (uint8_t)(((uint16_t)(PINNAME))&000XFF)

#define
typedef enum {
    PIN_INPUT,
    PIN_OUTPUT,
    PIN_INPUT_OUTPUT    //pin state can be set and read back
} PinDirection;

typedef enum {
    P00  = GENERATE_PIN_NAME(0, 0),
    P01  ,
    P02  ,
    P03  ,
    P04  ,
    P05  ,
    P06  ,
    P07  ,
    P10  = GENERATE_PIN_NAME(1, 0),
    P11  ,
    P12  ,
    P13  ,
    P14  ,
    P15  ,
    P16  ,
    P17  ,

    P20  = GENERATE_PIN_NAME(2, 0),
    P21  ,
    P22  ,
    P23  ,
    P24  ,
    P25  ,
    P27  ,

    P30  = GENERATE_PIN_NAME(3, 0),
    P31  ,
    P32  ,
    P33  ,
    P34  ,
    P35  ,
    P36  ,
    P37  ,

    P40  = GENERATE_PIN_NAME(4, 0),
    P41  ,
    P42  ,
    P43  ,
    P44  ,
    P45  ,
    P46  ,
    P47  ,
    GPIO_SPI_SCK = NC,
     GPIO_UART2_TX = NC,
     GPIO_FILE_DON = NC,
     GPIO_SPI_CS_NFLASH = NC,
     GPIO_CHARGER_PG = NC,
     GPIO_SPI_MISO = NC,
     GPIO_SPI_MOSI = NC,
     GPIO_SENSOR_INT_2 = NC,

    // GPIO PORT 1
     GPIO_OLED_RST = NC,
     GPIO_SENSOR_INT_1  = NC,
     GPIO_TWI_CLK  = NC,
     GPIO_TWI_DATA   = NC,

    // GPIO PORT 2
     GPIO_TOUCH_INT   = NC,
     GPIO_SPI_CS_ACC  = NC,
     GPIO_CHARGER_CHGFLAG = NC,
     GPIO_VIBRATOR_EN    = NC,
     GPIO_UART2_RX       = NC,
     GPIO_RESERVE_5      = NC,
     GPIO_RESERVE_6      = NC,
     GPIO_RESERVE_7      = NC,
     GPIO_UART1_RX        = NC,
     GPIO_UART1_TX       = NC,

    // Not connected
    NC = (int)0xFFFFFFFF


} PinName;

/*
#define GPIO_SPI_SCK         GPIO_PIN_0
#define GPIO_UART2_TX        GPIO_PIN_1
#define GPIO_FILE_DON        GPIO_PIN_2
#define GPIO_SPI_CS_NFLASH   GPIO_PIN_3
#define GPIO_CHARGER_PG      GPIO_PIN_4
#define GPIO_SPI_MISO        GPIO_PIN_5
#define GPIO_SPI_MOSI        GPIO_PIN_6
#define GPIO_SENSOR_INT_2    GPIO_PIN_7

// GPIO PORT 1
#define GPIO_OLED_RST        GPIO_PIN_0
#define GPIO_SENSOR_INT_1    GPIO_PIN_1
#define GPIO_TWI_CLK         GPIO_PIN_2
#define GPIO_TWI_DATA        GPIO_PIN_3

// GPIO PORT 2
#define GPIO_TOUCH_INT       GPIO_PIN_0
#define GPIO_SPI_CS_ACC      GPIO_PIN_1
#define GPIO_CHARGER_CHGFLAG GPIO_PIN_2
#define GPIO_VIBRATOR_EN     GPIO_PIN_3
#define GPIO_UART2_RX        GPIO_PIN_4
#define GPIO_RESERVE_5       GPIO_PIN_5
#define GPIO_RESERVE_6       GPIO_PIN_6
#define GPIO_RESERVE_7       GPIO_PIN_7
#define GPIO_UART1_RX        GPIO_PIN_8
#define GPIO_UART1_TX        GPIO_PIN_9
*/
typedef enum {
    PullNone = 0,
    PullUp = 1,
    PullDown = 2,
    PullDefault = PullUp
} PinMode;

#ifdef __cplusplus
}
#endif

#endif
