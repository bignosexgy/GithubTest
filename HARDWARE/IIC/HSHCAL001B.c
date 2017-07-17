#include "HSHCAL001B.h" 
#include "delay.h" 
#include "i2c_mems.h"



#define TRUE 1
#define FAUSE 0

 /*I2C Address*/
#define	HSHCAL_SLVADR	0x30

/*--------------------------------------------------------------------------------------------------------------*/
/*	Register Address(HSHCAL001B)																				*/
/*--------------------------------------------------------------------------------------------------------------*/

/* Register for hshcal */
#define HSHCAL_STR			0x0C	/* Self test response  */
#define HSHCAL_INF1			0x0D	/* More info Version   */
#define HSHCAL_INF2			0x0E	/* More info ALPS      */
#define HSHCAL_WIA			0x0F	/* Who I am            */
#define HSHCAL_HUMI1		0x10	/* Humidity output1    */
#define HSHCAL_HUMI2		0x11	/* Humidity output2    */
#define HSHCAL_TEMP1		0x12	/* Temperature output1 */
#define HSHCAL_TEMP2		0x13	/* Temperature output2 */
#define HSHCAL_STAT			0x18	/* Status              */
#define HSHCAL_CTL1			0x1B	/* Control1            */
#define HSHCAL_RDAT1		0x20	/* Reserved data1      */
#define HSHCAL_RDAT2		0x21	/* Reserved data2      */
#define HSHCAL_RDAT3		0x22	/* Reserved data3      */
#define HSHCAL_RDAT4		0x23	/* Reserved data4      */

/* Action Command Register for hshcal */
#define HSHCAL_REST			0x30	/* Reset command         */
#define HSHCAL_DET			0x31	/* Sensor detect command */
#define HSHCAL_STST			0x32	/* Self test command     */	
#define HSHCAL_CHIP_ID		0x2311

/* Self-test resiter value */
#define HSHCAL_ST_REG_DEF	0x55
#define HSHCAL_ST_REG_PASS	0xAA

/* Self-test error number */
#define HSHCAL_ST_OK		0x00
#define HSHCAL_ST_ERR_I2C	0x01
#define HSHCAL_ST_ERR_INIT	0x02
#define HSHCAL_ST_ERR_1ST	0x03
#define HSHCAL_ST_ERR_2ND	0x04

#define HSHCAL_MLT_DATA_ACCESS_NUM	4
#define HSHCAL_SIG_DATA_ACCESS_NUM	2
#define HSHCAL_INITIALL_DELAY		20

/*--------------------------------------------------------------------------------------------------------------*/
/*	Register丒Bit Define(HSHCAL001B)																			*/
/*--------------------------------------------------------------------------------------------------------------*/

#define		HUM_SRDY		0x80			//	The flag under starting (0:Inactive, 1:Active)
#define		HUM_DRDY		0x40			//	The flag of the end of humidity sensor measurement (0:Inactive, 1:Active)
#define		HUM_DOR			0x20			//	Detection of data overrun (0:No overrrun, 1:Detect Overrun)
#define		HUM_MEAS		0x10			//	The flag of measurement execution (0:Sleep, 1:Execute Measurement)
#define		HUM_TRDY		0x02			//	The flag of the end of temperature sensor measurement (0:Inactive, 1:Active)   

//Mini STM32开发板
//24CXX驱动函数(适合24C01~24C16,24C32~256未经过测试!有待验证!)
//正点原子@ALIENTEK
//2010/6/10
//V1.2
 /*******************************************************************************
* Function Name  : I2C_ByteWrite
* Description    : write a Byte to I2C Bus
* Input          : deviceAddr is the I2C address of the device
*                  WriteAddr is the register address you want to write to
*                  pBuffer contains bytes to write
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_ByteWrite(uint8_t* pBuffer, uint8_t deviceAddress, uint8_t WriteAddr);


/*******************************************************************************
* Function Name  : I2C_BufferRead
* Description    : read a numByteToRead bytes from I2C Bus
* Input          : deviceAddr is the I2C address of the device
*                  readAddr is the register address you want to read from
*                  numByteToRead is the number of bytes to read
* Output         : pBuffer is the buffer that contains bytes read
* Return         : None
*******************************************************************************/
uint8_t I2C_BufferRead(uint8_t* pBuffer, uint8_t deviceAddr, uint8_t readAddr, uint16_t numByteToRead);




//初始化IIC接口
void HSHCAL001B_Init(void)
{
	IIC1_Init();
}
//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
u8 HSHCAL001B_ReadOneByte(u16 ReadAddr)
{				  
	u8 temp,ACK_temp=0,i;		  	    																 
    IIC1_Start();  	   
	IIC1_Send_Byte(HSHCAL_SLVADR & 0xFE);	   //发送写命令
	ACK_temp=IIC1_Wait_Ack();	
    IIC1_Send_Byte(ReadAddr);   //发送低地址
	ACK_temp=IIC1_Wait_Ack();	    
	IIC1_Start();  	 	   	
    IIC1_Send_Byte(HSHCAL_SLVADR | 0x01);           //进入接收模式			   		   
	ACK_temp=IIC1_Wait_Ack();	 
    temp=IIC1_Read_Byte(0);	
	
    IIC1_Stop();//产生一个停止条件	    
	return temp;
}

void HSHCAL001B_ReadMultiByte(u8 *ReadBuf,u16 ReadAddr,u8 ReadLenth)   
{				  
	u8 temp[14]={0},ACK_temp=0,i;		  	    																 
    IIC1_Start();  	  
	IIC1_Send_Byte(HSHCAL_SLVADR & 0xFE);	   //发送写命令
	ACK_temp=IIC1_Wait_Ack(); 
	for(i=0;i<ReadLenth;i++)
	{
    IIC1_Send_Byte(ReadAddr++);   //发送低地址
	ACK_temp=IIC1_Wait_Ack();	    
	IIC1_Start();  	 	   	
    IIC1_Send_Byte(HSHCAL_SLVADR | 0x01);           //进入接收模式			   		   
	ACK_temp=IIC1_Wait_Ack();	 
    *(ReadBuf++)=IIC1_Read_Byte(0);		   
	}
    IIC1_Stop();//产生一个停止条件	    
	//return temp;
}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void HSHCAL001B_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC1_Start();  	
	IIC1_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 
	IIC1_Wait_Ack();	   
    IIC1_Send_Byte(WriteAddr%256);   //发送低地址
	IIC1_Wait_Ack(); 	 										  		   
	IIC1_Send_Byte(DataToWrite);     //发送字节							   
	IIC1_Wait_Ack();  		    	   
    IIC1_Stop();//产生一个停止条件 
	delay_ms(10);	 
}
//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void HSHCAL001B_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{  	
	u8 t;
	for(t=0;t<Len;t++)
	{
		HSHCAL001B_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
/*
u32 HSHCAL001B_ReadLenByte(u16 ReadAddr,u8 Len)
{  	
	u8 t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=HSHCAL001B_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}
*/
//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
/*
u8 HSHCAL001B_Check(void)
{
	u8 temp;
	temp=HSHCAL001B_ReadOneByte(255);//避免每次开机都写AT24CXX			   
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		HSHCAL001B_WriteOneByte(255,0X55);
	    temp=HSHCAL001B_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}
*/
//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
/*
void HSHCAL001B_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=HSHCAL001B_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
} 
*/ 
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数
void HSHCAL001B_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		HSHCAL001B_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}	
 u16 Temp,Humi;
//void Temp_Humi_process(double *Sample, u8 *I2c_Sample)
void Temp_Humi_process(u16 *Sample, u8 *I2c_Sample)
{
  //u16 Temp,Humi;
  u16 temp1,temp2,temp3,temp4; 
  u16 temp8[4];  
  temp8[0]= (u16)(*(I2c_Sample++) | *(I2c_Sample++)<<8);
  temp8[1]= (u16)(*(I2c_Sample++) | *(I2c_Sample++)<<8);
  Humi = (u16)(temp8[0]*15625/10000-1400)/100; //100倍 
  Temp = (u16)(2*temp8[1]-4192)/100;      //100倍
  if (Humi>0x63)
	  Humi=0x63;
  *(Sample++) = Humi;
  *(Sample++) = Temp;
} 