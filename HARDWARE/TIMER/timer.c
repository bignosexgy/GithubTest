#include "timer.h"
//#include "basic.h"
#include "gpio.h"
//#include "task.h" 

//extern u8 PubFlagSecond;
//u8 PubFlagSecond;
//void DisplayRefresh(void);
//Í¨ÓÃ¶¨Ê±Æ÷ÖÐ¶Ï³õÊ¼»¯
//ÕâÀïÊ±ÖÓÑ¡ÔñÎªAPB1µÄ2±¶£¬¶øAPB1Îª36M
//arr£º×Ô¶¯ÖØ×°Öµ¡£
//psc£ºÊ±ÖÓÔ¤·ÖÆµÊý
//ÕâÀïÊ¹ÓÃµÄÊÇ¶¨Ê±Æ÷3!

void TIM_SMCR_ECEConfig(TIM_TypeDef* TIMx, FunctionalState NewState);  //TIM_ETRClockMode2Config   xgy20140417

void TIM_SMCR_SMSConfig(TIM_TypeDef* TIMx, uint16_t TIM_slave_select); //sms

void TIM_SMCR_ETPConfig(TIM_TypeDef* TIMx,FunctionalState NewState); //ece

void TIM_SMCR_ETPSConfig(TIM_TypeDef* TIMx,uint16_t TIM_Etr_psc); //Íâ²¿´¥·¢Ô¤·ÖÆµ

void	TIM_SMCR_ETFConfig(TIM_TypeDef* TIMx,uint16_t TIM_Etr_filter); //Íâ²¿´¥·¢ÂË²¨

void	TIM_CCMR1_CC1sConfig(TIM_TypeDef* TIMx,uint16_t TIM_cap_comp); //²¶»ñ/±È½Ï 1Ñ¡Ôñ

void	TIM_CCMR1_IC1PSCConfig(TIM_TypeDef* TIMx,uint16_t TIM_cap_psc); //ÊäÈë²¶»ñ1Ô¤·ÖÆµÆ÷

void TIM_CCMR1_IC1FConfig(TIM_TypeDef* TIMx,uint16_t TIM_cap_filter); //ÊäÈë²¶»ñ1ÂË²¨Æ÷

void TIM_CCER_CC1Econfig(TIM_TypeDef* TIMx, FunctionalState NewState); //¼ÆÊýÆ÷µÄÖµ´æÈëCCR1

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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //Ê±ÖÓÊ¹ÄÜ
	TIM_TimeBaseStructure.TIM_Period = 0xffff; //ÉèÖÃÔÚÏÂÒ»¸ö¸üÐÂÊÂ¼þ×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ	 ¼ÆÊýµ½200Îª20ms
	//TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //ÉèÖÃÓÃÀ´×÷ÎªTIMxÊ±ÖÓÆµÂÊ³ýÊýµÄÔ¤·ÖÆµÖµ  10KhzµÄ¼ÆÊýÆµÂÊ  
	TIM_TimeBaseStructure.TIM_Prescaler =0; //ÉèÖÃÓÃÀ´×÷ÎªTIMxÊ±ÖÓÆµÂÊ³ýÊýµÄÔ¤·ÖÆµÖµ  10KhzµÄ¼ÆÊýÆµÂÊ  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //ÉèÖÃÊ±ÖÓ·Ö¸î:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMÏòÉÏ¼ÆÊýÄ£Ê½
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //¸ù¾ÝTIM_TimeBaseInitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯TIMxµÄÊ±¼ä»ùÊýµ¥Î» 	
	TIM_SMCR_ECEConfig(TIM1,ENABLE); //ece=1  //ext clock eanble  //smcr´ÓÄ£Ê½¿ØÖÆ¼Ä´æÆ÷		
	TIM_SMCR_ETPConfig(TIM1,DISABLE); //etp=0    //Íâ²¿´¥·¢¼«ÐÔ  ²»·´Ïò
	TIM_SMCR_ETPSConfig(TIM1,TIM_SMCR_ETPS_1); //etps=10    //Íâ²¿´¥·¢Ô¤·ÖÆµ  4 
	TIM_SMCR_ETFConfig(TIM1,0x0000); //etf=0000    //Íâ²¿´¥·¢ÂË²¨  ÎÞ
	TIM_CCMR1_CC1sConfig(TIM1,TIM_CCMR1_CC1S_0|TIM_CCMR1_CC1S_1); //cc1s=11  //²¶»ñ/±È½Ï1Ñ¡Ôñ  cc1Í¨µÀ±»ÅäÖÃÎªÊäÈë£¬ic1Ó³Éäµ½trcÉÏ  //ccmr1 ÊäÈë/²¶»ñÄ£Ê½¼Ä´æÆ÷
	TIM_CCMR1_IC1PSCConfig(TIM1,0x00); //IC1PSC=00      //ÊäÈë/²¶»ñ1Ô¤·ÖÆµÆ÷   ÎÞ 	
	TIM_CCMR1_IC1FConfig(TIM1,0x0000); //ic1f=0000      //ÊäÈë²¶»ñ1ÂË²¨Æ÷  ÎÞ 
	TIM_SelectInputTrigger(TIM1,    //TS=0x001               //ÄÚ²¿´¥·¢1(ITR1)£¬TIM2
		TIM_SMCR_TS_0);	 		
	TIM_SMCR_SMSConfig(TIM1,TIM_SMCR_SMS_2); //sms=100    //Ñ¡ÖÐµÄ´¥·¢ÊäÈë(TRGI)µÄÉÏÉýÑØÖØÐÂ³õÊ¼»¯¼ÆÊýÆ÷£¬²¢²úÉúÒ»¸ö¸üÐÂ¼Ä´æÆ÷µÄÐÅºÅ	
	TIM_CCER_CC1Econfig(TIM1,ENABLE); //CC1e=1  //ÔÊÐí½«cntµÄÖµ²¶»ñÈçCCR1			
	TIM_ITConfig(  //TIMÖÐ¶ÏÊ¹ÄÜ
		TIM1, //TIM1
		TIM_IT_Update  |  //TIM ÖÐ¶ÏÔ´
		TIM_IT_Trigger |  //TIM ´¥·¢ÖÐ¶ÏÔ´ 
		TIM_IT_CC1,       //ÔÊÐí²¶»ñ/±È½Ï1ÖÐ
		ENABLE  //Ê¹ÄÜ
		);		
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;  //TIM1ÖÐ¶Ï  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //ÏÈÕ¼ÓÅÏÈ¼¶0¼¶
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //´ÓÓÅÏÈ¼¶3¼¶
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQÍ¨µÀ±»Ê¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);  //¸ù¾ÝNVIC_InitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷
	TIM_Cmd(TIM1, ENABLE);  //Ê¹ÄÜTIMxÍâÉè   Ê¹ÄÜ¼ÆÊýÆ÷		
}	
void Timer3_Init(void)
{
 	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //????	 
	//TIM_TimeBaseStructure.TIM_Period = 200; //ÉèÖÃÔÚÏÂÒ»¸ö¸üÐÂÊÂ¼þ×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ	 ¼ÆÊýµ½200Îª20ms
	TIM_TimeBaseStructure.TIM_Period = 9999; //ÉèÖÃÔÚÏÂÒ»¸ö¸üÐÂÊÂ¼þ×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ	 ¼ÆÊýµ½10000Îª1s
	//TIM_TimeBaseStructure.TIM_Period = 29999; //ÉèÖÃÔÚÏÂÒ»¸ö¸üÐÂÊÂ¼þ×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ	 ¼ÆÊýµ½30000Îª3s
	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //ÉèÖÃÓÃÀ´×÷ÎªTIMxÊ±ÖÓÆµÂÊ³ýÊýµÄÔ¤·ÖÆµÖµ  10KhzµÄ¼ÆÊýÆµÂÊ  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; ////ÉèÖÃÊ±ÖÓ·Ö¸î:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMÏòÉÏ¼ÆÊýÄ£Ê½
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //¸ù¾ÝTIM_TimeBaseInitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯TIMxµÄÊ±¼ä»ùÊýµ¥Î»	 
	TIM_InternalClockConfig( //SMCR  SMS=000 ???
		TIM3); //TIM2	 		
	TIM_SMCR_ECEConfig(TIM3,DISABLE); //smcr ece=0	   	
	TIM_SelectOutputTrigger( //¸üÐÂ--¸üÐÂÊÂ¼þÓÃÓÚ´¥·¢Êä³ö
		TIM3,
		TIM_CR2_MMS_1);		 
	TIM_ITConfig(  //Ê¹ÄÜ»òÕßÊ§ÄÜÖ¸¶¨µÄTIMÖÐ¶Ï  dier
		TIM3, //TIM2
		TIM_IT_Update  |  //TIM ÖÐ¶ÏÔ´
		TIM_IT_Trigger,   //TIM ´¥·¢ÖÐ¶ÏÔ´ 
		ENABLE  //Ê¹ÄÜ
		);	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM2ÖÐ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //ÏÈÕ¼ÓÅÏÈ¼¶0¼¶
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //ÏìÓ¦ÓÅÏÈ¼¶1¼¶
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQÍ¨µÀ±»Ê¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);  //¸ù¾ÝNVIC_InitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷	   
	TIM_Cmd(TIM3, ENABLE);  //Ê¹ÄÜTIMxÍâÉè	
}

static void TIM3_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM3 clock enable */
	//PCLK1¾­¹ý2±¶Æµºó×÷ÎªTIM3µÄÊ±ÖÓÔ´µÈÓÚ72MHz
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 

  /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // ¸´ÓÃÍÆÍìÊä³ö
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

	/* PWMÐÅºÅµçÆ½Ìø±äÖµ */
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
  TIM_TimeBaseStructure.TIM_Period = 999;       //µ±¶¨Ê±Æ÷´Ó0¼ÆÊýµ½999£¬¼´Îª1000´Î£¬ÎªÒ»¸ö¶¨Ê±ÖÜÆÚ
  TIM_TimeBaseStructure.TIM_Prescaler = 16;	    //ÉèÖÃÔ¤·ÖÆµ£º²»Ô¤·ÖÆµ£¬¼´Îª72MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4 ;	//ÉèÖÃÊ±ÖÓ·ÖÆµÏµÊý£º²»·ÖÆµ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //ÏòÉÏ¼ÆÊýÄ£Ê½

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //ÅäÖÃÎªPWMÄ£Ê½1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //ÉèÖÃÌø±äÖµ£¬µ±¼ÆÊýÆ÷¼ÆÊýµ½Õâ¸öÖµÊ±£¬µçÆ½·¢ÉúÌø±ä
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //µ±¶¨Ê±Æ÷¼ÆÊýÖµÐ¡ÓÚCCR1_ValÊ±Îª¸ßµçÆ½

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //Ê¹ÄÜÍ¨µÀ1

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //ÉèÖÃÍ¨µÀ2µÄµçÆ½Ìø±äÖµ£¬Êä³öÁíÍâÒ»¸öÕ¼¿Õ±ÈµÄPWM

  TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  //Ê¹ÄÜÍ¨µÀ2

  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//ÉèÖÃÍ¨µÀ3µÄµçÆ½Ìø±äÖµ£¬Êä³öÁíÍâÒ»¸öÕ¼¿Õ±ÈµÄPWM
//
//  TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //Ê¹ÄÜÍ¨µÀ3
//
//  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
//
//  /* PWM1 Mode configuration: Channel4 */
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	//ÉèÖÃÍ¨µÀ4µÄµçÆ½Ìø±äÖµ£¬Êä³öÁíÍâÒ»¸öÕ¼¿Õ±ÈµÄPWM
//
//  TIM_OC4Init(TIM3, &TIM_OCInitStructure);	//Ê¹ÄÜÍ¨µÀ4
//
//  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM3, ENABLE);			 // Ê¹ÄÜTIM3ÖØÔØ¼Ä´æÆ÷ARR

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);                   //Ê¹ÄÜ¶¨Ê±Æ÷3	
}

/*
 * º¯ÊýÃû£ºTIM3_PWM_Init
 * ÃèÊö  £ºTIM3 Êä³öPWMÐÅºÅ³õÊ¼»¯£¬Ö»Òªµ÷ÓÃÕâ¸öº¯Êý
 *         TIM3µÄËÄ¸öÍ¨µÀ¾Í»áÓÐPWMÐÅºÅÊä³ö
 * ÊäÈë  £ºÎÞ
 * Êä³ö  £ºÎÞ
 * µ÷ÓÃ  £ºÍâ²¿µ÷ÓÃ
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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //Ê±ÖÓÊ¹ÄÜ
	TIM_TimeBaseStructure.TIM_Period = TIM_Period; //ÉèÖÃÔÚÏÂÒ»¸ö¸üÐÂÊÂ¼þ×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ	 ¼ÆÊýµ½28Îª280us
	TIM_TimeBaseStructure.TIM_Prescaler =TIM_Prescaler; //ÉèÖÃÓÃÀ´×÷ÎªTIMxÊ±ÖÓÆµÂÊ³ýÊýµÄÔ¤·ÖÆµÖµ  100KhzµÄ¼ÆÊýÆµÂÊ  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //ÉèÖÃÊ±ÖÓ·Ö¸î:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMÏòÉÏ¼ÆÊýÄ£Ê½
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //¸ù¾ÝTIM_TimeBaseInitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯TIMxµÄÊ±¼ä»ùÊýµ¥Î»    
	TIM_ITConfig(  //Ê¹ÄÜ»òÕßÊ§ÄÜÖ¸¶¨µÄTIMÖÐ¶Ï
		TIM2, //TIM3
		TIM_IT_Update  |  //TIM ÖÐ¶ÏÔ´
		TIM_IT_Trigger,   //TIM ´¥·¢ÖÐ¶ÏÔ´ 
		ENABLE  //Ê¹ÄÜ
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3ÖÐ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //ÏÈÕ¼ÓÅÏÈ¼¶0¼¶
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //´ÓÓÅÏÈ¼¶3¼¶
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQÍ¨µÀ±»Ê¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);  //¸ù¾ÝNVIC_InitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷ 
	TIM_Cmd(TIM2, ENABLE);  //Ê¹ÄÜTIMxÍâÉè	
}
void Timer4_Init(void)
{
 	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //Ê±ÖÓÊ¹ÄÜ
	//TIM_TimeBaseStructure.TIM_Period = 2; //ÉèÖÃÔÚÏÂÒ»¸ö¸üÐÂÊÂ¼þ×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ	 ¼ÆÊýµ½200Îª20ms
	TIM_TimeBaseStructure.TIM_Period = 9; //ÉèÖÃÔÚÏÂÒ»¸ö¸üÐÂÊÂ¼þ×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ	 ¼ÆÊýµ½10Îª1ms
	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //ÉèÖÃÓÃÀ´×÷ÎªTIMxÊ±ÖÓÆµÂÊ³ýÊýµÄÔ¤·ÖÆµÖµ  10KhzµÄ¼ÆÊýÆµÂÊ  	
	//TIM_TimeBaseStructure.TIM_Prescaler =(7200-10); //ÉèÖÃÓÃÀ´×÷ÎªTIMxÊ±ÖÓÆµÂÊ³ýÊýµÄÔ¤·ÖÆµÖµ  10KhzµÄ¼ÆÊýÆµÂÊ  	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //ÉèÖÃÊ±ÖÓ·Ö¸î:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIMÏòÉÏ¼ÆÊýÄ£Ê½
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //¸ù¾ÝTIM_TimeBaseInitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯TIMxµÄÊ±¼ä»ùÊýµ¥Î» 
	TIM_ITConfig(  //Ê¹ÄÜ»òÕßÊ§ÄÜÖ¸¶¨µÄTIMÖÐ¶Ï
		TIM4, //TIM4
		TIM_IT_Update  |  //TIM ÖÐ¶ÏÔ´
		TIM_IT_Trigger,   //TIM ´¥·¢ÖÐ¶ÏÔ´ 
		ENABLE  //Ê¹ÄÜ
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4ÖÐ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //ÏÈÕ¼ÓÅÏÈ¼¶0¼¶
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //´ÓÓÅÏÈ¼¶3¼¶
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQÍ¨µÀ±»Ê¹ÄÜ
	NVIC_Init(&NVIC_InitStructure);  //¸ù¾ÝNVIC_InitStructÖÐÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯ÍâÉèNVIC¼Ä´æÆ÷
	TIM_Cmd(TIM4, ENABLE);  //Ê¹ÄÜTIMxÍâÉè	
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
	if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET) //¼ì²éÖ¸¶¨µÄTIMÖÐ¶Ï·¢ÉúÓë·ñ:TIM ÖÐ¶ÏÔ´    
	{
		TIM_Cmd(TIM4, DISABLE);  //Ê¹ÄÜTIMxÍâÉè
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1  );  //Çå³ýTIMxµÄÖÐ¶Ï´ý´¦ÀíÎ»:TIM ÖÐ¶Ï  TIM1->CCR1		
		Fre_Test=(u32) TIM_GetCapture1(TIM1);
		TIM1->CNT=0;
		TIM4->CNT=0;
		TIM_Cmd(TIM4,ENABLE);  //Ê¹ÄÜTIMxÍâÉè			
	}		
}


/////////////////////////////////////////////////////////////////////////////////
// Name:    void TIM2_IRQHandler(void)                                         //
// Para:    None																															 //	
// Return:  None																												       //
/////////////////////////////////////////////////////////////////////////////////
void TIM3_IRQHandler(void)   //TIM2ÖÐ¶Ï	 ÓÃÓÚ²úÉúÊ±¼ä´¥·¢T1¼ÆÊý
{	 
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //¼ì²éÖ¸¶¨µÄTIMÖÐ¶Ï·¢ÉúÓë·ñ:TIM ÖÐ¶ÏÔ´ 	  
	{		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //Çå³ýTIMxµÄÖÐ¶Ï´ý´¦ÀíÎ»:TIM ÖÐ¶ÏÔ
 					
	}		
}



/////////////////////////////////////////////////////////////////////////////////
// Name:    void TIM3_IRQHandler(void)                                         //
// Para:    None																															 //	
// Return:  None																												       //
/////////////////////////////////////////////////////////////////////////////////
void TIM2_IRQHandler(void)   //TIM3ÖÐ¶Ï	ÓÃÓÚADCÑÓÊ±²ÉÑù
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //¼ì²éÖ¸¶¨µÄTIMÖÐ¶Ï·¢ÉúÓë·ñ:TIM ÖÐ¶ÏÔ´ 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //Çå³ýTIMxµÄÖÐ¶Ï´ý´¦ÀíÎ»:TIM ÖÐ¶ÏÔ´ 
		TIM_Cmd(TIM2, DISABLE);  //½ûÖ¹TIMxÍâÉè	
		//ADC_Cmd(ADC1, ENABLE);			   			
		/* ÓÉÓÚÃ»ÓÐ²ÉÓÃÍâ²¿´¥·¢£¬ËùÒÔÊ¹ÓÃÈí¼þ´¥·¢ADC×ª»» */ 
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
			//Timer3_Init(4,71999);   //ÉèÖÃÔÚÏÂÒ»¸ö¸üÐÂÊÂ¼þ×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ	 ¼ÆÊýµ½4Îª40us
			//ADC_Cmd(ADC1, ENABLE);	 //Ê¹ÄÜTIMxÍâÉè	
			//PM25_LED=1;
		//}
		//else if(Timer_40us_Flag)
		//{
		   //PM25_LED=0;
		   //PM25_LED=~PM25_LED;
		   //Timer_40us_Flag=1;
		   //Timer3_Init(28,71999); //ÉèÖÃÔÚÏÂÒ»¸ö¸üÐÂÊÂ¼þ×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ	 ¼ÆÊýµ½28Îª280us
		   	//Timer3_Init(40,71999);   //ÉèÖÃÔÚÏÂÒ»¸ö¸üÐÂÊÂ¼þ×°Èë»î¶¯µÄ×Ô¶¯ÖØ×°ÔØ¼Ä´æÆ÷ÖÜÆÚµÄÖµ	 ¼ÆÊýµ½4Îª40us
		//	ADC_Cmd(ADC1, ENABLE);
		//}
	}
}


/////////////////////////////////////////////////////////////////////////////////
// Name:    void TIM4_IRQHandler(void)                                         //
// Para:    None																															 //	
// Return:  None																												       //
/////////////////////////////////////////////////////////////////////////////////

void TIM4_IRQHandler(void)   //TIM4ÖÐ¶Ï  ×÷ÎªÏµÍ³ÔËÐÐÊ±ÖÓ
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //¼ì²éÖ¸¶¨µÄTIMÖÐ¶Ï·¢ÉúÓë·ñ:TIM ÖÐ¶ÏÔ´ 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //Çå³ýTIMxµÄÖÐ¶Ï´ý´¦ÀíÎ»:TIM ÖÐ¶ÏÔ
		Sys_Counter++;		//ÏµÍ³¼ÆÊýÊ±ÖÓ
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
void TIM_SMCR_ETPSConfig(TIM_TypeDef* TIMx,uint16_t TIM_Etr_psc) //Íâ²¿´¥·¢Ô¤·ÖÆµ
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
void	TIM_SMCR_ETFConfig(TIM_TypeDef* TIMx,uint16_t TIM_Etr_filter) //Íâ²¿´¥·¢ÂË²¨
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
void	TIM_CCMR1_CC1sConfig(TIM_TypeDef* TIMx,uint16_t TIM_cap_comp) //²¶»ñ/±È½Ï 1Ñ¡Ôñ
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
void	TIM_CCMR1_IC1PSCConfig(TIM_TypeDef* TIMx,uint16_t TIM_cap_psc) //ÊäÈë²¶»ñ1Ô¤·ÖÆµÆ÷
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

void TIM_CCMR1_IC1FConfig(TIM_TypeDef* TIMx,uint16_t TIM_cap_filter) //ÊäÈë²¶»ñ1ÂË²¨Æ÷
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







