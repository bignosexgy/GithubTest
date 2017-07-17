#ifndef __HT1622_H
#define __HT1622_H	 
#include "stm32f10x.h"

//HT1622¶Ë¿Ú¶¨Òå
#define LCD_CS PAout(3)// PA3
#define LCD_RD PAout(4)// PA4
#define LCD_WR PAout(5)// PA5
#define LCD_DA PAout(6)// PA6

void HT1622_Init(void);
void SendBit_1622(u8 data,u8 cnt);
void SendDataBit_1622(u8 data,u8 cnt);
void SendCmd(u8 command);
void Write_1622(u8 addr,u8 data);
void WriteAll_1622(u8 addr,u8 data);
void Display_RTCtime(u8 data[9]);
//u8 data_led[10]={0xee,0x48,0xd6,0xda,0x78,0xba,0xbe,0xc8,0xfe,0xfa};
//void disp_date(u8 data[32],u8 addr_start,u8 count);
#endif
