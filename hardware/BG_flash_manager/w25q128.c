#include "W25Q128.h"
#include "spi.h"
#include "usart.h"
#include <stdio.h>
#include "delay.h"
#include "usbd_cdc_if.h"
uint32_t FLASH_SIZE=16*1024*1024;	//FLASH ��СΪ16M�ֽ�
uint32_t Data_Address = 4090; //���Ե�ַ 250����ַ����ҳ֮�䣩 �� 4090 ����ַ��������������ҳ֮�䣩
 
//Ҫд������
uint8_t Write_data[]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x41};
#define Write_data_SIZE sizeof(Write_data)
 
//Ҫ��������
uint8_t Read_data[100] = {0};
#define Read_data_SIZE sizeof(Read_data)
 
/* Nicky ******************************************************************* */
//����ʹ��
void W25Q128_Enable()
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET);
}
 
/* Nicky ******************************************************************* */
//����ʧ��
void W25Q128_Disable()
{
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);
}
 
/* Nicky ******************************************************************* */
//SPI2 ���� 1 ���ֽ�����
void spi2_Transmit_one_byte(uint8_t _dataTx)
{
	HAL_SPI_Transmit_DMA(&hspi1,(uint8_t*) &_dataTx,1);
	delay_us(50);
}
 
/* Nicky ******************************************************************* */
//SPI2 ���� 1 ���ֽ�����
uint8_t spi2_Receive_one_byte()
{
	uint16_t _dataRx;
	HAL_SPI_Receive_DMA(&hspi1,(uint8_t*) &_dataRx, 1);
	delay_us(50);
	return _dataRx;
}
 
/* Nicky ******************************************************************* */
//W25Q128дʹ��,��WEL��1 
void W25Q128_Write_Enable()   
{
	W25Q128_Enable();                            //ʹ������   
    spi2_Transmit_one_byte(0x06); 
	W25Q128_Disable();                            //ȡ��Ƭѡ     	      
}
 
/* Nicky ******************************************************************* */
//W25Q128дʧ��,��WEL��0 
void W25Q128_Write_Disable()   
{
	W25Q128_Enable();                            //ʹ������   
    spi2_Transmit_one_byte(0x04); 
	W25Q128_Disable();                            //ȡ��Ƭѡ     	      
}
 
/* Nicky ******************************************************************* */
//��ȡ�Ĵ���״̬
uint8_t W25Q128_ReadSR(void)   
{  
	uint8_t byte=0;   
	W25Q128_Enable();                            //ʹ������   
	spi2_Transmit_one_byte(0x05);    //���Ͷ�ȡ״̬�Ĵ�������
	byte=spi2_Receive_one_byte();             //��ȡһ���ֽ�
	W25Q128_Disable();                           //ȡ��Ƭѡ     
	return byte;   
} 
 
/* Nicky ******************************************************************* */
//�ȴ�����
void W25Q128_Wait_Busy()   
{   
	while((W25Q128_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}
 
/* Nicky ******************************************************************* */
//������ַ���ڵ�һ������
void Erase_one_Sector(uint32_t Address)
{
	W25Q128_Write_Enable();                  //SET WEL 	 
	W25Q128_Wait_Busy(); 		
	W25Q128_Enable();                            //ʹ������ 
	spi2_Transmit_one_byte(0x20);      //������������ָ�� 
	spi2_Transmit_one_byte((uint8_t)((Address)>>16));  //����24bit��ַ    
	spi2_Transmit_one_byte((uint8_t)((Address)>>8));   
	spi2_Transmit_one_byte((uint8_t)Address);  
	W25Q128_Disable();                            //ȡ��Ƭѡ     	      
	W25Q128_Wait_Busy(); 				   //�ȴ��������
}
 
 
/* Nicky ******************************************************************* */
//������ַ���ڵ�����
void Erase_Write_data_Sector(uint32_t Address,uint32_t Write_data_NUM)   
{
	//�ܹ�4096������
	//���� д�����ݿ�ʼ�ĵ�ַ + Ҫд�����ݸ���������ַ ����������	
	uint16_t Star_Sector,End_Sector,Num_Sector;
	Star_Sector = Address / 4096;						//����д�뿪ʼ������
	End_Sector = (Address + Write_data_NUM) / 4096;		//����д�����������
	Num_Sector = End_Sector - Star_Sector;  			//����д��缸������
 
	//��ʼ��������
	for(uint16_t i=0;i <= Num_Sector;i++)
	{
		Erase_one_Sector(Address);
		Address += 4095;
	}
 
}
 
/* Nicky ******************************************************************* */
//��������оƬ �ȴ�ʱ�䳬��... 10-20S
void Erase_W25Q128_Chip(void)   
{                                   
    W25Q128_Write_Enable();                  //SET WEL 
    W25Q128_Wait_Busy();   
  	W25Q128_Enable();                            //ʹ������   
    spi2_Transmit_one_byte(0x60);        //����Ƭ��������  
	W25Q128_Disable();                            //ȡ��Ƭѡ     	      
	W25Q128_Wait_Busy();   				   //�ȴ�оƬ��������
} 
 
/* Nicky ******************************************************************* */
//��ȡW25Q128����
void Read_W25Q128_data(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
 	uint16_t i=0;   										    
	W25Q128_Enable();                     //ʹ������   
    spi2_Transmit_one_byte(0x03);         //���Ͷ�ȡ����   
    spi2_Transmit_one_byte((uint8_t)((ReadAddr)>>16));  //����24bit��ַ    
    spi2_Transmit_one_byte((uint8_t)((ReadAddr)>>8));   
    spi2_Transmit_one_byte((uint8_t)ReadAddr);   
    for(;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=spi2_Receive_one_byte();   //ѭ������  
    }
	W25Q128_Disable(); 				    	      
}
 
/* Nicky ******************************************************************* */
//д�֣�һ�����һҳ
void Write_Word(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
 	uint16_t i; 
 
	W25Q128_Write_Enable();                  //SET WEL
	W25Q128_Enable();                            //ʹ������
	spi2_Transmit_one_byte(0x02);
    spi2_Transmit_one_byte((uint8_t)((WriteAddr) >> 16)); //д���Ŀ���ַ   
    spi2_Transmit_one_byte((uint8_t)((WriteAddr) >> 8));   
    spi2_Transmit_one_byte((uint8_t)WriteAddr);   
    for (i = 0; i < NumByteToWrite; i++)
		spi2_Transmit_one_byte(pBuffer[i]);//ѭ��д���ֽ�����  
	W25Q128_Disable();
	W25Q128_Wait_Busy();		//д��֮����Ҫ�ȴ�оƬ�����ꡣ
}
 
/* Nicky ******************************************************************* */
//��λ��ҳ
void Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{
	uint16_t Word_remain;
	Word_remain=256-WriteAddr%256; 	//��λҳʣ�������	
	
	if(NumByteToWrite <= Word_remain)
		Word_remain=NumByteToWrite;		//��λҳ��һ��д��
	while(1)
	{
		Write_Word(pBuffer,WriteAddr,Word_remain);	
		if(NumByteToWrite==Word_remain)
		{
			break;	//�ж�д��� break
		}	
	 	else //ûд�꣬��ҳ��
		{
			pBuffer += Word_remain;		//ֱ����Ƶ�ҳ��д����
			WriteAddr += Word_remain;	
			NumByteToWrite -= Word_remain;	//��ȥ�Ѿ�д���˵�����
			if(NumByteToWrite>256)
				Word_remain=256; 		//һ�ο���д��256����
			else 
				Word_remain=NumByteToWrite; 	//����256������
		}
	}	    
} 
 
/* Nicky ******************************************************************* */
// ��ȡ ID ���� OK 0xEF 0X17
void read_W25Q128_ID()
{
	uint8_t _RxData[2]={0x00};
	W25Q128_Enable();
	
	//����ָ��
    spi2_Transmit_one_byte(0x90);
	spi2_Transmit_one_byte(0x00);
	spi2_Transmit_one_byte(0x00);
	spi2_Transmit_one_byte(0x00);
	
	//��������
	_RxData[0] = spi2_Receive_one_byte();
	_RxData[1] = spi2_Receive_one_byte();
	
    W25Q128_Disable();
    
	usb_printf("%d\n",_RxData);	//���ڴ�ӡ ID
}
 
/* Nicky ******************************************************************* */
//���Գ���
void W25Q128_test()
{
	//�����ݣ���ԭʼ���ڵ�����
	Read_W25Q128_data(Read_data,Data_Address,Read_data_SIZE);	
	for(uint8_t i=0;i<Write_data_SIZE;i++){
			usb_printf("%d\n",Read_data[i]);
		delay_us(50);
	}
		
	
	//������Ҫд�������ڵ�����
 	Erase_Write_data_Sector(Data_Address,Write_data_SIZE);
	Read_W25Q128_data(Read_data,Data_Address,Read_data_SIZE);
	for(uint8_t i=0;i<Write_data_SIZE;i++){
			usb_printf("%d\n",Read_data[i]);
		delay_us(50);
	}

			
	//д����
	Write_Page(Write_data,Data_Address,Write_data_SIZE);
	Read_W25Q128_data(Read_data,Data_Address,Read_data_SIZE); 
		
	//���ڴ�ӡ����
	for(uint8_t i=0;i<Write_data_SIZE;i++){
		usb_printf("%d\n",Read_data[i]);
		delay_us(50);
	}
}



