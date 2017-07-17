#ifndef __CONTROL_H
#define __CONTROL_H 
#include "stm32f10x.h"

//端口定义

#define SCL1 PBout(7)	// PB7
#define SDA1 PBout(8)	// PB8



void Control_Init(void);//初始化
		 				    
#endif
