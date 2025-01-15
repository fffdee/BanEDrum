#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "bg_read.h"
#include "hardware_interfance.h"
#include "BanGTsynth.h"
#include "bg_flash_manager.h"

FILE *file;

// 调整后的读取函数，跳过每页的头两个字节
void seek_and_read(uint32_t seek, uint8_t count, uint8_t *info) {
    // 计算当前页的起始地址
    uint32_t page_start = seek & ~(256 - 1);
    // 计算跳过页首两个字节后的起始地址
    uint32_t adjusted_seek = page_start + 2;
    
    // 计算需要跳过的字节数
    uint8_t skip_bytes = (seek - page_start) + 2;
    
    // 如果调整后的起始地址超过了原始的起始地址加上要读取的字节数，则不需要读取
    if (adjusted_seek >= seek + count) {
        return;
    }
    
    // 计算实际需要读取的字节数
    uint8_t actual_read_count = count - skip_bytes;
    
    // 读取数据，跳过每页的头两个字节
    BG_flash_manager.ReadData(adjusted_seek, info, actual_read_count, DEV_NOR);
    
    // 如果需要跳过的字节数超过2，则说明读取的数据需要从中间开始
    if (skip_bytes > 2) {
        // 将读取的数据向后移动，以跳过页首的两个字节
        for (int i = actual_read_count - 1; i >= 0; i--) {
            info[i + skip_bytes - 2] = info[i];
        }
    }
}

void get_bg_headInfo()
{
    uint32_t BRH_address;

    uint8_t info[4];
    
    uint8_t file_haeder = 0;

    seek_and_read(file_haeder, FILE_HEADER_BYTE, info);
    usb_printf("header count is %d\n",info[0]);
    BRH_address = info[0];
    file_haeder+=FILE_HEADER_BYTE;
    // program data set data
    usb_printf("read count is %d\n",file_haeder);
    seek_and_read(file_haeder, PROGRAM_COUNT_BYTE, info);
    BG_reader.Data.program_count = (uint16_t)info[0] | (uint16_t)info[1] << 8;
    file_haeder+=PROGRAM_COUNT_BYTE;
    file_haeder+=FILE_ENCODER_BYTE;

    seek_and_read(file_haeder, FILE_VERSION_BYTE, info);
    memcpy(BG_reader.Data.version,info,3);
    file_haeder+=FILE_VERSION_BYTE;

    seek_and_read(file_haeder,FILE_AUTHOR_BYTE , info);
    BG_reader.Data.name_len = info[0];
    file_haeder+=FILE_AUTHOR_BYTE;

    uint8_t *name_info  =(uint8_t *)malloc(BG_reader.Data.name_len * sizeof(uint8_t)) ;
    seek_and_read(file_haeder,BG_reader.Data.name_len , name_info);
    BG_reader.Data.author_name = name_info;
    file_haeder+=BG_reader.Data.name_len;
   

    seek_and_read(file_haeder,FILE_EMAIL_BYTE , info);
    BG_reader.Data.email_len = info[0];
    file_haeder+=FILE_EMAIL_BYTE;

    uint8_t *email_info  =(uint8_t *)malloc(BG_reader.Data.email_len * sizeof(uint8_t)) ;
    seek_and_read(file_haeder,BG_reader.Data.email_len , email_info);
    BG_reader.Data.author_name = email_info;
    file_haeder+=BG_reader.Data.email_len;
    

    BG_reader.Data.ProgramData = (BG_ProgramData *)malloc(BG_reader.Data.program_count * sizeof(BG_ProgramData));
    //BG_reader.Data.ProgramData = (BG_ProgramData *)malloc(BG_reader.Data.program_count * sizeof(BG_ProgramData));;
    uint32_t base_address[BG_reader.Data.program_count];
   usb_printf("program count is %d\n",BG_reader.Data.program_count);
    
    for (uint8_t count = 0; count < BG_reader.Data.program_count; count++)
    {
        uint32_t temp_address = BRH_address;
         usb_printf("BRH_address %X\n",BRH_address);
        seek_and_read(BRH_address, PROGRAM_HEADER_BYTE, info);
        BRH_address+=(uint32_t)info[0] | (uint32_t)info[1] << 8;
        usb_printf("PROGRAM_HEADER_BYTE; %X\n",(uint32_t)info[0] | (uint32_t)info[1]);
        temp_address+=PROGRAM_HEADER_BYTE;

        usb_printf("temp_address %X\n",temp_address);
        
        seek_and_read(temp_address, PROGRAM_BANK_BYTE, info);
        BG_reader.Data.ProgramData[count].bank_index = info[0];
        temp_address += PROGRAM_BANK_BYTE;

        seek_and_read(temp_address, PROGRAM_INDEX_BYTE, info);
        BG_reader.Data.ProgramData[count].program_index = info[0];
        temp_address += PROGRAM_INDEX_BYTE;

        seek_and_read(temp_address, PROGRAM_NAME_BYTE, info);
        BG_reader.Data.ProgramData[count].name_len = info[0];
        temp_address += PROGRAM_NAME_BYTE;

        uint8_t *pgm_name  =(uint8_t *)malloc(BG_reader.Data.ProgramData[count].name_len  * sizeof(uint8_t)) ;
        seek_and_read(temp_address, BG_reader.Data.ProgramData[count].name_len, pgm_name);
        BG_reader.Data.ProgramData[count].name = pgm_name;
        temp_address += BG_reader.Data.ProgramData[count].name_len;
        usb_printf("program_anme :%d %s\n",BG_reader.Data.ProgramData[count].name_len, BG_reader.Data.ProgramData[count].name);

        seek_and_read(temp_address, PROGRAM_DESCRIPT_BYTE, info);
        BG_reader.Data.ProgramData[count].descript_len = info[0];
        temp_address += PROGRAM_DESCRIPT_BYTE;
        

        uint8_t *decript  =(uint8_t *)malloc(BG_reader.Data.ProgramData[count].descript_len * sizeof(uint8_t)) ;
        seek_and_read(temp_address, BG_reader.Data.ProgramData[count].descript_len, decript );
        BG_reader.Data.ProgramData[count].descript = decript;
        temp_address += BG_reader.Data.ProgramData[count].descript_len;
        usb_printf("program_descript :%d %s\n", BG_reader.Data.ProgramData[count].descript_len, BG_reader.Data.ProgramData[count].descript);

       
        seek_and_read(temp_address, PROGRAM_TOTAL_BYTE, info);
        temp_address += PROGRAM_TOTAL_BYTE;
        if (count < 1)
        {
            base_address[count] = 0;
            base_address[count+1] = (uint32_t)info[0] | (uint32_t)info[1] << 8 | (uint32_t)info[2] << 16 | (uint32_t)info[3] << 24;
            
        }
        else
        {
            if(count<BG_reader.Data.program_count-1&&BG_reader.Data.program_count>1)
            base_address[count+1] = (uint32_t)info[0] | (uint32_t)info[1] << 8 | (uint32_t)info[2] << 16 | (uint32_t)info[3] << 24;
            base_address[count+1] += base_address[count];
        }
        

    }
   
    BG_reader.Data.base_address = base_address;
    
    
   
    for (uint8_t count = 0; count < BG_reader.Data.program_count; count++)
    {

      //  memset(info,sizeof(info),0x00);   
        uint32_t program_bia_address = BRH_address + BG_reader.Data.base_address[count];
        usb_printf("program_bia_address %X\n",program_bia_address);
        
        seek_and_read(program_bia_address, WAV_HEADER_BYTE, info);
        BG_reader.Data.ProgramData[count].wav_header_count = info[0];
        program_bia_address +=WAV_HEADER_BYTE;

        seek_and_read(program_bia_address, WAV_FILE_COUNT_BYTE, info);
        BG_reader.Data.ProgramData[count].file_count = (uint16_t)info[0] | (uint16_t)info[1] << 8 ;
        program_bia_address +=WAV_FILE_COUNT_BYTE;

        usb_printf("file_count is %X\n",BG_reader.Data.ProgramData[count].file_count);
        usb_printf("file_size :%X\n", BG_reader.Data.base_address[count]);     
       // memset(info,sizeof(info),0x00);      
        seek_and_read(program_bia_address, WAV_SAMPLERATE_BYTE, info);
        BG_reader.Data.ProgramData[count].samplerate = (uint32_t)info[0] | (uint32_t)info[1] << 8 | (uint32_t)info[2] << 16 | (uint32_t)info[3] << 24;
        program_bia_address +=WAV_SAMPLERATE_BYTE;    
        
        seek_and_read(program_bia_address, WAV_DEPTH_BYTE, info);
        BG_reader.Data.ProgramData[count].audio_width = info[0];
        program_bia_address +=WAV_DEPTH_BYTE;
        
        seek_and_read(program_bia_address, WAV_CHANNEL_BYTE, info);
        BG_reader.Data.ProgramData[count].Ch = info[0];
        program_bia_address +=WAV_CHANNEL_BYTE;

        seek_and_read(program_bia_address, PROGRAM_TYPE_BYTE, info);
        BG_reader.Data.ProgramData[count].type = info[0];
        program_bia_address +=PROGRAM_TYPE_BYTE;

        seek_and_read(program_bia_address, VEL_COUNT_BYTE, info);
        BG_reader.Data.ProgramData[count].vel_count = info[0];
        program_bia_address +=VEL_COUNT_BYTE;
        
        seek_and_read(program_bia_address, NOTE_HEADER_BYTE, info);
        BG_reader.Data.ProgramData[count].note_info_count = info[0];
        program_bia_address +=NOTE_HEADER_BYTE;
    
        BG_reader.Data.ProgramData[count].Note_Info = (Read_Note_Info *)malloc(BG_reader.Data.ProgramData[count].file_count * sizeof(Read_Note_Info));

        BG_reader.Data.ProgramData[count].bytecount = (uint32_t *)malloc(BG_reader.Data.ProgramData[count].file_count * sizeof(uint32_t));


        BG_reader.Data.ProgramData[count].address_index = (uint32_t *)malloc(BG_reader.Data.ProgramData[count].file_count * sizeof(uint32_t));;

        usb_printf("size of address_index %ld\n",sizeof(BG_reader.Data.ProgramData[count].address_index));
#ifdef READ_LINUX_DEBUG
        usb_printf(" file_count :%d ", BG_reader.Data.file_count);
        usb_printf(" samplerate :%d ", BG_readDBG_reader.Data.address_index[] ata.samplerate);
        usb_printf(" audio_width :%d ", BG_reader.Data.audio_width);
        usb_printf(" ch :%d \n", BG_reader.Data.Ch);
#endif

        uint8_t size_info[BG_reader.Data.ProgramData[count].file_count][BG_reader.Data.ProgramData[count].note_info_count];

   
        for (int i = 0; i < BG_reader.Data.ProgramData[count].file_count; i++)
        {
            
            BG_reader.Data.ProgramData[count].address_index[i] = 0;

            seek_and_read(i * BG_reader.Data.ProgramData[count].note_info_count + program_bia_address, BG_reader.Data.ProgramData[count].note_info_count, size_info[i]);
            BG_reader.Data.ProgramData[count].bytecount[i] = (uint32_t)size_info[i][0] | (uint32_t)size_info[i][1] << 8 | (uint32_t)size_info[i][2] << 16 | (uint32_t)size_info[i][3] << 24;
            BG_reader.Data.ProgramData[count].Note_Info[i].note = size_info[i][4];
            BG_reader.Data.ProgramData[count].Note_Info[i].min_note = size_info[i][5]; 
            BG_reader.Data.ProgramData[count].Note_Info[i].max_note = size_info[i][6];
            BG_reader.Data.ProgramData[count].Note_Info[i].vel_id = size_info[i][7];  
            BG_reader.Data.ProgramData[count].Note_Info[i].min_vel = size_info[i][8];
            BG_reader.Data.ProgramData[count].Note_Info[i].max_vel = size_info[i][9];
            if (i > 0)
                BG_reader.Data.ProgramData[count].Note_Info[i].address = BG_reader.Data.ProgramData[count].Note_Info[i - 1].address + BG_reader.Data.ProgramData[count].bytecount[i] * 2;
            else
                BG_reader.Data.ProgramData[count].Note_Info[0].address = 0;
#ifdef READ_LINUX_DEBUG
            usb_printf("data %d is : %d \n", i, BG_reader.Data.ProgramData[count].bytecount[i]);
#endif
            usb_printf("program %d data %d is : %d  count is %d\n",count, BG_reader.Data.
            ProgramData[count].Note_Info[i].note, BG_reader.Data.ProgramData[count].Note_Info[i].address, i);
            usb_printf("note range:%d-%d vel is:%d vel range:%d-%d\n",size_info[i][5],size_info[i][6],size_info[i][7],
            size_info[i][8],size_info[i][9]);
        }
        usb_printf("count is %d\n",program_bia_address);
        BG_reader.Data.ProgramData[count].biaadress = BG_reader.Data.ProgramData[count].file_count * BG_reader.Data.ProgramData[count].note_info_count + program_bia_address;
        usb_printf(" BG_reader.Data.biaadress is :%d\n", BG_reader.Data.ProgramData[count].biaadress);
    }
    

}


uint8_t bg_read_callback(short *data, uint32_t index, uint32_t count, uint8_t program)
{
    uint8_t BytePerData = (BG_reader.Data.ProgramData[program].audio_width / 8 * BG_reader.Data.ProgramData[program].Ch);
    uint8_t bytedata[2];
    // BG_reader.Data.address_index[0]= BG_reader.Data.Note_Info[0].address;
#ifdef READ_LINUX_DEBUG
    usb_printf("data count is %d\n", BG_reader.Data.ProgramData[program].bytecount[0] * 2);
#endif
    for (uint8_t i = 0; i < BG_reader.Data.ProgramData[program].file_count; i++)
    {
        if (index == BG_reader.Data.ProgramData[program].Note_Info[i].note)
        {

            usb_printf("data %d is : %d  count is %d \n ", BG_reader.Data.ProgramData[program].Note_Info[i].note, BG_reader.Data.ProgramData[program].Note_Info[i].address + BG_reader.Data.ProgramData[program].bytecount[i] * 2, i);
            if (BG_reader.Data.ProgramData[program].address_index[i] < BG_reader.Data.ProgramData[program].Note_Info[i].address + BG_reader.Data.ProgramData[program].bytecount[i] * 2)
            {

                for (uint16_t index = 0; index < count; index++)
                {

                    seek_and_read(BG_reader.Data.ProgramData[program].address_index[i] + BG_reader.Data.ProgramData[program].biaadress + index * BytePerData, BytePerData, bytedata);
                    data[index] = ((short)bytedata[0] | (short)(bytedata[1] << 8));
#ifdef READ_LINUX_DEBUG
                    usb_printf("|data %d is %d  ", index, data[index]);
#endif
                }
                BG_reader.Data.ProgramData[program].address_index[i] += count * BG_reader.Data.ProgramData[program].Ch * BytePerData;
#ifdef READ_LINUX_DEBUG
                usb_printf("index is %d\n", BG_reader.Data.address_index[i]);
#endif
                // usb_printf("index %d is %d\n",i, BG_reader.Data.address_index[i]);
                // exit(1);
                return 1;
            }
            else
            {
                usb_printf("OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOK!!!!!!!!!!!\n");
                BG_reader.Data.ProgramData[program].address_index[i] = BG_reader.Data.ProgramData[program].Note_Info[i].address;
                // exit(1);
                return 0;
            }
        }
    }
}







