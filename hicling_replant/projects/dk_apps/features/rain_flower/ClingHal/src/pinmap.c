/******************************************************************************
 * Copyright 2013-2016 hicling Systems (MikeWang)
 *
 * FileName: pinmap.c
 *
 * Description:  abstract a layer between rtos and cling sdk for pin driver.
 *
 * Modification history:
 *     2016/7/24, v1.0 create this file mike.
 *******************************************************************************/
#include "pinmap.h"
static const PinMap cling_pin_assign[] =
        {
                { .pin = GPIO_SPI_SCK, .direction = PIN_OUTPUT, .function = PIN_FUNC_SPI_CLK,
                        .voltage = PIN_POWER_V33 },
                { .pin = GPIO_SPI_MISO, .direction = PIN_INPUT, .function = PIN_FUNC_SPI_DI,
                        .voltage = PIN_POWER_V33 },
                { .pin = GPIO_SPI_MOSI, .direction = PIN_OUTPUT, .function = PIN_FUNC_SPI_DO,
                        .voltage = PIN_POWER_V33 },
                { .pin = GPIO_SPI_CS_NFLASH, .direction = PIN_OUTPUT, .function = PIN_FUNC_GPIO,
                        .voltage = PIN_POWER_V33 },
                { .pin = GPIO_SPI_CS_ACC, .direction = PIN_OUTPUT, .function = PIN_FUNC_GPIO,
                        .voltage = PIN_POWER_V33 },
                { .pin = OLED_SPI_CS, .direction = PIN_OUTPUT, .function = PIN_FUNC_GPIO,
                        .voltage = PIN_POWER_V33 },
                { .pin = LIS3DH_SPI_CS, .direction = PIN_OUTPUT, .function = PIN_FUNC_GPIO,
                        .voltage = PIN_POWER_V33 },
                { .pin = GPIO_UART_DBG_TX, .direction = PIN_OUTPUT, .function = PIN_FUNC_UART_TX,
                        .voltage = PIN_POWER_V33 },
                { .pin = GPIO_UART_DBG_RX, .direction = PIN_INPUT, .function = PIN_FUNC_UART_RX,
                        .voltage = PIN_POWER_V33 },
                { .pin = GPIO_TEST_PIN, .direction = PIN_OUTPUT_PUSH_PULL, .function = PIN_FUNC_GPIO,
                        .voltage = PIN_POWER_V33 },

        };
/*
 *
 *
 * Attenion: this functioon must obly be called by dialog preph initiated procedure
 *
 *
 * */
void pin_map_init()
{
        for (int i = 0; i < sizeof(cling_pin_assign) / sizeof(PinMap); i++)
                {
                if (cling_pin_assign[i].pin != NC) {
                        hw_gpio_set_pin_function(GET_PORT_NUM(cling_pin_assign[i].pin),
                                GET_PIN_NUM(cling_pin_assign[i].pin), cling_pin_assign[i].direction,
                                cling_pin_assign[i].function);
                        hw_gpio_configure_pin_power(GET_PORT_NUM(cling_pin_assign[i].pin),
                                GET_PIN_NUM(cling_pin_assign[i].pin), cling_pin_assign[i].voltage);
                }

        }
}

void pin_set_high(PinName pin)
{
        hw_gpio_set_active(GET_PORT_NUM(pin), GET_PIN_NUM(pin));
}

void pin_set_low(PinName pin)
{
        hw_gpio_set_inactive(GET_PORT_NUM(pin), GET_PIN_NUM(pin));
}

int pin_get_status(PinName pin)
{
        return (int)hw_gpio_get_pin_status(GET_PORT_NUM(pin), GET_PIN_NUM(pin));
}

void pin_toggle(PinName pin)
{
        hw_gpio_toggle(GET_PORT_NUM(pin), GET_PIN_NUM(pin));

}
