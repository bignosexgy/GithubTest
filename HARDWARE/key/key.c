#include "key.h"
//#include "basic.h"
#include "gpio.h"
#include "basic.h"
#include "delay.h"



//���尴��ֵ
//extern u8	KeyValue;
//extern u8  KeyNum;         //����ֵ
//extern u8  KeyPressed;     //������ѹ��ʶ	
//u8 key_down=0;
//extern u8 Beep_Flag;
//extern u8 Beep_Counter;
//extern u32 Runtime;	   
//extern u8 Key_Press_Counter;
//extern u8 Fan_Flag;
extern u8 Coldtrap_Flash;
extern u8 Model_Flash;	  
extern u8 USART_TX_Compressor_Flag;
//extern u8 USART_TX_Vacuumpump_Flag;
//extern u8 USART_TX_Vacuometer_Flag;


 /*

 * �������� Delay
 * ����  ������ȷ����ʱ
 * ����  ����ʱ������
 * ���  ����
 */
 void Delay(__IO u32 nCount)
{
	for(; nCount != 0; nCount--);
}


/******************************************************************************/
// Name:void void KeyScan(void)
// Para:    None
// Return:  None
/******************************************************************************/
/**/
//void key_process(void)   //�����͹ܽŲ���Ӧ  key_process
//{                             //���ò���
//	static u8 KeyNum_Temp1;
//	static u8 KeyNum_Counter1,KeyNum_Counter2;
//	u8 KeyNum_Temp2=0;
//	static u8 KeyNum_Temp3;
//	KeyNum=KeyNum_Temp1;
//
//	KeyNum_Temp2=(u8)(GPIO_ReadInputData(GPIOB) >> 10);
//	KeyNum_Temp2 ^= 0x3f;
//	KeyNum_Temp2 &= 0x3f;
//	KeyNum_Counter1++;
//	if(KeyNum_Counter1==1)
//			KeyNum_Temp3=KeyNum_Temp2;
//	if(KeyNum_Temp3==KeyNum_Temp2)
//			KeyNum_Counter2++;					 
//	else
//	{	
//			KeyNum_Counter2=0;
//			KeyNum_Counter1=0;
//	}	
//	if(KeyNum_Counter2 > 1)
//	{
//		KeyNum_Counter2=0;
//		KeyNum_Counter1=0;
//		KeyNum_Temp1=KeyNum_Temp3;
//	}	
//	switch(KeyNum)     //
//	{
//		case 0:    //no key is press    		
//			
//			break;
//		case 8:    //compressor  A
//		
//			break;
//		default:
//			break;    	
//	}
//		
//	
//	
//}

/*
 * ��������Key_Scan
 * ����  ������Ƿ��а�������
 * ����  ��GPIOx��x ������ A��B��C��D���� E
 *		     GPIO_Pin������ȡ�Ķ˿�λ 	
 * ���  ��KEY_OFF(û���°���)��KEY_ON�����°�����
 */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,u16 GPIO_Pin)
{			
	/*����Ƿ��а������� */
//	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON ) 
//	{	   
		/*��ʱ����*/
	//	Delay(100);		
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
		{	 
			/*�ȴ������ͷ� */
		//	while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON);   
			return 	0;	 
		}
		else
			return 1;
//	}
//	else
//		return KEY_OFF;
}
