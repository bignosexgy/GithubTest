//#include "mmc_sd.h"
#include "diskio.h"
#include "spi_flash.h"
//#include "malloc.h"

#include "stm32f10x.h"
extern u16 SPI_FLASH_TYPE;		 		   
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEKս��STM32������
//FATFS disio.c ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/18
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

 
#define SD_CARD	 1  //SD��,����Ϊ0
#define EX_FLASH 0	//�ⲿflash,����Ϊ1

#define FLASH_SECTOR_SIZE 	512			  
//����W25Q64 
//ǰ6M�ֽڸ�fatfs��,6M�ֽں�~6M+500K���û���,6M+500K�Ժ�,���ڴ���ֿ�,�ֿ�ռ��1.5M.		 			    
u32	    FLASH_SECTOR_COUNT=2048*64;//6M�ֽ�,Ĭ��ΪW25Q64
#define FLASH_BLOCK_SIZE  	8     //ÿ��BLOCK��8������



//��ʼ������
DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{	
	u8 res=0;	    
	switch(drv)
	{
//		case SD_CARD://SD��
//			res = SD_Initialize();//SD_Initialize() 
//		 	if(res)//STM32 SPI��bug,��sd������ʧ�ܵ�ʱ�������ִ����������,���ܵ���SPI��д�쳣
//			{
//				SD_SPI_SpeedLow();
//				SD_SPI_ReadWriteByte(0xff);//�ṩ�����8��ʱ��
//				SD_SPI_SpeedHigh();
//			}
//  			break;
//		case EX_FLASH://�ⲿflash
//			SPI_Flash_Init();
//			if(SPI_FLASH_TYPE==W25Q64)FLASH_SECTOR_COUNT=2048*6;//W25Q64
//			else FLASH_SECTOR_COUNT=2048*2;						//����
// 			break;
//		default:
//			res=1; 
	}		 
	if(res)return  STA_NOINIT;
	else return 0; //��ʼ���ɹ�
}   
//��ô���״̬
DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{		   
    return 0;
}
 //������
 //drv:���̱��0~9
 //*buff:���ݽ��ջ����׵�ַ
 //sector:������ַ
 //count:��Ҫ��ȡ��������
DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{
	u8 res=0; 
    if (!count)return RES_PARERR;//count���ܵ���0�����򷵻ز�������		 	 
	switch(drv)
	{
//		case SD_CARD://SD��
//			res=SD_ReadDisk(buff,sector,count);	 
//		 	if(res)//STM32 SPI��bug,��sd������ʧ�ܵ�ʱ�������ִ����������,���ܵ���SPI��д�쳣
//			{
//				SD_SPI_SpeedLow();
//				SD_SPI_ReadWriteByte(0xff);//�ṩ�����8��ʱ��
//				SD_SPI_SpeedHigh();
//			}
//			break;
		case EX_FLASH://�ⲿflash
			for(;count>0;count--)
			{
				SPI_Flash_Read(buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
				sector++;
				buff+=FLASH_SECTOR_SIZE;
			}
			res=0;
			break;
		default:
			res=1; 
	}
   //��������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    if(res==0x00)return RES_OK;	 
    else return RES_ERROR;	   
}  
 //д����
 //drv:���̱��0~9
 //*buff:���������׵�ַ
 //sector:������ַ
 //count:��Ҫд���������	    
#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	        /* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
	u8 res=0;  
    if (!count)return RES_PARERR;//count���ܵ���0�����򷵻ز�������		 	 
	switch(drv)
	{
//		case SD_CARD://SD��
//			res=SD_WriteDisk((u8*)buff,sector,count);
//			break;
		case EX_FLASH://�ⲿflash
			for(;count>0;count--)
			{										    
				SPI_Flash_Write((u8*)buff,sector*FLASH_SECTOR_SIZE,FLASH_SECTOR_SIZE);
				sector++;
				buff+=FLASH_SECTOR_SIZE;
			}
			res=0;
			break;
		default:
			res=1; 
	}
    //��������ֵ����SPI_SD_driver.c�ķ���ֵת��ff.c�ķ���ֵ
    if(res == 0x00)return RES_OK;	 
    else return RES_ERROR;		 
}
#endif /* _READONLY */

//�����������Ļ��
 //drv:���̱��0~9
 //ctrl:���ƴ���
 //*buff:����/���ջ�����ָ��
DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{	
	DRESULT res;						  			     
	if(drv==SD_CARD)//SD��
	{
	    switch(ctrl)
	    {
//		    case CTRL_SYNC:
//				SD_CS=0;
//		        if(SD_WaitReady()==0)res = RES_OK; 
//		        else res = RES_ERROR;	  
//				SD_CS=1;
//		        break;	 
//		    case GET_SECTOR_SIZE:
//		        *(WORD*)buff = 512;
//		        res = RES_OK;
//		        break;	 
//		    case GET_BLOCK_SIZE:
//		        *(WORD*)buff = 8;
//		        res = RES_OK;
//		        break;	 
//		    case GET_SECTOR_COUNT:
//		        *(DWORD*)buff = SD_GetSectorCount();
//		        res = RES_OK;
//		        break;
//		    default:
//		        res = RES_PARERR;
//		        break;
	    }
	}else if(drv==EX_FLASH)	//�ⲿFLASH  
	{
	    switch(ctrl)
	    {
		    case CTRL_SYNC:
				res = RES_OK; 
		        break;	 
		    case GET_SECTOR_SIZE:
		        *(WORD*)buff = FLASH_SECTOR_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
		        *(WORD*)buff = FLASH_BLOCK_SIZE;
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = FLASH_SECTOR_COUNT;
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }
	}else res=RES_ERROR;//�����Ĳ�֧��
    return res;
}   
//���ʱ��
//User defined function to give a current time to fatfs module      */
//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{				 
	return 0;
}			 
//��̬�����ڴ�
//void *ff_memalloc (UINT size)			
//{
////	return (void*)mymalloc(SRAMIN,size);
//}
//�ͷ��ڴ�
void ff_memfree (void* mf)		 
{
//	myfree(SRAMIN,mf);
}











