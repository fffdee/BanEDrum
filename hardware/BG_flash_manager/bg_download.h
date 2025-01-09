#ifndef BG_DOWNLOAD_H__
#define BG_DOWNLOAD_H__

#include<stdio.h>
#include<stdint.h>



typedef struct BG_Download
{
    void (*SerialRead)(uint8_t*,uint32_t,uint32_t);
    void (*SerialAck)(uint8_t);
    void (*Write)(uint32_t,uint8_t*,uint16_t,uint8_t);
    void (*Callback)(void);

}BG_Download;

BG_Download BG_Download_Init(void(*Write)(uint32_t,uint8_t*,uint16_t,uint8_t)
         ,void(*SerialRead)(uint8_t*,uint32_t,uint32_t)
         ,void(*SerialAck)(uint8_t));

#endif 