#ifndef __HT1632_H
#define __HT1632_H

#include "sys.h"
//#include "delay.h" 
//#include "stm32f10x_lib.h"
/**/

#define cs1   PCout(7) 
#define wr   PCout(9)
#define rd PCout(8)
#define dat PAout(8)


#define ht1632_cs   PCout(7) 
#define ht1632_wr   PCout(9)
#define ht1632_rd PCout(8)
#define ht1632_data PAout(8)
//#define ht1632_rd   PAout(8)
 
#define C_COMMAND1632_SYSEN 0x01//SYSTEMENABLE系统使能
#define C_COMMAND1632_SYSDIS 0x00//SYSTEMDISABLE使能关这个要关掉若开LED全灭

#define C_COMMAND1632_OPTION 0X24//0X200x24//N-MOSOUTPUTAND16COMMONOPTION(8-com0x20)设定为16*24
#define C_COMMAND1632_LEDON 0x03//LEDON
#define C_COMMAND1632_LEDOFF 0x02//LEDOFF
#define C_COMMAND1632_BLINKOFF 0x08//BLINKOFF
#define C_COMMAND1632_BLINKON 0x09//BLINKON
#define C_COMMAND1632_PWMDUTY 0xaf//PWMDUTYSET(0xa0~0xaf)
#define C_COMMAND1632_SLAVE 0x10//SETSLAVEMODE//设为从模式
#define C_COMMAND1632_MASTER 0x18//0x18//SETMASTERMODE//设为主模式uchar
/**/
//   00    02   04   06   08   0A   0C  0E 

//R	 0X04 0X04 0X04 0X04 0X04 0X04 0X04 0X04
//G	 0X02 0X02 0X02 0X02 0X02 0X02 0X02 0X02 
//B	 0X01 0X01 0X01 0X01 0X01 0X01 0X01 0X01

void LED_red(void);
void LED_green(void);
void LED_blue(void);
#endif
