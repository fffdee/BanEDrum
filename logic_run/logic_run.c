#include "logic_run.h"
#include "adc.h"
#include "dma.h"
#include "i2s.h"
#include "sdio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "bg_lcd.h"
#include "sd_card.h"
#include "usbd_cdc_if.h"
#include "delay.h"
#include "bg_flash_manager.h"
#include "bg_download.h"



void flash_write_func(uint8_t *data, uint16_t size){

		HAL_SPI_Transmit_DMA(&hspi1, data,size);
}


void flash_read_func(uint8_t *data, uint16_t size){

		HAL_SPI_Receive_DMA(&hspi1, data,size);
}


void flash_test()
{
		BG_flash_manager.Init(flash_write_func,flash_read_func);

		uint32_t total = BG_flash_manager.GetTotalByte(DEV_NOR);
		
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
	
}
void logic_init()

{
	
		BG_lcd.Init();
		BG_lcd.Clear(BLUE);
	
		BG_Download downloader = BG_Download_Init(BG_flash_manager.PageProgram,);
		
	
}


void logic_run()
{
	
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);

		delay_ms(1000);

		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);

		delay_ms(1000);
	
	
}


