#ifndef _HARDWARE_INTERFANCE_H__
#define _HARDWARE_INTERFANCE_H__

#include <stdint.h>
#define PCMDEVICE "default"
#define SAMPLERATE 48000
#define MAXCHANNELS 2
#define BYTESPERSAMPLE (sizeof(short) * MAXCHANNELS) // 2 bytes per sample * number of channels

#define BUFFER_SIZE ((SAMPLERATE / 1000))     // Number of samples per millisecond
#define MAXRING_BUFFER_SIZE (BUFFER_SIZE * 2) // Ring buffer size (double buffering)

#define FILECOUNT 4
#define SAMPLERATECOUNT 4
#define CH_COUNT 2
#define WIDITH_COUNT 4
#define MAX_FILE_COUNT 100
#define WAV_START 0x19000

// #define READ_LINUX_DEBUG
/*************************************************BGS FILE SETTING*************************************************/
#define FILE_HEADER_BYTE 1
#define PROGRAM_COUNT_BYTE 2
#define FILE_VERSION_BYTE 3
#define FILE_ENCODER_BYTE 1
#define FILE_AUTHOR_BYTE 1
#define FILE_EMAIL_BYTE 1

#define PROGRAM_HEADER_BYTE 2
#define PROGRAM_BANK_BYTE 1
#define PROGRAM_INDEX_BYTE 1
#define PROGRAM_NAME_BYTE 1
#define PROGRAM_DESCRIPT_BYTE 1
#define PROGRAM_TOTAL_BYTE 4
#define PROGRAM_TYPE_BYTE 1


#define WAV_HEADER_BYTE 1
#define WAV_FILE_COUNT_BYTE 2
#define WAV_SAMPLERATE_BYTE 4
#define WAV_DEPTH_BYTE 1
#define WAV_CHANNEL_BYTE 1
#define WAV_FILESIZE_BYTE 4
#define NOTE_HEADER_BYTE 1
#define NOTE_BYTE 1
#define NOTE_MIN_BYTE 1
#define NOTE_MAX_BYTE 1
#define VEL_COUNT_BYTE 1
#define VEL_ID_BYTE 1
#define VEL_MIN_BYTE 1
#define VEL_MAX_BYTE 1


/*************************************************BGS FILE SETTING*************************************************/


typedef struct
{
    
    uint8_t vel_id;
    uint8_t note;
    uint8_t min_note;
    uint8_t max_note;
    uint8_t min_vel;
    uint8_t max_vel;
    uint32_t address;

} Read_Note_Info;

typedef struct
{
    uint8_t bank_index;
    uint8_t program_index;
    uint8_t name_len;
    uint8_t descript_len;
    uint8_t wav_header_count;
    uint8_t note_info_count;
    uint8_t audio_width;
    uint8_t type;
    uint8_t Ch;
    uint8_t vel_count;
    uint8_t *name;
    uint8_t *descript;
    uint16_t frame;
    uint16_t file_count;
    uint32_t samplerate;
    uint32_t biaadress;
    uint32_t *bytecount;
    uint32_t *address_index;
    Read_Note_Info *Note_Info;

} BG_ProgramData;

typedef struct
{
    
    uint8_t name_len;
    uint8_t email_len;
    uint8_t *author_name;
    uint8_t *author_email;
    uint8_t version[3];
    uint16_t program_count;
    uint32_t biaheader;
    uint32_t *base_address;
    BG_ProgramData *ProgramData;

} BG_ReadData;

typedef struct
{

    uint8_t (*Callback)(short *, uint32_t, uint32_t, uint8_t);
    BG_ReadData Data;

} BG_Reader;

typedef struct
{
    void (*Enable)(uint8_t);
    void (*Init)(uint8_t, uint8_t, uint16_t);
    void (*DeInit)(void);
    void (*PlayLoop)(void);
    void (*Callbaclk)(uint16_t *);
    uint8_t (*GetState)(void);

} AudioPlay;

extern BG_Reader BG_reader;

extern AudioPlay audioPlay;

#endif










