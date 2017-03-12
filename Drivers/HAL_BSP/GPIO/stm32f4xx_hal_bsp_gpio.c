#include "stm32f4xx_hal_bsp_gpio.h"

/**
  * @brief  Initializes and configures the IO functionalities and configures all
  *         necessary hardware resources (GPIOs, clocks..).
  * @note   BSP_IO_Init() is using HAL_Delay() function to ensure that stmpe1600
  *         IO Expander is correctly reset. HAL_Delay() function provides accurate
  *         delay (in milliseconds) based on variable incremented in SysTick ISR. 
  *         This implies that if BSP_IO_Init() is called from a peripheral ISR process,
  *         then the SysTick interrupt must have higher priority (numerically lower)
  *         than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
  * @retval IO_OK if all initializations are OK. Other value if error.
  */
uint8_t BSP_IO_Init(void)
{
	
	return 0;
}

/**
  * @brief  Configures the IO pin(s) according to IO mode structure value.
  * @param  IO_Pin: IO pin(s) to be configured. 
  *          This parameter can be one of the following values:
  *            @arg  STMPE1600_PIN_x: where x can be from 0 to 15.
  * @param  IO_Mode: IO pin mode to configure
  *          This parameter can be one of the following values:
  *            @arg  IO_MODE_INPUT
  *            @arg  IO_MODE_OUTPUT
  *            @arg  IO_MODE_IT_RISING_EDGE
  *            @arg  IO_MODE_IT_FALLING_EDGE
  * @retval IO_OK if all initializations are OK. Other value if error.  
  */
uint8_t BSP_IO_ConfigPin(uint16_t IO_Pin, IO_ModeTypedef IO_Mode)
{
	
	return 0;
}

/**
  * @brief  Gets the selected pins current state.
  * @param  IO_Pin: Selected pins to read. 
  *          This parameter can be any combination of the IO pins. 
  * @retval The current pins state 
  */
uint16_t BSP_IO_ReadPin(uint16_t IO_Pin)
{
	HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
}

/**
  * @brief  Clears all the IO IT pending bits.
  */
void BSP_IO_ITClear(void)
{
	
}	






