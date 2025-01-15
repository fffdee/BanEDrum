#ifndef _BG_READ_H__
#define _BG_READ_H__


#include "stdint.h"
#define FILECOUNT 4
#define SAMPLERATECOUNT 4
#define CH_COUNT 2
#define WIDITH_COUNT 4
#define MAX_FILE_COUNT 100
#define WAV_START      0x19000


uint8_t bg_read_callback(short *data, uint32_t index, uint32_t count,uint8_t program);
void get_bg_headInfo(void);

#endif 




