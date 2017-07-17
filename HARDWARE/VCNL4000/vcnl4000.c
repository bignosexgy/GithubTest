#include "vcnl4000.h" 

u16 VCNL4000_ALS_Result1 = 0;
u16 VCNL4000_PROX_Result1 = 0;
u8 VCNL4000_Register1[12] ={0};
u16 VCNL4000_ALS_Result2 = 0;
u16 VCNL4000_PROX_Result2 = 0;
u8 VCNL4000_Register2[12] ={0};

u16 VCNL4000_PROX_Result1_1 = 0xFFEE;
u16 VCNL4000_PROX_Result1_2 = 0xFFEE;
u16 VCNL4000_PROX_Result2_1 = 0xFFEE;
u16 VCNL4000_PROX_Result2_2 = 0xFFEE;
u16 VCNL4000_PROX_Res1 = 0;
u16 VCNL4000_PROX_Res2 = 0;

u8 VCNL4000_ReadOneByte1(u8 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    I2C_Start1();
	I2C_SendByte1(VCNL4000_Write);  	//����д����
	I2C_WaitAck1();	    
	I2C_SendByte1(ReadAddr);   	 	//������Ҫд��Ŀ�ĵ�ַ
	I2C_WaitAck1();
	I2C_Stop1();						//����һ��ֹͣ����

	I2C_Start1();  	 	   						   
	I2C_SendByte1(VCNL4000_Read);   	//д�� д����,�������ģʽ
	I2C_WaitAck1();	 				
    temp=I2C_ReadByte1();		    //��������
    I2C_Stop1();						//ֹͣ
		    
	return temp;					//���ض���������
}

u8 VCNL4000_ReadOneByte2(u8 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    I2C_Start2();
	I2C_SendByte2(VCNL4000_Write);  	//����д����
	I2C_WaitAck2();	    
	I2C_SendByte2(ReadAddr);   	 	//������Ҫд��Ŀ�ĵ�ַ
	I2C_WaitAck2();
	I2C_Stop2();						//����һ��ֹͣ����

	I2C_Start2();  	 	   						   
	I2C_SendByte2(VCNL4000_Read);   	//д�� д����,�������ģʽ
	I2C_WaitAck2();	 				
    temp=I2C_ReadByte2();		    //��������
    I2C_Stop2();						//ֹͣ
		    
	return temp;					//���ض���������
}

void VCNL4000_WriteOneByte1(u8 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    I2C_Start1();					//��ʼ
	I2C_SendByte1(VCNL4000_Write);	//����д����
	I2C_WaitAck1();					//�ȴ�VCNL����һ��ACK
    I2C_SendByte1(WriteAddr);   	 	//������Ҫд��Ŀ�ĵ�ַ
	I2C_WaitAck1();					//�ȴ�VCNL����һ��ACK
	I2C_SendByte1(DataToWrite);      //����Ҫд��ĵ�ַ							   
	I2C_WaitAck1();  		    	//�ȴ�VCNL����һ��ACK  
    I2C_Stop1();						//����һ��ֹͣ���� 
//	delay_ms(10);	 
}


void VCNL4000_WriteOneByte2(u8 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    I2C_Start2();					//��ʼ
	I2C_SendByte2(VCNL4000_Write);	//����д����
	I2C_WaitAck2();					//�ȴ�VCNL����һ��ACK
    I2C_SendByte2(WriteAddr);   	 	//������Ҫд��Ŀ�ĵ�ַ
	I2C_WaitAck2();					//�ȴ�VCNL����һ��ACK
	I2C_SendByte2(DataToWrite);      //����Ҫд��ĵ�ַ							   
	I2C_WaitAck2();  		    	//�ȴ�VCNL����һ��ACK  
    I2C_Stop2();						//����һ��ֹͣ���� 
//	delay_ms(10);	 
}


//----------��ʼ��----------
void VCNL4000_Init1(void)
{
	I2C_Initial1();										//��ʼ��I2C����
	VCNL4000_WriteOneByte1(VCNL4000_CMD,0x80);			//��������Ĵ�������������ı�־λ��������λ��ע:�˼Ĵ������λ�̶�Ϊ1.
	VCNL4000_WriteOneByte1(VCNL4000_LEDCurrent,0x14);	//IR LED current = 0x10*10 = 100ma������LED�ĵ������ã�����Խ�����Խǿ for proximity
	VCNL4000_WriteOneByte1(VCNL4000_PROX_Measurement,0x03);
//	VCNL4000_WriteOneByte1(VCNL4000_ALS_Para,0x0d);		//AMBIENT LIGHT PARAMETER:1)bit7 = 0 ����ת��ģʽѡ��;2)bit3=1,�Զ�ƫ�Ʋ���;3)bit2-bit0=5,ÿת��һ�β����Ĵ���
}
void VCNL4000_Init2(void)
{
	I2C_Initial2();										//��ʼ��I2C����
	VCNL4000_WriteOneByte2(VCNL4000_CMD,0x80);			//��������Ĵ�������������ı�־λ��������λ��ע:�˼Ĵ������λ�̶�Ϊ1.
	VCNL4000_WriteOneByte2(VCNL4000_LEDCurrent,0x14);	//IR LED current = 0x10*10 = 100ma������LED�ĵ������ã�����Խ�����Խǿ for proximity
    VCNL4000_WriteOneByte2(VCNL4000_PROX_Measurement,0x03);
//	VCNL4000_WriteOneByte2(VCNL4000_ALS_Para,0x0d);		//AMBIENT LIGHT PARAMETER:1)bit7 = 0 ����ת��ģʽѡ��;2)bit3=1,�Զ�ƫ�Ʋ���;3)bit2-bit0=5,ÿת��һ�β����Ĵ���
}



void VCNL4000_ReadAllRegisters1(u8 *RegisterBuf)
{
	u8 i=0;
	for(i=0;i<12;i++)
	{
		RegisterBuf[i]=VCNL4000_ReadOneByte1(0x80+i);
	}
}


void VCNL4000_ReadAllRegisters2(u8 *RegisterBuf)
{
	u8 i=0;
	for(i=0;i<12;i++)
	{
		RegisterBuf[i]=VCNL4000_ReadOneByte2(0x80+i);
	}
}

void VCNL4000_MeasurementResult1(u16 *AlsResult,u16 *ProxResult)	    //��ȡ�������
{
	VCNL4000_Init1();									//��ʼ��VCNL4000
//	VCNL4000_WriteOneByte1(VCNL4000_CMD,0x98);			//д����Ĵ���,��ʼ����
//	delay_ms(10);
	//while((VCNL4000_ReadOneByte1(0x80)&0x60 != 0x20));	//�ж����������Ƿ����
	if((VCNL4000_ReadOneByte1(0x80)&0x60 == 0x20));
//	*AlsResult = (u16)VCNL4000_ReadOneByte1(VCNL4000_ALS_HSB)<<8 | VCNL4000_ReadOneByte1(VCNL4000_ALS_LSB);
	*ProxResult = (u16)VCNL4000_ReadOneByte1(VCNL4000_PROX_HSB)<<8 | VCNL4000_ReadOneByte1(VCNL4000_PROX_LSB);
}

void VCNL4000_MeasurementResult2(u16 *AlsResult,u16 *ProxResult)	    //��ȡ�������
{
	VCNL4000_Init2();									//��ʼ��VCNL4000
//	VCNL4000_WriteOneByte2(VCNL4000_CMD,0x98);			//д����Ĵ���,��ʼ����
//	delay_ms(10);
//	while((VCNL4000_ReadOneByte2(0x80)&0x60 != 0x20));	//�ж����������Ƿ����
	if((VCNL4000_ReadOneByte2(0x80)&0x60 == 0x20));
//	*AlsResult = (u16)VCNL4000_ReadOneByte2(VCNL4000_ALS_HSB)<<8 | VCNL4000_ReadOneByte2(VCNL4000_ALS_LSB);
	*ProxResult = (u16)VCNL4000_ReadOneByte2(VCNL4000_PROX_HSB)<<8 | VCNL4000_ReadOneByte2(VCNL4000_PROX_LSB);
}
