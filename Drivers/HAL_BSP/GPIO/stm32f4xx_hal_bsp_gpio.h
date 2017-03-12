#ifndef __STM32F4xx_HAL_BSP_GPIO_H
#define __STM32F4xx_HAL_BSP_GPIO_H

#include "stm32f427xx.h"
#include "stm32f4xx_hal.h"

uint8_t BSP_IO_Init(void);
uint8_t BSP_IO_ConfigPin(uint16_t IO_Pin, IO_ModeTypedef IO_Mode);
uint16_t BSP_IO_ReadPin(uint16_t IO_Pin);
void BSP_IO_ITClear(void);

#endif
