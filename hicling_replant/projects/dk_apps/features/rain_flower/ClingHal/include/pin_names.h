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
#include  "hw_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PIN_VOLTAGE_CHANGE_SUPPORTED    (1)
#define _PIN_NAME_PORT_OFFSET_          8
#define GENERATE_PIN_NAME(PORT, PINNUM)  (uint16_t)((PORT<<8) | (PINNUM))
#define GET_PORT_NUM(PINNAME)            (uint8_t)(((uint16_t)(PINNAME>>8))&0X00FF)
#define GET_PIN_NUM(PINNAME)             (uint8_t)(((uint16_t)(PINNAME))&0X00FF)

typedef enum {
        PIN_INPUT = HW_GPIO_MODE_INPUT, /**< GPIO as an input */
        PIN_INPUT_PULL_UP = HW_GPIO_MODE_INPUT_PULLUP,/**< GPIO as an input with pull-up */
        PIN_INPUT_PULL_DOWN = HW_GPIO_MODE_INPUT_PULLDOWN,/**< GPIO as an input with pull-down */
        PIN_OUTPUT = HW_GPIO_MODE_OUTPUT,/**< GPIO as an (implicitly push-pull) output */
        PIN_OUTPUT_PUSH_PULL = HW_GPIO_MODE_OUTPUT_PUSH_PULL,/**< GPIO as an (explicitly push-pull) output */
        PIN_OUTPUT_OPEN_DRAIN = HW_GPIO_MODE_OUTPUT_OPEN_DRAIN /**< GPIO as an open-drain out*/ //pin state can be set and read back
} PinDirection;

#if PIN_VOLTAGE_CHANGE_SUPPORTED
typedef enum {
        PIN_POWER_V33 = HW_GPIO_POWER_V33, /**< V33 (3.3 V) power rail */
        PIN_POWER_POWER_VDD1V8P = HW_GPIO_POWER_VDD1V8P, /**< VDD1V8P (1.8 V) power rail */
} PinVoltageLevel;
#endif

typedef enum {
        PIN_FUNC_NC = 0XFFFF, /*if function below not exised, set it to 0xffff*/
        PIN_FUNC_GPIO = HW_GPIO_FUNC_GPIO, /**< GPIO */
        PIN_FUNC_UART_RX = HW_GPIO_FUNC_UART_RX, /**< GPIO as UART RX */
        PIN_FUNC_UART_TX = HW_GPIO_FUNC_UART_TX, /**< GPIO as UART TX */
        PIN_FUNC_UART_IRDA_RX = HW_GPIO_FUNC_UART_IRDA_RX, /**< GPIO as UART IRDA RX */
        PIN_FUNC_UART_IRDA_TX = HW_GPIO_FUNC_UART_IRDA_TX, /**< GPIO as UART IRDA TX */
        PIN_FUNC_UART2_RX = HW_GPIO_FUNC_UART2_RX, /**< GPIO as UART2 RX */
        PIN_FUNC_UART2_TX = HW_GPIO_FUNC_UART2_TX, /**< GPIO as UART2 TX */
        PIN_FUNC_UART2_IRDA_RX = HW_GPIO_FUNC_UART2_IRDA_RX, /**< GPIO as UART2 IRDA RX */
        PIN_FUNC_UART2_IRDA_TX = HW_GPIO_FUNC_UART2_IRDA_TX, /**< GPIO as UART2 IRDA TX */
        PIN_FUNC_UART2_CTSN = HW_GPIO_FUNC_UART2_CTSN, /**< GPIO as UART2 CTSN */
        PIN_FUNC_UART2_RTSN = HW_GPIO_FUNC_UART2_RTSN, /**< GPIO as UART2 RTSN */
        PIN_FUNC_SPI_DI = HW_GPIO_FUNC_SPI_DI, /**< GPIO as SPI DI */
        PIN_FUNC_SPI_DO = HW_GPIO_FUNC_SPI_DO, /**< GPIO as SPI DO */
        PIN_FUNC_SPI_CLK = HW_GPIO_FUNC_SPI_CLK, /**< GPIO as SPI CLK */
        PIN_FUNC_SPI_EN = HW_GPIO_FUNC_SPI_EN, /**< GPIO as SPI EN */
        PIN_FUNC_SPI2_DI = HW_GPIO_FUNC_SPI2_DI, /**< GPIO as SPI2 DI */
        PIN_FUNC_SPI2_DO = HW_GPIO_FUNC_SPI2_DO, /**< GPIO as SPI2 DO */
        PIN_FUNC_SPI2_CLK = HW_GPIO_FUNC_SPI2_CLK, /**< GPIO as SPI2 CLK */
        PIN_FUNC_SPI2_EN = HW_GPIO_FUNC_SPI2_EN, /**< GPIO as SPI2 EN */
        PIN_FUNC_I2C_SCL = HW_GPIO_FUNC_I2C_SCL, /**< GPIO as I2C SCL */
        PIN_FUNC_I2C_SDA = HW_GPIO_FUNC_I2C_SDA, /**< GPIO as I2C SDA */
        PIN_FUNC_I2C2_SCL = HW_GPIO_FUNC_I2C2_SCL, /**< GPIO as I2C2 SCL */
        PIN_FUNC_I2C2_SDA = HW_GPIO_FUNC_I2C2_SDA, /**< GPIO as I2C2 SDA */
        PIN_FUNC_PWM0 = HW_GPIO_FUNC_PWM0, /**< GPIO as PWM0 */
        PIN_FUNC_PWM1 = HW_GPIO_FUNC_PWM1, /**< GPIO as PWM1 */
        PIN_FUNC_PWM2 = HW_GPIO_FUNC_PWM2, /**< GPIO as PWM2 */
        PIN_FUNC_PWM3 = HW_GPIO_FUNC_PWM3, /**< GPIO as PWM3 */
        PIN_FUNC_PWM4 = HW_GPIO_FUNC_PWM4, /**< GPIO as PWM4 */
        PIN_FUNC_BLE_DIAG = HW_GPIO_FUNC_BLE_DIAG, /**< GPIO as BLE DIAG */
        PIN_FUNC_FTDF_DIAG = HW_GPIO_FUNC_FTDF_DIAG, /**< GPIO as FTDF DIAG */
        PIN_FUNC_PCM_DI = HW_GPIO_FUNC_PCM_DI, /**< GPIO as PCM DI */
        PIN_FUNC_PCM_DO = HW_GPIO_FUNC_PCM_DO, /**< GPIO as PCM DO */
        PIN_FUNC_PCM_FSC = HW_GPIO_FUNC_PCM_FSC, /**< GPIO as PCM FSC */
        PIN_FUNC_PCM_CLK = HW_GPIO_FUNC_PCM_CLK, /**< GPIO as PCM CLK */
        PIN_FUNC_PDM_DI = HW_GPIO_FUNC_PDM_DI, /**< GPIO as PDM DI */
        PIN_FUNC_PDM_DO = HW_GPIO_FUNC_PDM_DO, /**< GPIO as PDM DO */
        PIN_FUNC_PDM_CLK = HW_GPIO_FUNC_PDM_CLK, /**< GPIO as PDM CLK */
        PIN_FUNC_USB_SOF = HW_GPIO_FUNC_USB_SOF, /**< GPIO as USB SOF */
        PIN_FUNC_ADC = HW_GPIO_FUNC_ADC, /**< GPIO as ADC */
        PIN_FUNC_USB = HW_GPIO_FUNC_USB, /**< GPIO as USB */
        PIN_FUNC_QSPI = HW_GPIO_FUNC_QSPI, /**< GPIO as QSPI */
        PIN_FUNC_XTAL32 = HW_GPIO_FUNC_XTAL32, /**< GPIO as XTAL32 */
        PIN_FUNC_QUADEC_XA = HW_GPIO_FUNC_QUADEC_XA, /**< GPIO as QUADEC XA */
        PIN_FUNC_QUADEC_XB = HW_GPIO_FUNC_QUADEC_XB, /**< GPIO as QUADEC XB */
        PIN_FUNC_QUADEC_YA = HW_GPIO_FUNC_QUADEC_YA, /**< GPIO as QUADEC YA */
        PIN_FUNC_QUADEC_YB = HW_GPIO_FUNC_QUADEC_YB, /**< GPIO as QUADEC YB */
        PIN_FUNC_QUADEC_ZA = HW_GPIO_FUNC_QUADEC_ZA, /**< GPIO as QUADEC ZA */
        PIN_FUNC_QUADEC_ZB = HW_GPIO_FUNC_QUADEC_ZB, /**< GPIO as QUADEC ZB */
        PIN_FUNC_IR_OUT = HW_GPIO_FUNC_IR_OUT, /**< GPIO as IR OUT */
        PIN_FUNC_BREATH = HW_GPIO_FUNC_BREATH, /**< GPIO as BREATH */
        PIN_FUNC_KB_ROW = HW_GPIO_FUNC_KB_ROW, /**< GPIO as KB ROW */
        PIN_FUNC_COEX_EXT_ACT0 = HW_GPIO_FUNC_COEX_EXT_ACT0, /**< GPIO as COEX EXT ACT0 */
        PIN_FUNC_COEX_EXT_ACT1 = HW_GPIO_FUNC_COEX_EXT_ACT1, /**< GPIO as COEX EXT ACT1 */
        PIN_FUNC_COEX_SMART_ACT = HW_GPIO_FUNC_COEX_SMART_ACT, /**< GPIO as COEX SMART ACT */
        PIN_FUNC_COEX_SMART_PRI = HW_GPIO_FUNC_COEX_SMART_PRI, /**< GPIO as COEX SMART PRI */
        PIN_FUNC_CLOCK = HW_GPIO_FUNC_CLOCK, /**< GPIO as CLOCK */
        PIN_FUNC_ONESHOT = HW_GPIO_FUNC_ONESHOT, /**< GPIO as ONESHOT */
        PIN_FUNC_PWM5 = HW_GPIO_FUNC_PWM5, /**< GPIO as PWM5 */
        PIN_FUNC_PORT0_DCF = HW_GPIO_FUNC_PORT0_DCF, /**< GPIO as PORT0 DCF */
        PIN_FUNC_PORT1_DCF = HW_GPIO_FUNC_PORT1_DCF, /**< GPIO as PORT1 DCF */
        PIN_FUNC_PORT2_DCF = HW_GPIO_FUNC_PORT2_DCF, /**< GPIO as PORT2 DCF */
        PIN_FUNC_PORT3_DCF = HW_GPIO_FUNC_PORT3_DCF, /**< GPIO as PORT3 DCF */
        PIN_FUNC_PORT4_DCF = HW_GPIO_FUNC_PORT4_DCF, /**< GPIO as PORT4 DCF */
        PIN_FUNC_RF_ANT_TRIM0 = HW_GPIO_FUNC_RF_ANT_TRIM0, /**< GPIO as RF ANT TRIM0 */
        PIN_FUNC_RF_ANT_TRIM1 = HW_GPIO_FUNC_RF_ANT_TRIM1, /**< GPIO as RF ANT TRIM1 */
        PIN_FUNC_RF_ANT_TRIM2 = HW_GPIO_FUNC_RF_ANT_TRIM2 /**< GPIO as RF ANT TRIM2 */
} PIN_FUNC;

typedef enum {
        // Not connected
        NC = (int)0xFFFFFFFF,
        P00 = GENERATE_PIN_NAME(0, 0),
        P01,
        P02,
        P03,
        P04,
        P05,
        P06,
        P07,
        P10 = GENERATE_PIN_NAME(1, 0),
        P11,
        P12,
        P13,
        P14,
        P15,
        P16,
        P17,

        P20 = GENERATE_PIN_NAME(2, 0),
        P21,
        P22,
        P23,
        P24,
        P25,
        P27,

        P30 = GENERATE_PIN_NAME(3, 0),
        P31,
        P32,
        P33,
        P34,
        P35,
        P36,
        P37,

        P40 = GENERATE_PIN_NAME(4, 0),
        P41,
        P42,
        P43,
        P44,
        P45,
        P46,
        P47,
        /*test pin*/
        GPIO_TEST_PIN = P07,

        GPIO_UART2_TX = P13,
        GPIO_UART2_RX = P23,
        /*retarget uart pin group*/
        GPIO_UART_DBG_TX = P13,
        GPIO_UART_DBG_RX = P23,

        GPIO_FILE_DON = NC,

        /*spi device cs group*/
        GPIO_SPI_CS_ACC = NC,
        OLED_SPI_CS = P35,
        LIS3DH_SPI_CS = P36,
        GPIO_SPI_CS_NFLASH = NC,
        /*spi bus pin assignment group*/
        GPIO_SPI_SCK = NC,
        GPIO_SPI_MISO = NC,
        GPIO_SPI_MOSI = NC,

        GPIO_CHARGER_PG = NC,
        GPIO_SENSOR_INT_2 = NC,
        GPIO_OLED_RST = NC,
        GPIO_SENSOR_INT_1 = NC,
        GPIO_TWI_CLK = NC,
        GPIO_TWI_DATA = NC,

        GPIO_TOUCH_INT = NC,
        GPIO_CHARGER_CHGFLAG = NC,
        GPIO_VIBRATOR_EN = NC,
        GPIO_RESERVE_5 = NC,
        GPIO_RESERVE_6 = NC,
        GPIO_RESERVE_7 = NC,

        GPIO_UART1_RX = NC,
        GPIO_UART1_TX = NC,

} PinName;

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
