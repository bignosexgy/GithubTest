#include "stm32f10x.h"
#include "ht1622.h"
#include "sys.h"
#include "delay.h"


////////////////////////////lcd display/////////////////////////////////////////////////////
//i=0--(前6个地址有效)屏右下脚6个;1---(后5个地址有效)右上角5个;2--蓝色扇叶和左侧下面右侧第一个的8;3--扇叶圆点和右侧第二个8;
//4---冒号和第三个8;5---绿色扇叶和第四个8;6---黄色扇叶和5个8;7---网络标号和6个8;8---0的右侧和7个8;9----0的左侧和8个8;
//10----20和9个8;11---runtimer min和左下属第二行右侧第1个8；12---2个8；13---3个8；14--4个8；15--5个8；16---vacuum ba和第三行右侧第1个8；
//17--2个8；18---3个8；19--最上一层的N和4个8；20---1和5个8；21--coldtrap C 和四层右第1个8；22--scientz-和2个8；23---  -和3个8；
//24---sample C和第五行右第1个8；25--logo和2个8；26---  -和3个8；27--第六行18；28到31没有相关动作


void HT1622_Init(void)
{ 
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	                        //使能PA口时钟	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;		//PA.3,PA.4,PA.5,PA.6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		                        //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                               //
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                          //配置参数
    GPIO_SetBits(GPIOA,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6);						                            //初始化LCD_CS--PA.3片选失效 

    delay_init(72);	    	//延时函数初始化	
    
    SendCmd(0x0);     //关闭系统振荡器和LCD驱动
    delay_us(5);
    //SendCmd(0x52);     //关闭系统振荡器和LCD驱动
    //delay_us(5);
    SendCmd(0x02);     //打开系统振荡器
    delay_us(5);
    SendCmd(0x04);     //关闭LCD显示
    delay_us(5);
    SendCmd(0x06);     //打开LCD显示
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
