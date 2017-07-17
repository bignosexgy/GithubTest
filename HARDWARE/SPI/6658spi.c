#include "6658spi.h" 
#include "delay.h" 

//ע�⣺�����������ͺ�3���Ӻ������ͻ������Լ���Ӳ������Լ����ú�ʵ��

//#define  T_UInt8		unsigned char				//�޷��ŵ��ֽ�
//#define  T_UInt16		unsigned int				//�޷���2�ֽ�����
//#define  T_VOID		void
//
//#define true 1
//#define false 0
//
//sbit BUSY = P3^2;
//
//sbit SPISEL = P1^7;
//sbit SPISCK = P1^6;
//sbit SPISDI = P1^5;
//sbit SPISDO = P1^4;
//
//#define TTS_SYNC  0x5A

/******************************************************************
 * ����  : ��ʱ�����и����ĺ�����
 * ����1 : ����ʱ�ĺ�����
 ******************************************************************/
 T_VOID SYN6658SpiDelayMs(T_UInt16 n)
{
	T_UInt8 i=2;

	while(n--)
	{
		while(i--);
	}
}

 /******************************************************************
 * ����  : ���״̬R/B���ŵĵ�ƽֵȷ��оƬæ��״̬
 * ����ֵ: 1��R/B����Ϊ�ߵ�ƽ����ʾSYN6658оƬΪBusy
 *		  0��R/B����Ϊ�͵�ƽ����ʾSYN6658оƬΪReady
 ******************************************************************/
T_UInt8 SYN6658SpiCheckBusy(T_VOID)
{
	if(BUSY == 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/******************************************************************
 * ����  : ����1���ֽ�
 * ����1 : ���͵ı��ֽ�ֵ
 * ����ֵ: ���յ��ֽ�ֵ
 ******************************************************************/
T_UInt8 SYN6658SpiSendOneByte( T_UInt8 cmd)
{
		register int i;

		SPISCK = 0;
	    SPISEL = 0;
		
		SYN6658SpiDelayMs(3);

		//д����
		i = 8;
	
		while(i--)
		{
			if ((cmd & 0x80) == 0x80)
			{ 
				SPISDI = 1;
			}
			else
			{
				SPISDI = 0;
			}
			
			SYN6658SpiDelayMs(3);
			SPISCK = 1;
			cmd = (cmd << 1);
			SYN6658SpiDelayMs(10);
			SPISCK = 0;
			SYN6658SpiDelayMs(3);		
		}
	
		SPISDI = 0;
		SYN6658SpiDelayMs(10);
		SPISEL = 1;
			  
		SYN6658SpiDelayMs(20);

		return 1;
}


/******************************************************************
 * ����  :  ����1������֡����
 * ����1 :  ����֡�ĵ�ַ
 * ����2 :  ����֡���ܳ��ȣ����ֽڼ�����
 * ����ֵ:  0x41����ʾ��λ�����ճɹ�
 *		    0x45����ʾʧ�ܣ�ͨѶ�쳣��׼������֡�д�
 ******************************************************************/
T_UInt8  SYN6658SpiSendFrame(T_UInt8 *buffOfFrame, T_UInt16 nFrameLen)
{
	T_UInt8  nReceiveData;
	T_UInt8  nCircleTimes;
	T_UInt16 i;
	T_UInt8  nBuffStopAddEnd[5]={0xFD,0x00,0x01,0x02,0xFF};
  	
	//ѭ��ȷ��SYN6658оƬΪReady״̬����R/B����Ϊ�͵�ƽ��
	nCircleTimes = 0;
	while( SYN6658SpiCheckBusy() == 1 )		
	{
		SYN6658SpiSendOneByte(0xA5);	//������ͣͬ���ֽ�0xA5��
		SYN6658SpiDelayMs(5); 
		nCircleTimes++;	  
		if( nCircleTimes > 10 )			// 1�γɹ�����98%���ϣ����ȷ���ɹ�
			return 0x45;				//ʧ�ܣ�ͨѶ�쳣
	}

	//��Ϊ���ϳ������Standby����
	if((*(buffOfFrame+3)==0x01 )||(*(buffOfFrame+3)==0x22 || *(buffOfFrame+3)==0x88))
	{
		//ѭ��ȷ������ֹͣ
		nCircleTimes = 0;
		do
		{	
			if( nCircleTimes != 0)
				SYN6658SpiDelayMs(200);	 	
			
			for(i=0;i<5;i++)		 		//����Stop����(�������ֽ�)
				nReceiveData = SYN6658SpiSendOneByte(*(nBuffStopAddEnd+i));
  
  			nCircleTimes++;				
			if( nCircleTimes >= 4 )			// 1�γɹ�����99.5%���ϣ����ȷ���ɹ�
				break;					//�˳�
		}while(nReceiveData != 0x41) ;
		SYN6658SpiDelayMs(200);
	}
	//ѭ��ȷ����λ���ɹ���������֡
	nCircleTimes = 0;
	do
	{	
		if( nCircleTimes != 0)
			SYN6658SpiDelayMs(200);

		//���ͱ�����֡
		for(i=0;i<nFrameLen;i++)
		{
			nReceiveData = SYN6658SpiSendOneByte(*(buffOfFrame+i));	  
			if( nReceiveData == 0x41 || nReceiveData == 0x45)				//
				break;				
		}

		//����֡�����ֽ�0xFF�����õ���λ�����ص�ֵ
		if (nReceiveData != 0x41 && nReceiveData != 0x45 )			   		
			nReceiveData = SYN6658SpiSendOneByte(0xFF);

		nCircleTimes++;	   		
		if( nCircleTimes >= 4 )				// 1�γɹ�����99.5%���ϣ����ȷ���ɹ�
			return 0x45;					//ʧ�ܣ�ͨѶ�쳣��׼������֡�д�
	}while(nReceiveData != 0x41) ; 

	return 0x41;							//���ճɹ�
}

//	��װ�������þ���
void syn6658()
{
	T_UInt8 nReturnByte;

	//����֡���ϳ�"��ӭ�ۿ������ϳ�ϵͳ����ʾ"
	T_UInt8 buffSyn[31] = {0xFD,0x00,0x1C,0x01,0x01,0xBB,0xB6,0xD3,0xAD,0xB9,0xDB,0xBF,
0xB4,0xD3,0xEF,0xD2,0xF4,0xBA,0xCF,0xB3,0xC9,0xCF,0xB5,0xCD,
0xB3,0xB5,0xC4,0xD1,0xDD,0xCA,0xBE};		
	T_UInt8 buffStop[4]     = {0xFD,0x00,0x01,0x02};			//����֡��ֹͣ	
	T_UInt8 buffPause[4]    = {0xFD,0x00,0x01,0x03};			//����֡����ͣ	
	T_UInt8 buffResume[4]  = {0xFD,0x00,0x01,0x04};			//����֡���ָ�	
	T_UInt8 buffStandby[4]  = {0xFD,0x00,0x01,0x22};			//����֡��Standby	
	T_UInt8 buffWake[4]    = {0xFD,0x00,0x01,0xFF};			//����֡������
		
	//�ϳ�"��ӭ�ۿ������ϳ�ϵͳ����ʾ"�����ȴ�������ϣ���R/B����Ϊ�͵�ƽ��
	nReturnByte =  SYN6658SpiSendFrame(buffSyn, 31);		
	SYN6658SpiDelayMs(10);
	while(true)					
	{
		if( SYN6658SpiCheckBusy() == 0 )
			break;
	}	
	
	//�ϳ�"��ӭ�ۿ������ϳ�ϵͳ����ʾ"�����ڲ��������У�ִ��"��ͣ""�ָ�""ֹͣ"����
	nReturnByte = SYN6658SpiSendFrame(buffSyn, 31);			
	SYN6658SpiDelayMs(25000);
	nReturnByte = SYN6658SpiSendFrame(buffPause, 4);
	SYN6658SpiDelayMs(5000);
	nReturnByte = SYN6658SpiSendFrame(buffResume, 4);
	SYN6658SpiDelayMs(5000);
	nReturnByte = SYN6658SpiSendFrame(buffStop, 4);
	SYN6658SpiDelayMs(5000);
	
	//�ϳ�"��ӭ�ۿ������ϳ�ϵͳ����ʾ"�����ڲ��������У�ִ��"Standy"���Ȼ����"����"��Ȼ���ٺϳ�
	nReturnByte = SYN6658SpiSendFrame(buffSyn, 31);			
	SYN6658SpiDelayMs(25000);
	nReturnByte = SYN6658SpiSendFrame(buffStandby, 4);
	SYN6658SpiDelayMs(5000);
	nReturnByte = SYN6658SpiSendFrame(buffWake, 4);
	SYN6658SpiDelayMs(5000);
	nReturnByte = SYN6658SpiSendFrame(buffSyn, 4);
	SYN6658SpiDelayMs(5000);		
}
