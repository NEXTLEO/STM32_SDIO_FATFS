#ifndef __STM32F4xx_HAL_BSP_LED_H
#define __STM32F4xx_HAL_BSP_LED_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#define LEDn                                       2
  
#define LED_GRE_PIN                                GPIO_PIN_10
#define LED_GRE_GPIO_PORT                          GPIOE
#define LED_GRE_GPIO_CLK_ENABLE()                  __GPIOE_CLK_ENABLE()  
#define LED_GRE_GPIO_CLK_DISABLE()                 __GPIOE_CLK_DISABLE()  

#define LED_RED_PIN                                GPIO_PIN_12
#define LED_RED_GPIO_PORT                          GPIOE
#define LED_RED_GPIO_CLK_ENABLE()                  __GPIOE_CLK_ENABLE()  
#define LED_RED_GPIO_CLK_DISABLE()                 __GPIOE_CLK_DISABLE()  

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)  do{if((__INDEX__) == 0) LED_GRE_GPIO_CLK_ENABLE(); else \
                                            if((__INDEX__) == 1) LED_RED_GPIO_CLK_ENABLE(); \
                                            }while(0)
#define LEDx_GPIO_CLK_DISABLE(__INDEX__) do{if((__INDEX__) == 0) LED_GRE_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 1) LED_RED_GPIO_CLK_DISABLE(); \
                                            }while(0)

typedef enum 
{
  LED_GRE = 0,
  LED_RED = 1
}Led_TypeDef;


void BSP_LED_Init(Led_TypeDef Led);
void BSP_LED_On(Led_TypeDef Led);
void BSP_LED_Off(Led_TypeDef Led);
void BSP_LED_Toggle(Led_TypeDef Led);


#ifdef __cplusplus
}
#endif

#endif
