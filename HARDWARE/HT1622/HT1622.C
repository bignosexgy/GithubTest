#include "stm32f10x.h"
#include "ht1622.h"
#include "sys.h"
#include "delay.h"


////////////////////////////lcd display/////////////////////////////////////////////////////
//i=0--(ǰ6����ַ��Ч)�����½�6��;1---(��5����ַ��Ч)���Ͻ�5��;2--��ɫ��Ҷ����������Ҳ��һ����8;3--��ҶԲ����Ҳ�ڶ���8;
//4---ð�ź͵�����8;5---��ɫ��Ҷ�͵��ĸ�8;6---��ɫ��Ҷ��5��8;7---�����ź�6��8;8---0���Ҳ��7��8;9----0������8��8;
//10----20��9��8;11---runtimer min���������ڶ����Ҳ��1��8��12---2��8��13---3��8��14--4��8��15--5��8��16---vacuum ba�͵������Ҳ��1��8��
//17--2��8��18---3��8��19--����һ���N��4��8��20---1��5��8��21--coldtrap C ���Ĳ��ҵ�1��8��22--scientz-��2��8��23---  -��3��8��
//24---sample C�͵������ҵ�1��8��25--logo��2��8��26---  -��3��8��27--������18��28��31û����ض���


void HT1622_Init(void)
{ 
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	                        //ʹ��PA��ʱ��	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;		//PA.3,PA.4,PA.5,PA.6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		                        //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                               //
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                          //���ò���
    GPIO_SetBits(GPIOA,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);						                            //��ʼ��LCD_CS--PA.3ƬѡʧЧ 

    delay_init(72);	    	//��ʱ������ʼ��	
    
    SendCmd(0x0);     //�ر�ϵͳ������LCD����
    delay_us(5);
    //SendCmd(0x52);     //�ر�ϵͳ������LCD����
    //delay_us(5);
    SendCmd(0x02);     //��ϵͳ����
    delay_us(5);
    SendCmd(0x04);     //�ر�LCD��ʾ
    delay_us(5);
    SendCmd(0x06);     //��LCD��ʾ
    delay_us(5);
    
}

void SendBit_1622(u8 data,u8 cnt)	
{
	u8 i;
	for(i =0; i <cnt; i ++)
	{
		if((data&0x80)==0) LCD_DA=0;
		else LCD_DA=1;
		LCD_WR=0;
		delay_us(5);
		LCD_WR=1;
		data<<=1;
        delay_us(5);
	}
}

void SendDataBit_1622(u8 data,u8 cnt)	
{
	u8 i;
	for(i = 0; i < cnt; i ++)
	{
		if((data&0x01)==0) LCD_DA=0;
		else LCD_DA=1;
		LCD_WR=0;
		delay_us(5);;
		LCD_WR=1;
		data>>=1;
    delay_us(5);
	}
}

void SendCmd(u8 command)
{
	LCD_CS=0;
    delay_us(5);
	SendBit_1622(0x80,4);			
	SendBit_1622(command,8); 	
    delay_us(5);	
	LCD_CS=1;						
}

void Write_1622(u8 addr,u8 data)
{
	LCD_CS=0;
  delay_us(5);
	SendBit_1622(0xa0,3);			
	SendBit_1622(addr,6);		
  SendDataBit_1622(data,8);	
  delay_us(5);	
	LCD_CS=1;
}

void WriteAll_1622(u8 addr,u8 data)
{
	u8 i;
  LCD_CS=0;
  delay_us(5);
	SendBit_1622(0xa0,3);			
	SendBit_1622(addr,6);			
	for(i =0; i <32; i ++)		
	{
		SendDataBit_1622(data,8);
	}
    delay_us(5);
	LCD_CS=1;
}

void Display_RTCtime(u8 data[9])
{
	u8 i;
  LCD_CS=0;
  delay_us(5);
	SendBit_1622(0xa0,3);			
	SendBit_1622(16,6);			
	for(i =0; i <9; i ++)		
	{
		SendDataBit_1622(data[i],8);
	}
    delay_us(5);
	LCD_CS=1;
}	


void disp_date(u8 data[32],u8 addr_start,u8 count)
{
	u8 i;
	LCD_CS=0;
  delay_us(5);
	SendBit_1622(0xa0,3);			
	SendBit_1622(addr_start*8,6);	
	for(i=0;i<count;i++)		
		SendDataBit_1622(data[i+addr_start],8);	 
	delay_us(5);
	LCD_CS=1;
}