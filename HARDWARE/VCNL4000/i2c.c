#include "i2c.h"

#define SCL_H1         GPIOD->BSRR = GPIO_Pin_13
#define SCL_L1         GPIOD->BRR  = GPIO_Pin_13 
   
#define SDA_H1         GPIOD->BSRR = GPIO_Pin_14
#define SDA_L1         GPIOD->BRR  = GPIO_Pin_14

#define SDA_read1      GPIOD->IDR  & GPIO_Pin_14

#define SCL_H2         GPIOC->BSRR = GPIO_Pin_6
#define SCL_L2         GPIOC->BRR  = GPIO_Pin_6 
   
#define SDA_H2         GPIOD->BSRR = GPIO_Pin_15
#define SDA_L2         GPIOD->BRR  = GPIO_Pin_15

#define SDA_read2      GPIOD->IDR  & GPIO_Pin_15
//|---------------------------------------------------|
//|Function		:I2C_Init
//|Description	:initialize I2C GPIO,SCL and SDA
//|Input		:None
//|Output		:None
//|Return		:None
//|---------------------------------------------------|
void I2C_Initial1(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE); 		//有时间测试下，不开I2C1的时钟可以吗？
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);		//GPB时钟

	GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_13;				//GPB12
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;			//开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  		//输出速率50Mhz
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_14;				//GPB13
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;			//开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//输出速率50Mhz
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
}

void I2C_Initial2(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE); 		//有时间测试下，不开I2C1的时钟可以吗？
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);		//GPB时钟

	GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_15;				//GPB12
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;			//开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  		//输出速率50Mhz
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);		//GPB时钟
	GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_6;				//GPB13
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;			//开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//输出速率50Mhz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}

//|---------------------------------------------------|
//|Function		:I2C_SDA_OUT
//|Description	:set GPIOB13 as output,sda_out
//|Input		:None
//|Output		:None
//|Return		:None
//|---------------------------------------------------|
void I2C_SDA_OUT1(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14 ;				//引脚GPB13
	GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_Out_OD;		  	//开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	//最高输出速率50MHz
	GPIO_Init(GPIOD, &GPIO_InitStructure);				 	    //选择C端口
}

void I2C_SDA_OUT2(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15 ;				//引脚GPB13
	GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_Out_OD;		  	//开漏输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 	//最高输出速率50MHz
	GPIO_Init(GPIOD, &GPIO_InitStructure);				 	    //选择C端口
}

//|---------------------------------------------------|
//|Function		:I2C_SDA_IN
//|Description	:set GPIOB13 as input,sda_in
//|Input		:None
//|Output		:None
//|Return		:None
//|---------------------------------------------------|
void I2C_SDA_IN1(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14;					//引脚GPB13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  		//浮空输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);				     	//选择C端口
}

void I2C_SDA_IN2(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;					//引脚GPB13
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  		//浮空输入
	GPIO_Init(GPIOD, &GPIO_InitStructure);				     	//选择C端口
}

/*******************************************************************************
* Function Name  : I2C_delay
* Description    : Simulation IIC Timing series delay
* Input          : None
* Output         : None
* Return         : None
********************************************************************************/
void I2C_delay1(void)
{	
	/*u8 i=30; //这里可以优化速度	，经测试最低到5还能写入
	while(i) 
	{ 
	i--; 
	} */
	delay_us(2);
}

void delay5ms1(void)
{	
	/*u16 i=5000;  
	while(i) 
	{ 
	i--; 
	}*/
	delay_ms(5);
}
/*******************************************************************************
* Function Name  : I2C_Start
* Description    : Master Start Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : Wheather	 Start
****************************************************************************** */
u8 I2C_Start1(void)
{
	I2C_SDA_OUT1();
	SDA_H1;
	SCL_H1;
	I2C_delay1();
	if(!SDA_read1)return 0;	//SDA线为低电平则总线忙,退出
	SDA_L1;
	I2C_delay1();
	if(SDA_read1) return 0;	//SDA线为高电平则总线出错,退出
	SDA_L1;
	I2C_delay1();
	SCL_L1;
	return 1;
}

u8 I2C_Start2(void)
{
	I2C_SDA_OUT2();
	SDA_H2;
	SCL_H2;
	I2C_delay1();
	if(!SDA_read2)return 0;	//SDA线为低电平则总线忙,退出
	SDA_L2;
	I2C_delay1();
	if(SDA_read2) return 0;	//SDA线为高电平则总线出错,退出
	SDA_L2;
	I2C_delay1();
	SCL_L2;
	return 1;
}
/*******************************************************************************
* Function Name  : I2C_Stop
* Description    : Master Stop Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Stop1(void)
{
	I2C_SDA_OUT1();
	SCL_L1;
	I2C_delay1();
	SDA_L1;
	I2C_delay1();
	SCL_H1;
	I2C_delay1();
	SDA_H1;
	I2C_delay1();
} 

void I2C_Stop2(void)
{
	I2C_SDA_OUT2();
	SCL_L2;
	I2C_delay1();
	SDA_L2;
	I2C_delay1();
	SCL_H2;
	I2C_delay1();
	SDA_H2;
	I2C_delay1();
}
/*******************************************************************************
* Function Name  : I2C_Ack
* Description    : Master Send Acknowledge I2C
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Ack1(void)
{	
	SCL_L1;
	I2C_delay1();
	SDA_L1;
	I2C_delay1();
	SCL_H1;
	I2C_delay1();
	SCL_L1;
	I2C_delay1();
}
void I2C_Ack2(void)
{	
	SCL_L2;
	I2C_delay1();
	SDA_L2;
	I2C_delay1();
	SCL_H2;
	I2C_delay1();
	SCL_L2;
	I2C_delay1();
}   

/*******************************************************************************
* Function Name  : I2C_NoAck
* Description    : Master Send No Acknowledge I2C
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_NoAck1(void)
{	
	SCL_L1;
	I2C_delay1();
	SDA_H1;
	I2C_delay1();
	SCL_H1;
	I2C_delay1();
	SCL_L1;
	I2C_delay1();
} 
void I2C_NoAck2(void)
{	
	SCL_L2;
	I2C_delay1();
	SDA_H2;
	I2C_delay1();
	SCL_H2;
	I2C_delay1();
	SCL_L2;
	I2C_delay1();
}
/*******************************************************************************
* Function Name  : I2C_WaitAck
* Description    : Master Reserive Slave Acknowledge I2C
* Input          : None
* Output         : None
* Return         : Wheather	 Reserive Slave Acknowledge I2C
****************************************************************************** */
u8 I2C_WaitAck1(void) 	 //返回为:=1有ACK,=0无ACK
{
	I2C_SDA_IN1();
	SCL_L1;
	I2C_delay1();
	SDA_H1;			
	I2C_delay1();
	SCL_H1;
	I2C_delay1();
	if(SDA_read1)
	{
      SCL_L1;
		  I2C_SDA_OUT1();
	    I2C_delay1();
      return 0;
	}
	SCL_L1;
	I2C_SDA_OUT1();
	I2C_delay1();
	return 1;
}
u8 I2C_WaitAck2(void) 	 //返回为:=1有ACK,=0无ACK
{
	I2C_SDA_IN2();
	SCL_L2;
	I2C_delay1();
	SDA_H2;			
	I2C_delay1();
	SCL_H2;
	I2C_delay1();
	if(SDA_read2)
	{
      SCL_L2;
		  I2C_SDA_OUT2();
	    I2C_delay1();
      return 0;
	}
	SCL_L2;
	I2C_SDA_OUT2();
	I2C_delay1();
	return 1;
}
/*******************************************************************************
* Function Name  : I2C_SendByte
* Description    : Master Send a Byte to Slave
* Input          : Will Send Date
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_SendByte1( u8 SendByte) //数据从高位到低位//
{
	u8 i=8;
	while(i--)
	{
		SCL_L1;
		I2C_delay1();
		if(SendByte&0x80)	SDA_H1;  
		else 				SDA_L1;   
		SendByte<<=1;
		I2C_delay1();
		SCL_H1;
		I2C_delay1();
	}
	SCL_L1;
}  
void I2C_SendByte2( u8 SendByte) //数据从高位到低位//
{
	u8 i=8;
	while(i--)
	{
		SCL_L2;
		I2C_delay1();
		if(SendByte&0x80)	SDA_H2;  
		else 				SDA_L2;   
		SendByte<<=1;
		I2C_delay1();
		SCL_H2;
		I2C_delay1();
	}
	SCL_L2;
}
/*******************************************************************************
* Function Name  : I2C_ReadByte
* Description    : Master Reserive a Byte From Slave
* Input          : None
* Output         : None
* Return         : Date From Slave 
****************************************************************************** */
u8 I2C_ReadByte1(void)  //数据从高位到低位//
{ 
    u8 i=8;
    u8 ReceiveByte=0;
    I2C_SDA_IN1();	
	while(i--)
	{
		ReceiveByte<<=1;      
		SCL_L1;
		I2C_delay1();
		SCL_H1;
		I2C_delay1();	
		if(SDA_read1)
		{
			ReceiveByte|=0x01;
		}
	}
    SCL_L1;
    return ReceiveByte;
}
u8 I2C_ReadByte2(void)  //数据从高位到低位//
{ 
    u8 i=8;
    u8 ReceiveByte=0;
    I2C_SDA_IN2();	
	while(i--)
	{
		ReceiveByte<<=1;      
		SCL_L2;
		I2C_delay1();
		SCL_H2;
		I2C_delay1();	
		if(SDA_read2)
		{
			ReceiveByte|=0x01;
		}
	}
    SCL_L2;
    return ReceiveByte;
}
//|---------------------------------------------------|
//|Function		:I2C_Write
//|Description	:Write data to SlaveDeive's register
//|Input		:
//|					SlaveAddress:Slave Device's Address
//|					REG_Address :Slave Device's Register Address
//|					REG_data	:Data to be wrote into register
//|Output		:None
//|Return		:u8 data of slave device's register
//|---------------------------------------------------|
u8 I2C_Write1(u8 SlaveAddress, u8 REG_Address, u8 REG_data)		     //void
{
	if(!I2C_Start1())return 0;
	I2C_SendByte1(SlaveAddress);   //发送设备地址+写信号//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//设置高起始地址+器件地址 
	I2C_WaitAck1();	
	I2C_SendByte1(REG_Address);    //设置低起始地址      
	I2C_WaitAck1();	
	I2C_SendByte1(REG_data);
	I2C_WaitAck1();   
	I2C_Stop1(); 
//	delay5ms1();
	return 1;
}
u8 I2C_Write2(u8 SlaveAddress, u8 REG_Address, u8 REG_data)		     //void
{
	if(!I2C_Start2())return 0;
	I2C_SendByte2(SlaveAddress);   //发送设备地址+写信号//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//设置高起始地址+器件地址 
	I2C_WaitAck2();	
	I2C_SendByte2(REG_Address);    //设置低起始地址      
	I2C_WaitAck2();	
	I2C_SendByte2(REG_data);
	I2C_WaitAck2();   
	I2C_Stop2(); 
//	delay5ms1();
	return 1;
}
//|---------------------------------------------------------------------|
//|Function		:I2C_Read
//|Description	:Read I2C_Device's Register
//|Input		:
//|					SlaveAddress:Slave Device's Address
//|					REG_Address :Slave Device's Register Address
//|Output		:None
//|Return		:u8  value of Register
//|---------------------------------------------------------------------|
u8 I2C_Read1(u8 SlaveAddress,u8 REG_Address)
{   
	u8 REG_data;     	
	I2C_Start1();
	I2C_SendByte1(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//设置高起始地址+器件地址 
	I2C_WaitAck1();
	I2C_SendByte1((u8) REG_Address);   //设置低起始地址      
	I2C_WaitAck1();
	I2C_Start1();
	I2C_SendByte1(SlaveAddress+1);
	I2C_WaitAck1();
	
	REG_data= I2C_ReadByte1();
	I2C_NoAck1();
	I2C_Stop1();
	return REG_data;
}
u8 I2C_Read2(u8 SlaveAddress,u8 REG_Address)
{   
	u8 REG_data;     	
	I2C_Start2();
	I2C_SendByte2(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//设置高起始地址+器件地址 
	I2C_WaitAck2();
	I2C_SendByte2((u8) REG_Address);   //设置低起始地址      
	I2C_WaitAck2();
	I2C_Start2();
	I2C_SendByte2(SlaveAddress+1);
	I2C_WaitAck2();
	
	REG_data= I2C_ReadByte2();
	I2C_NoAck2();
	I2C_Stop2();
	return REG_data;
}

//void I2C_ReadRegister(u8 startaddr, u8 *regbuf, u8 num)
//{
//	u8 i=0;
//	for(i=0;i<num;i++,regbuf++,startaddr++)
//	{
//		*regbuf = I2C_Read(0x26,startaddr);
//	}
//}
