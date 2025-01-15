#include "sd_card.h"
#include "sdio.h"
#include "usbd_cdc_if.h"


#define BLOCK_START_ADDR         0     /* Block start address      */
#define NUM_OF_BLOCKS            2   /* Total number of blocks   */
#define BUFFER_WORDS_SIZE        ((BLOCKSIZE * NUM_OF_BLOCKS) >> 2) /* Total data size in bytes */


uint8_t Buffer_Tx[512],Buffer_Rx[512] = {0};



void get_sd_info(void){

	
		if(HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_TRANSFER) //获取当前sd卡数据状态
		{
				// 这行代码是条件判断，用于检查SD卡是否处于数据传输状态。
				// HAL_SD_GetCardState()函数用于获取SD卡的状态。
				// &hsd是SD卡的句柄（handle），它包含了SD卡的控制和状态信息。
				// HAL_SD_CARD_TRANSFER是一个枚举值，表示SD卡当前可以进行数据传输。

				usb_printf("Initialize SD card successfully!\r\n");
				// 如果SD卡处于数据传输状态，这条语句会通过usb_printf函数打印一条消息，
				// 表示SD卡初始化成功。usb_printf是一个自定义的函数，可能用于通过USB发送打印信息。

				// 打印SD卡基本信息
				usb_printf(" SD card information! \r\n");
				// 打印一条信息，表示接下来要打印SD卡的信息。

				usb_printf(" CardCapacity  : %llu \r\n", (unsigned long long)hsd.SdCard.BlockSize * hsd.SdCard.BlockNbr);
				// 打印SD卡的总容量。容量是通过块大小（BlockSize）乘以块数量（BlockNbr）计算得到的。
				// %llu是格式化输出，用于打印无符号的长长整数（unsigned long long）。

				usb_printf(" CardBlockSize : %d \r\n", hsd.SdCard.BlockSize);
				// 打印SD卡的块大小。

				usb_printf(" LogBlockNbr   : %d \r\n", hsd.SdCard.LogBlockNbr);
				// 打印SD卡逻辑块的数量。

				usb_printf(" LogBlockSize  : %d \r\n", hsd.SdCard.LogBlockSize);
				// 打印SD卡逻辑块的大小。

				usb_printf(" RCA           : %d \r\n", hsd.SdCard.RelCardAdd);
				// 打印SD卡的相对卡地址（Relative Card Address）。

				usb_printf(" CardType      : %d \r\n", hsd.SdCard.CardType);
				// 打印SD卡的类型。

				// 读取并打印SD卡的CID信息
				HAL_SD_CardCIDTypeDef sdcard_cid;
				// 声明一个HAL_SD_CardCIDTypeDef类型的变量sdcard_cid，用于存储SD卡的CID（Card IDentification）信息。

				HAL_SD_GetCardCID(&hsd,&sdcard_cid);
				// 调用HAL_SD_GetCardCID()函数来读取SD卡的CID信息，并将结果存储在sdcard_cid变量中。

				usb_printf(" ManufacturerID: %d \r\n",sdcard_cid.ManufacturerID);
				// 打印SD卡的制造商ID，这是CID信息的一部分。
		}
		else
		{
				usb_printf("SD card init fail!\r\n" );
		}
		 

}

void sd_write_test(){
	uint32_t i;
	/* 填充缓冲区数据 */
		memset(Buffer_Tx, 0x15, sizeof(Buffer_Tx));
		Buffer_Tx[0] = 0xff;
/* 向SD卡块写入数据 */
			usb_printf("------------------- Write SD card block data Test ------------------\r\n");
			if(HAL_SD_WriteBlocks_DMA(&hsd, Buffer_Tx, BLOCK_START_ADDR, NUM_OF_BLOCKS) == HAL_OK)
			{
					while(HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER)
					{
					}
					usb_printf("\r\nWrite Block Success!\r\n");
					for(i = 0; i < sizeof(Buffer_Tx); i++)
					{
						usb_printf("0x%02x:%02x ", i, Buffer_Tx[i]);
					}
					//usb_printf("%s",Buffer_Tx);
					usb_printf("\r\n");
			}
			else
			{
					usb_printf("\r\nWrite Block Failed!\r\n");
			}	
}

void sd_read_test()
{
		uint32_t i;
		/* 读取操作之后的数据 */
	usb_printf("------------------- Read SD card block data after Write ------------------\r\n");
	 
	if(HAL_SD_ReadBlocks_DMA(&hsd, Buffer_Rx, BLOCK_START_ADDR, NUM_OF_BLOCKS) == HAL_OK)
	{
		
			while(HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER)
			{
			}
			usb_printf("\r\nRead Block Success!\r\n");
			for(i = 0; i < sizeof(Buffer_Rx); i++)
			{
				usb_printf("0x%02x:%02x ", i, Buffer_Rx[i]);
			}
			usb_printf("\r\n");
			usb_printf("Read%s",&Buffer_Rx[0]);
			usb_printf("\r\n");
	}
	else
	{
			usb_printf("\r\nRead Block Failed!\r\n");				
	}
	 
}
