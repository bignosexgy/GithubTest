/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * 文件名  ：adc.c
 * 描述    ：adc应用函数库        
 * 实验平台：野火STM32开发板
 * 硬件连接：----------------------------------------------------
 *          |                                                    |
 *          | PC1 - ADC1 连接外部电压(通过一个滑动变阻器分压而来)|
 *          |                                                    |
 *           ----------------------------------------------------
 * 库版本  ：ST3.5.0 
 *
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/
#include "crc.h"

u16  CRC16(u8 *puchMsg, u8 usDataLen)
{
	u8 uchCRCHi = 0xFF ; /* 初始化高字节*/
	u8 uchCRCLo = 0xFF ; /* 初始化低字节*/
	u8 uIndex ; /*把CRC表*/
	while (usDataLen--) /*通过数据缓冲器*/
	{
		uIndex = uchCRCHi ^ *puchMsg++ ; /*计算CRC */
	 	uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
		uchCRCLo = auchCRCLo[uIndex] ;
	}
	return (uchCRCHi << 8 | uchCRCLo) ;
}

// unsigned short int CRC_16(unsigned char *data,int len)	  //小端
//{
//	unsigned char *buf;
//	unsigned short int * CRC;
//	unsigned short int crch,crcl;
//	uchar p;
//	uchar j;
//	char err;
//
//	buf= & data[len];
//	CRC=(unsigned short int *)buf;
//	buf[0]=0xff;//lsb
//	buf[1]=0xff;//msb
//	for(p=0;p<len;p++)
//	{
//		buf[1]=buf[1]^data[p];
//		for(j=0;j<8;j++)
//		{
//			err=buf[1]&1;
//			
//			*CRC=*CRC/2;
//			
//			if(err) *CRC=*CRC^0xa001;
//		}
//	}
//	  crch=*CRC>>8;
//    crcl=*CRC<<8;
//    *CRC=crch+crcl;
//	return(*CRC);
//}
