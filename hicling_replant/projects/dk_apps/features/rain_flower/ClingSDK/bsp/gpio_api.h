/***************************************************************************//**
 * File gpio.h
 * 
 * Description: User Interface header
 *
 *
 ******************************************************************************/

#ifndef __GPIO_HEADER__
#define __GPIO_HEADER__

#include "standards.h"

// GPIO PORT 0
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

// Initialization
void GPIO_api_init(void);

// ADC analog pin configuration for battery
I16U GPIO_vbat_adc_reading(void);

// TWI pin configuration
void GPIO_twi_init(I8U twi_master_instance);
void GPIO_twi_enable(I8U twi_master_instance);
void GPIO_twi_disable(I8U twi_master_instance);

// Vibrator control
void GPIO_vibrator_set(BOOLEAN b_on);
void GPIO_vibrator_on_block(I8U latency);

// Interrupt control
void GPIO_interrupt_enable(void);

// Charger reset
void GPIO_charger_reset(void);
void GPIO_system_powerup(void);
void GPIO_system_powerdown(void);

// Interrupt handle 
void GPIO_interrupt_handle(void);

#endif
