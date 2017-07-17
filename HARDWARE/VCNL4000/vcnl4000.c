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
	I2C_SendByte1(VCNL4000_Write);  	//发送写命令
	I2C_WaitAck1();	    
	I2C_SendByte1(ReadAddr);   	 	//发送需要写的目的地址
	I2C_WaitAck1();
	I2C_Stop1();						//产生一个停止条件

	I2C_Start1();  	 	   						   
	I2C_SendByte1(VCNL4000_Read);   	//写入 写命令,进入接收模式
	I2C_WaitAck1();	 				
    temp=I2C_ReadByte1();		    //接收数据
    I2C_Stop1();						//停止
		    
	return temp;					//返回读出的数据
}

u8 VCNL4000_ReadOneByte2(u8 ReadAddr)
{				  
	u8 temp=0;		  	    																 
    I2C_Start2();
	I2C_SendByte2(VCNL4000_Write);  	//发送写命令
	I2C_WaitAck2();	    
	I2C_SendByte2(ReadAddr);   	 	//发送需要写的目的地址
	I2C_WaitAck2();
	I2C_Stop2();						//产生一个停止条件

	I2C_Start2();  	 	   						   
	I2C_SendByte2(VCNL4000_Read);   	//写入 写命令,进入接收模式
	I2C_WaitAck2();	 				
    temp=I2C_ReadByte2();		    //接收数据
    I2C_Stop2();						//停止
		    
	return temp;					//返回读出的数据
}

void VCNL4000_WriteOneByte1(u8 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    I2C_Start1();					//开始
	I2C_SendByte1(VCNL4000_Write);	//发送写命令
	I2C_WaitAck1();					//等待VCNL返回一个ACK
    I2C_SendByte1(WriteAddr);   	 	//发送需要写的目的地址
	I2C_WaitAck1();					//等待VCNL返回一个ACK
	I2C_SendByte1(DataToWrite);      //发送要写入的地址							   
	I2C_WaitAck1();  		    	//等待VCNL返回一个ACK  
    I2C_Stop1();						//产生一个停止条件 
//	delay_ms(10);	 
}


void VCNL4000_WriteOneByte2(u8 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    I2C_Start2();					//开始
	I2C_SendByte2(VCNL4000_Write);	//发送写命令
	I2C_WaitAck2();					//等待VCNL返回一个ACK
    I2C_SendByte2(WriteAddr);   	 	//发送需要写的目的地址
	I2C_WaitAck2();					//等待VCNL返回一个ACK
	I2C_SendByte2(DataToWrite);      //发送要写入的地址							   
	I2C_WaitAck2();  		    	//等待VCNL返回一个ACK  
    I2C_Stop2();						//产生一个停止条件 
//	delay_ms(10);	 
}


//----------初始化----------
void VCNL4000_Init1(void)
{
	I2C_Initial1();										//初始化I2C引脚
	VCNL4000_WriteOneByte1(VCNL4000_CMD,0x80);			//清零命令寄存器，清除测量的标志位，和允许位，注:此寄存器最高位固定为1.
	VCNL4000_WriteOneByte1(VCNL4000_LEDCurrent,0x14);	//IR LED current = 0x10*10 = 100ma，红外LED的电流设置，电流越大红外越强 for proximity
	VCNL4000_WriteOneByte1(VCNL4000_PROX_Measurement,0x03);
//	VCNL4000_WriteOneByte1(VCNL4000_ALS_Para,0x0d);		//AMBIENT LIGHT PARAMETER:1)bit7 = 0 连续转换模式选择;2)bit3=1,自动偏移补偿;3)bit2-bit0=5,每转换一次测量的次数
}
void VCNL4000_Init2(void)
{
	I2C_Initial2();										//初始化I2C引脚
	VCNL4000_WriteOneByte2(VCNL4000_CMD,0x80);			//清零命令寄存器，清除测量的标志位，和允许位，注:此寄存器最高位固定为1.
	VCNL4000_WriteOneByte2(VCNL4000_LEDCurrent,0x14);	//IR LED current = 0x10*10 = 100ma，红外LED的电流设置，电流越大红外越强 for proximity
    VCNL4000_WriteOneByte2(VCNL4000_PROX_Measurement,0x03);
//	VCNL4000_WriteOneByte2(VCNL4000_ALS_Para,0x0d);		//AMBIENT LIGHT PARAMETER:1)bit7 = 0 连续转换模式选择;2)bit3=1,自动偏移补偿;3)bit2-bit0=5,每转换一次测量的次数
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

void VCNL4000_MeasurementResult1(u16 *AlsResult,u16 *ProxResult)	    //获取测量结果
{
	VCNL4000_Init1();									//初始化VCNL4000
//	VCNL4000_WriteOneByte1(VCNL4000_CMD,0x98);			//写命令寄存器,开始测量
//	delay_ms(10);
	//while((VCNL4000_ReadOneByte1(0x80)&0x60 != 0x20));	//判断两个测量是否都完成
	if((VCNL4000_ReadOneByte1(0x80)&0x60 == 0x20));
//	*AlsResult = (u16)VCNL4000_ReadOneByte1(VCNL4000_ALS_HSB)<<8 | VCNL4000_ReadOneByte1(VCNL4000_ALS_LSB);
	*ProxResult = (u16)VCNL4000_ReadOneByte1(VCNL4000_PROX_HSB)<<8 | VCNL4000_ReadOneByte1(VCNL4000_PROX_LSB);
}

void VCNL4000_MeasurementResult2(u16 *AlsResult,u16 *ProxResult)	    //获取测量结果
{
	VCNL4000_Init2();									//初始化VCNL4000
//	VCNL4000_WriteOneByte2(VCNL4000_CMD,0x98);			//写命令寄存器,开始测量
//	delay_ms(10);
//	while((VCNL4000_ReadOneByte2(0x80)&0x60 != 0x20));	//判断两个测量是否都完成
	if((VCNL4000_ReadOneByte2(0x80)&0x60 == 0x20));
//	*AlsResult = (u16)VCNL4000_ReadOneByte2(VCNL4000_ALS_HSB)<<8 | VCNL4000_ReadOneByte2(VCNL4000_ALS_LSB);
	*ProxResult = (u16)VCNL4000_ReadOneByte2(VCNL4000_PROX_HSB)<<8 | VCNL4000_ReadOneByte2(VCNL4000_PROX_LSB);
}
