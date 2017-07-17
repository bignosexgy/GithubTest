#include "exti.h"
#include "gpio.h"
#include "delay.h"
//#include "basic.h"  
//#include "stm32f10x.h"


extern u8 FlagKeyInt;


//外部中断0服务程序
//初始化PA0,PA13,PA15为中断输入.
void EXTIX_Init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;


  	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	//关闭jtag

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);    //Enables or disables the High Speed APB (APB2) peripheral clock.

  //初始化 KEY0-->GPIOB.6 下拉输入
  	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);

  //GPIOB.6 中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource6);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line6;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;					//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
}

void EXTI9_5_IRQHandler(void)
{
 	delay_ms(10);    //消抖			 
 
    if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
        FlagKeyInt = 1;
	}
	EXTI_ClearITPendingBit(EXTI_Line6);  //清除EXTI6线路挂起位
}


