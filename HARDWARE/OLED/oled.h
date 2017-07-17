#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    
//////////////////////////////////////////////////////////////////////////////////	 
//imodule	
//////////////////////////////////////////////////////////////////////////////////	

//OLED模式设置
//0:4线串行模式
//1:并行8080模式
#define OLED_MODE 0
		    						  
//-----------------OLED端口定义----------------  					   

#define OLED_CS_Clr()  GPIO_ResetBits(GPIOD,GPIO_Pin_8)
#define OLED_CS_Set()  GPIO_SetBits(GPIOD,GPIO_Pin_8)



#define OLED_RST_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_9)
#define OLED_RST_Set() GPIO_SetBits(GPIOD,GPIO_Pin_9)

#define OLED_RS_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_10)
#define OLED_RS_Set() GPIO_SetBits(GPIOD,GPIO_Pin_10)

#define OLED_WR_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_14)
#define OLED_WR_Set() GPIO_SetBits(GPIOG,GPIO_Pin_14)

#define OLED_RD_Clr() GPIO_ResetBits(GPIOG,GPIO_Pin_13)
#define OLED_RD_Set() GPIO_SetBits(GPIOG,GPIO_Pin_13)



//PC0~7,作为数据线
#define DATAOUT(x) GPIO_Write(GPIOC,x);//输出  
//使用4线串行接口时使用 

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_11)
#define OLED_SCLK_Set() GPIO_SetBits(GPIOD,GPIO_Pin_11)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOD,GPIO_Pin_12)
#define OLED_SDIN_Set() GPIO_SetBits(GPIOD,GPIO_Pin_12)

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u16 *p);
void OLED_ShowChar_11(u8 x,u8 y,u8 z, u8 mode);
void OLED_ShowChar_1(u8 x,u8 y,u8 z, u8 mode);
 void OLED_Display5( char *p1, char *p2);
void OLED_Display3( char *p1,char *p2);
void OLED_Display2( char *p1, char *p2,char *p3);
void OLED_Display1( char *p1, char *p3);
void LCD2_update(void);
extern u16 get_params(u8 num);
extern int16_t get_params2(u8 num);
extern float get_params1(u8 num);
extern float Sht21Work(u8 order);
extern void EC808_Process(void);
extern void EC805_Process(void); 
extern void USART3_TX_State(void);	   	 
#endif  
	 



