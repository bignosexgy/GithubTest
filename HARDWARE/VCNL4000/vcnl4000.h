#ifndef __VCNL4000_H
#define __VCNL4000_H

#include "i2c.h" 
#include "delay.h"  

//-----------寄存器------------共有12个，下面为常用-------------------------------------------------------|
#define	VCNL4000_CMD		0x80	//VCNL控制命令	**	(重要)
#define	VCNL4000_ID			0x81	//VCNL ID 固定值=0x11
#define	VCNL4000_PROX_Measurement	0x82	//VCNL ID 固定值=0x11												
#define	VCNL4000_LEDCurrent	0x83	//VCNL IR LED Current Register for proximity function	
#define	VCNL4000_ALS_Para   0x84	//VCNL Ambient Light Parameter							
#define	VCNL4000_ALS_HSB	0x85	//VCNL Ambient Light Measurement Result HSB	 **	(重要)	
#define	VCNL4000_ALS_LSB	0x86	//VCNL Ambient Light Measurement Result LSB	 **			
#define	VCNL4000_PROX_HSB	0x87	//VCNL proximity measurement result HSB		 **			
#define	VCNL4000_PROX_LSB	0x88	//VCNL proximity measurement result LSB		 **

//-----------命令------------------------------------------------------------------------------------------|
#define VCNL4000_Write		0x26 	//写命令   	
#define VCNL4000_Read		0x27 	//读命令   	
#define VCNL4000_MODE_PROX	0x88	//PROX_OD=1开始近距离测量（1-200mm）
#define	VCNL4000_MODE_ALS	0x90	//ALS(Ambient Light Sensor),ALS_OD=1，开始环境光测试

extern u16 VCNL4000_ALS_Result1;			//ALS的测量结果
extern u16 VCNL4000_PROX_Result1;			//PROX的测量结果
extern u8 VCNL4000_Register1[12];			//VCNL4000的12个寄存器的值
extern u16 VCNL4000_ALS_Result2;			//ALS的测量结果
extern u16 VCNL4000_PROX_Result2;			//PROX的测量结果
extern u8 VCNL4000_Register2[12];			//VCNL4000的12个寄存器的值

void VCNL4000_Init1(void);									//初始化VCNL的引脚
u8 VCNL4000_ReadID1(void);
void VCNL4000_StartMeasure1(u8 mode);
u8 VCNL4000_ReadOneByte1(u8 ReadAddr);						//读出一个字节从VCNL4000中指定的地址
void VCNL4000_ReadAllRegisters1(u8 *RegisterBuf);			//读出12个寄存器的值
void VCNL4000_WriteOneByte1(u8 WriteAddr,u8 DataToWrite);	//写入一个字节到VCNL4000中指定的地址
void VCNL4000_Init2(void);									//初始化VCNL的引脚
u8 VCNL4000_ReadID2(void);
void VCNL4000_StartMeasure2(u8 mode);
u8 VCNL4000_ReadOneByte2(u8 ReadAddr);						//读出一个字节从VCNL4000中指定的地址
void VCNL4000_ReadAllRegisters2(u8 *RegisterBuf);			//读出12个寄存器的值
void VCNL4000_WriteOneByte2(u8 WriteAddr,u8 DataToWrite);	//写入一个字节到VCNL4000中指定的地址

void VCNL4000_Measurement1(u16 ProxResult,u16 ALSResult);
void VCNL4000_Measurement2(u16 ProxResult,u16 ALSResult);


void VCNL4000_StartMeasurement1(void);
void VCNL4000_MeasurementResult1(u16 *AlsResult,u16 *ProxResult);	    //获取测量结果
void VCNL4000_StartMeasurement2(void);
void VCNL4000_MeasurementResult2(u16 *AlsResult,u16 *ProxResult);	    //获取测量结果
#endif

