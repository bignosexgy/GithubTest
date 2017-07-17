#include "spi.h"
#include "gpio.h"
#include "usart.h"
//Mini STM32������
//SPI ���� V1.2
//����ԭ��@ALIENTEK
//2010/6/13	
extern u16 PM25_Value_1;
extern  float VOC_Data;
extern int16_t Temperature;
extern u16 Humidity	;
extern	u16 CO2_Data;
extern	u16 CO_Data;
//extern static char *itoa(int value, char *string, int radix);
//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ������SD Card/W25X16/24L01/JF24C	
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


						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
void SPIx_Init(void)
{	 
	RCC->APB2ENR|=1<<2;       //PORTAʱ��ʹ�� 	 
	RCC->APB2ENR|=1<<12;      //SPI1ʱ��ʹ�� 
		   
	//����ֻ���SPI�ڳ�ʼ��
	GPIOA->CRL&=0X000FFFFF; 
	GPIOA->CRL|=0XBBB00000;//PA5.6.7���� 	    
	GPIOA->ODR|=0X7<<5;    //PA5.6.7����
		
	SPI1->CR1|=0<<10;//ȫ˫��ģʽ	
	SPI1->CR1|=1<<9; //���nss����
	SPI1->CR1|=1<<8;  

	SPI1->CR1|=1<<2; //SPI����
	SPI1->CR1|=0<<11;//8bit���ݸ�ʽ	
	SPI1->CR1|=1<<1; //����ģʽ��SCKΪ1 CPOL=1
	SPI1->CR1|=1<<0; //���ݲ����ӵڶ���ʱ����ؿ�ʼ,CPHA=1  
	SPI1->CR1|=7<<3; //Fsck=Fcpu/256
	SPI1->CR1|=0<<7; //MSBfirst   
	SPI1->CR1|=1<<6; //SPI�豸ʹ��
	SPIx_ReadWriteByte(0xff);//��������		 
}   
//SPI �ٶ����ú���
//SpeedSet:
//SPI_SPEED_2   2��Ƶ   (SPI 36M@sys 72M)
//SPI_SPEED_8   8��Ƶ   (SPI 9M@sys 72M)
//SPI_SPEED_16  16��Ƶ  (SPI 4.5M@sys 72M)
//SPI_SPEED_256 256��Ƶ (SPI 281.25K@sys 72M)
void SPIx_SetSpeed(u8 SpeedSet)
{
	SPI1->CR1&=0XFFC7;//Fsck=Fcpu/256
	if(SpeedSet==SPI_SPEED_2)//����Ƶ
	{
		SPI1->CR1|=0<<3;//Fsck=Fpclk/2=36Mhz	
	}else if(SpeedSet==SPI_SPEED_8)//�˷�Ƶ 
	{
		SPI1->CR1|=2<<3;//Fsck=Fpclk/8=9Mhz	
	}else if(SpeedSet==SPI_SPEED_16)//ʮ����Ƶ
	{
		SPI1->CR1|=3<<3;//Fsck=Fpclk/16=4.5Mhz
	}else			 	 //256��Ƶ
	{
		SPI1->CR1|=7<<3; //Fsck=Fpclk/256=281.25Khz ����ģʽ
	}
	SPI1->CR1|=1<<6; //SPI�豸ʹ��	  
} 
//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPIx_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 
	while((SPI1->SR&1<<1)==0)//�ȴ���������	
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI1->DR=TxData;	 	  //����һ��byte 
	retry=0;
	while((SPI1->SR&1<<0)==0) //�ȴ�������һ��byte  
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI1->DR;          //�����յ�������				    
}
void SPI1_SetSpeed(u8 SpeedSet)
{
	SpeedSet&=0X07;			//���Ʒ�Χ
	SPI1->CR1&=0XFFC7; 
	SPI1->CR1|=SpeedSet<<3;	//����SPI1�ٶ�  
	SPI1->CR1|=1<<6; 		//SPI�豸ʹ��	   
}

u8 SPI1_ReadWriteByte(u8 TxData)
{				   			 
	while((SPI1->SR&1<<1)==0);//�ȴ���������  
	SPI1->DR=TxData;	 	  //����һ��byte 	   
	while((SPI1->SR&1<<0)==0); //�ȴ�������һ��byte   			    
	return SPI1->DR;          //�����յ�������				    
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
////unsigned char  hui1[100]={/*--  ����:  ��  --*/
///////*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
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
//void Show_Pattern(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d) //�����ݣ�ҳ�ף�ҳδ�����ף��п�
//{
//unsigned char *Src_Pointer;
//unsigned char i,j;
//
//	Src_Pointer=Data_Pointer;
//	for(i=a;i<(b+1);i++)//д4ҳ
//	{
//		Set_Start_Page(i);//������ҳ
//		Set_Start_Column(c);//��������
//
//		for(j=0;j<d;j++)//д48�п�
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
//void Vertical_Scroll(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e)//����/�¹��������У�β�У����У����̣�
//{
//unsigned int i,j;	
//
//	Write_Command(0xA3);			// Set Vertical Scroll Area  ���ô�ֱ����
//	Write_Command(b);			//   Default => 0x00 (Top Fixed Area)����
//	Write_Command(c);			//   Default => 0x40 (Vertical Scroll Area)β��
//
//	switch(a)
//	{
//		case 0://�Ϲ���
//			for(i=0;i<c;i+=d)
//			{
//				Set_Start_Line(i);
//				for(j=0;j<e;j++)
//				{
//					uDelay(200);
//				}
//			}
//			break;
//		case 1://�¹���
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
//	//����ֱˮƽ����/�ң�����ˮƽ��������ҳ��βҳ�����У�β�У����򶥵㣬����ף����������������������ʱ��
//	Write_Command(0xA3);			// Set Vertical Scroll Area���ô�ֱ��������
//	Write_Command(g);			//   Default => 0x00 (Top Fixed Area)
//	Write_Command(h);			//   Default => 0x40 (Vertical Scroll Area)
//
//	Write_Command(0x29+a);			// Continuous Vertical & Horizontal Scroll Setup��ֱ��ˮƽ���ҹ���
//	Write_Command(b);//����ˮƽ����
//	Write_Command(c);//��ʼҳ
//	Write_Command(j);//�������
//	Write_Command(d);//����ҳ
//	Write_Command(i);//��������
//	Write_Command(e);//����
//	Write_Command(f);//β��	
//	Write_Command(0x2F);			// Activate Scrolling����
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
//		Set_Contrast_Control(i);//����
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
//		Set_Contrast_Control(i-1);//����
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
//	Set_Command_Lock(0x12);			// (0x12����/0x16)		��������IC FDH
//	Set_Display_On_Off(0xAE);		// (0xAE��/0xAF��)				��ʾ��
//	Set_Display_Clock(0xA0);		// Set Clock as 116 Frames/Sec			��ʾƵ��
//	Set_Multiplex_Ratio(0x3F);		// 1/64 Duty (0x0F~0x3F)				ռ�ձ�
//	Set_Display_Offset(0x00);		// Shift Mapping RAM Counter (0x00~0x3F)��ʾƫ����
//	Set_Start_Line(0x00);			// Set Mapping RAM Display Start Line (0x00~0x3F)������ʼ��
//	Set_Low_Power(0x04);			// Set Normal Power Mode (0x04/0x05)	���������Ĺ���ģʽ��δ�ҵ�
//	Set_Addressing_Mode(0x02);		// (0x00ˮƽ/0x01��ֱ/0x02ҳ��)����ҳ��Ѱַ��ʽ
//	
//	Set_Segment_Remap(0xa1);		//��A0 ��ַ0��SEG0����A1��ַ0��S127)
//	Set_Common_Remap(0xC8);			// (0xC0����/0xC8����)COM��ɨ�跽��C63-C0
//	Set_Common_Config(0x12);		// Set Alternative Configuration (0x02/0x12) ѡ��COM��ӳ��
//	
//	Set_Contrast_Control(Brightness);	// Set SEG Output Current   		���öԱȶ�
//	Set_Precharge_Period(0x82);		// Set Pre-Charge as 8 Clocks & Discharge as 2 Clocks����Ԥ��8�䵽2
//	Set_VCOMH(0x34);			// Set VCOM Deselect Level  				ȥ��ˮ����
//	Set_Entire_Display(0xA4);		// Disable Entire Display On (0xA4/0xA5)ȫ����ʾ��
//	Set_Inverse_Display(0xA6);		// Disable Inverse Display On (0xA6/0xA7)������ʾ������
//
//	Fill_RAM(0x00);				// Clear Screen����
//
//	Set_Display_On_Off(0xAF);		// Display On (0xAE/0xAF)��ʾ��
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
////EX0=1;//�����ⲿINT0���ж� 
////IT0=1;// �����ж�
////}
////
//// int scankey1() interrupt 0 using 1 //ʹ���ⲿ�ж�1,�Ĵ�����3
////{ 
////while(P3^2==0){for(;;);}
////   IE1=0;//�жϱ�־���� 
////   
////}
//
////IIIIIIIIIIIIIIIIIIIIIIIIIIIIII
////����ʱ
////void delay_us(unsigned int n_us)
////	{
////	unsigned int j,k;
////	for(j=0;j<n_us;j++)
////	for(k=0;k<1;k++);
////	}
//
////FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
////��ָ��ֿ�IC
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
////��ָ����ַ��������д��Һ����ָ����page,column)������
//void get_and_write_16x16(unsigned long fontaddr,unsigned char page,unsigned char column)
//	{
//	unsigned char i,j,disp_data;
//	Rom_CS=0;
//	send_command_to_ROM(0x03);
//	send_command_to_ROM((fontaddr&0xff0000)>>16); //��ַ�ĸ�8 λ,��24 λ
//	send_command_to_ROM((fontaddr&0xff00)>>8); //��ַ����8 λ,��24 λ
//	send_command_to_ROM(fontaddr&0xff); //��ַ�ĵ�8 λ,��24 λ
//	for(j=0;j<2;j++)
//		{
//		//lcd_address(page+j,column);
//		Set_Start_Page(page+j);
//		Set_Start_Column(column);
//		for(i=0; i<16; i++ )
//			{
//			disp_data=get_data_from_ROM();
//			Write_Data(disp_data); //д���ݵ�LCD,ÿд��1 �ֽڵ����ݺ��е�ַ�Զ���1
//			}
//		}
//	Rom_CS=1;
//	}
//
////��ָ����ַ��������д��Һ����ָ����page,column)������
//void get_and_write_8x16(unsigned long fontaddr,unsigned char page,unsigned char column)
//{
//	unsigned char i,j,disp_data;
//	Rom_CS = 0;
//	send_command_to_ROM(0x03);
//	send_command_to_ROM((fontaddr&0xff0000)>>16); //��ַ�ĸ�8 λ,��24 λ
//	send_command_to_ROM((fontaddr&0xff00)>>8); //��ַ����8 λ,��24 λ
//	send_command_to_ROM(fontaddr&0xff); //��ַ�ĵ�8 λ,��24 λ
//	for(j=0;j<2;j++)
//
//		{
//		//lcd_address(page+j,column);
//		Set_Start_Page(page+j);
//		Set_Start_Column(column);
//		for(i=0; i<8; i++ )
//			{
//			disp_data=get_data_from_ROM();
//			Write_Data(disp_data); //д���ݵ�LCD,ÿд��1 �ֽڵ����ݺ��е�ַ�Զ���1
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
//		//������壨GB2312���������ֿ�IC �еĵ�ַ�����¹�ʽ�����㣺
//		//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0
//		//���ڵ���8 λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ
//		fontaddr = (text[i]- 0xb0)*94;
//		fontaddr += (text[i+1]-0xa1)+846;
//		fontaddr = (unsigned long)(fontaddr*32);
//		get_and_write_16x16(fontaddr,page,column); //��ָ����ַ��������д��Һ����ָ����page,column)������
//		i+=2;
//		column+=16;
//		}
//	else if(((text[i]>=0xa1) &&(text[i]<=0xa3))&&(text[i+1]>=0xa1))
//		{
//		//������壨GB2312��15x16 ����ַ����ֿ�IC �еĵ�ַ�����¹�ʽ�����㣺
//		//Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0
//		//���ڵ���8 λ��Ƭ���г˷�������⣬���Է�����ȡ��ַ
//		fontaddr = (text[i]- 0xa1)*94;
//		fontaddr += (text[i+1]-0xa1);
//		fontaddr = (unsigned long)(fontaddr*32);
//		get_and_write_16x16(fontaddr,page,column); //��ָ����ַ��������д��Һ����ָ����page,column)������
//		i+=2;
//		column+=16;
//		}
//	else if((text[i]>=0x20) &&(text[i]<=0x7e))
//
//		{
//		fontaddr = (text[i]- 0x20);
//		fontaddr = (unsigned long)(fontaddr*16);
//		fontaddr = (unsigned long)(fontaddr+0x3cf80);
//		get_and_write_8x16(fontaddr,page,column); //��ָ����ַ��������д��Һ����ָ����page,column)������
//		i+=1;
//		column+=8;
//		}
//	else
//	i++;
//	}
//}
//	
////romjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj
////���ֿ�IC ��ȡ���ֻ��ַ����ݣ�1 ���ֽڣ�
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
////��ָ����ַ��������д��Һ����ָ����page,column)������
//void get_and_write_5x8(unsigned long fontaddr,unsigned char page,unsigned char column)
//	{
//	unsigned char i,disp_data;
//	Rom_CS = 0;
//	send_command_to_ROM(0x03);
//	send_command_to_ROM((fontaddr&0xff0000)>>16); //��ַ�ĸ�8 λ,��24 λ
//	send_command_to_ROM((fontaddr&0xff00)>>8); //��ַ����8 λ,��24 λ
//	send_command_to_ROM(fontaddr&0xff); //��ַ�ĵ�8 λ,��24 λ
//	//lcd_address(page,column);
//		Set_Start_Page(page);
//		Set_Start_Column(column);
//		for(i=0; i<5; i++ )
//		{
//		disp_data=get_data_from_ROM();
//		Write_Data(disp_data); //д���ݵ�LCD,ÿд��1 �ֽڵ����ݺ��е�ַ�Զ���1
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
//		get_and_write_5x8(fontaddr,page,column); //��ָ����ַ��������д��Һ����ָ����page,column)������
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
////	ini_int1();//���ж�
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
//		Fill_RAM(0x00);		// ����
//		Frame();			//�߿�
//		display_GB2312_string(0,5,"PM25:");
//		display_GB2312_string(0,45,PM25);//ͼ�μӺ���  �ֿ���ʾ
//
//		display_GB2312_string(0,70,"VOC:");
//		display_GB2312_string(0,100,VOC);//ͼ�μӺ���  �ֿ���ʾ
//
//		display_GB2312_string(2,5,"CO2:");
//		display_GB2312_string(2,38 ,CO2);//ͼ�μӺ���  �ֿ���ʾ
//
//		display_GB2312_string(2,75,"CO:");
//		display_GB2312_string(2,98,CO);//ͼ�μӺ���  �ֿ���ʾ
//
//		display_GB2312_string(4,10,"�¶�:");
//		display_GB2312_string(4,50,Temp1);//ͼ�μӺ���  �ֿ���ʾ
//
//		display_GB2312_string(6,10,"ʪ��:");
//		display_GB2312_string(6,50,Humi1);//ͼ�μӺ���  �ֿ���ʾ
//	//	display_GB2312_string(4,50,ans);
////		Delay(2);//��ʱ400MS��X��
//	//	display_graphic_16x16(4,2,0,ans);	
////	//~~~~~~~~~~~~~~~~~~~~~@~~~~~~~~&~~~~~~~~~~@~~~~~~~~~~~~ROM����/�ַ�ͼ��	
////
////
////		
////	//~~~~~~~~~~~~~~~~~~~~~@~~~~~~~~&~~~~~~~~~~@~~~~~~~~~~~~9�����Խ�����ͼ��		
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
//////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~������ɨ����ʾ	
////		
////		display_GB2312_string(0,1,"��ӭʹ�û澧ģ��"); //�ڵ�1 ҳ����1 �У���ʾһ��16x16 �����ֻ�8x16 ��ASCII ��
////		display_GB2312_string(2,1,"������ʾ���庺��"); //��ʾһ��16x16 �����ֻ�8x16 ��ASCII ��.������ͬ
////		display_GB2312_string(4,1,"������ʾͨ���ַ�");
////		display_GB2312_string(6,1,"���������Ա�ͼ��");
////		Delay(1);
//////		Fill_RAM(0x00);			// Clear Screen
////		display_GB2312_string(0,1,"�澧�Ƽ�רҵ����");
////		display_GB2312_string(2,1,"�з�����Һ��ģ��");
////		display_GB2312_string(4,1,"��ַ���ڱ���ʯ��");
////		display_GB2312_string(6,1,"���鲼�ڶ�ҵ����");
////		Delay(1);
////		Fill_RAM(0x00);			// Clear Screen
////		display_GB2312_string(0,1,"ͼ�ε�����ʾģ��");
////		display_GB2312_string(2,1,"�����ֿ���ʾģ��");
////		display_GB2312_string(4,1,"�Ա຺�ַ�����ʾ");
////		display_GB2312_string(6,1,"���壬���磺 ");
////		display_graphic_16x16(0,6,96,hui1); //�ڵ�7ҳ����81����ʾ�����Ա���Ƨ���֡��桱
////		display_graphic_16x16(0,6,112,jing1); //��ʾ�����Ա���Ƨ����"����
////		Delay(1);
////		Fill_RAM(0x00);			// Clear Screen
////
////		display_GB2312_string(0,1,"<!@#$%^&*()_-+]/"); //�ڵ�1 ҳ����1 �У���ʾһ��16x16 �����ֻ�8*16 ��ASCII ��
////		display_string_5x8(2,1,"<!@#$%^&*()_-+]/;.,?[");//�ڵ�3 ҳ����1 �У���ʾһ��5x8 �����ASCII ��
////		display_string_5x8(3,1,"H J electronics Co., ");//��ʾһ��5x8 �����ASCII ��
////		display_string_5x8(4,1,"Ltd. established at ");//��ʾһ��5x8 �����ASCII ��
////		display_string_5x8(5,1,"year 2010.Focus LCM. ");//��ʾһ��5x8 �����ASCII ��
////		display_string_5x8(6,1,"TEL:0755-23146001 ");//��ʾһ��5x8 �����ASCII ��
////		display_string_5x8(7,1,"FAX:0755-61901158 ");//��ʾһ��5x8 �����ASCII ��
////		
////		Delay(1);
////		Fill_RAM(0x00);			// Clear Screen
////		display_GB2312_string(0,1,"����������������"); //�ڵ�1 ҳ����1 �У���ʾһ��16x16 �����ֻ�8x16 ��ASCII ��
////		display_GB2312_string(2,1,"����������������"); //��ʾһ��16x16 �����ֻ�8x16 ��ASCII ��.������ͬ
////		display_GB2312_string(4,1,"����������������");
////		display_GB2312_string(6,1,"����������������");
////		Delay(1);
////		Fill_RAM(0x00);			// Clear Screen
////		display_GB2312_string(0,1,"����������������");
////		display_GB2312_string(2,1,"����������������");
////		display_GB2312_string(4,1,"����������������");
////		display_GB2312_string(6,1,"����������������");
////		Delay(1);
////		Fill_RAM(0x00);			// Clear Screen
////
////	}
///*	
//for(m1=0;m1<2;m1++)		//���¹�����ʾ
//	{
////11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111	
//	// Show Pattern - Huijing OLED
//		Show_Pattern(&Huijing[0][0],0x02,0x05,XLevel+0x28,0x30);//�����ݣ�ҳ�ף�ҳδ�����ף��п�
//		Delay(1);//��ʱ400MS��X��
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~��ʾͼƬ��ʾ��˾��־
//	// Fade In/Out (Full Screen)
//		Fade_Out();//����������ʾ
//		Fade_In();//����ʾ������
//		Fade_Out();//����������ʾ
//		Fade_In();//����ʾ������
//		Delay(1);
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~�Աȶȱ仯��ʾ
//	// Scrolling (Partial or Full Screen)
//		Vertical_Scroll(0x00,0x00,Max_Row,0x01,0x60);//����/�¹��������У�β�У����У����̣�
//						// Upward - Full Screen
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~���Ϲ�����ʾ						
//		Delay(1);
//		Vertical_Scroll(0x01,0x00,Max_Row,0x01,0x60);//����/�¹��������У�β�У����У����̣�
//						// Downward - Full Screen
//		Delay(1);
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~���¹�����ʾ				
//		Deactivate_Scroll();//�����е�ַ
//		//����ֱˮƽ����/�ң�����ˮƽ��������ҳ��βҳ�����У�β�У����򶥵㣬����ף����������������������ʱ��
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
//		Deactivate_Scroll();//�����е�ַ
////~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~������ʾ	
//		Fill_RAM(0x00);			// Clear Screen
//		
//	}	
//*/
////	Fill_RAM(0x00);			// Clear Screen
////	Set_Contrast_Control(0xff);	// Set SEG Output Current   		���öԱȶ�	
////	display_graphic_16x16(0,0,0,hui1); 	
////	display_graphic_16x16(0,2,0,jing1); 
////
////	Continuous_Scroll(0x00,0x00,0x00,0x00,XLevel,XLevel+Max_Column-1,0x00,0x20,0x1F,0x00,0x01);
//						// Downward - Top Area
//
//}
//
////unsigned char  hui1[]={/*--  ����:  ��  --*/
/////*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
////0x40,0x70,0x7C,0xFE,0xFC,0xF0,0xE0,0xF0,0x7C,0x7E,0x4E,0x7E,0x78,0x60,0xE0,0x40,
////0x60,0x66,0x27,0x27,0x35,0x34,0x76,0x72,0x7A,0x5E,0x4E,0x5A,0xFA,0xF2,0xC2,0x00,
////}
//
////unsigned char  jing1[]={/*--  ����:  ��  --*/
/////*--  ����12;  �������¶�Ӧ�ĵ���Ϊ����x��=16x16   --*/
////0x00,0x00,0x00,0x7C,0x7C,0x54,0x54,0x54,0x54,0x54,0x7C,0x7C,0x7C,0x00,0x00,0x00,
////0x00,0x7F,0x7F,0x29,0x29,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x29,0x29,0x7F,0x7F,0x00,
////}
//
////unsigned char  Huijing[][48]={			// Huijing OLED
//////
/////*--  ���x�߶�=48x32  --*/
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
