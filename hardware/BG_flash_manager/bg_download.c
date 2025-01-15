#include "bg_download.h"



void BG_Callback(){

}



BG_Download BG_Download_Init(void(*Write)(uint32_t,uint8_t*,uint16_t,uint8_t),void(*SerialRead)(uint8_t*,uint32_t),void(*SerialAck)(uint8_t)){

    BG_Download BG_download = {
    
        .Callback = BG_Callback,
        .SerialAck = SerialAck,
        .Write = Write,
        .SerialRead = SerialRead,

    };

    return BG_download;
}



