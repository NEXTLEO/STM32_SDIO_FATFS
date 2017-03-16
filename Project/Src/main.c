/**
  ******************************************************************************
  * @file    STM32_SDIO_FATFS/Project/Src/main.c 
  * @author  Laurence
  * @version V1.0.0.0
  * @date    01-22-2017 
  * @brief   This is a STM32 project for the STABILITY TESTING of SDIO+FatFs.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS SDFatFs;  						/* File system object for SD card logical drive */
FIL MyFile, fTest;     					/* File object */
char SDPath[4]; 						/* SD card logical drive path */
uint8_t ecgbuff[1500] = {0xFF, 0x00};

volatile uint32_t MonitoringTotalSeconds;
volatile uint32_t SamplingCount;
volatile uint8_t SamplingFlag;
volatile uint8_t TimeOutFlag;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static uint8_t HAL_SD_Test(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	UINT bw;
	volatile FRESULT res_lseek, res_write, res_sync;
	volatile unsigned long location;
	
	/* STM32F4xx HAL library initialization:
	   - Configure the Flash prefetch, instruction and Data caches
	   - Configure the Systick to generate an interrupt each 1 msec
	   - Set NVIC Group Priority to 4
	   - Global MSP (MCU Support Package) initialization
	*/
	HAL_Init();
	
	/* Configure the system clock to 160 MHz */
	SystemClock_Config();
	
	#ifdef SEGGER_PRINTF
	SEGGER_RTT_Init();
	SEGGER_RTT_printf(0,"\r HELLO SEGGER RTT \r\n");
	#endif
	
	//SD CardReader Power Off
	BSP_SD_CardReader_Power_Control_Init();
	BSP_SD_CardReader_Power_Control(CR_PWR_ON);

	BSP_LED_Init(LED_GRE);
	BSP_LED_Init(LED_RED);
	
	//Close Green Led and Red Led
	BSP_LED_Off(LED_GRE);
	BSP_LED_Off(LED_RED);
	
	HAL_SD_Test();
	TIMx_Config();
	
	/* Infinite loop */
	MonitoringTotalSeconds = 0;
	SamplingFlag = 0;
	while (1)
	{
		MonitoringTotalSeconds++;
		SamplingFlag = 1;
		
		if((SamplingFlag == 1) && (MonitoringTotalSeconds > 0))
		{
			location = (unsigned long)(1500 * (MonitoringTotalSeconds - 1));
			
			/* Moves the file write pointer to a new location*/
			res_lseek = f_lseek(&fTest, location);
			if(res_lseek == FR_OK)
			{
			}
			else
			{
				Error_Handler();
			}
			/*Writes data to file*/
			res_write = f_write(&fTest, ecgbuff, 1500, &bw);
			if(res_write == FR_OK)
			{
			}
			else
			{
				Error_Handler();
			}
			/*Flushes the cached information of a writing file*/
			res_sync = f_sync(&fTest);
			if(res_sync == FR_OK)
			{
			}
			else
			{
				Error_Handler();
			}
			
			SamplingFlag = 0;
		}
		
		if(TimeOutFlag == 1)
		{
			TimeOutFlag = 0;
			BSP_LED_Toggle(LED_GRE);
		}
	}
}

//SD TEST
uint8_t HAL_SD_Test(void)
{
	FRESULT res;                                          	/* FatFs function common result code */
	uint32_t byteswritten;									/* File write counts */
	uint8_t wtext[] = "This is STM32 working with FatFs"; 	/* File write buffer */
	
	/*##-1- Link the micro SD disk I/O driver ##################################*/
	if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
	{
	}
	else
	{
		/* FatFs Initialization Error */
		Error_Handler();
	}
	
	/*##-2- Register the file system object to the FatFs module ##############*/
	if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) != FR_OK)
	{
		/* FatFs Initialization Error */
		Error_Handler();
	}
	else
	{
	}
	
//	/*##-3- Create a FAT file system (format) on the logical drive #########*/
//	/* WARNING: Formatting the uSD card will delete all content on the device */
//	if(f_mkfs((TCHAR const*)SDPath, 0, 0) != FR_OK)
//	{
//		/* FatFs Format Error */
////		Error_Handler();
//		SEGGER_RTT_printf(0,"\f_mkfs ERROR\r\n");
//	}
//	else
//	{
//		SEGGER_RTT_printf(0,"\f_mkfs OK\r\n");
//	}
	
	/*##-4- Create and Open a new text file object with write access #####*/
	if(f_open(&MyFile, "0:/STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
	{
		/* 'STM32.TXT' file Open for write Error */
		Error_Handler();
	}
	else
	{
	}

	/*##-5- Write data to the text file ################################*/
	res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);
	if((byteswritten == 0) || (res != FR_OK))
	{
		/* 'STM32.TXT' file Write or EOF Error */
		Error_Handler();
	}
	else
	{
	}
	
	/*##-6- Close the open text file #################################*/
	f_close(&MyFile);
	
	/*##-7- Open the text file object with read access ###############*/
	if(f_open(&fTest, "0:/TEST.bin", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
	{
		/* 'STM32.TXT' file Open for read Error */
		Error_Handler();
	}
	else
	{
	}
	
	/*##-8- Read data from the text file ###########################*/
	if(f_sync(&fTest) != FR_OK)
	{
		/* 'STM32.TXT' file Read or EOF Error */
		Error_Handler();
	}
	else
	{
		BSP_LED_On(LED_GRE);
	}
	
	return 0;
}

uint32_t timecounter = 0;
/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim: TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	timecounter++;
	if(timecounter >= 1000)
	{
		timecounter = 0;
		TimeOutFlag = 1;
	}
}

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 160000000
  *            HCLK(Hz)                       = 160000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 160
  *            PLL_P                          = 2
  *            PLL_Q                          = 8
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is 
	 clocked below the maximum system frequency, to update the voltage scaling value 
	 regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/*
	PLL_VCO input clock = (HSE_VALUE or HSI_VALUE / PLL_M)
	PLL_VCO output clock = (PLL_VCO input clock) * PLL_N
	System Clock = (PLL_VCO output clock) / PLL_P
	USB / SDIO Clock = (PLL VCO output clock) / PLL_Q
	*/

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	
	RCC_OscInitStruct.PLL.PLLN = 160;	//External Oscillator = 16MHz
	
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 8;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	/* Activate the Over-Drive mode */
//	HAL_PWREx_EnableOverDrive();

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
	 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn RED LED on */
  BSP_LED_On(LED_RED);
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
