#ifndef RC522_H_
#define RC522_H_

#include "stm32f4xx_spi.h"
#include "stm32f4xx_gpio.h"

#define VERSION_REG 0x37



uint8_t SPI_send(uint8_t data);
uint8_t get_register(uint8_t address);
uint8_t get_version();
void delay(uint32_t time);

#endif RC522_H_
