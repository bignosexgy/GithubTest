#ifndef __TIMER_H
#define __TIMER_H

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
#define KEY_ON	0
#define KEY_OFF	1
 
//extern void DisplayRefresh(void); 
//extern void Beep(void); 
extern void Beep(u8 Beep_Static);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin); 
#endif
