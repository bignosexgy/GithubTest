#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
//Mini STM32������
//SPI ���� V1.1
//����ԭ��@ALIENTEK
//2010/6/13	

// SPI�����ٶ�����
#define SPI_SPEED_2   		0
#define SPI_SPEED_4   		1
#define SPI_SPEED_8   		2
#define SPI_SPEED_16  		3
#define SPI_SPEED_32 		4
#define SPI_SPEED_64 		5
#define SPI_SPEED_128 		6
#define SPI_SPEED_256 		7 

static char *itoa_1(int value, char *string, int radix)	;					  	    													  
void SPIx_Init(void);			 //��ʼ��SPI��
void SPIx_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPIx_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�
void Write_Command(unsigned char Data);
void Write_Data(unsigned char Data);
u8 SPI1_ReadWriteByte(u8 TxData);
extern void Delay(__IO u32 nCount) ;
extern void delay_us(u32 nus);
void OLED_Init(void);		 
#endif

