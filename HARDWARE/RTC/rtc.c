#include "sys.h"
#include "rtc.h" 
#include "delay.h"
#include "usart.h" 
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 

extern void disp_date(u8 data[32],u8 addr_start,u8 count);

extern u8 FlagRtcInt;
extern u16 PM25_Value;
const u8 *COMPILED_DATE;//获得编译日期
const u8 *COMPILED_TIME;//获得编译时间
const u8 Month_Tab[12][3]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"}; 
u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表	  
//平年的月份日期表
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

extern u8 displaydata_RTC[9];
extern u8 data_led[14];
extern u8 data_disp[32];
extern u8 USART4_RX_BUF[64];
extern u8 USART2_RX_BUF[64];
u8 Set_Number;
u8 data_temp1,data_temp2;
u8 temp_RTC;
extern u16  CO2_Data	;
extern u16  CO_Data	;

u8 rtc_data[8];
u16 rtc_data1;

PubTimer Timer;//时钟结构体 	  
PubTimer RTC_Set_Time,RTC_Set_Time1,RTC_Set_Time2,RTC_Set_Time3,RTC_Set_Time4;

//extern u8 Parameter_Time_Set_Flash;

//extern u8 parameter_Set_Flag;
//extern u8 Parameter_Set_Read;
//extern u8 Key_Press_Counter;
extern u8 KeyNum;


//实时时钟配置
//初始化RTC时钟,同时检测时钟是否工作正常 
//BKP->DR1用于保存是否第一次配置的设置
//返回0:正常
//其他:错误代码
void RTC_NVIC_Config(void)
{	
  NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTC全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//先占优先级1位,从优先级3位
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//先占优先级0位,从优先级4位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//使能该通道中断
	NVIC_Init(&NVIC_InitStructure);		//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

u8 RTC_Init(void)
{
	u8 temp=0;
//	u16 temp1=0;		   //
	RTC_NVIC_Config();
	BKP_ReadBackupRegister(BKP_DR1);
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050)		//从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
	{	 			
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
		PWR_BackupAccessCmd(ENABLE);	//使能RTC和后备寄存器访问 
		BKP_DeInit();	//将外设BKP的全部寄存器重设为缺省值 	
		RCC_LSEConfig(RCC_LSE_ON);	//设置外部低速晶振(LSE),使用外设低速晶振
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//检查指定的RCC标志位设置与否,等待低速晶振就绪
		{
			temp++;
			delay_ms(10);
		}
		if(temp>=250)return 1;//初始化时钟失败,晶振有问题	    
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
		RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟  
		RTC_WaitForSynchro();		//等待最近一次对RTC寄存器的写操作完成
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_ITConfig(RTC_IT_SEC, ENABLE);		//使能RTC秒中断
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_SetPrescaler(32767); //设置RTC预分频的值
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
    RTC_Set(2015,11,3,10,35,0);  //设置时间	    
		BKP_WriteBackupRegister(BKP_DR1, 0x5050);	//向指定的后备寄存器中写入用户程序数据
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
	}
	else//系统继续计时
	{
	 
		RTC_WaitForSynchro();	//等待最近一次对RTC寄存器的写操作完成
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	//使能RTC秒中断
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
	}	     
	RTC_Get();//更新时间
			// Check if the Power On Reset flag is set
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

   /* Allow access to BKP Domain */
   PWR_BackupAccessCmd(ENABLE);
	//BKP_WriteBackupRegister(BKP_DR1, 0x5050); 	
	RCC_ClearFlag();	//清除RCC的复位标志位
	return 0; //ok
}


//RTC中断服务函数
void RTC_IRQHandler(void)
{							    
	if(RTC->CRL&0x0001)//秒钟中断
	{							
		RTC_Get();//更新时间 	 
        FlagRtcInt = 1;
	}
	if(RTC->CRL&0x0002)//闹钟中断
	{
		RTC->CRL&=~(0x0002);//清闹钟中断
		//闹钟处理
	} 				  								 
	RTC->CRL&=0X0FFA;         //清除溢出，秒钟中断标志
	while(!(RTC->CRL&(1<<5)));//等待RTC寄存器操作完成		   							 	   	 
}
 

//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
u8 Is_Leap_Year(u16 year)
{			  
	if(year%4==0) //必须能被4整除
	{ 
		if(year%100==0) 
		{ 
			if(year%400==0)return 1;//如果以00结尾,还要能被400整除 	   
			else return 0;   
		}
        else
        {    
            return 1;   
        }
	}
    else 
    {
        return 0;
    }
}	
 			   
//设置时钟
//把输入的时钟转换为秒钟
//以1970年1月1日为基准
//1970~2099年为合法年份
//返回值:0,成功;其他:错误代码.
//月份数据表											 



u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec)
{
	u16 t;
	u32 seccount=0;
	if(syear<1970||syear>2099)return 1;	   
	for(t=1970;t<syear;t++)	//把所有年份的秒钟相加
		{
		if(Is_Leap_Year(t))seccount+=31622400;//闰年的秒钟数
		else seccount+=31536000;			  //平年的秒钟数
		}
	smon-=1;
	for(t=0;t<smon;t++)	   //把前面月份的秒钟数相加
		{
		seccount+=(u32)mon_table[t]*86400;//月份秒钟数相加
		if(Is_Leap_Year(syear)&&t==1)seccount+=86400;//闰年2月份增加一天的秒钟数	   
		}
	seccount+=(u32)(sday-1)*86400;//把前面日期的秒钟数相加 
	seccount+=(u32)hour*3600;//小时秒钟数
	seccount+=(u32)min*60;	 //分钟秒钟数
	seccount+=sec;//最后的秒钟加上去	
		
	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成

	RTC_SetCounter(seccount);	//设置RTC计数器的值
 

	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成 
		
	return 0;	    
	}

//得到当前的时间
//返回值:0,成功;其他:错误代码.
u8 RTC_Get(void)
{
	static u16 daycnt=0;
	u32 timecount=0; 
	u32 temp=0;
	u16 temp1=0;	  
	   
	timecount=RTC->CNTH;//得到计数器中的值(秒钟数)
	timecount<<=16;
	timecount+=RTC->CNTL;			 

	temp=timecount/86400;   //得到天数(秒钟数对应的)
	if(daycnt!=temp)//超过一天了
	{	  
		daycnt=temp;
		temp1=1970;	//从1970年开始
		while(temp>=365)
		{				 
			if(Is_Leap_Year(temp1))//是闰年
			{
				if(temp>=366)temp-=366;//闰年的秒钟数
				else {temp1++;break;}  
			}
			else temp-=365;	  //平年 
			temp1++;  
		}   
		Timer.w_year=temp1;//得到年份
		temp1=0;
		while(temp>=28)//超过了一个月
		{
			if(Is_Leap_Year(Timer.w_year)&&temp1==1)//当年是不是闰年/2月份
			{
				if(temp>=29)temp-=29;//闰年的秒钟数
				else break; 
			}
			else 
			{
				if(temp>=mon_table[temp1])temp-=mon_table[temp1];//平年
				else break;
			}
			temp1++;  
		}
		Timer.w_month=temp1+1;//得到月份
		Timer.w_date=temp+1;  //得到日期 
	}
	temp=timecount%86400;     //得到秒钟数   	   
	Timer.hour=temp/3600;     //小时
	Timer.min=(temp%3600)/60; //分钟	
	Timer.sec=(temp%3600)%60; //秒钟
	Timer.week=RTC_Get_Week(Timer.w_year,Timer.w_month,Timer.w_date);//获取星期   
	return 0;
}	 
//获得现在是星期几
//功能描述:输入公历日期得到星期(只允许1901-2099年)
//输入参数：公历年月日 
//返回值：星期号																						 
u8 RTC_Get_Week(u16 year,u8 month,u8 day)
	{	
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 
	// 如果为21世纪,年份数加100  
	if (yearH>19)yearL+=100;
	// 所过闰年数只算1900年之后的  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
	} 
//比较两个字符串指定长度的内容是否相等
//参数:s1,s2要比较的两个字符串;len,比较长度
//返回值:1,相等;0,不相等

u8 str_cmpx(u8*s1,u8*s2,u8 len)
{
	u8 i;
	for(i=0;i<len;i++)if((*s1++)!=*s2++)return 0;
	return 1;	   
}

//自动设置时间为编译器时间   
void Auto_Time_Set(void)
{
	u8 temp[3];
	u8 i;
	u8 mon,date;
	u16 year;
	u8 sec,min,hour;
	for(i=0;i<3;i++)temp[i]=COMPILED_DATE[i];   
	for(i=0;i<12;i++)if(str_cmpx((u8*)Month_Tab[i],temp,3))break;	
	mon=i+1;//得到月份
	if(COMPILED_DATE[4]==' ')date=COMPILED_DATE[5]-'0'; 
	else date=10*(COMPILED_DATE[4]-'0')+COMPILED_DATE[5]-'0';  
	year=1000*(COMPILED_DATE[7]-'0')+100*(COMPILED_DATE[8]-'0')+10*(COMPILED_DATE[9]-'0')+COMPILED_DATE[10]-'0';	   
	hour=10*(COMPILED_TIME[0]-'0')+COMPILED_TIME[1]-'0';  
	min=10*(COMPILED_TIME[3]-'0')+COMPILED_TIME[4]-'0';  
	sec=10*(COMPILED_TIME[6]-'0')+COMPILED_TIME[7]-'0';  
	RTC_Set(year,mon,date,hour,min,sec)	;
} 


//2--蓝色扇叶和左侧下面右侧第一个的8;3--扇叶圆点和右侧第二个8;
//4---冒号和第三个8;5---绿色扇叶和第四个8;6---黄色扇叶和5个8;7---网络标号和6个8;8---0的右侧和7个8;9----0的左侧和8个8;
//10----20和9个8;
void RTC_Display(void)
{

//    u8 j;
//    	RTC_Set_Time2=Timer;	
//	  	
//	   rtc_data[0] =RTC_Set_Time2.hour;
//	  rtc_data[1] =RTC_Set_Time2.min;
//	  rtc_data[2] =RTC_Set_Time2.sec;
//	  rtc_data1 =RTC_Set_Time2.w_year ;
//	  
//	  rtc_data[5] =RTC_Set_Time2.w_month;  
//	  rtc_data[6] =RTC_Set_Time2.w_date;  
//	  rtc_data[7] =RTC_Set_Time2.week;
//	   USART3_RX_BUF[4]

//	  printf(" Time: %0.2d:%0.2d:%0.2d:%0.2d:%0.2d:%0.2d:%0.2d PM2.5:%0.2d CO2:%0.2d:%0.2d\r",rtc_data[0],rtc_data[1],rtc_data[2],rtc_data1,rtc_data[5],rtc_data[6],rtc_data[7],PM25_Value,CO2_Data[0],CO2_Data[1]);
	
//			do
//      temp_RTC=RTC_Get();
//			while(temp_RTC);
//			//if(temp_RTC == 0)      //返回有效数值
//			//{		  
////				if(parameter_Set_Flag != 1)  //nomal display
//					RTC_Set_Time2=Timer;	
//				/**/
//				if(RTC_Set_Time2.w_year>=2000)
//				{
//						if((RTC_Set_Time2.w_year-2000)<100)
//							data_disp[10] = 0x01;
//						else
//							data_disp[10] = 0x00;	
//						data_temp1=(RTC_Set_Time2.w_year-2000)/10;
//						data_disp[10] |= data_led[data_temp1];  //0
//						data_temp2=(RTC_Set_Time2.w_year-2000)%10;
//						data_disp[9] |= data_led[data_temp2];   //1
//						
//				}	
//				if(RTC_Set_Time2.w_month<=12)
//				{
//						if(RTC_Set_Time2.w_month>9)
//								data_disp[9] &= 0xFE;  //2
//						else
//								data_disp[9] |= 0x01;
//						data_temp1=RTC_Set_Time2.w_month%10;
//						data_disp[8] = 0x01;      //2
//						data_disp[8] |= data_led[data_temp1];  //3   
//				}
//				else{;}		
//				if(RTC_Set_Time2.w_date<=31)
//				{
//					data_temp1=RTC_Set_Time2.w_date/10;
//					data_temp2=RTC_Set_Time2.w_date%10;
//					data_disp[7] &= 0x01;   //4
//					data_disp[7] |= data_led[data_temp1]; 
//					data_disp[6] &= 0x01;   //5 
//					data_disp[6] |= data_led[data_temp2];
//				}
//				else{;}
//				if(RTC_Set_Time2.hour<=24)
//				{
//					data_temp1=RTC_Set_Time2.hour/10;
//					data_temp2=RTC_Set_Time2.hour%10;
//					data_disp[5] &= 0x01; //6
//					data_disp[5] |= data_led[data_temp1]; 
//					data_disp[4] = 0x01;   //7
//					data_disp[4] |= data_led[data_temp2];
//				}
//				else{;}
//				if(RTC_Set_Time2.min<=60)
//				{
//					data_temp1=RTC_Set_Time2.min/10;
//					data_temp2=RTC_Set_Time2.min%10;
//					data_disp[3] &= 0x01; //8
//					data_disp[3] |= data_led[data_temp1]; 
//					data_disp[2] &= 0x01; //9
//					data_disp[2] |= data_led[data_temp2];
//				}
//				else{;}	
////				if((Parameter_Time_Set_Flash) && (parameter_Set_Flag==1))
//				{	
//					if((Set_Number==0) || (Set_Number==1))
//					{	
//						data_disp[10-Set_Number]&=0x01;
//						data_disp[10-Set_Number]|=data_led[10];
//					}
//					else if(Set_Number==2)
//					{
//						data_disp[9] &= 0xFE;
//						data_disp[8] &= 0xFE;
//					}	
//					else	
//					{	
//						data_disp[11-Set_Number]&=0x01;
//						data_disp[11-Set_Number]|=data_led[10];						
//					}	
//				}	
//						//Set_Number	
//				/**/
//
//
//
//				
//				/**
//				if(parameter_Set_Flag)
//				{
//					Timer=RTC_Set_Time2;	
//				}	
//				if(Timer.w_year>=2000)
//				{
//						if((Timer.w_year-2000)<100)
//							data_disp[10] = 0x01;
//						else
//							data_disp[10] = 0x00;	
//						data_temp1=(Timer.w_year-2000)/10;
//						data_disp[10] |= data_led[data_temp1];
//						data_temp2=(Timer.w_year-2000)%10;
//						data_disp[9] = data_led[data_temp2];
//				}	
//				if(Timer.w_month<=12)
//				{
//						if(Timer.w_month>9)
//								data_disp[9] &= 0x01;
//						else
//								data_disp[9] |= 0x01;
//						data_temp1=Timer.w_month%10;
//						data_disp[8] = 0x01;
//						data_disp[8] |= data_led[data_temp1];     
//				}
//				else{;}		
//				if(Timer.w_date<=31)
//				{
//					data_temp1=Timer.w_date/10;
//					data_temp2=Timer.w_date%10;
//					data_disp[7] &= 0x01; 
//					data_disp[7] |= data_led[data_temp1]; 
//					data_disp[6] &= 0x01; 
//					data_disp[6] |= data_led[data_temp2];
//				}
//				else{;}
//				if(Timer.hour<=24)
//				{
//					data_temp1=Timer.hour/10;
//					data_temp2=Timer.hour%10;
//					data_disp[5] &= 0x01; 
//					data_disp[5] |= data_led[data_temp1]; 
//					data_disp[4] = 0x01; 
//					data_disp[4] |= data_led[data_temp2];
//				}
//				else{;}
//				if(Timer.min<=60)
//				{
//					data_temp1=Timer.min/10;
//					data_temp2=Timer.min%10;
//					data_disp[3] &= 0x01; 
//					data_disp[3] |= data_led[data_temp1]; 
//					data_disp[2] &= 0x01; 
//					data_disp[2] |= data_led[data_temp2];
//				}
//				else{;}	
//				**/					
//				disp_date(&data_disp[0],2,9);		
			//}
			//else
			//{;}	
}			
/********************************************************************************
*** @brief  change number by different condition
*** @note   None
*** @param  None
*** @retval None
********************************************************************************/
void RTC_Numver_Change(u8 Set_Number,u8 Direction)
{
	u8 Num1,Num2;
	int8_t temp;
	u16 Time_Temp;
	Num1 = Set_Number/2;
	Num2 = Set_Number%2;
	if(Num1==0)    //year
	{
		if(Num2==0)  //1
		{	
			Time_Temp=RTC_Set_Time.w_year-((RTC_Set_Time.w_year-2000)/10)*10;
			RTC_Set_Time1.w_year=Time_Temp;
			temp =(RTC_Set_Time.w_year-2000)/10;
			if(Direction==1)
			{	
				temp++;
				if(temp >= 10)
					temp=0;
			}	
			else
			{	
				temp--;
				if(temp < 0)
					temp=9;
			}	
			RTC_Set_Time.w_year=Time_Temp+temp*10;
		}	
		else if(Num2==1)  //2
		{	
			
			temp =(RTC_Set_Time.w_year-2000)%10;
			Time_Temp=RTC_Set_Time.w_year-temp;
			RTC_Set_Time1.w_year=Time_Temp;
			if(Direction==1)
			{	
				temp++;
				if(temp >= 10)
					temp=0;
			}	
			else
			{	
				temp--;
				if(temp < 0)
					temp=9;
			}	
			RTC_Set_Time.w_year=Time_Temp+temp;
		}
	}	
	else if(Num1==1)  //month
	{
		if(Num2==0)  //1
		{				
			temp =RTC_Set_Time.w_month/10;	
			Time_Temp=RTC_Set_Time.w_month-temp*10;
			RTC_Set_Time1.w_month=Time_Temp;
			if(Direction==1)
			{	
				temp++;
				if(temp >= 1)
					temp=0;
			}	
			else
			{	
				temp--;
				if(temp < 0)
					temp=1;
			}	
			RTC_Set_Time.w_month=Time_Temp+temp*10;
		}	
		else if(Num2==1)  //2
		{	
			temp =RTC_Set_Time.w_month%10;	
			Time_Temp=RTC_Set_Time.w_month-temp;
			RTC_Set_Time1.w_month=Time_Temp;
			if((RTC_Set_Time.w_month/10)==0) //十位数是0
			{	
				if(Direction==1)
				{	
					temp++;
					if(temp >= 10)
						temp=0;
				}	
				else
				{	
					temp--;
					if(temp < 0)
						temp=9;
				}
				
			}
			else if((RTC_Set_Time.w_month/10)==1) //十位数是1
			{	
				if(Direction==1)
				{	
					temp++;
					if(temp >= 3)
						temp=0;
				}	
				else
				{	
					temp--;
					if(temp < 0)
						temp=2;
				}
			}	
			RTC_Set_Time.w_month=Time_Temp+temp;
		}			
	}	
	else if(Num1==2)  //date   ????????????????????为区分年份和月份
	{
		if(Num2==0)   //1
		{	
			temp =RTC_Set_Time.w_date/10;
			Time_Temp=RTC_Set_Time.w_date-temp*10;
			RTC_Set_Time1.w_date=Time_Temp;
			if(Direction==1)
			{	
				temp++;
				if(temp >= 4)
					temp=0;
			}	
			else
			{	
				temp--;
				if(temp < 0)
					temp=3;
			}	
			RTC_Set_Time.w_date=Time_Temp+temp*10;
		}	
		else if(Num2==1) //2
		{	
			temp =RTC_Set_Time.w_date%10;		
			Time_Temp=RTC_Set_Time.w_date-temp;
			RTC_Set_Time1.w_date=Time_Temp;
			if((RTC_Set_Time.w_date/10) < 3) //十位数是0、1、2
			{	
				if(Direction==1)
				{	
					temp++;
					if(temp >= 10)
						temp=0;
				}	
				else
				{	
					temp--;
					if(temp < 0)
						temp=9;
				}
			}
			else if((RTC_Set_Time.w_date/10) == 3) //十位数是1
			{	
				if(Direction==1)
				{	
					temp++;
					if(temp >= 2)
						temp=0;
				}	
				else
				{	
					temp--;
					if(temp < 0)
						temp=1;
				}
			}
			RTC_Set_Time.w_date=Time_Temp+temp;			
		}
	}	
	else if(Num1==3) //hour
	{
		if(Num2==0)  //1
		{	
			temp =RTC_Set_Time.hour/10;
			Time_Temp=RTC_Set_Time.hour-temp*10;
			RTC_Set_Time1.hour=Time_Temp;
			if(Direction==1)
			{	
				temp++;
				if(temp >= 3)
					temp=0;
			}	
			else
			{	
				temp--;
				if(temp < 0)
					temp=2;
			}
			RTC_Set_Time.hour=Time_Temp+temp*10;
		}	
		else if(Num2==1) //2
		{	
			temp =RTC_Set_Time.hour%10;
			Time_Temp=RTC_Set_Time.hour-temp;
			RTC_Set_Time1.hour=Time_Temp;
			if((RTC_Set_Time.hour/10) < 2) //十位数是0、1、2
			{	
				if(Direction==1)
				{	
					temp++;
					if(temp >= 10)
						temp=0;
				}	
				else
				{	
					temp--;
					if(temp < 0)
						temp=9;
				}
			}
			else if((RTC_Set_Time.hour/10) == 2) //十位数是1
			{	
				if(Direction==1)
				{	
					temp++;
					if(temp >= 5)
						temp=0;
				}	
				else
				{	
					temp--;
					if(temp < 0)
						temp=4;
				}
			}
			RTC_Set_Time.hour=Time_Temp+temp;
		}	
	}	
	else if(Num1==4) //min
	{
		if(Num2==0) //1
		{	
			temp =RTC_Set_Time.min/10;
			Time_Temp=RTC_Set_Time.min-temp*10;
			RTC_Set_Time1.min=Time_Temp;
			if(Direction==1)
			{	
				temp++;
				if(temp >= 6)
					temp=0;
			}	
			else
			{	
				temp--;
				if(temp < 0)
					temp=5;
			}
			RTC_Set_Time.min=Time_Temp+temp*10;
		}	
		else if(Num2==1) //2
		{	
			temp =RTC_Set_Time.min%10;					
			Time_Temp=RTC_Set_Time.min-temp;
			RTC_Set_Time1.min=Time_Temp;	
			if(Direction==1)
			{	
				temp++;
				if(temp >= 10)
					temp=0;
			}	
			else
			{	
				temp--;
				if(temp < 0)
					temp=9;
			}	
			RTC_Set_Time.min=Time_Temp+temp;	
		}	
	}
}
/********************************************************************************
*** @brief  Sets RTC time by key
*** @note   None
*** @param  None
*** @retval None
********************************************************************************/
//void RTC_Time_Set(void)
//{
//	static u8 RTC_Time_Set_Step;
////	if(Parameter_Set_Read == 1)
////	{	
//		
//		RTC_Get();
//		RTC_Set_Time=Timer;	
//		RTC_Set_Time1=Timer;		
//		RTC_Set_Time1.w_year -= ((RTC_Set_Time.w_year-2000)/10)*10;
//		
//		//RTC_Set_Time.w_year=Timer.w_year;
//		//RTC_Set_Time.w_month=Timer.w_month;//得到月份
//		//RTC_Set_Time.w_date=Timer.w_date;  //得到日期 
//		//RTC_Set_Time.hour=Timer.hour;     //小时
//		//RTC_Set_Time.min=Timer.min; //分钟	
////		Parameter_Set_Read=0;	
////	}		
//	//if(Parameter_Time_Set_Flash)
//	//{
//		//RTC_Set_Time2=RTC_Set_Time1;
//	//}
//	//else
//	//{
//		RTC_Set_Time2=RTC_Set_Time;
//	//}	
//	switch(RTC_Time_Set_Step)
//	{
//		case 0:
//			if(Key_Press_Counter < 10) //less 10S
//				RTC_Time_Set_Step=1;
//			else    //excess 10S
//			{	
//				RTC_Time_Set_Step=5;				
//			}	
//			break;
//		case 1:    //定位在年份的第三个数字上 闪动 等待上下键选择数字 设置定时10S 若期限内没有动作 则推出时间设定
//			//if(KeyNum==1)  //compressor  A  press
//			if(KeyNum==8)  //compressor  A  press
//			{	
//				//Timer=RTC_Set_Time;		
//				//RTC_Set(Timer.w_year,Timer.w_month,Timer.w_date,Timer.hour,Timer.min,Timer.sec);  //设置时间	
//				if(Set_Number>9)  //complete RTC time set
//					RTC_Time_Set_Step=5;
//				else
//				{									
//					RTC_Time_Set_Step=2;
//				}					
//			}	
//			//else if(KeyNum==2)  //compressor  B  press
//			else if(KeyNum==16)  //compressor  B  press
//			{
//				 RTC_Time_Set_Step=3;
//			}	
//			//else if(KeyNum==4)  //compressor  D  press
//			else if(KeyNum==32)  //compressor  D  press
//			{
//				 RTC_Time_Set_Step=4;
//			}	
//			else
//				RTC_Time_Set_Step=0;
//			break;
//		case 2:    //wait A up
//			if(KeyNum==0) //A up
//			{
//				Set_Number++;
//				Key_Press_Counter=0;//reset 10S
//				RTC_Time_Set_Step=0;
//			}	
//			break;
//		case 3:    //wait B up
//			if(KeyNum==0) //B up
//			{
//				RTC_Numver_Change(Set_Number,0x01);
//				//RTC_Set_Time1=RTC_Set_Time;
//				//RTC_Set_Time2=RTC_Set_Time;
//				Key_Press_Counter=0;//reset 10S
//				RTC_Time_Set_Step=0;
//			}	
//			break;
//		case 4:    //wait D up
//			if(KeyNum==0) //D up
//			{
//				RTC_Numver_Change(Set_Number,0x00);
//				//RTC_Set_Time1=RTC_Set_Time;
//				//RTC_Set_Time2=RTC_Set_Time;
//				Key_Press_Counter=0;//reset 10S
//				RTC_Time_Set_Step=0;
//			}	
//			break;
//		case 5:    // the end of RTC time set
//			Timer=RTC_Set_Time;		
//			/////////////////////////////////////////////////
//			//BKP_WriteBackupRegister(BKP_DR1, 0x0000);	//向指定的后备寄存器中写入用户程序数据
//			RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
//			PWR_BackupAccessCmd(ENABLE);	//使能RTC和后备寄存器访问 						
//			//RTC_Set(2014,1,1,12,0,0);  //设置时间	  
//			RTC_Set(Timer.w_year,Timer.w_month,Timer.w_date,Timer.hour,Timer.min,Timer.sec);  //设置时间	
//			BKP_WriteBackupRegister(BKP_DR1, 0x5050);     
//			//////////////////////////////////////////////////
//			Key_Press_Counter=0;   //reset RTC time set counter
////			parameter_Set_Flag=0;  //cancel RTC time set
//			RTC_Time_Set_Step=0;			
//			break;		
//		default:    //等待确认键确认上一个时间设定
//			break;
//	}	
//	
//}			
