/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * �ļ���  ��adc.c
 * ����    ��adcӦ�ú�����        
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ�----------------------------------------------------
 *          |                                                    |
 *          | PC1 - ADC1 �����ⲿ��ѹ(ͨ��һ��������������ѹ����)|
 *          |                                                    |
 *           ----------------------------------------------------
 * ��汾  ��ST3.5.0 
 *
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/
#include "crc.h"

u16  CRC16(u8 *puchMsg, u8 usDataLen)
{
	u8 uchCRCHi = 0xFF ; /* ��ʼ�����ֽ�*/
	u8 uchCRCLo = 0xFF ; /* ��ʼ�����ֽ�*/
	u8 uIndex ; /*��CRC��*/
	while (usDataLen--) /*ͨ�����ݻ�����*/
	{
		uIndex = uchCRCHi ^ *puchMsg++ ; /*����CRC */
	 	uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ;
		uchCRCLo = auchCRCLo[uIndex] ;
	}
	return (uchCRCHi << 8 | uchCRCLo) ;
}

// unsigned short int CRC_16(unsigned char *data,int len)	  //С��
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
