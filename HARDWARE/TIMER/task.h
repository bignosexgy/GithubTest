#ifndef __TASK_H
#define __TASK_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//ͨ�ö�ʱ�� ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/12/03
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	  
//#define Compressor_Close 0
//#define Compressor_Open 1
//#define Vacuumpump_Close 0
//#define Vacuumpump_Open 1
//#define	Vacuometer_Close 0
//#define	Vacuometer_Open 1


#define LOGIN_CMD	0x01//1����¼��
//11����¼��Ӧ��
#define DATA_CMD	0x02//21�����ݲɼ���
//21�����ݲɼ���Ӧ��
#define DAWN_CMD	0x03//3�����п��ƣ�
#define DAWN_CMD_Reply	0x31//3�����п���//31�����п�����Ӧ��
#define HEART_CMD	0x04//4��������Ϣ"
#define TIME_CMD	0x05//4��Уʱ��Ϣ"



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
