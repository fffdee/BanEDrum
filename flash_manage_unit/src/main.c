#include<stdio.h>
#include"bg_download.h"

BG_Download bg_dl;
uint8_t ack_data = 0;
void ack(uint8_t ack);
void read(uint8_t *data,uint32_t count,uint32_t timeout);
void flash(uint32_t address,uint8_t* data, uint16_t count,uint8_t type);

void main()
{
    uint8_t data[32] = {0};
    bg_dl = BG_Download_Init(flash,read,ack);
    printf("Init start\n");
    bg_dl.SerialAck(ack_data);
    bg_dl.SerialRead(data,32,100);
    bg_dl.Write(0,data,32,0);
    while(1){
        ;
    }

}


void ack(uint8_t ack)
{
    printf("load ack\n");
}

void read(uint8_t *data,uint32_t count,uint32_t timeout)
{
    printf("load read\n");
}

void flash(uint32_t address,uint8_t* data, uint16_t count,uint8_t type)
{
    printf("load flash\n");
}