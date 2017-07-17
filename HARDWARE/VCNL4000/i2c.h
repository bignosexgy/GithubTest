#include "stm32f10x.h"
#include "delay.h"

u8 I2C_Start1(void);
void I2C_Ack1(void);
void delay5ms1(void);
void I2C_Stop1(void);
void I2C_delay1(void);
void I2C_NoAck1(void);
u8 I2C_WaitAck1(void);
u8 I2C_ReadByte1(void);
void I2C_SDA_IN1(void);
void I2C_SDA_OUT1(void);
void I2C_Initial1(void);
void I2C_SendByte1(u8 SendByte);
//void I2C_ReadRegister(u8 startaddr, u8 *regbuf, u8 num);
u8 I2C_Read1(unsigned char SlaveAddress,unsigned char REG_Address);
u8 I2C_Write1(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);

u8 I2C_Start2(void);
void I2C_Ack2(void);
void delay5ms2(void);
void I2C_Stop2(void);
void I2C_delay2(void);
void I2C_NoAck2(void);
u8 I2C_WaitAck2(void);
u8 I2C_ReadByte2(void);
void I2C_SDA_IN2(void);
void I2C_SDA_OUT2(void);
void I2C_Initial2(void);
void I2C_SendByte2(u8 SendByte);
//void I2C_ReadRegister(u8 startaddr, u8 *regbuf, u8 num);
u8 I2C_Read2(unsigned char SlaveAddress,unsigned char REG_Address);
u8 I2C_Write2(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);

