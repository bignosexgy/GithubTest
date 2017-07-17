#ifndef __BASIC_H
#define __BASIC_H	 
#include "stm32f10x.h"
//������������
//u8	FlagKeyInt;     //����������־λ
//u8  FlagRtcInt;     //ʱ�����жϱ�־λ
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
//u8	PlayStatus;		//����/��ͣ״̬��ʶ
//u8	PosiValue;

//0~9 ������ʾ�ַ�
//��ʱ��ʾ
//u8  DISPLAYSETIME[10] = {0xd7,0x06,0xe3,0xa7,0x36,0xb5,0xf5,0x07,0xf7,0xb7};
//������ʾ
//u8  DISPLAYWAVE[10] = {0xbe,0x06,0x7c,0x5e,0xc6,0xda,0xfa,0x0e,0xfe,0xde};
//ʱ����ʾ
//u8  DISPLAYTIME[10] = {0x5f,0x50,0x6b,0x79,0x74,0x3d,0x3f,0x58,0x7f,0x7d};
//��ʱ��ʾ
//u8  DISPLAYTIMEMETER[10] = {0xfa,0x60,0xd6,0xf4,0x6c,0xbc,0xbe,0xe0,0xfe,0xfc};




typedef struct{
	u8 	SeTimeBegin_H;		//����ʱ��Сʱ��λ
	u8	SeTimeBegin_L;		//����ʱ��Сʱ��λ
	u8 	SeTimeEnd_H;			//����ʱ�����λ
	u8  SeTimeEnd_L;			//����ʱ�����λ
	u8  TimeHour_H;				//ʱ����ʾСʱ��λ
	u8 	TimeHour_L;				//ʱ����ʾСʱ��λ
	u8	TimeMin_H;				//ʱ����ʾ���Ӹ�λ
	u8	TimeMin_L;				//ʱ����ʾ���ӵ�λ
	u8	TimeMeterMin_H;		//��ʱ��ʾ���Ӹ�λ
	u8	TimeMeterMin_L;		//��ʱ��ʾ���ӵ�λ
	u8 	TimeMeterSec_H;		//��ʱ��ʾ���λ
	u8	TimeMeterSec_L;		//��ʱ��ʾ���λ
	u8	Wave;							//������ʾ
	u8	Figure_H;					//ͼ����ʾ��λ
	u8	Figure_L;					//ͼ����ʾ��λ
	u8	IconVolume;					//ͼ����ʾ��������������������/�أ�
} DefStrDisplay;

//DefStrDisplay PubStrDisplay; //��ʱ����Ϣ 

//u8 DisplayBuff[16];			//��ʾ����


//��������
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

//�豸״̬
#define IDLE		0x12	//����
#define PLAY		0x23	//����
//#define PAUSE		0x34	//ͣ��
#define SETT		0x45	//����ʱ��
#define SETR		0x56	//����ң��
#define SETTIME		0x67	//���ò���ʱ��
#define STOP		0x78	//ͣ��
#define INTER   	0x89	//���ż��
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

//���岨��ʱ��
#define WAVE1BEGIN		5
#define INTERVAL		2
#define WAVE1END		5

#define WAVE2BEGIN		10
#define WAVE2END		10

#define WAVE3BEGIN		15
#define WAVE3END		15



#define KEYW1    	0x01  		//����1
#define KEYW2    	0x02  		//����2
#define KEYOK  		0x03 		//����/��ͣ
#define KEYSTOP  	0x04  		//ֹͣ
#define KEYUP		0x05		//�Ϸ�
#define KEYDOWN 	0x06		//�·�
#define KEYSET		0x07		//����ʱ�估ң�����
#define KEYTIME		0x08		//��ʱѡ��

//�ṹ������ϵͳ��ʱ����������
typedef struct {
	u8  u8interval;
	u8  u8buzzer; //������
	u8  u8second;
	u8	u8light;
	u8  u8reversal;
	u8	u8syncounter; 
	u16 u16meter;
} DefStrTimer;

//DefStrTimer     PubstrTimer; //��ʱ����Ϣ 
//u8 	PubFlagSecond;
//u8 	PubFlagFinished;
#define		SYSTEMINT	50	//20ms�ն�һ�Σ�20x25=500ms
#define		MINUTE		60	//1����

//u8 	SetHour;
//u8  SetMin;

#endif
