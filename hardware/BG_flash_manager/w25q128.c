#include "W25Q128.h"
#include "spi.h"
#include "usart.h"
#include <stdio.h>
#include "delay.h"
#include "usbd_cdc_if.h"
uint32_t FLASH_SIZE=16*1024*1024;	//FLASH 大小为16M字节
uint32_t Data_Address = 4090; //测试地址 250（地址在两页之间） 和 4090 （地址在两扇区并且两页之间）
 
//要写的数据
uint8_t Write_data[]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x41};
#define Write_data_SIZE sizeof(Write_data)
 
//要读的数据
uint8_t Read_data[100] = {0};
#define Read_data_SIZE sizeof(Read_data)
 
/* Nicky ******************************************************************* */
//器件使能
void W25Q128_Enable()
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET);
}
 
/* Nicky ******************************************************************* */
//器件失能
void W25Q128_Disable()
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);
}
 
/* Nicky ******************************************************************* */
//SPI2 发送 1 个字节数据
void spi2_Transmit_one_byte(uint8_t _dataTx)
{
	HAL_SPI_Transmit_DMA(&hspi1,(uint8_t*) &_dataTx,1);
	delay_us(50);
}
 
/* Nicky ******************************************************************* */
//SPI2 接收 1 个字节数据
uint8_t spi2_Receive_one_byte()
{
	uint16_t _dataRx;
	HAL_SPI_Receive_DMA(&hspi1,(uint8_t*) &_dataRx, 1);
	delay_us(50);
	return _dataRx;
}
 
/* Nicky ******************************************************************* */
//W25Q128写使能,将WEL置1 
void W25Q128_Write_Enable()   
{
	W25Q128_Enable();                            //使能器件   
    spi2_Transmit_one_byte(0x06); 
	W25Q128_Disable();                            //取消片选     	      
}
 
/* Nicky ******************************************************************* */
//W25Q128写失能,将WEL置0 
void W25Q128_Write_Disable()   
{
	W25Q128_Enable();                            //使能器件   
    spi2_Transmit_one_byte(0x04); 
	W25Q128_Disable();                            //取消片选     	      
}
 
/* Nicky ******************************************************************* */
//读取寄存器状态
uint8_t W25Q128_ReadSR(void)   
{  
	uint8_t byte=0;   
	W25Q128_Enable();                            //使能器件   
	spi2_Transmit_one_byte(0x05);    //发送读取状态寄存器命令
	byte=spi2_Receive_one_byte();             //读取一个字节
	W25Q128_Disable();                           //取消片选     
	return byte;   
} 
 
/* Nicky ******************************************************************* */
//等待空闲
void W25Q128_Wait_Busy()   
{   
	while((W25Q128_ReadSR()&0x01)==0x01);   // 等待BUSY位清空
}
 
/* Nicky ******************************************************************* */
//擦除地址所在的一个扇区
void Erase_one_Sector(uint32_t Address)
{
	W25Q128_Write_Enable();                  //SET WEL 	 
	W25Q128_Wait_Busy(); 		
	W25Q128_Enable();                            //使能器件 
	spi2_Transmit_one_byte(0x20);      //发送扇区擦除指令 
	spi2_Transmit_one_byte((uint8_t)((Address)>>16));  //发送24bit地址    
	spi2_Transmit_one_byte((uint8_t)((Address)>>8));   
	spi2_Transmit_one_byte((uint8_t)Address);  
	W25Q128_Disable();                            //取消片选     	      
	W25Q128_Wait_Busy(); 				   //等待擦除完成
}
 
 
/* Nicky ******************************************************************* */
//擦除地址所在的扇区
void Erase_Write_data_Sector(uint32_t Address,uint32_t Write_data_NUM)   
{
	//总共4096个扇区
	//计算 写入数据开始的地址 + 要写入数据个数的最后地址 所处的扇区	
	uint16_t Star_Sector,End_Sector,Num_Sector;
	Star_Sector = Address / 4096;						//数据写入开始的扇区
	End_Sector = (Address + Write_data_NUM) / 4096;		//数据写入结束的扇区
	Num_Sector = End_Sector - Star_Sector;  			//数据写入跨几个扇区
 
	//开始擦除扇区
	for(uint16_t i=0;i <= Num_Sector;i++)
	{
		Erase_one_Sector(Address);
		Address += 4095;
	}
 
}
 
/* Nicky ******************************************************************* */
//擦除整个芯片 等待时间超长... 10-20S
void Erase_W25Q128_Chip(void)   
{                                   
    W25Q128_Write_Enable();                  //SET WEL 
    W25Q128_Wait_Busy();   
  	W25Q128_Enable();                            //使能器件   
    spi2_Transmit_one_byte(0x60);        //发送片擦除命令  
	W25Q128_Disable();                            //取消片选     	      
	W25Q128_Wait_Busy();   				   //等待芯片擦除结束
} 
 
/* Nicky ******************************************************************* */
//读取W25Q128数据
void Read_W25Q128_data(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
 	uint16_t i=0;   										    
	W25Q128_Enable();                     //使能器件   
    spi2_Transmit_one_byte(0x03);         //发送读取命令   
    spi2_Transmit_one_byte((uint8_t)((ReadAddr)>>16));  //发送24bit地址    
    spi2_Transmit_one_byte((uint8_t)((ReadAddr)>>8));   
    spi2_Transmit_one_byte((uint8_t)ReadAddr);   
    for(;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=spi2_Receive_one_byte();   //循环读数  
    }
	W25Q128_Disable(); 				    	      
}
 
/* Nicky ******************************************************************* */
//写字，一次最多一页
void Write_Word(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
 	uint16_t i; 
 
	W25Q128_Write_Enable();                  //SET WEL
	W25Q128_Enable();                            //使能器件
	spi2_Transmit_one_byte(0x02);
    spi2_Transmit_one_byte((uint8_t)((WriteAddr) >> 16)); //写入的目标地址   
    spi2_Transmit_one_byte((uint8_t)((WriteAddr) >> 8));   
    spi2_Transmit_one_byte((uint8_t)WriteAddr);   
    for (i = 0; i < NumByteToWrite; i++)
		spi2_Transmit_one_byte(pBuffer[i]);//循环写入字节数据  
	W25Q128_Disable();
	W25Q128_Wait_Busy();		//写完之后需要等待芯片操作完。
}
 
/* Nicky ******************************************************************* */
//定位到页
void Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{
	uint16_t Word_remain;
	Word_remain=256-WriteAddr%256; 	//定位页剩余的字数	
	
	if(NumByteToWrite <= Word_remain)
		Word_remain=NumByteToWrite;		//定位页能一次写完
	while(1)
	{
		Write_Word(pBuffer,WriteAddr,Word_remain);	
		if(NumByteToWrite==Word_remain)
		{
			break;	//判断写完就 break
		}	
	 	else //没写完，翻页了
		{
			pBuffer += Word_remain;		//直针后移当页已写字数
			WriteAddr += Word_remain;	
			NumByteToWrite -= Word_remain;	//减去已经写入了的字数
			if(NumByteToWrite>256)
				Word_remain=256; 		//一次可以写入256个字
			else 
				Word_remain=NumByteToWrite; 	//不够256个字了
		}
	}	    
} 
 
/* Nicky ******************************************************************* */
// 读取 ID 测试 OK 0xEF 0X17
void read_W25Q128_ID()
{
	uint8_t _RxData[2]={0x00};
	W25Q128_Enable();
	
	//发送指令
    spi2_Transmit_one_byte(0x90);
	spi2_Transmit_one_byte(0x00);
	spi2_Transmit_one_byte(0x00);
	spi2_Transmit_one_byte(0x00);
	
	//接收数据
	_RxData[0] = spi2_Receive_one_byte();
	_RxData[1] = spi2_Receive_one_byte();
	
    W25Q128_Disable();
    
	usb_printf("%d\n",_RxData);	//串口打印 ID
}
 
/* Nicky ******************************************************************* */
//测试程序
void W25Q128_test()
{
	//读数据，看原始存在的数据
	Read_W25Q128_data(Read_data,Data_Address,Read_data_SIZE);	
	for(uint8_t i=0;i<Write_data_SIZE;i++){
			usb_printf("%d\n",Read_data[i]);
		delay_us(50);
	}
		
	
	//擦除需要写数据所在的扇区
 	Erase_Write_data_Sector(Data_Address,Write_data_SIZE);
	Read_W25Q128_data(Read_data,Data_Address,Read_data_SIZE);
	for(uint8_t i=0;i<Write_data_SIZE;i++){
			usb_printf("%d\n",Read_data[i]);
		delay_us(50);
	}

			
	//写数据
	Write_Page(Write_data,Data_Address,Write_data_SIZE);
	Read_W25Q128_data(Read_data,Data_Address,Read_data_SIZE); 
		
	//串口打印数据
	for(uint8_t i=0;i<Write_data_SIZE;i++){
		usb_printf("%d\n",Read_data[i]);
		delay_us(50);
	}
}



