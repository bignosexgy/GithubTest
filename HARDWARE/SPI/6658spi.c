#include "6658spi.h" 
#include "delay.h" 

//注意：以下数据类型和3个子函数，客户根据自己的硬件情况自己配置和实现

//#define  T_UInt8		unsigned char				//无符号单字节
//#define  T_UInt16		unsigned int				//无符号2字节整型
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
 * 功能  : 延时参数中给定的毫秒数
 * 参数1 : 需延时的毫秒数
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
 * 功能  : 检测状态R/B引脚的电平值确定芯片忙闲状态
 * 返回值: 1：R/B引脚为高电平，表示SYN6658芯片为Busy
 *		  0：R/B引脚为低电平，表示SYN6658芯片为Ready
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
 * 功能  : 传输1个字节
 * 参数1 : 发送的本字节值
 * 返回值: 接收的字节值
 ******************************************************************/
T_UInt8 SYN6658SpiSendOneByte( T_UInt8 cmd)
{
		register int i;

		SPISCK = 0;
	    SPISEL = 0;
		
		SYN6658SpiDelayMs(3);

		//写命令
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
 * 功能  :  发送1个命令帧数据
 * 参数1 :  命令帧的地址
 * 参数2 :  命令帧的总长度（按字节计数）
 * 返回值:  0x41：表示下位机接收成功
 *		    0x45：表示失败（通讯异常或准备命令帧有错）
 ******************************************************************/
T_UInt8  SYN6658SpiSendFrame(T_UInt8 *buffOfFrame, T_UInt16 nFrameLen)
{
	T_UInt8  nReceiveData;
	T_UInt8  nCircleTimes;
	T_UInt16 i;
	T_UInt8  nBuffStopAddEnd[5]={0xFD,0x00,0x01,0x02,0xFF};
  	
	//循环确保SYN6658芯片为Ready状态（即R/B引脚为低电平）
	nCircleTimes = 0;
	while( SYN6658SpiCheckBusy() == 1 )		
	{
		SYN6658SpiSendOneByte(0xA5);	//发送暂停同步字节0xA5，
		SYN6658SpiDelayMs(5); 
		nCircleTimes++;	  
		if( nCircleTimes > 10 )			// 1次成功率在98%以上，多次确保成功
			return 0x45;				//失败：通讯异常
	}

	//若为：合成命令或Standby命令
	if((*(buffOfFrame+3)==0x01 )||(*(buffOfFrame+3)==0x22 || *(buffOfFrame+3)==0x88))
	{
		//循环确保播音停止
		nCircleTimes = 0;
		do
		{	
			if( nCircleTimes != 0)
				SYN6658SpiDelayMs(200);	 	
			
			for(i=0;i<5;i++)		 		//发送Stop命令(含结束字节)
				nReceiveData = SYN6658SpiSendOneByte(*(nBuffStopAddEnd+i));
  
  			nCircleTimes++;				
			if( nCircleTimes >= 4 )			// 1次成功率在99.5%以上，多次确保成功
				break;					//退出
		}while(nReceiveData != 0x41) ;
		SYN6658SpiDelayMs(200);
	}
	//循环确保下位机成功接收命令帧
	nCircleTimes = 0;
	do
	{	
		if( nCircleTimes != 0)
			SYN6658SpiDelayMs(200);

		//发送本命令帧
		for(i=0;i<nFrameLen;i++)
		{
			nReceiveData = SYN6658SpiSendOneByte(*(buffOfFrame+i));	  
			if( nReceiveData == 0x41 || nReceiveData == 0x45)				//
				break;				
		}

		//发送帧结束字节0xFF，并得到下位机返回的值
		if (nReceiveData != 0x41 && nReceiveData != 0x45 )			   		
			nReceiveData = SYN6658SpiSendOneByte(0xFF);

		nCircleTimes++;	   		
		if( nCircleTimes >= 4 )				// 1次成功率在99.5%以上，多次确保成功
			return 0x45;					//失败：通讯异常或准备命令帧有错
	}while(nReceiveData != 0x41) ; 

	return 0x41;							//接收成功
}

//	封装函数调用举例
void syn6658()
{
	T_UInt8 nReturnByte;

	//命令帧：合成"欢迎观看语音合成系统的演示"
	T_UInt8 buffSyn[31] = {0xFD,0x00,0x1C,0x01,0x01,0xBB,0xB6,0xD3,0xAD,0xB9,0xDB,0xBF,
0xB4,0xD3,0xEF,0xD2,0xF4,0xBA,0xCF,0xB3,0xC9,0xCF,0xB5,0xCD,
0xB3,0xB5,0xC4,0xD1,0xDD,0xCA,0xBE};		
	T_UInt8 buffStop[4]     = {0xFD,0x00,0x01,0x02};			//命令帧：停止	
	T_UInt8 buffPause[4]    = {0xFD,0x00,0x01,0x03};			//命令帧：暂停	
	T_UInt8 buffResume[4]  = {0xFD,0x00,0x01,0x04};			//命令帧：恢复	
	T_UInt8 buffStandby[4]  = {0xFD,0x00,0x01,0x22};			//命令帧：Standby	
	T_UInt8 buffWake[4]    = {0xFD,0x00,0x01,0xFF};			//命令帧：唤醒
		
	//合成"欢迎观看语音合成系统的演示"，并等待播音完毕（即R/B引脚为低电平）
	nReturnByte =  SYN6658SpiSendFrame(buffSyn, 31);		
	SYN6658SpiDelayMs(10);
	while(true)					
	{
		if( SYN6658SpiCheckBusy() == 0 )
			break;
	}	
	
	//合成"欢迎观看语音合成系统的演示"，并在播音过程中，执行"暂停""恢复""停止"命令
	nReturnByte = SYN6658SpiSendFrame(buffSyn, 31);			
	SYN6658SpiDelayMs(25000);
	nReturnByte = SYN6658SpiSendFrame(buffPause, 4);
	SYN6658SpiDelayMs(5000);
	nReturnByte = SYN6658SpiSendFrame(buffResume, 4);
	SYN6658SpiDelayMs(5000);
	nReturnByte = SYN6658SpiSendFrame(buffStop, 4);
	SYN6658SpiDelayMs(5000);
	
	//合成"欢迎观看语音合成系统的演示"，并在播音过程中，执行"Standy"命令；然后再"唤醒"；然后再合成
	nReturnByte = SYN6658SpiSendFrame(buffSyn, 31);			
	SYN6658SpiDelayMs(25000);
	nReturnByte = SYN6658SpiSendFrame(buffStandby, 4);
	SYN6658SpiDelayMs(5000);
	nReturnByte = SYN6658SpiSendFrame(buffWake, 4);
	SYN6658SpiDelayMs(5000);
	nReturnByte = SYN6658SpiSendFrame(buffSyn, 4);
	SYN6658SpiDelayMs(5000);		
}
