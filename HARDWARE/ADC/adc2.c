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
#include "adc.h"
#include <math.h>

#define ADC1_DR_Address    ((u32)0x40012400+0x4c)

extern void disp_date(u8 data[32],u8 addr_start,u8 count);
extern float abs(float num);
//__IO u16 ADC_ConvertedValueLocal;
/*
int  temperature[235] =
{
		//0    1     2     3     4     5     6     7     8     9   
		4096, 4077, 4055, 4037, 4019, 4000, 3982, 3964, 3945, 3927,
		3909, 3891, 3872, 3854, 3836, 3818, 3799, 3781, 3763, 3745,
		3727, 3708, 3690, 3672, 3654, 3636, 3618, 3600, 3582, 3564,
		3545, 3527, 3509, 3491, 3473, 3455, 3437, 3419, 3401, 3383,
		3365, 3347, 3329, 3311, 3293, 3275, 3257, 3239, 3221, 3203,
		3185, 3167, 3149, 3132, 3114, 3096, 3078, 3060, 3042, 3024,
		3006, 2988, 2970, 2953, 2935, 2917, 2899, 2881, 2864, 2846,
		2828, 2810, 2792, 2774, 2756, 2739, 2721, 2703, 2686, 2668,
		2650, 2632, 2614, 2597, 2579, 2561, 2544, 2526, 2508, 2491,
		2473, 2455, 2437, 2420, 2402, 2384, 2367, 2349, 2332, 2314,
		2296, 2279, 2261, 2243, 2226, 2208, 2191, 2173, 2155, 2138,
		2120, 2102, 2085, 2068, 2050, 2033, 2015, 1997, 1962, 1980,
		1945, 1927, 1927, 1910, 1892, 1875, 1857, 1840, 1822, 1805,
		1787, 1770, 1752, 1735, 1717, 1700, 1683, 1665, 1648, 1630,
		1613, 1596, 1578, 1561, 1543, 1526, 1508, 1491, 1474, 1456,
		1439, 1421, 1404, 1387, 1369, 1352, 1335, 1317, 1300, 1283,
		1265, 1248, 1231, 1213, 1196, 1179, 1162, 1144, 1127, 1110,
		1092, 1075, 1058, 1041, 1023, 1006,  989,  972,  954,  937,
		 920,  903,  886,  868,  851,  834,  817,  800,  783,  765,
		 748,  731,  714,  696,  679,  662,  645,  628,  611,  594,
		 577,  559,  542,  525,  508,  491,  474,  457,  440,  423,
		 406,  389,  372,  355,  337,  320,  303,  286,  269,  252,
		 235,  218,  201,  185,  168,  151,  134,  117,   99,   82,
			65,   49,   32,   15,    0
};
*/
/*
int  temperature[235] =//-121~~+112
{
		3297,	3282, 3267,	3252,	3238,	3223,	3208,	3194,	3179,	3164,	
		3149,	3135, 3120,	3105,	3091,	3076,	3061,	3047,	3032,	3017,	
		3003,	2988, 2973,	2958,	2944,	2929,	2915,	2900,	2886,	2871,	
		2856,	2842, 2827,	2813,	2798,	2784,	2769,	2754,	2740,	2726,	
		2711,	2696, 2682,	2667,	2653,	2639,	2624,	2610,	2595,	2581,	
		2566,	2552, 2537,	2523,	2509,	2494,	2480,	2465,	2451,	2436,	
		2422,	2407, 2393,	2379,	2364,	2350,	2336,	2321,	2307,	2293,	
		2278,	2264, 2250,	2235,	2221,	2207,	2192,	2178,	2164,	2149,	
		2135,	2121, 2106,	2092,	2078,	2064,	2049,	2035,	2021,	2007,	
		1992,	1978, 1964,	1950,	1935,	1921,	1907,	1893,	1878,	1864,	
		1850,	1836, 1822,	1807,	1793,	1779,	1765,	1751,	1737,	1722,	
		1708,	1694, 1680,	1666,	1652,	1638,	1624,	1609,	1595,	1581,	
		1567,	1553, 1553,	1539,	1525,	1510,	1496,	1482,	1468,	1454,	
		1440,	1426, 1412,	1398,	1384,	1370,	1356,	1341,	1327,	1313,	
		1299,	1286, 1271,	1257,	1243,	1229,	1215,	1201,	1187,	1173,	
		1159,	1145, 1131,	1117,	1103,	1089,	1075,	1061,	1047,	1033,	
		1019,	1006,  992,	 977,	 964,	 950,	 936,	 922,	 908,	 894,	
		 880,	 866,  852,	 838,	 824,	 811,	 797,	 783,	 769,	 755,	
		 741,	 727,  714,	 699,	 686,	 672,	 658,	 644,	 630,	 616,	
		 603,	 589,  575,	 561,	 547,	 534,	 520,	 506,	 492,	 478,	
		 465,	 451,  437,	 423,	 409,	 396,	 382,	 368,	 355,	 341,	
		 327,	 313,  299,	 286,	 272,	 258,	 244,	 231,	 217,	 203,	
		 190,	 176,  162,	 149,	 135,	 121,	 108,	  94,	  80,	  66,	
			53,	  39,   26,	  12,	   0
};*/	

int  temperature[235] =//-121~~+112
{

4096,	4077, 4059,	4041,	4022,	4004,	3986,	3967,	3949,	3931,	
3912,	3894, 3876,	3858,	3840,	3821,	3803,	3785,	3767,	3749,	
3730,	3712, 3694,	3675,	3658,	3639,	3621,	3603,	3585,	3567,	
3548,	3531, 3513,	3494,	3476,	3458,	3440,	3422,	3404,	3386,	
3368,	3350, 3332,	3314,	3296,	3278,	3260,	3242,	3224,	3206,	
3188,	3170, 3152,	3134,	3116,	3099,	3081,	3063,	3045,	3027,	
3009,	2991, 2973,	2955,	2937,	2920,	2902,	2884,	2866,	2848,	
2830,	2812, 2795,	2777,	2759,	2741,	2724,	2706,	2688,	2670,	
2653,	2635, 2617,	2599,	2581,	2564,	2546,	2528,	2510,	2493,	
2475,	2457, 2440,	2422,	2405,	2387,	2369,	2351,	2334,	2316,	
2298,	2281, 2263,	2245,	2228,	2210,	2193,	2175,	2157,	2140,	
2122,	2104, 2087,	2069,	2052,	2034,	2017,	1999,	1982,	1964,	
1947,	1929, 1929,	1912,	1894,	1877,	1859,	1842,	1824,	1807,	
1789,	1772, 1754,	1737,	1719,	1702,	1684,	1667,	1649,	1632,	
1614,	1597, 1580,	1562,	1545,	1527,	1510,	1493,	1475,	1458,	
1440,	1423, 1405,	1388,	1371,	1353,	1336,	1318,	1301,	1284,	
1266,	1249, 1232,	1214,	1197,	1180,	1163,	1145,	1128,	1111,	
1093,	1076, 1059,	1042,	1024,	1007,	 990,	 973,	 955,	 938,	
 921,	 903,  886,	 869,	 852,	 835,	 817,	 800,	 783,	 766,	
 749,	 732,  714,	 697,	 680,	 663,	 646,	 629,	 611,	 594,	
 577,	 560,  543,	 526,	 509,	 492,	 475,	 458,	 441,	 423,	
 406,	 389,  372,	 355,	 338,	 321,	 304,	 287,	 270,	 253,	
 236,	 218,  201,	 185,	 168,	 151,	 134,	 117,	 100,	  83,	
 65,	  49,   32,	  15,	   0															
};

int16_t  temperature_ADJ[14][2] =//-121~~+112
{
	{-113,1062},{-103,1068},{-84,1071},{-65,1077},{-46,1087},{-27,1111},{-9,1111},{9,1111},{26,1154},{43,1163},
	{60,1167},{76,1184},{92,1196},{94,1191}	
};

extern u8 data_led[14];
extern u8 data_disp[32];
u16 AdcTemp[2][8];
u16 ADC_ConvertedValueLocal[4];  // 局部变量，用于保存转换计算后的电压值			
//int16_t ADC_ConvertedValueLocal[4];  // 局部变量，用于保存转换计算后的电压值			
u16 Coldtrap_Tem;
//int16_t Coldtrap_Tem;
u16 Sample_Tem;
u8 Coldtrap_data[3];
u16 Coldtrap_Tem_Temp;
u8 Sample_data[3];
u16 Sample_Tem_Temp;	


//__IO uint16_t ADC_ConvertedValue[4];// ADC1转换的电压值通过MDA方式传到SRAM
u16 ADC_ConvertedValue[4];// ADC1转换的电压值通过MDA方式传到SRAM

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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	/* Configure PA.00  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);				// PA0,输入时不用设置速率
	
	
	/* Configure PA.01  as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);				// PA1,输入时不用设置速率
	
	
	
	
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
	DMA_DeInit(DMA1_Channel1);   //adc1-->dma1
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 //ADC1地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	//DMA_InitStructure.DMA_BufferSize = 1;
	DMA_InitStructure.DMA_BufferSize = 2;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址固定
	//DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  //内存地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存递增模式
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
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 //扫描模式，扫描模式用于多通道采集
	//ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 	 //禁止扫描模式，扫描模式用于多通道采集
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//开启连续转换模式，即不停地进行ADC转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//不使用外部触发转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//采集数据右对齐
	//ADC_InitStructure.ADC_NbrOfChannel = 1;	 	//要转换的通道数目1
	ADC_InitStructure.ADC_NbrOfChannel = 2;	 	//要转换的通道数目2
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*配置ADC时钟，为PCLK2的8分频，即9Hz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	/*配置ADC1的通道11为55.	5个采样周期，序列为1 */ 
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	
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
	
	while(RESET==DMA_GetFlagStatus(DMA1_FLAG_TC1));   //xgy 20140504
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


//u16* TemperaturePrint(u32* AdcTemp)
//void TemperaturePrint(u16* AdcTemp1,u16* AdcTemp2)
void TemperaturePrint(int16_t* AdcTemp1,u16* AdcTemp2)
{
    u8 i=0,j;
    u32 temp=0;    
	  int16_t temp2=0;
    //if(0==channel)
    //{
       
				/**/
			for(j=0;j<2;j++)	
			{
				AdcTemp[j][7]=AdcTemp[j][6];
				AdcTemp[j][6]=AdcTemp[j][5];
				AdcTemp[j][5]=AdcTemp[j][4];
				AdcTemp[j][4]=AdcTemp[j][3];
				AdcTemp[j][3]=AdcTemp[j][2];
				AdcTemp[j][2]=AdcTemp[j][1];
				AdcTemp[j][1]=AdcTemp[j][0];
				AdcTemp[j][0]=*AdcTemp2;				
				temp=(AdcTemp[j][7]+AdcTemp[j][6]+AdcTemp[j][5]+AdcTemp[j][4]+AdcTemp[j][3]+AdcTemp[j][2]+AdcTemp[j][1]+AdcTemp[j][0])/8;
				while(temp<temperature[i++]);
				if((temp-temperature[i-1])/(temperature[i]-temperature[i-1])>=0.5)
					i++;
				temp=i-1;
				/**/
				if(temp<121)   //负温度
				{			
					temp=121-temp;
					temp=temp+4096;					
				}
				else           //正温度
				{
					temp=temp-121;					
				}
			/**/				
				//temp2=temp-121;
				//*AdcTemp1=temp2;
				*AdcTemp1=temp;				
				AdcTemp1++;
				AdcTemp2++;
				i=0;
			}
	      /**/
    //}
    //else
    //{
	    //for(i=0;TEMP_Channel2>=temperature[i];i++);
	    //temp=(TEMP_Channel2-temperature[i-1])*10/(temperature[i]-temperature[i-1]);
	    //temp=(TEMP_Channel2-temperature[i-1])*100/(temperature[i]-temperature[i-1]);
	    //temp=temp+(i<<8);
	    //temp=temp+((i-1)<<8);
    //}
    //return(temp);
}

void ADC_Process(void)
{
	u8 i;
	///////////////////////////////////////coldtrap C///////////////////////////////////////
	//ADC_ConvertedValueLocal[0] =(float) ADC_ConvertedValue[0]/4096*3.3; // 读取转换的AD值	
	//Coldtrap_Tem = TemperaturePrint(ADC_ConvertedValue[0]); // 读取转换的AD值	
	TemperaturePrint(&ADC_ConvertedValueLocal[0],&ADC_ConvertedValue[0]); // 读取转换的AD值	
	Coldtrap_Tem=ADC_ConvertedValueLocal[0];
	/*
	if(Coldtrap_Tem>0)
	{
		data_disp[23] &= 0x00;
		Coldtrap_Tem_Temp=Coldtrap_Tem;
	}	
	else
	{
		data_disp[23] |= 0x01; 	
		//Coldtrap_Tem_Temp=abs(Coldtrap_Tem);	
		//Coldtrap_Tem_Temp=Coldtrap_Tem-4096;	
		//负数变正数
	}	
	*/
	/**/
	if(Coldtrap_Tem<999)
	{				
		data_disp[23] &= 0x00;
		Coldtrap_Tem_Temp=Coldtrap_Tem;
	}	
	else
	{	
		data_disp[23] |= 0x01; 	
		Coldtrap_Tem_Temp=Coldtrap_Tem-4096;				
	}	
 /**/	
	for(i=0;i<2;i++)
	{
		Coldtrap_data[i]=Coldtrap_Tem_Temp%10;
		Coldtrap_Tem_Temp=Coldtrap_Tem_Temp/10;
	}	
	Coldtrap_data[2]=Coldtrap_Tem_Temp;	
	data_disp[21] &= 0x00; 
	data_disp[21] |= (0x01 | data_led[Coldtrap_data[0]]); 
	data_disp[22] &= 0x00; 
	data_disp[22] |= (data_led[Coldtrap_data[1]]); 
	data_disp[23] &= 0x01; 
	data_disp[23] |= (data_led[Coldtrap_data[2]]); 			 
	if(!(Coldtrap_data[2]))
	{
		data_disp[23] &= 0x01; 
		data_disp[23] |= data_led[10]; 
		if(!(Coldtrap_data[1]))
		{
			data_disp[22] &= 0x01; 
			data_disp[22] |= data_led[10]; 		
		}
	}							
	disp_date(&data_disp[0],21,3);			
				
	///////////////////////////////////////////////sample c///////////////////////////////
	//ADC_ConvertedValueLocal[1] =(float) ADC_ConvertedValue[1]/4096*3.3; // 读取转换的AD值
	//Sample_Tem = TemperaturePrint(ADC_ConvertedValue[1]); // 读取转换的AD值	
	Sample_Tem=ADC_ConvertedValueLocal[1];
	if(Sample_Tem<999)
	{				
		data_disp[26] &= 0x00;
		Sample_Tem_Temp=Sample_Tem;
	}	
	else
	{	
		data_disp[26] |= 0x01; 	
		Sample_Tem_Temp=Sample_Tem-4096;				
	}				
	for(i=0;i<2;i++)
	{
		Sample_data[i]=Sample_Tem_Temp%10;
		Sample_Tem_Temp=Sample_Tem_Temp/10;
	}	
	Sample_data[2]=Sample_Tem_Temp;	
	data_disp[24] &= 0x00; 
	data_disp[24] |= (0x01 | data_led[Sample_data[0]]); 
	data_disp[25] &= 0x01; 
	data_disp[25] |= (0x01 | data_led[Sample_data[1]]);   //logo
	data_disp[26] &= 0x01; 
	data_disp[26] |= (data_led[Sample_data[2]]); 			 
	if(!(Sample_data[2]))
	{
		data_disp[26] &= 0x01; 
		data_disp[26] |= data_led[10]; 
		if(!(Sample_data[1]))
		{
			data_disp[25] &= 0x01; 
			data_disp[25] |= data_led[10]; 		
		}
	}							
	disp_date(&data_disp[0],24,3);		
}

