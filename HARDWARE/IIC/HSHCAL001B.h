#ifndef __HSHCAL001B_H
#define __HSHCAL001B_H
#include "myiic.h"   

//Mini STM32������
//24CXX��������(�ʺ�24C01~24C16,24C32~256δ��������!�д���֤!)
//����ԭ��@ALIENTEK
//2010/6/10
//V1.2
//#define AT24C01		127
//#define AT24C02		255
//#define AT24C04		511
//#define AT24C08		1023
//#define AT24C16		2047
//#define AT24C32		4095
//#define AT24C64	    8191
//#define AT24C128	16383
//#define AT24C256	32767  
//Mini STM32������ʹ�õ���24c02�����Զ���EE_TYPEΪAT24C02
//#define EE_TYPE AT24C02
					  
u8 HSHCAL001B_ReadOneByte(u16 ReadAddr);							//ָ����ַ��ȡһ���ֽ�
void HSHCAL001B_ReadMultiByte(u8 *ReadBuf,u16 ReadAddr,u8 ReadLenth);
//void HSHCAL001B_ReadMultiByte(u8 *ReadBuf,u8 ReadAddr,u8 ReadLenth);
void HSHCAL001B_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//ָ����ַд��һ���ֽ�
void HSHCAL001B_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//ָ����ַ��ʼд��ָ�����ȵ�����
u32 HSHCAL001B_ReadLenByte(u16 ReadAddr,u8 Len);					//ָ����ַ��ʼ��ȡָ����������
void HSHCAL001B_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//��ָ����ַ��ʼд��ָ�����ȵ�����
void HSHCAL001Bt_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//��ָ����ַ��ʼ����ָ�����ȵ�����

u8 HSHCAL001B_Check(void);  //�������
void HSHCAL001B_Init(void); //��ʼ��IIC
//static void HumInit(void);
void Temp_Humi_process(u16 *Sample, u8 *I2c_Sample);
//void Temp_Humi_process(double *Sample, u8 *I2c_Sample);

#endif
















