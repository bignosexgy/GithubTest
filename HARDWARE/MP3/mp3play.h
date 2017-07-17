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
 * MP3文件大体分为3部分:TAG_V2(ID3V2),Frame,TAG_V1(ID3V1)。要注意
 * 有些MP3文件尾是没有ID3V1 tag的，有的是MP3文件头部的ID3V2 tag。
 * 其中ID3V1 存放在MP3文件的末尾 ，用16进制的编辑器打开一个MP3文件，
 * 查看其末尾的128个顺序存放字节，数据结构定义如下：
 */

 /*
  *1、每帧的播放时间：无论帧长是多少，每帧的播放时间是26ms。
	*2、FrameSize = ( (( MPEGVersion == MPEG1 ? 144 : 72 ) * Bitrate) / SampleRate ) + PaddingBit 
	*/
typedef struct tagID3V1
{
	char Header[3];	     // 标签头必须是"TAG"，否则被认为没有标签头
	char Title[30];	     // 标题
	char Artist[30];		 // 作者
	char Album[30];			 // 专辑
	char Year[4];				 // 出品年代
	char Comment[28];		 // 备注
	char reserve;				 // 保留
	char track;					 // 音轨
	char Genre;					 // 风格
}ID3V1, *pID3V1;


__packed typedef struct 
{							  
	u8 mvol;		//主音量,范围:0~254
	u8 bflimit;		//低音频率限定,范围:2~15(单位:10Hz)
	u8 bass;		//低音,范围:0~15.0表示关闭.(单位:1dB)
	u8 tflimit;		//高音频率限定,范围:1~15(单位:Khz)
	u8 treble;		//高音,范围:0~15(单位:1.5dB)(原本范围是:-8~7,通过函数修改了);
	u8 effect;		//空间效果设置.0,关闭;1,最小;2,中等;3,最大.

	u8 saveflag; 	//保存标志,0X0A,保存过了;其他,还从未保存	   
}_vs10xx_obj;
										   
int Read_ID3V1(FIL *FileObject, struct tag_info *info);
void Read_ID3V2(FIL *FileObject, struct tag_info *info);
void MP3_Play(void);


u16  VS_RD_Reg(u8 address);				//读寄存器
u16  VS_WRAM_Read(u16 addr);	    	//读RAM
void VS_WR_Data(u8 data);				//写数据
void VS_WR_Cmd(u8 address,u16 data);	//写命令
u8   VS_HD_Reset(void);			    	//硬复位
void VS_Soft_Reset(void);           	//软复位
u16 VS_Ram_Test(void);             		//RAM测试	    
void VS_Sine_Test(void);            	//正弦测试
													 
u8 	 VS_SPI_ReadWriteByte(u8 data);
void VS_SPI_SpeedLow(void);
void VS_SPI_SpeedHigh(void);
void VS_Init(void);						//初始化VS10XX	 
void VS_Set_Speed(u8 t);				//设置播放速度
u16  VS_Get_HeadInfo(void);     		//得到比特率
u32 VS_Get_ByteRate(void);				//得到字节速率
u16 VS_Get_EndFillByte(void);			//得到填充字节
u8 	 VS_Send_MusicData(u8* buf);		//向VS10XX发送32字节
void VS_Restart_Play(void);				//重新开始下一首歌播放	  
void VS_Reset_DecodeTime(void);			//重设解码时间
u16  VS_Get_DecodeTime(void);   		//得到解码时间

void VS_Load_Patch(u16 *patch,u16 len);	//加载用户patch
u8 	 VS_Get_Spec(u16 *p);       		//得到分析数据	   
void VS_Set_Bands(u16 *buf,u8 bands);	//设置中心频率
void VS_Set_Vol(u8 volx);				//设置主音量   
void VS_Set_Bass(u8 bfreq,u8 bass,u8 tfreq,u8 treble);//设置高低音
void VS_Set_Effect(u8 eft);				//设置音效
void VS_Set_All(void);
extern u8 SPI1_ReadWriteByte(u8 TxData);
extern void SPI1_SetSpeed(u8 SpeedSet);
void vs10xx_read_para(_vs10xx_obj * vs10xxdev);
void vs10xx_save_para(_vs10xx_obj * vs10xxdev);


#endif /* __MP3PLAY_H */

