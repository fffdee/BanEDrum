#include "standard_request _processing.h"
#include "midi_info.h"
#include "hardware_interfance.h"
#include "midi_controller.h"
#include <stdio.h>
#include <string.h>
#include "BanGTsynth.h"

void NoteOnHandle(uint8_t *data, uint8_t len);
void NoteOffHandle(uint8_t *data, uint8_t len);
void ProgramChange(uint8_t *data, uint8_t len);
void CCHandle(uint8_t *data, uint8_t len);



MIDI_Funcstion MIDI_funcstion[FUNC_COUNT] = {

    {NOTE_ON, NoteOnHandle},
    {NOTE_OFF, NoteOffHandle},
    {PROGRAM_CHANGE,ProgramChange},
    {CC,CCHandle},
    
};


void ChannelVolume(uint8_t *data, uint8_t len);
void AllNoteOff(uint8_t *data, uint8_t len);
void MonoOn(uint8_t *data, uint8_t len);
void PolyOn(uint8_t *data, uint8_t len);
MIDI_Funcstion CC_funcstion[CC_COUNT] = {


    {CTRL_07,ChannelVolume},
    {CTRL_123,AllNoteOff},
    {CTRL_126,MonoOn},
    {CTRL_127,PolyOn},

};


void NoteOnHandle(uint8_t *data, uint8_t len)
{

    uint8_t channel;
    channel = data[0] & 0x0F;
    uint8_t is_note = 0;
 
  
    for (uint8_t i = 0; i < BG_reader.Data.ProgramData[BG_MIDI_data.BG_channel_info[channel].program_index].file_count; i++)
    {
        if(BG_MIDI_data.BG_channel_info[channel].mono_poly_onoff == 1){
                BG_MIDI_data.BG_channel_info[channel].Note_Map[BG_MIDI_data.BG_channel_info[channel].last_note] = 0;
              
                BG_MIDI_data.BG_channel_info[channel].NoteOn_count = 0;    
            
        }  
        if (data[1] == BG_reader.Data.ProgramData[BG_MIDI_data.BG_channel_info[channel].program_index].Note_Info[i].note){
            uint8_t min_vel = BG_reader.Data.ProgramData[BG_MIDI_data.BG_channel_info[channel].program_index].Note_Info[i].min_vel;
            uint8_t max_vel = BG_reader.Data.ProgramData[BG_MIDI_data.BG_channel_info[channel].program_index].Note_Info[i].min_vel;
            if(data[2]>=min_vel&&data[2]>=max_vel){

            
                 BG_reader.Data.ProgramData[BG_MIDI_data.BG_channel_info[channel].program_index].address_index[i] = BG_reader.Data.ProgramData[BG_MIDI_data.BG_channel_info[channel].program_index].Note_Info[i].address;
                 is_note = 1;
            } 
            usb_printf("replay %d %d\n",BG_reader.Data.ProgramData[BG_MIDI_data.BG_channel_info[channel].program_index].address_index[i],BG_reader.Data.ProgramData[0].Note_Info[i].note); 
            
            
        }
       
            
           
    }

    if (BG_MIDI_data.BG_channel_info[channel].NoteOn_count < 255&&BG_MIDI_data.BG_channel_info[channel].Note_Map[data[1]]<1 &&is_note ==1 ){
         if(BG_MIDI_data.BG_channel_info[channel].mono_poly_onoff==1){
           
            BG_MIDI_data.BG_channel_info[channel].NoteOn_count = 1;
            
         }else{
            BG_MIDI_data.BG_channel_info[channel].NoteOn_count++;
         }
         

    }
    
    BG_MIDI_data.BG_channel_info[channel].Note_Map[data[1]] = data[2];
    BG_MIDI_data.BG_channel_info[channel].last_note = data[1];
   // usb_printf("data %d is : %d  count is %d\n", data[0], data[1], data[2]);
   
} 

void NoteOffHandle(uint8_t *data, uint8_t len)
{

    uint8_t channel,i;
    channel = data[0] & 0x0F;
    // for (i = 0; i < BG_reader.Data.ProgramDatant8_t last_note;[channel].file_count; i++)
    // {
    //     if (data[1] == BG_reader.Data.ProgramData[channel].Note_Info[i].note)
    //         BG_reader.Data.ProgramData[channel].address_index[i] = BG_reader.Data.ProgramData[channel].Note_Info[i].address;
    // }
    if (BG_MIDI_data.BG_channel_info[channel].NoteOn_count > 0&&BG_MIDI_data.BG_channel_info[channel].Note_Map[data[1]]>0)
        BG_MIDI_data.BG_channel_info[channel].NoteOn_count--;

    BG_MIDI_data.BG_channel_info[channel].Note_Map[data[1]] = 0;

  
}


void ProgramChange(uint8_t *data, uint8_t len)
{
    uint8_t channel;
    channel = data[0] & 0x0F;
    AllNoteOff(data,len);
    
    BG_MIDI_data.BG_channel_info[channel].program_index = data[1];
    
}



void CCHandle(uint8_t *data, uint8_t len)
{   
    uint8_t cmd = 0;
    while(!(data[1]==CC_funcstion[cmd].Funcstion_ID)){
        cmd+=1;
        if(cmd>=CC_COUNT){
            return;
        }
    }
    CC_funcstion[cmd].MIDI_FUNC(data,len);
}



void ChannelVolume(uint8_t *data, uint8_t len)
{
    uint8_t channel;
    channel = data[0] & 0x0F;
    BG_MIDI_data.Channel_volume[channel] = data[2];
}


void MonoOn(uint8_t *data, uint8_t len)
{   
    uint8_t channel;
    channel = data[0] & 0x0F;
    AllNoteOff(data,len);
    BG_MIDI_data.BG_channel_info[channel].mono_poly_onoff = 1;

}

void PolyOn(uint8_t *data, uint8_t len)
{
    uint8_t channel;
    channel = data[0] & 0x0F;
    BG_MIDI_data.BG_channel_info[channel].mono_poly_onoff = 0;
}

void AllNoteOff(uint8_t *data, uint8_t len){

     uint8_t channel;
     channel = data[0] & 0x0F;
     memset(BG_MIDI_data.BG_channel_info[channel].Note_Map,0x00,sizeof(BG_MIDI_data.BG_channel_info[channel].Note_Map));
     BG_MIDI_data.BG_channel_info[channel].NoteOn_count=0;
     usb_printf("note off all \n");
}



