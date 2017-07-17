/******************************************************************************/
// Starting by 20130-12-03
// Pargramed by qlx
/******************************************************************************/
#include "gpio.h"
//#include "ht1622.h"
#include "usart.h"
#include "delay.h"
#include "sys.h"
#include "ht1632.h"
#include "6658spi.h"
//#include "exti.h"
#include "basic.h"
#include "rtc.h"
#include "adc.h"
#include "timer.h"
#include "stm32f10x.h"
#include "spi.h"
#include "HSHCAL001B.h" 
#include "crc.h"
#include "string.h"
#include "spi_flash.h"
//#include "hw_config.h" 
////#include "usb_lib.h"
//#include "usb_pwr.h"
//#include "ff.h"			/* FatFs configurations and declarations */
//#include "diskio.h"
//#include "mp3play.h "
//#include <math.h>;
#include "delay.h"
#include "vcnl4000.h" 
#include "ec808.h"

typedef enum { FAILED = 0, PASSED = !FAILED} TestStatus;

/* 获取缓冲区的长度 */
#define TxBufferSize1   (countof(TxBuffer1) - 1)
#define RxBufferSize1   (countof(TxBuffer1) - 1)
#define countof(a)      (sizeof(a) / sizeof(*(a)))
#define  BufferSize (countof(Tx_Buffer)-1)

#define  FLASH_WriteAddress     0x00000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress
#define  sFLASH_ID              0xEF4017 
   
//#define  sFLASH_ID              0xEF4015	 //W25Q16
/* 发送缓冲区初始化 */
uint8_t Tx_Buffer[] = "请打开网络为我配置联网! \r\n";
uint8_t Rx_Buffer[BufferSize];
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

/************************************************************/
//  Declare the function
/************************************************************/
//void Beep(void);
//void Beep(u8 Beep_Static);
//void BeepErr(void);
//void LedLight(u8 mode);
//void Volume(u8 mode,u8 v);
void EnveramentInit(void);
void receice_from_usb(void);
void receice_from_wifi(void);
void receice_from_co2(void)	;
//void LightControl(u8 keyn);
//void KeyLight(void);
void KeyScan(void);
//void DisplayInit(void);
//void DisplayRefresh(void);
//void DisplayTime(void);
//void DisplayFigure(u8 h);
//void RtcUpdate(u8 mode);
//void RtcInitlize(void);
void SetTimer(void);
//void GetBasicInformation(void);
//void StateChange(void);
//void SelectWaveTime(u8 mode);
//void Timerx_Init(void);
//void DisplayWave(u8 mode);
//void DisplayWaveTime(u8 mode,u8 wavedat);
//void DisplayWaveTimePosition(u8 mode,u8 posi);
//void DisplayInstructions(u8 mode);
//void DisplayInterval(u8 mode);
//void DisplayMeter(u16 cnt,u8 mode,u8 py);
//void DisplayVolume(u8 voice);
//void DisplayPlayStat(u8 mode,u8 st);
//void DisplayStop(void);
//void DisplaySetT(u8 mode,u8 dat);
//void DisplaySetR(u8 mode);
void SystemClock(void);
//void Stop(void);
void TimeAdjust(u8 mode,u8 p);


//extern void TemperaturePrint(int16_t* AdcTemp1,u16* AdcTemp2);
extern void ADC_Data_process(int16_t* AdcTemp1,u16* AdcTemp2);

extern void task(void);


extern void ADC_Process(void);
extern void RTC_Display(void);
extern void fan_run(u8 fan_static);	  
extern void Connect_Process(u8 Connect_static);

extern void disp_date(u8 data[32],u8 addr_start,u8 count);
extern u8 flash_write(u32 StartAddr,u32 *p_data,u32 size);
extern int flash_read(u32 StartAddr,u32 *p_data,u32 size);


/************************************************************/
//  Declare the vira
/************************************************************/
#define	INC	0X01
#define DEC	0X02


//环境变量声明
//u8	FlagKeyInt;     //按键触发标志位
u8  FlagRtcInt;     //时钟秒中断标志位
//u8 	FlagBuzzer;
//u8	FlagShine;
//u8	FlagLight;
//u8  FlagShineSetTime1;
//u8  FlagShineSetTime2;
//u8 	FlagShineWave;
//u8 	FlagShineTime;
//u8  FlagShineTimePosi;
//u8	FlagShineInterval;
//u8	FlagShineIntervalBak;
//u8  FlagShineInstruction;
//u8  FlagShineSetT;
//u8  FlagShineSetR;
//u8  FlagShinePause;
//u8	PlayStatus;		//播放/暂停状态标识
//u8	PosiValue;

//extern u8 data_led[10];sdv
//    |-7-|
//    5   6
//    |-4-|
//    2   3
//    |-1-|


//                0     1    2   3    4    5    6    7     8    9   wu    v    e    r   h
//u8 data_led[15]={0xee,0x48,0xd6,0xda,0x78,0xba,0xbe,0xc8,0xfe,0xfa,0x00,0x6e,0xb6,0x14,0x7c};   //0~9 数字显示字符
//                0     1    2   3    4    5    6    7     8    9   wu    v    e    r   h    H
//u8 data_led[16]={0xee,0x48,0xd6,0xda,0x78,0xba,0xbe,0xc8,0xfe,0xfa,0x00,0x6e,0xb6,0x14,0x3c,0x7c};   //0~9 数字显示字符
//u8 DisplayBuff[16];			//显示缓冲
//
//u8  DISPLAYSETIME[10] = {0xd7,0x06,0xe3,0xa7,0x36,0xb5,0xf5,0x07,0xf7,0xb7}; //定时显示
//u8  DISPLAYWAVE[10] = {0xbe,0x06,0x7c,0x5e,0xc6,0xda,0xfa,0x0e,0xfe,0xde};  //波形显示
//u8  DISPLAYTIME[10] = {0x5f,0x50,0x6b,0x79,0x74,0x3d,0x3f,0x58,0x7f,0x7d};   //时间显示
//u8  DISPLAYTIMEMETER[10] = {0xfa,0x60,0xd6,0xf4,0x6c,0xbc,0xbe,0xe0,0xfe,0xfc};  //计时显示

PubTimer TimerBuf; 
DefStrDisplay PubStrDisplay; //定时器信息 
DefStrTimer     PubstrTimer; //定时器信息 
u32 Sys_Counter;
u32 vncl_Sys_Counter;
u32 Runtime=0;


//定义按键值
//u8	KeyValue;
//u8  KeyNum;         //按键值
//u8  KeyPressed;     //按键按压标识

//u8 WaveValue;
//u8 WaveTime;
//u8 WaveTimeNum;
//u8 WaveTimeStart;
//u8 WaveTimeStop;

//声音参数
//u8	VolumeSignal;
//u8	VolumeSignalBak;

u8 	SetHour;
u8  SetMin;
u8 	PubFlagSecond;
u8 	PubFlagFinished;
u8 data_disp[32];
//u8 Beep_Flag=0;

extern u32 Flash_Write_Address;	   
extern u32 p_data_write[56];
extern u32 p_data_read[56];
extern int16_t Sample_Value_Set;
extern int16_t Coldtrap_Value_Set;
extern u8 Model;
extern u16 PM25_Vs;
extern u16  PM25_k;
extern u8 rtc_data[8];
extern u16 PM25_Value;
extern u16 rtc_data1;
extern u16 Temp,Humi;
 

extern u8 Flash_Data_Length;
extern int16_t ADC_ConvertedValueLocal[4];  // 局部变量，用于保存转换计算后的电压值	
extern u16 ADC_ConvertedValue[4];// ADC1转换的电压值通过MDA方式传到SRAM
extern void IAQ_Init(void);


//extern static void HumInit(void);
u8 Temp1_B,Temp1_C,Temp2_B,Temp2_C,Temp3_B,Temp3_C,Temp4_B,Temp4_C;


u8 CO2_Receive_Flag = 0;
extern u8 USART1_TX_Data_Flag;
extern u8 USART4_RX_BUF[64];     //接收缓冲,最大64个字节.
extern u8 USART4_TX_BUF[10]; 
extern u8 USART1_TX_BUF[64];     //发送缓冲,最大64个字节.
extern u8 USART1_RX_BUF[564]; 
extern u8 USART4_RX_BUF_Counter;
extern u8 USART1_RX_BUF_Counter;
extern u8 USART2_RX_BUF[64];     //接收缓冲,最大64个字节
extern u8 USART2_RX_BUF_Counter;
extern u8 USART5_RX_BUF_Counter;
extern u8 USART5_RX_BUF[64];
u8 USART1_RX_Timer=0;
u8 USART2_RX_Timer=0;
extern u8 USART1_RX_BUF1[564];
u8 USART4_RX_Timer=0;
u8 USART5_RX_Timer=0;
extern void USART3_TX_Run_State_updata(u8 Length);
extern void USART_TX_Run_State_updata(u8 Send_Lenth);
extern void OLED_Init(void);
extern void USART4_Config(u32 bound);

extern void USART5_Config(u32 bound);
extern void Timerx_Init(void);
extern u16  CRC16(u8 *puchMsg, u8 usDataLen);
extern void receive_msg_from_usb(void); 
extern void HT1632C_Init(void);
extern void LCD2_update(void);
extern void Mac_TX_State(void);	
u16 CRC_Code;
u8 Data_Test[6]={0xFE,0x04,0x00,0x03,0x00,0x01};
u16 CO2_Data;
u16 CO2_Data_Vo;
u16 CO2_Data_tem[5];
int16_t CO2_Data_Vs=10000;
int16_t CO_Data_Vs=500;
u16 CO_Data;
extern u8 sound_display;
u8  senser_co2_flag=7;
extern float VOC_Data;
extern u8 co2_ok;
extern u8 co_ok;
extern PubTimer RTC_Set_Time4;
//FATFS fs;
//FRESULT res;
//TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
u8 USART6_TX_BUF[10];     //发送缓冲,最大64个字节.
extern u16 re_frequency;
extern u8 wifi_msg_temp[1000]; 
extern u8 wifi_msg_temp_1[100]; 
char textFile1[] = "m51";
u16 receive_wifi_num=6;
u16 receive_usb_num=8;
extern u8 Mac_msg[18]; 
u8 ID_msg[13];
extern u16 Temperature_Vs;
extern u16 Humidity_Vs;
extern	u16 VOC_Data_Vs;
extern u32 data_num;
extern u32 data_size;
extern u32 data_started;
extern u32 data_ended;
extern u8 data_num_buf[15];
extern u8 Sound_Vs;
u8 flash_ceshi=0;  //1 ok
u8 rtc_ceshi=0;	 //1  ok
u8 ceshu_flash_sound=0;
extern u16 VCNL4000_PROX_Result1_1 ;
extern u16 VCNL4000_PROX_Result2_1 ;
/************************************************************/
//  main
/************************************************************/
int main(void)
{	
//   u8 j;
	SystemInit(); 		    //系统时钟初始化为72M	  SYSCLK_FREQ_72MHz
	NVIC_Configuration(); 	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    IWDG_Configuration() ;
  
 	    	/* 2M串行flash W25X16初始化 */
	SPI_FLASH_Init();
  	  	MY_GPIO_Init();
 
//  	SPIx_Init(); 
	//HT1622_Init();          //LCD初始化
	//WriteAll_1622(0,0xff);
	delay_init(72);	    	//延时函数初始化		  
	RTC_Init();
  	Timerx_Init();   //		
	ADC1_Init();    //ADC 20140504 		
    uart1_init(115200);
	USART4_Config(9600);
	USART5_Config(9600);
	USART2_Config(9600);
	//HSHCAL001B_Init();
	//HumInit();
//	IIC1_Init();
	IAQ_Init();
//	RTC_Set(2015,11,6,12,6,0);  //设置时间		
//	INIT_1632();	
	 HT1632C_Init()	;
	/*设置USB时钟为48M*/
//	Set_USBClock();
// 	
//	/*配置USB中断(包括SDIO中断)*/
//	USB_Interrupts_Config();
// 
//	/*USB初始化*/
// 	USB_Init();
// 
// 	while (bDeviceState != CONFIGURED);	 //等待配置完成	
//	 {}
  //res =  f_mount(0,&fs);				 
  	EnveramentInit();
	//	VS_Init();	
//	VS1003_SPI_Init();		/* MP3硬件I/O初始化 */ 
//		
//	MP3_Start(); 			/* MP3就绪，准备播放，在vs1003.c实现 */ 
	
	
//	RTC_Display();		 //RTC process	
//	   OLED_Init();			
	OLED_Init();  	  
	//	Beep();	
	CO2_C=1;
	//PEout(9)=1;
	sound_C=0;
	SPIRES=1;
	PEout(10)=1;
	PEout(12)=1;
	PEout(13)=1;


//	f_mkfs(0,0,512);
	LCD2_update();
 //  LCD2_update();
//		USART6_TX_BUF[0]=0xFD;
//	USART6_TX_BUF[1]=0x00;
//	USART6_TX_BUF[2]=strlen(textFile1)+2;
//	USART6_TX_BUF[3]=0x01;
//	USART6_TX_BUF[4]=0x01;
//
//
//	for(j=0;j<5;j++)  //return ACK
//	{
//		while( USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET );
//		USART_SendData(UART5,USART6_TX_BUF[j]);
//	}

//	void USART3_TX_State(void)	   //用于向CO2传感器发送命令
//{
//	
//	for(j=0;j<8;j++)  //return ACK
//	{
//		while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
//		USART_SendData(USART1,0X03);
	//}

//}
     Mac_TX_State();

	CO2_C=1;
	VCNL4000_Init1();
	VCNL4000_Init2();
	VCNL4000_WriteOneByte1(VCNL4000_CMD,0x88);
	VCNL4000_WriteOneByte2(VCNL4000_CMD,0x88);	
																				 	
	while(1)
	{			
	//	u8 flash_temp=0;
		u8 k=0,L=0;
	//	static u8 m;
	//	int flash_read_temp;
//	 	PAout(4)=1;															 		
		task();			
		///////////////////////////////////////////////////BeeP///////////////////////////////////////////////////
	

		if(L)
		//	flash_read_temp=
			flash_read(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
		if(k) 
		//	flash_temp=
			flash_write(Flash_Write_Address,&p_data_write[0],(u32)Flash_Data_Length);					
//		if(m)
//		{ 
//			while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
//				USART_SendData(USART1,0x55);
//		}	
		//////////////////CO2//////////////////////////////////////////////////////					
		if(CO2_Receive_Flag)											 
		{
			CO2_Receive_Flag=0;
	
		}

		  receice_from_co2();
		
	   receice_from_wifi();
	   receice_from_usb();  //uart2



		if(USART5_RX_BUF_Counter>=1)		  //uart5 receive	syn6658
		{
			if(USART5_RX_BUF[0]==0x4f)
			{
			 sound_C=0;
			 sound_display=0;
			 }
			 if(USART5_RX_BUF[0]==0x41)
			{
			 sound_C=1;
			 sound_display=1;
			 }
			 USART5_RX_Timer=0;
			USART5_RX_BUF_Counter=0;
			 
		}
		/////////////////////////crc//////////////////////////////////////////
		 //CRC_Code=CRC16(Data_Test, 6);
	
		///////////////////////////////ADC///////////////////////////////////
	//	ADC_Process();

//	
		 
	}
}
//TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
//{
//  while(BufferLength--)
//  {
//    if(*pBuffer1 != *pBuffer2)
//    {
//      return FAILED;
//    }
//
//    pBuffer1++;
//    pBuffer2++;
//  }
//  return PASSED;
//}

u8 ID_msg[13];
 void EnveramentInit(void)
{

//  u8 data_ceshi_r[2];
 
  RTC_Set_Time4=Timer;
//	u8 temp;
	/////////////////////////read flash parameter///////////////////////////////
	//temp=
	flash_read(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);

	 if(((p_data_read[0]&0xFFFF)>0)&&((p_data_read[0]&0xFFFF)<4096))
	   PM25_Vs  = p_data_read[0]&0xFFFF;
	 else
	  PM25_Vs=4095;
//	Sound_Vs = p_data_read[1]&0xFFFF;
	if(((p_data_read[16]&0xFFFF)>0)&&((p_data_read[16]&0xFFFF)<20000))
	   CO2_Data_Vs = p_data_read[16]&0xFFFF;
	else
	   CO2_Data_Vs =10000;
	if(((p_data_read[17]&0xFFFF)>=50)&&((p_data_read[17]&0xFFFF)<=200))
	   Temperature_Vs = p_data_read[17]&0xFFFF;
	else
	   Temperature_Vs=100;
	if(((p_data_read[18]&0xFFFF)>0)&&((p_data_read[18]&0xFFFF)<200))
	   Humidity_Vs = p_data_read[18]&0xFFFF;
	else
	   Humidity_Vs=100;
	if(((p_data_read[19]&0xFFFF)>0)&&((p_data_read[19]&0xFFFF)<1000))
	    CO_Data_Vs = p_data_read[19]&0xFFFF;
	else
		CO_Data_Vs=500;
	if(((p_data_read[20]&0xFFFF)>0)&&((p_data_read[20]&0xFFFF)<400))
	    VOC_Data_Vs = p_data_read[20]&0xFFFF;
	else
	    VOC_Data_Vs =200;


//	PM25_k =p_data_read[1]&0xFFFF;
//	if(PM25_k>100)
//		PM25_k=100;
    re_frequency=p_data_read[2]&0xFFFF;
	if((re_frequency<5)|| (re_frequency>3600))
		re_frequency=300;
	 if(((p_data_read[3]&0xFF)!=0)&&((p_data_read[3]&0xFF)!=0xFF))
   {
		ID_msg[0]=p_data_read[3]&0xFF;
		ID_msg[1]=p_data_read[4]&0xFF;
		ID_msg[2]=p_data_read[5]&0xFF;
	
		ID_msg[3]=p_data_read[6]&0xFF;
		ID_msg[4]=p_data_read[7]&0xFF;;
		ID_msg[5]=p_data_read[8]&0xFF;
		ID_msg[6]=p_data_read[9]&0xFF;
	
		ID_msg[7]=p_data_read[10]&0xFF;		
		ID_msg[8]=p_data_read[11]&0xFF;
	
		ID_msg[9]=p_data_read[12]&0xFF;
		ID_msg[10]=p_data_read[13]&0xFF;
	
		ID_msg[11]=p_data_read[14]&0xFF;
		ID_msg[12]=p_data_read[15]&0xFF;
	
	
		
	 }
	 else
	 {
		ID_msg[0]=0x02;
		ID_msg[1]=0x01;
		ID_msg[2]=0x01;
	
		ID_msg[3]=0x14;
		ID_msg[4]=0x10;
		ID_msg[5]=0x04;
		ID_msg[6]=0x1e;
	
		ID_msg[7]=0x00;		
		ID_msg[8]=0x16;
	
		ID_msg[9]=0x10;
		ID_msg[10]=0x10;
	
		ID_msg[11]=0x00;
		ID_msg[12]=0x00;


		
	
		
	 }
	

	

	  	SPI_Flash_Read(data_num_buf,0x0000,11); //存储数据量
		if(data_num_buf[10]!=0xAA)
		{ 
		  // SPI_FLASH_BulkErase();
		   data_num=0;
		   data_size= data_num*18;
		   data_started=0x000fff;
		   data_ended=0x000fff;
		   data_num_buf[0]=(data_num>>24)&0xff;
		   data_num_buf[1]=(data_num>>16)&0xff;
		   data_num_buf[2]=(data_num>>8)&0xff;
		   data_num_buf[3]=data_num&0xff;
		   
		   data_num_buf[4]=(data_started>>16)&0xff;
		   data_num_buf[5]=(data_started>>8)&0xff;
		   data_num_buf[6]=data_started&0xff;
		   		
		   data_num_buf[7]=(data_ended>>16)&0xff;
		   data_num_buf[8]=(data_ended>>8)&0xff;
		   data_num_buf[9]=data_ended&0xff;
		   data_num_buf[10]=0xAA;

		   SPI_Flash_Write(data_num_buf, 0x000000,11)  ;		    
		
		  flash_ceshi=0;

		}
		else	 
		{
		   flash_ceshi=1;
			data_num=	((data_num_buf[0]<<24)&0xff000000)+((data_num_buf[1]<<16)&0x00ff0000)
			            +((data_num_buf[2]<<8)&0x0000ff00)+((data_num_buf[3])&0x000000ff);

   		    data_size= data_num*18;		 
		}

	   	if((p_data_read[25]&0xFFFF)==0x25 )
		  {
//			SPI_Flash_Read(data_ceshi_r, 0xffffff00,2);
//			if((data_ceshi_r[0]==1)&&(data_ceshi_r[1]==1)) 
//			  {
//				flash_ceshi=1;
//				data_ceshi_r[0]=0;
//				data_ceshi_r[1]=0;
//			    SPI_Flash_Write(data_ceshi_r, 0xffffff00,2);
//
//			  }
//			 else
//			  flash_ceshi=0;

			 if((RTC_Set_Time4.w_year==2000)&&(RTC_Set_Time4.w_month==1)) 
				{
					RTC_Set(2016,1,1,1,1,1);  //设置时间
					
					rtc_ceshi=1;	 //1  ok
				}
			    else
				 rtc_ceshi=0;

			 ceshu_flash_sound=1;			 
			  p_data_read[25] =0x00&0xffff;
			  flash_write(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length); 
		   }
//			 if((RTC_Set_Time4.w_year==2000)&&(RTC_Set_Time4.w_month==1)) 
//				{
//				RTC_Set(2016,1,1,1,1,1);  //设置时间
//				
//				rtc_ceshi=1;	 //1  ok
//				}
//			 else
//				 rtc_ceshi=0;

}

void receice_from_co2(void)
{
   u16 CO2_Data_temp;

 if(USART4_RX_BUF_Counter)		  //uart4 receive
		{
			if(USART4_RX_Timer > 2) 
			{
				////////////////////////CRC16 check//////////////////////////////////
			    if((CRC16(USART4_RX_BUF, USART4_RX_BUF_Counter-2) == ((USART4_RX_BUF[USART4_RX_BUF_Counter-1]) | USART4_RX_BUF[USART4_RX_BUF_Counter-2]<<8))&&(USART4_RX_BUF[0]==0x68))
				{

				   CO2_Data_temp =(( USART4_RX_BUF[3]<<8 &0xff00 )+ (USART4_RX_BUF[4]&0xff));
					 CO2_Data_Vo=(CO2_Data_Vo+CO2_Data_temp	)/2;
					  CO2_Data_Vo=  data_lvbo(CO2_Data_Vo,&CO2_Data_tem[0],&CO2_Data_tem[0]);

				   if((CO2_Data_Vo+10200)> CO2_Data_Vs)
				         CO2_Data =CO2_Data_Vo+10000-CO2_Data_Vs;
					 else
					  CO2_Data=200;
				   //CO2_Data[1] = USART3_RX_BUF[4];
//					for(i=0;i<=USART3_RX_BUF_Counter;i++)
//						USART_TX_BUF[i]=USART3_RX_BUF[i];        			
//					USART_TX_Run_State_updata(USART3_RX_BUF_Counter);
					 if(CO2_Data < 200)
					    CO2_Data=200;
					if(CO2_Data >10000)
					    CO2_Data=10000;
						co2_ok=1;
					 senser_co2_flag=0;
				}
				else
				senser_co2_flag=7;

				USART4_RX_Timer=0;
				USART4_RX_BUF_Counter=0;
			 }
		}



}


void receice_from_wifi(void)
{
	   if(USART1_RX_BUF_Counter)		  //uart4 receive
		{
			if(USART1_RX_Timer > 2) 
			{
		
		//	if(USART1_RX_BUF_Counter>=receive_wifi_num)		  //uart1 receive wifi
				if(USART1_RX_BUF_Counter>=receive_wifi_num)
				{	
				     if(USART1_RX_BUF[0]==0x04)
					   receive_wifi_num=6;
					 else if(USART1_RX_BUF[0]==0x03)
					   receive_wifi_num=22;
					 else if(USART1_RX_BUF[0]==0x55)
					        receive_wifi_num=USART1_RX_BUF[2]+3;
						  	 else if(USART1_RX_BUF[1]==0x55)
					        receive_wifi_num=USART1_RX_BUF[3]+4;
		
				    if(((USART1_RX_BUF[0]==0x03)||(USART1_RX_BUF[0]==0x04))&&(USART1_RX_BUF_Counter>=receive_wifi_num))
				   	    {
						receive_mac_from_wifi();
		//			   	USART1_RX_Timer=0;
		 //               memset(USART1_RX_BUF,0x00,USART1_RX_BUF_Counter);
						USART1_RX_BUF_Counter=0;
						receive_wifi_num=6;
						
						}
		
		
					    if( (USART1_RX_BUF[0]==0x55)&&(USART1_RX_BUF_Counter>=receive_wifi_num)&&(USART1_RX_BUF[receive_wifi_num-1]==0xFF))
						{
						 memcpy(USART1_RX_BUF1,USART1_RX_BUF,receive_wifi_num);
						 receive_msg_from_wifi();
		//				 	USART1_RX_Timer=0;
						USART1_RX_BUF_Counter=0;
						receive_wifi_num=6;
						}
		
					    if( (USART1_RX_BUF[1]==0x55)&&(USART1_RX_BUF_Counter>=receive_wifi_num)&&(USART1_RX_BUF[receive_wifi_num-1]==0xFF))
						{
						 memcpy(USART1_RX_BUF1,USART1_RX_BUF+1,receive_wifi_num-1);
					 receive_msg_from_wifi();
		//				 	USART1_RX_Timer=0;
						USART1_RX_BUF_Counter=0;
						receive_wifi_num=6;
						}
		
						if((USART1_RX_BUF[0]==0x03)||(USART1_RX_BUF[0]==0x04)||(USART1_RX_BUF[0]==0x55)||(USART1_RX_BUF[1]==0x55)||(USART1_RX_BUF_Counter<=receive_wifi_num))
					     {
		//			     receive_wifi_num=6;
		//////				   	 USART1_RX_Timer=0;
		//				 USART1_RX_BUF_Counter=0;
		////					 memset(USART1_RX_BUF,0x00,18);
						}
						else
						 {
						 receive_wifi_num=6;
		////				   	 USART1_RX_Timer=0;
						 USART1_RX_BUF_Counter=0;
		//					 memset(USART1_RX_BUF,0x00,18);
						}

				}

				 receive_wifi_num=6;
			   	 USART1_RX_Timer=0;
				 USART1_RX_BUF_Counter=0;

		   }
		}

}



void receice_from_usb(void)
{
	   if(USART2_RX_BUF_Counter)		  //uart4 receive
		{
			if(USART2_RX_Timer > 2) 
			{
		 		if(USART2_RX_BUF_Counter>=receive_usb_num)		  // receive  usb
				{
					  if(USART2_RX_BUF[0]==0x55)
					   receive_usb_num=( (USART2_RX_BUF[1]<<8 &0xff00 )+ (USART2_RX_BUF[2]&0xff));
		
			    }
				
				if( (USART2_RX_BUF[0]==0x55)&&(USART2_RX_BUF_Counter>=receive_usb_num)&&(USART2_RX_BUF[receive_usb_num-1]==0xff))
				{
				
				 receive_msg_from_usb();
		
				USART2_RX_BUF_Counter=0;
				receive_usb_num=8;
		
				}
				if((USART2_RX_BUF[0]!=0x55)||(USART2_RX_BUF_Counter>receive_usb_num))
				{
				
				USART2_RX_BUF_Counter=0;
				receive_usb_num=8;
				
				
				}
				receive_usb_num=8;
				USART2_RX_Timer=0;
				USART2_RX_BUF_Counter=0;
		   }
	   }
}


