
#include "stm32f10x.h"
#include <stdio.h>
#include "delay.h"
#include "sht21.h"
#include "sys.h"  
#include "math.h"
#include "ec808.h"
float Temp_Calib=0; //���ڼ�1.5

//#if (self_flag == 0) 
//#define uchar unsigned char	
//
//#define I2C_OK		0
//#define I2C_ERROR	1
//#define I2C_ACK		0
//#define I2C_NAK		1
///* ����22.1184M, 1T = (1/22.1184)us */
//#define DELAY_1us	23		  
//#define SCL PBout(6)	// PB6
//#define SDA PBout(7)	// PB7
//#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=8<<28;}	 //��������/����
//#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=3<<28;}	 //�������	 
//
//
//#define CMD_RESET 0xa4
//#define CMD_TEST  0xbf
//#define DECODE0	  0x80
//#define DECODE1	  0xc8
//#define CMD_READ  0x15
//#define UNDECODE  0x90
//#define RTL_CYCLE 0xa3
//#define RTR_CYCLE 0xa2
//#define RTL_UNCYC 0xa1
//#define RTR_UNCYC 0xa0
//#define ACTCTL    0x98
//#define SEGON	  0xe0
//#define SEGOFF	  0xc0
//#define BLINKCTL  0x88
//
//u8 count;
//u16 tcount;
//u16 Press_Adc; 
//
//u8 Read_flag,SendFlag;  
//unsigned char flag; 
//
//
//u8 Receive_Byte(void);//recieve one byte
//u8 Recv_Uart(void);	
//void Send_Byte(u8 dat);				
//void Send_Uart(u8 dat);	 
//
//
//
//
//
///**
//void IAQ_Init(void)
//{ 
//        GPIO_InitTypeDef GPIO_InitStructure;
//	 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //ʹ��PB�˿�ʱ��
//        //����I2C1_SCL(PB6)��I2C1_SDA(PB7)��©���
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
//        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
//        GPIO_Init(GPIOB, &GPIO_InitStructure);	 
//}
//**/   
//
//void I2C_Start(void)
//{
//	/* SDA,SCL��Ϊ��, ׼��������ʼ���� */
//	SDA_OUT();     //sda�����
//	SDA = 1;
//	delay_us(2);
//	SCL = 1;
//	delay_us(10);
//    
//	/* ������ʼ����: SCL��, SDA��->�� */
//	SDA = 0;
//	delay_us(10);
//	SCL = 0;
//	delay_us(4);
//
//	return;
//}
//
//void I2C_Stop(void)
//{
//	/* ��������: SCL��, SDA��->�� */  
//	SDA_OUT();     //sda�����
//	SDA = 0;
//	delay_us(2);     
//	SCL = 1;
//	delay_us(10);
//	SDA = 1;
//	delay_us(8);   
//	return;
//}
///*================================================================ 
//void I2CStart(void)
//================================================================*/ 
//void I2CStart(void) 
//{	
//	SDA = 1;    
//	SCL = 1;     
//	delay_us(10);    
//	SDA = 0;     
//	delay_us(10);    
//	SCL = 0;     
//	delay_us(10);    
//}  
///*================================================================ 
//���� �ơ�void I2CStop(void) 
//���� �ܡ�I2Cֹͣ�ź�  
//���� ע��SCL��SDAͬΪ�ͣ�SCL����ɸ�֮��SDA����ɸ�  
//================================================================*/ 
//void I2CStop(void) 
//{
//	SDA = 0;    
//	SCL = 0;  
//	delay_us(10);    
//	SCL = 1;     
//	delay_us(10);    
//	SDA = 1;     
//	delay_us(10);    
//}
///*================================================================ 
//���� �ơ�unsigned char I2CWRByte(unsigned char WriteData) 
//���� �ܡ�I2Cдһ���ֽ����ݣ�����ACK����NACK 
//���� ע���Ӹߵ��ͣ����η���  
//================================================================*/ 
//unsigned char I2CWriteByte(unsigned char WriteData) 
//{  
//	unsigned char i;  SCL = 0;  
//	for(i = 0;i < 8;i++)  
//	{   
//		if(WriteData & 0x80)    
//			SDA = 1;    
//		else    
//			SDA = 0;    
//		delay_us(10); 
//		SCL = 1;     //���SDA�ȶ�������SCL���������أ��ӻ���⵽��������ݲ���   
//		delay_us(10); 
//		SCL = 0;   
//		delay_us(10); 
//		WriteData <<= 1;  
//	}   
//	SDA = 1;  
//	SCL = 1;  
//	delay_us(20); 
//	if(SDA == 1)   //SDAΪ�ߣ��յ�NACK  
//	{   
//		SCL = 0;   
//		delay_us(50); 
//		return NACK;   
//	}  
//	else         //SDAΪ�ͣ��յ�ACK  
//	{   
//		SCL = 0;   
//		delay_us(50); 
//		return ACK;  
//	} 
//}  
///*================================================================ 
//���� �ơ�unsigned char I2CRDByte(unsigned char AckValue)  
//���� �ܡ�I2C��һ���ֽ����ݣ���ڲ������ڿ���Ӧ��״̬��ACK����NACK 
//���� ע���Ӹߵ��ͣ����ν���  
//================================================================*/ 
//unsigned char I2CReadByte(unsigned char AckValue) 
//{  
//	unsigned char i,ReadData = 0;  
//	SCL = 0;  
//	SDA = 1;   //�ͷ�����   
//	for(i = 0;i < 8;i++)   
//	{   
//		ReadData <<= 1;    
//		SCL = 1;  //����������   
//		delay_us(0); 
//		if(SDA == 1)   //������ȡ����   
//		{    
//			ReadData |= 0x01;   
//		}   
//		else    
//		{    
//			ReadData &= 0xfe;   
//		}   
//		SCL = 0;     //�½��أ��ӻ�������һλֵ   
//		delay_us(10); 
//	}     
//	SDA = AckValue; //Ӧ��״̬  
//	delay_us(10); 
//	SCL = 1;                             
//	delay_us(50); 
//	SCL = 0;     
//	SDA = 1;                    
//	delay_us(2); 
//	return ReadData; 
//} 
//void I2C_Ack(u8 a)
//{
//	/* ����Ӧ����Ӧ���ź� */
//	SDA_OUT();     //sda�����
//	if(a == 0)	
//	{
//		SDA = 0;	/* Ӧ�� */        
//	}
//	else 
//	{
//		SDA = 1;	/* ��Ӧ�� */
//	}
//	delay_us(6);      
//	SCL = 1;
//	delay_us(10);
//	SCL = 0;
//	delay_us(4);    
//
//	
//	return;
//}
//
///****************************************************************
//* Prototype      : u_char I2C_WriteByte()
//* Parameters     : u_char c, �����͵��ַ�.
//* Return Value   : none
//* Global Variable: none
//* Description    : ����һ���ֽڵ�����.
//*****************************************************************/ 
//u8 I2C_WriteByte(u8 c)
//{
//	u8 i,ack;
//	SDA_OUT();     //sda�����
//	for(i=0; i<8; i++)	
//	{
//		if((c << i) & 0x80)		
//		{
//			SDA = 1;    
//		}
//        else   
//		{
//			SDA = 0;                
//		}
//		delay_us(10);
//		SCL = 1;
//		delay_us(10);
//		SCL = 0; 
//	}
//    
//	delay_us(4);
//	SDA = 1;
//	delay_us(6);   
//	SCL = 1;
//	delay_us(10);
//	SDA_IN();      //SDA����Ϊ���� 
//	if(SDA == 1)
//	{
//		ack = I2C_NAK;     
//	}
//	else 
//	{
//		ack = I2C_ACK;		
//	}
//	
//	SCL = 0;
//	delay_us(4);
//
//	return ack;
//}
//
///****************************************************************
//* Prototype      : u_char I2C_ReadByte()
//* Parameters     : none
//* Return Value   : u_char, ���յ����ַ�.
//* Global Variable: none
//* Description    : ����һ���ֽڵ�����.
//*****************************************************************/ 
//
//u8 I2C_ReadByte(void)
//{
//	u8 i,retc=0;
//	
//	/* ��������Ϊ���뷽ʽ */
//	SDA_OUT();     //sda�����
//	SDA = 1;	
//	SDA_IN();      //SDA����Ϊ���� 			  
//	for(i=0; i<8; i++)
//	{
//		SCL = 0;
//		delay_us(10);
//		SCL = 1;
//		delay_us(10);
//		retc = retc << 1;
//		if(SDA == 1) 
//		{
//			retc++;
//		}
//	}
//
//	SCL = 0;    
//	delay_us(10); 
//	return retc;
//}
// 
///*================================================================ 
//�����ơ�void Delay(unsigned int t) 
//�����ܡ���ʱ��
//  
//================================================================*/ 
///**
//void Delay(unsigned int t) 
//{ 
//	unsigned char i;  
//	while(t--)   
//	{ 
//		for(i = 0;i < 200;i++);
//	} 
//}  
//**/
///*================================================================ 
//�����ơ�void SHT21Init(void) 
//�����ܡ�SHT21��ʼ������
//  
//================================================================*/ 
//void SHT21Init(void) 
//{  
////	I2CInit(); 
////	#if 0
////		IAQ_Init();
////	#endif
//	I2CStart();  
//	I2CWriteByte(SHT21ADDR&0xfe);  
//	I2CWriteByte(0xfe);  
//	I2CStop(); 
//	delay_us(600);
//}  
///*================================================================ 
//�����ơ�long Sht21Work(unsigned char order) 
//�����ܡ�Sht21������ʪ��
//  
//================================================================*/ 
//int Sht21Work(unsigned char order) 
//{   
//	float temp = 0,TH = 0;  
//	unsigned char MSB,LSB;  
//	unsigned long shidu,wendu;  
//	I2CStart();  
//	if(I2CWriteByte(SHT21ADDR & 0xfe) == ACK) 
//	{ 
//		if(I2CWriteByte(order) == ACK) 
//		{ 
//			do  
//			{ 
//				delay_us(30); 
//				I2CStart(); 
//			} 
//			while(I2CWriteByte(SHT21ADDR | 0x01) == NACK); 
//			MSB = I2CReadByte(ACK); 
//			LSB = I2CReadByte(NACK); 
//			I2CStop(); 
//			LSB &= 0xfc; 
//			temp = MSB*256+LSB; 
//			if (order == 0xf3)  
//			{ 
//				TH=(175.72)*temp/65536-46.85; 
//			//	wendu =fabs(TH)*100; 
//			    wendu =TH*100; 
//				if(TH >= 0) 
//					flag = 0; 
//				else 
//					flag = 1; 
//				return wendu;
//			} 
//			else 
//			{ 
//				TH = (temp*125)/65536-6; 
//				shidu = fabs(TH)*100; 
//				return shidu; 
//			} 
//		} 
//	}  
//	return 0;  
//}
//#endif



#if (self_flag == 1)
/**************************************************************************************
*******************************ģ��IIC����ʽ*****************************************
�ٲ���OK�������룬�����ʽΪ ff ** ** ** ** ** ** **��ȡ�ĵ�һλΪ��Чλ
�ڸ�һ��ʱ����ȡ��һ����Чֵ��ff ** ff ff ff ff ff ff���̶�������ȡ����Чֵ
**************************************************************************************/
/* Private macro -------------------------------------------------------------*/
#define SCLL                                GPIOA->BRR = GPIO_Pin_6
#define SCLH                                GPIOA->BSRR = GPIO_Pin_6                                             
#define SDAL                                GPIOA->BRR = GPIO_Pin_7
#define SDAH                                GPIOA->BSRR = GPIO_Pin_7

#define SDA_STATE                        GPIOA->IDR&GPIO_Pin_7
enum I2C_REPLY {I2C_NACK = 0, I2C_ACK = 1};
enum I2C_STATE {I2C_READY = 0, I2C_BUSY = 1, I2C_ERROR = 2};  


/* Private functions ---------------------------------------------------------*/
static u8 I2C_Start(void);
static void I2C_Stop(void);
static void I2C_Ack(u8);
static u8 I2C_Wait_Ack(void);
static void I2C_SendByte(u8);
static u8 I2C_ReceiveByte(void);
//static void I2C_Delay_us(u16 n);
 extern int16_t Temperature;
extern u16 Humidity	;
extern u16 USART1_TX_BUF[64];
extern int16_t Temperature_Vs;
extern int16_t Humidity_Vs;
extern u8 tmp_ok;
extern u8 hmp_ok;
u16 shidu;
u16 shidu_temp;
u16 shidu_data_temp[5];
u8   senser_th_flag=0;
u8   senser_flag=0;
int16_t	wendu;
int16_t	wendu_tem;
int16_t TP_data_temp[5];  
/*******************************************************************************/
//void IAQ_Init(void)
//{ 
//	GPIO_InitTypeDef GPIO_InitStructure;  
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		
//	// Configure IO connected to IIC//////////////////////
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6|GPIO_Pin_7;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
////	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	
//	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_OD;	
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//}
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
          //      printf("I2C is busy!\r\n");
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
		delay_us(9); 
        SCLH;
		delay_us(9); 
        SDAH;
		delay_us(9); 
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
         //               printf("I2C NACK!\r\n");
                        return I2C_NACK;
                }
        }
        SCLL;
		delay_us(2); 
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
		delay_us(2); 
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



/*================================================================ 
�����ơ�void SHT21Init(void) 
�����ܡ�SHT21��ʼ������
  
================================================================*/ 
void SHT21Init(void) 
{  
//	I2CInit(); 
//	#if 0
//		IAQ_Init();
//	#endif
	I2C_Start();  
	I2C_SendByte(SHT21ADDR&0xfe);  
	I2C_SendByte(0xfe);  
	I2C_Stop(); 
	delay_us(600);
} 

float Sht21Work(u8 order) 
{ 
	float temp = 0,TH = 0;  
	u8 MSB,LSB;  



	I2C_Start();
	I2C_SendByte(SHT21ADDR & 0xfe);
	delay_us(5); 
    if(I2C_Wait_Ack()) 
	{
   	 
	  
//	if(I2C_SendByte(SHT21ADDR & 0xfe) == ACK) 
//	{ 
		I2C_SendByte(order);
		delay_us(5); 
	    if(I2C_Wait_Ack()) 
		{
 
			do  
			{ 
				delay_us(5); 
				I2C_Start(); 
				I2C_SendByte(SHT21ADDR | 0x01);
				delay_us(5);
			}			
			while(!(I2C_Wait_Ack())); 

		 



//			while(I2C_SendByte(SHT21ADDR | 0x01) == NACK); 
			MSB = I2C_ReceiveByte(); 
			delay_us(5); 
			I2C_Ack(1);
		
			LSB = I2C_ReceiveByte();
			delay_us(5);  
			I2C_Ack(0);

			I2C_Stop(); 

		
			LSB &= 0xfc; 
			temp = MSB*256+LSB; 
			if(temp==0)
			 {
			 senser_th_flag=7;
			 Temperature=0;
			 Humidity=0;
			 }  
			 else if (order == 0xf3)  
			{ 
				TH=(175.72)*temp/65536-46.85; 
				TH=TH-Temp_Calib;

			//	TH=(int)(TH+0.5)>(int)TH?(int)TH+1:(int)TH;
//				  if(TH>=0)
//				    USART1_TX_BUF[28]=0x00;
//				else 
//				   	USART1_TX_BUF[28]=0x01;	
//
////				wendu =fabs(TH)*100; 
			//	wendu =fabs(TH);
				//wendu =TH;  
//				if(TH >= 0) 
//					flag = 0; 
//				else 
//					flag = 1;
	           if((ABS(TH,wendu) <= 20)||(wendu==0))

	               {
				   if(wendu==0)
					   wendu=TH;
					 	wendu =(wendu+TH)/2;
						   wendu_tem=  TP_data_lvbo(wendu,&TP_data_temp[0],&TP_data_temp[0]);
						
						if((wendu_tem+120)>Temperature_Vs)

						 
							    Temperature=  wendu_tem+100-Temperature_Vs;	
			                   // Temperature=  (wendu+Temperature+100-Temperature_Vs)/2;	
							else 
								Temperature=-20;
						
					}

/*
			    if((ABS(TH,wendu) <= 20)||(wendu==0))
               {
				 	wendu =TH;
					 if((ABS(Temperature,wendu) <= 20)||(Temperature==0))
					 {
		//			  	 if(Temperature==0)
		//                     Temperature=wendu;
						 if(((wendu+Temperature)/2+120) > Temperature_Vs )
						    Temperature=  (wendu+Temperature)/2+100-Temperature_Vs;	
		                   // Temperature=  (wendu+Temperature+100-Temperature_Vs)/2;	
						else if((wendu+120) > Temperature_Vs )
						    Temperature=  wendu+100-Temperature_Vs;					   
						else	Temperature=-20;
					}
				}
				*/
		 	if(Temperature>=100)
				  Temperature=100;
			if(Temperature<=-20)
				  Temperature=-20;		
			tmp_ok=1;
			senser_th_flag=0;
	        	return wendu;
			} 
			else if (order == 0xf5)  
			{ 
				TH = (temp*125)/65536-6 +3; 
//				shidu = fabs(TH)*100; 

			   if((ABS(fabs(TH),shidu)<=20)||(shidu==0))

		   	   {
			       if (shidu==0)
					  shidu= fabs(TH);
			     	shidu = (fabs(TH)+shidu)/2;
					   shidu_temp=  data_lvbo(shidu,&shidu_data_temp[0],&shidu_data_temp[0]);

						shidu_temp=2.3+1.04*shidu_temp;


						if((shidu_temp+100)>Humidity_Vs)
						  Humidity =shidu_temp+100-Humidity_Vs;
						 
						 else Humidity=0;
				  
			   }
/*

          if((ABS(fabs(TH),shidu)<=20)||(shidu==0))
		   	   {
				shidu = fabs(TH);
					 if((ABS(shidu,Humidity) <= 30)||(Humidity==0))
					 {
						if(((shidu+Humidity)/2+100)>Humidity_Vs)
						  Humidity =(shidu+Humidity)/2+100-Humidity_Vs;
						 
						else if((shidu+100)>Humidity_Vs)
						      Humidity =shidu+100-Humidity_Vs;
						 else Humidity=0;
					 } 
				}	 
*/					 
				if(Humidity>=100)
				Humidity=100;
				hmp_ok=1;
				senser_th_flag=0;
				return shidu; 
			} 
		}
	   	else
	     senser_th_flag=7; 
	}
  
	return 0;  	 
}
int16_t TP_data_lvbo( int16_t data1,int16_t* data_in,int16_t* data_out)
{
	u8 i;
	int16_t data_sum=0,data_ave=0;
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

