//#include <reg52.h>
//#include <intrins.h>
//#include <stdio.h>
//#include <float.h>
#include "stm32f10x.h"
//#include "util.h"
#include <stdio.h>
#include "delay.h"
#include "TVOC.h"
#include "sys.h"  

#define uchar unsigned char
//typedef unsigned char byte;
//typedef unsigned char u08;
//typedef unsigned int  word;
//byte count;
//word tcount;
//word Press_Adc;


u8 count;
u16 tcount;
u16 Press_Adc;

/* 宏定义 */
//typedef unsigned char	u8;
//typedef unsigned int	u16;
//typedef unsigned long	ulong;

#define I2C_OK		0
#define I2C_ERROR	1
#define I2C_ACK		0
#define I2C_NAK		1
/* 晶振22.1184M, 1T = (1/22.1184)us */
#define DELAY_1us	23
//sbit SDA = P1^7;	/* 模拟I2C数据传送位 */
//sbit SCL = P1^6;	/* 模拟I2C时钟控制位 */

#define SCL PBout(6)	// PB6
#define SDA PBout(7)	// PB7
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<28;}	 //输入上拉/下拉
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=3<<28;}	 //推挽输出


//bit Read_flag,SendFlag;
u8 Read_flag,SendFlag;

//void Write_hd7279(byte,byte);	//write to hd7279
//unsigned char Read_hd7279(byte);  //read from hd7279
//void Send_Byte(byte); //send one byte
//byte Receive_Byte(void);//recieve one byte
//void Delay(word dat);
//byte Recv_Uart(void);

u8 Receive_Byte(void);//recieve one byte
u8 Recv_Uart(void);

//void Send_Byte(byte dat);				
//void Send_Uart(byte dat);

void Send_Byte(u8 dat);				
void Send_Uart(u8 dat);


//void Write_hd7279(byte cmd,byte dat);

//sbit Hd7279_KEY=P2^4;
//sbit Hd7279_DATA=P2^5;
//sbit Hd7279_CLK=P2^6;
//sbit Hd7279_CS=P2^7;
//HD7279 CODE


#define CMD_RESET 0xa4
#define CMD_TEST  0xbf
#define DECODE0	  0x80
#define DECODE1	  0xc8
#define CMD_READ  0x15
#define UNDECODE  0x90
#define RTL_CYCLE 0xa3
#define RTR_CYCLE 0xa2
#define RTL_UNCYC 0xa1
#define RTR_UNCYC 0xa0
#define ACTCTL    0x98
#define SEGON	  0xe0
#define SEGOFF	  0xc0
#define BLINKCTL  0x88

/**/
void IAQ_Init(void)
{ 
        GPIO_InitTypeDef GPIO_InitStructure;
	 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //使能PB端口时钟
        //配置I2C1_SCL(PB6)和I2C1_SDA(PB7)开漏输出
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
        GPIO_Init(GPIOB, &GPIO_InitStructure);	 
}
///**/
/**
void IAQ_Init(void)
{					     
 	//RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟 
	RCC->APB2ENR|=1<<3;//先使能外设IO PORTB时钟 							 
	GPIOB->CRL&=0X00FFFFFF;//PB6/PB7 推挽输出
	GPIOB->CRL|=0X33000000;	   
	GPIOB->ODR|=3<<6;     //PB6,7 输出高
}
**/
/**
 void Delay_us(unsigned char d)
{
	unsigned char i,cnt;

	//cnt = d * DELAY_1us;
	cnt = d * 23;
	for(i=0; i<cnt; i++)
	{
		_nop_();
	}

	//return;
}

void Delay(word dat)
{
     while (dat--)
      {
       _nop_();
    }
}
**/
void I2C_Start(void)
{
	/* SDA,SCL都为高, 准备发送起始条件 */
	SDA_OUT();     //sda线输出
	SDA = 1;
	delay_us(2);
	SCL = 1;
	delay_us(10);
    
	/* 发送起始条件: SCL高, SDA高->低 */
	SDA = 0;
	delay_us(10);
	SCL = 0;
	delay_us(4);

	return;
}

void I2C_Stop(void)
{
	/* 结束条件: SCL高, SDA低->高 */  
	SDA_OUT();     //sda线输出
	SDA = 0;
	delay_us(2);     
	SCL = 1;
	delay_us(10);
	SDA = 1;
	delay_us(8);

	return;
}

void I2C_Ack(u8 a)
{
	/* 发送应答或非应答信号 */
	SDA_OUT();     //sda线输出
	if(a == 0)	
	{
		SDA = 0;	/* 应答 */        
	}
	else 
	{
		SDA = 1;	/* 非应答 */
	}
	delay_us(6);      
	SCL = 1;
	delay_us(10);
	SCL = 0;
	delay_us(4);    

	
	return;
}

/****************************************************************
* Prototype      : u_char I2C_WriteByte()
* Parameters     : u_char c, 待发送的字符.
* Return Value   : none
* Global Variable: none
* Description    : 发送一个字节的数据.
*****************************************************************/ 
u8 I2C_WriteByte(u8 c)
{
	u8 i,ack;
	SDA_OUT();     //sda线输出
	for(i=0; i<8; i++)	
	{
		if((c << i) & 0x80)		
		{
			SDA = 1;    
		}
        else   
		{
			SDA = 0;                
		}
		delay_us(10);
		SCL = 1;
		delay_us(10);
		SCL = 0; 
	}
    
	delay_us(4);
	SDA = 1;
	delay_us(6);   
	SCL = 1;
	delay_us(10);
	SDA_IN();      //SDA设置为输入 
	if(SDA == 1)
	{
		ack = I2C_NAK;     
	}
	else 
	{
		ack = I2C_ACK;		
	}
	
	SCL = 0;
	delay_us(4);

	return ack;
}

/****************************************************************
* Prototype      : u_char I2C_ReadByte()
* Parameters     : none
* Return Value   : u_char, 接收到的字符.
* Global Variable: none
* Description    : 接收一个字节的数据.
*****************************************************************/ 

u8 I2C_ReadByte(void)
{
	u8 i,retc=0;
	
	/* 置数据线为输入方式 */
	SDA_OUT();     //sda线输出
	SDA = 1;	
	SDA_IN();      //SDA设置为输入 			  
	for(i=0; i<8; i++)
	{
		SCL = 0;
		delay_us(10);
		SCL = 1;
		delay_us(10);
		retc = retc << 1;
		if(SDA == 1) 
		{
			retc++;
		}
	}

	SCL = 0;    
	delay_us(10);

	return retc;
}

void IAQ(u8 *s,u8 length)
{
	u8 ack,i;
    I2C_Start();
    ack = I2C_WriteByte(0xb5);
	/**/
	for(i = 0; i < (length - 1); i++)
    {   
		*s = I2C_ReadByte();
		I2C_Ack(0);
		s++;
    }
	/**/
	*s = I2C_ReadByte();
    I2C_Ack(1);	
	I2C_Stop();	
}
//ulong Get_TVOC()        //TVOC
u32 Get_TVOC()        //TVOC
{
	u8 buf[7];
	//ulong TVOC;
	u32 TVOC;

	IAQ(buf,14);
	//{
		//TVOC = ((buf[2] << 24) + (buf[3] << 16) + (buf[4] << 8) +  buf[5]);	
	//}
	//return TVOC;
	return 0;
}
/**
void Timer_Init(void)
{
      EA   = 0;
     TMOD = 0x11;
     TCON = 0x55;
     TH0  = 0xfc;
     TL0  = 0x18; 
     TR0=1; 
    //IP=0x10;
    IE   = 0x9a;
}
**/
/*******************************************************************
函数原型: 
功能:       
返回值：      
********************************************************************/
/**
 void Rs232_Init(void)
{
     ES = 0;
     SCON = 0x50;
     T2CON = 0x34;
     RCAP2H = 0xff;//11.0592//
    RCAP2L = 0xdb;//11.0592//9600
  
    ES = 1;
}
**/
/**
void Send_Uart(byte dat)
{
    TI = 0;
    SBUF = dat;
    while(!TI);
    TI = 0;
}
**/
/*******************************************************************
函数原型: 
功能:       
返回值：      
********************************************************************/
/**
void Send_Byte(byte dat)
{
     byte i;
     Hd7279_CS=0;
     Delay(48);//long delay
     for(i=0;i<8;i++)
   {
     if(dat&0x80)
	{
	  Hd7279_DATA=1;
     }
	else
	{
	  Hd7279_DATA=0;
	}
	Hd7279_CLK=1;
	Delay(8);//short delay
	Hd7279_CLK=0;
	Delay(8);//short delay
 	dat=dat*2;
     }
       Hd7279_DATA=0;  //output
}  

void Write_hd7279(byte cmd,byte dat)
{
    Send_Byte(cmd);
    Send_Byte(dat);
}
**/
/*读取空气质量TVOC的显示程序*/
/**
void Display1(void)
{   unsigned long int a; 
    a=Get_TVOC();  //下面有小数点处理才乘以10，为了不丢失个位数，不显示小数点就不用乘以10
    a=a/100000;     //显示高位
    Write_hd7279(DECODE0+1,	a/10000);
    Write_hd7279(DECODE0+2,(a%10000)/1000);
    Write_hd7279(DECODE0+3,(a %1000)/100);
    // Write_hd7279(DECODE0+4,0x80|((a %100)/10))//显示小数点
    Write_hd7279(DECODE0+4,(a %100)/10);
    Write_hd7279(DECODE0+5,	a %10);
  }
void Display2(void)
{   unsigned long int a; 
    a=Get_TVOC();  //下面有小数点处理才乘以10，为了不丢失个位数，不显示小数点就不用乘以10
    a=a%100000;     //显示低位
    Write_hd7279(DECODE0+1,	a/10000);
    Write_hd7279(DECODE0+2,(a%10000)/1000);
    Write_hd7279(DECODE0+3,(a %1000)/100);
   // Write_hd7279(DECODE0+4,0x80|((a %100)/10))//显示小数点
    Write_hd7279(DECODE0+4,(a %100)/10);
    Write_hd7279(DECODE0+5,	a %10);
  }
 **/
 /**
void main(void)
{
    //P3=0xff;
    //P1=0xff;
    //P2=0xf0;
    //Rs232_Init();
    //Delay(20);
    //Timer_Init();
	IAQ_Init();
    while(1)
   {
     //Display1();    //显示高位
     //Delay(2000);
     //D/elay(2000);
     //Delay(2000);
     //Delay(2000);
     //Delay(2000);
     //Delay(2000);
     ///Delay(2000);
     //Delay(2000);
     //Delay(2000);
     //Display2();   //显示低位
     //Delay(2000);
     //Delay(2000);
     //Delay(2000);
     //Delay(2000);
     //Delay(2000);
     //Delay(2000);
     //Delay(2000);
     //Delay(2000);
     //Delay(2000);
     //Delay(2000);
  }
}
**/































