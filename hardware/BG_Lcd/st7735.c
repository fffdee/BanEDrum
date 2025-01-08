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
		Lcd_WriteIndex(0x36);                                             //设置内存扫描方向，0X00正常扫描，从上往下，从左往右，RGB方式
//    if(USE_HORIZONTAL==0)Lcd_WriteData(0x00);
//    else if(USE_HORIZONTAL==1)Lcd_WriteData(0xC0);
//    else if(USE_HORIZONTAL==2)Lcd_WriteData(0x70);
//    else Lcd_WriteData(0xA0);
  
    Lcd_WriteIndex(0x3A);                                             //数据格式，65K色,565
    Lcd_WriteData(0x05);
  
    Lcd_WriteIndex(0xB2);                                             //帧频设置
    Lcd_WriteData(0x0C);
    Lcd_WriteData(0x0C);
    Lcd_WriteData(0x00);
    Lcd_WriteData(0x33);
    Lcd_WriteData(0x33); 
  
    Lcd_WriteIndex(0xC6);                                               //正常模式下的帧速率控制
    Lcd_WriteData(0x01);                                            //正常模式下：屏幕刷新率 111Hz
  //  Lcd_WriteData(0x1F);                                            //正常模式下：屏幕刷新率 39Hz
    
    Lcd_WriteIndex(0xB7);                                             //GATE 设置
    Lcd_WriteData(0x35);  
  
    Lcd_WriteIndex(0xBB);                                             //VCOM设置 
    Lcd_WriteData(0x19);
  
    Lcd_WriteIndex(0xC0);                                             //LCM设置,默认0x2c
    Lcd_WriteData(0x2C);
  
    Lcd_WriteIndex(0xC2);                                             //VDV&VRH SET ,默认0x01
    Lcd_WriteData(0x01);
  
    Lcd_WriteIndex(0xC3);                                             //VRHS SET，默认0x0b
    Lcd_WriteData(0x12);                                           //此处根据实际情况修正
  
    Lcd_WriteIndex(0xC4);                                             //VDV SET，默认0x20
    Lcd_WriteData(0x20);  
  
    Lcd_WriteIndex(0xC6);                                             // FR SET, 默认0x0F
    Lcd_WriteData(0x0F);    
  
    Lcd_WriteIndex(0xD0);                                             //电源控制1
    Lcd_WriteData(0xA4);                                           //该参数不变  
    Lcd_WriteData(0xA1);                                           //此处根据实际情况修改
  
    Lcd_WriteIndex(0xE0);                                             //正极性GAMMA调整 
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
  
    Lcd_WriteIndex(0xE1);                                              //负极性GAMMA调整
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
  
    Lcd_WriteIndex(0x21);                                             //反显开，默认是0X20，正常模式
    
    Lcd_WriteIndex(0x29);   


}
