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
#include "math.h"
#include "midi_controller.h"
#define BUFFER_SIZE2 256
int16_t sine_wave[BUFFER_SIZE2];

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



uint16_t i=0;
void BanGTsynth_Callback(void);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim2)
	{
//		if(++i == 1000)					// 当 i 加到 1000 时，则时间到达 1 s, 翻转 led 电平
//		{
//			i=0;
//			HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
//		}
		
	
		
	}
}


void GenerateSineWave(void) {
    for (int i = 0; i < BUFFER_SIZE2; i++) {
        sine_wave[i] = (int16_t)(sin((float)i * 2 * M_PI / BUFFER_SIZE2) * 32767);
    }
}

void flash_write_func(uint8_t *data, uint16_t size){

		HAL_SPI_Transmit_DMA(&hspi1, data,size);
}


void flash_read_func(uint8_t *data, uint16_t size){

		HAL_SPI_Receive_DMA(&hspi1, data,size);
}
extern uint8_t usb_Buf[512];
extern uint8_t revflag;
extern uint8_t usb_len ;
uint8_t send[2] = {0xff,0xff};
uint32_t address=0;
uint32_t readaddr=0;
uint8_t read[512];
uint8_t count;


void flash_test()
{
	
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
		
		#ifdef DOWNLOAD	
			if(revflag==1){
				
				BG_flash_manager.PageProgram(address,usb_Buf,usb_len,DEV_NOR);

				address+=usb_len;
			
				CDC_Transmit_FS(send, 1);
			}
		#else
			
			usb_printf("data %d is ------------------------------------------------------------------------------------------------------------------------\n",readaddr);
			BG_flash_manager.ReadData(readaddr, read, 256,DEV_NOR);
			for(uint16_t i=0;i<256;i++){
					usb_printf("%X ",read[i]);
					delay_us(50);
			}		
			readaddr+=256;
			usb_printf("\n" );
		  delay_ms(1000);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);

		delay_ms(1000);
#endif
	
}
//#define DOWNLOAD
uint8_t write[255] = {0};



void BanGTsynth_Callback(void){

		BG_MIDI_controller.Callback();

}



void logic_init()

{
	
		BG_lcd.Init();
		BG_lcd.Clear(BLUE);
		GenerateSineWave();
		BG_flash_manager.Init(flash_write_func,flash_read_func);
	#ifdef DOWNLOAD
  	BG_flash_manager.EraseAll(DEV_NOR);
	#endif

		delay_ms(3000);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);
		HAL_TIM_Base_Start_IT(&htim2);

	
}


void logic_run()
{
		uint8_t data[3] = {0x90, 36, 100};
    BG_MIDI_controller.MIDI_Handle(data, 3);
		delay_ms(1000);
	
}


