#ifndef __MP3PLAY_H
#define	__MP3PLAY_H

#include "stm32f10x.h"
#include "usart1.h"
//#include "SysTick.h"
#include "vs1003.h"
#include "delay.h"
//#include "sdio_sdcard.h"
#include "ff.h"
#include "diskio.h"
#include <string.h>
#include <stdio.h>	 

#define MIN(x, y)			((x) < (y)? (x) : (y))
#define READBUF_SIZE  ( 5*1024 )/*5*1024*/

#define TRUE 1
#define FALSE 0

/* music tag information structure */
struct tag_info
{
	char title[40];        // music title               
	char artist[40];        // music artist
	char album[40];         // album        
	char year[8];						// year

	int duration;           // music total duration (second)   
	int position;           // music current position (second) 

	uint8_t  length;        // bits PerSample			       
	uint32_t bit_rate;      // bit rate                       
	uint32_t sampling;      // sampling                     
	uint32_t data_start;    // start position of data        
};



/*
 * MP3�ļ������Ϊ3����:TAG_V2(ID3V2),Frame,TAG_V1(ID3V1)��Ҫע��
 * ��ЩMP3�ļ�β��û��ID3V1 tag�ģ��е���MP3�ļ�ͷ����ID3V2 tag��
 * ����ID3V1 �����MP3�ļ���ĩβ ����16���Ƶı༭����һ��MP3�ļ���
 * �鿴��ĩβ��128��˳�����ֽڣ����ݽṹ�������£�
 */

 /*
  *1��ÿ֡�Ĳ���ʱ�䣺����֡���Ƕ��٣�ÿ֡�Ĳ���ʱ����26ms��
	*2��FrameSize = ( (( MPEGVersion == MPEG1 ? 144 : 72 ) * Bitrate) / SampleRate ) + PaddingBit 
	*/
typedef struct tagID3V1
{
	char Header[3];	     // ��ǩͷ������"TAG"��������Ϊû�б�ǩͷ
	char Title[30];	     // ����
	char Artist[30];		 // ����
	char Album[30];			 // ר��
	char Year[4];				 // ��Ʒ���
	char Comment[28];		 // ��ע
	char reserve;				 // ����
	char track;					 // ����
	char Genre;					 // ���
}ID3V1, *pID3V1;


__packed typedef struct 
{							  
	u8 mvol;		//������,��Χ:0~254
	u8 bflimit;		//����Ƶ���޶�,��Χ:2~15(��λ:10Hz)
	u8 bass;		//����,��Χ:0~15.0��ʾ�ر�.(��λ:1dB)
	u8 tflimit;		//����Ƶ���޶�,��Χ:1~15(��λ:Khz)
	u8 treble;		//����,��Χ:0~15(��λ:1.5dB)(ԭ����Χ��:-8~7,ͨ�������޸���);
	u8 effect;		//�ռ�Ч������.0,�ر�;1,��С;2,�е�;3,���.

	u8 saveflag; 	//�����־,0X0A,�������;����,����δ����	   
}_vs10xx_obj;
										   
int Read_ID3V1(FIL *FileObject, struct tag_info *info);
void Read_ID3V2(FIL *FileObject, struct tag_info *info);
void MP3_Play(void);


u16  VS_RD_Reg(u8 address);				//���Ĵ���
u16  VS_WRAM_Read(u16 addr);	    	//��RAM
void VS_WR_Data(u8 data);				//д����
void VS_WR_Cmd(u8 address,u16 data);	//д����
u8   VS_HD_Reset(void);			    	//Ӳ��λ
void VS_Soft_Reset(void);           	//��λ
u16 VS_Ram_Test(void);             		//RAM����	    
void VS_Sine_Test(void);            	//���Ҳ���
													 
u8 	 VS_SPI_ReadWriteByte(u8 data);
void VS_SPI_SpeedLow(void);
void VS_SPI_SpeedHigh(void);
void VS_Init(void);						//��ʼ��VS10XX	 
void VS_Set_Speed(u8 t);				//���ò����ٶ�
u16  VS_Get_HeadInfo(void);     		//�õ�������
u32 VS_Get_ByteRate(void);				//�õ��ֽ�����
u16 VS_Get_EndFillByte(void);			//�õ�����ֽ�
u8 	 VS_Send_MusicData(u8* buf);		//��VS10XX����32�ֽ�
void VS_Restart_Play(void);				//���¿�ʼ��һ�׸貥��	  
void VS_Reset_DecodeTime(void);			//�������ʱ��
u16  VS_Get_DecodeTime(void);   		//�õ�����ʱ��

void VS_Load_Patch(u16 *patch,u16 len);	//�����û�patch
u8 	 VS_Get_Spec(u16 *p);       		//�õ���������	   
void VS_Set_Bands(u16 *buf,u8 bands);	//��������Ƶ��
void VS_Set_Vol(u8 volx);				//����������   
void VS_Set_Bass(u8 bfreq,u8 bass,u8 tfreq,u8 treble);//���øߵ���
void VS_Set_Effect(u8 eft);				//������Ч
void VS_Set_All(void);
extern u8 SPI1_ReadWriteByte(u8 TxData);
extern void SPI1_SetSpeed(u8 SpeedSet);
void vs10xx_read_para(_vs10xx_obj * vs10xxdev);
void vs10xx_save_para(_vs10xx_obj * vs10xxdev);


#endif /* __MP3PLAY_H */

