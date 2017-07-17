#ifndef __TIMER_H
#define __TIMER_H

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
#define KEY_ON	0
#define KEY_OFF	1
 
//extern void DisplayRefresh(void); 
//extern void Beep(void); 
extern void Beep(u8 Beep_Static);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin); 
#endif
