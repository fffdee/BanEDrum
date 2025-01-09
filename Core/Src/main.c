/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2s.h"
#include "sdio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#include "bg_lcd.h"
#include "sd_card.h"

#include "usbd_cdc_if.h"
#include "delay.h"
#include "bg_flash_manager.h"
extern uint8_t CDC_Transmit_FS(uint8_t* Buf, uint16_t Len);


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

void adc_handle()
{
	uint32_t ADC_Value[4];
	//		HAL_ADC_Start(&hadc1);	
		HAL_ADC_PollForConversion(&hadc1,50);
		for(uint8_t i=0;i<4;i++)
	  {
			if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))
	  		{							

					ADC_Value[i] = HAL_ADC_GetValue(&hadc1);
	 		}
	  		HAL_ADC_Stop (&hadc1);//��ADC
				
     
		}
		CDC_Transmit_FS((uint8_t*)ADC_Value, 16);
}


void flash_write_func(uint8_t *data, uint16_t size){

		HAL_SPI_Transmit_DMA(&hspi1, data,size);
}


void flash_read_func(uint8_t *data, uint16_t size){

		HAL_SPI_Receive_DMA(&hspi1, data,size);
}


/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2S2_Init();
  MX_SPI1_Init();
  MX_SDIO_SD_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USB_DEVICE_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	BG_lcd.Init();
	BG_lcd.Clear(BLUE);
//	delay_ms(2000);
//	read_W25Q128_ID();
//	delay_ms(500);
//	W25Q128_test();
	BG_flash_manager.Init(flash_write_func,flash_read_func);
//	BG_flash_manager.EraseAll(DEV_NOR);
	//usb_printf("erase ok!\n");
	//delay_ms(2000);
	uint32_t total = BG_flash_manager.GetTotalByte(DEV_NOR);
	
//	get_sd_info();
////	sd_write_test();
//	sd_read_test();

    uint8_t writeData[7] = {1,2,3,4,5,6,7};
    uint8_t readData[50]= {0};
    uint32_t writeAddress = 4090; // 请确保写入地址在W25Q64的有效范围内
		usb_printf("total is : %d!\n",total);

		uint8_t manufacturerID, memoryType, deviceID;
    BG_flash_manager.ReadID(&manufacturerID, &memoryType, &deviceID,DEV_NOR);
		delay_ms(10);
		usb_printf("ID is %d  %d  %d \n",manufacturerID ,memoryType ,deviceID);
    // 写入数据
    BG_flash_manager.PageProgram(writeAddress, writeData, sizeof(writeData),DEV_NOR);
		
		 BG_flash_manager.GetRemainingCapacity(DEV_NOR);
	

		
	

		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);

			//读数据，看原始存在的数据
		BG_flash_manager.ReadData(writeAddress, readData, 50,DEV_NOR);
		for(uint8_t i=0;i<sizeof(writeData);i++){
				usb_printf("%d\n",readData[i]);
			delay_us(50);
		}
		//BG_flash_manager.EraseAll(DEV_NOR);
		//usb_printf("erase ok!\n");			
			
		//擦除需要写数据所在的扇区
		BG_flash_manager.DataErase(writeAddress,50,DEV_NOR);
		BG_flash_manager.ReadData(writeAddress, readData, 50,DEV_NOR);
		for(uint8_t i=0;i<sizeof(writeData);i++){
				usb_printf("%d\n",readData[i]);
			delay_us(50);
		}

		BG_flash_manager.PageProgram(writeAddress,writeData,sizeof(writeData),DEV_NOR);
		BG_flash_manager.ReadData(writeAddress, readData, 50,DEV_NOR);
		for(uint8_t i=0;i<sizeof(writeData);i++){
				usb_printf("%d\n",readData[i]);
			delay_us(50);
		}		
		
  while (1)
  {
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);

		delay_ms(1000);

		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);

		delay_ms(1000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
//  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: usb_printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
