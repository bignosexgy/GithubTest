#include "ec808.h"
#include "sht21.h"
#include <stdio.h>

#if (lib_flag == 0)
/**************************************************************************************
*******************************模拟IIC处理方式*****************************************
①测试OK，有误码，输出格式为 ff ** ** ** ** ** ** **读取的第一位为无效位
②隔一段时间会读取到一次无效值，ff ** ff ff ff ff ff ff，继而继续读取到有效值
**************************************************************************************/
/* Private macro -------------------------------------------------------------*/
#define SCLL                                GPIOA->BRR = GPIO_Pin_6
#define SCLH                                GPIOA->BSRR = GPIO_Pin_6                                             
#define SDAL                                GPIOA->BRR = GPIO_Pin_7
#define SDAH                                GPIOA->BSRR = GPIO_Pin_7

#define SDA_STATE                        GPIOA->IDR&GPIO_Pin_7
enum I2C_REPLY {I2C_NACK = 0, I2C_ACK = 1};
enum I2C_STATE {I2C_READY = 0, I2C_BUSY = 1, I2C_ERROR = 2};

#define IAQ_READ_ADDRESS	0xB5
#define EC808_READ_ADDRESS	0x10
#define EC805_READ_ADDRESS	0x12

u8 Co_Buf[15],Tvoc_Buf[15],Tvoc_Status;
/* Private functions ---------------------------------------------------------*/
static u8 I2C_Start(void);
static void I2C_Stop(void);
static void I2C_Ack(u8);
static u8 I2C_Wait_Ack(void);
static void I2C_SendByte(u8);
static u8 I2C_ReceiveByte(void);
//static void I2C_Delay_us(u16 n);
extern u8 voc_ok;
extern u8 co_ok;
 extern 	float VOC_Data;
extern  u16 CO_Data;
extern	int16_t VOC_Data_Vs ;
extern	int16_t CO_Data_Vs ;
u16 CO_Data_Vo;
u16 CO_Data_temp[5];
u16 hcho_Data_temp[5];
u8 senser_voc_flag=0;
u8 senser_co_flag=0;
float VOC_Data_Vo;

/*******************************************************************************/
void IAQ_Init(void)
{ 
//	GPIO_InitTypeDef GPIO_InitStructure;
//	//配置I2C1_SCL(PB6)和I2C1_SDA(PB7)开漏输出
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_OD;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);


	GPIO_InitTypeDef GPIO_InitStructure;  
//	I2C_InitTypeDef I2C_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);		
	// Configure IO connected to IIC//////////////////////
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	
GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_OD;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);

//	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
//	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
//	I2C_InitStructure.I2C_OwnAddress1 = 0x0F;
//	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
//	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//	I2C_InitStructure.I2C_ClockSpeed = 50000;

//	I2C_Cmd(I2C1, ENABLE);   
//	I2C_Init(I2C1, &I2C_InitStructure);	   

}
/*******************************************************************************/

//static void I2C_Delay_us(u16 nTime)
//{
//	u16 i = 0;
//	
//	while(nTime--)
//	{
//		i = 10;
//		while(i--);
//	}
//}

static u8 I2C_Start(void)
{
        SDAH;
        SCLH;
		delay_us(5);
        if(!SDA_STATE) {
    //            printf("I2C is busy!\r\n");
                return I2C_BUSY;         
        }
        SDAL;
			delay_us(5);
        SCLL;
//			delay_us(10);
        if(SDA_STATE) {
      //          printf("I2C is error!\r\n");
                return I2C_ERROR;
        }
        return I2C_READY;
}
/*******************************************************************************/
static void I2C_Stop(void)
{
        SDAL;
		delay_us(5); 
        SCLH;
		delay_us(5); 
        SDAH;
		delay_us(5); 
}
/*******************************************************************************/
static void I2C_Ack(u8 i)
{
        if(i)
                SDAL;                                //ACK
        else                                        
                SDAH;                                //NACK
		delay_us(2); 
        SCLH;
		delay_us(7); 
        SCLL;
		delay_us(2); 
}
/*******************************************************************************/
static u8 I2C_Wait_Ack(void)
{
        u8 ucErrTime = 0;
        SDAH;
		delay_us(2); 
        SCLH;
		delay_us(7); 
        while(SDA_STATE)
        {
                ucErrTime++;
                if(ucErrTime > 250)
                {
                        I2C_Stop();
           //             printf("I2C NACK!\r\n");
                        return I2C_NACK;
                }
        }
        SCLL;
//		delay_us(2); 
        return I2C_ACK;
}
/*******************************************************************************/
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
		  delay_us(7); 	
          SCLH;
		  delay_us(7); 
          SCLL;
//		  delay_us(7); 
        }
}
/*******************************************************************************/
static u8 I2C_ReceiveByte(void)
{
        u8 i, d=0;
        SDAH;
        SCLL;
//		delay_us(2); 
        for(i=0x80;i!=0;i/=2)
        {
                SCLH;
				delay_us(7); 
                if(SDA_STATE)
                	d |= i;
                SCLL;
				delay_us(7); 
        }
        return d;
}
/*******************************************************************************/
u8 IAQ_Read(u8 *pBuffer, u8 NumByteToRead)
{ 
        if(I2C_Start()) {
      //          printf("Start error!\r\n");
                return 1;
        }
		delay_us(20);   	
        I2C_SendByte(0xB5);
		delay_us(20); 
        if(!I2C_Wait_Ack()) {
       //         printf("No Ack after send address!\r\n");
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

static u8 EC808_IIC1_Read(u8 *pBuffer, u8 NumByteToRead)
{ 
        if(I2C_Start()) 
		{
    //        printf("Start error!\r\n");
      //     return 1;
        }
		delay_us(5);   	
      I2C_SendByte(EC808_READ_ADDRESS | 0x01);
//  		   I2C_SendByte( 0x51);

		delay_us(25); 
        if(!I2C_Wait_Ack()) 
		{
      //      printf("No Ack after send address!\r\n");
	       
		   senser_voc_flag=7;  
           return 2;
        }	     
        while(NumByteToRead--) 
		{
	//		delay_us(5); 	
            *pBuffer++ = I2C_ReceiveByte();
			delay_us(9); 
            if(NumByteToRead)
                I2C_Ack(1);
            else
                I2C_Ack(0);        
        }
		I2C_Stop();
       return 0;
}

void EC808_Process(void)
{
//	u16 Prediction;	
	u32 Tvov_Temp;



	EC808_IIC1_Read(&Tvoc_Buf[0], 7);
	 if((Tvoc_Buf[0]==0x0B)&&(Tvoc_Buf[5]==0x0B)&&(Tvoc_Buf[4]>0x00))
	     {
		  Tvov_Temp = (Tvoc_Buf[3] << 8)+Tvoc_Buf[4];

			if(ABS(Tvov_Temp,VOC_Data_Vo) <= 100)
			   	VOC_Data_Vo=(Tvov_Temp+VOC_Data_Vo)/2;

				VOC_Data_Vo=  data_lvbo(VOC_Data_Vo,&hcho_Data_temp[0],&hcho_Data_temp[0]);

			if((VOC_Data_Vo+200)>VOC_Data_Vs)
	          VOC_Data = (float)((VOC_Data_Vo+200-VOC_Data_Vs)/100);
		    else  VOC_Data=0;
/*
		 Tvov_Temp = (Tvoc_Buf[3] << 8)+Tvoc_Buf[4];
//		  if(Tvov_Temp>200)
//			Tvov_Temp=200;
//			 if(Tvov_Temp<=0)
//			Tvov_Temp=0;
			VOC_Data_Vo=Tvov_Temp;

			if((VOC_Data_Vo+VOC_Data*100+200)>VOC_Data_Vs)
	          VOC_Data = (float)((VOC_Data_Vo+VOC_Data*100+200-VOC_Data_Vs)/200);
		   else	if((2*VOC_Data_Vo+200)>VOC_Data_Vs)
		        VOC_Data = (float)((2*VOC_Data_Vo+200-VOC_Data_Vs)/200);  
             else  VOC_Data=0;
*/
		 if(VOC_Data>1000)
			   VOC_Data=1000;
		if(VOC_Data<=0)
			   VOC_Data=0;
		 voc_ok=1;
		 senser_voc_flag=0; 
		 }
	 else if((Tvoc_Buf[1]==0x0B)&&(Tvoc_Buf[6]==0x0B)&&(Tvoc_Buf[5]>0x00))
	       { 
		   Tvov_Temp = (Tvoc_Buf[4] << 8)+Tvoc_Buf[5];

		//	VOC_Data_Vo=Tvov_Temp;

//			if((VOC_Data_Vo+VOC_Data*100+200)>VOC_Data_Vs)
//		 VOC_Data = (float)((VOC_Data_Vo+VOC_Data*100+200-VOC_Data_Vs)/200);
//			else
//			  VOC_Data=0;

//			if((ABS(Tvov_Temp,VOC_Data_Vo) <= 2)&&(Tvov_Temp==0))
//			   VOC_Data_Vo=Tvov_Temp;
			 if(ABS(Tvov_Temp,VOC_Data_Vo) <= 100)
			VOC_Data_Vo=(Tvov_Temp+VOC_Data_Vo)/2;
			 	VOC_Data_Vo=  data_lvbo(VOC_Data_Vo,&hcho_Data_temp[0],&hcho_Data_temp[0]);
			if((VOC_Data_Vo+200)>VOC_Data_Vs)
	          VOC_Data = (float)((VOC_Data_Vo+200-VOC_Data_Vs)/100);
		    else  VOC_Data=0;

			 if(VOC_Data>1000)
			   VOC_Data=1000;
			 if(VOC_Data<=0)
			   VOC_Data=0;

			senser_voc_flag=0; 
		    voc_ok=1;
		}
		else if(((Tvoc_Buf[0]==0x0B)&&(Tvoc_Buf[5]==0x0B))||((Tvoc_Buf[1]==0x0B)&&(Tvoc_Buf[6]==0x0B)))
		    senser_voc_flag=0;
		  else 
		    senser_voc_flag=7; 
// printf("{id:2,voc:%0.2f}\r",
//			  VOC_Data );		 
//	Tvov_Temp =	(u32)((Tvoc_Buf[2] << 24)+(Tvoc_Buf[3] << 16)+(Tvoc_Buf[4] << 8)+Tvoc_Buf[5]);
//	Tvoc_Status = Tvoc_Buf[10];
//	Sensor_Resistance = (Tvoc_Buf[11] << 24)+(Tvoc_Buf[12] << 16)+(Tvoc_Buf[13] << 8)+Tvoc_Buf[14];
//	
//	Tvoc_Value_Temp[4] = Tvoc_Value_Temp[3];
//	Tvoc_Value_Temp[3] = Tvoc_Value_Temp[2];
//	Tvoc_Value_Temp[2] = Tvoc_Value_Temp[1];
//	Tvoc_Value_Temp[1] = Tvoc_Value_Temp[0];
//	Tvoc_Value_Temp[0] = Tvov_Temp;
//	Tvoc_Value=(Tvoc_Value_Temp[4] + Tvoc_Value_Temp[3] + Tvoc_Value_Temp[2] + Tvoc_Value_Temp[1] + Tvoc_Value_Temp[0])/5;
//	Tvoc_Value_Temp1 = (u8)(Tvoc_Value);
//	Tvoc_Value = (float)Tvoc_Value_Temp1/100;	 
}
static u8 EC805_IIC1_Read(u8 *pBuffer, u8 NumByteToRead)
{ 
//     u8 HCHO_i;
        
		if(I2C_Start()) 
		{
    //        printf("Start error!\r\n");
      //     return 1;
        }
		delay_us(5);   	
      I2C_SendByte(EC805_READ_ADDRESS | 0x01);
//  		   I2C_SendByte( 0x51);

		delay_us(25); 
        if(!I2C_Wait_Ack()) 
		{
      //      printf("No Ack after send address!\r\n");
	    
           senser_co_flag=7;
           return 2;
        }	     
        while(NumByteToRead--) 
		{
			delay_us(20); 	
            *pBuffer++ = I2C_ReceiveByte();
			delay_us(20); 
            if(NumByteToRead)
                I2C_Ack(1);
            else
                I2C_Ack(0);        
        }
		I2C_Stop();
       return 0;

 
    
   
   




}

void EC805_Process(void)
{
//	u16 Prediction;	
	u32 Co_Temp;
//	u32 Sensor_Resistance;
//	static float  Tvoc_Value_Temp[5];
//	u8 	Tvoc_Value_Temp1;
	

	EC805_IIC1_Read(&Co_Buf[0], 7);
	 if((Co_Buf[0]==0x0B)&&(Co_Buf[5]==0x0B))
	     {

		 Co_Temp= (Co_Buf[3] << 8)+Co_Buf[4];

		 if((ABS(Co_Temp,CO_Data_Vo) <= 2)&&(Co_Temp==0))
			CO_Data_Vo=Co_Temp;
		   if(ABS(Co_Temp,CO_Data_Vo) <= 200)

		   CO_Data_Vo =(Co_Temp+CO_Data_Vo)/2 ;
		   CO_Data_Vo=69*CO_Data_Vo/100;

		CO_Data_Vo=  data_lvbo(CO_Data_Vo,&CO_Data_temp[0],&CO_Data_temp[0]);

		 if((CO_Data_Vo+500) > CO_Data_Vs)
		   CO_Data=CO_Data_Vo+500-CO_Data_Vs;
		 else CO_Data=0;

         if(CO_Data <=0)
		 	   CO_Data =0;
		  if(CO_Data >1000)
		 	   CO_Data =1000;
		senser_co_flag=0; 
		 co_ok=1;
		 }
	 else if((Co_Buf[1]==0x0B)&&(Co_Buf[6]==0x0B))
	       { 
				Co_Temp= (Co_Buf[4] << 8)+Co_Buf[5];
				if((ABS(Co_Temp,CO_Data_Vo) <= 2)&&(Co_Temp==0))
			      CO_Data_Vo=Co_Temp;

				  if(ABS(Co_Temp,CO_Data_Vo) <= 200)
				   CO_Data_Vo =(Co_Temp+CO_Data_Vo)/2;

				   CO_Data_Vo=69*CO_Data_Vo/100;
				   	CO_Data_Vo=  data_lvbo(CO_Data_Vo,&CO_Data_temp[0],&CO_Data_temp[0]);
				 if((CO_Data_Vo+500) > CO_Data_Vs)
				   CO_Data=CO_Data_Vo+500-CO_Data_Vs;
				 else CO_Data=0;

//		     CO_Data_Vo = (Co_Buf[4] << 8)+Co_Buf[5];
		   	 
//		 if((CO_Data_Vo+CO_Data+500) > CO_Data_Vs)
//		   CO_Data=(CO_Data_Vo+CO_Data)/2+500-CO_Data_Vs;		 1
//			 else
//			   CO_Data =0;

//		 if(((CO_Data_Vo+CO_Data)/2+500) > CO_Data_Vs)			  2
//		   CO_Data=(CO_Data_Vo+CO_Data)/2+500-CO_Data_Vs;
//		  else	 if((CO_Data_Vo+500) > CO_Data_Vs)
//		    CO_Data=CO_Data_Vo+500-CO_Data_Vs;
//		  else CO_Data=0;

	         if(CO_Data <=0)
			 	   CO_Data =0;
			  if(CO_Data >1000)
			 	   CO_Data =1000;
		   
            senser_co_flag=0; 
		    co_ok=1;
		}
		else if((Co_Buf[0]==0x00)&&(Co_Buf[1]==0x1F))

		         senser_co_flag=0;
		    else 
	 			  senser_co_flag=7;
}


u16 data_lvbo( u16 data1,u16* data_in,u16* data_out)
{
	u8 i;
	u16 data_sum=0,data_ave=0;
 	 data_in[4]=data_in[3];	
	 data_in[3]=data_in[2];
	 data_in[2]=data_in[1];	
	 data_in[1]=data_in[0];
	 data_in[0]=data1;
	 for(i=0;i<5;i++)
	 {
	   data_out[i]=data_in[i];
	   data_sum=data_sum+data_in[i];
	 
	 }
	  data_ave =data_sum/5;
	 return data_ave;
}



#endif





//#if (lib_flag == 1)
///**************************************************************************************
//*******************************硬件IIC处理方式*****************************************
//************************************测试OK*********************************************
//**************************************************************************************/
//
//#define IAQ_READ_ADDRESS	0xB5
//#define EC808_READ_ADDRESS	0x10
//
//u8 Tvoc_Buf[15],Tvoc_Status;
//float Tvoc_Value;
//
////Init
///**
//void IAQ_IIC_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//
//	I2C_InitTypeDef I2C_InitStructure;
//
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
//	
//	// Configure IO connected to IIC//////////////////////
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//
//	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
//	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
//	I2C_InitStructure.I2C_OwnAddress1 = 0x0F;
//	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
//	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//	I2C_InitStructure.I2C_ClockSpeed = 200000;
//
//	I2C_Cmd(I2C2, ENABLE);   
//	I2C_Init(I2C2, &I2C_InitStructure);	   	
//}
//**/
//
///**/
//void IAQ_IIC1_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//
//	I2C_InitTypeDef I2C_InitStructure;
//
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
//	
//	// Configure IO connected to IIC//////////////////////
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 	
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	//GPIO_SetBits(GPIOB,GPIO_Pin_6);    //add by xgy
//
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//
//
//
//	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
//	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
//	I2C_InitStructure.I2C_OwnAddress1 = 0x0F;
//	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
//	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//	I2C_InitStructure.I2C_ClockSpeed = 50000;
//
//	I2C_Cmd(I2C1, ENABLE);   
//	I2C_Init(I2C1, &I2C_InitStructure);	   	
//}
///**/
//
//
//
///**
//void IAQ_IIC_Read(u8 *pBuffer, u8 NumByteToRead)
//{
//	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY)); // Added by Najoua 27/08/2008
// 
//  // Send START condition //
//  I2C_GenerateSTART(I2C2, ENABLE);
//	
//	// Test on EV5 and clear it //
//  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
//	
//	// Send IAQ Address for read //
//  I2C_Send7bitAddress(I2C2, IAQ_READ_ADDRESS, I2C_Direction_Receiver);
//	
//	// Test on EV6 and clear it //
//  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
//	
//	 // While there is data to be read //
//  while(NumByteToRead)  
//  {
//    if(NumByteToRead == 1)
//    {
//      // Disable Acknowledgement //
//      I2C_AcknowledgeConfig(I2C2, DISABLE);
//      
//      // Send STOP Condition //
//      I2C_GenerateSTOP(I2C2, ENABLE);
//    }
//
//    // Test on EV7 and clear it //
//    if(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))  
//    {      
//      // Read a byte from the EEPROM //
//      *pBuffer = I2C_ReceiveData(I2C2);
//
//      // Point to the next location where the byte read will be saved //
//      pBuffer++; 
//      
//      // Decrement the read bytes counter //
//      NumByteToRead--;        
//    }   
//  }
//
//  // Enable Acknowledgement to be ready for another reception //
//  I2C_AcknowledgeConfig(I2C2, ENABLE);
//	
//}
//**/
//
//
///**/
//void IAQ_IIC1_Read(u8 *pBuffer, u8 NumByteToRead)
//{
//	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); // Added by Najoua 27/08/2008
// 
//  // Send START condition //
//  I2C_GenerateSTART(I2C1, ENABLE);
//	
//	// Test on EV5 and clear it //
//  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
//	
//	// Send IAQ Address for read //
//  I2C_Send7bitAddress(I2C1, IAQ_READ_ADDRESS, I2C_Direction_Receiver);
//	
//	// Test on EV6 and clear it //
//  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
//	
//	 // While there is data to be read //
//  while(NumByteToRead)  
//  {
//    if(NumByteToRead == 1)
//    {
//      // Disable Acknowledgement //
//      I2C_AcknowledgeConfig(I2C1, DISABLE);
//      
//      // Send STOP Condition //
//      I2C_GenerateSTOP(I2C1, ENABLE);
//    }
//
//    // Test on EV7 and clear it //
//    if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  
//    {      
//      // Read a byte from the EEPROM //
//      *pBuffer = I2C_ReceiveData(I2C1);
//
//      // Point to the next location where the byte read will be saved //
//      pBuffer++; 
//      
//      // Decrement the read bytes counter //
//      NumByteToRead--;        
//    }   
//  }
//
//  // Enable Acknowledgement to be ready for another reception //
//  I2C_AcknowledgeConfig(I2C1, ENABLE);
//	
//}
///**/
//
//void Tvoc_Process(void)
//{
//	//u8 Tvoc_Buf[15],Tvoc_Status;
//	u16 Prediction;	
//	u32 Tvov_Temp;
//	u32 Sensor_Resistance;
//	static float  Tvoc_Value_Temp[5];
//	u8 	Tvoc_Value_Temp1;
//
//
//	IAQ_IIC1_Read(&Tvoc_Buf[0], 15);
//	Prediction = (Tvoc_Buf[0] << 8)+Tvoc_Buf[1];
//	Tvov_Temp =	(u32)((Tvoc_Buf[2] << 24)+(Tvoc_Buf[3] << 16)+(Tvoc_Buf[4] << 8)+Tvoc_Buf[5]);
//	Tvoc_Status = Tvoc_Buf[10];
//	Sensor_Resistance = (Tvoc_Buf[11] << 24)+(Tvoc_Buf[12] << 16)+(Tvoc_Buf[13] << 8)+Tvoc_Buf[14];
//	
//	Tvoc_Value_Temp[4] = Tvoc_Value_Temp[3];
//	Tvoc_Value_Temp[3] = Tvoc_Value_Temp[2];
//	Tvoc_Value_Temp[2] = Tvoc_Value_Temp[1];
//	Tvoc_Value_Temp[1] = Tvoc_Value_Temp[0];
//	Tvoc_Value_Temp[0] = (float)(1.79*Tvov_Temp/10);
//	Tvoc_Value=(Tvoc_Value_Temp[4] + Tvoc_Value_Temp[3] + Tvoc_Value_Temp[2] + Tvoc_Value_Temp[1] + Tvoc_Value_Temp[0])/5;
//	Tvoc_Value_Temp1 = (u8)(Tvoc_Value);
//	Tvoc_Value = (float)Tvoc_Value_Temp1/100;	 
//}
//
//void EC808_IIC1_Read(u8 *pBuffer, u8 NumByteToRead)
//{
//	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); // Added by Najoua 27/08/2008
// 
//  // Send START condition //
//  I2C_GenerateSTART(I2C1, ENABLE);
//	
//	// Test on EV5 and clear it //
//  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
//	
//	// Send IAQ Address for read //
//  I2C_Send7bitAddress(I2C1, EC808_READ_ADDRESS, I2C_Direction_Receiver);
//	
//	// Test on EV6 and clear it //
//  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
//	
//	 // While there is data to be read //
//  while(NumByteToRead)  
//  {
//    if(NumByteToRead == 1)
//    {
//      // Disable Acknowledgement //
//      I2C_AcknowledgeConfig(I2C1, DISABLE);
//      
//      // Send STOP Condition //
//      I2C_GenerateSTOP(I2C1, ENABLE);
//    }
//
//    // Test on EV7 and clear it //
//    if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))  
//    {      
//      // Read a byte from the EEPROM //
//      *pBuffer = I2C_ReceiveData(I2C1);
//
//      // Point to the next location where the byte read will be saved //
//      pBuffer++; 
//      
//      // Decrement the read bytes counter //
//      NumByteToRead--;        
//    }   
//  }
//
//  // Enable Acknowledgement to be ready for another reception //
//  I2C_AcknowledgeConfig(I2C1, ENABLE);
//	
//}
//void EC808_Process(void)
//{
//	u16 Prediction;	
//	u32 Tvov_Temp;
//	u32 Sensor_Resistance;
//	static float  Tvoc_Value_Temp[5];
//	u8 	Tvoc_Value_Temp1;
//
//
//	EC808_IIC1_Read(&Tvoc_Buf[0], 6);
////	Prediction = (Tvoc_Buf[0] << 8)+Tvoc_Buf[1];
////	Tvov_Temp =	(u32)((Tvoc_Buf[2] << 24)+(Tvoc_Buf[3] << 16)+(Tvoc_Buf[4] << 8)+Tvoc_Buf[5]);
////	Tvoc_Status = Tvoc_Buf[10];
////	Sensor_Resistance = (Tvoc_Buf[11] << 24)+(Tvoc_Buf[12] << 16)+(Tvoc_Buf[13] << 8)+Tvoc_Buf[14];
////	
////	Tvoc_Value_Temp[4] = Tvoc_Value_Temp[3];
////	Tvoc_Value_Temp[3] = Tvoc_Value_Temp[2];
////	Tvoc_Value_Temp[2] = Tvoc_Value_Temp[1];
////	Tvoc_Value_Temp[1] = Tvoc_Value_Temp[0];
////	Tvoc_Value_Temp[0] = (float)(1.79*Tvov_Temp/10);
////	Tvoc_Value=(Tvoc_Value_Temp[4] + Tvoc_Value_Temp[3] + Tvoc_Value_Temp[2] + Tvoc_Value_Temp[1] + Tvoc_Value_Temp[0])/5;
////	Tvoc_Value_Temp1 = (u8)(Tvoc_Value);
////	Tvoc_Value = (float)Tvoc_Value_Temp1/100;	 
//}
//
//#endif

