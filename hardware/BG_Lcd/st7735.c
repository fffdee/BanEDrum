#include "st7735.h"
#include "bg_lcd.h"

extern void Lcd_WriteData(uint8_t data);
extern void Lcd_WriteIndex(uint8_t index);

void st7735_init(void)
{
	//LCD Init For 1.44Inch LCD Panel with ST7735R.

		LCD_RST_ENABLE();
		
//		Lcd_WriteIndex(0x11);//Sleep exit


//	    //ST7735R Frame Rate
//	    Lcd_WriteIndex(0xB1);
//	    Lcd_WriteData(0x01);
//	    Lcd_WriteData(0x2C);
//	    Lcd_WriteData(0x2D);

//	    Lcd_WriteIndex(0xB2);
//	    Lcd_WriteData(0x01);
//	    Lcd_WriteData(0x2C);
//	    Lcd_WriteData(0x2D);

//	    Lcd_WriteIndex(0xB3);
//	    Lcd_WriteData(0x01);
//	    Lcd_WriteData(0x2C);
//	    Lcd_WriteData(0x2D);
//	    Lcd_WriteData(0x01);
//	    Lcd_WriteData(0x2C);
//	    Lcd_WriteData(0x2D);

//	    Lcd_WriteIndex(0xB4); //Column inversion
//	    Lcd_WriteData(0x07);

//	    //ST7735R Power Sequence
//	    Lcd_WriteIndex(0xC0);
//	    Lcd_WriteData(0xA2);
//	    Lcd_WriteData(0x02);
//	    Lcd_WriteData(0x84);
//	    Lcd_WriteIndex(0xC1);
//	    Lcd_WriteData(0xC5);

//	    Lcd_WriteIndex(0xC2);
//	    Lcd_WriteData(0x0A);
//	    Lcd_WriteData(0x00);

//	    Lcd_WriteIndex(0xC3);
//	    Lcd_WriteData(0x8A);
//	    Lcd_WriteData(0x2A);
//	    Lcd_WriteIndex(0xC4);
//	    Lcd_WriteData(0x8A);
//	    Lcd_WriteData(0xEE);

//	    Lcd_WriteIndex(0xC5); //VCOM
//	    Lcd_WriteData(0x0E);

//	    Lcd_WriteIndex(0x36); //MX, MY, RGB mode
//	   // Lcd_WriteData(0x20);
//	    Lcd_WriteData(0x60);
//	   // Lcd_WriteData(0x28);
//	    //ST7735R Gamma Sequence
//	    Lcd_WriteIndex(0xe0);
//	    Lcd_WriteData(0x0f);
//	    Lcd_WriteData(0x1a);
//	    Lcd_WriteData(0x0f);
//	    Lcd_WriteData(0x18);
//	    Lcd_WriteData(0x2f);
//	    Lcd_WriteData(0x28);
//	    Lcd_WriteData(0x20);
//	    Lcd_WriteData(0x22);
//	    Lcd_WriteData(0x1f);
//	    Lcd_WriteData(0x1b);
//	    Lcd_WriteData(0x23);
//	    Lcd_WriteData(0x37);
//	    Lcd_WriteData(0x00);
//	    Lcd_WriteData(0x07);
//	    Lcd_WriteData(0x02);
//	    Lcd_WriteData(0x10);

//	    Lcd_WriteIndex(0xe1);
//	    Lcd_WriteData(0x0f);
//	    Lcd_WriteData(0x1b);
//	    Lcd_WriteData(0x0f);
//	    Lcd_WriteData(0x17);
//	    Lcd_WriteData(0x33);
//	    Lcd_WriteData(0x2c);
//	    Lcd_WriteData(0x29);
//	    Lcd_WriteData(0x2e);
//	    Lcd_WriteData(0x30);
//	    Lcd_WriteData(0x30);
//	    Lcd_WriteData(0x39);
//	    Lcd_WriteData(0x3f);
//	    Lcd_WriteData(0x00);
//	    Lcd_WriteData(0x07);
//	    Lcd_WriteData(0x03);
//	    Lcd_WriteData(0x10);

//	    Lcd_WriteIndex(0x2a);
//	    Lcd_WriteData(0x00);
//	    Lcd_WriteData(0x00);
//	    Lcd_WriteData(0x00);
//	    Lcd_WriteData(0x7f);

//	    Lcd_WriteIndex(0x2b);
//	    Lcd_WriteData(0x00);
//	    Lcd_WriteData(0x00);
//	    Lcd_WriteData(0x00);
//	    Lcd_WriteData(0x9f);

//	    Lcd_WriteIndex(0xF0); //Enable test command
//	    Lcd_WriteData(0x01);
//	    Lcd_WriteIndex(0xF6); //Disable ram power save mode
//	    Lcd_WriteData(0x00);

//	    Lcd_WriteIndex(0x3A); //65k mode
//	    Lcd_WriteData(0x05);

//	    Lcd_WriteIndex(0x29);//Display on
//		HAL_Delay(120); 
		Lcd_WriteIndex(0x11);   
		Lcd_WriteIndex(0x36);                                             //�����ڴ�ɨ�跽��0X00����ɨ�裬�������£��������ң�RGB��ʽ
//    if(USE_HORIZONTAL==0)Lcd_WriteData(0x00);
//    else if(USE_HORIZONTAL==1)Lcd_WriteData(0xC0);
//    else if(USE_HORIZONTAL==2)Lcd_WriteData(0x70);
//    else Lcd_WriteData(0xA0);
  
    Lcd_WriteIndex(0x3A);                                             //���ݸ�ʽ��65Kɫ,565
    Lcd_WriteData(0x05);
  
    Lcd_WriteIndex(0xB2);                                             //֡Ƶ����
    Lcd_WriteData(0x0C);
    Lcd_WriteData(0x0C);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x33);
    Lcd_WriteData(0x33); 
  
    Lcd_WriteIndex(0xC6);                                               //����ģʽ�µ�֡���ʿ���
    Lcd_WriteData(0x01);                                            //����ģʽ�£���Ļˢ���� 111Hz
  //  Lcd_WriteData(0x1F);                                            //����ģʽ�£���Ļˢ���� 39Hz
    
    Lcd_WriteIndex(0xB7);                                             //GATE ����
    Lcd_WriteData(0x35);  
  
    Lcd_WriteIndex(0xBB);                                             //VCOM���� 
    Lcd_WriteData(0x19);
  
    Lcd_WriteIndex(0xC0);                                             //LCM����,Ĭ��0x2c
    Lcd_WriteData(0x2C);
  
    Lcd_WriteIndex(0xC2);                                             //VDV&VRH SET ,Ĭ��0x01
    Lcd_WriteData(0x01);
  
    Lcd_WriteIndex(0xC3);                                             //VRHS SET��Ĭ��0x0b
    Lcd_WriteData(0x12);                                           //�˴�����ʵ���������
  
    Lcd_WriteIndex(0xC4);                                             //VDV SET��Ĭ��0x20
    Lcd_WriteData(0x20);  
  
    Lcd_WriteIndex(0xC6);                                             // FR SET, Ĭ��0x0F
    Lcd_WriteData(0x0F);    
  
    Lcd_WriteIndex(0xD0);                                             //��Դ����1
    Lcd_WriteData(0xA4);                                           //�ò�������  
    Lcd_WriteData(0xA1);                                           //�˴�����ʵ������޸�
  
    Lcd_WriteIndex(0xE0);                                             //������GAMMA���� 
    Lcd_WriteData(0xD0);
    Lcd_WriteData(0x04);
    Lcd_WriteData(0x0D);
    Lcd_WriteData(0x11);
    Lcd_WriteData(0x13);
    Lcd_WriteData(0x2B);
    Lcd_WriteData(0x3F);
    Lcd_WriteData(0x54);
    Lcd_WriteData(0x4C);
    Lcd_WriteData(0x18);
    Lcd_WriteData(0x0D);
    Lcd_WriteData(0x0B);
    Lcd_WriteData(0x1F);
    Lcd_WriteData(0x23);
  
    Lcd_WriteIndex(0xE1);                                              //������GAMMA����
    Lcd_WriteData(0xD0);
    Lcd_WriteData(0x04);
    Lcd_WriteData(0x0C);
    Lcd_WriteData(0x11);
    Lcd_WriteData(0x13);
    Lcd_WriteData(0x2C);
    Lcd_WriteData(0x3F);
    Lcd_WriteData(0x44);
    Lcd_WriteData(0x51);
    Lcd_WriteData(0x2F);
    Lcd_WriteData(0x1F);
    Lcd_WriteData(0x1F);
    Lcd_WriteData(0x20);
    Lcd_WriteData(0x23);
  
    Lcd_WriteIndex(0x21);                                             //���Կ���Ĭ����0X20������ģʽ
    
    Lcd_WriteIndex(0x29);   


}
