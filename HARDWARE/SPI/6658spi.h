#ifndef __6658spi_H
#define __6658spi_H

#include "sys.h"
//#include "delay.h" 
//#include "stm32f10x_lib.h"

//注意：以下数据类型和3个子函数，客户根据自己的硬件情况自己配置和实现

#define  T_UInt8		unsigned char				//无符号单字节
#define  T_UInt16		unsigned int				//无符号2字节整型
#define  T_VOID		void

#define true 1
#define false 0

#define BUSY   PEin(14) 
#define SPISEL PEout(12)
#define SPISCK PEout(13)
#define SPISDI PEout(11)
#define SPISDO PEin(10)
#define SPIRES PEout(15)
//sbit BUSY = P3^2;
//
//sbit SPISEL = P1^7;
//sbit SPISCK = P1^6;
//sbit SPISDI = P1^5;
//sbit SPISDO = P1^4;

#define TTS_SYNC  0x5A



#endif

