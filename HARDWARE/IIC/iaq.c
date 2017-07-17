#include "stm32f10x.h"
//#include "util.h"
#include <stdio.h>
#include "delay.h"
#include "iaq.h"
/* Private macro -------------------------------------------------------------*/
#define SCLL                                   GPIOB->BRR = GPIO_Pin_7
#define SCLH                                GPIOB->BSRR = GPIO_Pin_7                                             
#define SDAL                                GPIOB->BRR = GPIO_Pin_6
#define SDAH                                GPIOB->BSRR = GPIO_Pin_6

#define SDA_STATE                        GPIOB->IDR&GPIO_Pin_6

enum I2C_REPLY {I2C_NACK = 0, I2C_ACK = 1};

enum I2C_STATE {I2C_READY = 0, I2C_BUSY = 1, I2C_ERROR = 2};
/* Private functions ---------------------------------------------------------*/
static u8 I2C_Start(void);
static void I2C_Stop(void);
static void I2C_Ack(u8);
static u8 I2C_Wait_Ack(void);
static void I2C_SendByte(u8);
static u8 I2C_ReceiveByte(void);
/*******************************************************************************/
/**/
//void IAQ_Init(void)
//{ 
//        GPIO_InitTypeDef GPIO_InitStructure;
//
//        //配置I2C1_SCL(PB6)和I2C1_SDA(PB7)开漏输出
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
//        GPIO_Init(GPIOB, &GPIO_InitStructure);
//}
/**/
/*******************************************************************************/
/**/
static u8 I2C_Start(void)
{
        SDAH;
        SCLH;
        delay_us(5);
        if(!SDA_STATE) {
                printf("I2C is busy!\r\n");
                return I2C_BUSY;         
        }
        SDAL;
        delay_us(5);
        SCLL;
        delay_us(10);
        if(SDA_STATE) {
                printf("I2C is error!\r\n");
                return I2C_ERROR;
        }
        return I2C_READY;
}
/**/
/*******************************************************************************/
/**/
static void I2C_Stop(void)
{
        SDAL;
        delay_us(5);
        SCLH;
        delay_us(5);
        SDAH;
        delay_us(5);
}
/**/
/*******************************************************************************/
/**/
static void I2C_Ack(u8 i)
{
        if(i)
                SDAL;                                //ACK
        else                                        
                SDAH;                                //NACK
        delay_us(2);
        SCLH;
        delay_us(2);
        SCLL;
        delay_us(2); 
}
/**/
/*******************************************************************************/
/**/
static u8 I2C_Wait_Ack(void)
{
        u8 ucErrTime = 0;
        SDAH;
        delay_us(2);
        SCLH;
        delay_us(2);
        while(SDA_STATE)
        {
                ucErrTime++;
                if(ucErrTime > 250)
                {
                        I2C_Stop();
                        printf("I2C NACK!\r\n");
                        return I2C_NACK;
                }
        }
        SCLL;
        delay_us(2);
        return I2C_ACK;
}
/**/
/*******************************************************************************/
/**/
static void I2C_SendByte(u8 d)
{
        u8 i;

        SCLL;
        delay_us(2);
        for(i=0x80;i!=0;i/=2)
        {
                  if(i&d)
                           SDAH;
                  else
                           SDAL;
                delay_us(2);
                  SCLH;
                  delay_us(2);
                  SCLL;
                  delay_us(2);
        }
}
/**/
/*******************************************************************************/
/**/
static u8 I2C_ReceiveByte(void)
{
        u8 i, d=0;
        SDAH;
        SCLL;
        delay_us(2);
        for(i=0x80;i!=0;i/=2)
        {
                  SCLH;
                delay_us(2);
                if(SDA_STATE)
                           d |= i;
                 SCLL;
                  delay_us(2);
        }
        return d;
}
/**/
/*******************************************************************************/
/**/
u8 IAQ_Read(u8 *pBuffer, u8 NumByteToRead)
{
        
        if(I2C_Start()) {
			printf("Start error!\r\n");
            return 1;
        }
		I2C_SendByte(0xB5);		               
        delay_us(20);	           		
        if(!I2C_Wait_Ack()) {
                printf("No Ack after send address!\r\n");
                return 2;
        }
  
        while(NumByteToRead--) {
                        delay_us(20);
                        *pBuffer++ = I2C_ReceiveByte();
                        delay_us(20);
                        if(NumByteToRead)
                                I2C_Ack(1);
                        else
                                I2C_Ack(0);        
        }
        return 0;
}
/**/
/**
//void IAQ_Read(u8 *ReadBuf,u16 ReadAddr,u8 ReadLenth)   
void IAQ_Read(u8 *ReadBuf,u8 ReadLenth)   
{				  
	u8 temp[14]={0},ACK_temp=0,i;		  	    																 
    IIC1_Start();  	  
	IIC1_Send_Byte(HSHCAL_SLVADR & 0xB5);	   //发送写命令
	ACK_temp=IIC1_Wait_Ack(); 
	for(i=0;i<ReadLenth;i++)
	{
		delay_us(20);
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
**/

