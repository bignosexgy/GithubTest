#ifndef __6658spi_H
#define __6658spi_H

#include "sys.h"
//#include "delay.h" 
//#include "stm32f10x_lib.h"

//ע�⣺�����������ͺ�3���Ӻ������ͻ������Լ���Ӳ������Լ����ú�ʵ��

#define  T_UInt8		unsigned char				//�޷��ŵ��ֽ�
#define  T_UInt16		unsigned int				//�޷���2�ֽ�����
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

