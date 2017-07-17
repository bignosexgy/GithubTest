#include "timer.h"
//#include "basic.h"
#include "gpio.h"
//#include "task.h" 

//extern u8 PubFlagSecond;
//u8 PubFlagSecond;
//void DisplayRefresh(void);
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!

void TIM_SMCR_ECEConfig(TIM_TypeDef* TIMx, FunctionalState NewState);  //TIM_ETRClockMode2Config   xgy20140417

void TIM_SMCR_SMSConfig(TIM_TypeDef* TIMx, uint16_t TIM_slave_select); //sms

void TIM_SMCR_ETPConfig(TIM_TypeDef* TIMx,FunctionalState NewState); //ece

void TIM_SMCR_ETPSConfig(TIM_TypeDef* TIMx,uint16_t TIM_Etr_psc); //�ⲿ����Ԥ��Ƶ

void	TIM_SMCR_ETFConfig(TIM_TypeDef* TIMx,uint16_t TIM_Etr_filter); //�ⲿ�����˲�

void	TIM_CCMR1_CC1sConfig(TIM_TypeDef* TIMx,uint16_t TIM_cap_comp); //����/�Ƚ� 1ѡ��

void	TIM_CCMR1_IC1PSCConfig(TIM_TypeDef* TIMx,uint16_t TIM_cap_psc); //���벶��1Ԥ��Ƶ��

void TIM_CCMR1_IC1FConfig(TIM_TypeDef* TIMx,uint16_t TIM_cap_filter); //���벶��1�˲���

void TIM_CCER_CC1Econfig(TIM_TypeDef* TIMx, FunctionalState NewState); //��������ֵ����CCR1

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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //ʱ��ʹ��
	TIM_TimeBaseStructure.TIM_Period = 0xffff; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������200Ϊ20ms
	//TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_Prescaler =0; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ 	
	TIM_SMCR_ECEConfig(TIM1,ENABLE); //ece=1  //ext clock eanble  //smcr��ģʽ���ƼĴ���		
	TIM_SMCR_ETPConfig(TIM1,DISABLE); //etp=0    //�ⲿ��������  ������
	TIM_SMCR_ETPSConfig(TIM1,TIM_SMCR_ETPS_1); //etps=10    //�ⲿ����Ԥ��Ƶ  4 
	TIM_SMCR_ETFConfig(TIM1,0x0000); //etf=0000    //�ⲿ�����˲�  ��
	TIM_CCMR1_CC1sConfig(TIM1,TIM_CCMR1_CC1S_0|TIM_CCMR1_CC1S_1); //cc1s=11  //����/�Ƚ�1ѡ��  cc1ͨ��������Ϊ���룬ic1ӳ�䵽trc��  //ccmr1 ����/����ģʽ�Ĵ���
	TIM_CCMR1_IC1PSCConfig(TIM1,0x00); //IC1PSC=00      //����/����1Ԥ��Ƶ��   �� 	
	TIM_CCMR1_IC1FConfig(TIM1,0x0000); //ic1f=0000      //���벶��1�˲���  �� 
	TIM_SelectInputTrigger(TIM1,    //TS=0x001               //�ڲ�����1(ITR1)��TIM2
		TIM_SMCR_TS_0);	 		
	TIM_SMCR_SMSConfig(TIM1,TIM_SMCR_SMS_2); //sms=100    //ѡ�еĴ�������(TRGI)�����������³�ʼ����������������һ�����¼Ĵ������ź�	
	TIM_CCER_CC1Econfig(TIM1,ENABLE); //CC1e=1  //����cnt��ֵ������CCR1			
	TIM_ITConfig(  //TIM�ж�ʹ��
		TIM1, //TIM1
		TIM_IT_Update  |  //TIM �ж�Դ
		TIM_IT_Trigger |  //TIM �����ж�Դ 
		TIM_IT_CC1,       //������/�Ƚ�1��
		ENABLE  //ʹ��
		);		
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;  //TIM1�ж�  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIMx����   ʹ�ܼ�����		
}	
void Timer3_Init(void)
{
 	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //????	 
	//TIM_TimeBaseStructure.TIM_Period = 200; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������200Ϊ20ms
	TIM_TimeBaseStructure.TIM_Period = 9999; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������10000Ϊ1s
	//TIM_TimeBaseStructure.TIM_Period = 29999; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������30000Ϊ3s
	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; ////����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ	 
	TIM_InternalClockConfig( //SMCR  SMS=000 ???
		TIM3); //TIM2	 		
	TIM_SMCR_ECEConfig(TIM3,DISABLE); //smcr ece=0	   	
	TIM_SelectOutputTrigger( //����--�����¼����ڴ������
		TIM3,
		TIM_CR2_MMS_1);		 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�  dier
		TIM3, //TIM2
		TIM_IT_Update  |  //TIM �ж�Դ
		TIM_IT_Trigger,   //TIM �����ж�Դ 
		ENABLE  //ʹ��
		);	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //��Ӧ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���	   
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����	
}

static void TIM3_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	/* TIM3 clock enable */
	//PCLK1����2��Ƶ����ΪTIM3��ʱ��Դ����72MHz
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

  /* GPIOA and GPIOB clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 

  /*GPIOA Configuration: TIM3 channel 1 and 2 as alternate function push-pull */
//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
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

	/* PWM�źŵ�ƽ����ֵ */
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
  TIM_TimeBaseStructure.TIM_Period = 999;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
  TIM_TimeBaseStructure.TIM_Prescaler = 16;	    //����Ԥ��Ƶ����Ԥ��Ƶ����Ϊ72MHz
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV4 ;	//����ʱ�ӷ�Ƶϵ��������Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ

  TIM_OC1Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��1

  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;	  //����ͨ��2�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM

  TIM_OC2Init(TIM3, &TIM_OCInitStructure);	  //ʹ��ͨ��2

  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	//����ͨ��3�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
//
//  TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��3
//
//  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
//
//  /* PWM1 Mode configuration: Channel4 */
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;	//����ͨ��4�ĵ�ƽ����ֵ���������һ��ռ�ձȵ�PWM
//
//  TIM_OC4Init(TIM3, &TIM_OCInitStructure);	//ʹ��ͨ��4
//
//  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM3, ENABLE);			 // ʹ��TIM3���ؼĴ���ARR

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);                   //ʹ�ܶ�ʱ��3	
}

/*
 * ��������TIM3_PWM_Init
 * ����  ��TIM3 ���PWM�źų�ʼ����ֻҪ�����������
 *         TIM3���ĸ�ͨ���ͻ���PWM�ź����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	TIM_TimeBaseStructure.TIM_Period = TIM_Period; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������28Ϊ280us
	TIM_TimeBaseStructure.TIM_Prescaler =TIM_Prescaler; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  100Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ    
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM2, //TIM3
		TIM_IT_Update  |  //TIM �ж�Դ
		TIM_IT_Trigger,   //TIM �����ж�Դ 
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����	
}
void Timer4_Init(void)
{
 	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	//TIM_TimeBaseStructure.TIM_Period = 2; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������200Ϊ20ms
	TIM_TimeBaseStructure.TIM_Period = 9; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������10Ϊ1ms
	TIM_TimeBaseStructure.TIM_Prescaler =(7200-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  	
	//TIM_TimeBaseStructure.TIM_Prescaler =(7200-10); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM4, //TIM4
		TIM_IT_Update  |  //TIM �ж�Դ
		TIM_IT_Trigger,   //TIM �����ж�Դ 
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx����	
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
	if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ    
	{
		TIM_Cmd(TIM4, DISABLE);  //ʹ��TIMx����
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1  );  //���TIMx���жϴ�����λ:TIM �ж�  TIM1->CCR1		
		Fre_Test=(u32) TIM_GetCapture1(TIM1);
		TIM1->CNT=0;
		TIM4->CNT=0;
		TIM_Cmd(TIM4,ENABLE);  //ʹ��TIMx����			
	}		
}


/////////////////////////////////////////////////////////////////////////////////
// Name:    void TIM2_IRQHandler(void)                                         //
// Para:    None																															 //	
// Return:  None																												       //
/////////////////////////////////////////////////////////////////////////////////
void TIM3_IRQHandler(void)   //TIM2�ж�	 ���ڲ���ʱ�䴥��T1����
{	 
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 	  
	{		
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж��
 					
	}		
}



/////////////////////////////////////////////////////////////////////////////////
// Name:    void TIM3_IRQHandler(void)                                         //
// Para:    None																															 //	
// Return:  None																												       //
/////////////////////////////////////////////////////////////////////////////////
void TIM2_IRQHandler(void)   //TIM3�ж�	����ADC��ʱ����
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		TIM_Cmd(TIM2, DISABLE);  //��ֹTIMx����	
		//ADC_Cmd(ADC1, ENABLE);			   			
		/* ����û�в����ⲿ����������ʹ���������ADCת�� */ 
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
			//Timer3_Init(4,71999);   //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������4Ϊ40us
			//ADC_Cmd(ADC1, ENABLE);	 //ʹ��TIMx����	
			//PM25_LED=1;
		//}
		//else if(Timer_40us_Flag)
		//{
		   //PM25_LED=0;
		   //PM25_LED=~PM25_LED;
		   //Timer_40us_Flag=1;
		   //Timer3_Init(28,71999); //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������28Ϊ280us
		   	//Timer3_Init(40,71999);   //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������4Ϊ40us
		//	ADC_Cmd(ADC1, ENABLE);
		//}
	}
}


/////////////////////////////////////////////////////////////////////////////////
// Name:    void TIM4_IRQHandler(void)                                         //
// Para:    None																															 //	
// Return:  None																												       //
/////////////////////////////////////////////////////////////////////////////////

void TIM4_IRQHandler(void)   //TIM4�ж�  ��Ϊϵͳ����ʱ��
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж��
		Sys_Counter++;		//ϵͳ����ʱ��
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
void TIM_SMCR_ETPSConfig(TIM_TypeDef* TIMx,uint16_t TIM_Etr_psc) //�ⲿ����Ԥ��Ƶ
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
void	TIM_SMCR_ETFConfig(TIM_TypeDef* TIMx,uint16_t TIM_Etr_filter) //�ⲿ�����˲�
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
void	TIM_CCMR1_CC1sConfig(TIM_TypeDef* TIMx,uint16_t TIM_cap_comp) //����/�Ƚ� 1ѡ��
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
void	TIM_CCMR1_IC1PSCConfig(TIM_TypeDef* TIMx,uint16_t TIM_cap_psc) //���벶��1Ԥ��Ƶ��
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

void TIM_CCMR1_IC1FConfig(TIM_TypeDef* TIMx,uint16_t TIM_cap_filter) //���벶��1�˲���
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







