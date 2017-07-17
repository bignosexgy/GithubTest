#include "vac.h"
//#include "basic.h"
#include "gpio.h"
#include "basic.h"
#include "stm32f10x.h"
#include"stdio.h"


#define Vacuometer_Open 1;	
#define Vacuumpump_Open 1;

extern void Parameter_Flash_write(void);
extern void disp_date(u8 data[32],u8 addr_start,u8 count);

extern u8 data_led[14];
extern u8 data_disp[32];
extern u32 Fre_Test;   // test frequency


extern u8 Parameter_Time_Set_Flash;
extern u32 Vac_Value_Set_Temp;
//extern u8 Parameter_Set_Read;
//extern u8 parameter_Set_Flag;
extern u8 Key_Press_Counter;
extern u8 KeyNum;
extern u8 Set_Number;

extern u8 Vac_Flag;
extern u8 Vac_Flash;
extern u8 Coldtrap_Flash;
extern u8 Coldtrap_Flag;
extern u8 Model_Flash;
extern u8 Model_Flag;	
extern u8	Sample_Flash;
extern u8 Sample_Flag;	






extern u8 Filament_Static;
extern int16_t Coldtrap_Tem;
extern int16_t Coldtrap_Value_Set;




u16 Vac_Temp;
u16 Fre_temp;
u32 Vac;
u8  Vac_Ovl=0;
u8  Vac_Data_temp[5];
u32 Vac_Disp_Temp;
u32 Vac_Value_Set;
u32 Vac_Value_Set_Temp;

//extern u16 fretovac[230][2];
extern u8  Vac_Ovl;
u16 fre_t0_vac[231][2]={
//  0              1             2             3            4              5             6            7             8              9	
	{1,255},      {2,417},      {3,660},      {4,856},      {5,1064},     {6,1205},     {7,1415},     {8,1634},     {9,1831},     {10,2039},
	{11,2127},    {12,2215},    {13,2369},    {14,2457},    {15,2580},    {16,2658},    {17,2769},    {18,2812},    {19,2932},    {20,3077},
	{21,3148},    {22,3234},    {23,3363},    {24,3432},    {25,3583},    {26,3615},    {27,3770},    {28,3814},    {29,3890},    {30,3945},
	{31,4035},    {32,4135},    {33,4178},    {34,4224},    {35,4302},    {36,4354},    {37,4430},    {38,4500},    {39,4610},    {40,4653},
	{41,4718},    {42,4787},    {43,4843},    {44,4923},    {45,4965},    {46,5021},    {47,5065},    {48,5126},    {49,5200},    {50,5211},
	{51,5308},    {52,5352},    {53,5418},    {54,5452},    {55,5541},    {56,5547},    {57,5628},    {58,5673},    {59,5716},    {60,5771},
	{61,5794},    {62,5828},    {63,5840},    {64,5904},    {65,5971},    {66,6017},    {67,6042},    {68,6068},    {69,6092},    {70,6115},
	{71,6182},    {72,6213},    {73,6246},    {74,6266},    {75,6300},    {76,6400},    {77,6432},    {78,6457},    {79,6490},    {80,6509},
	{81,6536},    {82,6616},    {83,6634},    {84,6654},    {85,6706},    {86,6732},    {87,6761},    {88,6789},    {89,6817},    {90,6860},
	{91,6883},    {92,6904},    {93,6948},    {94,6981},    {95,6994},    {96,7037},    {97,7058},    {98,7067},    {99,7085},    {100,7147},
	{110,7352},   {120,7644},   {130,7930},   {140,8162},   {150,8316},   {160,8571},   {170,8754},   {180,9152},   {190,9154},   {200,9401},
	{210,9558},   {220,9601},   {230,9742},   {240,10030},  {250,10129},  {260,10196},  {270,10284},  {280,10362},  {290,10400},  {300,10560},
	{310,10650},  {320,10720},  {330,10800},  {340,10890},  {350,10900},  {360,11030},  {370,11070},  {380,11120},  {390,11220},  {400,11290},
	{410,11370},  {420,11410},  {430,11480},  {440,11580},  {450,11620},  {460,11670},  {470,11730},  {480,11750},  {490,11820},  {500,11870},
	{510,11990},  {520,12070},  {530,12090},  {540,12120},  {550,12150},  {560,12184},  {570,12251},  {580,12294},  {590,12328},  {600,12383},
	{610,12439},  {620,12481},  {630,12590},  {640,12661},  {650,12667},  {660,12720},  {670,12768},  {680,12837},  {690,12918},  {700,12940},
	{710,12973},  {720,13030},  {730,13072},  {740,13104},  {750,13129},  {760,13141},  {770,13183},  {780,13216},  {790,13240},  {800,13264},
	{810,13294},  {820,13331},  {830,13354},  {840,13403},  {850,13469},  {860,13562},  {870,13525},  {880,13558},  {890,13611},  {900,13650},
	{910,13664},  {920,13726},  {930,13774},  {940,13800},  {950,13847},  {960,13926},  {970,13931},  {980,13977},  {990,14000},  {1000,14032},
	{1100,14072}, {1200,14175}, {1300,14250}, {1400,14325}, {1500,14441}, {1600,14532}, {1700,14615}, {1800,14731}, {1900,14897}, {2000,14996},
	{2100,15091}, {2200,15151}, {2300,15229}, {2400,15324}, {2500,15424}, {2600,15512}, {2700,15570}, {2800,15600}, {2900,15670}, {3000,15815},
	{3600,15856}, {3800,15938}, {4000,16015}, {4800,16115}, {5500,16188}, {6000,16230}, {7000,16256}, {7500,16328}, {8500,16457}, {9000,16569},
	{10000,16811},{20000,16928},{30000,16959},{40000,17029},{50000,17071},{60000,17174},{70000,17297},{80000,17344},{90000,17349},{100000,17380},
	{110000,17438}			
	//{10000,16811},{20000,16928},{30000,16959},{40000,17029},{50000,17071},{60000,17174},{7000,17297}, {8000,17344}, {9000,17349}, {10000,17380},		
	//{11000,17438}	
	};  
	
	
u32 fre2vac(u32 fre)
{
	u8 mid=0, left=0,right=230;
	u8 i;
	u16 temp;
	Vac_Ovl=0;
	if((fre_t0_vac[230][1]>=fre) && (fre_t0_vac[0][1]<=fre))
	{	
		mid=(left+right)/2;		
		if(fre_t0_vac[mid][1]==fre)
			return fre_t0_vac[mid][0];
		else if(fre_t0_vac[mid][1]<fre)			
			left=mid;
		else
			right=mid;	
		//result：return	temp;	
		for(i=left;i<=right;i++)
		{
			if(i>225)
				Vac_Ovl=1;				
			if(fre_t0_vac[i][1]==fre)	
			{
				//return fre_t0_vac[i][0];	
				temp=fre_t0_vac[i][0];	
			  goto result;
			}
			else if((fre_t0_vac[i][1]<fre) && (fre_t0_vac[i+1][1]>fre))
			{	
				//return ((fre-fre_t0_vac[i][1])*(fre_t0_vac[i+1][0]-fre_t0_vac[i][0])/(fre_t0_vac[i+1][1]-fre_t0_vac[i][1])+fre_t0_vac[i][0]);
				temp=((fre-fre_t0_vac[i][1])*(fre_t0_vac[i+1][0]-fre_t0_vac[i][0])/(fre_t0_vac[i+1][1]-fre_t0_vac[i][1])+fre_t0_vac[i][0]);
				goto result;
			}	
		}
	}	
	else if((fre_t0_vac[230][1]<fre) || (fre_t0_vac[0][1]>fre))
	{
		Vac_Ovl=1;			
	  //return 0xffff;		
		temp=0xffff;
		goto result;
	}		 
	result:return temp;
}




void Vac_Process(void)
{
	u8 i;
	//u32 temp;	   	
	Vac_Temp=fre2vac(Fre_Test*4/10);   
	Vac=(u32)(Vac_Temp+Vac_Ovl*0x10000);
	
	if(Filament_Static) //灯丝断 显示“Er”
	{
		data_disp[16] &= 0x00; 		
		data_disp[16] |= (0x001 | data_led[13]);  
		data_disp[17] &= 0x01; 			
		data_disp[17] |= data_led[12]; 
		data_disp[18] &= 0x01; 
		data_disp[18] |= data_led[10]; 
		data_disp[19] &= 0x01; 
		data_disp[19] |= data_led[10]; 
		data_disp[20] &= 0x00; 
		data_disp[20] |= data_led[10]; 
	}
	else
	{		 			
		if(Vac==0x1ffff)   //overload
		{	 			
			data_disp[16] &= 0x00; 
			data_disp[16] |= (0x01 | data_led[0]);			
			data_disp[17] &= 0x01; 			
			data_disp[17] |= data_led[0]; 
			data_disp[18] &= 0x01; 
			data_disp[18] |= data_led[0]; 
			data_disp[19] &= 0x01; 
			data_disp[19] |= data_led[0]; 
			data_disp[20] &= 0x00; 				
			data_disp[20] |= (0x01 | data_led[1]);
			
		}
		else  //正常显示
		{		
			Vac_Disp_Temp=Vac;
			for(i=0;i<5;i++)
			{
				Vac_Data_temp[i]=Vac_Disp_Temp%10;
				Vac_Disp_Temp=Vac_Disp_Temp/10;
			}				
			if(Vac_Disp_Temp)
				data_disp[20] |= 0x01; 			
			else
				data_disp[20] &= ~(0x01); 
			data_disp[16] &= 0x01; 
			data_disp[16] |= (0x01 | data_led[Vac_Data_temp[0]]); 
			data_disp[17] &= 0x01; 			
			data_disp[17] |= data_led[Vac_Data_temp[1]]; 
			data_disp[18] &= 0x01; 
			data_disp[18] |= data_led[Vac_Data_temp[2]]; 
			data_disp[19] &= 0x01; 
			data_disp[19] |= data_led[Vac_Data_temp[3]]; 
			data_disp[20] &= 0x01; 
			data_disp[20] |= data_led[Vac_Data_temp[4]]; 		
	}		
	}
		
					
			
				
	if((Vac_Flash==1) && (Parameter_Time_Set_Flash)) //biao shi shan dong
		data_disp[16] &= 0xFE;  
	if((Parameter_Time_Set_Flash) && (Vac_Flag==1))
	{	
		if(Set_Number==0)
		{	
			data_disp[20] &= 0xFE;			
		}
		else 
		{
			data_disp[21-Set_Number] &= 0x01;
			data_disp[21-Set_Number] |= data_led[10];	
		}		
	}
	disp_date(&data_disp[0],16,5);
}		
