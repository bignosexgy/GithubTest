#include "stm32f10x.h"
#include "gpio.h"
#include "sys.h"
  
/*********************************************************************/
// PA.0				SPI2_MP3_DERQ		            	GPIO_Mode_Out_PP			
// PA.1				SPI2_MP3_XDC						GPIO_Mode_Out_PP
// PA.2				TX2_CO					GPIO_Mode_Out_PP					
// PA.3				RX2_CO			    GPIO_Mode_Out_PP		Interrupt
// PA.4				SPI2_MP3_NSS			    GPIO_Mode_Out_PP
// PA.5				SPI2_MP3_SCK			    GPIO_Mode_Out_PP
// PA.6				SPI2_MP3_MISO			    GPIO_Mode_Out_PP
// PA.7				SPI2_MP3_MOSI			    GPIO_Mode_Out_PP
// PA.8							            GPIO_Mode_Out_PP
// PA.9				TxD1_vifi			            
// PA.10			RxD1_vifi						
// PA.11			USBDM						GPIO_Mode_Out_PP
// PA.12			USBDP   		GPIO_Mode_Out_PP	
// PA.13			JTMS/SWDIO						
// PA.14			JTCK/SWCLK						
// PA.15			JTDI				GPIO_Mode_Out_PP	

// PB.0				SDA_1			    GPIO_Mode_Out_PP
// PB.1				SCL_1				GPIO_Mode_Out_PP
// PB.2				BOOT1				
// PB.3				JTDO				GPIO_Mode_Out_PP
// PB.4				JTRST             GPIO_Mode_Out_PP
// PB.5							    GPIO_Mode_Out_PP
// PB.6				SCL1	            GPIO_Mode_Out_PP		Interrupt
// PB.7				SDA1	            GPIO_Mode_Out_PP
// PB.8				LED_LEFT            GPIO_Mode_Out_PP
// PB.9				LED_RIGHT              GPIO_Mode_Out_PP
// 				
// PB.10			SCL2				GPIO_Mode_Out_PP
// PB.11			SDA2				GPIO_Mode_Out_PP
// PB.12			FLASH_CS				    GPIO_Mode_Out_PP
// PB.13			FLASH_SCK				    GPIO_Mode_Out_PP
// PB.14			FLASH_DO			        GPIO_Mode_Out_PP
// PB.15			FLASH_DIO				    GPIO_Mode_Out_PP

// PC.13			LED		                GPIO_Mode_Out_PP
/*********************************************************************/

u8 Filament_Flag;



//初始化GPIO.并使能时钟		    
//GPIO初始化
void MY_GPIO_Init(void) 
{
//PA 
    GPIO_InitTypeDef  GPIO_InitStructure;
    //////////////////PA//////////////////////////////
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	         //使能PA端口时钟
  
		 
		   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_8|GPIO_Pin_0;				 //输出控制-->PA.8、PA.11、PA.12 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		    //推挽输出
 //   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
	GPIO_SetBits(GPIOA,GPIO_Pin_8); 
	GPIO_SetBits(GPIOA,GPIO_Pin_0);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;			 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

	///////////////////////////PB/////////////////////////////////////////
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //使能PB端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_9|GPIO_Pin_8;
	//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	    		 //控制输出-->PB9 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);		 
   GPIO_SetBits(GPIOB,GPIO_Pin_9); 	 	 //PB.9 输出高 
	GPIO_SetBits(GPIOB,GPIO_Pin_8); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;			 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure); 

//PC   /////////////////////////PC//////////////////////////////////////
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);   //使能PC端口时钟

	
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	    		 //-->PC.0123 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //	OK
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;  
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;	    		 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;  
    GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	    		 //-->PC.0123 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //	OK
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;  
    GPIO_Init(GPIOC, &GPIO_InitStructure);	

 //PD   /////////////////////////PD//////////////////////////////////////

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);   // OK
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	    		 //-->PC.0123 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	    		 //-->PC.0123 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;	    		 //-->PC.0123 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
    GPIO_Init(GPIOD, &GPIO_InitStructure);	
	
	
//PE   /////////////////////////PE//////////////////////////////////////


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);   // PE2-WIFI

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_10|GPIO_Pin_14;	    		 //-->PC.0123 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //	上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
    GPIO_Init(GPIOE, &GPIO_InitStructure);
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	    		 //-->PC.0123 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //	//	下拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
    GPIO_Init(GPIOE, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin =GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;	    		 //-->PC.0123 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
    GPIO_Init(GPIOE, &GPIO_InitStructure);

}





