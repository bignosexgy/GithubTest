#ifndef __BASIC_H
#define __BASIC_H	 
#include "stm32f10x.h"
//环境变量声明
//u8	FlagKeyInt;     //按键触发标志位
//u8  FlagRtcInt;     //时钟秒中断标志位
//u8 	FlagBuzzer;
//u8	FlagShine;
//u8	FlagLight;
//u8  FlagShineSetTime1;
//u8  FlagShineSetTime2;
//u8 	FlagShineWave;
//u8 	FlagShineTime;
//u8  FlagShineTimePosi;
//u8	FlagShineInterval;
//u8	FlagShineIntervalBak;
//u8  FlagShineInstruction;
//u8  FlagShineSetT;
//u8  FlagShineSetR;
//u8  FlagShinePause;
//u8	PlayStatus;		//播放/暂停状态标识
//u8	PosiValue;

//0~9 数字显示字符
//定时显示
//u8  DISPLAYSETIME[10] = {0xd7,0x06,0xe3,0xa7,0x36,0xb5,0xf5,0x07,0xf7,0xb7};
//波形显示
//u8  DISPLAYWAVE[10] = {0xbe,0x06,0x7c,0x5e,0xc6,0xda,0xfa,0x0e,0xfe,0xde};
//时间显示
//u8  DISPLAYTIME[10] = {0x5f,0x50,0x6b,0x79,0x74,0x3d,0x3f,0x58,0x7f,0x7d};
//计时显示
//u8  DISPLAYTIMEMETER[10] = {0xfa,0x60,0xd6,0xf4,0x6c,0xbc,0xbe,0xe0,0xfe,0xfc};




typedef struct{
	u8 	SeTimeBegin_H;		//设置时间小时高位
	u8	SeTimeBegin_L;		//设置时间小时地位
	u8 	SeTimeEnd_H;			//设置时间秒高位
	u8  SeTimeEnd_L;			//设置时间秒低位
	u8  TimeHour_H;				//时间显示小时高位
	u8 	TimeHour_L;				//时间显示小时低位
	u8	TimeMin_H;				//时间显示分钟高位
	u8	TimeMin_L;				//时间显示分钟低位
	u8	TimeMeterMin_H;		//计时显示分钟高位
	u8	TimeMeterMin_L;		//计时显示分钟低位
	u8 	TimeMeterSec_H;		//计时显示秒高位
	u8	TimeMeterSec_L;		//计时显示秒低位
	u8	Wave;							//波形显示
	u8	Figure_H;					//图形显示高位
	u8	Figure_L;					//图形显示低位
	u8	IconVolume;					//图标显示音量（音量，蜂鸣器开/关）
} DefStrDisplay;

//DefStrDisplay PubStrDisplay; //定时器信息 

//u8 DisplayBuff[16];			//显示缓冲


//声音参数
#define INCREASE	0x15
#define DECREASE	0x26

//u8	VolumeSignal;
//u8	VolumeSignalBak;

#define RFON	0x80
#define RFOFF	0x40
#define VALUME	0x20
#define VALUME1	0x10
#define VALUME2	0x01
#define VALUME3	0x02
#define VALUME4	0x04
#define VALUME5	0x08

//设备状态
#define IDLE		0x12	//播放
#define PLAY		0x23	//播放
//#define PAUSE		0x34	//停机
#define SETT		0x45	//设置时间
#define SETR		0x56	//设置遥控
#define SETTIME		0x67	//设置播放时间
#define STOP		0x78	//停机
#define INTER   	0x89	//播放间隔
#define WAVE		0x90


//u8 WaveValue;
//u8 WaveTime;
//u8 WaveTimeNum;
//u8 WaveTimeStart;
//u8 WaveTimeStop;

#define WAVE1	0x01
#define WAVE2	0x02

#define WAVETIME1	0x01
#define WAVETIME2	0x02
#define WAVETIME3	0x03

//定义波形时间
#define WAVE1BEGIN		5
#define INTERVAL		2
#define WAVE1END		5

#define WAVE2BEGIN		10
#define WAVE2END		10

#define WAVE3BEGIN		15
#define WAVE3END		15



#define KEYW1    	0x01  		//波形1
#define KEYW2    	0x02  		//波形2
#define KEYOK  		0x03 		//播放/暂停
#define KEYSTOP  	0x04  		//停止
#define KEYUP		0x05		//上翻
#define KEYDOWN 	0x06		//下翻
#define KEYSET		0x07		//设置时间及遥控配对
#define KEYTIME		0x08		//定时选择

//结构：各个系统定时器计数变量
typedef struct {
	u8  u8interval;
	u8  u8buzzer; //蜂鸣器
	u8  u8second;
	u8	u8light;
	u8  u8reversal;
	u8	u8syncounter; 
	u16 u16meter;
} DefStrTimer;

//DefStrTimer     PubstrTimer; //定时器信息 
//u8 	PubFlagSecond;
//u8 	PubFlagFinished;
#define		SYSTEMINT	50	//20ms终端一次，20x25=500ms
#define		MINUTE		60	//1分钟

//u8 	SetHour;
//u8  SetMin;

#endif
