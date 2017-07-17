#ifndef __HSHCAL001B_H
#define __HSHCAL001B_H
#include "myiic.h"   

//Mini STM32开发板
//24CXX驱动函数(适合24C01~24C16,24C32~256未经过测试!有待验证!)
//正点原子@ALIENTEK
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
//Mini STM32开发板使用的是24c02，所以定义EE_TYPE为AT24C02
//#define EE_TYPE AT24C02
					  
u8 HSHCAL001B_ReadOneByte(u16 ReadAddr);							//指定地址读取一个字节
void HSHCAL001B_ReadMultiByte(u8 *ReadBuf,u16 ReadAddr,u8 ReadLenth);
//void HSHCAL001B_ReadMultiByte(u8 *ReadBuf,u8 ReadAddr,u8 ReadLenth);
void HSHCAL001B_WriteOneByte(u16 WriteAddr,u8 DataToWrite);		//指定地址写入一个字节
void HSHCAL001B_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//指定地址开始写入指定长度的数据
u32 HSHCAL001B_ReadLenByte(u16 ReadAddr,u8 Len);					//指定地址开始读取指定长度数据
void HSHCAL001B_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void HSHCAL001Bt_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead);   	//从指定地址开始读出指定长度的数据

u8 HSHCAL001B_Check(void);  //检查器件
void HSHCAL001B_Init(void); //初始化IIC
//static void HumInit(void);
void Temp_Humi_process(u16 *Sample, u8 *I2c_Sample);
//void Temp_Humi_process(double *Sample, u8 *I2c_Sample);

#endif
















