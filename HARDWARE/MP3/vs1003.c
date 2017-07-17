/******************** (C) COPYRIGHT 2012 WildFire Team ************************** 
 * 文件名  ：vs1003.c
 * 描述    ：VS1003B(音频解码芯片)应用函数库。
 *          
 * 实验平台：野火STM32开发板
 * 硬件连接： ------------------------------
 *           | PB13-SPI2_SCK : VS1003B-SCLK |
 *           | PB14-SPI2_MISO: VS1003B-SO	|
 *           | PB15-SPI2_MOSI: VS1003B-SI	|
 *           | PB12-SPI2_NSS : VS1003B-XCS	|
 *           | PB11          : VS1003B-XRET	|
 *           | PC6           : VS1003B-XDCS	|
 *           | PC7           : VS1003B-DREQ	|
 *            ------------------------------
 * 库版本  ：ST3.5.0
 *
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/
#include "vs1003.h"
//#include "SysTick.h"			/* 延时函数头文件 */
#include "delay.h"

/*
 * 函数名：VS1003_SPI_Init
 * 描述  ：VS1003所用I/O初始化
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
 void SPI1_Init(void)
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
	SPI1->CR1|=1<<1; //CPOL=0时空闲模式下SCK为1  CPOL=1
	SPI1->CR1|=1<<0; //数据采样从第二个时间边沿开始,CPHA=1  
	  
	SPI1->CR1|=7<<3; //Fsck=Fcpu/256
	SPI1->CR1|=0<<7; //MSBfirst   
	SPI1->CR1|=1<<6; //SPI设备使能
 	SPI1_ReadWriteByte(0xff);//启动传输	 
}
 void VS_Init(void)
{
	RCC->APB2ENR|=1<<4;       //PORTC时钟使能 
	RCC->APB2ENR|=1<<6;       //PORTE时钟使能 
	RCC->APB2ENR|=1<<7;       //PORTF时钟使能 
	  
	GPIOC->CRH&=0XFF0FFFFF;//PC13
	GPIOC->CRH|=0X00800000;
	GPIOC->ODR|=1<<13;

	GPIOE->CRL&=0XF0FFFFFF;//PE6
	GPIOE->CRL|=0X03000000;
	GPIOE->ODR|=1<<6;

	GPIOF->CRL&=0X00FFFFFF;//PF6,PF7
	GPIOF->CRL|=0X33000000;
	GPIOF->ODR|=3<<6;
	SPI1_Init();	 
}
 
void VS1003_SPI_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;



    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE);

  //  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
   //   RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1, ENABLE ) ;
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); 
	  
//		
//  /* 配置 SPI2 引脚: PB13-SCK, PB14-MISO 和 PB15-MOSI */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
//  GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
// 
//  /* PB12-XCS(片选) */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_Init(GPIOA, &GPIO_InitStructure); 

    //////下面是SPI相关GPIO初始化//////
    //Configure SPI1 pins: SCK,  MOSI 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);  
		 	 //PB.9 输出高 
	
	//MISO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//Configure PA4 pin: SD_CS pin 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);   
 	GPIO_SetBits(GPIOA,GPIO_Pin_4); 
	/* PB11-XRST(复位) */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;      
  GPIO_Init(GPIOD, &GPIO_InitStructure);


  /* PC6-XDCS(数据命令选择) */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;    
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* PC7-DREQ(数据中断) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			/* 设置为上拉输入 */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;	 
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 
//  /* SPI2 configuration */ 
//  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
//  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
//  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
//  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
//  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
//  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//  SPI_InitStructure.SPI_CRCPolynomial = 7;
//  SPI_Init(SPI1, &SPI_InitStructure);


    /* SPI2 configuration */ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;				  	
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;			  	
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//SPI_CPOL_Low;				  		
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; 
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				  
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  /* Enable SPI1  */
  SPI_Cmd(SPI1, ENABLE);  									  	 
}

/*
 * 函数名：VS1003_WriteByte
 * 描述  ：以SPI方式向VS1003写入一个字节
 * 输入  ：-byte 
 * 输出  ：无
 * 调用  ：内/外部调用
 */
u8 VS1003_WriteByte( u8 byte )
{
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);		// 判断SPI1 发送缓冲区是否空  
  SPI_I2S_SendData(SPI1, byte);											                // 发送8位数据
//  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);	  // 判断是否接收缓冲区非空
//  return SPI_I2S_ReceiveData(SPI1);
} 

/*
 * 函数名：VS1003_ReadByte
 * 描述  ：以SPI方式从VS1003读出一个字节
 * 输入  ：无 
 * 输出  ：-u8
 * 调用  ：内/外部调用
 */	
u8 VS1003_ReadByte(void)
{
   while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);  	 // 判断SPI1 发送缓冲区是否空  
   SPI_I2S_SendData(SPI1, 0);										                     // 发送一个空字节	   
   while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);	 // 判断是否接收缓冲区非空
   return SPI_I2S_ReceiveData(SPI1);								                 // 返回接收到的数据
}

/*
 * 函数名：Mp3WriteRegister
 * 描述  ：写VS1003寄存器
 * 输入  ：-addressbyte 寄存器地址
 *         -highbyte  高8位
 *         -lowbyte   低8位 
 * 输出  ：无
 * 调用  ：内/外部调用
 */
void Mp3WriteRegister(u8 addressbyte, u8 highbyte, u8 lowbyte)
{
	TXDCS_SET(1);  
	TCS_SET(0);      

	VS1003_WriteByte( VS_WRITE_COMMAND );
	VS1003_WriteByte( addressbyte );
	VS1003_WriteByte( highbyte );
	VS1003_WriteByte( lowbyte );

	TCS_SET(1);     
}	

/*
 * 函数名：Mp3ReadRegister
 * 描述  ：读VS1003寄存器
 * 输入  ：-addressbyte 寄存器地址 
 * 输出  ：-u16
 * 调用  ：内/外部调用
 */
u16 Mp3ReadRegister(u8 addressbyte)
{
	u16 resultvalue = 0;
	TXDCS_SET(1);          
	TCS_SET(0);              

	VS1003_WriteByte(VS_READ_COMMAND); 		                   //发送读寄存器命令
	VS1003_WriteByte((addressbyte));				                 //发送寄存器的地址
	resultvalue = (unsigned int )(VS1003_ReadByte() << 8);   //读取高8位数据
	resultvalue |= VS1003_ReadByte();  		                   //读取低8位数据

	TCS_SET(1);                      
	return resultvalue;                                      //返回16位寄存器的值
}

/*
 * 函数名：MP3_Start
 * 描述  ：使MP3进入就绪模式，随时准备播放音乐。
 * 输入  ：无 
 * 输出  ：无
 * 调用  ：外部调用
 */
 
void MP3_Start(void)
{
	u8 BassEnhanceValue = 0x00;					      // 低音值先初始化为0
	u8 TrebleEnhanceValue = 0x00;				      // 高音值先初始化为0
	TRST_SET(0);          					 
	delay_us( 1000 );                         // 1000*10us = 10ms
						
	VS1003_WriteByte(0xff);                   // 发送一个字节的无效数据，启动SPI传输
	TXDCS_SET(1);         					
	TCS_SET(1);           				
	TRST_SET(1);          			 
	delay_us( 1000 );             	

 	Mp3WriteRegister( SPI_MODE,0x08,0x00);  	// 进入VS1003的播放模式
	Mp3WriteRegister(3, 0x65, 0x54);   		    // 设置vs1003的时钟,3倍频
//	Mp3WriteRegister(4, 0x65, 0x54);
	Mp3WriteRegister(5, 0xda, 0xc0);   		    // 采样率48k，立体声
	Mp3WriteRegister(SPI_BASS, TrebleEnhanceValue, BassEnhanceValue);// 设置重低音
	Mp3WriteRegister(0x0b,0x10,0x10);      	                         // VS1003 音量
	delay_us( 1000 );

	while( DREQ == 0 );						   	        // 等待DREQ为高  表示能够接受音乐数据输入


}

/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
													  
