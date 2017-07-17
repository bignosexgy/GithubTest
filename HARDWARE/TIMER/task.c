//#include "timer.h"
#include "gpio.h"
#include "stm32f10x.h" 
#include "task.h" 
#include "key.h"
#include "rtc.h"
#include "delay.h"
#include "HSHCAL001B.h" 
#include "iaq.h"
#include "TVOC.h"
//#include "ff.h"			/* FatFs configurations and declarations */
//#include "diskio.h"
#include "stdio.h"
#include "vcnl4000.h"
#include "oled.h"
#include "ht1632.h"
#include "usart.h"
#include "spi_flash.h"
#include <string.h>
#define CO2_UART1_CONFIG 
//extern void key_process(void);
extern u8 IAQ_Read(u8 *pBuffer, u8 NumByteToRead);
//extern void Runtime_Process(void);
extern void ADC_Process(void);
extern void RTC_Display(void);
extern void fan_run(u8 fan_static);
extern void USART_RX_Process(void);
extern u32 Get_TVOC(void);
extern void EC808_Process(void);
extern float Sht21Work(u8 order) ;
 
extern void USART_TX_Run_State_updata(u8 Send_Lenth);
extern void USART3_TX_Run_State_updata(u8 Length);

extern void USART3_TX_State(void);
extern void USART2_TX_State(void);
extern u8 USART1_RX_Timer;
extern u8 USART2_RX_Timer;
extern u8 USART4_RX_Timer;
extern u8 USART5_RX_Timer;
u8 USART_TX_Data_Time_Conter=0;

extern void RTC_Time_Set(void);	
//u16 RTC_Time_Set_flag=0;
extern void Parameter_Set(void);   	
//extern void	Vac_ValueSet(void);		  
extern void Model_Display(u8 Model_temp); 
extern void Beep(u8 Beep_Static);  
extern u32 Sys_Counter;
extern u32 vncl_Sys_Counter;
//extern u8 USART_RX_Flag; 
//extern u8 USART_TX_Data_Flag;	
//extern u8 parameter_Set_Flag;
//extern u8 Beep_Flag;
extern PubTimer Timer;//时钟结构体 	
extern PubTimer RTC_Set_Time2,RTC_Set_Time3;

extern u16 CO_Value;
extern u16 PM25_Value_1;
//u8 Beep_Counter=0;
extern u16 VCNL4000_PROX_Result1_1 ;
extern u16 VCNL4000_PROX_Result1_2 ;
extern u16 VCNL4000_PROX_Result2_1 ;
extern u16 VCNL4000_PROX_Result2_2 ;
//u8 Parameter_Time_Set_Flash;
//u8 Parameter_Time_Set_Flash_Counter;
u8 vncl4010_Flag=1;
u8 vncl4010_Flag_1=0;
u8 Task_Flag=1;
//u8 Fan_Flag=0;
u8 LOGIN_CMD_Flag=0 ;//1：登录，
u8 LOGIN_CMD_Flag_num=0 ;//1：登录，
u8 Second_LOGIN_CMD_Flag_num=0 ;//1：登录，
u8 Pingji_Flag=0 ;//，
//u8 Connect_Flag=0; 
//u8 Fan_Flag=0;
u8 RE_HEART_CMD_Flag=0 ;//1：心跳，
u8 SE_HEART_CMD_Flag=0;

u8 RE_DATA_CMD_Flag=0 ;//1：心跳，
u8 SE_DATA_CMD_Flag=0;
u8  WIFI_Flag=0;//0断网
u8  WIFI_internet_Flag=0;//0断网 1连接路由器
u16	 re_frequency=10;
u8  test_imid=0;//
u8  imid_test=1;//
u8  pm25_test_st=0;//
u8 Set_time_Flag=0;
u8 Set_mac_Flag=0;
u32 send_i_len = 0;
u32 send_i_len_num = 20;
u8  heart_init=0;
u8  send_data_init=1;
u8  send_data_init_one=1;
u8  WIFI_internet_send_Flag=0;
u32 data_num=0;
u32 data_size=0;
u32 data_started=0;
u32 data_ended=0;
//u8 Key_Press_Counter;
//u8 I2C_Temp[14];
//u16 Temp_Humi[2];
//double Temp_Humi[2];
//u8 Timer_280us_Flag=0;
//u8 Timer_40us_Flag=1;
u16 Sensor_Sample_Flag_Timer=0;
u8 Sensor_Sample_Flag=0;
u16 PM25_Detect_Timer=0;		
u8 PM25_Detect_Flag=1;
u8 PM25_Vs_Flag=1;
u8 PM25_Detect_Timer2=0;
extern u8  Alpha;
extern u8  Beta;
extern u8 ADC_Start_Flag;
extern u16 BAT_Value;
extern u16  PM25_Vo;
extern u16  PM25_Vs;
extern u32 p_data_write[56];
extern u32 p_data_read[56];
extern u8 Flash_Data_Length;
extern u8 Mac_msg[18]; 
extern u32 Flash_Write_Address;

u8 time_num_20s=0;

u8 oled_Flag=0;
u8 oled_Flag_1=0;//1屏保

u8 oled_time_Flag=0; //1显示时间
u8 task_10_Flag=0;
u8 task_25_Flag=0;
u8 task_75_Flag=0;
//u8 Iaq_Data[15];
u32 Value_Tvoc; 
		
extern u8 rtc_data[8];
extern u16 PM25_Value;
extern u16 rtc_data1;
extern u16 Temp,Humi;
extern	u16 CO2_Data;
extern	u16 CO_Data;
float VOC_Data ;
extern	u16 VOC_Data_Vs=200;
extern u8 Model;
u16 Humidity;
int16_t Temperature;
int16_t Temperature_Vs=100;
int16_t Humidity_Vs=100;
extern u8 mm;
int a;

extern int16_t Temperature_display;
extern u16   Humidity_display;
extern u16 PM25_Value_display;
extern u16 CO2_Data_display;
extern u16 CO_Data_display;
extern float VOC_Data_display ;
extern u8   sound_display;

//FIL fsrc,fdst,fdst1,fdst2;
//FATFS fs;
//UINT br, bw;            // File R/W count
//BYTE buffer[512];       // file copy buffer
//BYTE textFileBuffer[] = "请打开网络为我配置联网! \r\n";
u8 pm25_ok=0;
u8 voc_ok=0;
u8 co2_ok=0;
u8 co_ok=0;
u8 tmp_ok=0;

u8 hmp_ok=0;
u8 	 VCNL4000_1=0;
u8 	 VCNL4000_2=0;
//u8 Timer_280us_Flag;
u8 data_buffer[20];
u8 re_data_buffer[1000];
u8 seng_data_buffer[1000];
u32 seng_data_num;
static u16 frequency_num;
static	u8  Second_flg=0;
static	u8  Second5_flg=0;
static	u8  Second51_flg=0;
static	u8 Second_SE_DATA_flg=0;
u16 pm25_test_mun =550;
u8 led_hx_num =0XA0;
u8	led_hx_dir=0 ;
u8 co_LED=1;
u8 tmp_LED=1;
u8 co2_LED=1;
u8 hum_LED=1;
u8 voc_LED=1;
u8 pm25_LED=1;
u8 WIFI_flag_view=0;
u8 hx_LED_times=0;
u8 hx_LED_leibie=0;
u8 VCNL1=0;
u8 VCNL1_num=0;
u8 VCNL2=0;
u8 VCNL2_num=0;
u8 VCNL_page=0;
u8 VCNL_page_num=0;
u8 VCNL4000_1_num=0;
u8 VCNL4000_2_num=0;
u8 power_flag_inimd=0;
u8 power_flag_inimd_num=0;
u8 wifi_set=0;
u8  wifi_oled_set=0;
u8  wifi_oled_set_num=0;
u16  wifi_set_num=0;
extern u8  sound_oled;
extern u8  sound_oled_num;
extern int8_t OLED_curPos ;
extern  u8 Sound_Vs;
extern u8 internet;
u8 internet__send_flag=0;
u8 internet__send_flag_0=0;
u8  BAT_Value_down=0;
u8 VNCL_ceshi=0;  //1 ok
u8 senser_ceshi=0;	 //1  ok
u8 senser_level=0;	 //1  ok
extern u8 senser_co2_flag;
extern u8 senser_th_flag;
extern u8 senser_co_flag;
extern u8 senser_voc_flag;
extern u8 senser_pm_flag;
extern u8 cmd_jianya_flag ;
u8 WiFi_server_flag ;
u8 Ceshi1_num=0;
extern u8 Usb_led_Vs;
u8  Usb_led_Vs_num=0;
u8  Usb_led_red_Vs_num=0;
extern u8 ceshu_flash_sound;
 u8 ceshu_flash_sound_num=0;

u8 data_num_10=0;
u8 WIFI_internet_send__num_Flag=0;

void RED_hx(void)
{
	 		  
			  HT1632C_Write_CMD(0x09); 
			   LED_red();
		 /*
    	   if((led_hx_dir==0)&&(led_hx_num<0XAE))
		    { 
			  HT1632C_Write_CMD(0x03);//kaiLED显示?
			  led_hx_num++;		  
		   	  HT1632C_Write_CMD(led_hx_num);
			}
		   if(led_hx_num>=0XAE)
			  {
			   HT1632C_Write_CMD(0x03);//kaiLED显示?
			   led_hx_num=0XAE;
			   led_hx_dir=1;
			  }
			// if(led_hx_num==0XA0)
		 //  if((led_hx_dir==1)&&(led_hx_num>0XA0))
    	   if((led_hx_dir==1)&&(led_hx_num>0XA0))
		    { 
			  HT1632C_Write_CMD(0x03);//kaiLED显示?
			  led_hx_num--;	
			  if(led_hx_num<=0XA0)
			  { 
			 //  led_hx_dir=0;
			  // led_hx_num=0XA0	;
			   HT1632C_Write_CMD(0XA0);
	 		  }
			  else
			  	HT1632C_Write_CMD(led_hx_num);
  	
			}
			if(led_hx_num<=0XA0)
			  {
				   hx_LED_times++;
				   led_hx_num=0XA0;
				   led_hx_dir=3;
				   if(hx_LED_times<4)
				     {
					  HT1632C_Write_CMD(0x03);//kaiLED显示?
				      HT1632C_Write_CMD(led_hx_num);
					  }
				   else if(hx_LED_times<=6)
				     HT1632C_Write_CMD(0x02);//关LED显示?
	
				    if(hx_LED_times>6)
					{
					 HT1632C_Write_CMD(0x03);//kaiLED显示?
					 led_hx_num=0XA0;
					 HT1632C_Write_CMD(led_hx_num);
				     led_hx_dir=4;
					 }
					if(hx_LED_times>9)
					{
					 HT1632C_Write_CMD(0x03);//kaiLED显示?
					 led_hx_num=0XA1;
					 hx_LED_times=0;
					 HT1632C_Write_CMD(led_hx_num);
				     led_hx_dir=0;
					 }
			}
	 */
}
void GREEN_hx(void)
{
	 		  
			    HT1632C_Write_CMD(0x09);
				 LED_green(); 
/*	   
    	   if((led_hx_dir==0)&&(led_hx_num<0XAE))
		    { 
			  HT1632C_Write_CMD(0x03);//kaiLED显示?
			  led_hx_num++;		  
		   	  HT1632C_Write_CMD(led_hx_num);
			}
		   if(led_hx_num>=0XAE)
			  {
			   HT1632C_Write_CMD(0x03);//kaiLED显示?
			   led_hx_num=0XAE;
			   led_hx_dir=1;
			  }
			// if(led_hx_num==0XA0)
		 //  if((led_hx_dir==1)&&(led_hx_num>0XA0))
    	   if((led_hx_dir==1)&&(led_hx_num>0XA0))
		    { 
			  HT1632C_Write_CMD(0x03);//kaiLED显示?
			  led_hx_num--;	
			  if(led_hx_num<=0XA0)
			  { 
			 //  led_hx_dir=0;
			  // led_hx_num=0XA0	;
			   HT1632C_Write_CMD(0XA0);
	 		  }
			  else
			  	HT1632C_Write_CMD(led_hx_num);
  	
			}
			if(led_hx_num<=0XA0)
			  {
				   hx_LED_times++;
				   led_hx_num=0XA0;
				   led_hx_dir=3;
				   if(hx_LED_times<4)
				     {
					  HT1632C_Write_CMD(0x03);//kaiLED显示?
				      HT1632C_Write_CMD(led_hx_num);
					  }
				   else if(hx_LED_times<=6)
				     HT1632C_Write_CMD(0x02);//关LED显示?
	
				    if(hx_LED_times>6)
					{
					 HT1632C_Write_CMD(0x03);//kaiLED显示?
					 led_hx_num=0XA0;
					 HT1632C_Write_CMD(led_hx_num);
				     led_hx_dir=4;
					 }
					if(hx_LED_times>9)
					{
					 HT1632C_Write_CMD(0x03);//kaiLED显示?
					 led_hx_num=0XA1;
					 hx_LED_times=0;
					 HT1632C_Write_CMD(led_hx_num);
				     led_hx_dir=0;
					 }  
		   	
			}
*/	 
}


void BLUE_hx(void)
{
	 		   LED_blue();
			  // HT1632C_Write_CMD(0XAE);
    	   if((led_hx_dir==0)&&(led_hx_num<0XAE))
		    { 
			  HT1632C_Write_CMD(0x03);//kaiLED显示?
			  led_hx_num++;		  
		   	  HT1632C_Write_CMD(led_hx_num);
			}
		   if(led_hx_num>=0XAE)
			  {
			   HT1632C_Write_CMD(0x03);//kaiLED显示?
			   led_hx_num=0XAE;
			   led_hx_dir=1;
			  }
			// if(led_hx_num==0XA0)
		 //  if((led_hx_dir==1)&&(led_hx_num>0XA0))
    	   if((led_hx_dir==1)&&(led_hx_num>0XA0))
		    { 
			  HT1632C_Write_CMD(0x03);//kaiLED显示?
			  led_hx_num--;	
			  if(led_hx_num<=0XA0)
			  { 
			 //  led_hx_dir=0;
			  // led_hx_num=0XA0	;
			   HT1632C_Write_CMD(0XA0);
	 		  }
			  else
			  	HT1632C_Write_CMD(led_hx_num);
  	
			}
			if(led_hx_num<=0XA0)
			  {
				   hx_LED_times++;
				   led_hx_num=0XA0;
				   led_hx_dir=3;
				   if(hx_LED_times<4)
				     {
					  HT1632C_Write_CMD(0x03);//kaiLED显示?
				      HT1632C_Write_CMD(led_hx_num);
					  }
				   else if(hx_LED_times<=6)
				     HT1632C_Write_CMD(0x02);//关LED显示?
	
				    if(hx_LED_times>6)
					{
					 HT1632C_Write_CMD(0x03);//kaiLED显示?
					 led_hx_num=0XA0;
					 HT1632C_Write_CMD(led_hx_num);
				     led_hx_dir=4;
					 }
					if(hx_LED_times>9)
					{
					 HT1632C_Write_CMD(0x03);//kaiLED显示?
					 led_hx_num=0XA1;
					 hx_LED_times=0;
					 HT1632C_Write_CMD(led_hx_num);
				     led_hx_dir=0;
					 }

			  } 

}

void pm25_test(void)
{

	 u16 pm25_test_num;
	 if(re_frequency>300)
		 pm25_test_num =re_frequency;
	   else
		  pm25_test_num=300;
		  
 	if((Sensor_Sample_Flag_Timer++ >=pm25_test_num)||(test_imid==1)||(imid_test==1))	  //sample's time is 5 min
	{
		PM25_fan=1;
		Sensor_Sample_Flag_Timer=0;
		//Sensor_Sample_Flag=1;	
		PM25_Detect_Flag=1;
		ADC_Start_Flag=1;
		if(imid_test==1)
		imid_test=0;
		if(test_imid==1)
		 {
		  pm25_test_st=1;
		  test_imid=0;
		  pm25_ok=0;
		  }
	}

}



/*
 表示CO2和PM2.5灯一样，都是三种状态：绿、橙、红
表示电量的灯三种颜色：蓝、橙、红

红色亮表示低电、红色闪表示低电充电


橙色亮表示中等电量、橙色闪表示中等电量充电


蓝色亮表示高电量或者充电完成、亮色闪表示浮充
*/

static void  led_display(void)
 {
	if((power_flag==1)&&(power_flag_inimd==0))	  //电源3色等
		 {
			
			oled_Flag=0;
		    oled_Flag_1=0 ;
			 OLED_Display_On();
			if( power_flag_inimd_num==0)
			{
			  oled_time_Flag=0;
			  OLED_curPos=6;
		   	 LCD2_update();
			 }
		     HT1632C_Write_CMD(0x03);//打开LED显示?
		 // HT1632C_Write_CMD(0x09); 
			if(power_flag_inimd_num<2)
			LED_red();
			else  if(power_flag_inimd_num<4)
		   	 LED_blue();
			else  if(power_flag_inimd_num<6)
			 LED_green();
			else
			 power_flag_inimd=1;
			 power_flag_inimd_num++;
		  }
		else
		{	
		    if(power_flag==0)
			 {
			 power_flag_inimd=0;
			 power_flag_inimd_num=0;
			 }
		  				 
		    if(cmd_jianya_flag==0) 
			{
					 
				Usb_led_red_Vs_num=0;	 
				if(Usb_led_Vs==0x00) 
					 
				{
					if(BAT_Value_down==0)
					 {
					     
						 Usb_led_Vs_num=0;
						  HT1632C_Write_CMD(0x08); 
						  if(WIFI_Flag==1)
						     hx_LED_leibie=Pingji_Flag;
						   else	
						   {
						     hx_LED_leibie =senser_level;
							}
						
						  if(oled_Flag_1==0)
						   {
							 	HT1632C_Write_CMD(0x03);//打开LED显示?
							 if(hx_LED_leibie==1)
								 //GREEN_hx();
								 LED_green();
								 else  if(hx_LED_leibie==3)
								  //RED_hx();
								   LED_red();
								  else
								//	BLUE_hx();
									LED_blue();
							}
							else
							HT1632C_Write_CMD(0x02);//关LED显示?
						  }
						else
							HT1632C_Write_CMD(0x02);//关LED显示?

				}

				else  if(Usb_led_Vs_num<180)
				{
				    Usb_led_Vs_num++;
					HT1632C_Write_CMD(0x03);//打开LED显示?	
					 GREEN_hx();
				}
				     else
					 {
					  Usb_led_Vs_num=0;
					  Usb_led_Vs=0;
					  HT1632C_Write_CMD(0x08); 
					 }

		    }
			else if(Usb_led_red_Vs_num<180)
			 {
				Usb_led_red_Vs_num++;
				 Usb_led_Vs_num=0;
				 Usb_led_Vs=0;
				 HT1632C_Write_CMD(0x03);//打开LED显示?
				 RED_hx();
			
			}
			   else
			   	   				  
				 {
				  cmd_jianya_flag=0;
				  Usb_led_red_Vs_num=0;
				  Usb_led_Vs=0;
				  HT1632C_Write_CMD(0x08); 
				 }

	  }
   /*
	u8  t=0;//pc8=0;
	   t =  Key_Scan(GPIOE,GPIO_Pin_4);
	 //	pc8=Key_Scan(GPIOC,GPIO_Pin_8);

	 
	 if(BAT_Value<=3350)	 ////3.6v
	    {	 
		 if(t == 1)
			{
			    BAT_LED_R =~BAT_LED_R;
			 	BAT_LED_G =1;
				BAT_LED_B =1;
			}
	     else
	        {
				BAT_LED_R =0;
				BAT_LED_G =1;
				BAT_LED_B =1;
			}
		}


		if((BAT_Value<3700)&&(BAT_Value>3350))	 ////3.6v		3350
	    {	 
		 if(t == 1)
			{
				  BAT_LED_G = BAT_LED_R =~BAT_LED_R;
				  //BAT_LED_G=~ BAT_LED_G;
				  BAT_LED_B =1;
			 }
	     else
	        {
				 BAT_LED_R =0;
				 BAT_LED_G =0;
				 BAT_LED_B =1;
			 }
		}

		if((BAT_Value>=3700)&&(BAT_Value<3850)	) ////4.1v	  4.2  3722  3900
	    {	 
		 if(t == 1)
			{
		 	    BAT_LED_R =1;
				BAT_LED_G =1;
			    BAT_LED_B=~ BAT_LED_B;
			}
	     else
		 	{
				BAT_LED_R =1;
				BAT_LED_G =1;
		        BAT_LED_B =0;
			}
		}
			if(BAT_Value>=3850)	  ///4.2
			BAT_LED_B =0;

		  if(CO2_Data<=1000 )
			 {
				 CO2_LED_green = 0;
				 CO2_LED_red =1;
			 }
		  if((CO2_Data>1000 )&&(CO2_Data<=5000))
			 {
				 CO2_LED_green = 0;
				 CO2_LED_red =0;
			 }
		   if(CO2_Data>5000)
			 {
				 CO2_LED_green = 1;
				 CO2_LED_red =0;
			 }

		if(PM25_Value_1<=60 )
			 {
				 PM25_LED_green = 0;
				 PM25_LED_red =1;
			 }
		  if((PM25_Value_1>60 )&&(PM25_Value_1<=200))
			 {
				 PM25_LED_green = 0;
				 PM25_LED_red =0;
			 }
		   if(PM25_Value_1>200)
			 {
				 PM25_LED_green = 1;
				 PM25_LED_red =0;
			 }

   */
 }


 u16 get_params(u8 num)
{
	switch (num){
	case 0:
	    if(((sound_oled==1)&&(!Sound_Vs))||(sound_display==1))
		 return Temperature_display;
		else
		return Temperature;	
	case 1:
	     if(((sound_oled==1)&&(!Sound_Vs))||(sound_display==1))
		  return Humidity_display;
		 else
		return Humidity;
	case 2:
		 if(((sound_oled==1)&&(!Sound_Vs))||(sound_display==1))
		 return CO_Data_display;
		 else
    	return CO_Data;
	case 3:
		if(((sound_oled==1)&&(!Sound_Vs))||(sound_display==1))
		return CO2_Data_display;
		else
	    return CO2_Data;
	case 4:
	    if(((sound_oled==1)&&(!Sound_Vs))||(sound_display==1))
		return VOC_Data_display;
		else
		return VOC_Data;
	case 5:
	    if(((sound_oled==1)&&(!Sound_Vs))||(sound_display==1))
		 return PM25_Value_display;
		else
		return PM25_Value_1;
	case 6:
		return 0;
	case 7:
		return 0;


	default:
		return 0;
	}
}
 float get_params1(u8 num)
{			   
       if(((sound_oled==1)&&(!Sound_Vs))||(sound_display==1))
		  return VOC_Data_display;
		else
		return VOC_Data;

	
}
 int16_t get_params2(u8 num)
{
	switch (num){
	case 0:
	     if(((sound_oled==1)&&(!Sound_Vs))||(sound_display==1))
		   return abs(Temperature_display);
		 else
		   return abs(Temperature);	
			default:
		return 0;
		}
}
static void task_data(void)
{
	 RTC_Set_Time3=Timer;
  	   if((pm25_ok==1)||(voc_ok==1)||(co2_ok==1)||(co_ok==1)||(tmp_ok==1)||(hmp_ok==1))
   {
		   		 //	六合一设备 检测时间YYMMDDHHMMSS湿度、温度、PM2.5、CO2、CO、VOC。
		  	//data_buffer[0]=(RTC_Set_Time3.w_year>>8)&0xFF;
			//data_buffer[1]=RTC_Set_Time3.w_year&0xFF;
			data_buffer[0]=RTC_Set_Time3.w_year%100;
			data_buffer[1]=RTC_Set_Time3.w_month;
			data_buffer[2]=RTC_Set_Time3.w_date;
		    data_buffer[3]=RTC_Set_Time3.hour;
			data_buffer[4]=RTC_Set_Time3.min;
			data_buffer[5]=RTC_Set_Time3.sec;
		if(hmp_ok==1)
		 {
			data_buffer[6]=(Humidity>>8)&0xFF;
			data_buffer[7]=Humidity&0xFF;
			if((Humidity<20)||(Humidity>80))
				hum_LED=3;
			  else  if(((Humidity>=20)&&(Humidity<=30))||((Humidity>=70)&&(Humidity<=80)))
				hum_LED=2;
				else
				   hum_LED=1;

		
		 }		   
		if(tmp_ok==1)
		 {	
			data_buffer[8]=((Temperature+50)>>8)&0xFF;
			data_buffer[9]=(Temperature+50)&0xFF;

			if((Temperature<11)||(Temperature>37))
				tmp_LED=3;
			  else  if(((Temperature>=11)&&(Temperature<=18))||((Temperature>=28)&&(Temperature<=37)))
				tmp_LED=2;
				else
				   tmp_LED=1;

		 }
		 if(voc_ok==1)
		 {
			data_buffer[10]=((u16)(100*VOC_Data)>>8)&0xFF;
			data_buffer[11]=(u16)(100*VOC_Data)&0xFF;

			if((100*VOC_Data)<=10)
				voc_LED=1;
			  else  if((100*VOC_Data)>=100)
				voc_LED=3;
				else
				   voc_LED=2;
		  }
		 if(pm25_ok==1)
		  {
			data_buffer[12]=(PM25_Value_1>>8)&0xFF;
			data_buffer[13]=PM25_Value_1&0xFF;
		   	if((PM25_Value_1)<=65)
				pm25_LED=1;
			  else  if((PM25_Value_1)>250)
				pm25_LED=3;
				else
				   pm25_LED=2;
		    if(pm25_test_st==1)
		     {
		   	  send_data_init=0;
			  pm25_test_st=0;
		     }



		  }
		  if(co_ok==1)
		  {
			data_buffer[14]=(CO_Data>>8)&0xFF;
			data_buffer[15]=CO_Data&0xFF;

			if((CO_Data)<=50)
				co_LED=1;
			  else  if((CO_Data)>1000)
				co_LED=3;
				else
				   co_LED=2;


		  }

		 if(co2_ok==1)
		  {
			data_buffer[16]=(CO2_Data>>8)&0xFF;
			data_buffer[17]=CO2_Data&0xFF;
			if((CO2_Data)<=1000)
				co2_LED=1;
			  else  if((CO2_Data)>5000)
				co2_LED=3;
				else
				   co2_LED=2;

		  }

		  

	   }


	 
}

static void vncl_4010_1(void)
{


		VCNL4000_MeasurementResult1(&VCNL4000_ALS_Result1,&VCNL4000_PROX_Result1);	//测量结果存放在VCNL4000_ALS_Result和VCNL4000_PROX_Result中

	   
  	  if(VCNL4000_1_num>1)
		 VCNL4000_1=0;
//	 if(((VCNL4000_PROX_Result1-VCNL4000_PROX_Result1_1 )>180)&&((VCNL4000_PROX_Result1_2-VCNL4000_PROX_Result1 )>180)&&( vncl4010_Flag==0))
	  if(VCNL4000_PROX_Result1_2>VCNL4000_PROX_Result1)
		 {
			  if(((VCNL4000_PROX_Result1_2-VCNL4000_PROX_Result1 )>80)&&( vncl4010_Flag==0))
			 {
			    VCNL4000_1=1;
				VCNL4000_1_num=0;
				}
		}
	  if(VCNL4000_1==1)
		VCNL4000_1_num++;
	  else
	    VCNL4000_1_num=0; 
  	  

		
	if((VCNL4000_1==1)&&(VCNL4000_2==1)&&( vncl4010_Flag==0))	 //翻页
	{
	     if(oled_Flag_1==1)
		   {
		     
		    oled_time_Flag=1;

			if( BAT_Value_down==1)
			{
			   OLED_curPos=6;
		  	  LCD2_update();
			 }
			 else
		    	LCD_time_update();
			
			 
		    OLED_Display_On();
			if((!Sound_Vs)&&(BAT_Value_down==0))
			time_sound();
		   
		   }
		else  if(BAT_Value_down==0)
		   {
		    sound_oled=0;
		  // oled_Flag_1=0 ;
		  
			   if(oled_time_Flag==1)
			   {
			   	if(OLED_curPos == 0)
					OLED_curPos = 9 - 1;
				else
					OLED_curPos--;
				}
				 oled_time_Flag=0;
		   DisplayProcess(1) ;
		   }
//		   VCNL1=0;
//		   VCNL2=0;
		   oled_Flag=0;
		   oled_Flag_1=0 ;
		    vncl4010_Flag=1;
			VCNL4000_1=0;
			VCNL4000_2=0;
	   }


  


	//if((VCNL4000_PROX_Result1_1- VCNL4000_PROX_Result1)>200)
	 if	((VCNL4000_PROX_Result1_2<VCNL4000_PROX_Result1)&& (VCNL1==0))
	   {
	       if((VCNL4000_PROX_Result1- VCNL4000_PROX_Result1_2)>100)

	       VCNL1=1;
		   VCNL4000_PROX_Result1_1= VCNL4000_PROX_Result1;
	   }
	 if(VCNL4000_PROX_Result1_1> VCNL4000_PROX_Result1)
	   {
	    // if((VCNL4000_PROX_Result1_2- VCNL4000_PROX_Result1)>300)
	     VCNL1=0;
	   }

//	 if((VCNL4000_PROX_Result1_1- VCNL4000_PROX_Result1)>100)
//	    VCNL4000_1=1;
//	 else
//	   VCNL4000_1=0;



   	 if((VCNL1==1)&&(VCNL1_num<32))
	  VCNL1_num++;
	 else
	 {
	  VCNL1=0;
	  VCNL1_num=0;
	  }

 if((VCNL1_num>=20)&&(VCNL2_num>=20)&&( vncl4010_Flag_1==0))	//	 语音
	  {
		      VCNL1_num=0;
			  VCNL2_num=0;
		  VCNL_page=1;
		  vncl4010_Flag_1=1;
//		  VCNL1=0;
//		  VCNL2=0;		   
		  if(Sound_Vs==0)
			 {
		     Sound_Vs=1;
			 sound_oled=0;
			 sound_C=0;
			 }
		  else if(Sound_Vs)
		  {
		   	Sound_Vs=0;
			
			}
//		  flash_read(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
//		  p_data_read[1] =Sound_Vs&0xffff;
//		  flash_write(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
		  oled_Flag=0;
		  oled_Flag_1=0 ;
		  OLED_curPos=7;
		  oled_time_Flag=0;
		  OLED_Display_On();
		  DisplayProcess(1) ;
	  }
 
 
	   VCNL4000_PROX_Result1_2= VCNL4000_PROX_Result1;


}

static void vncl_4010_2(void)
{
//		u8 i;
		 if((vncl4010_Flag==1)&&(VCNL_page_num>3))
 	    // if((vncl4010_Flag==1)&&(VCNL_page==0&&(VCNL_page_num>15)))
	       {
		   vncl4010_Flag=0;
		   VCNL_page_num=0;
		   }
		   if(vncl4010_Flag_1==1)
			  {
			  vncl4010_Flag=1;
			  VCNL_page_num=0;
			  }
		   else if(vncl4010_Flag==1)
		   VCNL_page_num++;

		  if((VCNL1==0)&&(VCNL2==0))
		   vncl4010_Flag_1=0;

		VCNL4000_MeasurementResult2(&VCNL4000_ALS_Result2,&VCNL4000_PROX_Result2);	//测量结果存放在VCNL4000_ALS_Result和VCNL4000_PROX_Result中
//		VCNL4000_ReadAllRegisters2(VCNL4000_Register2);							//读出VCNL4000中所有寄存器的值
//		printf("\r\n\r\n           The result of measurement2\r\n");				//用串口查看数据
//		printf("VCNL4000_ALS_Result2 = %-8d VCNL4000_PROX_Result2 = %-4d.\r\n",VCNL4000_ALS_Result2,VCNL4000_PROX_Result2);
//		printf("\r\n           The value of registers2\r\n");
//		for(i=0;i<12;i++)						                                                
//		{
//			printf("VCNL4000_Register2[%d] = %x\r\n",i,VCNL4000_Register2[i]);	//打印出寄存器的值,注意了是以十六进制打印出来的。
//		}
	 	if(VCNL4000_2_num>1)
		 VCNL4000_2=0;
//	 if(((VCNL4000_PROX_Result2- VCNL4000_PROX_Result2_1)>180)&&((VCNL4000_PROX_Result2_2- VCNL4000_PROX_Result2)>180)&&( vncl4010_Flag==0))
	 
	 	 if(VCNL4000_PROX_Result2_2> VCNL4000_PROX_Result2)
		  {
			  if(((VCNL4000_PROX_Result2_2- VCNL4000_PROX_Result2)>80)&&( vncl4010_Flag==0)) 
			  {
			    VCNL4000_2=1;
				 VCNL4000_2_num=0;
				}
			}
	 if(VCNL4000_2==1)
		VCNL4000_2_num++;
	  else
	    VCNL4000_2_num=0;
	  if((VCNL4000_1==1)&&(VCNL4000_2==1)&&(vncl4010_Flag==0))
	     {
			 if(oled_Flag_1==1)
			 {
			    oled_time_Flag=1;
				if( BAT_Value_down==1)
				{
				   OLED_curPos=6;
			  	   LCD2_update();
				 }
				 else
		    	LCD_time_update();

			    OLED_Display_On();
				if((!Sound_Vs)&&(BAT_Value_down==0))
				time_sound();
				}
			else if (BAT_Value_down==0)
			   {
			    sound_oled=0;
			  // oled_Flag_1=0 ;
			 
				   if(oled_time_Flag==1)
			      {
				  	OLED_curPos++;
					if(OLED_curPos == 9)
						OLED_curPos = 0;
				   }
				   oled_time_Flag=0;
			   DisplayProcess(2) ;
			   }
//			VCNL1=0;
//		    VCNL2=0;
			oled_Flag=0;
		    oled_Flag_1=0 ;
		    vncl4010_Flag=1;
			VCNL4000_1=0;
			VCNL4000_2=0;

		  }
	   



	 if(	(VCNL4000_PROX_Result2_2<VCNL4000_PROX_Result2)&& (VCNL2==0))
	   {
	       if((VCNL4000_PROX_Result2- VCNL4000_PROX_Result2_2)>100)
		   {
		 //     VCNL4000_PROX_Res2=VCNL4000_PROX_Result2;
	          VCNL2=1;
			  VCNL4000_PROX_Result2_1= VCNL4000_PROX_Result2;
		   }
	   }
	// if(ABS(VCNL4000_PROX_Res2,VCNL4000_PROX_Result2) <= 50)

	 if(VCNL4000_PROX_Result2_1> VCNL4000_PROX_Result2)
	   {
	    // if((VCNL4000_PROX_Result2_2- VCNL4000_PROX_Result2)>300)
	     VCNL2=0;
	   }

	   	 

//	   if((VCNL4000_PROX_Result2_2- VCNL4000_PROX_Result2)>100)
//	      VCNL4000_2=1;
//	   	else
//	      VCNL4000_2=0;




  	   if((VCNL2==1)&&(VCNL2_num<32))
		  VCNL2_num++;
		 else
		 {
		 VCNL2=0;
		  VCNL2_num=0;

		  }

		 if((VCNL1_num>=20)&&(VCNL2_num>=20)&&( vncl4010_Flag_1==0))
		  {
		      VCNL1_num=0;
			  VCNL2_num=0;
			  VCNL_page=1;
			  vncl4010_Flag_1=1;
//			  VCNL1=0;
//			  VCNL2=0;
			  if(Sound_Vs==0)
			  {
			   	Sound_Vs=1;
				sound_oled=0;
				sound_C=0;
				}
			  else if(Sound_Vs)
			  {
			   	Sound_Vs=0;
				
				}
//			  flash_read(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
//			  p_data_read[1] =Sound_Vs&0xffff;
//			  flash_write(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
			  oled_Flag=0;
			   oled_Flag_1=0 ;
			   OLED_curPos=7;
			   oled_time_Flag=0;
			   OLED_Display_On();
			   DisplayProcess(1) ;
		  }

		VCNL4000_PROX_Result2_2= VCNL4000_PROX_Result2;



}
/*
*********************************************************************************************************
** 函数名称:task_1ms()
** 函数功能 :1ms定时任务
** 入口参数 :无
** 出口参数 :无
*********************************************************************************************************
*/
u8 led_t=40 ;
u8 led_fag=0 ;
u8 led_t_1=100 ;
static void task_1ms(void)
{
//	   	  led_t++;
//	  if(led_t>led_t_1)
//	  {
//		  PM25_fan=1;
//		  if(led_t>led_t_1+2)
//		   led_t=40;
//	   }
//	  else
//	    PM25_fan=0;

	//static u16 da;
	//while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
	//USART_SendData(USART1,da++);
  // if(mm==0)
  // MP3_Play();
}

/*
*********************************************************************************************************
** 函数名称:task_5ms()
** 函数功能 :1ms定时任务
** 入口参数 :无
** 出口参数 :无
*********************************************************************************************************
*/

static void task_5ms(void)
{
//	   	  led_t++;
//	  if(led_t>led_t_1)
//	  {
//		  PM25_fan=1;
//		  if(led_t>led_t_1+5)
//		   led_t=0;
//	   }
//	  else
//	    PM25_fan=0;
//  	   if((pm25_ok==1)||(voc_ok==1)||(co2_ok==1)||(co_ok==1)||(tmp_ok==1)||(hmp_ok==1))
//   {
//		   		 //	六合一设备 检测时间YYMMDDHHMMSS湿度、温度、PM2.5、CO2、CO、VOC。
//		  	data_buffer[0]=RTC_Set_Time3.w_year>>8&0xFF;
//			data_buffer[1]=RTC_Set_Time3.w_year&0xFF;
//			data_buffer[2]=RTC_Set_Time3.w_month;
//			data_buffer[3]=RTC_Set_Time3.w_date;
//		    data_buffer[4]=RTC_Set_Time3.hour;
//			data_buffer[5]=RTC_Set_Time3.min;
//			data_buffer[6]=RTC_Set_Time3.sec;
//		if(hmp_ok==1)
//		 {
//			data_buffer[8]=Humidity>>8&0xFF;
//			data_buffer[9]=Humidity&0xFF;
//		 }		   
//		if(hmp_ok==1)
//		 {	
//			data_buffer[10]=(Temperature+50)>>8&0xFF;
//			data_buffer[11]=(Temperature+50)&0xFF;
//		 }
//		 if(co2_ok==1)
//		  {
//			data_buffer[12]=CO2_Data>>8&0xFF;
//			data_buffer[13]=CO2_Data&0xFF;
//		  }
//		 if(pm25_ok==1)
//		  {
//			data_buffer[14]=PM25_Value_1>>8&0xFF;
//			data_buffer[15]=PM25_Value_1&0xFF;
//		  }
//		 if(co_ok==1)
//		  {
//			data_buffer[16]=CO_Data>>8&0xFF;
//			data_buffer[17]=CO_Data&0xFF;
//		  }
//	    if(voc_ok==1)
//		 {
//			data_buffer[18]=(u16)VOC_Data>>8&0xFF;
//			data_buffer[19]=(u16)VOC_Data&0xFF;
//		  }
//	   }
	
	    
//if(power_flag==0)	

   if(wifi_set==0)
	 { 
	   if(WIFI==0)
	   {
	     
	      wifi_set=1;
		  	      
	      WIFI_flag_view=1;
		  WIFI_Flag=0;
		  wifi_oled_set=1;
		  wifi_oled_set_num=0;
		  sound_oled=0;
		  sound_oled_num=0;
		  oled_time_Flag=0;
		  oled_Flag_1=0 ;
		  oled_Flag=0;
		  OLED_curPos=7;  
		  LCD2_update();
		 OLED_Display_On();
		 	 sound_C=1;
		 USART5_TX_State2();
		 LOGIN_CMD_Flag_num=0;
		 Second_LOGIN_CMD_Flag_num=0;	 
		}
	  }
	 if(wifi_set==1)
	 {
	   wifi_set_num++;
	 }
	 if(wifi_set_num>=600)
	 {
	   wifi_set_num=0;
	   wifi_set=0;
	   LOGIN_CMD_Flag=0;
	   Second_flg=0;
	   heart_init=0 ;
	 }


	if(WIFI_Flag==1)
	   WIFI_flag_view=0;

	USART1_RX_Timer++;
	USART2_RX_Timer++;	
	USART4_RX_Timer++;
	USART5_RX_Timer++;
//	   if(Set_mac_Flag==0)
//		   {
//	  		while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
//			USART_SendData(USART1,0x03);   //mac地址
//		   }	

//		task_data();
		 //if(Sensor_Sample_Flag)   //	


}// task_5ms()
/*
*********************************************************************************************************
** 函数名称:task_5ms()
** 函数功能 :10ms定时任务
** 入口参数 :无
** 出口参数 :无
*********************************************************************************************************
*/
 	u16 led0pwmval=0;    
	u8 dir=1;
static void task_10ms(void)
{
 
     	
//
//     	if(dir)
//		   {led0pwmval++;
//		   led0pwmval++;}
//		 else 
//		   {led0pwmval--;
//		   led0pwmval--;}	 
// 		if(led0pwmval>980)
//		     dir=0;
//		if(led0pwmval<=0)
//		     dir=1;	   					 
//	LED0_PWM_VAL=led0pwmval;
	//if(PM25_Detect_Timer2++ > 8)
	//{
	//PM25_Detect_Timer2=0;
	   if(BAT_Value_down==0)	
		{
		   	if(PM25_Detect_Flag)       //PM2.5 sample flag
			{ 
			  
					pm25_ok=0;
			 
			//		fan_run(Fan_Flag);	          //fan control
					if(PM25_Detect_Timer++<pm25_test_mun)	  //sample 10 times
					{  
					    PM25_fan=1;
						PM25_LED=1;
			
						TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
						//Timer_280us_Flag=1;
						//Timer_40us_Flag=1;
						//delay_us(320);
						delay_us(320);
						ADC_Cmd(ADC1, DISABLE);
						ADC_SoftwareStartConvCmd(ADC1, DISABLE);
						PM25_LED=0;
					}
					else
					{
						PM25_Detect_Timer=0;
						//Sensor_Sample_Flag=0;
						PM25_Detect_Flag=0;
						PM25_fan=0;
						pm25_test_mun=550;	
					}
			    }
	

			}

   			else
			{
					TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
					//Timer_280us_Flag=1;
					//Timer_40us_Flag=1;
					//delay_us(320);
					

					ADC_Cmd(ADC1, DISABLE);
					ADC_SoftwareStartConvCmd(ADC1, DISABLE);
					PM25_LED=0;
					PM25_fan=0;
			
			}

		 	 ADC_Process();

}// task_10ms()

static void task_20ms(void)

{


}
/*
*********************************************************************************************************
** 函数名称:task_5ms()
** 函数功能 :100ms定时任务
** 入口参数 :无
** 出口参数 :无
*********************************************************************************************************
*/
task_25ms(void)
{
      vncl_4010_1();
//    u8 i;
//		 	if((led_t_1>1)&&(led_fag==0)&&(led_t_1<45) )
// 	 led_t_1=led_t_1 - 1;
//	 else if (led_t_1<40)
//	   {
//		   led_t_1=led_t_1 + 1;	
//		   led_fag=1 ;
//	   }
//	   else
//		   led_fag=0 ;
  	   
//		VCNL4000_MeasurementResult1(&VCNL4000_ALS_Result1,&VCNL4000_PROX_Result1);	//测量结果存放在VCNL4000_ALS_Result和VCNL4000_PROX_Result中
////		VCNL4000_ReadAllRegisters1(VCNL4000_Register1);							//读出VCNL4000中所有寄存器的值
////		printf("\r\n\r\n           The result of measurement1\r\n");				//用串口查看数据
////		printf("VCNL4000_ALS_Result1 = %-8d VCNL4000_PROX_Result1 = %-4d.\r\n",VCNL4000_ALS_Result1,VCNL4000_PROX_Result1);
////		printf("\r\n           The value of registers1\r\n");
////		for(i=0;i<12;i++)
////		{
////			printf("VCNL4000_Register1[%d] = %x\r\n",i,VCNL4000_Register1[i]);	//打印出寄存器的值,注意了是以十六进制打印出来的。
////		}
//	if((VCNL4000_PROX_Result1_1- VCNL4000_PROX_Result1)>200)
//	   VCNL4000_1=1;
//	 else
//	   VCNL4000_1=0;
//	if((VCNL4000_1==1)&&(VCNL4000_2==1)&&( vncl4010_Flag==0))
//	{
//	     if(oled_Flag_1==1)
//		   {
//		     
//		    oled_time_Flag=1;
//			LCD_time_update();
//		   OLED_Display_On();
//		   
//		   }
//		else
//		   {
//		  // oled_Flag_1=0 ;
//		   oled_time_Flag=0;
//		   DisplayProcess(1) ;
//		   }
//		   oled_Flag=0;
//		   oled_Flag_1=0 ;
//		    vncl4010_Flag=1;
//	   }
//	   VCNL4000_PROX_Result1_1= VCNL4000_PROX_Result1;
}


task_75ms(void)	
{
	   vncl_4010_2();
////		u8 i;
//		VCNL4000_MeasurementResult2(&VCNL4000_ALS_Result2,&VCNL4000_PROX_Result2);	//测量结果存放在VCNL4000_ALS_Result和VCNL4000_PROX_Result中
////		VCNL4000_ReadAllRegisters2(VCNL4000_Register2);							//读出VCNL4000中所有寄存器的值
////		printf("\r\n\r\n           The result of measurement2\r\n");				//用串口查看数据
////		printf("VCNL4000_ALS_Result2 = %-8d VCNL4000_PROX_Result2 = %-4d.\r\n",VCNL4000_ALS_Result2,VCNL4000_PROX_Result2);
////		printf("\r\n           The value of registers2\r\n");
////		for(i=0;i<12;i++)
////		{
////			printf("VCNL4000_Register2[%d] = %x\r\n",i,VCNL4000_Register2[i]);	//打印出寄存器的值,注意了是以十六进制打印出来的。
////		}
//
//	   if((VCNL4000_PROX_Result2_2- VCNL4000_PROX_Result2)>200)
//	      VCNL4000_2=1;
//	   	else
//	      VCNL4000_2=0;
//	  if((VCNL4000_1==1)&&(VCNL4000_2==1)&&(vncl4010_Flag==0))
//	     {
//			 if(oled_Flag_1==1)
//			 {
//			    oled_time_Flag=1;
//				LCD_time_update();
//			    OLED_Display_On();
//				}
//			else
//			   {
//			  // oled_Flag_1=0 ;
//			   oled_time_Flag=0;
//			   DisplayProcess(2) ;
//			   }
//			oled_Flag=0;
//		   oled_Flag_1=0 ;
//		    vncl4010_Flag=1;
//
//		  }
//	   VCNL4000_PROX_Result2_2= VCNL4000_PROX_Result2;

}
/*
static void task_50ms(void)

{
 

}

static void task_100ms(void)

{
         
//		   LED_red();
//    	   if((led_hx_dir==0)&&(led_hx_num<0XAE))
//		    { 
//			  led_hx_num++;		  
//		   	  HT1632C_Write_CMD(led_hx_num);
//			}
//		   if(led_hx_num>=0XAE)
//			  {
//			   led_hx_num=0XAE;
//			   led_hx_dir=1;
//			  }
//    	   if((led_hx_dir==1)&&(led_hx_num>0XA0))
//		    { 
//			  led_hx_num--;		  
//		   	  HT1632C_Write_CMD(led_hx_num);
//			}
//			if(led_hx_num<=0XA0)
//			  { 
//			   led_hx_dir=0;
//			   led_hx_num=0XA0	;
//	 		  }
//	if((led_t_1>6)&&(led_fag==0)&&(led_t_1<125) )
// 	 led_t_1=led_t_1 - 2;
//	 else if (led_t_1<100)
//	   {
//		   led_t_1=led_t_1 + 2;	
//		   led_fag=1 ;
//	   }
//	   else
//		   led_fag=0 ;
////	  ;
//    u8 i;
//  
//		VCNL4000_MeasurementResult1(&VCNL4000_ALS_Result1,&VCNL4000_PROX_Result1);	//测量结果存放在VCNL4000_ALS_Result和VCNL4000_PROX_Result中
//		VCNL4000_ReadAllRegisters1(VCNL4000_Register1);							//读出VCNL4000中所有寄存器的值
//		printf("\r\n\r\n           The result of measurement1\r\n");				//用串口查看数据
//		printf("VCNL4000_ALS_Result1 = %-8d VCNL4000_PROX_Result1 = %-4d.\r\n",VCNL4000_ALS_Result1,VCNL4000_PROX_Result1);
//		printf("\r\n           The value of registers1\r\n");
//		for(i=0;i<12;i++)
//		{
//			printf("VCNL4000_Register1[%d] = %x\r\n",i,VCNL4000_Register1[i]);	//打印出寄存器的值,注意了是以十六进制打印出来的。
//		}
//		 
//		
//		VCNL4000_MeasurementResult2(&VCNL4000_ALS_Result2,&VCNL4000_PROX_Result2);	//测量结果存放在VCNL4000_ALS_Result和VCNL4000_PROX_Result中
//		VCNL4000_ReadAllRegisters2(VCNL4000_Register2);							//读出VCNL4000中所有寄存器的值
//		printf("\r\n\r\n           The result of measurement2\r\n");				//用串口查看数据
//		printf("VCNL4000_ALS_Result2 = %-8d VCNL4000_PROX_Result2 = %-4d.\r\n",VCNL4000_ALS_Result2,VCNL4000_PROX_Result2);
//		printf("\r\n           The value of registers2\r\n");
//		for(i=0;i<12;i++)
//		{
//			printf("VCNL4000_Register2[%d] = %x\r\n",i,VCNL4000_Register2[i]);	//打印出寄存器的值,注意了是以十六进制打印出来的。
//		}

	//key_process();		 //key process		
//	RTC_Display();		 //RTC process				
//	//Vac_Process();			//test vac			
//	//Runtime_Process();  //runtimer
//	//Beep(Beep_Flag);  
//	RTC_Time_Set();

//	if(Beep_Counter++ >= 1)
//	{
//		Beep_Flag=0;
//		Beep_Counter=200;
//	}	
//	if(Parameter_Time_Set_Flash_Counter++ >=5)
//	{
//		Parameter_Time_Set_Flash_Counter=0;	
//		{	
//			//Key_Press_Counter++;
//			Parameter_Time_Set_Flash = ~Parameter_Time_Set_Flash;
//		}	
//	}	
		
}// task_100ms()
*/
/*
*********************************************************************************************************
** 函数名称:task_5ms()
** 函数功能 :1s定时任务
** 入口参数 :无
** 出口参数 :无
*********************************************************************************************************
*/
static void task_300ms()
{


	 task_data();

    if((co_LED==1)&&(tmp_LED==1)&&(co2_LED==1)&&(hum_LED==1)&&(voc_LED==1)&&(pm25_LED==1))
		 {
		     if(senser_level==3)
			{
			   senser_level=1;
			  send_data_init=0;
			  }
			  else
			  {
				 senser_level=1;
				 send_data_init_one=1;
			 }
		 }

		 else  if((co_LED==3)||(tmp_LED==3)||(co2_LED==3)||(hum_LED==3)||(voc_LED==3)||(pm25_LED==3))
		  {
		      if(senser_level==3)
			    send_data_init_one=1;
				else
				send_data_init_one=0;
			   senser_level=3;
		    
		  }

		  else
		  {
		    if(senser_level==3)
			{
			   senser_level=2;
			  send_data_init=0;
			  }
			  else
			  {
				senser_level=2;
				send_data_init_one=1;
			  }
		   }
	 if((senser_level==3)&&(send_data_init_one==0))
	     send_data_init=0;

   if (oled_Flag_1==1)
		sound_C=0;
 
}	
 u8 PM25_fan_num=0;
static void task_1s(void)
{ 	 


  		 u8   data=0;
	RTC_Set_Time2=Timer;	
	  	




		 LED_sysrun=~LED_sysrun;
  //   	LED_sysrun=~LED_sysrun;
		  //  vncl_4010_1();



		if((oled_time_Flag==1)&&( BAT_Value_down==0))
		  LCD_time_update();
	    else
		 {
		   if( BAT_Value_down==1)
		   OLED_curPos=6;
	  	  LCD2_update();
		  }
		if(!PM25_Detect_Flag)       //PM2.5 sample flag
	     { 
		    PM25_fan_num++;
		  if(PM25_fan_num>=30)
		  {
		     PM25_fan=~PM25_fan;
			 PM25_fan_num=0;
			 }
		  }

        if((sound_oled==1)&&(!Sound_Vs)&&(BAT_Value_down==0))
            sound_oled_num++;
			else
			sound_oled_num=0;
		if(sound_oled_num==7)
		 {
		    OLED_curPos=0;
			
		    USART5_TX_State1()	;
		   LCD2_update();
		 }
		else if(sound_oled_num==12)
		 {
		    OLED_curPos=1;
		
			USART5_TX_State1();	
		 		LCD2_update();
		 }
		 else if(sound_oled_num==17)
		 {
		    OLED_curPos=2;
		
			USART5_TX_State1();	
		 		LCD2_update();
		 }
		 else if(sound_oled_num==22)
		 {
		    OLED_curPos=3;
		
			USART5_TX_State1();	
		 		LCD2_update();
		 }
		 else if(sound_oled_num==27)
		 {
		    OLED_curPos=4;
			
			USART5_TX_State1();	
		 	LCD2_update();
		 }
		 else if(sound_oled_num==32)
		 {
		    OLED_curPos=5;
		
			USART5_TX_State1();	
		 		LCD2_update();
		 }
		 else if(sound_oled_num==37)
		 {
		    OLED_curPos=8;
			LCD2_update();
		    sound_oled=0;	
		 
		 }	
    
	   

	 //      test_imid =1;
       pm25_test();



	   	if((RTC_Set_Time2.hour==0 )&&(RTC_Set_Time2.min==30 )&&(RTC_Set_Time2.sec==30 )&&(WIFI_Flag==1))
		 Set_time_Flag=0;

	   if((Set_time_Flag==0)&&(WIFI_Flag==1))
	   send_msg_to_wifi( TIME_CMD,&data,0);	//校时

 	   if((Set_mac_Flag==0)&&(BAT_Value_down==0))
		   {
	  	    Mac_TX_State();   //mac地址
		   }

// 	   if((Set_mac_Flag==1)&&(internet__send_flag==0))
//		   {
//		    
//            Internet_TX_State();
//		   }
		if(internet==1)
		{
		    internet=0;
			WIFI_internet_Flag=0;
			USART5_TX_State3();
		 }
		if(internet==2)
		{
		   
		    internet=0;
			WIFI_internet_Flag=1; // 1连接路由器

			if(WiFi_server_flag==2)
			USART5_TX_State4();

			LOGIN_CMD_Flag_num=0;
		 }

	   if( BAT_Value_down==0)
		{ 	
				//	if((LOGIN_CMD_Flag==0)&&(Set_mac_Flag==1)&&(WIFI_internet_Flag==1))	 //登录
				   if((LOGIN_CMD_Flag==0)&&(Set_mac_Flag==1)&&(LOGIN_CMD_Flag_num<120))	 //登录
					{
					    LOGIN_CMD_Flag_num++;
						send_msg_to_wifi_1( LOGIN_CMD,&Mac_msg[0],6); 
						SE_HEART_CMD_Flag=1;
					}

					  if(((LOGIN_CMD_Flag_num==120)||(Second_LOGIN_CMD_Flag_num==60))&&(WIFI_Flag==0))
					   {
					   LOGIN_CMD_Flag_num=130;
					    Second_LOGIN_CMD_Flag_num=0;

			            Internet_TX_State();
					   }
			
			
			         //登录成功，心跳
			
					Second_flg++;
					Second5_flg++;
					Second51_flg++;
			
					if((LOGIN_CMD_Flag==1)&&(heart_init==0)&&(WIFI_Flag==0))
					{
						 if((SE_HEART_CMD_Flag==1)&&(RE_HEART_CMD_Flag==1))
				
							{
							   
							    WIFI_Flag=1;
								SE_HEART_CMD_Flag=0;
								RE_HEART_CMD_Flag=0;
			
							  }
						 else  if(Second_flg<=50)
								 {  

			                       if(Second51_flg>=5)
								   {                      
									   send_msg_to_wifi( HEART_CMD,&data,0);
									   SE_HEART_CMD_Flag=1;
									   Second51_flg=0;
								   }
					
								 } 
			 	     }
			
				 
				 if(Second_flg>=60)
				 {

					 heart_init=1;
					 
						 //登录成功，心跳
					if(LOGIN_CMD_Flag==1)
					{
					   if((SE_HEART_CMD_Flag==1)&&(RE_HEART_CMD_Flag==1))
				
						{
						   Second_flg=Second_flg-60 ;
						    WIFI_Flag=1;
							SE_HEART_CMD_Flag=0;
							RE_HEART_CMD_Flag=0;
						   	send_msg_to_wifi( HEART_CMD,&data,0);
						    SE_HEART_CMD_Flag=1;
						  }
						else  if(Second_flg<=110)
								 {  

								   if(Second5_flg>=5)
									  {
										   send_msg_to_wifi( HEART_CMD,&data,0);
										   SE_HEART_CMD_Flag=1;
										   Second5_flg=0;
									   }					  
							
								 } 
								 else
								 {
								  Second_flg=Second_flg-60 ;
								  WIFI_Flag=0;
								  WIFI_internet_Flag=0;
								  
								  LOGIN_CMD_Flag=0;
								  heart_init=0;
								  send_msg_to_wifi_1( LOGIN_CMD,&Mac_msg[0],6); 
						          SE_HEART_CMD_Flag=1;
								  LOGIN_CMD_Flag_num=120;
					              Second_LOGIN_CMD_Flag_num=60;
			
								  }
				
			 		}
							 
					 else 
					 {
					   if((LOGIN_CMD_Flag==0)&&(Set_mac_Flag==1))	 //登录
					      send_msg_to_wifi_1( LOGIN_CMD,&Mac_msg[0],6); 
						  SE_HEART_CMD_Flag=1;
				       
					 }
				 
				 }
			
			  
				  if((SE_DATA_CMD_Flag==1)&&(RE_DATA_CMD_Flag==0)&&(send_i_len_num<20))
				  {
					   Second_SE_DATA_flg++;
					   if(Second_SE_DATA_flg>=5)
					   {
						    Second_SE_DATA_flg=0 ;
						    send_i_len_num++;
							send_msg_to_wifi( DATA_CMD,&seng_data_buffer[0],seng_data_num);
						}
				  }
			
				 frequency_num++;
			
			
				if(time_num_20s<=30)
				  {
				    time_num_20s++;
					 if(time_num_20s>=20)
						{
						time_num_20s=40	;
						send_data_init=0;
						}
				   }
			
			
			
				 if((frequency_num>=re_frequency)||(((data_num>0)||((send_data_init==0)&&(time_num_20s>=20)))&&(WIFI_Flag==1) ))
				 {
					 if(send_data_init==0)
					 {
					  send_data_init=1;
					  SE_DATA_CMD_Flag=1;
					  Second_SE_DATA_flg=0;
					  memcpy(seng_data_buffer,&data_buffer,18);
			          send_msg_to_wifi(DATA_CMD,&data_buffer[0],18);
			
					 }
					 else
					 {
			
					   frequency_num=0;
					   send_data_init=1;
					   SE_DATA_CMD_Flag=0;
					   RE_DATA_CMD_Flag=0;
					   frequency_num=0;
					   task_message();
				   }	  
			
				 }
		}
		else
		{
		LOGIN_CMD_Flag=0;
		 WIFI_Flag=0;
		LOGIN_CMD_Flag_num=0;
		heart_init=0;
		time_num_20s=0;
		}	
   IWDG_ReloadCounter();
//	 if(BAT_Value_down==0)
	   led_display();
//	   else
//	   HT1632C_Write_CMD(0x02);//关LED显示?	

	   if(WIFI_Flag==1)
	     WiFi_server_flag=1;
	  if((WIFI_Flag==0)&&(WiFi_server_flag==1))
	     WiFi_server_flag=2;
	
}// task_1s()
/*
*********************************************************************************************************
** 函数名称:task_5ms()
** 函数功能 :2s定时任务
** 入口参数 :无
** 出口参数 :无
*********************************************************************************************************
*/
static void task_2s(void)
{ 
  //  u8 data_ceshi_w[2]={0x01,0x01} ;
   if(VCNL_page==1)
	   VCNL_page=0;



	  if(BAT_Value<=3100)
	     BAT_Value_down=1;
	if(BAT_Value>=3200)
		 BAT_Value_down=0;

  	 //  if(!PM25_Detect_Flag) 
	//	  {
		 if(BAT_Value_down==0)
		 { 
		   tmp_ok=0;
		  	Sht21Work(0xf3);
			hmp_ok=0;
			Sht21Work(0xf5);
		}	
		//  LCD2_update();





	//	} 

   		  if(wifi_oled_set==1)
	         wifi_oled_set_num++;

		 if( wifi_oled_set_num>=42)
		 	{
		 	  wifi_oled_set=0;
		      wifi_oled_set_num=0;

			  if(WIFI_Flag==0)
			  {
			  	WIFI_flag_view=0;
	   			if(internet__send_flag_0==1)
			    internet__send_flag=0;
				WIFI_internet_Flag=0;
			  
			  }
		    }

//		if(WIFI_internet_send_Flag==1)
//		  { 
//		   WIFI_internet_send__num_Flag++;
//		   if((WIFI_internet_send__num_Flag==4)&&(internet__send_flag==0)&&(WIFI_Flag==0))
//		 	 USART5_TX_State4();
//		   }
//			else
//			   WIFI_internet_send__num_Flag=0;
		if((Ceshi1==0)||(PBin(1)==0)||(PCin(4)==0)||(PCin(5)==0))
		 {
		   
		   	 if(Ceshi1_num%20==0)
		   	 {
			   if((VCNL4000_PROX_Result1_1>0X01)&&(VCNL4000_PROX_Result1_1<0XFFFF)&&(VCNL4000_PROX_Result2_2>0X01)&&(VCNL4000_PROX_Result2_2<0XFFFF))
				  VNCL_ceshi=1;
			   else
			      VNCL_ceshi=0;
			   if((senser_th_flag==0)&&(senser_co_flag==0)&&(senser_voc_flag==0)&&(senser_co2_flag==0)&&(senser_pm_flag==0))
		          senser_ceshi=1;
				else
				  senser_ceshi=0;
				  Ceshi1_num++;
				  USART5_TX_ceshi_1();
			   }
		   }
		   else
		      Ceshi1_num=0;
		if((Ceshi_flash==0)||(PDin(3)==0)||(PDin(4)==0)||(PDin(6)==0)||(PDin(7)==0))
	 	  {	
			  flash_read(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
			  p_data_read[25] =0x25&0xffff;
			  flash_write(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);

			RTC_Set(2000,1,1,1,1,1);  //设置时间
//			SPI_Flash_Write(data_ceshi_w, 0xffffff00,2); 
			Ceshi_flash=1;
		   }
//  
}// task_2s()

static void task_3s(void)
{ 
 
 
 
//  HSHCAL001B_ReadMultiByte(I2C_Temp,0x0c,0x08);
//  I2C_Temp[8]=HSHCAL001B_ReadOneByte(0x18);
//  I2C_Temp[9]=HSHCAL001B_ReadOneByte(0x1B);
//  HSHCAL001B_ReadMultiByte(&I2C_Temp[10],0x20,0x04);
//  Temp_Humi_process(Temp_Humi,&I2C_Temp[4]);
  //IAQ_Read(&Iaq_Data[0], 15);
  //Value_Tvoc=Get_TVOC();
  	   	 if(BAT_Value_down==0) 
		  {
		  	voc_ok=0;
			EC808_Process();
			co_ok=0;
			EC805_Process();

		}  
	 if(BAT_Value_down==0)
		  {
		  	co2_ok=0;
		   
			USART3_TX_State(); //co2
		
		}
//  
}// task_2s()
/*
*********************************************************************************************************
** 函数名称:task_5ms()
** 函数功能 :10s定时任务
** 入口参数 :无
** 出口参数 :无
*********************************************************************************************************
*/
 u8 Mac_msg0[10];
static void task_10s(void)
{ 
//	  u8 i;
       oled_Flag++;
   if(oled_Flag>5)
    { 
	 sound_C=0;
	 OLED_Display_Off();
	 oled_Flag_1=1;
	 oled_time_Flag=1;
	 LCD_time_update();
	 }
//	 OLED_up();
  


   	 	  // if((Set_mac_Flag==1)&&(internet__send_flag==0)&&(WIFI_internet_Flag==0))
//		    if((internet__send_flag==0)&&(WIFI_internet_Flag==0)&&(WIFI_internet_send_Flag==0))
//		   {
//	//	    internet__send_flag_0=1;
//	        WIFI_internet_send_Flag=1;
//            Internet_TX_State();
//		   }


//	syn6658();  
		  if((ceshu_flash_sound==1)&&(ceshu_flash_sound_num<5))
		  {
		   USART5_TX_ceshi_2();
		   ceshu_flash_sound_num++;
		  }
//  printf("{id:2,temp:%0.2d,hum:%0.2d,co2:%0.2d,pm25:%0.2d,co:%0.2d,voc:%0.2f}\r",
//       Temperature,Humidity,CO2_Data,PM25_Value_1,CO_Data,VOC_Data );
}// task_10s()

/*
*********************************************************************************************************
** 函数名称:task_5ms()
** 函数功能 :1m定时任务
** 入口参数 :无
** 出口参数 :无
*********************************************************************************************************
*/

static void task_1m(void)
{  
//   u8   data=0;
//		//硬件的MAC地址是由17位ASCII码组成，如"00-50-56-85-43-FF"。在传输过程中采用17个字节进行存储。
//	Mac_msg[0]=0x00;		
//	Mac_msg[1]=0x50;
//	Mac_msg[2]=0x56;
//	Mac_msg[3]=0x85;
//   	Mac_msg[4]=0x43;
//	Mac_msg[5]=0xff;
//
//	num++;
//
//	   if(BAT_Value_down==0)
//
//		PM25_Vs_Flag = 0;
//		else
//		 PM25_Vs_Flag = 1;

//		if((RTC_Time_Set_flag>1000 )&&(WIFI_Flag==1))
//		 Set_time_Flag=0;
		 
//		 
//		 //登录成功，心跳
//		if(LOGIN_CMD_Flag==1)
//		{
//		   	if((SE_HEART_CMD_Flag==1)&&(RE_HEART_CMD_Flag==1))
//
//				{
//				 
//				    WIFI_Flag=1;
//					SE_HEART_CMD_Flag=0;
//					RE_HEART_CMD_Flag=0;
//				  }
//			else
//				 {  
//				    SE_HEART_CMD_Flag=0;
//					RE_HEART_CMD_Flag=0;
//				    WIFI_Flag=0;
//				  
//				 } 
//			send_msg_to_wifi( HEART_CMD,&data,0);
//
//			SE_HEART_CMD_Flag=1;
//	
//		}
//		 else
//		   send_msg_to_wifi_1( LOGIN_CMD,&Mac_msg,6); 

//		   if((WIFI_Flag==1)&&(num>=re_frequency))
//		   {
//		   	 num=0;
//		   	 send_msg_to_wifi( DATA_CMD,&data_buffer,0);
//		   }
	/*
	if(Sensor_Sample_Flag_Timer++ >=5)	  //sample's time is 5 min
	{
		Sensor_Sample_Flag_Timer=0;
		Sensor_Sample_Flag=1;	
	}
	*/
//
//	if((Temp_Humi[0]/100) <= 50)
//    	Beta=100;
//	else if((Temp_Humi[0]/100) > 50)
//		Beta=100-((Temp_Humi[0]) - 5000)*1467/100000;


}

static void task_5m(void)
{
 	   if(BAT_Value_down==0)

		PM25_Vs_Flag = 0;
		else
		 PM25_Vs_Flag = 1;

 }
/*
static void send_message(void)
{
u8   data=0;
		//硬件的MAC地址是由17位ASCII码组成，如"00-50-56-85-43-FF"。在传输过程中采用17个字节进行存储。



	
//	if(Sensor_Sample_Flag_Timer++ >=5)	  //sample's time is 5 min
//	{
//		Sensor_Sample_Flag_Timer=0;
//		Sensor_Sample_Flag=1;	
//	}
	
//
//	if((Temp_Humi[0]/100) <= 50)
//    	Beta=100;
//	else if((Temp_Humi[0]/100) > 50)
//		Beta=100-((Temp_Humi[0]) - 5000)*1467/100000;
//		PM25_Vs_Flag = 0;
		 
		 //登录成功，心跳
		if(LOGIN_CMD_Flag==1)
		{
		   	if((SE_HEART_CMD_Flag==1)&&(RE_HEART_CMD_Flag==1))

				{
				 
				    WIFI_Flag=1;
					SE_HEART_CMD_Flag=0;
					RE_HEART_CMD_Flag=0;
				  }
			else
				 {  
				    SE_HEART_CMD_Flag=0;
					RE_HEART_CMD_Flag=0;
				    WIFI_Flag=0;
				  
				 } 
			send_msg_to_wifi( HEART_CMD,&data,0);

			SE_HEART_CMD_Flag=1;
	
		}
		 else if(Mac_msg[17]==1)
		       send_msg_to_wifi_1( LOGIN_CMD,&Mac_msg[0],6); 
		      else
			    {
//				 while( USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET );
//		            USART_SendData(USART1,0X03);
				
				 Mac_TX_State();
				}




}
*/

u8 data_num_buf[15];

static void task_message(void)
{
   

  if(WIFI_Flag==0)
  {	 
		  SPI_Flash_Read(data_num_buf,0x0000,11);
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
		   data_num_buf[10]=0XAA;

		   SPI_Flash_Write(data_num_buf, 0x000000,11)  ;
		 }
		 else
		 {	 
			  data_num=	((data_num_buf[0]<<24)&0xff000000)+((data_num_buf[1]<<16)&0x00ff0000)
			            +((data_num_buf[2]<<8)&0x0000ff00)+((data_num_buf[3])&0x000000ff);
			  data_started=((data_num_buf[4]<<16)&0x00ff0000)+((data_num_buf[5]<<8)&0x0000ff00)
			             +((data_num_buf[6])&0x000000ff);;
              data_ended=((data_num_buf[7]<<16)&0x00ff0000)+((data_num_buf[8]<<8)&0x0000ff00)
			            +((data_num_buf[9])&0x000000ff);
		   }

   		   SPI_Flash_Write(data_buffer, data_ended,18) ;
		   if(data_num==0)
		   {
			data_started=0x00000fff;
			data_ended=0x00000fff;
			}
		   data_num=data_num+1;
		   data_size= data_num*18;
		   data_ended=data_ended+18;
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

//		   data_start=0x000fff;
//           data_end=data_start+data_size;
		   data_num_buf[10]=0xAA;
		    
		   

		   SPI_Flash_Write(data_num_buf, 0x0000,11)  ;
			//	data_size= data_num*20;
			 //  SPI_Flash_Read(re_data_buffer,0x0fff,data_size); 
	}
    else
	  {
	       send_i_len_num=0;
		   SE_DATA_CMD_Flag=1;
		   Second_SE_DATA_flg=0;
		   SPI_Flash_Read(data_num_buf,0x0000,11);
		 if(data_num_buf[10]!=0xAA)
		 { 
		   //SPI_FLASH_BulkErase();
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
		   send_msg_to_wifi(DATA_CMD,&data_buffer[0],18);
		 }
		 else	 
		 {	  
			  data_num=	((data_num_buf[0]<<24)&0xff000000)+((data_num_buf[1]<<16)&0x00ff0000)
			            +((data_num_buf[2]<<8)&0x0000ff00)+((data_num_buf[3])&0x000000ff);
			  data_started=((data_num_buf[4]<<16)&0x00ff0000)+((data_num_buf[5]<<8)&0x0000ff00)
			             +((data_num_buf[6])&0x000000ff);
              data_ended=((data_num_buf[7]<<16)&0x00ff0000)+((data_num_buf[8]<<8)&0x0000ff00)
			            +((data_num_buf[9])&0x000000ff);
			   if(  data_num==0)
			    {	
				    seng_data_num=18;
					memcpy(seng_data_buffer,&data_buffer,seng_data_num);
			        send_msg_to_wifi(DATA_CMD,&data_buffer[0],18);
	
				 }
			   else if(data_num<=10)
		 	   {
			 	   SPI_Flash_Read(re_data_buffer,data_started,data_num*18);
				   seng_data_num=data_num*18;

				   memcpy(seng_data_buffer,&re_data_buffer,seng_data_num); 
				   send_msg_to_wifi( DATA_CMD,&re_data_buffer[0],data_num*18);
					   
				   data_num=0;
				   data_started=0x000fff;
		           data_ended=0x000fff;
				  // SPI_FLASH_BulkErase();
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
				   SPI_Flash_Write(data_num_buf, 0x0000,11)  ;
		 	   }
			   else	if (data_num>10)
			   {
					 if(data_num_10==0)

					 {

				   	SPI_Flash_Read(re_data_buffer,data_started,10*18);
					seng_data_num=10*18;
					memcpy(seng_data_buffer,&re_data_buffer,seng_data_num); 
				   send_msg_to_wifi( DATA_CMD,&re_data_buffer[0],seng_data_num);
					data_num=data_num-10;

					if(data_num>10)
					   data_num_10=6 ;

					data_started=data_started+10*18;

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
				    data_num_buf[10]=0XAA;
					SPI_Flash_Write(data_num_buf, 0x0000,11)  ;
					}
					else if(data_num_10>0)
						  data_num_10--;
			   }
	      }		 
		 
	}	 
		    



}
/*
*********************************************************************************************************
** 函数名称 :task()
** 函数功能 :设定系统运行的时间片
** 入口参数 :无
** 出口参数 :无
*********************************************************************************************************
*/

//static void task(void)
void task(void)
{		
	static u8 ms100_Counter, Second_Counter,Second_Counter2,Second_Counter3,Second_Counter5,Second_Counter10,Min_Counter,Min5_Counter;
	
	if(Task_Flag)
	{

		if((vncl_Sys_Counter >= 80)&&(task_75_Flag==0))     //1ms 
		{	
			   task_75ms();
				VCNL4000_WriteOneByte1(VCNL4000_CMD,0x88);
				vncl_Sys_Counter=0;
				task_75_Flag=1;
				task_25_Flag=0;	
		}
		if((vncl_Sys_Counter >=40 )&&(task_25_Flag==0))     //1ms

		{			
			task_25ms();
			task_75_Flag=0;
			task_25_Flag=1;	
			VCNL4000_WriteOneByte2(VCNL4000_CMD,0x88);
		}
	if(Sys_Counter % 1 == 0)     //1ms 
		task_1ms();
	if(Sys_Counter % 5 == 0)     //5ms     
		task_5ms();	
			
    if(Sys_Counter % 10 == 0)     //10ms 
	 {
//	   if(task_10_Flag==0)
//	   {
//	     task_10_Flag=1;
  	     task_10ms();
		// }
	}
	    if(Sys_Counter % 20 == 0)     //10ms 
	 {
//	   if(task_10_Flag==0)
//	   {
//	     task_10_Flag=1;
  	     task_20ms();
		// }
	}
		    if(Sys_Counter % 50 == 0)     //10ms 
	 {
//	   if(task_10_Flag==0)
//	   {
//	     task_10_Flag=1;
//  	     task_50ms();
		// }
	}

//	if(Sys_Counter >= 25)   //100ms  
//	{	
//		if(task_25_Flag==0)
//		{
//			task_25ms();	
//			task_25_Flag=1;
//				VCNL4000_WriteOneByte2(VCNL4000_CMD,0x88);
//		}
//	}
//	if(Sys_Counter >= 75)   //100ms  
//	{
//
//	   	if(task_75_Flag==0)
//		{
//			task_75ms();	
//			task_75_Flag=1;
//			//	VCNL4000_WriteOneByte2(VCNL4000_CMD,0x88);
//				VCNL4000_WriteOneByte1(VCNL4000_CMD,0x88);
//		}		
//	}
	
	if(Sys_Counter %100==0)   //100ms  
	{	
	//	task_100ms();	
		Second_Counter++;
		ms100_Counter++;	
    	

	//	task_25_Flag=0;
//		task_75_Flag=0;
		task_10_Flag=0;
	}
		 if(Sys_Counter  >=100)   //100ms  
	{	
	//	task_150ms();	
		Sys_Counter=0;	
		
	}
	 	if(ms100_Counter >= 3)   //100ms  
	{	
		task_300ms();	
		ms100_Counter=0;	
	
		
	}
		
	if(Second_Counter  >= 10)  //1s 	
	{	
		task_1s();
		Second_Counter=0;
		Second_Counter2++;
		Second_Counter10++;
		 Second_Counter3++;
		Second_Counter5++;
		 

//		Key_Press_Counter++;		
	}	
	if(Second_Counter2  >= 2)  //2s 	
	{	
		task_2s();
		Second_Counter2=0;
	}
		if(Second_Counter3  >= 3)  //2s 	
	{	
		task_3s();
		Second_Counter3=0;
	}
			if(Second_Counter5  >= 5)  //2s 	
	{	
	//	task_5s();
		Second_Counter5=0;
	}	
	if(Second_Counter10  >= 10)  //10s 	
	{	
		task_10s();
		Second_Counter10=0;
		Min_Counter++;
	}	
	if(Min_Counter  >= 6)  //1M 	
	{	
		task_1m();
	   Min_Counter=0;
	   Min5_Counter++;
	}
	if(Min5_Counter  >= 2)  //10M 	
	{	
		task_5m();
//	RTC_Time_Set_flag++;
		Min5_Counter=0;
	}	
	Task_Flag=0;
  }
	
} // task()



