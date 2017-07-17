#include "spi.h"
#include "gpio.h"
#include "usart.h"
//Mini STM32开发板
//SPI 驱动 V1.2
//正点原子@ALIENTEK
//2010/6/13	
extern u16 PM25_Value_1;
extern  float VOC_Data;
extern int16_t Temperature;
extern u16 Humidity	;
extern	u16 CO2_Data;
extern	u16 CO_Data;
//extern static char *itoa(int value, char *string, int radix);
//以下是SPI模块的初始化代码，配置成主机模式，访问SD Card/W25X16/24L01/JF24C	
//static char *itoa_1(int value, char *string, int radix)
//{
//	int     i, d;
//	int     flag = 0;
//	char    *ptr = string;
//	
//	// This implementation only works for decimal numbers. //
//	if (radix != 10)
//	{
//	    *ptr = 0;
//	    return string;
//	}
//	
//	if (!value)
//	{
//	    *ptr++ = 0x30;
//	    *ptr = 0;
//	    return string;	 
//
//	}
//	
//	// if this is a negative value insert the minus sign. //
//	if (value < 0)
//	{
//	    *ptr++ = '-';
//	
//	    // Make the value positive. //
//	    value *= -1;
//	}
//	
//	for (i = 10000; i > 0; i /= 10)
//	{
//	    d = value / i;
//	
//	    if (d || flag)
//	    {
//	        *ptr++ = (char)(d + 0x30);
//	        value -= (d * i);
//	        flag = 1;
//	    }
//	}
//	
//	// Null terminate the string. //
//	*ptr = 0;
//	
//	return string;
//
//} 


						  
//SPI口初始化
//这里针是对SPI1的初始化
void SPIx_Init(void)
{	 
	RCC->APB2ENR|=1<<2;       //PORTA时钟使能 	 
	RCC->APB2ENR|=1<<12;      //SPI1时钟使能 
		   
	//这里只针对SPI口初始化
	GPIOA->CRL&=0X000FFFFF; 
	GPIOA->CRL|=0XBBB00000;//PA5.6.7复用 	    
	GPIOA->ODR|=0X7<<5;    //PA5.6.7上拉
		
	SPI1->CR1|=0<<10;//全双工模式	
	SPI1->CR1|=1<<9; //软件nss管理
	SPI1->CR1|=1<<8;  

	SPI1->CR1|=1<<2; //SPI主机
	SPI1->CR1|=0<<11;//8bit数据格式	
	SPI1->CR1|=1<<1; //空闲模式下SCK为1 CPOL=1
	SPI1->CR1|=1<<0; //数据采样从第二个时间边沿开始,CPHA=1  
	SPI1->CR1|=7<<3; //Fsck=Fcpu/256
	SPI1->CR1|=0<<7; //MSBfirst   
	SPI1->CR1|=1<<6; //SPI设备使能
	SPIx_ReadWriteByte(0xff);//启动传输		 
}   
//SPI 速度设置函数
//SpeedSet:
//SPI_SPEED_2   2分频   (SPI 36M@sys 72M)
//SPI_SPEED_8   8分频   (SPI 9M@sys 72M)
//SPI_SPEED_16  16分频  (SPI 4.5M@sys 72M)
//SPI_SPEED_256 256分频 (SPI 281.25K@sys 72M)
void SPIx_SetSpeed(u8 SpeedSet)
{
	SPI1->CR1&=0XFFC7;//Fsck=Fcpu/256
	if(SpeedSet==SPI_SPEED_2)//二分频
	{
		SPI1->CR1|=0<<3;//Fsck=Fpclk/2=36Mhz	
	}else if(SpeedSet==SPI_SPEED_8)//八分频 
	{
		SPI1->CR1|=2<<3;//Fsck=Fpclk/8=9Mhz	
	}else if(SpeedSet==SPI_SPEED_16)//十六分频
	{
		SPI1->CR1|=3<<3;//Fsck=Fpclk/16=4.5Mhz
	}else			 	 //256分频
	{
		SPI1->CR1|=7<<3; //Fsck=Fpclk/256=281.25Khz 低速模式
	}
	SPI1->CR1|=1<<6; //SPI设备使能	  
} 
//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPIx_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 
	while((SPI1->SR&1<<1)==0)//等待发送区空	
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI1->DR=TxData;	 	  //发送一个byte 
	retry=0;
	while((SPI1->SR&1<<0)==0) //等待接收完一个byte  
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI1->DR;          //返回收到的数据				    
}
void SPI1_SetSpeed(u8 SpeedSet)
{
	SpeedSet&=0X07;			//限制范围
	SPI1->CR1&=0XFFC7; 
	SPI1->CR1|=SpeedSet<<3;	//设置SPI1速度  
	SPI1->CR1|=1<<6; 		//SPI设备使能	   
}

u8 SPI1_ReadWriteByte(u8 TxData)
{				   			 
	while((SPI1->SR&1<<1)==0);//等待发送区空  
	SPI1->DR=TxData;	 	  //发送一个byte 	   
	while((SPI1->SR&1<<0)==0); //等待接收完一个byte   			    
	return SPI1->DR;          //返回收到的数据				    
}

////=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
////    HJ12864-OLED-1  
////	  www.huijinglcm.com
////    Dot Matrix: 128*64
////    Driver IC : SSD1309 (Solomon Systech)
////    Interface : 8-bit 68XX/80XX Parallel, 4-wire Serial
////    Revision  :
////    Date      :    2015/01/14
////    Author    :
////    Editor    : 
////=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
////#include <at89x51.h>
////
////#define	SPI				// 4-wire Serial
//						//   BS1=0; BS2=0
//						//   The unused pins should be connected with VSS mostly or floating (D2).
//						//   Please refer to the SSD1309 specification for detail.
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
////  Pin Definition
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
////#define xData	P1				// Parallel Data Input/Output
////
////#define SCLK	P1_0			// Serial Clock Input
////#define SDIN	P1_1			// Serial Data Input
////
////#define RES	P2_2				// Reset
////#define CS	P2_0				// Chip Select
////#define DC	P2_1				// Data/Command Control
////
////#define E	P2_4				// Read/Write Enable
////#define RW	P2_3				// Read/Write Select
////
////#define RD	P2_4				// Read Signal
////#define WR	P2_3				// Write Signal
////
////sbit Rom_IN=P1^6; 
////sbit Rom_OUT=P1^7; 
////sbit Rom_SCK=P1^5; 
////sbit Rom_CS=P1^4; 
//
////#define xData	P1				// Parallel Data Input/Output
//
////#define SCLK	P1_0			// Serial Clock Input
////#define SDIN	P1_1			// Serial Data Input
//
////#define RES_oled	PBout(5)				// Reset
////#define CS_oled	 PBout(5)				// Chip Select
////#define DC_oled	 PBout(5)				// Data/Command Control
////
//////#define E	P2_4				// Read/Write Enable
//////#define RW	P2_3				// Read/Write Select
//////
//////#define RD	P2_4				// Read Signal
//////#define WR	P2_3				// Write Signal
////
////#define Rom_IN  PBout; 
////#define Rom_OUT  PBout; 
////#define Rom_SCK   PBout; 
////#define Rom_CS   PBout; 
//unsigned char m1,z,z1,d,d1,s,s1,s10,s100;
//unsigned long fontaddr=0;
////unsigned char  hui1[100]={/*--  文字:  晶  --*/
///////*--  黑体12;  此字体下对应的点阵为：宽x高=16x16   --*/
////0x00,0x00,0x00,0x7C,0x7C,0x54,0x54,0x54,0x54,0x54,0x7C,0x7C,0x7C,0x00,0x00,0x00,
////0x00,0x7F,0x7F,0x29,0x29,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x29,0x29,0x7F,0x7F,0x00,
//// };
////unsigned char  jing1[100] ;
////unsigned char  Huijing[100][48] ;
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
////void get_and_write_16x16(unsigned long fontaddr,unsigned char page,unsigned char column);
//static unsigned char get_data_from_ROM( );
//
////  Delay Time
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//void uDelay(unsigned char l)
//{
//	while(l--);
//}
//
//
////void Delay(unsigned char n)
////{
////unsigned char i,j,k;
////
////	for(k=0;k<n;k++)
////	{
////		for(i=0;i<40;i++)
////		{
////			for(j=0;j<15;j++)
////			{
////				uDelay(203);	
////			}
////		}
////	}
////}
//
//
////  Read/Write Sequence
//
////#ifdef SPI					// 4-wire Serial
//void Write_Command(unsigned char Data)
//{
//unsigned char i;
//
//	CS_oled=0;
//	DC_oled=0;
//	for (i=0; i<8; i++)
//	{
//		SCLK=0;
//		SDIN=(Data&0x80)>>7;
//		Data = Data << 1;
////		uDelay(1);
//		SCLK=1;
////		uDelay(1);
//	}
////	SCLK=0;
//	DC_oled=1;
//	CS_oled=1;
//}
//
//
//void Write_Data(unsigned char Data)
//{
//unsigned char i;
//
//	CS_oled=0;
//	DC_oled=1;
//	for (i=0; i<8; i++)
//	{
//		SCLK=0;
//		SDIN=(Data&0x80)>>7;
//		Data = Data << 1;
////		uDelay(1);
//		SCLK=1;
////		uDelay(1);
//	}
////	SCLK=0;
//	DC_oled=1;
//	CS_oled=1;
//}
////#endif
//
//
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
////  Instruction Setting
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//void Set_Start_Column(unsigned char d)
//{
//	Write_Command(0x00+d%16);		// Set Lower Column Start Address for Page Addressing Mode
//						//   Default => 0x00
//	Write_Command(0x10+d/16);		// Set Higher Column Start Address for Page Addressing Mode
//						//   Default => 0x10
//}
//
//
//void Set_Addressing_Mode(unsigned char d)
//{
//	Write_Command(0x20);			// Set Memory Addressing Mode
//	Write_Command(d);			//   Default => 0x02
//						//     0x00 => Horizontal Addressing Mode
//						//     0x01 => Vertical Addressing Mode
//						//     0x02 => Page Addressing Mode
//}
//
//
//void Set_Start_Line(unsigned char d)
//{
//	Write_Command(0x40|d);			// Set Display Start Line
//						//   Default => 0x40 (0x00)
//}
//
//
//void Set_Contrast_Control(unsigned char d)
//{
//	Write_Command(0x81);			// Set Contrast Control for Bank 0
//	Write_Command(d);			//   Default => 0x7F
//}
//
//
//void Set_Segment_Remap(unsigned char d)
//{
//	Write_Command(d);			// Set Segment Re-Map
//						//   Default => 0xA0
//						//     0xA0 => Column Address 0 Mapped to SEG0
//						//     0xA1 => Column Address 0 Mapped to SEG127
//}
//
//
//void Set_Entire_Display(unsigned char d)
//{
//	Write_Command(d);			// Set Entire Display On / Off
//						//   Default => 0xA4
//						//     0xA4 => Normal Display
//						//     0xA5 => Entire Display On
//}
//
//
//void Set_Inverse_Display(unsigned char d)
//{
//	Write_Command(d);			// Set Inverse Display On/Off
//						//   Default => 0xA6
//						//     0xA6 => Normal Display
//						//     0xA7 => Inverse Display On
//}
//
//
//void Set_Multiplex_Ratio(unsigned char d)
//{
//	Write_Command(0xA8);			// Set Multiplex Ratio
//	Write_Command(d);			//   Default => 0x3F (1/64 Duty)
//}
//
//
//void Set_Display_On_Off(unsigned char d)	
//{
//	Write_Command(d);			// Set Display On/Off
//						//   Default => 0xAE
//						//     0xAE => Display Off
//						//     0xAF => Display On
//}
//
//
//void Set_Start_Page(unsigned char d)
//{
//	Write_Command(0xB0|d);			// Set Page Start Address for Page Addressing Mode
//						//   Default => 0xB0 (0x00)
//}
//
//
//void Set_Common_Remap(unsigned char d)
//{
//	Write_Command(d);			// Set COM Output Scan Direction
//						//   Default => 0xC0
//						//     0xC0 => Scan from COM0 to 63
//						//     0xC8 => Scan from COM63 to 0
//}
//
//
//void Set_Display_Offset(unsigned char d)
//{
//	Write_Command(0xD3);			// Set Display Offset
//	Write_Command(d);			//   Default => 0x00
//}
//
//
//void Set_Display_Clock(unsigned char d)
//{
//	Write_Command(0xD5);			// Set Display Clock Divide Ratio / Oscillator Frequency
//	Write_Command(d);			//   Default => 0x70
//						//     D[3:0] => Display Clock Divider
//						//     D[7:4] => Oscillator Frequency
//}
//
//
//void Set_Low_Power(unsigned char d)
//{
//	Write_Command(0xD8);			// Set Low Power Display Mode
//	Write_Command(d);			//   Default => 0x04 (Normal Power Mode)
//}
//
//
//void Set_Precharge_Period(unsigned char d)
//{
//	Write_Command(0xD9);			// Set Pre-Charge Period
//	Write_Command(d);			//   Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
//						//     D[3:0] => Phase 1 Period in 1~15 Display Clocks
//						//     D[7:4] => Phase 2 Period in 1~15 Display Clocks
//}
//
//
//void Set_Common_Config(unsigned char d)
//{
//	Write_Command(0xDA);			// Set COM Pins Hardware Configuration
//	Write_Command(d);			//   Default => 0x12
//						//     Alternative COM Pin Configuration
//						//     Disable COM Left/Right Re-Map
//}
//
//
//void Set_VCOMH(unsigned char d)
//{
//	Write_Command(0xDB);			// Set VCOMH Deselect Level
//	Write_Command(d);			//   Default => 0x34 (0.78*VCC)
//}
//
///*
//void Set_NOP()
//{
//	Write_Command(0xE3);			// Command for No Operation
//}
//*/
//
//void Set_Command_Lock(unsigned char d)
//{
//	Write_Command(0xFD);			// Set Command Lock
//	Write_Command(d);			//   Default => 0x12
//						//     0x12 => Driver IC interface is unlocked from entering command.
//						//     0x16 => All Commands are locked except 0xFD.
//}
//
//
////=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
////  Global Variables
////=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//#define XLevelL		0x00
//#define XLevelH		0x10
//#define XLevel		((XLevelH&0x0F)*16+XLevelL)
//#define Max_Column	128
//#define Max_Row		64
//#define	Brightness	0xFF//0XBF
//
//
////=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
//
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
////  Show Regular Pattern (Full Screen)
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//void Fill_RAM(unsigned char Data)
//{
//unsigned char i,j;
//
//	for(i=0;i<8;i++)
//	{
//		Set_Start_Page(i);
//		Set_Start_Column(0x00);
//	
//		for(j=0;j<128;j++)
//		{
//			Write_Data(Data);
//		}
//	}
//}
//
//
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
////  Show Checkboard (Full Screen)
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//void Checkerboard(unsigned char a, unsigned char b)
//{
//unsigned char i,j;
//	
//	for(i=0;i<8;i++)
//	{
//		Set_Start_Page(i);
//		Set_Start_Column(0x00);
//
//		for(j=0;j<64;j++)
//		{
//			Write_Data(a);
//			Write_Data(b);
//		}
//	}
//}
//
//
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
////  Show Frame (Full Screen)
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//void Frame()
//{
//unsigned char i,j;
//	
//	Set_Start_Page(0x00);
//	Set_Start_Column(XLevel);
//
//	for(i=0;i<Max_Column;i++)
//	{
//		Write_Data(0x01);
//	}
//
//	Set_Start_Page(0x07);
//	Set_Start_Column(XLevel);
//
//	for(i=0;i<Max_Column;i++)
//	{
//		Write_Data(0x80);
//	}
//
//	for(i=0;i<8;i++)
//	{
//		Set_Start_Page(i);
//
//		for(j=0;j<Max_Column;j+=(Max_Column-1))
//		{
//			Set_Start_Column(XLevel+j);
//
//			Write_Data(0xFF);
//		}
//	}
//}
//
//
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
////  Show Pattern (Partial or Full Screen)
////
////    a: Start Page
////    b: End Page
////    c: Start Column
////    d: Total Columns
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//void Show_Pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d) //（内容，页首，页未，列首，列宽）
//{
//unsigned char *Src_Pointer;
//unsigned char i,j;
//
//	Src_Pointer=Data_Pointer;
//	for(i=a;i<(b+1);i++)//写4页
//	{
//		Set_Start_Page(i);//设置首页
//		Set_Start_Column(c);//设置首列
//
//		for(j=0;j<d;j++)//写48列宽
//		{
//			Write_Data(*Src_Pointer);
//			Src_Pointer++;
//		}
//	}
//}
//
//
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
////  Vertical / Fade Scrolling (Partial or Full Screen)
////
////    a: Scrolling Direction
////       "0x00" (Upward)
////       "0x01" (Downward)
////    b: Set Top Fixed Area
////    c: Set Vertical Scroll Area
////    d: Set Numbers of Row Scroll per Step
////    e: Set Time Interval between Each Scroll Step
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//void Vertical_Scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e)//（上/下滚动，首行，尾行，滚行，滚程）
//{
//unsigned int i,j;	
//
//	Write_Command(0xA3);			// Set Vertical Scroll Area  设置垂直滚动
//	Write_Command(b);			//   Default => 0x00 (Top Fixed Area)首行
//	Write_Command(c);			//   Default => 0x40 (Vertical Scroll Area)尾行
//
//	switch(a)
//	{
//		case 0://上滚动
//			for(i=0;i<c;i+=d)
//			{
//				Set_Start_Line(i);
//				for(j=0;j<e;j++)
//				{
//					uDelay(200);
//				}
//			}
//			break;
//		case 1://下滚动
//			for(i=0;i<c;i+=d)
//			{
//				Set_Start_Line(c-i);
//				for(j=0;j<e;j++)
//				{
//					uDelay(200);
//				}
//			}
//			break;
//	}
//	Set_Start_Line(0x00);
//}
//
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
////  Continuous Vertical / Horizontal / Diagonal Scrolling (Partial or Full Screen)
////
////    a: Scrolling Direction
////       "0x00" (Vertical & Rightward)
////       "0x01" (Vertical & Leftward)
////    b: Set Numbers of Column Scroll per Step (Horizontal / Diagonal Scrolling)
////       "0x00" (Horizontal / Diagonal Scrolling Off)
////       "0x01" (Horizontal / Diagonal Scrolling by 1 Column)
////    c: Define Start Row Address (Horizontal / Diagonal Scrolling)
////    d: Define End Page Address (Horizontal / Diagonal Scrolling)
////    e: Define Start Column Address
////    f: Define End Column Address
////    g: Set Top Fixed Area (Vertical Scrolling)
////    h: Set Vertical Scroll Area (Vertical Scrolling)
////    i: Set Numbers of Row Scroll per Step (Vertical / Diagonal Scrolling)
////    j: Set Time Interval between Each Scroll Step in Terms of Frame Frequency
////    k: Delay Time
////    * e+f must be less than or equal to the Multiplex Ratio...
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//void Continuous_Scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f, unsigned char g, unsigned char h, unsigned char i, unsigned char j, unsigned char k)
//{
//	//（垂直水平向左/右，有无水平滚动，首页，尾页，首列，尾列，区域顶点，区域底，滚动行数，滚动间隔，延时）
//	Write_Command(0xA3);			// Set Vertical Scroll Area设置垂直滚动区域
//	Write_Command(g);			//   Default => 0x00 (Top Fixed Area)
//	Write_Command(h);			//   Default => 0x40 (Vertical Scroll Area)
//
//	Write_Command(0x29+a);			// Continuous Vertical & Horizontal Scroll Setup垂直和水平向右滚动
//	Write_Command(b);//有无水平滚动
//	Write_Command(c);//起始页
//	Write_Command(j);//滚动间隔
//	Write_Command(d);//结束页
//	Write_Command(i);//滚动行数
//	Write_Command(e);//首列
//	Write_Command(f);//尾列	
//	Write_Command(0x2F);			// Activate Scrolling激活
//	Delay(k);
//}
//
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
////  Deactivate Scrolling (Full Screen)
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//void Deactivate_Scroll()
//{
//	Write_Command(0x2E);			// Deactivate Scrolling
//}
//
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
////  Fade In (Full Screen)
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//void Fade_In()
//{
//unsigned int i;	
//
//	Set_Display_On_Off(0xAF);
//	for(i=0;i<(Brightness+1);i++)
//	{
//		Set_Contrast_Control(i);//渐亮
//		uDelay(200);
//		uDelay(200);
//		uDelay(200);
//	}
//}
//
////  Fade Out (Full Screen)
////-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//void Fade_Out()
//{
//unsigned int i;	
//
//	for(i=(Brightness+1);i>0;i--)
//	{
//		Set_Contrast_Control(i-1);//渐暗
//		uDelay(200);
//		uDelay(200);
//		uDelay(200);
//	}
//	Set_Display_On_Off(0xAE);
//}
//
////=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
////  Initialization
////=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//void OLED_Init()
//{
//unsigned char i;
//
//	RES_oled=0;
//	for(i=0;i<200;i++)
//	{
//		uDelay(40);
//	//	delay_us(200);
//	}
//	RES_oled=1;
//
//	Set_Command_Lock(0x12);			// (0x12解锁/0x16)		解锁驱动IC FDH
//	Set_Display_On_Off(0xAE);		// (0xAE关/0xAF开)				显示关
//	Set_Display_Clock(0xA0);		// Set Clock as 116 Frames/Sec			显示频率
//	Set_Multiplex_Ratio(0x3F);		// 1/64 Duty (0x0F~0x3F)				占空比
//	Set_Display_Offset(0x00);		// Shift Mapping RAM Counter (0x00~0x3F)显示偏移量
//	Set_Start_Line(0x00);			// Set Mapping RAM Display Start Line (0x00~0x3F)设置起始行
//	Set_Low_Power(0x04);			// Set Normal Power Mode (0x04/0x05)	设置正常的功率模式（未找到
//	Set_Addressing_Mode(0x02);		// (0x00水平/0x01垂直/0x02页面)设置页面寻址方式
//	
//	Set_Segment_Remap(0xa1);		//（A0 地址0列SEG0）（A1地址0列S127)
//	Set_Common_Remap(0xC8);			// (0xC0正常/0xC8反向)COM的扫描方向C63-C0
//	Set_Common_Config(0x12);		// Set Alternative Configuration (0x02/0x12) 选择COM，映射
//	
//	Set_Contrast_Control(Brightness);	// Set SEG Output Current   		设置对比度
//	Set_Precharge_Period(0x82);		// Set Pre-Charge as 8 Clocks & Discharge as 2 Clocks设置预充8变到2
//	Set_VCOMH(0x34);			// Set VCOM Deselect Level  				去除水波纹
//	Set_Entire_Display(0xA4);		// Disable Entire Display On (0xA4/0xA5)全部显示开
//	Set_Inverse_Display(0xA6);		// Disable Inverse Display On (0xA6/0xA7)正常显示，正显
//
//	Fill_RAM(0x00);				// Clear Screen清屏
//
//	Set_Display_On_Off(0xAF);		// Display On (0xAE/0xAF)显示开
//}
//
//void display_graphic_16x16(unsigned char fb,unsigned char page,unsigned char column,unsigned char *dp)
//{
//	unsigned int i,j;
//	for(j=0;j<2;j++)
//	{
//		Set_Start_Page(page+j);
//		Set_Start_Column(column);		
//		for (i=0;i<16;i++)
//		{
//			if(fb==1) Write_Data(*dp);
//			else Write_Data(~(*dp)); 
//			dp++;
//		}
//	}
//}
//
////void ini_int1(void) 
////{ 
////EA=1; 
////EX0=1;//允许外部INT0的中断 
////IT0=1;// 允许中断
////}
////
//// int scankey1() interrupt 0 using 1 //使用外部中断1,寄存器组3
////{ 
////while(P3^2==0){for(;;);}
////   IE1=0;//中断标志清零 
////   
////}
//
////IIIIIIIIIIIIIIIIIIIIIIIIIIIIII
////短延时
////void delay_us(unsigned int n_us)
////	{
////	unsigned int j,k;
////	for(j=0;j<n_us;j++)
////	for(k=0;k<1;k++);
////	}
//
////FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
////送指令到字库IC
//void send_command_to_ROM( unsigned char datu )
//	{
//
//	unsigned char i;
//	for(i=0;i<8;i++ )
//		{
//
//		Rom_SCK = 0;
//		delay_us(10);
//		if(datu&0x80)
//		  Rom_IN = 1;
//		else 
//		 Rom_IN = 0;
//		datu = datu<<1;
//		Rom_SCK=1;
//		delay_us(10);
//		}
//	}
//
////EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
////从指定地址读出数据写到液晶屏指定（page,column)座标中
//void get_and_write_16x16(unsigned long fontaddr,unsigned char page,unsigned char column)
//	{
//	unsigned char i,j,disp_data;
//	Rom_CS=0;
//	send_command_to_ROM(0x03);
//	send_command_to_ROM((fontaddr&0xff0000)>>16); //地址的高8 位,共24 位
//	send_command_to_ROM((fontaddr&0xff00)>>8); //地址的中8 位,共24 位
//	send_command_to_ROM(fontaddr&0xff); //地址的低8 位,共24 位
//	for(j=0;j<2;j++)
//		{
//		//lcd_address(page+j,column);
//		Set_Start_Page(page+j);
//		Set_Start_Column(column);
//		for(i=0; i<16; i++ )
//			{
//			disp_data=get_data_from_ROM();
//			Write_Data(disp_data); //写数据到LCD,每写完1 字节的数据后列地址自动加1
//			}
//		}
//	Rom_CS=1;
//	}
//
////从指定地址读出数据写到液晶屏指定（page,column)座标中
//void get_and_write_8x16(unsigned long fontaddr,unsigned char page,unsigned char column)
//{
//	unsigned char i,j,disp_data;
//	Rom_CS = 0;
//	send_command_to_ROM(0x03);
//	send_command_to_ROM((fontaddr&0xff0000)>>16); //地址的高8 位,共24 位
//	send_command_to_ROM((fontaddr&0xff00)>>8); //地址的中8 位,共24 位
//	send_command_to_ROM(fontaddr&0xff); //地址的低8 位,共24 位
//	for(j=0;j<2;j++)
//
//		{
//		//lcd_address(page+j,column);
//		Set_Start_Page(page+j);
//		Set_Start_Column(column);
//		for(i=0; i<8; i++ )
//			{
//			disp_data=get_data_from_ROM();
//			Write_Data(disp_data); //写数据到LCD,每写完1 字节的数据后列地址自动加1
//			}
//		}
//	Rom_CS=1;
//}
//
////ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
//void display_GB2312_string(unsigned char page,unsigned char column,unsigned char *text)
//{
//unsigned char i= 0;
//while((text[i]>0x00))
//{
//	if(((text[i]>=0xb0) &&(text[i]<=0xf7))&&(text[i+1]>=0xa1))
//		{
//		//国标简体（GB2312）汉字在字库IC 中的地址由以下公式来计算：
//		//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0
//		//由于担心8 位单片机有乘法溢出问题，所以分三部取地址
//		fontaddr = (text[i]- 0xb0)*94;
//		fontaddr += (text[i+1]-0xa1)+846;
//		fontaddr = (unsigned long)(fontaddr*32);
//		get_and_write_16x16(fontaddr,page,column); //从指定地址读出数据写到液晶屏指定（page,column)座标中
//		i+=2;
//		column+=16;
//		}
//	else if(((text[i]>=0xa1) &&(text[i]<=0xa3))&&(text[i+1]>=0xa1))
//		{
//		//国标简体（GB2312）15x16 点的字符在字库IC 中的地址由以下公式来计算：
//		//Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0
//		//由于担心8 位单片机有乘法溢出问题，所以分三部取地址
//		fontaddr = (text[i]- 0xa1)*94;
//		fontaddr += (text[i+1]-0xa1);
//		fontaddr = (unsigned long)(fontaddr*32);
//		get_and_write_16x16(fontaddr,page,column); //从指定地址读出数据写到液晶屏指定（page,column)座标中
//		i+=2;
//		column+=16;
//		}
//	else if((text[i]>=0x20) &&(text[i]<=0x7e))
//
//		{
//		fontaddr = (text[i]- 0x20);
//		fontaddr = (unsigned long)(fontaddr*16);
//		fontaddr = (unsigned long)(fontaddr+0x3cf80);
//		get_and_write_8x16(fontaddr,page,column); //从指定地址读出数据写到液晶屏指定（page,column)座标中
//		i+=1;
//		column+=8;
//		}
//	else
//	i++;
//	}
//}
//	
////romjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj
////从字库IC 中取汉字或字符数据（1 个字节）
//static unsigned char get_data_from_ROM( )
//	{
//	unsigned char i;
//	unsigned char ret_data=0;
//	for(i=0;i<8;i++)
//		{
//	//	Rom_OUT=1;
//		Rom_SCK=0;
//		//delay_us(1);
//		ret_data=ret_data<<1;
//		if(Rom_OUT)
//		ret_data=ret_data+1;
//		else
//		ret_data=ret_data+0;
//		Rom_SCK =1;
//		//delay_us(1);
//		}
//	return(ret_data);
//	}
//	
////从指定地址读出数据写到液晶屏指定（page,column)座标中
//void get_and_write_5x8(unsigned long fontaddr,unsigned char page,unsigned char column)
//	{
//	unsigned char i,disp_data;
//	Rom_CS = 0;
//	send_command_to_ROM(0x03);
//	send_command_to_ROM((fontaddr&0xff0000)>>16); //地址的高8 位,共24 位
//	send_command_to_ROM((fontaddr&0xff00)>>8); //地址的中8 位,共24 位
//	send_command_to_ROM(fontaddr&0xff); //地址的低8 位,共24 位
//	//lcd_address(page,column);
//		Set_Start_Page(page);
//		Set_Start_Column(column);
//		for(i=0; i<5; i++ )
//		{
//		disp_data=get_data_from_ROM();
//		Write_Data(disp_data); //写数据到LCD,每写完1 字节的数据后列地址自动加1
//		}
//	Rom_CS=1;
//	}
//	
//void display_string_5x8(unsigned char page,unsigned char column,unsigned char *text)
//{
//	unsigned char i= 0;
//	while((text[i]>0x00))
//	{
//		if((text[i]>=0x20) &&(text[i]<=0x7e))
//		{
//		fontaddr = (text[i]- 0x20);
//		fontaddr = (unsigned long)(fontaddr*8);
//		fontaddr = (unsigned long)(fontaddr+0x3bfc0);
//		get_and_write_5x8(fontaddr,page,column); //从指定地址读出数据写到液晶屏指定（page,column)座标中
//		i+=1;
//		column+=6;
//		}
//	else
//	i++;
//	}
//}
//
//
//
//		
////=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
////  Main Program
////=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//void oled_display()
//{
//
////	unsigned char i;
////	ini_int1();//开中断
////	P1=0xFF;
////	P3=0xFF;
//
//// unsigned char ans[4];
////11111111111111111111111111111111111111111111111111111111111111111111
////for(m1=0;m1<10;m1++)
////	{
//
////	 u16 coxx=0x1234;
//
//	//  PM25_Value_1;
//
//	  unsigned char *PM25;
//	  unsigned char *Temp1;
//	  unsigned char *Humi1;
//	  unsigned char *CO2;
//	  unsigned char *CO;
//	  unsigned char *VOC;
//	 
//	 unsigned char ctime[10];
//	 unsigned char ctime1[10];
//	 unsigned char ctime2[10];
//	 unsigned char ctime3[10];
//	 unsigned char ctime4[10];
//	 unsigned char ctime5[10];
//  	//OLED_Init();
//   sprintf(ctime, "%d", PM25_Value_1); 
//    PM25=ctime;
//	 sprintf(ctime1, "%d", VOC_Data); 
//	   VOC=ctime1;
//
//	 sprintf(ctime2, "%d", Temperature); 
//	   Temp1=ctime2;   
//	 sprintf(ctime3, "%d", Humidity); 
//	   Humi1=ctime3;   
//	 sprintf(ctime4, "%d", CO2_Data); 
//	   CO2=ctime4;   
//	 sprintf(ctime5, "%d", CO_Data); 
//	   CO=ctime5;   
//   	//	Delay(1);	
//		Fill_RAM(0x00);		// 清零
//		Frame();			//边框
//		display_GB2312_string(0,5,"PM25:");
//		display_GB2312_string(0,45,PM25);//图形加汉字  字库显示
//
//		display_GB2312_string(0,70,"VOC:");
//		display_GB2312_string(0,100,VOC);//图形加汉字  字库显示
//
//		display_GB2312_string(2,5,"CO2:");
//		display_GB2312_string(2,38 ,CO2);//图形加汉字  字库显示
//
//		display_GB2312_string(2,75,"CO:");
//		display_GB2312_string(2,98,CO);//图形加汉字  字库显示
//
//		display_GB2312_string(4,10,"温度:");
//		display_GB2312_string(4,50,Temp1);//图形加汉字  字库显示
//
//		display_GB2312_string(6,10,"湿度:");
//		display_GB2312_string(6,50,Humi1);//图形加汉字  字库显示
//	//	display_GB2312_string(4,50,ans);
////		Delay(2);//延时400MS（X）
//	//	display_graphic_16x16(4,2,0,ans);	
////	//~~~~~~~~~~~~~~~~~~~~~@~~~~~~~~&~~~~~~~~~~@~~~~~~~~~~~~ROM汉字/字符图形	
////
////
////		
////	//~~~~~~~~~~~~~~~~~~~~~@~~~~~~~~&~~~~~~~~~~@~~~~~~~~~~~~9屏，自建汉字图形		
////		Fill_RAM(0x00);			// Clear Screen
////		Checkerboard(0xaa,0xaa);
////		Delay(1);	
////		Checkerboard(0x55,0x55);
////		Delay(1);	
////		Checkerboard(0xff,0x00);
////		Delay(1);	
////		Checkerboard(0x00,0xff);
////		Delay(1);		
////		Checkerboard(0x55,0xaa);
////		Delay(1);
////		Checkerboard(0xaa,0x55);
////		Delay(1);		
////		Fill_RAM(0x00);			// Clear Screen
//////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~横竖点扫描演示	
////		
////		display_GB2312_string(0,1,"欢迎使用绘晶模块"); //在第1 页，第1 列，显示一串16x16 点阵汉字或8x16 的ASCII 字
////		display_GB2312_string(2,1,"可以显示简体汉字"); //显示一串16x16 点阵汉字或8x16 的ASCII 字.以下雷同
////		display_GB2312_string(4,1,"可以显示通用字符");
////		display_GB2312_string(6,1,"可以任意自编图形");
////		Delay(1);
//////		Fill_RAM(0x00);			// Clear Screen
////		display_GB2312_string(0,1,"绘晶科技专业生产");
////		display_GB2312_string(2,1,"研发销售液晶模块");
////		display_GB2312_string(4,1,"地址深圳宝安石岩");
////		display_GB2312_string(6,1,"镇麻布第二业区绘");
////		Delay(1);
////		Fill_RAM(0x00);			// Clear Screen
////		display_GB2312_string(0,1,"图形点阵显示模块");
////		display_GB2312_string(2,1,"中文字库显示模块");
////		display_GB2312_string(4,1,"自编汉字符号显示");
////		display_GB2312_string(6,1,"粗体，例如： ");
////		display_graphic_16x16(0,6,96,hui1); //在第7页，第81列显示单个自编生僻汉字“绘”
////		display_graphic_16x16(0,6,112,jing1); //显示单个自编生僻汉字"晶“
////		Delay(1);
////		Fill_RAM(0x00);			// Clear Screen
////
////		display_GB2312_string(0,1,"<!@#$%^&*()_-+]/"); //在第1 页，第1 列，显示一串16x16 点阵汉字或8*16 的ASCII 字
////		display_string_5x8(2,1,"<!@#$%^&*()_-+]/;.,?[");//在第3 页，第1 列，显示一串5x8 点阵的ASCII 字
////		display_string_5x8(3,1,"H J electronics Co., ");//显示一串5x8 点阵的ASCII 字
////		display_string_5x8(4,1,"Ltd. established at ");//显示一串5x8 点阵的ASCII 字
////		display_string_5x8(5,1,"year 2010.Focus LCM. ");//显示一串5x8 点阵的ASCII 字
////		display_string_5x8(6,1,"TEL:0755-23146001 ");//显示一串5x8 点阵的ASCII 字
////		display_string_5x8(7,1,"FAX:0755-61901158 ");//显示一串5x8 点阵的ASCII 字
////		
////		Delay(1);
////		Fill_RAM(0x00);			// Clear Screen
////		display_GB2312_string(0,1,"啊阿埃挨哎唉哀皑"); //在第1 页，第1 列，显示一串16x16 点阵汉字或8x16 的ASCII 字
////		display_GB2312_string(2,1,"癌蔼矮艾碍爱隘鞍"); //显示一串16x16 点阵汉字或8x16 的ASCII 字.以下雷同
////		display_GB2312_string(4,1,"氨安俺按暗岸胺案");
////		display_GB2312_string(6,1,"肮昂盎凹敖熬翱袄");
////		Delay(1);
////		Fill_RAM(0x00);			// Clear Screen
////		display_GB2312_string(0,1,"鬟鬣麽麾縻麂麇麈");
////		display_GB2312_string(2,1,"麋麒鏖麝麟黛黜黝");
////		display_GB2312_string(4,1,"黠黟黢黩黧黥黪黯");
////		display_GB2312_string(6,1,"鼢鼬鼯鼹鼷鼽鼾齄");
////		Delay(1);
////		Fill_RAM(0x00);			// Clear Screen
////
////	}
///*	
//for(m1=0;m1<2;m1++)		//以下滚动演示
//	{
////11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111	
//	// Show Pattern - Huijing OLED
//		Show_Pattern(&Huijing[0][0],0x02,0x05,XLevel+0x28,0x30);//（内容，页首，页未，列首，列宽）
//		Delay(1);//延时400MS（X）
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~显示图片演示公司标志
//	// Fade In/Out (Full Screen)
//		Fade_Out();//渐暗，关显示
//		Fade_In();//开显示，渐亮
//		Fade_Out();//渐暗，关显示
//		Fade_In();//开显示，渐亮
//		Delay(1);
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~对比度变化演示
//	// Scrolling (Partial or Full Screen)
//		Vertical_Scroll(0x00,0x00,Max_Row,0x01,0x60);//（上/下滚动，首行，尾行，滚行，滚程）
//						// Upward - Full Screen
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~向上滚动演示						
//		Delay(1);
//		Vertical_Scroll(0x01,0x00,Max_Row,0x01,0x60);//（上/下滚动，首行，尾行，滚行，滚程）
//						// Downward - Full Screen
//		Delay(1);
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~向下滚动演示				
//		Deactivate_Scroll();//设置列地址
//		//（垂直水平向左/右，有无水平滚动，首页，尾页，首列，尾列，区域顶点，区域底，滚动行数，滚动间隔，延时）
//		Continuous_Scroll(0x00,0x00,0x00,0x00,XLevel,XLevel+Max_Column-1,0x00,0x20,0x01,0x00,0x01);
//						// Upward - Top Area
//		Continuous_Scroll(0x00,0x00,0x00,0x00,XLevel,XLevel+Max_Column-1,0x00,0x20,0x1F,0x00,0x01);
//						// Downward - Top Area
//		Continuous_Scroll(0x00,0x01,0x00,0x03,XLevel,XLevel+Max_Column-1,0x00,0x20,0x01,0x00,0x01);
//						// Up & Rightward - Top Area
//		Continuous_Scroll(0x01,0x01,0x00,0x03,XLevel,XLevel+Max_Column-1,0x00,0x20,0x1F,0x00,0x01);
//						// Down & Leftward - Top Area
//		Continuous_Scroll(0x01,0x01,0x04,0x07,XLevel,XLevel+Max_Column-1,0x00,0x20,0x01,0x00,0x01);
//						// Upward - Top Area
//						// Leftward - Bottom Area
//		Continuous_Scroll(0x00,0x01,0x04,0x07,XLevel,XLevel+Max_Column-1,0x00,0x20,0x1F,0x00,0x01);
//						// Downward - Top Area
//						// Rightward - Bottom Area
//		Deactivate_Scroll();//设置列地址
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~滚动演示	
//		Fill_RAM(0x00);			// Clear Screen
//		
//	}	
//*/
////	Fill_RAM(0x00);			// Clear Screen
////	Set_Contrast_Control(0xff);	// Set SEG Output Current   		设置对比度	
////	display_graphic_16x16(0,0,0,hui1); 	
////	display_graphic_16x16(0,2,0,jing1); 
////
////	Continuous_Scroll(0x00,0x00,0x00,0x00,XLevel,XLevel+Max_Column-1,0x00,0x20,0x1F,0x00,0x01);
//						// Downward - Top Area
//
//}
//
////unsigned char  hui1[]={/*--  文字:  绘  --*/
/////*--  黑体12;  此字体下对应的点阵为：宽x高=16x16   --*/
////0x40,0x70,0x7C,0xFE,0xFC,0xF0,0xE0,0xF0,0x7C,0x7E,0x4E,0x7E,0x78,0x60,0xE0,0x40,
////0x60,0x66,0x27,0x27,0x35,0x34,0x76,0x72,0x7A,0x5E,0x4E,0x5A,0xFA,0xF2,0xC2,0x00,
////}
//
////unsigned char  jing1[]={/*--  文字:  晶  --*/
/////*--  黑体12;  此字体下对应的点阵为：宽x高=16x16   --*/
////0x00,0x00,0x00,0x7C,0x7C,0x54,0x54,0x54,0x54,0x54,0x7C,0x7C,0x7C,0x00,0x00,0x00,
////0x00,0x7F,0x7F,0x29,0x29,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x29,0x29,0x7F,0x7F,0x00,
////}
//
////unsigned char  Huijing[][48]={			// Huijing OLED
//////
/////*--  宽度x高度=48x32  --*/
////0x00,0x00,0x20,0x30,0xAC,0x63,0x10,0x20,0x10,0x48,0x44,0x43,0x44,0x48,0x10,0x20,
////0x20,0x00,0x00,0x00,0x00,0x00,0x7F,0x49,0x49,0x49,0x49,0x49,0x7F,0x00,0x00,0x00,
////0x00,0x00,0x24,0x24,0xA4,0xFE,0xA3,0x22,0x00,0x22,0xCC,0x00,0x00,0xFF,0x00,0x00,
////0x00,0x00,0x22,0x67,0x22,0x12,0x12,0x40,0xE2,0x52,0x4A,0x46,0x42,0x52,0x62,0xC2,
////0x00,0x00,0x00,0xFF,0x49,0x49,0x49,0x49,0xFF,0x00,0xFF,0x49,0x49,0x49,0x49,0xFF,
////0x00,0x00,0x08,0x06,0x01,0xFF,0x00,0x01,0x04,0x04,0x04,0x04,0x04,0xFF,0x02,0x02,
////0x00,0x00,0x10,0x10,0x10,0xFF,0x10,0x90,0x08,0x88,0x88,0x88,0xFF,0x88,0x88,0x88,
////0x08,0x00,0x00,0x00,0x00,0xF0,0x08,0x08,0x18,0xE0,0x00,0x08,0xF8,0x08,0x00,0x00,
////0x00,0x08,0xF8,0x88,0xE8,0x18,0x00,0x08,0xF8,0x08,0x10,0xE0,0x00,0x00,0x00,0x00,
////0x00,0x00,0x04,0x44,0x82,0x7F,0x01,0x80,0x80,0x40,0x43,0x2C,0x10,0x28,0x46,0x81,
////0x80,0x00,0x00,0x00,0x00,0x1F,0x20,0x20,0x30,0x0F,0x00,0x20,0x3F,0x20,0x20,0x20,
////0x30,0x20,0x3F,0x20,0x23,0x30,0x00,0x20,0x3F,0x20,0x10,0x0F,0x00,0x00,0x00,0x00,
////
////	}
//
