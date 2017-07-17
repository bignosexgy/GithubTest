#include "sys.h"
#include "usart.h"
#include "gpio.h"
#include "rtc.h"
//#include "crc.h"
#include "usart1.h"
#include <stdarg.h>	   
#include "misc.h"
#include <string.h>

#define CO2_UART1_CONFIG
#define usb_UART2_CONFIG 
#define wifi_UART1_CONFIG 
extern PubTimer RTC_Set_Time3;
//u8 USART_RX_Flag;
//u8 USART_TX_Flag;	  

//extern u32 Runtime;	
//extern u8 Fan_Flag;	  
//extern u8 Connect_Flag;	 
//u8 USART_TX_Connect_Flag=0;	 
//u8 USART_TX_Vacuumpump_Flag=0;	
//u8 USART_TX_Vacuometer_Flag=0;	
//u8 USART_TX_Data_Flag=0;
extern u8 USART1_RX_Timer;
extern u8 USART2_RX_Timer;
extern u8 USART4_RX_Timer;
extern u8 USART5_RX_Timer;
//static u8 USART_TX_Vacuometer_Conter;
//static u8 USART_TX_Vacuumpump_Conter;
//static u8 USART_TX_Compressor_Conter;
//static u8 USART_TX_Data_Conter;
//extern u8 USART_TX_Data_Time_Conter;
static u8 USART2_TX_Data_Conter;
static u8 USART4_TX_Data_Conter;
extern u8 LOGIN_CMD_Flag;
extern u8 Set_time_Flag;
extern u8 Set_mac_Flag;
extern u8 RE_HEART_CMD_Flag;
extern u8  WIFI_Flag;
extern u8 oled_Flag_1;//1屏保
extern u16 re_frequency;
extern u8  test_imid;
//extern u32 Runtime;
//extern u8 Filament_Static;
extern PubTimer Timer;//时钟结构体 	
extern PubTimer RTC_Set_Time2;
extern int16_t Coldtrap_Tem;
extern int16_t Sample_Tem;
extern u32 Vac;
extern u8 Model;
extern u8 CO2_Receive_Flag;
//extern u8 USART_RX_Timer;
extern u8 USART2_RX_Timer; 
extern u8 USART4_RX_Timer;
 
extern int16_t Temperature;
extern u16   Humidity;
extern u16 CO_Value;
extern u16 PM25_Value_1;
extern	u16 CO2_Data;
extern	u16 CO_Data;
extern	float VOC_Data ;

int16_t Temperature_display;
u16   Humidity_display;
u16 PM25_Value_display;
u16 CO2_Data_display;
u16 CO_Data_display;
float VOC_Data_display ;
u8 sound_display=0;


extern	u16 re_frequency;
extern u8 Pingji_Flag;
u16 Set_time[6];
extern u8 Flash_Data_Length;
extern u32 p_data_write[56];
extern u32 p_data_read[56];
extern u32 Flash_Write_Address;
extern u32 send_i_len ;
extern u8  RE_DATA_CMD_Flag;
extern u8 ID_msg[13];
extern int16_t PM25_Vs;
extern u16 PM25_Vo;
extern int16_t Temperature_Vs;
extern int16_t Humidity_Vs;
extern u16 shidu;
extern int16_t	wendu; 
extern u16 BAT_Value;
extern u8  senser_th_flag;
extern u8  senser_co_flag;
extern u8  senser_voc_flag;
extern u8  senser_co2_flag;
extern u8  senser_pm_flag;
extern int16_t CO2_Data_Vs;
extern int16_t CO_Data_Vs ;
extern u16 CO_Data_Vo;
extern u16 CO2_Data_Vo;
extern int16_t VOC_Data_Vs;

extern float VOC_Data_Vo;
extern u8 data_buffer[20];
u8 Mac_TX_BUF[10];
u8 Internet_TX_BUF[10];
u8 cmd_jianya_flag=0;
extern  u8 internet__send_flag;

extern  u8 LOGIN_CMD_Flag_num;
extern volatile int8_t OLED_curPos ;
u8 internet	=0;
extern u8   hx_LED_leibie;
extern u8 VNCL_ceshi;  //1 ok
extern u8 senser_ceshi;	 //1  ok
extern u8 flash_ceshi;  //1 ok
extern u8 rtc_ceshi;	 //1  ok
u8 Jiaoyan_Data_Vs=0x01;
u8 Usb_led_Vs=0;
double  dataa111_name;
//extern u16 RTC_Time_Set_flag;
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif
u8 wifi_msg_temp[1000]; 
u8 wifi_msg_temp_1[100]; 	 
u8 USART1_RX_BUF[564];     //接收缓冲,最大64个字节.
u8 USART1_RX_BUF1[564];
u8 USART1_TX_BUF[64];     //发送缓冲,最大64个字节. 
//u8 USART_RX_BUF[64];     //接收缓冲,最大64个字节.
//u8 USART_TX_BUF[64];     //发送缓冲,最大64个字节.
u8 USART2_RX_BUF[64];     //接收缓冲,最大64个字节.
u8 USART2_TX_BUF[64];     //发送缓冲,最大64个字节.


u8 USART4_RX_BUF[64];     //接收缓冲,最大64个字节.
u8 USART4_TX_BUF[10];     //发送缓冲,最大64个字节.
u8 USART5_TX_BUF[10];     //发送缓冲,最大64个字节.
u8 USART5_RX_BUF[64];     //发送缓冲,最大64个字节.
u8 USART1_RX_BUF_Counter=0;
u8 USART2_RX_BUF_Counter=0;

u8 USART4_RX_BUF_Counter=0;
u8 USART5_RX_BUF_Counter=0;
u8 Mac_msg[18]; 


//接收状态
//bit7，接收完成标志
//bit6，接收到0x0d
//bit5~0，接收到的有效字节数目
u8 USART_RX_STA=0;       //接收状态标记

void uart1_init(u32 bound){
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	 //USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

  //Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		//

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
  
   //USART 初始化设置   
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断   
  USART_Cmd(USART1, ENABLE);                    //使能串口 
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	uint16_t Res;
//	u8 j;
//	static u8 i;
//	static u8 USART_RX_Start_Flag,USART_RX_End_Flag,USART_TX_Start_Flag,USART_TX_End_Flag;
//	static u8 USART_RX_Length;
//	static u8 USART_RX_Step;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据wifi	
			USART1_RX_BUF[USART1_RX_BUF_Counter]=	Res;
		#ifdef wifi_UART1_CONFIG
			USART1_RX_Timer=0;
//			//USART_TX_BUF[USART3_RX_BUF_Counter]=USART3_RX_BUF[USART3_RX_BUF_Counter];        		
		#endif	
		USART1_RX_BUF_Counter++;		
     } 
} 


void USART2_Config(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;


  	
		/* config USART2 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* USART2 GPIO config */
   /* Configure USART2 Tx (PA.02) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	    
  /* Configure USART2 Rx (PA.03) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);


	 //Usart3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART3
	  
	/* USART3 mode config */
	//USART_InitStructure.USART_BaudRate = 115200; bound
	USART_InitStructure.USART_BaudRate = bound; 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure); 
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断   
	USART_Cmd(USART2, ENABLE);		     //使能串口    
}



void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	uint16_t Res;
	//u8 j;
	//static u8 i;
	//static u8 USART3_RX_Start_Flag,USART3_RX_End_Flag,USART3_TX_Start_Flag,USART3_TX_End_Flag;
//	static u8 USART2_RX_Length,USART2_RX_Counter;
//	static u8 USART2_RX_Step;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(usb)
	{
		Res =USART_ReceiveData(USART2);//(USART1->DR);	//读取接收到的数据 	
		USART2_RX_BUF[USART2_RX_BUF_Counter]=	Res;
	#ifdef usb_UART2_CONFIG
			USART2_RX_Timer=0;
//			//USART_TX_BUF[USART3_RX_BUF_Counter]=USART3_RX_BUF[USART3_RX_BUF_Counter];        		
		#endif	
		USART2_RX_BUF_Counter++;
		/*
		if(USART3_RX_Counter >6)
		{
			USART3_RX_Counter=0;			
			CO2_Receive_Flag = 1;
			USART_TX_Data_Flag=1;
			USART_TX_BUF[0]=0x08;
			USART_TX_BUF[1]=USART3_RX_BUF[0];
	        USART_TX_BUF[2]=USART3_RX_BUF[1];
			USART_TX_BUF[3]=USART3_RX_BUF[2];
			USART_TX_BUF[4]=USART3_RX_BUF[3];
			USART_TX_BUF[5]=USART3_RX_BUF[4];
			USART_TX_BUF[6]=USART3_RX_BUF[5];
			USART_TX_BUF[7]=USART3_RX_BUF[6];
			USART_TX_BUF[8]=USART3_RX_BUF[7];
		}
		*/
    } 
}	 
/*
 * 函数名：USART3_Config
 * 描述  ：USART3 GPIO 配置,工作模式配置。115200 8-N-1
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void USART4_Config(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* config USART3 clock */
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	
	//RCC_AHBPeriphClockCmd(RCC_APB1Periph_USART3 | RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART4, ENABLE);  	

	/* USART3 GPIO config */
	/* Configure USART4 Tx (Pc.10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);    

	/* Configure USART4 Rx (Pc.11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	 //Usart4 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART3
	  
	/* USART3 mode config */
	//USART_InitStructure.USART_BaudRate = 115200; bound
	USART_InitStructure.USART_BaudRate = bound; 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure); 
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启中断   
	USART_Cmd(UART4, ENABLE);		     //使能串口    
}



void UART4_IRQHandler(void)                
{
	uint16_t Res;
	//u8 j;
	//static u8 i;
	//static u8 USART3_RX_Start_Flag,USART3_RX_End_Flag,USART3_TX_Start_Flag,USART3_TX_End_Flag;
//	static u8 USART3_RX_Length,USART3_RX_Counter;
//	static u8 USART3_RX_Step;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //接收中断()
	{
		Res =USART_ReceiveData(UART4);//(USART1->DR);	//读取接收到的数据 	
		USART4_RX_BUF[USART4_RX_BUF_Counter]=	Res;
		#ifdef CO2_UART1_CONFIG
			USART4_RX_Timer=0;
			//USART_TX_BUF[USART3_RX_BUF_Counter]=USART3_RX_BUF[USART3_RX_BUF_Counter];        		
		#endif	
		USART4_RX_BUF_Counter++;
		/*
		if(USART3_RX_Counter >6)
		{
			USART3_RX_Counter=0;			
			CO2_Receive_Flag = 1;
			USART_TX_Data_Flag=1;
			USART_TX_BUF[0]=0x08;
			USART_TX_BUF[1]=USART3_RX_BUF[0];
	        USART_TX_BUF[2]=USART3_RX_BUF[1];
			USART_TX_BUF[3]=USART3_RX_BUF[2];
			USART_TX_BUF[4]=USART3_RX_BUF[3];
			USART_TX_BUF[5]=USART3_RX_BUF[4];
			USART_TX_BUF[6]=USART3_RX_BUF[5];
			USART_TX_BUF[7]=USART3_RX_BUF[6];
			USART_TX_BUF[8]=USART3_RX_BUF[7];
		}
		*/
    } 
} 
void USART5_Config(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* config USART3 clock */
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	
	//RCC_AHBPeriphClockCmd(RCC_APB1Periph_USART3 | RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART5, ENABLE);  	

	/* USART3 GPIO config */
	/* Configure USART4 Tx (Pc.10) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);    

	/* Configure USART4 Rx (Pc.11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);


	 //Usart4 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		//

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART3
	  
	/* USART3 mode config */
	//USART_InitStructure.USART_BaudRate = 115200; bound
	USART_InitStructure.USART_BaudRate = bound; 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART5, &USART_InitStructure); 
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启中断   
	USART_Cmd(UART5, ENABLE);		     //使能串口    
}



void UART5_IRQHandler(void)                	//串口3中断服务程序
{
	uint16_t Res;
	//u8 j;
	//static u8 i;
	//static u8 USART3_RX_Start_Flag,USART3_RX_End_Flag,USART3_TX_Start_Flag,USART3_TX_End_Flag;
//	static u8 USART3_RX_Length,USART3_RX_Counter;
//	static u8 USART3_RX_Step;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //接收中断()
	{
		Res =USART_ReceiveData(UART5);//(USART1->DR);	//读取接收到的数据 	
		USART5_RX_BUF[USART5_RX_BUF_Counter]=	Res;
//		#ifdef CO2_UART1_CONFIG
//			USART3_RX_Timer=0;
//			//USART_TX_BUF[USART3_RX_BUF_Counter]=USART3_RX_BUF[USART3_RX_BUF_Counter];        		
//		#endif	
		USART5_RX_BUF_Counter++;
		/*
		if(USART3_RX_Counter >6)
		{
			USART3_RX_Counter=0;			
			CO2_Receive_Flag = 1;
			USART_TX_Data_Flag=1;
			USART_TX_BUF[0]=0x08;
			USART_TX_BUF[1]=USART3_RX_BUF[0];
	        USART_TX_BUF[2]=USART3_RX_BUF[1];
			USART_TX_BUF[3]=USART3_RX_BUF[2];
			USART_TX_BUF[4]=USART3_RX_BUF[3];
			USART_TX_BUF[5]=USART3_RX_BUF[4];
			USART_TX_BUF[6]=USART3_RX_BUF[5];
			USART_TX_BUF[7]=USART3_RX_BUF[6];
			USART_TX_BUF[8]=USART3_RX_BUF[7];
		}
		*/
    } 
}
//函 数 名：AscToHex()
//功能描述：把ASCII转换为16进制
unsigned char AscToHex(unsigned char aHex)
{
    if((aHex>=0)&&(aHex<=9))
        aHex += 0x30;
    else if((aHex>=10)&&(aHex<=15))//A-F
        aHex += 0x37;
    else aHex = 0xff;
    return aHex;
}

//函 数 名：HexToAsc()
//功能描述：把16进制转换为ASCII
unsigned char HexToAsc(unsigned char aChar)
{
    if((aChar>=0x30)&&(aChar<=0x39))
        aChar -= 0x30;
    else if((aChar>=0x41)&&(aChar<=0x46))//大写字母
        aChar -= 0x37;
    else if((aChar>=0x61)&&(aChar<=0x66))//小写字母
        aChar -= 0x57;
    else aChar = 0xff;
    return aChar; 
}

// C prototype : void StrToHex(BYTE *pbDest, BYTE *pbSrc, int nLen)
// parameter(s): [OUT] pbDest - 输出缓冲区
//	[IN] pbSrc - 字符串
//	[IN] nLen - 16进制数的字节数(字符串的长度/2)
// return value: 
// remarks : 将字符串转化为16进制数 
void StrToHex(char *pbDest, char *pbSrc, int nLen)
{
	char h1,h2;
	char s1,s2;
	int i;
	
	for (i=0; i<nLen; i++)
	{
		h1 = pbSrc[2*i];
		h2 = pbSrc[2*i+1];
		
		s1 = toupper(h1) - 0x30;
		if (s1 > 9) 
		s1 -= 7;
		
		s2 = toupper(h2) - 0x30;
		if (s2 > 9) 
		s2 -= 7;
		
		pbDest[i] = s1*16 + s2;
	}
}

/*
// C prototype : void HexToStr(BYTE *pbDest, BYTE *pbSrc, int nLen)
// parameter(s): [OUT] pbDest - 存放目标字符串
//	[IN] pbSrc - 输入16进制数的起始地址
//	[IN] nLen - 16进制数的字节数
// return value: 
// remarks : 将16进制数转化为字符串
*/
void HexToStr(char *pbDest, char *pbSrc, int nLen)
{
	char	ddl,ddh;
	int i;
	
	for (i=0; i<nLen; i++)
	{
		ddh = 48 + pbSrc[i] / 16;
		ddl = 48 + pbSrc[i] % 16;
		if (ddh > 57) ddh = ddh + 7;
		if (ddl > 57) ddl = ddl + 7;
		pbDest[i*2] = ddh;
		pbDest[i*2+1] = ddl;
	}
	
	pbDest[nLen*2] = '\0';
}



/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */

//int fputc(int ch, FILE *f)
//{
	/* 将Printf内容发往串口 */
	//USART_SendData(USART1, (unsigned char) ch);
//	while (!(USART1->SR & USART_FLAG_TXE));
	//while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);	
	//return (ch);
//}

/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART1_printf()调用
 */
 /**/
static char *itoa(int value, char *string, int radix)
{
	int     i, d;
	int     flag = 0;
	char    *ptr = string;
	
	// This implementation only works for decimal numbers. //
	if (radix != 10)
	{
	    *ptr = 0;
	    return string;
	}
	
	if (!value)
	{
	    *ptr++ = 0x30;
	    *ptr = 0;
	    return string;	 

	}
	
	// if this is a negative value insert the minus sign. //
	if (value < 0)
	{
	    *ptr++ = '-';
	
	    // Make the value positive. //
	    value *= -1;
	}
	
	for (i = 10000; i > 0; i /= 10)
	{
	    d = value / i;
	
	    if (d || flag)
	    {
	        *ptr++ = (char)(d + 0x30);
	        value -= (d * i);
	        flag = 1;
	    }
	}
	
	// Null terminate the string. //
	*ptr = 0;
	
	return string;

} 
/**/
u16 itoa_1(u16 value1, u16 value2, int radix)
{

	u16   a1,a2,a3,b1,b2,b3;

        
		a1= value1/10;
	    b1=value1%10;
		a2= a1/10;
	    b2=a1%10;
		a3= a2/10;
		b3= a2%10;
		value2=b1+(b2<<4)+(b3<<8)+(a3<<12);
	 	return value2;

} 
u8 itoa_2(u8 value1, u8 value2, int radix)
{

	u8   a1,a2,b1,b2;

        
		a1= value1/10;
	    b1=value1%10;
		a2= a1/10;
	    b2=a1%10;
//		a3= a2/10;
//		b3= a2%10;
		value2=b1+(b2<<4)+(a2<<8);
	 	return value2;

}

void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
{
	const char *s;
	int d;   
	char buf[16];
	
	va_list ap;
	va_start(ap, Data);
	
	while ( *Data != 0)     // 判断是否到达字符串结束符
	{				                          
		if ( *Data == 0x5c )  //'\'
	{									  
	switch ( *++Data )
	{
		case 'r':							          //回车符
			USART_SendData(USARTx, 0x0d);
			Data ++;
		break;
		
		case 'n':							          //换行符
			USART_SendData(USARTx, 0x0a);	
			Data ++;
		break;
		
		default:
			Data ++;
		break;
	}			 
	}
	else if ( *Data == '%')
	{									  //
	switch ( *++Data )
	{				
		case 's':										  //字符串
			s = va_arg(ap, const char *);
	for ( ; *s; s++) 
	{
		USART_SendData(USARTx,*s);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
		Data++;
		break;
	
	case 'd':										//十进制
	d = va_arg(ap, int);
	itoa(d, buf, 10);
	for (s = buf; *s; s++) 
	{
		USART_SendData(USARTx,*s);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
	Data++;
	break;
		 default:
				Data++;
		    break;
	}		 
	} /* end of else if */
	else USART_SendData(USARTx, *Data++);
	while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
}


void USART_TX_Run_State_updata(u8 Send_Length)		  //
{
	u8 j;	
	for(j=0;j<Send_Length;j++)  //return ACK
	{
		while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
		USART_SendData(USART1,USART1_TX_BUF[j]);
	}
	
	
	//USART_TX_Data_Flag=0; 	
	//USART_TX_Data_Time_Conter	= 0; //5-3=2  2*2=4S  不成功则每隔4秒钟发送一次
}


void USART3_TX_Run_State_updata(u8 Send_Lenth)	   //
{
	u8 j;	
	for(j=0;j<Send_Lenth;j++)  //return ACK
	{
		while( USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET );
		USART_SendData(UART4,USART4_TX_BUF[j]);
	}
}
  


void USART3_TX_State(void)	   //用于向CO2传感器发送命令
{
	u8 j;	
	//if(USART_TX_Data_Flag){
	//USART3_TX_BUF[0]=0xFE;
	//USART3_TX_BUF[1]=0x04;
	//USART3_TX_BUF[2]=0x00;
	//USART3_TX_BUF[3]=0x03;
	//USART3_TX_BUF[4]=0x00;
	//USART3_TX_BUF[5]=0x01;
	//USART3_TX_BUF[6]=0xD5;
	//USART3_TX_BUF[7]=0xC5;
	USART4_TX_BUF[0]=0x68;
	USART4_TX_BUF[1]=0x04;
	USART4_TX_BUF[2]=0x00;
	USART4_TX_BUF[3]=0x03;
	USART4_TX_BUF[4]=0x00;
	USART4_TX_BUF[5]=0x01;
	USART4_TX_BUF[6]=0xc8;
	USART4_TX_BUF[7]=0xf3;
	//USART3_TX_BUF={0xFE,0x04,0x00,0x03,0x00,0x01,0xd5,0xc5};
	//USART3_TX_BUF={0x68,0x04,0x00,0x03,0x00,0x01,0xc8,0xf3};
	 
	//for(j=0;j<USART_TX_BUF[2]+3;j++)  //return ACK
	for(j=0;j<8;j++)  //return ACK
	{
		while( USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET );
		USART_SendData(UART4,USART4_TX_BUF[j]);
	}
	if(USART4_TX_Data_Conter++ > 9)  //  连续发送10次没有接受到应答信号 则停止发送 启动联机模式
	{
	//	USART_TX_Data_Flag=0;
		USART4_TX_Data_Conter=0;
	//	USART_TX_Connect_Flag=1;
	//	Connect_Flag=0;
	}		
//	USART_TX_Data_Time_Conter	= 3; //5-3=2  2*2=4S  不成功则每隔4秒钟发送一次
	//USART_TX_Data_Time_Conter	= 0; //5-3=2  2*2=4S  不成功则每隔4秒钟发送一次
//}
}



char textFile[] = "[m3][v10]请打开网络为我配置联网 ";
//char textFile[] = "[m53][s3][t8][v10]请打开网络为我配置联网 ";

char textFile_1[]= "[m3][v10]早上好";
char textFile_2[]= "[m3][v10]上午好";
char textFile_3[]= "[m3][v10]中午好";
char textFile_4[]= "[m3][v10]下午好";
char textFile_5[]= "[m3][v10]晚上好";
char TemperatureFile[]= "[m3][v10]室内温度";
char HumidityFile[]= "室内相对湿度百分之";
char CO_DataFile[]= "室内一氧化碳浓度";
char CO2_DataFile[]= "室内二氧化碳浓度";
char VOC_DataFile[]= "室内甲醛浓度";
char PM25_ValueFile[]= "室内PM2.5浓度";
char danweiFile1[]= "℃ ";
char danweiFile2[]= "皮皮m ";
char danweiFile3[]= "微克每立方米 ";
char danweiFile6[]= "电池电量低";
	
void USART5_TX_State(void)	   //
{
	u8 j;
	u16 k2_num,k3_num,k4_num;
    char Temperature_temp[2],Humidity_temp[2],VOC_Data_temp[4],PM25_Value_temp[3],CO_Data_temp[3],CO2_Data_temp[4];
//	OLED_curPos
	 //Temperature_temp=StrToHex(Temperature);
	
	 //	 RTC_Set_Time3=Timer;


   sound_C=1;
	
	if(OLED_curPos==0)	   //温度
	{	
		sound_display=1;
		Temperature_display=Temperature;
	    sprintf(Temperature_temp,"%d",Temperature);
	//	k1_num=strlen(textFile_1);
		k2_num=strlen(TemperatureFile);
		k3_num=strlen(Temperature_temp);
		k4_num=strlen(danweiFile1);
		USART5_TX_BUF[0]=0xFD;
		USART5_TX_BUF[1]=0x00;
		USART5_TX_BUF[2]=k2_num+k3_num+k4_num+2;
		USART5_TX_BUF[3]=0x01;
		USART5_TX_BUF[4]=0x01;
	
	
		for(j=0;j<5;j++)  
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,USART5_TX_BUF[j]);
			}
//		for(j=0;j<k1_num;j++)  
//			{
//			     while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
//				 {
//			       if(RTC_Set_Time3.hour<9)
//				    USART_SendData(UART5,textFile_1[j]);
//					else if(RTC_Set_Time3.hour<12)
//				       USART_SendData(UART5,textFile_2[j]);
//					   	else if(RTC_Set_Time3.hour<15)
//				       USART_SendData(UART5,textFile_3[j]);
//					   	else if(RTC_Set_Time3.hour<18)
//				       USART_SendData(UART5,textFile_4[j]);
//					   	else 
//				        USART_SendData(UART5,textFile_5[j]);
//				}
//			}
		for(j=0;j<k2_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,TemperatureFile[j]);
			 }
	    for(j=0;j<k3_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,Temperature_temp[j]);
			}
		for(j=0;j<k4_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,danweiFile1[j]);
			}
	}
	 if(OLED_curPos==1)	  //湿度
	{
	    sound_display=1;
		Humidity_display=Humidity;
		sprintf(Humidity_temp,"%d",Humidity);
	//	k1_num=strlen(textFile_1);
		k2_num=strlen(HumidityFile);
		k3_num=strlen(Humidity_temp);
		
		USART5_TX_BUF[0]=0xFD;
		USART5_TX_BUF[1]=0x00;
		USART5_TX_BUF[2]=k2_num+k3_num+2;
		USART5_TX_BUF[3]=0x01;
		USART5_TX_BUF[4]=0x01;
	
	
		for(j=0;j<5;j++)  
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,USART5_TX_BUF[j]);
			}
//		for(j=0;j<k1_num;j++)  
//			{
//			     while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
//				 {
//			       if(RTC_Set_Time3.hour<9)
//				    USART_SendData(UART5,textFile_1[j]);
//					else if(RTC_Set_Time3.hour<12)
//				       USART_SendData(UART5,textFile_2[j]);
//					   	else if(RTC_Set_Time3.hour<15)
//				       USART_SendData(UART5,textFile_3[j]);
//					   	else if(RTC_Set_Time3.hour<18)
//				       USART_SendData(UART5,textFile_4[j]);
//					   	else 
//				        USART_SendData(UART5,textFile_5[j]);
//				}
//			}
		for(j=0;j<k2_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,HumidityFile[j]);
			 }
	    for(j=0;j<k3_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,Humidity_temp[j]);
			}
	}
	 if(OLED_curPos==2)	  //一氧化碳
	{
	   
		sound_display=1;
		CO_Data_display=CO_Data;
		sprintf(CO_Data_temp,"%d",CO_Data);
	
	//	k1_num=strlen(textFile_1);
		k2_num=strlen(CO_DataFile);
		k3_num=strlen(CO_Data_temp);
		k4_num=strlen(danweiFile2);
		USART5_TX_BUF[0]=0xFD;
		USART5_TX_BUF[1]=0x00;
		USART5_TX_BUF[2]=k2_num+k3_num+k4_num+2;
		USART5_TX_BUF[3]=0x01;
		USART5_TX_BUF[4]=0x01;
	
	
		for(j=0;j<5;j++)  
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,USART5_TX_BUF[j]);
			}
//		for(j=0;j<k1_num;j++)  
//			{
//			     while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
//				 {
//			       if(RTC_Set_Time3.hour<9)
//				    USART_SendData(UART5,textFile_1[j]);
//					else if(RTC_Set_Time3.hour<12)
//				       USART_SendData(UART5,textFile_2[j]);
//					   	else if(RTC_Set_Time3.hour<15)
//				       USART_SendData(UART5,textFile_3[j]);
//					   	else if(RTC_Set_Time3.hour<18)
//				       USART_SendData(UART5,textFile_4[j]);
//					   	else 
//				        USART_SendData(UART5,textFile_5[j]);
//				}
//			}
		for(j=0;j<k2_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,CO_DataFile[j]);
			 }
	    for(j=0;j<k3_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,CO_Data_temp[j]);
			}
		for(j=0;j<k4_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,danweiFile2[j]);
			}
	}
	if(OLED_curPos==3)	  //二氧化碳
	{
	    sound_display=1;
		CO2_Data_display=CO2_Data;
	    sprintf(CO2_Data_temp,"%d",CO2_Data);

	//	k1_num=strlen(textFile_1);
		k2_num=strlen(CO2_DataFile);
		k3_num=strlen(CO2_Data_temp);
		k4_num=strlen(danweiFile2);
		USART5_TX_BUF[0]=0xFD;
		USART5_TX_BUF[1]=0x00;
		USART5_TX_BUF[2]=k2_num+k3_num+k4_num+2;
		USART5_TX_BUF[3]=0x01;
		USART5_TX_BUF[4]=0x01;
	
	
		for(j=0;j<5;j++)  
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,USART5_TX_BUF[j]);
			}
//		for(j=0;j<k1_num;j++)  
//			{
//			     while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
//				 {
//			       if(RTC_Set_Time3.hour<9)
//				    USART_SendData(UART5,textFile_1[j]);
//					else if(RTC_Set_Time3.hour<12)
//				       USART_SendData(UART5,textFile_2[j]);
//					   	else if(RTC_Set_Time3.hour<15)
//				       USART_SendData(UART5,textFile_3[j]);
//					   	else if(RTC_Set_Time3.hour<18)
//				       USART_SendData(UART5,textFile_4[j]);
//					   	else 
//				        USART_SendData(UART5,textFile_5[j]);
//				}
//			}
		for(j=0;j<k2_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,CO2_DataFile[j]);
			 }
	    for(j=0;j<k3_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,CO2_Data_temp[j]);
			}
		for(j=0;j<k4_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,danweiFile2[j]);
			}
	}
	if(OLED_curPos==4)	  //甲醛
	{
	    sound_display=1;
		VOC_Data_display=VOC_Data ;
	    sprintf(VOC_Data_temp,"%.2f",VOC_Data)	;

	//	k1_num=strlen(textFile_1);
		k2_num=strlen(VOC_DataFile);
		k3_num=strlen(VOC_Data_temp);
		k4_num=strlen(danweiFile2);
		USART5_TX_BUF[0]=0xFD;
		USART5_TX_BUF[1]=0x00;
		USART5_TX_BUF[2]=k2_num+k3_num+k4_num+2;
		USART5_TX_BUF[3]=0x01;
		USART5_TX_BUF[4]=0x01;
	
	
		for(j=0;j<5;j++)  
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,USART5_TX_BUF[j]);
			}
//		for(j=0;j<k1_num;j++)  
//			{
//			     while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
//				 {
//			       if(RTC_Set_Time3.hour<9)
//				    USART_SendData(UART5,textFile_1[j]);
//					else if(RTC_Set_Time3.hour<12)
//				       USART_SendData(UART5,textFile_2[j]);
//					   	else if(RTC_Set_Time3.hour<15)
//				       USART_SendData(UART5,textFile_3[j]);
//					   	else if(RTC_Set_Time3.hour<18)
//				       USART_SendData(UART5,textFile_4[j]);
//					   	else 
//				        USART_SendData(UART5,textFile_5[j]);
//				}
//			}
		for(j=0;j<k2_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,VOC_DataFile[j]);
			 }
	    for(j=0;j<k3_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,VOC_Data_temp[j]);
			}
		for(j=0;j<k4_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,danweiFile2[j]);
			}
	}
	if(OLED_curPos==5)	  //PM2.5
	{
	    sound_display=1;
		PM25_Value_display=PM25_Value_1;
		sprintf(PM25_Value_temp,"%d",PM25_Value_1);
	//	k1_num=strlen(textFile_1);
		k2_num=strlen(PM25_ValueFile);
		k3_num=strlen(PM25_Value_temp);
		k4_num=strlen(danweiFile3);
		USART5_TX_BUF[0]=0xFD;
		USART5_TX_BUF[1]=0x00;
		USART5_TX_BUF[2]=k2_num+k3_num+k4_num+2;
		USART5_TX_BUF[3]=0x01;
		USART5_TX_BUF[4]=0x01;
	
	
		for(j=0;j<5;j++)  
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,USART5_TX_BUF[j]);
			}
//		for(j=0;j<k1_num;j++)  
//			{
//			     while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
//				 {
//			       if(RTC_Set_Time3.hour<9)
//				    USART_SendData(UART5,textFile_1[j]);
//					else if(RTC_Set_Time3.hour<12)
//				       USART_SendData(UART5,textFile_2[j]);
//					   	else if(RTC_Set_Time3.hour<15)
//				       USART_SendData(UART5,textFile_3[j]);
//					   	else if(RTC_Set_Time3.hour<18)
//				       USART_SendData(UART5,textFile_4[j]);
//					   	else 
//				        USART_SendData(UART5,textFile_5[j]);
//				}
//			}
		for(j=0;j<k2_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,PM25_ValueFile[j]);
			 }
	    for(j=0;j<k3_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,PM25_Value_temp[j]);
			}
		for(j=0;j<k4_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,danweiFile3[j]);
			}
	}
		if((OLED_curPos==6)&&(BAT_Value<=3350))	  //power
	{
	    sound_display=1;
		k4_num=strlen(danweiFile6);
		USART5_TX_BUF[0]=0xFD;
		USART5_TX_BUF[1]=0x00;
		USART5_TX_BUF[2]=k4_num+2;
		USART5_TX_BUF[3]=0x01;
		USART5_TX_BUF[4]=0x01;
	
	
		for(j=0;j<5;j++)  
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,USART5_TX_BUF[j]);
			}

		for(j=0;j<k4_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,danweiFile6[j]);
			}
	}


}
char textFile_6[]= "[m3][v10]云罐空气感知器为您检测：室内空气质量优";
char textFile_7[]= "[m3][v10]云罐空气感知器为您检测：室内空气质量良";
char textFile_8[]= "[m3][v10]云罐空气感知器为您检测：室内空气质量差";
void USART5_TX_State1(void)	   //
{
	u8 j;
	u16 k2_num,k3_num,k4_num;
    char Temperature_temp1[2],Humidity_temp1[2],VOC_Data_temp[4],PM25_Value_temp[3],CO_Data_temp[3],CO2_Data_temp[4];
//	OLED_curPos
	 //Temperature_temp=StrToHex(Temperature);
	 
//	 RTC_Set_Time3=Timer;
	 sound_C=1;
	 if(OLED_curPos==8)	  //
	{
	//	k1_num=strlen(textFile_1);

	   				   if(hx_LED_leibie==1)
				    k2_num=strlen(textFile_6);
					else if(hx_LED_leibie==2)
				       k2_num=strlen(textFile_7);
					   	else if(hx_LED_leibie==3)				
					      k2_num=strlen(textFile_8);
						  else
							 k2_num=strlen(textFile_6);

	//	k2_num=strlen(textFile_6);

		USART5_TX_BUF[0]=0xFD;
		USART5_TX_BUF[1]=0x00;
		USART5_TX_BUF[2]=k2_num+2;
		USART5_TX_BUF[3]=0x01;
		USART5_TX_BUF[4]=0x01;
	
	
		for(j=0;j<5;j++)  
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,USART5_TX_BUF[j]);
			}
//		for(j=0;j<k1_num;j++)  
//			{
//			     while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
//				 {
//			       if(RTC_Set_Time3.hour<9)
//				    USART_SendData(UART5,textFile_1[j]);
//					else if(RTC_Set_Time3.hour<12)
//				       USART_SendData(UART5,textFile_2[j]);
//					   	else if(RTC_Set_Time3.hour<15)
//				       USART_SendData(UART5,textFile_3[j]);
//					   	else if(RTC_Set_Time3.hour<18)
//				       USART_SendData(UART5,textFile_4[j]);
//					   	else 
//				        USART_SendData(UART5,textFile_5[j]);
//				}
//			}
		for(j=0;j<k2_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				{
				   if(hx_LED_leibie==1)
				    USART_SendData(UART5,textFile_6[j]);
					else if(hx_LED_leibie==2)
				       USART_SendData(UART5,textFile_7[j]);
					   	else if(hx_LED_leibie==3)				
					      USART_SendData(UART5,textFile_8[j]);
						  else
							USART_SendData(UART5,textFile_6[j]);
				}
			 }
	}
	if(OLED_curPos==0)	   //温度
	{	
	    sprintf(Temperature_temp1,"%d",Temperature);
		 Temperature_display=Temperature;

		k2_num=strlen(TemperatureFile);
		k3_num=strlen(Temperature_temp1);
		k4_num=strlen(danweiFile1);
		USART5_TX_BUF[0]=0xFD;
		USART5_TX_BUF[1]=0x00;
		USART5_TX_BUF[2]=k2_num+k3_num+k4_num+2;
		USART5_TX_BUF[3]=0x01;
		USART5_TX_BUF[4]=0x01;
	
	
		for(j=0;j<5;j++)  
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,USART5_TX_BUF[j]);
			}
		for(j=0;j<k2_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,TemperatureFile[j]);
			 }
	    for(j=0;j<k3_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,Temperature_temp1[j]);
			}
		for(j=0;j<k4_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,danweiFile1[j]);
			}
	}
	 if(OLED_curPos==1)	  //湿度
	{
		sprintf(Humidity_temp1,"%d",Humidity);
	
        Humidity_display=Humidity;


		k2_num=strlen(HumidityFile);
		k3_num=strlen(Humidity_temp1);
		
		USART5_TX_BUF[0]=0xFD;
		USART5_TX_BUF[1]=0x00;
		USART5_TX_BUF[2]=k2_num+k3_num+2;
		USART5_TX_BUF[3]=0x01;
		USART5_TX_BUF[4]=0x01;
	
	
		for(j=0;j<5;j++)  
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,USART5_TX_BUF[j]);
			}
		
		for(j=0;j<k2_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,HumidityFile[j]);
			 }
	    for(j=0;j<k3_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,Humidity_temp1[j]);
			}
	}
	 if(OLED_curPos==2)	  //一氧化碳
	{
		sprintf(CO_Data_temp,"%d",CO_Data);

        CO_Data_display=CO_Data;

	
		k2_num=strlen(CO_DataFile);
		k3_num=strlen(CO_Data_temp);
		k4_num=strlen(danweiFile2);
		USART5_TX_BUF[0]=0xFD;
		USART5_TX_BUF[1]=0x00;
		USART5_TX_BUF[2]=k2_num+k3_num+k4_num+2;
		USART5_TX_BUF[3]=0x01;
		USART5_TX_BUF[4]=0x01;
	
	
		for(j=0;j<5;j++)  
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,USART5_TX_BUF[j]);
			}
		
		for(j=0;j<k2_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,CO_DataFile[j]);
			 }
	    for(j=0;j<k3_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,CO_Data_temp[j]);
			}
		for(j=0;j<k4_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,danweiFile2[j]);
			}
	}
	if(OLED_curPos==3)	  //二氧化碳
	{
	    sprintf(CO2_Data_temp,"%d",CO2_Data);

        CO2_Data_display=CO2_Data;

		
		k2_num=strlen(CO2_DataFile);
		k3_num=strlen(CO2_Data_temp);
		k4_num=strlen(danweiFile2);
		USART5_TX_BUF[0]=0xFD;
		USART5_TX_BUF[1]=0x00;
		USART5_TX_BUF[2]=k2_num+k3_num+k4_num+2;
		USART5_TX_BUF[3]=0x01;
		USART5_TX_BUF[4]=0x01;
	
	
		for(j=0;j<5;j++)  
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,USART5_TX_BUF[j]);
			}
		
		for(j=0;j<k2_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,CO2_DataFile[j]);
			 }
	    for(j=0;j<k3_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,CO2_Data_temp[j]);
			}
		for(j=0;j<k4_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,danweiFile2[j]);
			}
	}
	if(OLED_curPos==4)	  //甲醛
	{
	    sprintf(VOC_Data_temp,"%.2f",VOC_Data)	;

        VOC_Data_display=VOC_Data ;
	
		k2_num=strlen(VOC_DataFile);
		k3_num=strlen(VOC_Data_temp);
		k4_num=strlen(danweiFile2);
		USART5_TX_BUF[0]=0xFD;
		USART5_TX_BUF[1]=0x00;
		USART5_TX_BUF[2]=k2_num+k3_num+k4_num+2;
		USART5_TX_BUF[3]=0x01;
		USART5_TX_BUF[4]=0x01;
	
	
		for(j=0;j<5;j++)  
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,USART5_TX_BUF[j]);
			}
		
		for(j=0;j<k2_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,VOC_DataFile[j]);
			 }
	    for(j=0;j<k3_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,VOC_Data_temp[j]);
			}
		for(j=0;j<k4_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,danweiFile2[j]);
			}
	}
	if(OLED_curPos==5)	  //PM2.5
	{
		sprintf(PM25_Value_temp,"%d",PM25_Value_1);
	

        PM25_Value_display=PM25_Value_1;


		k2_num=strlen(PM25_ValueFile);
		k3_num=strlen(PM25_Value_temp);
		k4_num=strlen(danweiFile3);
		USART5_TX_BUF[0]=0xFD;
		USART5_TX_BUF[1]=0x00;
		USART5_TX_BUF[2]=k2_num+k3_num+k4_num+2;
		USART5_TX_BUF[3]=0x01;
		USART5_TX_BUF[4]=0x01;
	
	
		for(j=0;j<5;j++)  
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,USART5_TX_BUF[j]);
			}
		
		for(j=0;j<k2_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,PM25_ValueFile[j]);
			 }
	    for(j=0;j<k3_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,PM25_Value_temp[j]);
			}
		for(j=0;j<k4_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,danweiFile3[j]);
			}
	}


}
char textFile_9[]= "[m3][v10]正在建立连接，请您稍候！";
void USART5_TX_State2(void)	   //
{
	u8 j;
	u16 k1_num;
//    char Temperature_temp[2],Humidity_temp[2],VOC_Data_temp[4],PM25_Value_temp[3],CO_Data_temp[3],CO2_Data_temp[4];
//	OLED_curPos
	 //Temperature_temp=StrToHex(Temperature);
	 

	 sound_C=1;

		k1_num=strlen(textFile_9);
	

		USART5_TX_BUF[0]=0xFD;
		USART5_TX_BUF[1]=0x00;
		USART5_TX_BUF[2]=k1_num+2;
		USART5_TX_BUF[3]=0x01;
		USART5_TX_BUF[4]=0x01;
	
	
		for(j=0;j<5;j++)  
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,USART5_TX_BUF[j]);
			}


	    for(j=0;j<k1_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,textFile_9[j]);
			}

}
char textFile_10[]= "[m3][v10]欢迎使用空气感知，请检查路由器状态，或帮我配置联网！";
void USART5_TX_State3(void)	   //
{
	u8 j;
	u16 k1_num;
//    char Temperature_temp[2],Humidity_temp[2],VOC_Data_temp[4],PM25_Value_temp[3],CO_Data_temp[3],CO2_Data_temp[4];
//	OLED_curPos
	 //Temperature_temp=StrToHex(Temperature);
	 

	 sound_C=1;

		k1_num=strlen(textFile_10);
	

		USART5_TX_BUF[0]=0xFD;
		USART5_TX_BUF[1]=0x00;
		USART5_TX_BUF[2]=k1_num+2;
		USART5_TX_BUF[3]=0x01;
		USART5_TX_BUF[4]=0x01;
	
	
		for(j=0;j<5;j++)  
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,USART5_TX_BUF[j]);
			}


	    for(j=0;j<k1_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,textFile_10[j]);
			}

}
char textFile_11[]= "[m3][v10]网络连接异常！请检查您的路由器设置！";
void USART5_TX_State4(void)	   //
{
	u8 j;
	u16 k1_num;
//    char Temperature_temp[2],Humidity_temp[2],VOC_Data_temp[4],PM25_Value_temp[3],CO_Data_temp[3],CO2_Data_temp[4];
//	OLED_curPos
	 //Temperature_temp=StrToHex(Temperature);
	 

	 sound_C=1;

		k1_num=strlen(textFile_11);
	

		USART5_TX_BUF[0]=0xFD;
		USART5_TX_BUF[1]=0x00;
		USART5_TX_BUF[2]=k1_num+2;
		USART5_TX_BUF[3]=0x01;
		USART5_TX_BUF[4]=0x01;
	
	
		for(j=0;j<5;j++)  
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,USART5_TX_BUF[j]);
			}


	    for(j=0;j<k1_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,textFile_11[j]);
			}

}
char textFile_12[]= "[m3][v10]无法连接到Wi-Fi，请检查您的路由器设置并确保密码输入正确！";
void USART5_TX_State5(void)	   //
{
	u8 j;
	u16 k1_num;
//    char Temperature_temp[2],Humidity_temp[2],VOC_Data_temp[4],PM25_Value_temp[3],CO_Data_temp[3],CO2_Data_temp[4];
//	OLED_curPos
	 //Temperature_temp=StrToHex(Temperature);
	 

	 sound_C=1;

		k1_num=strlen(textFile_12);
	

		USART5_TX_BUF[0]=0xFD;
		USART5_TX_BUF[1]=0x00;
		USART5_TX_BUF[2]=k1_num+2;
		USART5_TX_BUF[3]=0x01;
		USART5_TX_BUF[4]=0x01;
	
	
		for(j=0;j<5;j++)  
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,USART5_TX_BUF[j]);
			}


	    for(j=0;j<k1_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,textFile_12[j]);
			}

}
 //正在执行来自客户端的检测指令
char textFile_13[]= "[m3][v10]正在执行来自客户端的指令！";
void USART5_TX_State6(void)	   //
{
	u8 j;
	u16 k1_num;
//    char Temperature_temp[2],Humidity_temp[2],VOC_Data_temp[4],PM25_Value_temp[3],CO_Data_temp[3],CO2_Data_temp[4];
//	OLED_curPos
	 //Temperature_temp=StrToHex(Temperature);
	   	Temperature_display=Temperature;
		Humidity_display=Humidity;
	    CO_Data_display=CO_Data;
	    CO2_Data_display=CO2_Data;
	    VOC_Data_display=VOC_Data ;
	    PM25_Value_display=PM25_Value_1;
	 sound_C=1;

		k1_num=strlen(textFile_13);
	

		USART5_TX_BUF[0]=0xFD;
		USART5_TX_BUF[1]=0x00;
		USART5_TX_BUF[2]=k1_num+2;
		USART5_TX_BUF[3]=0x01;
		USART5_TX_BUF[4]=0x01;
	
	
		for(j=0;j<5;j++)  
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,USART5_TX_BUF[j]);
			}


	    for(j=0;j<k1_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,textFile_13[j]);
			}

}
char textFile_14[]= "显示板端子正常,传感器端子正常";
char textFile_15[]= "显示板端子正常,传感器端子异常";
char textFile_16[]= "显示板端子异常,传感器端子正常";
char textFile_17[]= "显示板端子异常,传感器端子异常";
char textFile_18[]= "弗莱时正常,RTC正常";
char textFile_19[]= "弗莱时正常,RTC异常";
char textFile_20[]= "弗莱时异常,RTC正常";
char textFile_21[]= "弗莱时异常,RTC异常";
void USART5_TX_ceshi_1(void)	   //
{
	u8 j;
	u16 k1_num;
//    char Temperature_temp[2],Humidity_temp[2],VOC_Data_temp[4],PM25_Value_temp[3],CO_Data_temp[3],CO2_Data_temp[4];
//	OLED_curPos
	 //Temperature_temp=StrToHex(Temperature);
	 

	 sound_C=1;
		if((VNCL_ceshi==1)&&(senser_ceshi==1))
		   	k1_num=strlen(textFile_14);	
		else if((VNCL_ceshi==1)&&(senser_ceshi==0))
		   	k1_num=strlen(textFile_15);
		else if((VNCL_ceshi==0)&&(senser_ceshi==1))
		   	k1_num=strlen(textFile_16);
		else if((VNCL_ceshi==0)&&(senser_ceshi==0))
		   	k1_num=strlen(textFile_17);

//		if((flash_ceshi==1)&&(rtc_ceshi==1))
//		   	k2_num=strlen(textFile_18);	
//		else if((flash_ceshi==1)&&(rtc_ceshi==0))
//		   	k2_num=strlen(textFile_19);
//		else if((flash_ceshi==0)&&(rtc_ceshi==1))
//		   	k2_num=strlen(textFile_20);
//		else if((flash_ceshi==0)&&(rtc_ceshi==0))
//		   	k2_num=strlen(textFile_21);

		USART5_TX_BUF[0]=0xFD;
		USART5_TX_BUF[1]=0x00;
		USART5_TX_BUF[2]=k1_num+2;
		USART5_TX_BUF[3]=0x01;
		USART5_TX_BUF[4]=0x01;
	
	
		for(j=0;j<5;j++)  
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,USART5_TX_BUF[j]);
			}


	    for(j=0;j<k1_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				{
					if((VNCL_ceshi==1)&&(senser_ceshi==1))
					   	   USART_SendData(UART5,textFile_14[j]);
					else if((VNCL_ceshi==1)&&(senser_ceshi==0))
					   	   USART_SendData(UART5,textFile_15[j]);
					else if((VNCL_ceshi==0)&&(senser_ceshi==1))
					   	   USART_SendData(UART5,textFile_16[j]);
					else if((VNCL_ceshi==0)&&(senser_ceshi==0))
					   	   	USART_SendData(UART5,textFile_17[j]);
				}
			}
//		for(j=0;j<k2_num;j++)  //return ACK
//			{
//				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
//				{
//					if((VNCL_ceshi==1)&&(senser_ceshi==1))
//					   	   USART_SendData(UART5,textFile_18[j]);
//					else if((VNCL_ceshi==1)&&(senser_ceshi==0))
//					   	   USART_SendData(UART5,textFile_19[j]);
//					else if((VNCL_ceshi==0)&&(senser_ceshi==1))
//					   	   USART_SendData(UART5,textFile_20[j]);
//					else if((VNCL_ceshi==0)&&(senser_ceshi==0))
//					   	   	USART_SendData(UART5,textFile_21[j]);
//				}
//			}
			while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
			USART_SendData(USART1,0x01);
			while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
			USART_SendData(USART1,0x02);
			while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
			USART_SendData(USART1,0x03);
			while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
			USART_SendData(USART1,0x04);
			while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
			USART_SendData(USART1,0x05);

}

//flash正常"，"RTC正常flash异常"，"RTC异常
void USART5_TX_ceshi_2(void)	   //
{
	u8 j;
	u16 k1_num;
//    char Temperature_temp[2],Humidity_temp[2],VOC_Data_temp[4],PM25_Value_temp[3],CO_Data_temp[3],CO2_Data_temp[4];
//	OLED_curPos
	 //Temperature_temp=StrToHex(Temperature);
	 
	
	 sound_C=1;
		if((flash_ceshi==1)&&(rtc_ceshi==1))
		   	k1_num=strlen(textFile_18);	
		else if((flash_ceshi==1)&&(rtc_ceshi==0))
		   	k1_num=strlen(textFile_19);
		else if((flash_ceshi==0)&&(rtc_ceshi==1))
		   	k1_num=strlen(textFile_20);
		else if((flash_ceshi==0)&&(rtc_ceshi==0))
		   	k1_num=strlen(textFile_21);
	

		USART5_TX_BUF[0]=0xFD;
		USART5_TX_BUF[1]=0x00;
		USART5_TX_BUF[2]=k1_num+2;
		USART5_TX_BUF[3]=0x01;
		USART5_TX_BUF[4]=0x01;
	
	
		for(j=0;j<5;j++)  
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				USART_SendData(UART5,USART5_TX_BUF[j]);
			}


	    for(j=0;j<k1_num;j++)  //return ACK
			{
				while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				{
					if((flash_ceshi==1)&&(rtc_ceshi==1))
					   	   USART_SendData(UART5,textFile_18[j]);
					else if((flash_ceshi==1)&&(rtc_ceshi==0))
					   	   USART_SendData(UART5,textFile_19[j]);
					else if((flash_ceshi==0)&&(rtc_ceshi==1))
					   	   USART_SendData(UART5,textFile_20[j]);
					else if((flash_ceshi==0)&&(rtc_ceshi==0))
					   	   	USART_SendData(UART5,textFile_21[j]);
				}
			}


}
char timeiFile[]= "现在时间";
char timeiFile_10_fen[]= "一十";
char danweiFile4[]= "点 ";
char danweiFile5[]= "零 ";
void time_sound(void)
{
   	u8 j; 
	u16 k1_num,k2_num,k3_num,k4_num,k5_num,k6_num;
    char hour_temp[2],min_temp[2] ;
	 RTC_Set_Time3=Timer;

	  sound_C=1;
		
	    sprintf(hour_temp,"%2d",RTC_Set_Time3.hour); 
	sprintf(min_temp,"%02d",RTC_Set_Time3.min);
	
	  		if(RTC_Set_Time3.min==0)
 		{
		    k6_num=strlen(textFile_1);
			k1_num=strlen(timeiFile);
			k2_num=strlen(hour_temp);
			k3_num=strlen(danweiFile4);
		//	k4_num=strlen(danweiFile5);
		//	k5_num=strlen(min_temp);
			USART5_TX_BUF[0]=0xFD;
			USART5_TX_BUF[1]=0x00;
			USART5_TX_BUF[2]=k1_num+k2_num+k3_num+k6_num+2;
			USART5_TX_BUF[3]=0x01;
			USART5_TX_BUF[4]=0x01;
		
		
			for(j=0;j<5;j++)  
				{
					while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,USART5_TX_BUF[j]);
				}
	    	for(j=0;j<k6_num;j++)  
			{
			     while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				 {
			       if(RTC_Set_Time3.hour<9)
				    USART_SendData(UART5,textFile_1[j]);
					else if(RTC_Set_Time3.hour<12)
				       USART_SendData(UART5,textFile_2[j]);
					   	else if(RTC_Set_Time3.hour<15)
				       USART_SendData(UART5,textFile_3[j]);
					   	else if(RTC_Set_Time3.hour<18)
				       USART_SendData(UART5,textFile_4[j]);
					   	else 
				        USART_SendData(UART5,textFile_5[j]);
				}
			}

			for(j=0;j<k1_num;j++)  
				{
					while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,timeiFile[j]);
				}
			for(j=0;j<k2_num;j++)  
				{
					while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,hour_temp[j]);
				}
			for(j=0;j<k3_num;j++)  //return ACK
				{
					while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
						USART_SendData(UART5,danweiFile4[j]);
				 }
		    
		 }
		else if(RTC_Set_Time3.min<10)
 		{
		    k6_num=strlen(textFile_1);
			k1_num=strlen(timeiFile);
			k2_num=strlen(hour_temp);
			k3_num=strlen(danweiFile4);
			k4_num=strlen(danweiFile5);
			k5_num=strlen(min_temp);
			USART5_TX_BUF[0]=0xFD;
			USART5_TX_BUF[1]=0x00;
			USART5_TX_BUF[2]=k1_num+k2_num+k3_num+k4_num+k5_num+k6_num+2;
			USART5_TX_BUF[3]=0x01;
			USART5_TX_BUF[4]=0x01;
		
		
			for(j=0;j<5;j++)  
				{
					while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,USART5_TX_BUF[j]);
				}
		for(j=0;j<k6_num;j++)  
			{
			     while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				 {
			       if(RTC_Set_Time3.hour<9)
				    USART_SendData(UART5,textFile_1[j]);
					else if(RTC_Set_Time3.hour<12)
				       USART_SendData(UART5,textFile_2[j]);
					   	else if(RTC_Set_Time3.hour<15)
				       USART_SendData(UART5,textFile_3[j]);
					   	else if(RTC_Set_Time3.hour<18)
				       USART_SendData(UART5,textFile_4[j]);
					   	else 
				        USART_SendData(UART5,textFile_5[j]);
				}
			}

			for(j=0;j<k1_num;j++)  
				{
					while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,timeiFile[j]);
				}
			for(j=0;j<k2_num;j++)  
				{
					while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,hour_temp[j]);
				}
			for(j=0;j<k3_num;j++)  //return ACK
				{
					while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
						USART_SendData(UART5,danweiFile4[j]);
				 }
		    for(j=0;j<k4_num;j++)  //return ACK
				{
					while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,danweiFile5[j]);
				} 
			for(j=0;j<k5_num;j++)  //return ACK
				{
					while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,min_temp[j]);
				}
		 }
		 else if(RTC_Set_Time3.min==10)
			 {
					k6_num=strlen(textFile_1);
				  	k1_num=strlen(timeiFile);
					k2_num=strlen(hour_temp);
					k3_num=strlen(danweiFile4);
					k4_num=strlen(timeiFile_10_fen);
			
					USART5_TX_BUF[0]=0xFD;
					USART5_TX_BUF[1]=0x00;
					USART5_TX_BUF[2]=k1_num+k2_num+k3_num+k4_num+k6_num+2;
					USART5_TX_BUF[3]=0x01;
					USART5_TX_BUF[4]=0x01;
				
				
					for(j=0;j<5;j++)  
						{
							while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
							USART_SendData(UART5,USART5_TX_BUF[j]);
						}
								for(j=0;j<k6_num;j++)  
					{
					     while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
						 {
					       if(RTC_Set_Time3.hour<9)
						    USART_SendData(UART5,textFile_1[j]);
							else if(RTC_Set_Time3.hour<12)
						       USART_SendData(UART5,textFile_2[j]);
							   	else if(RTC_Set_Time3.hour<15)
						       USART_SendData(UART5,textFile_3[j]);
							   	else if(RTC_Set_Time3.hour<18)
						       USART_SendData(UART5,textFile_4[j]);
							   	else 
						        USART_SendData(UART5,textFile_5[j]);
						}
					}
					for(j=0;j<k1_num;j++)  
						{
							while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
							USART_SendData(UART5,timeiFile[j]);
						}
					for(j=0;j<k2_num;j++)  
						{
							while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
							USART_SendData(UART5,hour_temp[j]);
						}
					for(j=0;j<k3_num;j++)  //return ACK
						{
							while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
								USART_SendData(UART5,danweiFile4[j]);
						 }
				    for(j=0;j<k4_num;j++)  //return ACK
						{
							while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
							USART_SendData(UART5,timeiFile_10_fen[j]);
						}
			 
			 
			 
			 }

		 else
		 {
		   k6_num=strlen(textFile_1);
		  	k1_num=strlen(timeiFile);
			k2_num=strlen(hour_temp);
			k3_num=strlen(danweiFile4);
			k4_num=strlen(min_temp);
	
			USART5_TX_BUF[0]=0xFD;
			USART5_TX_BUF[1]=0x00;
			USART5_TX_BUF[2]=k1_num+k2_num+k3_num+k4_num+k6_num+2;
			USART5_TX_BUF[3]=0x01;
			USART5_TX_BUF[4]=0x01;
		
		
			for(j=0;j<5;j++)  
				{
					while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,USART5_TX_BUF[j]);
				}
						for(j=0;j<k6_num;j++)  
			{
			     while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
				 {
			       if(RTC_Set_Time3.hour<9)
				    USART_SendData(UART5,textFile_1[j]);
					else if(RTC_Set_Time3.hour<12)
				       USART_SendData(UART5,textFile_2[j]);
					   	else if(RTC_Set_Time3.hour<15)
				       USART_SendData(UART5,textFile_3[j]);
					   	else if(RTC_Set_Time3.hour<18)
				       USART_SendData(UART5,textFile_4[j]);
					   	else 
				        USART_SendData(UART5,textFile_5[j]);
				}
			}
			for(j=0;j<k1_num;j++)  
				{
					while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,timeiFile[j]);
				}
			for(j=0;j<k2_num;j++)  
				{
					while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,hour_temp[j]);
				}
			for(j=0;j<k3_num;j++)  //return ACK
				{
					while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
						USART_SendData(UART5,danweiFile4[j]);
				 }
		    for(j=0;j<k4_num;j++)  //return ACK
				{
					while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
					USART_SendData(UART5,min_temp[j]);
				}
		 
		 
		 }
}
 
 void USART2_TX_State(void)	   //用于向CO传感器发送命令
{
	u8 j;	
	//if(USART_TX_Data_Flag){
	//USART3_TX_BUF[0]=0xFE;
	//USART3_TX_BUF[1]=0x04;
	//USART3_TX_BUF[2]=0x00;
	//USART3_TX_BUF[3]=0x03;
	//USART3_TX_BUF[4]=0x00;
	//USART3_TX_BUF[5]=0x01;
	//USART3_TX_BUF[6]=0xD5;
	//USART3_TX_BUF[7]=0xC5;
	//010300000001840A
	USART2_TX_BUF[0]=0x01;
	USART2_TX_BUF[1]=0x03;
	USART2_TX_BUF[2]=0x00;
	USART2_TX_BUF[3]=0x00;
	USART2_TX_BUF[4]=0x00;
	USART2_TX_BUF[5]=0x01;
	USART2_TX_BUF[6]=0x84;
	USART2_TX_BUF[7]=0x0A;
	//USART3_TX_BUF={0xFE,0x04,0x00,0x03,0x00,0x01,0xd5,0xc5};
	//USART3_TX_BUF={0x68,0x04,0x00,0x03,0x00,0x01,0xc8,0xf3};
	 
	//for(j=0;j<USART_TX_BUF[2]+3;j++)  //return ACK
	for(j=0;j<8;j++)  //return ACK
	{
		while( USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET );
		USART_SendData(USART2,USART2_TX_BUF[j]);
	}
	if(USART2_TX_Data_Conter++ > 9)  //  连续发送10次没有接受到应答信号 则停止发送 启动联机模式
	{
	//	USART_TX_Data_Flag=0;
		USART2_TX_Data_Conter=0;
	//	USART_TX_Connect_Flag=1;
	//	Connect_Flag=0;
	}		
//	USART_TX_Data_Time_Conter	= 3; //5-3=2  2*2=4S  不成功则每隔4秒钟发送一次
	//USART_TX_Data_Time_Conter	= 0; //5-3=2  2*2=4S  不成功则每隔4秒钟发送一次
//}
} 

void Mac_TX_State(void)	   //
{
	u8 j;	

	Mac_TX_BUF[0]=0x03;
	Mac_TX_BUF[1]=0x0C;
	Mac_TX_BUF[2]=0x04;
	Mac_TX_BUF[3]=0x00;
	Mac_TX_BUF[4]=0x00;
	Mac_TX_BUF[5]=0x11;
	Mac_TX_BUF[6]=0xB3;
	Mac_TX_BUF[7]=0x09;

//  	Mac_TX_BUF[0]=0x04;
//	Mac_TX_BUF[1]=0x0C;
//	Mac_TX_BUF[2]=0x04;
//	Mac_TX_BUF[3]=0x00;
//	Mac_TX_BUF[4]=0x10;
//	Mac_TX_BUF[5]=0x00;
//	Mac_TX_BUF[6]=0x14;
//	Mac_TX_BUF[7]=0x4c;



	for(j=0;j<8;j++)  //return ACK
	{
		while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
		USART_SendData(USART1,Mac_TX_BUF[j]);
	}

}
void Internet_TX_State(void)	   
{
	u8 j;	



    Internet_TX_BUF[0]=0x04;
	Internet_TX_BUF[1]=0x0C;
	Internet_TX_BUF[2]=0x04;
	Internet_TX_BUF[3]=0x00;
	Internet_TX_BUF[4]=0x10;
	Internet_TX_BUF[5]=0x00;
	Internet_TX_BUF[6]=0x14;
	Internet_TX_BUF[7]=0x4c;


	for(j=0;j<8;j++)  //return ACK
	{
		while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
		USART_SendData(USART1,Internet_TX_BUF[j]);
	}

} 
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
 void USART1_TX_State(void)	   //wifi
{
//	  u8 aa,bb;
	u8 j, ff;
	  //u16 ff;
//	   int8_t ff= Temperature - 30;
//		u16 cc,dd;   
	//	char buf2[16];
	RTC_Set_Time2=Timer;
			
//	float VOC_Data_1=0.01 ;	
	//if(USART_TX_Data_Flag){
	//USART3_TX_BUF[0]=0xFE;
	//USART3_TX_BUF[1]=0x04;
	//USART3_TX_BUF[2]=0x00;
	//USART3_TX_BUF[3]=0x03;
	//USART3_TX_BUF[4]=0x00;
	//USART3_TX_BUF[5]=0x01;
	//USART3_TX_BUF[6]=0xD5;
	//USART3_TX_BUF[7]=0xC5;
	//010300000001840A
	//消息头
	USART1_TX_BUF[0]=0x00;
	USART1_TX_BUF[1]=0x00;
	USART1_TX_BUF[2]=0x00;
	USART1_TX_BUF[3]=0x27;
	USART1_TX_BUF[4]=0x01;
	USART1_TX_BUF[5]=0x02;
  //消息头体	DeviceID

	USART1_TX_BUF[6]=0x02;
	USART1_TX_BUF[7]=0x01;
	USART1_TX_BUF[8]=0x01;

	USART1_TX_BUF[9]=0x0D;
	USART1_TX_BUF[10]=0x7F;
	USART1_TX_BUF[11]=0x07;
	USART1_TX_BUF[12]=0x18;

	USART1_TX_BUF[13]=0x00;		
	USART1_TX_BUF[14]=0x06;

	USART1_TX_BUF[15]=0x10;
	USART1_TX_BUF[16]=0x10;

	USART1_TX_BUF[17]=0x00;
	USART1_TX_BUF[18]=0x00;
	 //	六合一设备 检测时间YYMMDDHHMMSS湿度、温度、PM2.5、CO2、CO、VOC。

 //   dd= itoa_1(RTC_Set_Time2.w_year, cc,  10);
	USART1_TX_BUF[19]=RTC_Set_Time2.w_year>>8&0xFF;
	USART1_TX_BUF[20]=RTC_Set_Time2.w_year&0xFF;

//	aa= itoa_2(RTC_Set_Time2.w_month, bb,  10);
	USART1_TX_BUF[21]=RTC_Set_Time2.w_month;

//	aa= itoa_2(RTC_Set_Time2.w_date, bb,  10);
	USART1_TX_BUF[22]=RTC_Set_Time2.w_date;

//	aa= itoa_2(RTC_Set_Time2.hour, bb,  10);
    USART1_TX_BUF[23]=RTC_Set_Time2.hour;

//	aa= itoa_2(RTC_Set_Time2.min, bb,  10);
	USART1_TX_BUF[24]=RTC_Set_Time2.min;

//	aa= itoa_2(RTC_Set_Time2.sec, bb,  10);	
	USART1_TX_BUF[25]=RTC_Set_Time2.sec;

//	aa= itoa_2(Humidity, bb,  10);	
	//USART1_TX_BUF[27]=Humidity;
	 	USART1_TX_BUF[26]=Humidity>>8&0xFF;
	USART1_TX_BUF[27]=Humidity&0xFF;
//USART1_TX_BUF[28]=(int)ltoa(Temperature,buf1,10) ;
//USART1_TX_BUF[28]=(int)itoa(Temperature, buf, 10);	
//	
//	USART1_TX_BUF[21]=Temperature&0xFF;

//INT 	Temperature
	   ff=Temperature+20;
	//	 USART1_TX_BUF[29]=ff ;
		USART1_TX_BUF[28]=ff>>8&0xFF;
	USART1_TX_BUF[29]=ff&0xFF;
//	 aa= itoa_2(ff, bb,  10);	

 //   dd= itoa_1(CO2_Data, cc,  10);
	USART1_TX_BUF[30]=CO2_Data>>8&0xFF;
	USART1_TX_BUF[31]=CO2_Data&0xFF;

//	dd= itoa_1(PM25_Value_1, cc,  10);
	USART1_TX_BUF[32]=PM25_Value_1>>8&0xFF;
	USART1_TX_BUF[33]=PM25_Value_1&0xFF;

//	dd= itoa_1(CO_Data, cc,  10);
	USART1_TX_BUF[34]=CO_Data>>8&0xFF;
	USART1_TX_BUF[35]=CO_Data&0xFF;
	
//	dd= itoa_1(VOC_Data, cc,  10);
	USART1_TX_BUF[36]=(u16)VOC_Data>>8&0xFF;
	USART1_TX_BUF[37]=(u16)VOC_Data&0xFF;	 
   



	for(j=0;j<38;j++)  //return ACK
	{
//	   if((j!=28)&&(j!=37))
		{
			while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
			USART_SendData(USART1,USART1_TX_BUF[j]);
		 }

			
	}


//}
}

 u8 send_msg_to_wifi(u8 cmd_data,u8* p_data,u32 i_data_len)
{
	
	u32 i_len = 0;
	 	u32 j = 0;
		u16 num=0;
		num=i_data_len/18;
//	memset(wifi_msg_temp,0x00,256);
		//	字节数
	i_len = 22 + i_data_len;
    wifi_msg_temp[0] = 0x55;//头
	//长度


	wifi_msg_temp[3] = 0x01;//消息版本号
	wifi_msg_temp[4] = cmd_data;//消息类型
	 //消息头体	DeviceID
//	 1	设备检测类型	01	02	03	04	05
//		水	气	土	声	风
//2	设备类型	01	02	03
//		室内设备	室外设备	数据采集器
//		02	分体机客厅	02	室外噪音		
//		03	分体机厨房	03			
//		04	分体机卧室	04			
//		05	分体机儿童房	05			
//4	生产时间	20150724	年月日
//5	当日生产序号	0100	同类产品当日生产序号
//6	软件版本号	10	V1.0
//7	硬件版本号	22	V2.2
//8	预留	0000	


//	wifi_msg_temp[6]=0x02;
//	wifi_msg_temp[7]=0x01;
//	wifi_msg_temp[8]=0x01;
//
//	wifi_msg_temp[9]=0x0D;
//	wifi_msg_temp[10]=0x7F;
//	wifi_msg_temp[11]=0x07;
//	wifi_msg_temp[12]=0x18;
//
//	wifi_msg_temp[13]=0x00;		
//	wifi_msg_temp[14]=0x05;
//
//	wifi_msg_temp[15]=0x10;
//	wifi_msg_temp[16]=0x10;
//
//	wifi_msg_temp[17]=0x00;
//	wifi_msg_temp[18]=0x00;


	//报送数据量
//	wifi_msg_temp[18]=(num>>8)&0xff;
	wifi_msg_temp[18]=num&0xff;
	//状态 "1:正常;2:VOC传感器超量程;3:PM2.5传感器通讯故障"。
	if((senser_th_flag==0)&&(senser_co_flag==0)&&(senser_voc_flag==0)&&(senser_co2_flag==0)&&(senser_pm_flag==0))
	 wifi_msg_temp[19]=0x02;
	 else
	  wifi_msg_temp[19]=0x07;
	 	 //	六合一设备 检测时间YYMMDDHHMMSS 
//	wifi_msg_temp[28]=RTC_Set_Time2.w_year>>8&0xFF;
//	wifi_msg_temp[29]=RTC_Set_Time2.w_year&0xFF;	 
//	wifi_msg_temp[30]=RTC_Set_Time2.w_month;	
//	wifi_msg_temp[31]=RTC_Set_Time2.w_date;	
//    wifi_msg_temp[32]=RTC_Set_Time2.hour;	
//	wifi_msg_temp[33]=RTC_Set_Time2.min;		
//	wifi_msg_temp[34]=RTC_Set_Time2.sec;
   	memcpy(wifi_msg_temp + 5,ID_msg,13);




	memcpy(wifi_msg_temp + 20,p_data,i_data_len);



	  if((100*VOC_Data)>=2000)
	    wifi_msg_temp[19]=0x04;
	  	
	if(CO_Data>=500)
	     wifi_msg_temp[19]=0x20;
	    	
	  if(CO2_Data>=10000)
	     wifi_msg_temp[19]=0x16;

	if((cmd_data==0x04)||(cmd_data==0x05)) //心跳	，校时
	  i_len=20;
	wifi_msg_temp[1] = ((i_len-3)>>8) & 0xff;
	wifi_msg_temp[2] = (i_len-3)&0xff;

	 wifi_msg_temp[i_len-2]=0;
	 wifi_msg_temp[i_len-1]=0xFF;
	 for(j=1;j<i_len-2;j++) 
	 {
	  wifi_msg_temp[i_len-2]+=wifi_msg_temp[j] ;
	 
	 }

	for(j=0;j<i_len;j++) 
	{
		{
			while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
			USART_SendData(USART1,wifi_msg_temp[j]);
		 }	
	}
	
	return i_len;	
}

 u8 send_msg_to_wifi_1(u8 cmd_data,u8* p_data,u32 i_data_len)
{
	
	u32 i_len = 0;
	 	u32 j = 0;
//	memset(wifi_msg_temp_1,0x00,256);
		//	字节数
	i_len = 21 + i_data_len;  //
	if(cmd_data==0x01)	//登录
		i_len=i_len-1;
	  else
		 wifi_msg_temp_1[i_len-3] = 1;//反馈
	//wifi_msg_temp_1[0] = 0x01;
	wifi_msg_temp_1[0] = 0x55;
	wifi_msg_temp_1[1] = ((i_len-3)>>8) & 0xff;
	wifi_msg_temp_1[2] = (i_len-3)&0xff;

	wifi_msg_temp_1[3] = 0x01;//消息版本号
	wifi_msg_temp_1[4] = cmd_data;//消息类型
		 //消息头体	DeviceID
//	 1	设备检测类型	01	02	03	04	05
//		水	气	土	声	风
//2	设备类型	01	02	03
//		室内设备	室外设备	数据采集器
//		02	分体机客厅	02	室外噪音		
//		03	分体机厨房	03			
//		04	分体机卧室	04			
//		05	分体机儿童房	05			
//4	生产时间	20150724	年月日
//5	当日生产序号	0100	同类产品当日生产序号
//6	软件版本号	10	V1.0
//7	硬件版本号	22	V2.2
//8	预留	0000	
	 

//	wifi_msg_temp_1[6]=0x02;
//	wifi_msg_temp_1[7]=0x01;
//	wifi_msg_temp_1[8]=0x01;
//
//	wifi_msg_temp_1[9]=0x0D;
//	wifi_msg_temp_1[10]=0x7F;
//	wifi_msg_temp_1[11]=0x07;
//	wifi_msg_temp_1[12]=0x18;
//
//	wifi_msg_temp_1[13]=0x00;		
//	wifi_msg_temp_1[14]=0x05;
//
//	wifi_msg_temp_1[15]=0x10;
//	wifi_msg_temp_1[16]=0x10;
//
//	wifi_msg_temp_1[17]=0x00;
//	wifi_msg_temp_1[18]=0x00;
	
//	 if(cmd_data==0x01)
//		i_len=i_len-1;
//	  else
//		 wifi_msg_temp_1[i_len-1] = 1;//反馈

	memcpy(wifi_msg_temp_1 + 5,ID_msg,13);

	memcpy(wifi_msg_temp_1 + 18,p_data,i_data_len);

	 wifi_msg_temp_1[i_len-2]=0;
	 wifi_msg_temp_1[i_len-1]=0xFF;
	 for(j=1;j<i_len-2;j++) 
	 {
	  wifi_msg_temp_1[i_len-2]+=wifi_msg_temp_1[j] ;
	 
	 }


	for(j=0;j<i_len;j++) 
	{
		{
			while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
			USART_SendData(USART1,wifi_msg_temp_1[j]);
		 }	
	}
	
	return i_len;	
}

u8 wifi_msg_temp_2[50];
u8  receive_msg_from_wifi()
{
	   	u8 i_len = 8,cmd=0,cmd_name=0;
//	 	u8 j = 0;
//  			if(USART1_RX_BUF_Counter>=7)		  //uart1 receive
//		{
//			if(USART1_RX_Timer > 2) 
//			{
				i_len = USART1_RX_BUF1[2];

				cmd = USART1_RX_BUF1[4];
				 cmd_name = USART1_RX_BUF1[24];
				 memcpy(wifi_msg_temp_2,USART1_RX_BUF1+18,6);

				  switch (cmd)
				  {
				    case 0x0B :
				      LOGIN_CMD_Flag=1;
					  RE_HEART_CMD_Flag=1;
					  LOGIN_CMD_Flag_num=120;
				      break;
					case 0x15 :
					  RE_DATA_CMD_Flag=1;
					  RE_HEART_CMD_Flag=1;
				      Pingji_Flag= USART1_RX_BUF1[18]; 	//评级
				      break;
				    case 0x29 :
				      RE_HEART_CMD_Flag=1;
				      break;
				    case 0x03 :
				      if(cmd_name==1)	  //实时报送
						{
						 test_imid=1;
					     send_msg_to_wifi_1(0x1F,wifi_msg_temp_2,6);
						 }
				      if(cmd_name==2)	 //上报频率调整
						  {	
						   re_frequency=(USART1_RX_BUF1[25]<<8&0xff00)+(USART1_RX_BUF1[26]&0xff);
						  // re_frequency=USART1_RX_BUF[26]&0xff;
						   	flash_read(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
							p_data_read[2]=re_frequency;
				             flash_write(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
						
						   send_msg_to_wifi_1(0x1F,wifi_msg_temp_2,6);
						  }
					   if(oled_Flag_1==0)
					     USART5_TX_State6();
				      break;
					 case 0x33 :
					  Set_time[0]=2000+USART1_RX_BUF1[18];
					  Set_time[1]=USART1_RX_BUF1[19];
					  Set_time[2]=USART1_RX_BUF1[20];
					  Set_time[3]=USART1_RX_BUF1[21];
					  Set_time[4]=USART1_RX_BUF1[22];
					  Set_time[5]=USART1_RX_BUF1[23];
				      RTC_Set(Set_time[0],Set_time[1],Set_time[2],Set_time[3],Set_time[4],Set_time[5]);
					  Set_time_Flag=1;
//					  RTC_Time_Set_flag=0;
				      break;
				    default:
				      break;
				  }



//				if(cmd==11)
//				 	 LOGIN_CMD_Flag=1;
//				if(cmd==41)
//				 	 HEART_CMD_Flag=0;
//
//				if((cmd==3)&&(cmd_name==1))	  //实时报送
//					  send_msg_to_wifi_1(0x31,wifi_msg_temp_2,i_len);
//				if((cmd==3)&&(cmd_name==2))	 //上报频率调整
//					  {	
//					   re_frequency=USART1_RX_BUF[34]<<8&0xff00+USART1_RX_BUF[35]&0xff;
//					   	flash_read(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
//						p_data_read[2]=re_frequency;
//			             flash_write(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
//					
//					   send_msg_to_wifi_1(0x32,wifi_msg_temp_2,i_len);
//					  }
			 
//				USART1_RX_Timer=0;
//				USART1_RX_BUF_Counter=0;
//			 }
//		}

	//	memset(USART1_RX_BUF,0x00,64);

	return i_len;	
	
}

void receive_mac_from_wifi(void)
{

char Mac_msg_temp[30];
 
 memcpy(Mac_msg_temp,USART1_RX_BUF,25);

  //if(Mac_msg_temp[1]==0x43)
   if(Mac_msg_temp[0]==0x03)
   {
//	   Mac_msg[0]= ((HexToAsc(Mac_msg_temp[1]))<<4)+HexToAsc(Mac_msg_temp[2]);
//	   Mac_msg[1]= ((HexToAsc(Mac_msg_temp[4]))<<4)+HexToAsc(Mac_msg_temp[5]);
//	   Mac_msg[2]= ((HexToAsc(Mac_msg_temp[7]))<<4)+HexToAsc(Mac_msg_temp[8]);
//	   Mac_msg[3]= ((HexToAsc(Mac_msg_temp[10]))<<4)+HexToAsc(Mac_msg_temp[11]);
//	   Mac_msg[4]= ((HexToAsc(Mac_msg_temp[13]))<<4)+HexToAsc(Mac_msg_temp[14]);
//	   Mac_msg[5]= ((HexToAsc(Mac_msg_temp[16]))<<4)+HexToAsc(Mac_msg_temp[17]);
       Mac_msg[0]= ((HexToAsc(Mac_msg_temp[4]))<<4)+HexToAsc(Mac_msg_temp[5]);
	   Mac_msg[1]= ((HexToAsc(Mac_msg_temp[7]))<<4)+HexToAsc(Mac_msg_temp[8]);
	   Mac_msg[2]= ((HexToAsc(Mac_msg_temp[10]))<<4)+HexToAsc(Mac_msg_temp[11]);
	   Mac_msg[3]= ((HexToAsc(Mac_msg_temp[13]))<<4)+HexToAsc(Mac_msg_temp[14]);
	   Mac_msg[4]= ((HexToAsc(Mac_msg_temp[16]))<<4)+HexToAsc(Mac_msg_temp[17]);
	   Mac_msg[5]= ((HexToAsc(Mac_msg_temp[19]))<<4)+HexToAsc(Mac_msg_temp[20]);
	   Set_mac_Flag=1;
  }
  else if ((Mac_msg_temp[0]==0x04)&&(Mac_msg_temp[4]==0x00))
       	 {
		 internet=1;	//失败
		 internet__send_flag=1;
		 }
	 else if ((Mac_msg_temp[0]==0x04)&&(Mac_msg_temp[4]==0x11))
	     {
		  internet=2;  //、、成功
		  internet__send_flag=1;
		  }
}
u8 usb_msg_temp[256] ;
u8 send_msg_to_usb(u8 cmd_data,u8* p_data,u32 i_data_len)
{
	
	u32 i_len = 0;
	 	u32 j = 0;
//	memset(wifi_msg_temp_1,0x00,256);
		//	字节数
	if((cmd_data==0x11)||(cmd_data==0x08))
	   i_len = 6+i_data_len+12;   //31
	else
	   i_len = 8 + i_data_len;  // 21
	
	usb_msg_temp[0] = 0x55;
	usb_msg_temp[1] = ((i_len)>>8) & 0xff;
	usb_msg_temp[2] = (i_len)&0xff;

	usb_msg_temp[3] = cmd_data;	
	 memcpy(usb_msg_temp + 4,ID_msg,13);
//	if(cmd_data==0x11)
//	{
//	  memcpy(usb_msg_temp + 4,ID_msg,13);
//      memcpy(usb_msg_temp + 17,p_data+6,i_data_len-6);
//	}
//	else
//	  memcpy(usb_msg_temp + 4,p_data,i_data_len);

 	  switch (cmd_data)
	  {
	  	case 0x01 :
		   usb_msg_temp[17]=0;
           usb_msg_temp[18]=0;
		break;
		case 0x02 :
		case 0x12 :
		   usb_msg_temp[17]=((Temperature)>>8)&0xFF;
           usb_msg_temp[18]=(Temperature)&0xFF;
		break;
	    case 0x03 :
		case 0x13 :
		   usb_msg_temp[17]=(Humidity>>8)&0xFF;
           usb_msg_temp[18]=Humidity&0xFF;
		break;
		case 0x04 :
		case 0x14 :
		   usb_msg_temp[17]=((u16)(100*VOC_Data)>>8)&0xFF;
           usb_msg_temp[18]=(u16)(100*VOC_Data)&0xFF;
		break;
		case 0x05 :
		case 0x15 :
		   usb_msg_temp[17]=(PM25_Value_1>>8)&0xFF;
           usb_msg_temp[18]=PM25_Value_1&0xFF;
		break;
		case 0x06 :
		case 0x16 :
		   usb_msg_temp[17]=(CO_Data>>8)&0xFF;
           usb_msg_temp[18]=CO_Data&0xFF;
		break;
		case 0x07 :
		case 0x17 :
		   usb_msg_temp[17]=(CO2_Data>>8)&0xFF;
           usb_msg_temp[18]=CO2_Data&0xFF;
		break;
		case 0x10 :
		   usb_msg_temp[17]=0;
           usb_msg_temp[18]=0;
		     break;
		case 0x08 :
		case 0x11 :
		   usb_msg_temp[17]=(Humidity>>8)&0xFF;
           usb_msg_temp[18]=Humidity&0xFF;
		   usb_msg_temp[19]=((Temperature+50)>>8)&0xFF;
           usb_msg_temp[20]=(Temperature+50)&0xFF;
		   usb_msg_temp[21]=((u16)(100*VOC_Data)>>8)&0xFF;
           usb_msg_temp[22]=(u16)(100*VOC_Data)&0xFF;
		   usb_msg_temp[23]=(PM25_Value_1>>8)&0xFF;
           usb_msg_temp[24]=PM25_Value_1&0xFF;
		   usb_msg_temp[25]=(CO_Data>>8)&0xFF;
           usb_msg_temp[26]=CO_Data&0xFF;
		   usb_msg_temp[27]=(CO2_Data>>8)&0xFF;
           usb_msg_temp[28]=CO2_Data&0xFF;
		    break;
		 default:
	      break;
		}



	 usb_msg_temp[i_len-2]=Jiaoyan_Data_Vs;

	 usb_msg_temp[i_len-1]=0xFF;
//	 for(j=1;j<i_len-2;j++) 
//	 {
//	  usb_msg_temp[i_len-2]+=usb_msg_temp[j] ;
//	 
//	 }


	for(j=0;j<i_len;j++) 
	{
		{
			while( USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET );
			USART_SendData(USART2,usb_msg_temp[j]);
		 }	
	}
	
	return i_len;	
}
void receive_msg_from_usb(void)
{
  u8 cmd=0;
  u16 cmd_name=0;
  u8 cmd_jianya=0;
  u8 cmd_number=0;
//   double  dataa111_name=0;
//	 	u8 j = 0;
//  			if(USART1_RX_BUF_Counter>=7)		  //uart1 receive
//		{
//			if(USART1_RX_Timer > 2) 
//			{
// 				i_len = USART2_RX_BUF[2];

		cmd = USART2_RX_BUF[3];

		 cmd_name = ( (USART2_RX_BUF[4]<<8 &0xff00 )+ (USART2_RX_BUF[5]&0xff));
		 //memcpy(wifi_msg_temp_2,USART1_RX_BUF+18,6);
		   
		   cmd_number=( (USART2_RX_BUF[1]<<8 &0xff00 )+ (USART2_RX_BUF[2]&0xff));
		   cmd_jianya= USART2_RX_BUF[cmd_number-2] ;

		  if(cmd_jianya==0)
		     {
				  switch (cmd)
				  {
				    case 0x01 :
				      ID_information();
				      break;
					case 0x2 :
						if(cmd_name<=50)
						   {
							  Temperature= cmd_name;
							  Temperature_Vs=100+wendu/2-cmd_name/2;
							  flash_read(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
							  p_data_read[17] =Temperature_Vs&0xffff;
							  flash_write(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
							  Jiaoyan_Data_Vs=0x01;
							  Usb_led_Vs=0x01; 
 							}
						else
						Jiaoyan_Data_Vs=0x00;
						 send_msg_to_usb(0x02,ID_msg,13);	
				      break;
				    case 0x3 :
						if(cmd_name<=100)
						   {
	 						  Humidity= cmd_name;
							  Humidity_Vs=100+shidu/2-cmd_name/2;						 
							  flash_read(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
							  p_data_read[18] =Humidity_Vs&0xffff;
							  flash_write(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
							  Jiaoyan_Data_Vs=0x01;
							  Usb_led_Vs=0x01; 
					  	  }
						else
						Jiaoyan_Data_Vs=0x00;
						send_msg_to_usb(0x03,ID_msg,13);				   
				      break;
				    case 0x04 :
						if(cmd_name<=200)
						   {
							   VOC_Data= (float)cmd_name/100;
							  VOC_Data_Vs=200+(u16)VOC_Data_Vo-cmd_name;						 
							  flash_read(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
							  p_data_read[20] =VOC_Data_Vs&0xffff;
							  flash_write(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
							  Jiaoyan_Data_Vs=0x01;
							  Usb_led_Vs=0x01; 
						  }
						else
						Jiaoyan_Data_Vs=0x00;
						send_msg_to_usb(0x04,ID_msg,13);
				      break;
					case 0x05 :
						if(cmd_name<=800)
						   {
						      PM25_Value_1=cmd_name;
							  //PM25_Vs=PM25_Vo-cmd_name*100/12;
							  if(cmd_name<=6)
							    PM25_Vs=PM25_Vo-(cmd_name-1)*10	;
							  else
							    PM25_Vs=PM25_Vo-cmd_name*10	;
							//   PM25_Vs=0x454	;
							 // PM25_Vs=PM25_Vo-cmd_name*5/3;					 
							  flash_read(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
							  p_data_read[0] =PM25_Vs&0xffff;
							  flash_write(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
							  Jiaoyan_Data_Vs=0x01;
							  Usb_led_Vs=0x01; 
    					  }
						else
						Jiaoyan_Data_Vs=0x00;
						send_msg_to_usb(0x05,ID_msg,13);
				      break;
					case 0x06 :
						 if(cmd_name<=1000)
						   {
							  CO_Data= cmd_name;
						  	  CO_Data_Vs=500+CO_Data_Vo/2-cmd_name/2;						 
							  flash_read(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
							  p_data_read[19] =CO_Data_Vs&0xffff;
							  flash_write(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
							  Jiaoyan_Data_Vs=0x01;
							  Usb_led_Vs=0x01; 
						  }
						 else
						   Jiaoyan_Data_Vs=0x00;
						   send_msg_to_usb(0x06,ID_msg,13);	
				      break;
					case 0x07 :
						  // CO2_Data_Vs=CO2_Data_Vs-(cmd_name-CO2_Data);
						  if(cmd_name<=10000)
						   {
							  CO2_Data= cmd_name;
							  CO2_Data_Vs=10000+CO2_Data_Vo/2-cmd_name/2;						 
							  flash_read(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
							  p_data_read[16] =CO2_Data_Vs&0xffff;
							  flash_write(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
							  Jiaoyan_Data_Vs=0x01;
							  Usb_led_Vs=0x01; 
						  }
							 else
							  Jiaoyan_Data_Vs=0x00;	
							  send_msg_to_usb(0x07,ID_msg,13);
				      break;
					case 0x08 :						 
						  Temperature=wendu;
						  Humidity=shidu ;
						  VOC_Data=VOC_Data_Vo/100;
//						  PM25_Value_1=5;
						  CO_Data=CO_Data_Vo ;						  
						  CO2_Data=CO2_Data_Vo; 
					  	  Temperature_Vs=100 ;
						  Humidity_Vs=100 ;
						  VOC_Data_Vs=200 ;
//						  PM25_Vs =PM25_Vo-17;
						  CO_Data_Vs=500 ;						  
						  CO2_Data_Vs=10000 ;						  						 
						  flash_read(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
//						  p_data_read[0] =PM25_Vs&0xffff;
						  p_data_read[16] =CO2_Data_Vs&0xffff;
						  p_data_read[17] =Temperature_Vs&0xffff;
						  p_data_read[18] =Humidity_Vs&0xffff;
						  p_data_read[19] =CO_Data_Vs&0xffff;
						  p_data_read[20] =VOC_Data_Vs&0xffff;
						  flash_write(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
						  send_msg_to_usb(0x08,ID_msg,13);	
				      break;
					case 0x10 :
					     Jiaoyan_Data_Vs=0x01;
						 send_msg_to_usb(0x10,ID_msg,13);
				      break;
					case 0x11 :
					      Jiaoyan_Data_Vs=0x01;
						  send_msg_to_usb(0x11,ID_msg,13);
				      break;
					 case 0x12 :
					     Jiaoyan_Data_Vs=0x01;
						 send_msg_to_usb(0x12,ID_msg,13);
				      break;
					case 0x13 :
					      Jiaoyan_Data_Vs=0x01;
						  send_msg_to_usb(0x13,ID_msg,13);
				      break;					
					case 0x14 :
					     Jiaoyan_Data_Vs=0x01;
						 send_msg_to_usb(0x14,ID_msg,13);
				      break;
					case 0x15 :
					      Jiaoyan_Data_Vs=0x01;
						  send_msg_to_usb(0x15,ID_msg,13);
				      break;					
					  case 0x16 :
					     Jiaoyan_Data_Vs=0x01;
						 send_msg_to_usb(0x16,ID_msg,13);
				      break;
					case 0x17 :
					      Jiaoyan_Data_Vs=0x01;
						  send_msg_to_usb(0x17,ID_msg,13);
				      break;					

				
				    default:
				      break;
				  }
		    cmd_jianya_flag=0;
		 }
		 else if(cmd_jianya==0x11)
		 {
			
	      Jiaoyan_Data_Vs=0x01;
		  send_msg_to_usb(cmd,ID_msg,13);
		   cmd_jianya_flag=1;
		 
		 }


}

void ID_information(void)
{

  if((USART2_RX_BUF[4]==0x02)&&(USART2_RX_BUF[5]==0x01)&&(USART2_RX_BUF[6]==0x01))
	{
	    flash_read(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);


	    p_data_read[3]=USART2_RX_BUF[4]&0xFF;
		p_data_read[4]=USART2_RX_BUF[5]&0xFF;
	    p_data_read[5]=USART2_RX_BUF[6]&0xFF;
	
		p_data_read[6]=USART2_RX_BUF[7]&0xFF;
	    p_data_read[7]=USART2_RX_BUF[8]&0xFF;
		p_data_read[8]=USART2_RX_BUF[9]&0xFF;
		p_data_read[9]=USART2_RX_BUF[10]&0xFF;
	
		p_data_read[10]=USART2_RX_BUF[11]&0xFF;		
		p_data_read[11]=USART2_RX_BUF[12]&0xFF;
	
		p_data_read[12]=USART2_RX_BUF[13]&0xFF;
		p_data_read[13]=USART2_RX_BUF[14]&0xFF;
	
		p_data_read[14]=USART2_RX_BUF[15]&0xFF;
		p_data_read[15]=USART2_RX_BUF[16]&0xFF;

		flash_write(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);	
		
		ID_msg[0]=USART2_RX_BUF[4]&0xFF;
		ID_msg[1]=USART2_RX_BUF[5]&0xFF;
		ID_msg[2]=USART2_RX_BUF[6]&0xFF;
	
		ID_msg[3]=USART2_RX_BUF[7]&0xFF;
		ID_msg[4]=USART2_RX_BUF[8]&0xFF;
		ID_msg[5]=USART2_RX_BUF[9]&0xFF;
		ID_msg[6]=USART2_RX_BUF[10]&0xFF;
	
		ID_msg[7]=USART2_RX_BUF[11]&0xFF;		
		ID_msg[8]=USART2_RX_BUF[12]&0xFF;
	
		ID_msg[9]=USART2_RX_BUF[13]&0xFF;
		ID_msg[10]=USART2_RX_BUF[14]&0xFF;
	
		ID_msg[11]=USART2_RX_BUF[15]&0xFF;
		ID_msg[12]=USART2_RX_BUF[16]&0xFF;
		Jiaoyan_Data_Vs=0x01;
	}
	else
	Jiaoyan_Data_Vs=0x00;
	send_msg_to_usb(0x01,ID_msg,13);
	Usb_led_Vs=0x01; 

}
