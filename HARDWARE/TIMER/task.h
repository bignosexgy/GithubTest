#ifndef __TASK_H
#define __TASK_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//通用定时器 驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2010/12/03
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
//#define Compressor_Close 0
//#define Compressor_Open 1
//#define Vacuumpump_Close 0
//#define Vacuumpump_Open 1
//#define	Vacuometer_Close 0
//#define	Vacuometer_Open 1


#define LOGIN_CMD	0x01//1：登录，
//11：登录响应，
#define DATA_CMD	0x02//21：数据采集，
//21：数据采集响应，
#define DAWN_CMD	0x03//3：下行控制，
#define DAWN_CMD_Reply	0x31//3：下行控制//31：下行控制响应，
#define HEART_CMD	0x04//4：心跳信息"
#define TIME_CMD	0x05//4：校时信息"



#define LED0_PWM_VAL TIM3->CCR2   

//extern void DisplayRefresh(void); 
//u8 Timer_280us_Flag; 

 int16_t get_params2(u8 num);
extern void MP3_Play(void); 
static void task_message(void);
static void send_message(void);
extern void LCD_time_update(void);
extern void DisplayProcess(u8 i);
extern void HT1632C_Write_CMD(unsigned char cmd);
extern void SPI_Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);
extern void SPI_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite); 
#endif
