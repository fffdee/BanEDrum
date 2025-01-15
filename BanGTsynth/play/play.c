#include "play.h"
#include <math.h>
#include "BanGTsynth.h"
#include "math.h"
#include "i2s.h"

void PlayCallback(uint16_t* data){

	if (HAL_I2S_GetState(&hi2s2) == HAL_I2S_STATE_READY) {
            // 重新启动传输
            if (HAL_I2S_Transmit_DMA(&hi2s2, (uint16_t *)data, sizeof(data)) != HAL_OK) {
                // 传输错误处理
            }
        }
}




