//#include "vac.h"
#include "ht1622.h"

extern void disp_date(u8 data[32],u8 addr_start,u8 count);
extern u8 data_led[14];
extern u8 data_disp[32];
extern u32 Runtime;

u32 Runtime_Temp;
u8 Runtime_data[5];


void Runtime_Process(void)
{
	u8 i;
	u32 timecount=0; 
	u32 temp=0;
	static u32 Runtime_temp1;	
	static u8 Runtime_RTC_Flag;	
	   
	timecount=RTC->CNTH;//得到计数器中的值(秒钟数)
	timecount<<=16;
	timecount+=RTC->CNTL;	


	Runtime_Temp=Runtime;
	for(i=0;i<4;i++)
	{
		Runtime_data[i]=Runtime_Temp%10;
		Runtime_Temp=Runtime_Temp/10;
	}					
	Runtime_data[4]=Runtime_Temp;	
	data_disp[11] &= 0x00; 
	data_disp[11] |= (0x01 | data_led[Runtime_data[0]]);   //display runtimer min
	data_disp[12] &= 0x01; 
	data_disp[12] |= (data_led[Runtime_data[1]]); 
	data_disp[13] &= 0x01; 
	data_disp[13] |= (data_led[Runtime_data[2]]); 
	data_disp[14] &= 0x01; 
	data_disp[14] |= (data_led[Runtime_data[3]]); 
	data_disp[15] &= 0x01; 
	data_disp[15] |= (data_led[Runtime_data[4]]); 			
	if(!(Runtime_data[4]))  //如果前面的数都为零则不显示0
	{	
		data_disp[15] &= 0x01; 
		data_disp[15] |= data_led[10]; 
		if(!(Runtime_data[3]))
		{
			data_disp[14] &= 0x01; 
			data_disp[14] |= data_led[10]; 
			if(!(Runtime_data[2]))
			{
				data_disp[13] &= 0x01; 
				data_disp[13] |= data_led[10]; 
				if(!(Runtime_data[1]))
				{
					data_disp[12] &= 0x01; 
					data_disp[12] |= data_led[10]; 		
				}
			}							
		}		
	}				
	disp_date(&data_disp[0],11,5);	
}			
