#ifndef __GPIO_H
#define __GPIO_H	 
#include "stm32f10x.h"



//#define BEEP PAout(12)//  PA12	

#define SCL1 PBout(0)	// PB7 OK
#define SDA1 PBout(1)	// PB8

//#define PLAY1 PBout(12)	// PB12
//#define PLAY2 PBout(9)	// PB13

#define LED_sysrun  PAout(4)// 

#define BAT_LED_R        PDout(11)// PB9  OK
#define BAT_LED_G        PDout(12)// PA11	
#define BAT_LED_B        PDout(13)// PA11
#define CO2_LED_red      PDout(14)// PA11	
#define CO2_LED_green    PDout(15)// PA11
#define PM25_LED_red     PCout(6) // PA11	 
#define PM25_LED_green   PCout(7)// PA11	


#define PM25_fan  PDout(1)	// PB8 OK	 
#define PM25_LED  PAout(1)// PA11

#define CO2_C  PDout(8)	// PB8 OK
#define sound_C  PEout(9)	// PB8 OK

#define Ceshi_flash  PDin(5)	// PD34567 OK
#define Ceshi1  PBin(0)	// PB8 OK  PBin(1) PCin(4) PBin(5)

#define WIFI  PEin(2)
#define power_flag  PEin(3)
//#define E	P2_4				// Read/Write Enable
//#define RW	P2_3				// Read/Write Select
//
//#define RD	P2_4				// Read Signal
//#define WR	P2_3				// Write Signal

				// Reset
//#define CS_oled	    PEout(15)	//OK			// Chip Select
#define DC_oled	    PEout(14)				// Data/Command Control
#define RES_oled	PEout(13)
#define SCLK	    PEout(12)			// Serial Clock Input
#define SDIN	    PEout(11)
#define Rom_CS   	PEout(10)
#define Rom_SCK    	PEout(9)
#define Rom_IN      PEout(8)
#define Rom_OUT 	PEin(7)//Key_Scan(GPIOB,GPIO_Pin_13)





void MY_GPIO_Init(void);//≥ı ºªØ






		 				    
#endif
