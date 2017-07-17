#include "basic.h"
#include "sys.h"



extern void disp_date(u8 data[32],u8 addr_start,u8 count);
extern void Parameter_Flash_write(void);   
extern u8 data_disp[32];
u8 Parameter_Set_Complete_Flag=0;

extern int16_t Sample_Value_Set; 
extern u8 KeyNum;
extern u8 Key_Press_Counter;   
extern u8 Model_Flash;
extern u8 Set_Number; 
u8 Model_Value_Set;
u8 Model_Value_Set_Temp; 
u8 Sample_Flash;
u8 Coldtrap_Flash;
u8 Model_Flash;
u8 Vac_Flash;
u8 Sample_Flag;
u8 Coldtrap_Flag;
u8 Model_Flag;
u8 Vac_Flag;   
u8 Model;
u8 Filament_Static;
extern u8 data_led[14];
extern u8 Parameter_Time_Set_Flash;
 
void fan_run(u8 fan_static)
{
	static u8 i=0;
	//KG1_Control(fan_static); 	
	if(fan_static)
	{	
		data_disp[3] |= 0x01;		
		if((i%3)==1)
		{
			data_disp[2] &= 0xFE;			
			data_disp[5] &= 0xFE;
			data_disp[6] |= 0x01;				
		}	
		else if ((i%3)==2)
		{
			data_disp[2] &= 0xFE;			
			data_disp[5] |= 0x01;
			data_disp[6] &= 0xFE;	
		}
		else if ((i%3)==0)
		{
			data_disp[2] |= 0x01;			
			data_disp[5] &= 0xFE;
			data_disp[6] &= 0xFE;	
		}
		i++;
		if(i>3) i=1;  		
	}
	else
	{
		i=1;
		data_disp[2] |= 0x01;	
		data_disp[3] |= 0x01;	
		data_disp[5] |= 0x01;
		data_disp[6] |= 0x01;		
	}	
	disp_date(&data_disp[0],3,5);		
}

