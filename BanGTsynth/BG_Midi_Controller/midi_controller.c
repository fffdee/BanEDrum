#include "midi_controller.h"
#include "hardware_interfance.h"
#include "standard_request _processing.h"
#include "BanGTsynth.h"
#include "midi_info.h"
#include "stdio.h"
#include "string.h"
#include <math.h>


BG_MIDI_Data BG_MIDI_data; 

void MIDI_Callback(void);
void MIDI_Message_Handle(uint8_t *data, uint8_t len);
void keyborad_mesg_handle(char value);
void MIDI_Init(void);
void apply_vel(short *buffer, int numsamples, uint8_t volume);

BG_MIDI_Controller BG_MIDI_controller = {

    .MIDI_Handle = MIDI_Message_Handle,
    .Callback = MIDI_Callback, // usb_printf(" ok\n");
    .Init = MIDI_Init,
    .ApplyVel = apply_vel,
    
};

void apply_vel(short *buffer, int numsamples, uint8_t volume) {
    float volume_gain = (float)volume / 127.0f;
    for (int i = 0; i < numsamples; ++i) {
        // 调整样本值
        buffer[i] = (short)(buffer[i] * volume_gain);
    }
}

int Re_Sample(int tone){

    float pitchShiftFactor = pow(2.0, (float)tone / 12.0); // 升一个全音的因子  
    int newSampleRate = (int)(SAMPLERATE * pitchShiftFactor);  
    return newSampleRate;
}


void MIDI_Init(){

    // for(uint8_t ch = 0; ch<16;ch++)
    //  for(uint8_t note=0; note<128; note++)
    //     BG_MIDI_data.BG_channel_info[ch].Note_Map[note]=0;

    memset(BG_MIDI_data.BG_channel_info,0,sizeof(BG_MIDI_data.BG_channel_info));
}


void MIDI_Callback(void)
{
    short data[MAX_POLYPHONY][49]={0};
    short temp_data[48];
    short play[48]={0}; 
    uint8_t polyphony_count = 0;
    uint8_t play_flag = 0;
    for(uint8_t ch=0; ch<16; ch++)
    {    // usb_printf(" ok\n");
        
        if(BG_MIDI_data.BG_channel_info[ch].NoteOn_count>0){
         usb_printf("channel %d play count is %d\n",ch, BG_MIDI_data.BG_channel_info[ch].NoteOn_count);
            for(uint8_t note=0; note<128; note++){

          
                if(BG_MIDI_data.BG_channel_info[ch].Note_Map[note]>0){
                     // memcpy(data[polyphony_count],temp_data,48);
                    if (!BG_reader.Callback(temp_data, note, 48,BG_MIDI_data.BG_channel_info[ch].program_index))
                    {
                        usb_printf(" stop %d %d vel is %d\n",ch,note,BG_MIDI_data.BG_channel_info[ch].Note_Map[note]);
                        BG_MIDI_data.BG_channel_info[ch].Note_Map[note]=0;
                        
                        if(BG_MIDI_data.BG_channel_info[ch].NoteOn_count>0)
                        BG_MIDI_data.BG_channel_info[ch].NoteOn_count--;
                    }

                    //BG_MIDI_controller.ApplyVel(temp_data,48,BG_MIDI_data.BG_channel_info[ch].Note_Map[note]);
                    // memcpy(data[polyphony_count],temp_data,48);
                    
                    for(uint8_t i=0;i<48;i++){
                        data[polyphony_count][i] = (short)(temp_data[i]*((float)BG_MIDI_data.BG_channel_info[ch].Note_Map[note]/127.0f));
                    }

                    data[polyphony_count][48] = 1;
                    polyphony_count++;
                    
                    if(BG_MIDI_data.BG_channel_info[ch].NoteOn_count>0)play_flag = 1;
  
                }

            }
        }
   

    }
   
    
    for(uint8_t channel = 0; channel<polyphony_count;channel++){
       
         if(data[channel][48]>0){
             for(uint8_t i=0; i<48;i++)
                play[i]+=data[channel][i];
         }
        
    }
    if(play_flag){
			
				audioPlay.Callbaclk((uint16_t*)play);
			
		}

    polyphony_count=0;
}

void MIDI_Message_Handle(uint8_t *data, uint8_t len)
{
    uint8_t state;
    state = (data[0]>>4)&0x0F; 
    if(len<4)
    {
        for(uint8_t i=0; i<FUNC_COUNT;i++){

            if(state==MIDI_funcstion[i].Funcstion_ID)
                MIDI_funcstion[i].MIDI_FUNC(data,sizeof(data));               
                
        } 
        
    }


}
