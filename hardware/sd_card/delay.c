#include "delay.h"
#include "main.h"


void delay_us(uint32_t i)
{
    uint32_t temp;
    SysTick->LOAD=9*i;         //设置重装数值, 72MHZ时
    SysTick->CTRL=0X01;         //使能，减到零是无动作，采用外部时钟源
    SysTick->VAL=0;                //清零计数器
    do
    {
        temp=SysTick->CTRL;           //读取当前倒计数值
    }
    while((temp&0x01)&&(!(temp&(1<<16))));     //等待时间到达
    SysTick->CTRL=0;    //关闭计数器
    SysTick->VAL=0;        //清空计数器
}

void delay_ms(uint32_t i)
{
    uint32_t temp;
    SysTick->LOAD=9000*i;      //设置重装数值, 72MHZ时
    SysTick->CTRL=0X01;        //使能，减到零是无动作，采用外部时钟源
    SysTick->VAL=0;            //清零计数器
    do
    {
        temp=SysTick->CTRL;       //读取当前倒计数值
    }
    while((temp&0x01)&&(!(temp&(1<<16))));    //等待时间到达
    SysTick->CTRL=0;    //关闭计数器
    SysTick->VAL=0;        //清空计数器
}

