#ifndef __BG_FLASH_MANAGER_H__
#define __BG_FLASH_MANAGER_H__

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

#define DEV_NOR 	0
#define DEV_NAND 	1



#define FLASH_CS_ENABLE()      HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET);
#define FLASH_CS_DISABLE()     HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET);

#define NAND_CS_ENABLE()     HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET);
#define NAND_CS_DISABLE()    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET);



#define FLASH_STATUS_OK 0

#define SECTOR_SIZE (4 * 1024*8) // ÿ�������Ĵ�С��32KBit

#define DEVICE_ID_64MBIT  0x17
#define DEVICE_ID_128MBIT 0x18
#define DEVICE_ID_256MBIT 0x14
#define DEVICE_ID_512MBIT 0x13
#define DEVICE_ID_1GBIT   0x12

#define PAGE_SIZE 256
// FLASH�����

#define FLASH_CMD_WRITE_ENABLE     0x06
#define FLASH_CMD_WRITE_DISABLE    0x04
#define FLASH_CMD_READ_STATUS_REG  0x05
#define FLASH_CMD_WRITE_STATUS_REG 0x01
#define FLASH_CMD_READ_DATA        0x03
#define FLASH_CMD_FAST_READ        0x0B
#define FLASH_CMD_PAGE_PROGRAM     0x02
#define FLASH_CMD_SECTOR_ERASE     0xD8
#define FLASH_CMD_BLOCK_ERASE_32K  0x52
#define FLASH_CMD_BLOCK_ERASE_64K  0xD8
#define FLASH_CMD_CHIP_ERASE       0xC7
#define FLASH_CMD_POWER_DOWN       0xB9
#define FLASH_CMD_RELEASE_POWER_DOWN 0xAB
#define FLASH_CMD_JEDEC_ID         0x9F
#define NAND_CMD_JEDEC_ID         0x90

typedef struct BG_Flash_Manager{

	void (*Init)(void (*Write)(uint8_t*,uint16_t),void (*Read)(uint8_t*,uint16_t));
	void (*Write)(uint8_t*,uint16_t);
	void (*Read)(uint8_t*,uint16_t);
	void (*PageProgram)(uint32_t,uint8_t*,uint16_t,uint8_t);
	void (*WriteEnable)(uint8_t,uint8_t);
	void (*DataErase)(uint32_t,uint32_t,uint8_t);
	void (*SectorErase)(uint32_t,uint8_t);
	void (*ReadData)(uint32_t,uint8_t*,uint16_t,uint8_t);
	void (*ReadID)(uint8_t*, uint8_t* , uint8_t*,uint8_t);
	uint32_t (*GetRemainingCapacity)(uint8_t);
	uint32_t (*GetTotalByte)(uint8_t);
	void (*EraseAll)(uint8_t);

}BG_Flash_Manager;

extern BG_Flash_Manager BG_flash_manager;


#endif
