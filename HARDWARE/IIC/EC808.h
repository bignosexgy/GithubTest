#ifndef __EC808_H
#define __EC808_H

#include "sys.h"

#define lib_flag 0

#if (lib_flag == 0)
//模拟IIC方式	  
void IAQ_Init(void);
static u8 EC808_IIC1_Read(u8 *pBuffer, u8 NumByteToRead);
u8 IAQ_Read(u8 *pBuffer, u8 NumByteToRead);
extern void delay_us(u32 nus);
u16 data_lvbo( u16 data1,u16* data_in,u16* data_out);
#endif 

//#if (lib_flag == 1)
////硬件IIC方式 
//void IAQ_IIC1_Init(void);
//void IAQ_IIC1_Read(u8 *pBuffer, u8 NumByteToRead);
//void EC808_Process(void);
//extern void delay_us(u32 nus);
//#endif 

#endif
