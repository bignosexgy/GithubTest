#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"  
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Defines for the GPIO pins used for the I2C communication */
#define I2C_MEMS            I2C1
#define I2C_MEMS_CLK        RCC_APB1Periph_I2C1
#define I2C_MEMS_GPIO       GPIOB
#define I2C_MEMS_GPIO_CLK   RCC_APB2Periph_GPIOB
#define I2C_MEMS_SCL        GPIO_Pin_6
#define I2C_MEMS_SDA        GPIO_Pin_7


#define CSON CS_GPIO_PORT->BSRR = CS_GPIO_PIN
#define CSOFF CS_GPIO_PORT->BRR = CS_GPIO_PIN
#define CSTOGGLE CS_GPIO_PORT->ODR ^= CS_GPIO_PIN

#define SA0ON SA0_GPIO_PORT->BSRR = SA0_GPIO_PIN
#define SA0OFF SA0_GPIO_PORT->BRR = SA0_GPIO_PIN
#define SA0TOGGLE SA0_GPIO_PORT->ODR ^= SA0_GPIO_PIN

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void I2C_MEMS_Init(void);
void I2C_MEMS_DeInit(void);
//void I2C_BufferRead(uint8_t* pBuffer, uint8_t deviceAddr, uint8_t readAddr, uint16_t numByteToRead);
uint8_t I2C_BufferRead(uint8_t* pBuffer, uint8_t deviceAddr, uint8_t readAddr, uint16_t numByteToRead);
void I2C_ByteWrite(uint8_t* pBuffer, uint8_t deviceAddress, uint8_t WriteAddr);
uint8_t I2C_Master_BufferRead1Byte(uint8_t deviceAddr, uint8_t readAddr);	


   	   		   
//IO方向设置
#define SDA_IN()  {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}		  //11
#define SDA_OUT() {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}

#define IIC1_SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<28;}	 //输入上拉/下拉
#define IIC1_SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=3<<28;}	 //推挽输出

//IO操作函数	 
#define IIC_SCL    PCout(12) //SCL
#define IIC_SDA    PCout(11) //SDA	 
#define READ_SDA   PCin(11)  //输入SDA 

#define IIC1_SCL    PBout(6) //SCL
#define IIC1_SDA    PBout(7) //SDA	 
#define IIC1_READ_SDA   PBin(7)  //输入SDA 

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	  


//IIC1所有操作函数
void IIC1_Init(void);                //初始化IIC的IO口				 
void IIC1_Start(void);				//发送IIC开始信号
void IIC1_Stop(void);	  			//发送IIC停止信号
void IIC1_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC1_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC1_Wait_Ack(void); 				//IIC等待ACK信号
void IIC1_Ack(void);					//IIC发送ACK信号
void IIC1_NAck(void);				//IIC不发送ACK信号

void IIC1_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC1_Read_One_Byte(u8 daddr,u8 addr);	 
#endif
















