#ifndef __VCNL4000_H
#define __VCNL4000_H

#include "i2c.h" 
#include "delay.h"  

//-----------�Ĵ���------------����12��������Ϊ����-------------------------------------------------------|
#define	VCNL4000_CMD		0x80	//VCNL��������	**	(��Ҫ)
#define	VCNL4000_ID			0x81	//VCNL ID �̶�ֵ=0x11
#define	VCNL4000_PROX_Measurement	0x82	//VCNL ID �̶�ֵ=0x11												
#define	VCNL4000_LEDCurrent	0x83	//VCNL IR LED Current Register for proximity function	
#define	VCNL4000_ALS_Para   0x84	//VCNL Ambient Light Parameter							
#define	VCNL4000_ALS_HSB	0x85	//VCNL Ambient Light Measurement Result HSB	 **	(��Ҫ)	
#define	VCNL4000_ALS_LSB	0x86	//VCNL Ambient Light Measurement Result LSB	 **			
#define	VCNL4000_PROX_HSB	0x87	//VCNL proximity measurement result HSB		 **			
#define	VCNL4000_PROX_LSB	0x88	//VCNL proximity measurement result LSB		 **

//-----------����------------------------------------------------------------------------------------------|
#define VCNL4000_Write		0x26 	//д����   	
#define VCNL4000_Read		0x27 	//������   	
#define VCNL4000_MODE_PROX	0x88	//PROX_OD=1��ʼ�����������1-200mm��
#define	VCNL4000_MODE_ALS	0x90	//ALS(Ambient Light Sensor),ALS_OD=1����ʼ���������

extern u16 VCNL4000_ALS_Result1;			//ALS�Ĳ������
extern u16 VCNL4000_PROX_Result1;			//PROX�Ĳ������
extern u8 VCNL4000_Register1[12];			//VCNL4000��12���Ĵ�����ֵ
extern u16 VCNL4000_ALS_Result2;			//ALS�Ĳ������
extern u16 VCNL4000_PROX_Result2;			//PROX�Ĳ������
extern u8 VCNL4000_Register2[12];			//VCNL4000��12���Ĵ�����ֵ

void VCNL4000_Init1(void);									//��ʼ��VCNL������
u8 VCNL4000_ReadID1(void);
void VCNL4000_StartMeasure1(u8 mode);
u8 VCNL4000_ReadOneByte1(u8 ReadAddr);						//����һ���ֽڴ�VCNL4000��ָ���ĵ�ַ
void VCNL4000_ReadAllRegisters1(u8 *RegisterBuf);			//����12���Ĵ�����ֵ
void VCNL4000_WriteOneByte1(u8 WriteAddr,u8 DataToWrite);	//д��һ���ֽڵ�VCNL4000��ָ���ĵ�ַ
void VCNL4000_Init2(void);									//��ʼ��VCNL������
u8 VCNL4000_ReadID2(void);
void VCNL4000_StartMeasure2(u8 mode);
u8 VCNL4000_ReadOneByte2(u8 ReadAddr);						//����һ���ֽڴ�VCNL4000��ָ���ĵ�ַ
void VCNL4000_ReadAllRegisters2(u8 *RegisterBuf);			//����12���Ĵ�����ֵ
void VCNL4000_WriteOneByte2(u8 WriteAddr,u8 DataToWrite);	//д��һ���ֽڵ�VCNL4000��ָ���ĵ�ַ

void VCNL4000_Measurement1(u16 ProxResult,u16 ALSResult);
void VCNL4000_Measurement2(u16 ProxResult,u16 ALSResult);


void VCNL4000_StartMeasurement1(void);
void VCNL4000_MeasurementResult1(u16 *AlsResult,u16 *ProxResult);	    //��ȡ�������
void VCNL4000_StartMeasurement2(void);
void VCNL4000_MeasurementResult2(u16 *AlsResult,u16 *ProxResult);	    //��ȡ�������
#endif

