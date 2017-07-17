#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
//Mini STM32开发板
//SPI 驱动 V1.1
//正点原子@ALIENTEK
//2010/6/13	

// SPI总线速度设置
#define SPI_SPEED_2   		0
#define SPI_SPEED_4   		1
#define SPI_SPEED_8   		2
#define SPI_SPEED_16  		3
#define SPI_SPEED_32 		4
#define SPI_SPEED_64 		5
#define SPI_SPEED_128 		6
#define SPI_SPEED_256 		7 

static char *itoa_1(int value, char *string, int radix)	;					  	    													  
void SPIx_Init(void);			 //初始化SPI口
void SPIx_SetSpeed(u8 SpeedSet); //设置SPI速度   
u8 SPIx_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
void Write_Command(unsigned char Data);
void Write_Data(unsigned char Data);
u8 SPI1_ReadWriteByte(u8 TxData);
extern void Delay(__IO u32 nCount) ;
extern void delay_us(u32 nus);
void OLED_Init(void);		 
#endif

