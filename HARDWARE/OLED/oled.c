#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"
#include "rtc.h"
#include "stdio.h" 
#include "sht21.h"
#include "ec808.h"
#include "usart.h"
#include "gpio.h"
//////////////////////////////////////////////////////////////////////////////////	 
//imodule	
//////////////////////////////////////////////////////////////////////////////////	

//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 
//int8_t *OLED_menu[] = { "TE","HUM"," CO","CO2","HC","PM",
//                      " POWER"," EIFI " };
//int8_t *OLED_menu2[] = { "MP "," "," "," ","HO","2.5",
//                       " POWER"," EIFI " };
char *OLED_menu[] = { "TEMP","HUM","CO","CO2","HCHO","PM2.5",
                      " POWER"," EIFI " };
char *OLED_menu2[] = { "MP "," "," "," ","HO","2.5",
                       " POWER"," EIFI " };
char OLED_menu1[8] = {0};
char OLED_menu4[8] = {0};
u8 Sound_Vs=1;	   //0����
volatile int8_t OLED_curPos = 0;
extern		u8  WIFI_Flag;//0����
extern		u16 BAT_Value ;
extern	float VOC_Data ;
extern u8  test_imid;
extern u16 pm25_test_mun;
extern PubTimer Timer;//ʱ�ӽṹ�� 	
extern PubTimer RTC_Set_Time3;
extern u8 WIFI_flag_view;
extern u8  WIFI_internet_Flag;//0����
extern u8 internet__send_flag;
extern u8 internet__send_flag_0;
extern u8 WIFI_internet_send_Flag;

extern u8 LOGIN_CMD_Flag_num;
extern u8 Second_LOGIN_CMD_Flag_num;
 
extern int16_t Temperature;
//extern	power_flag	;
#define OLED_UP 	1 
#define OLED_DOWN 	2 			   
u16 OLED_GRAM[132][8];
#define OLED_MAX_LEN 9
u8  sound_oled=0;
u8  sound_oled_num=0;
u8  wifi_oled=10;
u8  power_oled=3;
void OLED_up(void)
{
	OLED_curPos++;
	if(OLED_curPos == OLED_MAX_LEN)
		OLED_curPos = 0;
	if(OLED_curPos==0)	  		
		Sht21Work(0xf3);
	if(OLED_curPos==1)
		Sht21Work(0xf5);
	if(OLED_curPos==4)
		EC808_Process();
	if(OLED_curPos==2)
		EC805_Process();

	if(OLED_curPos==5)
	{
	    test_imid =1;
		pm25_test_mun=6000;
	}
	else
	 	pm25_test_mun=550;
    if(OLED_curPos==3)
		USART3_TX_State(); //co2

//    if(OLED_curPos==7)
//		internet__send_flag=0; 


	if((!Sound_Vs)&&(OLED_curPos!=8)&&(sound_oled==0))
	   USART5_TX_State();
   	if((!Sound_Vs)&&(sound_oled==0)&&(OLED_curPos==8))
       {
	       USART5_TX_State1();
		   sound_oled=1;
		   sound_oled_num=0;		
	   }
	   	   else
		sound_oled=0;
	  	LCD2_update();

}

void OLED_down(void)
{
	if(OLED_curPos == 0)
		OLED_curPos = OLED_MAX_LEN - 1;
	else
		OLED_curPos--;

	if(OLED_curPos==0)	  		
		Sht21Work(0xf3);
	if(OLED_curPos==1)
		Sht21Work(0xf5);
	if(OLED_curPos==4)
		EC808_Process();
	if(OLED_curPos==2)
		EC805_Process();

	if(OLED_curPos==5)
	{
	    test_imid =1;
		pm25_test_mun=6000;
	}
	else
	 	pm25_test_mun=550;
    if(OLED_curPos==3)
		USART3_TX_State(); //co2
//	if(OLED_curPos==7)
//		internet__send_flag=0; 
	
	if((!Sound_Vs)&&(OLED_curPos!=8)&&(sound_oled==0))
	   USART5_TX_State();
	if((!Sound_Vs)&&(sound_oled==0)&&(OLED_curPos==8))
       {
	       USART5_TX_State1();
		   sound_oled=1;
		    sound_oled_num=0;		
	   }
	   else
		sound_oled=0;
	  LCD2_update();
	 
}


void LCD2_update(void)
{
    u8 i;//cc;
	i=OLED_curPos;

	  if((i==0)||(i==1)	)
	 {	    
		internet__send_flag_0=1;
        OLED_Clear();
		if(i==0)
		 sprintf(OLED_menu1,"%d",get_params2(i)); 
		 else
	    sprintf(OLED_menu1,"%d",get_params(i)); 
		OLED_Display5(OLED_menu[i],OLED_menu1);

	
	
		OLED_Refresh_Gram();
		OLED_curPos=i;
	   
		
	  }
	 else if((i==2)||(i==3)||(i==5))
	  {	    
		internet__send_flag_0=1;
        OLED_Clear();
	    sprintf(OLED_menu1,"%d",get_params(i)); 
		OLED_Display3(OLED_menu[i],OLED_menu1);

		OLED_ShowChar_11(25,40,11,1); 
	
		OLED_Refresh_Gram();
		OLED_curPos=i;
	   
		
	  }
	 else if(i==4)
	 {	  
		 internet__send_flag_0=1;
		OLED_Clear();
	    	sprintf(OLED_menu1,"%.2f",VOC_Data)	;				   
		
		OLED_Display3(OLED_menu[i],OLED_menu1);
		OLED_ShowChar_11(25,40,11,1); 
		OLED_Refresh_Gram();
		OLED_curPos=i;
	   
	}
	 
	else if  (i==7)
	 {	  

         OLED_Clear();
		if (WIFI_Flag==1)
		 	 OLED_ShowChar_1(0,0,1,1);
		  else if(WIFI_flag_view==1)
			  {
			
			    OLED_ShowChar_1(0,0,wifi_oled,1);
				wifi_oled++;
				if(wifi_oled>=14)
				wifi_oled=10;
			   }

			  else	if(WIFI_internet_Flag==0)
					{
			       	    OLED_ShowChar_1(0,0,9,1);
						if(internet__send_flag_0==1)
					    
						{
						LOGIN_CMD_Flag_num=120;
		                Second_LOGIN_CMD_Flag_num=60;
						//internet__send_flag=0;
						internet__send_flag_0=0;
					    //WIFI_internet_send_Flag=0;
						}
					}
					else
					 {
				     OLED_ShowChar_1(0,0,2,1);

					 }

		OLED_Refresh_Gram();
		OLED_curPos=i;
		
	} 	  	
	else if  (i==6)
	 {	  
		 internet__send_flag_0=1;
			OLED_Clear();
	
			if((power_flag==1)&&(BAT_Value<3800))
			  {
				
				    OLED_ShowChar_1(0,0,power_oled,1);
					power_oled++;
					if(power_oled>=8)
					power_oled=3;
			}
	
			else
			  {
				if(BAT_Value<=3350)	 ////3.6v
			    	OLED_ShowChar_1(0,0,3,1);
				if((BAT_Value<3550)&&(BAT_Value>3350))	 ////3.6v		3350
				   OLED_ShowChar_1(0,0,4,1);
				if((BAT_Value<3700)&&(BAT_Value>=3550))	 ////3.6v		3350
				   OLED_ShowChar_1(0,0,5,1);
				if((BAT_Value>=3700)&&(BAT_Value<3800)	) ////4.1v	  4.2  3722  3900
			        OLED_ShowChar_1(0,0,6,1); 		
				if(BAT_Value>=3800)	  ///4.2
				    OLED_ShowChar_1(0,0,7,1);
				}	
 

		      OLED_Refresh_Gram();
		     OLED_curPos=i;
	  }

	  else if(i==8)
	   {
	    internet__send_flag_0=1;
	   	 OLED_Clear();
	   
	   	if(Sound_Vs ==1)
	   	   OLED_ShowChar_1(0,0,15,1); 
	    else
	   	   OLED_ShowChar_1(0,0,14,1); 
		 OLED_Refresh_Gram();
		 OLED_curPos=i;

		}
	   else
	  {	    

        OLED_Clear();
	    sprintf(OLED_menu1,"%3d",get_params(i)); 
		OLED_Display1(OLED_menu[i],OLED_menu1);
		OLED_Refresh_Gram();
		OLED_curPos=i;
	  }

}


void DisplayProcess(u8 i)
{
	
	switch(i)
	{

		case OLED_UP:

			OLED_up();
			break;
		case OLED_DOWN:
			OLED_down();
			break;
		default:
			break;
	}
}	 

//�����Դ浽LCD		 
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<131;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}
#if OLED_MODE==1
//��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	DATAOUT(dat);	    
	if(cmd)
	  OLED_RS_Set();
	else 
	  OLED_RS_Clr();		   
	OLED_CS_Clr();
	OLED_WR_Clr();	 
	OLED_WR_Set();
	OLED_CS_Set();	  
	OLED_RS_Set();	 
} 	    	    
#else
//��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_RS_Set();
	else 
	  OLED_RS_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_RS_Set();   	  
} 
#endif
	  	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
   
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<130;n++)OLED_GRAM[n][i]=0X00;  
	//OLED_Refresh_Gram();//������ʾ
}
//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>131||y>63)return;//������Χ��.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,���;1,���	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//������ʾ
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1,t2;
	u8 y0=y;
//		if(size==12)t2=size;  //����1206����
//		//else if(size==16) t2=size;		 //����1608���� 
//			  else if (size==16)t2=24;		 //����1608���� 
//			      // else if(size==40) t2=100;		 //����1608����
//				    else if(size==40) t2=100; 
//	chr=1;							
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ
//	if(chr==18)	
//	  size=12;
	  	if(size==12)t2=size;  //����1206����
		//else if(size==16) t2=size;		 //����1608���� 
			  else if (size==16)t2=24;		 //����1608���� 
			      // else if(size==40) t2=100;		 //����1608����
				    else if(size==40) t2=100; 			   
    for(t=0;t<t2;t++)
    {   
		if(size==12)temp=oled_asc2_1206[chr][t];  //����1206����
	//	else if(size==16) temp=oled_asc2_1608[chr][t];		 //����1608����
		  // else if (size==20)temp=oled_asc2_2016[chr][t]; 
			  else if (size==16)temp=oled_asc2_1612[chr][t];		 //����1608���� 
			       //else if(size==40) temp=oled_asc2_4036[chr][t];		 //����1608���� 
				      else if(size==40) temp=oled_asc2_4020[chr][t];		 //����1608����                            
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}

void OLED_ShowChar_12(u8 x,u8 y,u8 z, u8 mode)
{      			    
	u16 temp,t,t1,t2 ,size;
	u8 y0=y;
	if((z==0)||(z==1)||(z==2)||(z==3))
	{
	    t2=96;
		size=16	;	 
	}
	else if((z==4)||(z==5))
	  	{
	    t2=100;
		size=36	;	 
	}
    
    for(t=0;t<t2;t++)
    {   
	   	if(z==0)temp=oled_asc2_CO[t];
		else if(z==1)temp=oled_asc2_CO2[t]; 
		else if(z==2)temp=oled_asc2_HCHO[t];  
	    else if(z==3)temp=oled_asc2_PM[t]; 
		else if(z==4)temp=oled_asc2_wendu[t]; 
	    else if(z==5)temp=oled_asc2_baifenhao[t]; 
	                   
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}

void OLED_ShowChar_11(u8 x,u8 y,u8 z, u8 mode)
{      			    
	u16 temp,t,t1,t2 ,size;
	u8 y0=y;
	if(z==11)
	{
	    t2=76;
		size=1	;
		
	 
	}
    
    for(t=0;t<t2;t++)
    {   
	   
	     if(z==11)temp=oled_asc2_11[t]; 
	                   
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
void OLED_ShowChar_1(u8 x,u8 y,u8 z, u8 mode)
{      			    
	u16 temp,t,t1,t2 ,size;
	u8 y0=y;
	if(z==0)
	{
	    t2=5;
		size=40	;
		}
	  else if ((z>=1)&&(z<=15))
	  		{
	    t2=1024;
		size=64	;
		}
    
    for(t=0;t<t2;t++)
    {   
	   
	     if(z==0)temp=oled_asc2_402[t]; 
	     if(z==1)temp=oled_asc2_wifi4[t];
	     if(z==2)temp=oled_asc2_wifi0[t] ;
		 if(z==9)temp=oled_asc2_wifi5[t] ; 
	 	 if(z==3)temp=oled_asc2_pow0[t] ;
		 if(z==4)temp=oled_asc2_pow1[t] ;
		 if(z==5)temp=oled_asc2_pow2[t] ;
		 if(z==6)temp=oled_asc2_pow3[t] ;
		 if(z==7)temp=oled_asc2_pow4[t] ;
		// if(z==8)temp=oled_asc2_wifi3[t] ;
		 

         if(z==10)temp=oled_asc2_wifi1[t] ;
         if(z==11)temp=oled_asc2_wifi2[t] ;
	     if(z==12)temp=oled_asc2_wifi3[t] ;
	     if(z==13)temp=oled_asc2_wifi4[t] ; 
		 if(z==14)temp=oled_asc2_BOFANG[t] ;
	     if(z==15)temp=oled_asc2_ZANTING[t] ;               
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}


//m^n����
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
//��ʾ�ַ���
//x,y:�������  
//*p:�ַ�����ʼ��ַ
//��16����
void OLED_ShowString_16(u8 x,u8 y, u8 *p)
{
// #define MAX_CHAR_POSX 122
 #define MAX_CHAR_POSX 126
#define MAX_CHAR_POSY 58          
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,16,1);	 
        x+=8;
        p++;
    }  
}
void OLED_ShowString_20(u8 x,u8 y, u8 *p)
{
// #define MAX_CHAR_POSX 122
 #define MAX_CHAR_POSX 126
#define MAX_CHAR_POSY 58          
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,20,1);	 
        x+=13;
        p++;
    }  
}
void OLED_ShowString_24_1(u8 x,u8 y, char *p)
{
// #define MAX_CHAR_POSX 122
 #define MAX_CHAR_POSX 126
#define MAX_CHAR_POSY 58          
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
	  if(*p==50	)
		 OLED_ShowChar(x,y+5,*p,12,1);
		 else
        OLED_ShowChar(x,y,*p,16,1);
	 if(*p==46	) 
	    x+=6;
	 else		 
        x+=12;
        p++;
    }  
}

void OLED_ShowString_11(u8 x,u8 y, u8 *p)
{
// #define MAX_CHAR_POSX 122
 #define MAX_CHAR_POSX 126
#define MAX_CHAR_POSY 58          
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,16,1);
	 if(*p==46	) 
	    x+=6;
	 else		 
        x+=12;
        p++;
    }  
}
void OLED_ShowString_24(u8 x,u8 y, char *p)
{
// #define MAX_CHAR_POSX 122
 #define MAX_CHAR_POSX 126
#define MAX_CHAR_POSY 58          
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,16,1);
	 if(*p==46	) 
	    x+=6;
	 else		 
        x+=12;
        p++;
    }  
}
void OLED_ShowString_40(u8 x,u8 y, char *p)
{
// #define MAX_CHAR_POSX 122
 #define MAX_CHAR_POSX 126
#define MAX_CHAR_POSY 58 
         
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,40,1);
		if(*p==46	)  
         x+=10;
		else
		 x+=20;
        p++;
    }  
}
void OLED_ShowString(u8 x,u8 y, u16 *p)
{
// #define MAX_CHAR_POSX 122
 #define MAX_CHAR_POSX 126
#define MAX_CHAR_POSY 58          
    while(*p!='\0')
    {       
        if(x>MAX_CHAR_POSX){x=0;y+=16;}
        if(y>MAX_CHAR_POSY){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,16,1);	 
        x+=13;
        p++;
    }  
}	   
//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	
 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PC,D,G�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_8;	 //PD3,PD6�������  
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
 	GPIO_Init(GPIOD, &GPIO_InitStructure);	  //��ʼ��GPIOD3,6
 	GPIO_SetBits(GPIOD,GPIO_Pin_10|GPIO_Pin_8);	//PD3,PD6 �����

 #if OLED_MODE==1
 
 	GPIO_InitStructure.GPIO_Pin =0xFF; //PC0~7 OUT�������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOC,0xFF); //PC0~7�����

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //PG13,14,15 OUT�������
 	GPIO_Init(GPIOG, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOG,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);						 //PG13,14,15 OUT  �����

 #else
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;				 //PC0,1 OUT�������
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOD,GPIO_Pin_11|GPIO_Pin_12);						 //PC0,1 OUT  �����

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //PG15 OUT�������	  RST
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOD,GPIO_Pin_9);						 //PG15 OUT  �����


 #endif
  							  
	OLED_RST_Clr();
	delay_ms(200);
	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD); //�ر���ʾ
	OLED_WR_Byte(0xD5,OLED_CMD); //����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_WR_Byte(80,OLED_CMD);   //[3:0],��Ƶ����;[7:4],��Ƶ��
	OLED_WR_Byte(0xA8,OLED_CMD); //��������·��
	OLED_WR_Byte(0X3F,OLED_CMD); //Ĭ��0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //������ʾƫ��
	OLED_WR_Byte(0X00,OLED_CMD); //Ĭ��Ϊ0

	OLED_WR_Byte(0x40,OLED_CMD); //������ʾ��ʼ�� [5:0],����.
													    
	OLED_WR_Byte(0x8D,OLED_CMD); //��ɱ�����
	OLED_WR_Byte(0x14,OLED_CMD); //bit2������/�ر�
	OLED_WR_Byte(0x20,OLED_CMD); //�����ڴ��ַģʽ
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
	OLED_WR_Byte(0xA1,OLED_CMD); //���ض�������,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
	OLED_WR_Byte(0xDA,OLED_CMD); //����COMӲ����������
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]����
		 
	OLED_WR_Byte(0x81,OLED_CMD); //�Աȶ�����
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLED_WR_Byte(0xD9,OLED_CMD); //����Ԥ�������
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //����VCOMH ��ѹ����
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Byte(0xA4,OLED_CMD); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLED_WR_Byte(0xA6,OLED_CMD); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	    						   
	OLED_WR_Byte(0xAF,OLED_CMD); //������ʾ	 
	OLED_Clear();
}


void OLED_Display1( char *p1,char *p3)
{
 // uint8 i;
  
  	 //   OLED_ShowString_16(1,1, p1); 
    
	 OLED_ShowString_24_1(2,24,p1);

	   OLED_ShowChar_1(40,14,0,1);
	   OLED_ShowString_40(44,12,p3);
//      	  LCM_Command(0x80,1);
//      	  OSTimeDly(1);	
//		  for(i=0;i<8;i++)
//		  {
//		  	LCM_WriteData(*p1++,1);
//		  	OSTimeDly(1);
//		  }
//		  LCM_Command(0xc0,1);
//		  OSTimeDly(1);	
//		  for(i=0;i<8;i++)
//		  {
//		  	LCM_WriteData(*p2++,1);
//		  	OSTimeDly(1);
//		  }
}

void OLED_Display2( char *p1, char *p2,char *p3)
{
 // uint8 i;
  
 
     OLED_ShowString_24(12,14,p1); 
	 OLED_ShowString_24(12,34,p2);
 
	   OLED_ShowChar_1(40,14,0,1);
	   OLED_ShowString_40(44,12,p3);

}  

void OLED_Display3( char *p1, char *p2)
{
    u8 i;//cc;
	u16 data_temp_number;
	i=OLED_curPos;
	//get_params(i) 


	   data_temp_number=get_params(i);

	 if(i==4)
		OLED_ShowString_40(24,4,p2);
//	 else if((data_temp_number>=0)&&(data_temp_number<10))
     else if(data_temp_number<10)
          OLED_ShowString_40(54,4,p2); 
	   else	 if((data_temp_number>=10)&&(data_temp_number<100))
          OLED_ShowString_40(44,4,p2);
	   	else if((data_temp_number>=100)&&(data_temp_number<1000))
          OLED_ShowString_40(36,4,p2);
	   	else if((data_temp_number>=1000)&&(data_temp_number<10000))
          OLED_ShowString_40(24,4,p2);
		  else
			 OLED_ShowString_40(14,4,p2);
 	if(i==2)
	     OLED_ShowChar_12(40,44,0,1);
	 else if(i==3)
	     OLED_ShowChar_12(40,44,1,1);
	  else	if((i==4))
		    OLED_ShowChar_12(40,44,2,1);
		  else	if(i==5)
			  OLED_ShowChar_12(40,44,3,1);
		  
	 
 
} 
void OLED_Display4( char *p1, char *p2)
{
//    u8 i;//cc;
//	u16 data_temp_number;
//	i=OLED_curPos;
	//get_params(i) 
	OLED_ShowString_40(14,14,p1); 
    OLED_ShowString_40(54,14,":");
    OLED_ShowString_40(66,14,p2);
}
 void OLED_Display5( char *p1, char *p2)
{
    u8 i;//cc;
	u16 data_temp_number;
	int16_t data_temp_number1;
	i=OLED_curPos;
	//get_params(i) 
	 if(i==0)
	   data_temp_number1=Temperature;//get_params2(i);

	 if(i==1)
	   data_temp_number=get_params(i);

	 if(i==0)
	    {
		  if(data_temp_number1<=-10) //4λ
		   
			{OLED_ShowString_40(24,14,"-"); OLED_ShowString_40(44,14,p2); OLED_ShowChar_12(84,14,4,1); }
			 else  if(data_temp_number1<0) //3λ
			  { OLED_ShowString_40(34,14,"-");OLED_ShowString_40(54,14,p2);   OLED_ShowChar_12(74,14,4,1); }
			   	else  if(data_temp_number1<10)
			       {OLED_ShowString_40(44,14,p2); OLED_ShowChar_12(64,14,4,1); }	//2λ
				 	else   { OLED_ShowString_40(34,14,p2); OLED_ShowChar_12(74,14,4,1); }  //3λ
		
		}
		if(i==1)
	    {
		  if(data_temp_number<10)
			{ {OLED_ShowString_40(44,14,p2);	 OLED_ShowChar_12(64,14,5,1); }	   }  //2λ
			 else  {  OLED_ShowString_40(34,14,p2);	 OLED_ShowChar_12(74,14,5,1); }  //3λ
		
		}
}
void LCD_time_update(void)
{
//    u8 i;//cc;
//	i=OLED_curPos;
		 RTC_Set_Time3=Timer;

//		data_buffer[0]=RTC_Set_Time3.w_year%100;
//		data_buffer[1]=RTC_Set_Time3.w_month;
//		data_buffer[2]=RTC_Set_Time3.w_date;
//	    data_buffer[3]=RTC_Set_Time3.hour;
//		data_buffer[4]=RTC_Set_Time3.min;
//		data_buffer[5]=RTC_Set_Time3.sec;

		 OLED_Clear();
	    sprintf(OLED_menu1,"%2d",RTC_Set_Time3.hour); 
		sprintf(OLED_menu4,"%02d",RTC_Set_Time3.min); 
		OLED_Display4(OLED_menu1,OLED_menu4);
		OLED_Refresh_Gram();
	
	  
}



























