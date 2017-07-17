/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * 文件名  ：adc.c
 * 描述    ：adc应用函数库        
 * 实验平台：野火STM32开发板
 * 硬件连接：----------------------------------------------------
 *          |                                                    |
 *          | PC1 - ADC1 连接外部电压(通过一个滑动变阻器分压而来)|
 *          |                                                    |
 *           ----------------------------------------------------
 * 库版本  ：ST3.5.0 
 *
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/
//#include "adc.h"
#include <math.h>
#include "gpio.h"
#include"sys.h"
#include "ec808.h"
#include "stm32f10x.h"

#define ADC1_DR_Address    ((u32)0x40012400+0x4c)

extern void disp_date(u8 data[32],u8 addr_start,u8 count);
extern float abs(float num);
extern void Parameter_Flash_write(void);

//
//int  temperature[234] =//-121~~+112
//{//0    1     2     3     4     5     6     7     8     9   
//	4096,	4077, 4059,	4041,	4022,	4004,	3986,	3967,	3949,	3931,	   	//0
//	3912,	3894, 3876,	3858,	3840,	3821,	3803,	3785,	3767,	3749,	   	//10
//	3730,	3712, 3694,	3675,	3658,	3639,	3621,	3603,	3585,	3567,			//20
//	3548,	3531, 3513,	3494,	3476,	3458,	3440,	3422,	3404,	3386,			//30
//	3368,	3350, 3332,	3314,	3296,	3278,	3260,	3242,	3224,	3206,			//40
//	3188,	3170, 3152,	3134,	3116,	3099,	3081,	3063,	3045,	3027,			//50
//	3009,	2991, 2973,	2955,	2937,	2920,	2902,	2884,	2866,	2848,			//60
//	2830,	2812, 2795,	2777,	2759,	2741,	2724,	2706,	2688,	2670,			//70
//	2653,	2635, 2617,	2599,	2581,	2564,	2546,	2528,	2510,	2493,			//80
//	2475,	2457, 2440,	2422,	2405,	2387,	2369,	2351,	2334,	2316,			//90
//	2298,	2281, 2263,	2245,	2228,	2210,	2193,	2175,	2157,	2140,			//100
//	2122,	2104, 2087,	2069,	2052,	2034,	2017,	1999,	1982,	1964,			//110
//	1947,	1929, 1912,	1894,	1877,	1859,	1842,	1824,	1807,	1789,		  //120
//	1772, 1754,	1737,	1719,	1702,	1684,	1667,	1649,	1632,	1614,			//130
//	1597, 1580,	1562,	1545,	1527,	1510,	1493,	1475,	1458,	1440,			//140
//	1423, 1405,	1388,	1371,	1353,	1336,	1318,	1301,	1284,	1266,			//150
//	1249, 1232,	1214,	1197,	1180,	1163,	1145,	1128,	1111,	1093,			//160
//	1076, 1059,	1042,	1024,	1007,	 990,	 973,	 955,	 938,	 921,	 		//170
//	 903,  886,	 869,	 852,	 835,	 817,	 800,	 783,	 766,	 749,	 		//180
//	 732,  714,	 697,	 680,	 663,	 646,	 629,	 611,	 594,	 577,	 		//190
//	 560,  543,	 526,	 509,	 492,	 475,	 458,	 441,	 423,	 406,	 		//200
//	 389,  372,	 355,	 338,	 321,	 304,	 287,	 270,	 253,	 236,	 		//210
//	 218,  201,	 185,	 168,	 151,	 134,	 117,	 100,	  83,	  49,     //220
//	  32,	  15,	   0																		            //230
//};
//
//int16_t  temperature_ADJ[14][2] =//-121~~+112
//{
//	{-113,1062},{-103,1068},{-84,1071},{-65,1077},{-46,1087},{-27,1111},{-9,1111},{9,1111},{26,1154},{43,1163},
//	{60,1167},{76,1184},{92,1196},{94,1191}	
//};

extern u8 data_led[16];
extern u8 data_disp[32];
extern u8 Coldtrap_Flash;
extern u8 Sample_Flash;
//extern u8 Parameter_Time_Set_Flash;
//extern u8 Parameter_Set_Read;
extern u8 Set_Number;
//extern u8 Key_Press_Counter;
extern u8 KeyNum;
//extern u8 parameter_Set_Flag;
extern u8 Coldtrap_Flag;
extern u8 Model_Flash;	
extern u8 Model_Flag;	 
extern u8 Sample_Flag;
extern u8 pm25_ok ;
extern u16 Humidity;

u16 AdcTemp[4][10];
		
int16_t ADC_ConvertedValueLocal[6];  // 局部变量，用于保存转换计算后的电压值			
//int16_t ADC_ConvertedValueLocal_temp[4][1000];
int16_t ADC_ConvertedValueLocal_PM25[2000];
int16_t ADC_ConvertedValueLocal_ADC0[200];
int16_t ADC_ConvertedValueLocal_ADC2[200];
int16_t ADC_ConvertedValueLocal_ADC3[200];
int16_t ADC_ConvertedValueLocal_ADC4[200];
int16_t ADC_ConvertedValueLocal_ADC5[200];

int16_t Coldtrap_Tem;
int16_t Sample_Tem;
int16_t Sample_Value_Set;
int16_t Coldtrap_Value_Set;
int16_t Sample_Value_Set_Temp;
int16_t Coldtrap_Value_Set_Temp;
int16_t ADC_temp0,ADC_temp1,ADC_temp2,ADC_temp3;

u8 Coldtrap_data[3];
u16 Coldtrap_Tem_Temp;
u8 Sample_data[3];
u16 Sample_Tem_Temp;	
u32 PM25_Temp;
u16 PM25_Vo;
//u16 PM25_Vs=0x058C;
u16 PM25_Vs=0x0cd0;
u16 PM25_k =50;
u8 senser_pm_flag=0;
u8 	PM25_Cal=0x05;
int16_t PM25_Value_Temp[1000];
u16 BAT_Value;
u16 CO_Value;

u16 PM25_Value_temp[5];

u16 PM25_Value;
u16 PM25_Value_1=5;
u16 PM25_Value_2;
u16 PM25_Value_3;
u16 PM25_Value_4;
u16 PM25_Value_base =0x38;
u8  Alpha=0x06;
u8  Beta=0x64;
extern u8 ADC_Start_Flag;
u8 ADC_Start_Flag_Counter=0;
u16 ADC_Converted_Temp[20];
u16 PM25_Value_filter[5];			
u8 Inc_Flag=0;
u8 Dec_Flag=0;
extern u8 PM25_Vs_Flag;	


extern u8 USART_TX_BUF[64];     //发送缓冲,最大64个字节.
extern u32 Flash_Write_Address;
extern u8 Flash_Data_Length;
extern int32_t p_data_write[56];
extern int32_t p_data_read[56];



//__IO uint16_t ADC_ConvertedValue[4];// ADC1转换的电压值通过MDA方式传到SRAM
//u16 ADC_ConvertedValue[4];// ADC1转换的电压值通过MDA方式传到SRAM
uint16_t ADC_ConvertedValue[20];// ADC1转换的电压值通过MDA方式传到SRAM
/*
* 函数名：ADC1_GPIO_Config
 * 描述  ：使能ADC1和DMA1的时钟，初始化PC.01
 * 输入  : 无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void ADC1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);
	
//	/* Configure PA.00  as analog input */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);				// PA0,输入时不用设置速率

	/* Configure PA.01  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);				// PA1,输入时不用设置速率 
	
//	/* Configure PA.02  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);				// PA2,输入时不用设置速率

//	/* Configure PB.00  as analog input */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);				// PA3,输入时不用设置速率 
//	
//	
//	/* Configure PB.01  as analog input */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//	GPIO_Init(GPIOC, &GPIO_InitStructure); 
//		/* Configure PB.01  as analog input */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//		/* Configure PB.01  as analog input */
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}



/* 函数名：ADC1_Mode_Config
 * 描述  ：配置ADC1的工作模式为MDA模式
 * 输入  : 无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void ADC1_Mode_Config(void)
{


	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 //ADC地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 6;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	/* ADC1 configuration */
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//独立ADC模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 //禁止扫描模式，扫描模式用于多通道采集
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//开启连续转换模式，即不停地进行ADC转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//不使用外部触发转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//采集数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 6;	 	//要转换的通道数目1
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*配置ADC时钟，为PCLK2的8分频，即9Hz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	/*配置ADC1的通道11为55.	5个采样周期，序列为1 */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_7Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_7Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_7Cycles5);	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_7Cycles5);	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 5, ADC_SampleTime_7Cycles5);	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 6, ADC_SampleTime_7Cycles5);		
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/*复位校准寄存器 */   
	ADC_ResetCalibration(ADC1);
	/*等待校准寄存器复位完成 */
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	/* ADC校准 */
	ADC_StartCalibration(ADC1);
	/* 等待校准完成*/
	while(ADC_GetCalibrationStatus(ADC1));
	
	/* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}



/*
 * 函数名：ADC1_Init
 * 描述  ：无
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}


//void TemperaturePrint(int16_t* AdcTemp1,u16* AdcTemp2)
void ADC_Data_process(int16_t* AdcTemp1,u16* AdcTemp2)
{
    u16 i=0;
	//j,k;
   u32 temp=0 , temp0=0;    
//	u32 temp2=0;  
	//u16 ADC_Converted_MAX,ADC_Converted_MIN;

	 //for(i=0;i<16;i++)
	 //{
	 	//ADC_Converted_Temp[i]= *AdcTemp2;  		
		//AdcTemp2++;	
	 //}
	//////////////ADC0/////////////




	ADC_ConvertedValueLocal_ADC0[0]=*AdcTemp2;
	*AdcTemp1=ADC_ConvertedValueLocal_ADC0[0];

		for(i=200;i>0;i--)
	{
		ADC_ConvertedValueLocal_ADC0[i]=ADC_ConvertedValueLocal_ADC0[i-1];
		temp0=temp0+ ADC_ConvertedValueLocal_ADC0[i];
	}
	  temp0=temp0/200;	
	 *AdcTemp1=temp0; 
	AdcTemp1++;
	AdcTemp2++;	

		
	/////////////ADC1/////////////     
	for(i=399;i>0;i--)
	{
		ADC_ConvertedValueLocal_PM25[i]=ADC_ConvertedValueLocal_PM25[i-1];
		temp=temp+ ADC_ConvertedValueLocal_PM25[i];
	}
	if(ADC_Start_Flag)
	{
		if(ADC_Start_Flag_Counter++ < 10)
	  		ADC_ConvertedValueLocal_PM25[0]=ADC_ConvertedValueLocal_PM25[0];
		else
		{
			ADC_ConvertedValueLocal_PM25[0]=*AdcTemp2;
			ADC_Start_Flag=0;												  	
		}
	}
	else
		ADC_ConvertedValueLocal_PM25[0]=*AdcTemp2;
	temp=temp+ ADC_ConvertedValueLocal_PM25[0];	
	temp=temp/400;	
	//temp= (temp-ADC_ConvertedValueLocal_PM25[1999]-ADC_ConvertedValueLocal_PM25[1998]-ADC_ConvertedValueLocal_PM25[1997]-ADC_ConvertedValueLocal_PM25[1996]-ADC_ConvertedValueLocal_PM25[1995]-ADC_ConvertedValueLocal_PM25[1994]-ADC_ConvertedValueLocal_PM25[1993]-ADC_ConvertedValueLocal_PM25[1992]-ADC_ConvertedValueLocal_PM25[1991]-ADC_ConvertedValueLocal_PM25[1990])/1990;			
	*AdcTemp1=temp;
	AdcTemp1++;
	AdcTemp2++;	
	///////////ADC2/////////////
	ADC_ConvertedValueLocal_ADC2[0]=*AdcTemp2;
	*AdcTemp1=ADC_ConvertedValueLocal_ADC2[0];
	
	AdcTemp1++;
	AdcTemp2++;

	///////////ADC3/////////////
	ADC_ConvertedValueLocal_ADC3[0]=*AdcTemp2;
	*AdcTemp1=ADC_ConvertedValueLocal_ADC3[0];
	
	AdcTemp1++;
	AdcTemp2++;	
	///////////ADC4/////////////
	ADC_ConvertedValueLocal_ADC4[0]=*AdcTemp2;
	*AdcTemp1=ADC_ConvertedValueLocal_ADC4[0];
	
	AdcTemp1++;
	AdcTemp2++;
		///////////ADC5/////////////
	ADC_ConvertedValueLocal_ADC5[0]=*AdcTemp2;
	*AdcTemp1=ADC_ConvertedValueLocal_ADC5[0];
	
}

/*
int16_t temperature1_ADJ(int16_t temperature_temp)
{
	u8 j=0;
	int16_t temp;	
	while(temperature_temp > temperature_ADJ[j++][0]);
	//temp=0-temperature_temp;
	if((temperature_temp == temperature_ADJ[j-1][0]) || (j == 1))
		temp=temperature_ADJ[j-1][1];
	else
	{			
		//temp=(temperature_temp-temperature_ADJ[j-1][1])/(temperature_ADJ[j][1]-temperature_ADJ[j-1][1])+temperature_ADJ[j-1][1];
		temp=temperature_ADJ[j-2][1];
		temp=(temperature_temp-temperature_ADJ[j-2][0])*100*(temperature_ADJ[j-1][1]-temperature_ADJ[j-2][1]);
		temp=temp/(temperature_ADJ[j-1][0]-temperature_ADJ[j-2][0])/100+temperature_ADJ[j-2][1];
	
		
		//temperature_ADJ[j-2][1];
		//temp=(temperature_temp-temperature_ADJ[j-2][0])/(temperature_ADJ[j-1][0]-temperature_ADJ[j-2][0])+temperature_ADJ[j-1][1];
	}	
	return temp;
}
*/

 void ADC_Process(void)
{
//	static u16 i;
	u16 j;
	static u8 k,number;
//	u16 temp;
//	u16  Voltage_Data;


	//	if(SET==DMA_GetFlagStatus(ADC_FLAG_JEOC))	  
		if(SET==DMA_GetFlagStatus(DMA1_FLAG_TC1))   //xgy 20140504
//  if(SET==DMA_GetFlagStatus(DMA1_FLAG_GL1))   //xgy 20140504
	{
	 


	//BEEP=0;
	///////////////////////////////////////coldtrap C/////////////////////////////////////// 
	ADC_Data_process(&ADC_ConvertedValueLocal[0],&ADC_ConvertedValue[0]);
	//PM25_Value_Temp[i++] = ADC_ConvertedValueLocal[3];
	BAT_Value = ADC_ConvertedValueLocal[0];
//	Voltage_Data =ADC_ConvertedValueLocal[2]>> 10;
//	Voltage_Data= ((Voltage_Data * 625) >> 16);
//	CO_Value =Voltage_Data*100/65;
		for(j=100;j>0;j--)
	{
		PM25_Value_Temp[j]=PM25_Value_Temp[j-1];
		//temp=temp+ ADC_ConvertedValueLocal_PM25[i];
	}


	PM25_Value_Temp[0] = ADC_ConvertedValueLocal[1];
	number++; 
	if(number>=100)
	{
		number=100;
		//i=0; 	
		PM25_Temp=0;
		for(j=0;j<100;j++)
		{
			PM25_Temp=PM25_Temp+(u32)PM25_Value_Temp[j];
		}
		PM25_Temp=PM25_Temp/100;
		//PM25_Vo=PM25_Temp*660/4095;
		PM25_Vo=PM25_Temp*6600/4095/2+PM25_Vo/2;
		//if(PM25_Temp >= PM25_Vs)
		//{
			//PM25_Temp=(PM25_Temp-PM25_Vs)*113/100;
			//PM25_Value=	PM25_Temp;
			//if(PM25_Value >= PM25_Cal)
				//PM25_Value=	PM25_Value - PM25_Cal;
		//} 
		/**
		if(PM25_Vo >= PM25_Vs)
		{
			PM25_Value=(PM25_Vo-PM25_Vs)*Beta*Alpha/100;	
		} 
		**/
		//PM25_Vs=PM25_Vs*100;
		if(PM25_Vo>100)
		  {
			if(PM25_Vo >= PM25_Vs)//&&(Humidity<=50))
			{
			   PM25_Value=(PM25_Vo-PM25_Vs)*Beta*Alpha/1000;
			//	PM25_Value=(PM25_Vo-PM25_Vs)*Beta*Alpha*4/20000;	
			}
//			else 	if((PM25_Vo >= PM25_Vs)&&(Humidity>50))	
//			{
//				//PM25_Value=(PM25_Vo-PM25_Vs)*Beta*Alpha*4*(1-0.01467*(Humidity-50))/20000;
//				PM25_Value=(PM25_Vo-PM25_Vs)*Beta*Alpha*(1-0.01467*(Humidity-50))/1000;
//			}																						 
//	    	else if(( PM25_Vs_Flag==0)&&(PM25_Vo>=100)&&(BAT_Value>3600))  //			   3350
//	
//		         {
//				  //Parameter_Flash_write();
//				   PM25_Vs =PM25_Vo*5/100+PM25_Vs*95/100;
//				  flash_read(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
//				  p_data_read[0] =PM25_Vs&0xffff;
//				  flash_write(Flash_Write_Address,&p_data_read[0],(u32)Flash_Data_Length);
//				  //flash_write(Flash_Write_Address,&p_data_write[0],(u32)Flash_Data_Length);
//		 		   //flash_write(Flash_Write_Address,&p_data_write[0],(u32)Flash_Data_Length);
//				  // PM25_Vs =PM25_Vo;
//			     }
	
			PM25_Value_filter[k]= PM25_Value;
			if(k==1)																	  
			{
				if(PM25_Value_filter[1] >= PM25_Value_filter[0])
				{
					Inc_Flag=1;
					Dec_Flag=0;	
				}
				else
				{
				 	Dec_Flag=1;	
					Inc_Flag=0;
				}
			}
			if(Inc_Flag)
			{
				if(PM25_Value_filter[k] < PM25_Value_filter[0])
				{
					Inc_Flag=0;
					k=0;
				}
			} 
			else if(Dec_Flag)
			{
				if(PM25_Value_filter[k] > PM25_Value_filter[0])
				{
					Dec_Flag=0;
					k=0;
				}
			}
			if(k++ >= 4)
			{
				k=0;
				PM25_Value_2 = (PM25_Value_filter[0] +PM25_Value_filter[1] + PM25_Value_filter[2] + PM25_Value_filter[3] + PM25_Value_filter[4])/5; 
	//				USART_TX_BUF[0]= (u8)PM25_Value;
					//USART_TX_BUF[1]= (PM25_Temp >> 8);
					//USART_TX_BUF[2]= PM25_Temp;
	//				USART_TX_Run_State_updata(1);
			} 
			//	if(PM25_Value_2>PM25_Value_base)

			 // PM25_Value_2=0.4*PM25_Value_2;	
		      PM25_Value_3 =(PM25_Value_2+3*PM25_Value_3)/4;

				PM25_Value_4=PM25_Value_3*PM25_Value_3*0.0006+0.18*PM25_Value_3+4;
				PM25_Value_4= PM25_Value_4*0.6;
				PM25_Value_1=  data_lvbo(PM25_Value_4,&PM25_Value_temp[0],&PM25_Value_temp[0]);

					//	if(75*PM25_Value_3/100>54)
//				   PM25_Value_1=0.3*PM25_Value_3;
//				   else
//					PM25_Value_1=5;
			   if(PM25_Value_1<=5)
			      PM25_Value_1=5;
			   if(PM25_Value_1>=999)
			      PM25_Value_1=999;
			  pm25_ok=1;
			  senser_pm_flag=0;			
		 }
		 else
		  senser_pm_flag=7;
	}  	
	DMA_ClearFlag(DMA1_FLAG_TC1);
	}
}


//ADC中断服务函数
void ADC1_2_IRQHandler(void)
//void ADC1_IRQHandler(void)
{
	//adcval=ADC1->DR;
	//printf("ADC:%d\r\n",adcval);
	ADC1->SR&=~(1<<1);	//清除中断
}

/*
void DMA1_Channel1_IRQHandler(void)
{
	if(SET==DMA_GetFlagStatus(DMA1_FLAG_TC1))   //xgy 20140504
	{	
   		BEEP=1;
		DMA_ClearFlag(DMA1_FLAG_TC1);
	}
}
*/

