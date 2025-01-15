#ifndef _MIDI_CONTROLLER_H__
#define _MIDI_CONTROLLER_H__

#include <stdint.h>
#include "midi_info.h"



typedef struct
{
    void (*MIDI_Handle)(uint8_t*,uint8_t);
    void (*Callback)(void);
    void (*Init)(void);
    void (*ApplyVel)(short*, int, uint8_t);

}BG_MIDI_Controller;

extern BG_MIDI_Controller BG_MIDI_controller;

extern BG_MIDI_Data BG_MIDI_data;

#endif






