#ifndef __MZ_ECG3_TIMER_H__
#define __MZ_ECG3_TIMER_H__

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_bsp_led.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* User can use this section to tailor TIMx instance used and associated 
   resources */
/* Definition for TIMx clock resources */
#define TIMx                           TIM3
#define TIMx_CLK_ENABLE                __HAL_RCC_TIM3_CLK_ENABLE

/* Definition for TIMx's NVIC */
#define TIMx_IRQn                      TIM3_IRQn
#define TIMx_IRQHandler                TIM3_IRQHandler

/* Exported functions ------------------------------------------------------- */


void TIMx_Config(void);

#endif
