/******************** (C) COPYRIGHT 2012 WildFire Team ************************** 
 * �ļ���  ��vs1003.c
 * ����    ��VS1003B(��Ƶ����оƬ)Ӧ�ú����⡣
 *          
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ� ------------------------------
 *           | PB13-SPI2_SCK : VS1003B-SCLK |
 *           | PB14-SPI2_MISO: VS1003B-SO	|
 *           | PB15-SPI2_MOSI: VS1003B-SI	|
 *           | PB12-SPI2_NSS : VS1003B-XCS	|
 *           | PB11          : VS1003B-XRET	|
 *           | PC6           : VS1003B-XDCS	|
 *           | PC7           : VS1003B-DREQ	|
 *            ------------------------------
 * ��汾  ��ST3.5.0
 *
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/
#include "vs1003.h"
//#include "SysTick.h"			/* ��ʱ����ͷ�ļ� */
#include "delay.h"

/*
 * ��������VS1003_SPI_Init
 * ����  ��VS1003����I/O��ʼ��
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
 void SPI1_Init(void)
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
	SPI1->CR1|=1<<1; //CPOL=0ʱ����ģʽ��SCKΪ1  CPOL=1
	SPI1->CR1|=1<<0; //���ݲ����ӵڶ���ʱ����ؿ�ʼ,CPHA=1  
	  
	SPI1->CR1|=7<<3; //Fsck=Fcpu/256
	SPI1->CR1|=0<<7; //MSBfirst   
	SPI1->CR1|=1<<6; //SPI�豸ʹ��
 	SPI1_ReadWriteByte(0xff);//��������	 
}
 void VS_Init(void)
{
	RCC->APB2ENR|=1<<4;       //PORTCʱ��ʹ�� 
	RCC->APB2ENR|=1<<6;       //PORTEʱ��ʹ�� 
	RCC->APB2ENR|=1<<7;       //PORTFʱ��ʹ�� 
	  
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
//  /* ���� SPI2 ����: PB13-SCK, PB14-MISO �� PB15-MOSI */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
//  GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
// 
//  /* PB12-XCS(Ƭѡ) */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//  GPIO_Init(GPIOA, &GPIO_InitStructure); 

    //////������SPI���GPIO��ʼ��//////
    //Configure SPI1 pins: SCK,  MOSI 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);  
		 	 //PB.9 ����� 
	
	//MISO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//Configure PA4 pin: SD_CS pin 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	//�������
	GPIO_Init(GPIOA,&GPIO_InitStructure);   
 	GPIO_SetBits(GPIOA,GPIO_Pin_4); 
	/* PB11-XRST(��λ) */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;      
  GPIO_Init(GPIOD, &GPIO_InitStructure);


  /* PC6-XDCS(��������ѡ��) */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;    
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* PC7-DREQ(�����ж�) */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			/* ����Ϊ�������� */
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
 * ��������VS1003_WriteByte
 * ����  ����SPI��ʽ��VS1003д��һ���ֽ�
 * ����  ��-byte 
 * ���  ����
 * ����  ����/�ⲿ����
 */
u8 VS1003_WriteByte( u8 byte )
{
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);		// �ж�SPI1 ���ͻ������Ƿ��  
  SPI_I2S_SendData(SPI1, byte);											                // ����8λ����
//  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);	  // �ж��Ƿ���ջ������ǿ�
//  return SPI_I2S_ReceiveData(SPI1);
} 

/*
 * ��������VS1003_ReadByte
 * ����  ����SPI��ʽ��VS1003����һ���ֽ�
 * ����  ���� 
 * ���  ��-u8
 * ����  ����/�ⲿ����
 */	
u8 VS1003_ReadByte(void)
{
   while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);  	 // �ж�SPI1 ���ͻ������Ƿ��  
   SPI_I2S_SendData(SPI1, 0);										                     // ����һ�����ֽ�	   
   while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);	 // �ж��Ƿ���ջ������ǿ�
   return SPI_I2S_ReceiveData(SPI1);								                 // ���ؽ��յ�������
}

/*
 * ��������Mp3WriteRegister
 * ����  ��дVS1003�Ĵ���
 * ����  ��-addressbyte �Ĵ�����ַ
 *         -highbyte  ��8λ
 *         -lowbyte   ��8λ 
 * ���  ����
 * ����  ����/�ⲿ����
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
 * ��������Mp3ReadRegister
 * ����  ����VS1003�Ĵ���
 * ����  ��-addressbyte �Ĵ�����ַ 
 * ���  ��-u16
 * ����  ����/�ⲿ����
 */
u16 Mp3ReadRegister(u8 addressbyte)
{
	u16 resultvalue = 0;
	TXDCS_SET(1);          
	TCS_SET(0);              

	VS1003_WriteByte(VS_READ_COMMAND); 		                   //���Ͷ��Ĵ�������
	VS1003_WriteByte((addressbyte));				                 //���ͼĴ����ĵ�ַ
	resultvalue = (unsigned int )(VS1003_ReadByte() << 8);   //��ȡ��8λ����
	resultvalue |= VS1003_ReadByte();  		                   //��ȡ��8λ����

	TCS_SET(1);                      
	return resultvalue;                                      //����16λ�Ĵ�����ֵ
}

/*
 * ��������MP3_Start
 * ����  ��ʹMP3�������ģʽ����ʱ׼���������֡�
 * ����  ���� 
 * ���  ����
 * ����  ���ⲿ����
 */
 
void MP3_Start(void)
{
	u8 BassEnhanceValue = 0x00;					      // ����ֵ�ȳ�ʼ��Ϊ0
	u8 TrebleEnhanceValue = 0x00;				      // ����ֵ�ȳ�ʼ��Ϊ0
	TRST_SET(0);          					 
	delay_us( 1000 );                         // 1000*10us = 10ms
						
	VS1003_WriteByte(0xff);                   // ����һ���ֽڵ���Ч���ݣ�����SPI����
	TXDCS_SET(1);         					
	TCS_SET(1);           				
	TRST_SET(1);          			 
	delay_us( 1000 );             	

 	Mp3WriteRegister( SPI_MODE,0x08,0x00);  	// ����VS1003�Ĳ���ģʽ
	Mp3WriteRegister(3, 0x65, 0x54);   		    // ����vs1003��ʱ��,3��Ƶ
//	Mp3WriteRegister(4, 0x65, 0x54);
	Mp3WriteRegister(5, 0xda, 0xc0);   		    // ������48k��������
	Mp3WriteRegister(SPI_BASS, TrebleEnhanceValue, BassEnhanceValue);// �����ص���
	Mp3WriteRegister(0x0b,0x10,0x10);      	                         // VS1003 ����
	delay_us( 1000 );

	while( DREQ == 0 );						   	        // �ȴ�DREQΪ��  ��ʾ�ܹ�����������������


}

/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
													  
