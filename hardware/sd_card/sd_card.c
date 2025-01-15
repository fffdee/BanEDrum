#include "sd_card.h"
#include "sdio.h"
#include "usbd_cdc_if.h"


#define BLOCK_START_ADDR         0     /* Block start address      */
#define NUM_OF_BLOCKS            2   /* Total number of blocks   */
#define BUFFER_WORDS_SIZE        ((BLOCKSIZE * NUM_OF_BLOCKS) >> 2) /* Total data size in bytes */


uint8_t Buffer_Tx[512],Buffer_Rx[512] = {0};



void get_sd_info(void){

	
		if(HAL_SD_GetCardState(&hsd) == HAL_SD_CARD_TRANSFER) //��ȡ��ǰsd������״̬
		{
				// ���д����������жϣ����ڼ��SD���Ƿ������ݴ���״̬��
				// HAL_SD_GetCardState()�������ڻ�ȡSD����״̬��
				// &hsd��SD���ľ����handle������������SD���Ŀ��ƺ�״̬��Ϣ��
				// HAL_SD_CARD_TRANSFER��һ��ö��ֵ����ʾSD����ǰ���Խ������ݴ��䡣

				usb_printf("Initialize SD card successfully!\r\n");
				// ���SD���������ݴ���״̬����������ͨ��usb_printf������ӡһ����Ϣ��
				// ��ʾSD����ʼ���ɹ���usb_printf��һ���Զ���ĺ�������������ͨ��USB���ʹ�ӡ��Ϣ��

				// ��ӡSD��������Ϣ
				usb_printf(" SD card information! \r\n");
				// ��ӡһ����Ϣ����ʾ������Ҫ��ӡSD������Ϣ��

				usb_printf(" CardCapacity  : %llu \r\n", (unsigned long long)hsd.SdCard.BlockSize * hsd.SdCard.BlockNbr);
				// ��ӡSD������������������ͨ�����С��BlockSize�����Կ�������BlockNbr������õ��ġ�
				// %llu�Ǹ�ʽ����������ڴ�ӡ�޷��ŵĳ���������unsigned long long����

				usb_printf(" CardBlockSize : %d \r\n", hsd.SdCard.BlockSize);
				// ��ӡSD���Ŀ��С��

				usb_printf(" LogBlockNbr   : %d \r\n", hsd.SdCard.LogBlockNbr);
				// ��ӡSD���߼����������

				usb_printf(" LogBlockSize  : %d \r\n", hsd.SdCard.LogBlockSize);
				// ��ӡSD���߼���Ĵ�С��

				usb_printf(" RCA           : %d \r\n", hsd.SdCard.RelCardAdd);
				// ��ӡSD������Կ���ַ��Relative Card Address����

				usb_printf(" CardType      : %d \r\n", hsd.SdCard.CardType);
				// ��ӡSD�������͡�

				// ��ȡ����ӡSD����CID��Ϣ
				HAL_SD_CardCIDTypeDef sdcard_cid;
				// ����һ��HAL_SD_CardCIDTypeDef���͵ı���sdcard_cid�����ڴ洢SD����CID��Card IDentification����Ϣ��

				HAL_SD_GetCardCID(&hsd,&sdcard_cid);
				// ����HAL_SD_GetCardCID()��������ȡSD����CID��Ϣ����������洢��sdcard_cid�����С�

				usb_printf(" ManufacturerID: %d \r\n",sdcard_cid.ManufacturerID);
				// ��ӡSD����������ID������CID��Ϣ��һ���֡�
		}
		else
		{
				usb_printf("SD card init fail!\r\n" );
		}
		 

}

void sd_write_test(){
	uint32_t i;
	/* ��仺�������� */
		memset(Buffer_Tx, 0x15, sizeof(Buffer_Tx));
		Buffer_Tx[0] = 0xff;
/* ��SD����д������ */
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
		/* ��ȡ����֮������� */
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
