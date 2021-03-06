#include "timer.h"
//#include "basic.h"
#include "gpio.h"
//#include "task.h" 

//extern u8 PubFlagSecond;
//u8 PubFlagSecond;
//void DisplayRefresh(void);
//通用定时器中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!

void TIM_SMCR_ECEConfig(TIM_TypeDef* TIMx, FunctionalState NewState);  //TIM_ETRClockMode2Config   xgy20140417

void TIM_SMCR_SMSConfig(TIM_TypeDef* TIMx, uint16_t TIM_slave_select); //sms

void TIM_SMCR_ETPConfig(TIM_TypeDef* TIMx,FunctionalState NewState); //ece

void TIM_SMCR_ETPSConfig(TIM_TypeDef* TIMx,uint16_t TIM_Etr_psc); //外部触发预分频

void	TIM_SMCR_ETFConfig(TIM_TypeDef* TIMx,uint16_t TIM_Etr_filter); //外部触发滤波

void	TIM_CCMR1_CC1sConfig(TIM_TypeDef* TIMx,uint16_t TIM_cap_comp); //捕获/比较 1选择

void	TIM_CCMR1_IC1PSCConfig(TIM_TypeDef* TIMx,uint16_t TIM_cap_psc); //输入捕获1预分频器

void TIM_CCMR1_IC1FConfig(TIM_TypeDef* TIMx,uint16_t TIM_cap_filter); //输入捕获1滤波器

void TIM_CCER_CC1Econfig(TIM_TypeDef* TIMx, FunctionalState NewState); //计数器的值存入CCR1

u32 Fre_Test;   // test frequency
extern u8 Task_Flag;
extern u32 vncl_Sys_Counter;
extern  u32 Sys_Counter;
//static u32 Sys_Counter=0;
//extern u8 Timer_280us_Flag;
//extern u8 Timer_40us_Flag;	
u8 ADC_Start_Flag=1;

void Timer1_Init(void)
{
 	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能
	TIM_TimeBaseStructure.TIM_Period = 0xffff; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到200为20ms
	//TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_Prescaler =0; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 	
	TIM_SMCR_ECEConfig(TIM1,ENABLE); //ece=1  //ext clock eanble  //smcr从模式控制寄存器		
	TIM_SMCR_ETPConfig(TIM1,DISABLE); //etp=0    //外部触发极性  不反向
	TIM_SMCR_ETPSConfig(TIM1,TIM_SMCR_ETPS_1); //etps=10    //外部触发预分频  4 
	TIM_SMCR_ETFConfig(TIM1,0x0000); //etf=0000    //外部触发滤波  无
	TIM_CCMR1_CC1sConfig(TIM1,TIM_CCMR1_CC1S_0|TIM_CCMR1_CC1S_1); //cc1s=11  //捕获/比较1选择  cc1通道被配置为输入，ic1映射到trc上  //ccmr1 输入/捕获模式寄存器
	TIM_CCMR1_IC1PSCConfig(TIM1,0x00); //IC1PSC=00      //输入/捕获1预分频器   无 	
	TIM_CCMR1_IC1FConfig(TIM1,0x0000); //ic1f=0000      //输入捕获1滤波器  无 
	TIM_SelectInputTrigger(TIM1,    //TS=0x001               //内部触发1(ITR1)，TIM2
		TIM_SMCR_TS_0);	 		
	TIM_SMCR_SMSConfig(TIM1,TIM_SMCR_SMS_2); //sms=100    //选中的触发输入(TRGI)的上升沿重新初始化计数器，并产生一个更新寄存器的信号	
	TIM_CCER_CC1Econfig(TIM1,ENABLE); //CC1e=1  //允许将cnt的值捕获如CCR1			
	TIM_ITConfig(  //TIM中断使能
		TIM1, //TIM1
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger |  //TIM 触发中断源 
		TIM_IT_CC1,       //允许捕获/比较1中
		ENABLE  //使能
		);		
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;  //TIM1中断  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	TIM_Cmd(TIM1, ENABLE);  //使能TIMx外设   使能计数器		
}	
void Timer3_Init(void)
{
 	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //????	 
	//TIM_TimeBaseStructure.TIM_Period = 200; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到200为20ms
	TIM_TimeBaseStructure.TIM_Period = 9999; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到10000为1s
	//TIM_TimeBaseStructure.TIM_Period = 29999; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到30000为3s
	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; ////设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位	 
	TIM_InternalClockConfig( //SMCR  SMS=000 ???
		TIM3); //TIM2	 		
	TIM_SMCR_ECEConfig(TIM3,DISABLE); //smcr ece=0	   	
	TIM_SelectOutputTrigger( //更新--更新事件用于触发输出
		TIM3,
		TIM_CR2_MMS_1);		 
	TIM_ITConfig(  //使能或者失能指定的TIM中断  dier
		TIM3, //TIM2
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //响应优先级1级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器	   
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设	
}

static void TIM3_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM3 clock enable */
	//PCLK1经过2倍频后作为TIM3的时钟源等于72MHz
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 

  /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // 复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

//  /*GPIOB Configuration: TIM3 channel 3 and 4 as alternate function push-pull */
//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
//
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
static void TIM3_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	/* PWM信号电平跳变值 */
	u16 CCR1_Val = 500;        
	u16 CCR2_Val = 500;
//	u16 CCR3_Val = 250;
//	u16 CCR4_Val = 125;

/* -----------------------------------------------------------------------
    TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
    TIM3CLK = 72 MHz, Prescaler = 0x0, TIM3 counter clock = 72 MHz
    TIM3 ARR Register = 999 => TIM3 Frequency = TIM3 counter clock/(ARR + 1)
    TIM3 Frequency = 72 KHz.
    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
    TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
    TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%
  ----------------------------------------------------------------------- */

  /* Time base configuration */		 
  TIM_TimeBaseStructure.TIM_Period = 999;       //当定时器从0计数到999，即为1000次，为一个定时周期
  TIM_TimeBaseStructure.TIM_Prescaler = 16;	    //设置预分频：不预分频，即为72MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4 ;	//设置时钟分频系数：不分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //使能通道1

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //设置通道2的电平跳变值，输出另外一个占空比的PWM

  TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  //使能通道2

  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//设置通道3的电平跳变值，输出另外一个占空比的PWM
//
//  TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //使能通道3
//
//  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
//
//  /* PWM1 Mode configuration: Channel4 */
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	//设置通道4的电平跳变值，输出另外一个占空比的PWM
//
//  TIM_OC4Init(TIM3, &TIM_OCInitStructure);	//使能通道4
//
//  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM3, ENABLE);			 // 使能TIM3重载寄存器ARR

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);                   //使能定时器3	
}

/*
 * 函数名：TIM3_PWM_Init
 * 描述  ：TIM3 输出PWM信号初始化，只要调用这个函数
 *         TIM3的四个通道就会有PWM信号输出
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void TIM3_PWM_Init(void)
{
	TIM3_GPIO_Config();
	TIM3_Mode_Config();	
}


	

void Timer2_Init(uint16_t TIM_Period,uint16_t TIM_Prescaler)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	TIM_TimeBaseStructure.TIM_Period = TIM_Period; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到28为280us
	TIM_TimeBaseStructure.TIM_Prescaler =TIM_Prescaler; //设置用来作为TIMx时钟频率除数的预分频值  100Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位    
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM2, //TIM3
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设	
}
void Timer4_Init(void)
{
 	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	//TIM_TimeBaseStructure.TIM_Period = 2; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到200为20ms
	TIM_TimeBaseStructure.TIM_Period = 9; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到10为1ms
	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  	
	//TIM_TimeBaseStructure.TIM_Prescaler =(7200-10); //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM4, //TIM4
		TIM_IT_Update  |  //TIM 中断源
		TIM_IT_Trigger,   //TIM 触发中断源 
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
	TIM_Cmd(TIM4, ENABLE);  //使能TIMx外设	
}	

/******************************************************************************/
// Name:    void Timerx_Init(void)
// Para:    arr,psc
// Return:  None
/******************************************************************************/
void Timerx_Init(void)
{  
   	////////////////////////////////////////////////tim1///////////////////////////////////////////////////////////////////////////	
	Timer1_Init();	
	//////////////////////////////////////tim2////////////////////////////////////////////////////	 
	Timer3_Init();
  //  TIM3_PWM_Init();	
	////////////////////////////////////////////////tim3///////////////////////////////////////////////////////////////////////////
	Timer2_Init(27,719);  //100kHz  	 280us	
	////////////////////////////////////////////////tim4///////////////////////////////////////////////////////////////////////////	
	Timer4_Init();								 
}

///////////////////////////////////////////////////////////////////////////////////
// Name:    void TIM1_IRQHandler(void)                                      	 //
// Para:    None														       	//	
// Return:  None															    //
/////////////////////////////////////////////////////////////////////////////////  
void TIM1_CC_IRQHandler(void)
{	   
	if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET) //检查指定的TIM中断发生与否:TIM 中断源    
	{
		TIM_Cmd(TIM4, DISABLE);  //使能TIMx外设
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1  );  //清除TIMx的中断待处理位:TIM 中断  TIM1->CCR1		
		Fre_Test=(u32) TIM_GetCapture1(TIM1);
		TIM1->CNT=0;
		TIM4->CNT=0;
		TIM_Cmd(TIM4,ENABLE);  //使能TIMx外设			
	}		
}


/////////////////////////////////////////////////////////////////////////////////
// Name:    void TIM2_IRQHandler(void)                                         //
// Para:    None																															 //	
// Return:  None																												       //
/////////////////////////////////////////////////////////////////////////////////
void TIM3_IRQHandler(void)   //TIM2中断	 用于产生时间触发T1计数
{	 
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 	  
	{		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断�
 					
	}		
}



/////////////////////////////////////////////////////////////////////////////////
// Name:    void TIM3_IRQHandler(void)                                         //
// Para:    None																															 //	
// Return:  None																												       //
/////////////////////////////////////////////////////////////////////////////////
void TIM2_IRQHandler(void)   //TIM3中断	用于ADC延时采样
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
		TIM_Cmd(TIM2, DISABLE);  //禁止TIMx外设	
		//ADC_Cmd(ADC1, ENABLE);			   			
		/* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
		//if(Timer_280us_Flag)
		//{
		 	//PM25_LED=0;
//			BEEP=1;
	
			ADC_Cmd(ADC1, ENABLE);  
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);
			//ADC_Start_Flag=1;
//			BEEP=0;
			//BEEP=1;	 
			//Timer_280us_Flag=0;
			//Timer_40us_Flag=1;
			//Timer3_Init(4,71999);   //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到4为40us
			//ADC_Cmd(ADC1, ENABLE);	 //使能TIMx外设	
			//PM25_LED=1;
		//}
		//else if(Timer_40us_Flag)
		//{
		   //PM25_LED=0;
		   //PM25_LED=~PM25_LED;
		   //Timer_40us_Flag=1;
		   //Timer3_Init(28,71999); //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到28为280us
		   	//Timer3_Init(40,71999);   //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到4为40us
		//	ADC_Cmd(ADC1, ENABLE);
		//}
	}
}


/////////////////////////////////////////////////////////////////////////////////
// Name:    void TIM4_IRQHandler(void)                                         //
// Para:    None																															 //	
// Return:  None																												       //
/////////////////////////////////////////////////////////////////////////////////

void TIM4_IRQHandler(void)   //TIM4中断  作为系统运行时钟
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断�
		Sys_Counter++;		//系统计数时钟
		vncl_Sys_Counter++;
		Task_Flag=1;  	
	}
}

void TIM_CCER_CC1Econfig(TIM_TypeDef* TIMx, FunctionalState NewState)
{
	// Check the parameters //  
  assert_param(IS_TIM_ALL_PERIPH(TIMx));	
  assert_param(IS_FUNCTIONAL_STATE(NewState));
	
  if (NewState != DISABLE)
  {
      TIMx->CCER |= TIM_CCER_CC1E;
  }
  else
  {
    TIMx->CCER &=~TIM_CCER_CC1E; 
  }
}


void TIM_SMCR_ECEConfig(TIM_TypeDef* TIMx, FunctionalState NewState)  //TIM_ETRClockMode2Config
{
	// Check the parameters //  
  assert_param(IS_TIM_ALL_PERIPH(TIMx));	
  assert_param(IS_FUNCTIONAL_STATE(NewState));
	
  if (NewState != DISABLE)
  {
      TIMx->SMCR |= TIM_SMCR_ECE;
  }
  else
  {
    TIMx->SMCR &=~TIM_SMCR_ECE; 
  }
} 
void TIM_SMCR_SMSConfig(TIM_TypeDef* TIMx, uint16_t TIM_slave_select) //sms=100
{
		uint16_t tmpsmcr = 0;
		assert_param(IS_TIM_ALL_PERIPH(TIMx));	
		assert_param(IS_FUNCTIONAL_STATE(TIM_slave_select));
		// Get the TIMx SMCR register value //
		tmpsmcr = TIMx->SMCR;
		// Reset the TS Bits //
		tmpsmcr &= (uint16_t)(~((uint16_t)TIM_SMCR_SMS));
		// Set the Input Trigger source //
		tmpsmcr |= TIM_slave_select;
		// Write to TIMx SMCR //
		TIMx->SMCR = tmpsmcr;
	
}
void TIM_SMCR_ETPConfig(TIM_TypeDef* TIMx,FunctionalState NewState) //ece=0
{
		// Check the parameters //
		assert_param(IS_TIM_ALL_PERIPH(TIMx));	
		assert_param(IS_FUNCTIONAL_STATE(NewState));
		
		if (NewState != DISABLE)
		{
				TIMx->SMCR |= TIM_SMCR_ETP;
		}
		else
		{
			TIMx->SMCR &=~TIM_SMCR_ETP; 
		}
}
void TIM_SMCR_ETPSConfig(TIM_TypeDef* TIMx,uint16_t TIM_Etr_psc) //外部触发预分频
{
	uint16_t tmpsmcr = 0;
	assert_param(IS_TIM_ALL_PERIPH(TIMx));	
	assert_param(IS_FUNCTIONAL_STATE(TIM_Etr_psc));
	// Get the TIMx SMCR register value //
	tmpsmcr = TIMx->SMCR;
	// Reset the TS Bits //
	tmpsmcr &= (uint16_t)(~((uint16_t)TIM_SMCR_ETPS));
	// Set the Input Trigger source //
	tmpsmcr |= TIM_Etr_psc;
	// Write to TIMx SMCR //
	TIMx->SMCR = tmpsmcr;
}
void	TIM_SMCR_ETFConfig(TIM_TypeDef* TIMx,uint16_t TIM_Etr_filter) //外部触发滤波
{
	uint16_t tmpsmcr = 0;
	assert_param(IS_TIM_ALL_PERIPH(TIMx));	
	assert_param(IS_FUNCTIONAL_STATE(TIM_Etr_filter));
	// Get the TIMx SMCR register value //
	tmpsmcr = TIMx->SMCR;
	// Reset the TS Bits //
	tmpsmcr &= (uint16_t)(~((uint16_t)TIM_SMCR_ETF));
	// Set the Input Trigger source //
	tmpsmcr |= TIM_Etr_filter;
	// Write to TIMx SMCR //
	TIMx->SMCR = tmpsmcr;
}
void	TIM_CCMR1_CC1sConfig(TIM_TypeDef* TIMx,uint16_t TIM_cap_comp) //捕获/比较 1选择
{
	uint16_t tmpsmcr = 0;
	assert_param(IS_TIM_ALL_PERIPH(TIMx));	
	assert_param(IS_FUNCTIONAL_STATE(TIM_cap_comp));
	// Get the TIMx SMCR register value //
	tmpsmcr = TIMx->CCMR1; 
	// Reset the TS Bits //
	tmpsmcr &= (uint16_t)(~((uint16_t)TIM_CCMR1_CC1S));
	// Set the Input Trigger source //
	tmpsmcr |= TIM_cap_comp;
	// Write to TIMx SMCR //
	TIMx->CCMR1 = tmpsmcr;
}
void	TIM_CCMR1_IC1PSCConfig(TIM_TypeDef* TIMx,uint16_t TIM_cap_psc) //输入捕获1预分频器
{
	uint16_t tmpsmcr = 0;
	assert_param(IS_TIM_ALL_PERIPH(TIMx));	
	assert_param(IS_FUNCTIONAL_STATE(TIM_cap_psc));
	// Get the TIMx SMCR register value //
	tmpsmcr = TIMx->CCMR1;
	// Reset the TS Bits //
	tmpsmcr &= (uint16_t)(~((uint16_t)TIM_CCMR1_IC1PSC));
	// Set the Input Trigger source //
	tmpsmcr |= TIM_cap_psc;
	// Write to TIMx SMCR //
	TIMx->CCMR1 = tmpsmcr;
}

void TIM_CCMR1_IC1FConfig(TIM_TypeDef* TIMx,uint16_t TIM_cap_filter) //输入捕获1滤波器
{
	uint16_t tmpsmcr = 0;
	assert_param(IS_TIM_ALL_PERIPH(TIMx));	
	assert_param(IS_FUNCTIONAL_STATE(TIM_cap_filter));
	// Get the TIMx SMCR register value //
	tmpsmcr = TIMx->CCMR1;
	// Reset the TS Bits //
	tmpsmcr &= (uint16_t)(~((uint16_t)TIM_CCMR1_IC1F));
	// Set the Input Trigger source //
	tmpsmcr |= TIM_cap_filter;
	// Write to TIMx SMCR //
	TIMx->CCMR1 = tmpsmcr;
}







