#include "stm32f10x.h"
#include "control.h"

void Control_Init(void)//初始化
{ 
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	                        //使能PB口时钟	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;		//PB.3,PB.4,PB.5,PB.6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		                        //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                               //
    GPIO_Init(GPIOB, &GPIO_InitStructure);                                          //配置参数
    //GPIO_SetBits(GPIOA,GPIO_Pin_3);						                            //初始化LCD_CS--PA.3片选失效 
}
