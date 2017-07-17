/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  ��mp3play.c
 * ����    ��mp3Ӧ�ú�����         
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ�-----------------
 *          |                 |
 *          |       ��        |
 *          |                 |
 *           -----------------
 * ��汾  ��ST3.5.0
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/
#include "mp3play.h"
#include "delay.h"
#include "spi.h"
#include "gpio.h"
uint8_t readBuf[READBUF_SIZE];		     /* ���ڴ洢MP3֡ͷ��Ϣ */
struct tag_info id3v1;					 /* ID3V1�ṹ����������ڴ�����ĸ�����Ϣ */
struct tag_info id3v2;					 /* ID3V2�ṹ����������ڴ�����ĸ�����Ϣ */
 u8 mm=0;

 void VS_Load_Patch(u16 *patch,u16 len) 
{
	u16 i; 
	u16 addr, n, val; 	  			   
	for(i=0;i<len;) 
	{ 
		addr = patch[i++]; 
		n    = patch[i++]; 
		if(n & 0x8000U) //RLE run, replicate n samples 
		{ 
			n  &= 0x7FFF; 
			val = patch[i++]; 
			while(n--)VS_WR_Cmd(addr, val);  
		}else //copy run, copy n sample 
		{ 
			while(n--) 
			{ 
				val = patch[i++]; 
				VS_WR_Cmd(addr, val); 
			} 
		} 
	} 	
}
/*
 * ��������MP3_Play
 * ����  ����ȡSD���������Ƶ�ļ�����ͨ���������ų���
 *         ֧�ֵĸ�ʽ��mp3,mid,wma, ���ֵ�wav
 * ����  ����
 * ���  ����
 * ˵��  �������֧�ֳ������ļ���
 */
void MP3_Play(void)
{  
//	FATFS fs;                  // Work area (file system object) for logical drive
	FRESULT res;
	UINT br;					  /*��ȡ�����ֽ����������ж��Ƿ񵽴��ļ�β*/
	FIL fsrc;          		 // file objects
//	FILINFO finfo;			 /*�ļ���Ϣ*/
//	DIR dirs;
	uint16_t count = 0;
	
//	char lfn[70];   	  /*Ϊ֧�ֳ��ļ������飬[]���֧��255*/
	char j = 0;	  
//	char path[100] = {""}; /* MicroSD����Ŀ¼ */	
//	char *result1, *result2, *result3, *result4;
	
	BYTE buffer[4096];          /* ��Ŷ�ȡ�����ļ����� */
	
//	finfo.lfname = lfn; 		  /*Ϊ���ļ�������ռ�*/
//	finfo.lfsize = sizeof(lfn);   /*�ռ��С*/
	
//	f_mount(0, &fs);			                         /* �����ļ�ϵͳ��0�� */  

 	                res = f_open(&fsrc, "0:/��ӭ����.mp3",  FA_READ); /* ���ļ� */
 //					res = f_open( &fsrc, finfo.lfname, FA_OPEN_EXISTING | FA_READ );
					res = f_lseek(&fsrc, 0);
					
								
					br = 1;				  			     /* br Ϊȫ�ֱ��� */
					TXDCS_SET( 0 );            /* ѡ��VS1003�����ݽӿ�	*/
					/* ------------------- һ����ʼ --------------------*/
					printf( " \r\n ��ʼ���� \r\n" ); 
					sound_C=1;
				//	VS_SPI_SpeedHigh();
					for (;;) 
					{			
						res = f_read( &fsrc, buffer, sizeof(buffer), &br );
						if ( res == 0 )
						{				
							count = 0;								/* 512�ֽ������¼��� */
							
							delay_us( 1000 );         /* 10ms ��ʱ */ 
							while ( count < 4096)		  /* SD����ȡһ��sector��һ��sectorΪ512�ֽ� */	
							{ 						
								if ( DREQ != 0 )	      /* �ȴ�DREQΪ�ߣ������������� */
								{ 
								  VS_XDCS=0;  
								for (j=0; j<32; j++ ) /* VS1003��FIFOֻ��32���ֽڵĻ��� */
									{										
										VS1003_WriteByte( buffer[count] );										
										count++;
									}

								  	VS_XDCS=1; 
								}		    	      	
							}
						}
						if (res || br == 0) break;   /* ������ߵ���MP3�ļ�β */		   
					}
									/* ------------------- һ������ --------------------*/ 
					count = 0;
					/* ����VS1003��Ҫ����һ���������跢��2048��0��ȷ����һ�׵��������� */
					while ( count < 2048 )	
					{	
						if ( DREQ != 0 )
						{
							for ( j=0; j<32; j++ )
							{					   			
								VS1003_WriteByte( 0 );								
								count++;
							}
						}
					}
					count = 0;

				  sound_C=0;

				 	count = 0;
					TXDCS_SET( 1 );   /* �ر�VS1003���ݶ˿� */ 
					f_close(&fsrc);	 /* �رմ򿪵��ļ� */


					 res = f_open(&fsrc, "0:/��ӭ����111.mp3", FA_OPEN_EXISTING | FA_READ); /* ���ļ� */
 //					res = f_open( &fsrc, finfo.lfname, FA_OPEN_EXISTING | FA_READ );
					res = f_lseek(&fsrc, 0);
					
								
					br = 1;				  			     /* br Ϊȫ�ֱ��� */
					TXDCS_SET( 0 );            /* ѡ��VS1003�����ݽӿ�	*/
					/* ------------------- һ����ʼ --------------------*/
					printf( " \r\n ��ʼ���� \r\n" ); 
					sound_C=1;
					for (;;) 
					{			
						res = f_read( &fsrc, buffer, sizeof(buffer), &br );
						if ( res == 0 )
						{				
							count = 0;								/* 512�ֽ������¼��� */
							
							delay_us( 1000 );         /* 10ms ��ʱ */ 
							while ( count < 4096)		  /* SD����ȡһ��sector��һ��sectorΪ512�ֽ� */	
							{ 						
								if ( DREQ != 0 )	      /* �ȴ�DREQΪ�ߣ������������� */
								{
								for (j=0; j<32; j++ ) /* VS1003��FIFOֻ��32���ֽڵĻ��� */
									{										
										VS1003_WriteByte( buffer[count] );										
										count++;
									}
								}		    	      	
							}
						}
						if (res || br == 0) break;   /* ������ߵ���MP3�ļ�β */		   
					}
										count = 0;
					/* ����VS1003��Ҫ����һ���������跢��2048��0��ȷ����һ�׵��������� */
					while ( count < 2048 )	
					{	
						if ( DREQ != 0 )
						{
							for ( j=0; j<32; j++ )
							{					   			
								VS1003_WriteByte( 0 );								
								count++;
							}
						}
					}
					count = 0;

				  sound_C=0;

				 	count = 0;
					TXDCS_SET( 1 );   /* �ر�VS1003���ݶ˿� */ 
					f_close(&fsrc);	 /* �رմ򿪵��ļ� */

//	
//	if (f_opendir(&dirs,path) == FR_OK) 	          /* �򿪸�Ŀ¼ */
//	{
//		while (f_readdir(&dirs, &finfo) == FR_OK)    	/* ���ζ�ȡ�ļ��� */
//		{			
//		//	if ( finfo.fattrib & AM_ARC )        /* �ж��Ƿ�Ϊ�浵���ĵ� */
//		//	if ( finfo.fattrib  )
//		//	{
//				if(finfo.lfname[0] == NULL && finfo.fname !=NULL)  /*�����ļ�����Ϊ�գ����ļ����ǿ�ʱת��*/
//				finfo.lfname =finfo.fname;				
//				
//				if( !finfo.lfname[0] )	 /* �ļ���Ϊ�ռ�������Ŀ¼��ĩβ���˳� */
//				break;  
//			//	   ����Ѹһ˿����.mp3
//				printf(  " \r\n �ļ���Ϊ: %s \r\n",finfo.lfname );
//				
//				result1 = strstr( finfo.lfname, ".mp3" );	/* �ж��Ƿ�Ϊ��Ƶ�ļ� */ 		
//				result2 = strstr( finfo.lfname, ".mid" );
//				result3 = strstr( finfo.lfname, ".wav" );
//				result4 = strstr( finfo.lfname, ".wma" );				
//				
//				if ( result1!=NULL || result2!=NULL || result3!=NULL || result4!=NULL )
//				{					
//					if(result1 != NULL)/*����mp3�ļ����ȡmp3����Ϣ*/
//         //	if (1 )
////				{					
////					if(1)/*����mp3�ļ����ȡmp3����Ϣ*/
//					{
//						res = f_open( &fsrc, finfo.lfname, FA_OPEN_EXISTING | FA_READ ); /* ��ֻ����ʽ�� */					
//						
//						
//						/* ��ȡ������Ϣ(ID3V1 tag / ID3V2 tag) */					
//						if ( Read_ID3V1(&fsrc, &id3v1) == TRUE )
//						{// ID3V1 tag
//							printf( "\r\n ��Ŀ    ��%s \r\n", id3v1.title );
//							printf( "\r\n ������  ��%s \r\n", id3v1.artist );														
//							printf( "\r\n ר��    ��%s \r\n", id3v1.album ); 				
//						}
//						else
//						{// ��ЩMP3�ļ�û��ID3V1 tag,ֻ��ID3V2 tag						
//							res = f_lseek(&fsrc, 0);
//							Read_ID3V2(&fsrc, &id3v2);
//							
//							printf( "\r\n ��Ŀ    ��%s \r\n", id3v2.title );
//							printf( "\r\n ������  ��%s \r\n", id3v2.artist );               	   	
//						}  
//					}
//					/* ʹ�ļ�ָ�� fsrc ����ָ���ļ�ͷ����Ϊ�ڵ���Read_ID3V1/Read_ID3V2ʱ��
//					fsrc ��λ�øı��� */
//					res = f_open( &fsrc, finfo.lfname, FA_OPEN_EXISTING | FA_READ );
//					res = f_lseek(&fsrc, 0);
//					
//								
//					br = 1;				  			     /* br Ϊȫ�ֱ��� */
//					TXDCS_SET( 0 );            /* ѡ��VS1003�����ݽӿ�	*/
//					/* ------------------- һ����ʼ --------------------*/
//					printf( " \r\n ��ʼ���� \r\n" ); 
//					for (;;) 
//					{			
//						res = f_read( &fsrc, buffer, sizeof(buffer), &br );
//						if ( res == 0 )
//						{				
//							count = 0;								/* 512�ֽ������¼��� */
//							
//							delay_us( 1000 );         /* 10ms ��ʱ */ 
//							while ( count < 512)		  /* SD����ȡһ��sector��һ��sectorΪ512�ֽ� */	
//							{ 						
//								if ( DREQ != 0 )	      /* �ȴ�DREQΪ�ߣ������������� */
//								{
//								for (j=0; j<32; j++ ) /* VS1003��FIFOֻ��32���ֽڵĻ��� */
//									{										
//										VS1003_WriteByte( buffer[count] );										
//										count++;
//									}
//								}		    	      	
//							}
//						}
//						if (res || br == 0) break;   /* ������ߵ���MP3�ļ�β */		   
//					}
//					printf( " \r\n ���Ž��� \r\n" );
//					/* ------------------- һ������ --------------------*/ 
//					count = 0;
//					/* ����VS1003��Ҫ����һ���������跢��2048��0��ȷ����һ�׵��������� */
//					while ( count < 2048 )	
//					{	
//						if ( DREQ != 0 )
//						{
//							for ( j=0; j<32; j++ )
//							{					   			
//								VS1003_WriteByte( 0 );								
//								count++;
//							}
//						}
//					}
//					count = 0;
//					TXDCS_SET( 1 );   /* �ر�VS1003���ݶ˿� */ 
//					f_close(&fsrc);	 /* �رմ򿪵��ļ� */
//				}   		                      
//			}
//	//	} /* while (f_readdir(&dirs, &finfo) == FR_OK) */  
//	} /* if (f_opendir(&dirs, path) == FR_OK)  */	  

   mm=1;

} /* end of MP3_Play */



/*
 * ��������Read_ID3V1
 * ����  ����MP3�ļ�β��ȡID3V1����Ϣ��������Щ��Ϣ������һ��ȫ��������        
 * ����  ��- FileObject: file system
 *         - info: struct tag_info
 * ���  ����
 * ˵��  �����ڵ�mp3�����϶�û��ID3V1 tag(���ļ�β��)���е���ID3V2 tag(���ļ�ͷ��)��
 */
int Read_ID3V1(FIL *FileObject, struct tag_info *info)
{	
	UINT n_Read;
	uint8_t readBuf[128];	  
	f_lseek(FileObject,FileObject->fsize - 128 ); /* ID3V1����Ϣ�������ļ�ĩβ��128��byte�� */
	f_read(FileObject, &readBuf, 128 , &n_Read);	/* ��ID3V1�е�128��byte����Ϣ���������� */    
	
	if (strncmp("TAG", (char *) readBuf, 3) == 0)	      /* ID3V1�ı�ǩͷ�����ǡ�TAG����������Ϊû�б�ǩ */
	{				
		strncpy(info->title, (char *) readBuf + 3, MIN(30, sizeof(info->title) - 1)); 	    /* ���� */
		strncpy(info->artist,(char *) readBuf + 3 + 30, MIN(30, sizeof(info->artist) - 1));	/* ���� */
		strncpy(info->album,(char *) readBuf + 3 + 30 + 30, MIN(30, sizeof(info->album) - 1));	/* ר�� */
		strncpy(info->year,(char *) readBuf + 3 + 30 + 30 + 30, MIN(4, sizeof(info->year) - 1));	/* ʱ�� */
		return TRUE;
	}
	else 
	{ 
		return FALSE; 
	}		
}

/*
 * ��������Read_ID3V2
 * ����  ����MP3�ļ�ͷ����ȡID3V2����Ϣ��������Щ��Ϣ������һ��ȫ��������
 * ����  ��- FileObject: file system
 *         - info: struct tag_info
 * ���  ����
 * ˵��  ��MP3�ļ�����ID3V2�����������ߣ���Ŀ��ר������Ϣ�����Ȳ��̶�����չ��ID3V1����Ϣ����
 */
void Read_ID3V2(FIL *FileObject, struct tag_info *info)
{
	UINT n_Read;  		
	uint32_t p = 0;	 
	f_lseek(FileObject,0); /* ��λ���ļ���ͷ�� */
	f_read(FileObject, readBuf, sizeof(readBuf) , &n_Read); /* ���ļ�ͷ����ʼ��ȡREADBUF_SIZE���ֽ� */
	
	if (strncmp("ID3", (char *) readBuf, 3) == 0)	/* ID3V3��ͷ����ǩ����Ϊ��ID3�� */
	{
		uint32_t tag_size, frame_size, i;
		uint8_t version_major;
		int frame_header_size;
		
		/* ��ǩ��С��������ǩͷ��10���ֽں����б�ǩ֡�Ĵ�С */
		tag_size = ((uint32_t) readBuf[6] << 21) | ((uint32_t) readBuf[7] << 14) | ((uint16_t) readBuf[8] << 7) | readBuf[9];
		
		info->data_start = tag_size;
		version_major = readBuf[3];   /* �汾��ΪID3V2.3ʱ�ͼ�¼Ϊ3 */
		
		if (version_major >= 3)
		{
			frame_header_size = 10;		/* ID3V2.3�ı�ǩͷΪ10���ֽ� */
		}
		else
		{
			frame_header_size = 6;
		}
		i = p = 10;
		
		/* iterate through frames */
		while (p < tag_size)				 /* �ӱ�ǩͷ��ʼ���� */
		{
			if (version_major >= 3)
			{
				frame_size = ((uint32_t) readBuf[i + 4] << 24) | ((uint32_t) readBuf[i + 5] << 16) | ((uint16_t) readBuf[i + 6] << 8) | readBuf[i + 7];
			}
			else
			{
				frame_size = ((uint32_t) readBuf[i + 3] << 14) | ((uint16_t) readBuf[i + 4] << 7) | readBuf[i + 5];
			}
			if (i + frame_size + frame_header_size + frame_header_size >= sizeof(readBuf))
			{
				if (frame_size + frame_header_size > sizeof(readBuf))
				{
					f_lseek(FileObject,FileObject->fptr + p + frame_size + frame_header_size); 
					f_read(FileObject, &readBuf, READBUF_SIZE , &n_Read);
					p += frame_size + frame_header_size;
					i = 0;
					continue;
				}
				else
				{
					int r = sizeof(readBuf) - i;
					memmove(readBuf, readBuf + i, r);
					f_read(FileObject, (char *) readBuf + r, i , &n_Read);
					i = 0;
				}
			}
			
			/* ֡��ʶ"TT2"/"TIT2"��ʾ����Ϊ���׸�� ���� */
			if (strncmp("TT2", (char *) readBuf + i, 3) == 0 || strncmp("TIT2", (char *) readBuf + i, 4) == 0)
			{
				strncpy(info->title, (char *) readBuf + i + frame_header_size + 1, MIN(frame_size - 1, sizeof(info->title) - 1));
				if( ( info->title[0] == 0xFE && info->title[1] == 0xFF ) || ( info->title[0] == 0xFF && info->title[1] == 0xFE ) )
				{
					/* unicode ��ʽ*/
					memset(info->title,0,sizeof(info->title));
					printf( "-- MP3 title no support unicode \r\n");			
				}       
			}
			
			/* ֡��ʶ"TP1"/"TPE1"��ʾ����Ϊ���׸�� ���� */
			else if (strncmp("TP1", (char *) readBuf + i, 3) == 0 || strncmp("TPE1", (char *) readBuf + i, 4) == 0)
			{
				strncpy(info->artist, (char *) readBuf + i + frame_header_size + 1, MIN(frame_size - 1, sizeof(info->artist) - 1));
				
				if( ( info->artist[0] == 0xFE && info->artist[1] == 0xFF ) || ( info->artist[0] == 0xFF && info->artist[1] == 0xFE ) )
				{
					/* unicode ��ʽ*/
					memset(info->artist,0,sizeof(info->artist));
					printf( "-- MP3 artist no support unicode \r\n");
				}
			}
			
			p += frame_size + frame_header_size;
			i += frame_size + frame_header_size;
		}
	}
}


//VS10XXĬ�����ò���
_vs10xx_obj vsset=
{
	220,	//����:220
	6,		//�������� 60Hz
	15,		//�������� 15dB	
	10,		//�������� 10Khz	
	15,		//�������� 10.5dB
	0,		//�ռ�Ч��	
};

////////////////////////////////////////////////////////////////////////////////
//��ֲʱ��Ľӿ�
//data:Ҫд�������
//����ֵ:����������
u8 VS_SPI_ReadWriteByte(u8 data)
{			  	 
	return SPI1_ReadWriteByte(data);	  
}
//SD����ʼ����ʱ��,��Ҫ����
void VS_SPI_SpeedLow(void)
{								 
	SPI1_SetSpeed(SPI_SPEED_32);//���õ�����ģʽ 
}
//SD������������ʱ��,���Ը�����
void VS_SPI_SpeedHigh(void)
{						  
	SPI1_SetSpeed(SPI_SPEED_8);//���õ�����ģʽ		 
}
//��ʼ��VS10XX��IO��	 
//void VS_Init(void)
//{
//	RCC->APB2ENR|=1<<4;       //PORTCʱ��ʹ�� 
//	RCC->APB2ENR|=1<<6;       //PORTEʱ��ʹ�� 
//	RCC->APB2ENR|=1<<7;       //PORTFʱ��ʹ�� 
//	  
//	GPIOC->CRH&=0XFF0FFFFF;//PC13
//	GPIOC->CRH|=0X00800000;
//	GPIOC->ODR|=1<<13;
//
//	GPIOE->CRL&=0XF0FFFFFF;//PE6
//	GPIOE->CRL|=0X03000000;
//	GPIOE->ODR|=1<<6;
//
//	GPIOF->CRL&=0X00FFFFFF;//PF6,PF7
//	GPIOF->CRL|=0X33000000;
//	GPIOF->ODR|=3<<6;
//	SPI1_Init();	 
//}	  
////////////////////////////////////////////////////////////////////////////////	 	  
//��λVS10XX
u16 aaa= 0x0804	;
void VS_Soft_Reset(void)
{	 
	u8 retry=0;  				   
	while(VS_DQ==0); //�ȴ������λ����	   
	VS_SPI_ReadWriteByte(0Xff);//��������
	retry=0;
	while(VS_RD_Reg(SPI_MODE)!=0x0800)// �����λ,��ģʽ  
	{
	//	VS_WR_Cmd(0x00,0x0804);// �����λ,��ģʽ
	VS_WR_Cmd(SPI_MODE,aaa);// �����λ,��ģʽ	    
		delay_ms(2);//�ȴ�����1.35ms 
		if(retry++>100)break; 	    
	}	 		 
	while(VS_DQ==0);//�ȴ������λ����	 
	retry=0;
	while(VS_RD_Reg(SPI_CLOCKF)!=0X9800)//����VS10XX��ʱ��,3��Ƶ ,1.5xADD 
	{
		VS_WR_Cmd(SPI_CLOCKF,0X9800);//����VS10XX��ʱ��,3��Ƶ ,1.5xADD
		if(retry++>100)break; 	    
	}		    										    
	delay_ms(20);
} 
//Ӳ��λMP3
//����1:��λʧ��;0:��λ�ɹ�	   
u8 VS_HD_Reset(void)
{
	u8 retry=0;
	VS_RST=0;
	delay_ms(20);
	VS_XDCS=1;//ȡ�����ݴ���
	VS_XCS=1;//ȡ�����ݴ���
	VS_RST=1;	   
	while(VS_DQ==0&&retry<200)//�ȴ�DREQΪ��
	{
		retry++;
		delay_us(50);
	};
	delay_ms(20);	
	if(retry>=200)return 1;
	else return 0;	    		 
}
//���Ҳ��� 
void VS_Sine_Test(void)
{											    
	VS_HD_Reset();
		VS_WR_Cmd(0x0b,0X0101);	 	 
//	VS_WR_Cmd(0x0b,0X2020);	  //��������	 
 	VS_WR_Cmd(SPI_MODE,0x0820);//����VS10XX�Ĳ���ģʽ     
	while(VS_DQ==0);     //�ȴ�DREQΪ��
	//printf("mode sin:%x\n",VS_RD_Reg(SPI_MODE));
 	//��VS10XX�������Ҳ������0x53 0xef 0x6e n 0x00 0x00 0x00 0x00
 	//����n = 0x24, �趨VS10XX�����������Ҳ���Ƶ��ֵ��������㷽����VS10XX��datasheet
  	VS_SPI_SpeedLow();//���� 
	VS_XDCS=0;//ѡ�����ݴ���
	VS_SPI_ReadWriteByte(0x53);
	VS_SPI_ReadWriteByte(0xef);
	VS_SPI_ReadWriteByte(0x6e);
	VS_SPI_ReadWriteByte(0x24);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	delay_ms(100);
	VS_XDCS=1; 
    //�˳����Ҳ���
    VS_XDCS=0;//ѡ�����ݴ���
	VS_SPI_ReadWriteByte(0x45);
	VS_SPI_ReadWriteByte(0x78);
	VS_SPI_ReadWriteByte(0x69);
	VS_SPI_ReadWriteByte(0x74);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	delay_ms(100);
	VS_XDCS=1;		 

    //�ٴν������Ҳ��Բ�����nֵΪ0x44���������Ҳ���Ƶ������Ϊ�����ֵ
    VS_XDCS=0;//ѡ�����ݴ���      
	VS_SPI_ReadWriteByte(0x53);
	VS_SPI_ReadWriteByte(0xef);
	VS_SPI_ReadWriteByte(0x6e);
	VS_SPI_ReadWriteByte(0x44);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	delay_ms(100);
 	VS_XDCS=1;
    //�˳����Ҳ���
    VS_XDCS=0;//ѡ�����ݴ���
	VS_SPI_ReadWriteByte(0x45);
	VS_SPI_ReadWriteByte(0x78);
	VS_SPI_ReadWriteByte(0x69);
	VS_SPI_ReadWriteByte(0x74);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	delay_ms(100);
	VS_XDCS=1;	 
}	 
//ram ���� 
//����ֵ:RAM���Խ��
// VS1003����õ���ֵΪ0x807F����������;VS1053Ϊ0X83FF.																				 
u16 VS_Ram_Test(void)
{ 
	VS_HD_Reset();     
 	VS_WR_Cmd(SPI_MODE,0x0820);// ����VS10XX�Ĳ���ģʽ
	while (VS_DQ==0); // �ȴ�DREQΪ��			   
 	VS_SPI_SpeedLow();//���� 
	VS_XDCS=0;	       		    // xDCS = 1��ѡ��VS10XX�����ݽӿ�
	VS_SPI_ReadWriteByte(0x4d);
	VS_SPI_ReadWriteByte(0xea);
	VS_SPI_ReadWriteByte(0x6d);
	VS_SPI_ReadWriteByte(0x54);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	VS_SPI_ReadWriteByte(0x00);
	delay_ms(150);  
	VS_XDCS=1;
	return VS_RD_Reg(SPI_HDAT0);// VS1003����õ���ֵΪ0x807F����������;VS1053Ϊ0X83FF.;       
}     					   
//��VS10XXд����
//address:�����ַ
//data:��������
void VS_WR_Cmd(u8 address,u16 data)
{  
	while(VS_DQ==0);//�ȴ�����		  
	VS_SPI_SpeedLow();//���� 	   
	VS_XDCS=1; 	 
	VS_XCS=0; 	 
	VS_SPI_ReadWriteByte(VS_WRITE_COMMAND);//����VS10XX��д����
	VS_SPI_ReadWriteByte(address); //��ַ
	VS_SPI_ReadWriteByte(data>>8); //���͸߰�λ
	VS_SPI_ReadWriteByte(data);	 //�ڰ�λ
	VS_XCS=1;           
	VS_SPI_SpeedHigh();//����	   
} 
//��VS10XXд����
//data:Ҫд�������
void VS_WR_Data(u8 data)
{
	VS_SPI_SpeedHigh();//����,��VS1003B,���ֵ���ܳ���36.864/4Mhz����������Ϊ9M 
	VS_XDCS=0;   
	VS_SPI_ReadWriteByte(data);
	VS_XDCS=1;      
}         
//��VS10XX�ļĴ���           
//address���Ĵ�����ַ
//����ֵ��������ֵ
//ע�ⲻҪ�ñ��ٶ�ȡ,�����
u16 VS_RD_Reg(u8 address)
{ 
	u16 temp=0;    	 
    while(VS_DQ==0);//�ǵȴ�����״̬ 		  
	VS_SPI_SpeedLow();//���� 
	VS_XDCS=1;       
	VS_XCS=0;        
	VS_SPI_ReadWriteByte(VS_READ_COMMAND);	//����VS10XX�Ķ�����
	VS_SPI_ReadWriteByte(address);       	//��ַ
	temp=VS_SPI_ReadWriteByte(0xff); 		//��ȡ���ֽ�
	temp=temp<<8;
	temp+=VS_SPI_ReadWriteByte(0xff); 		//��ȡ���ֽ�
	VS_XCS=1;     
	VS_SPI_SpeedHigh();//����	  
   return temp; 
}  
//��ȡVS10xx��RAM
//addr��RAM��ַ
//����ֵ��������ֵ
u16 VS_WRAM_Read(u16 addr) 
{ 
	u16 res;			   	  
 	VS_WR_Cmd(SPI_WRAMADDR, addr); 
	res=VS_RD_Reg(SPI_WRAM);  
 	return res;
} 
//���ò����ٶȣ���VS1053��Ч�� 
//t:0,1,�����ٶ�;2,2���ٶ�;3,3���ٶ�;4,4����;�Դ�����
void VS_Set_Speed(u8 t)
{
	VS_WR_Cmd(SPI_WRAMADDR,0X1E04);	//�ٶȿ��Ƶ�ַ  
	while(VS_DQ==0); 				//�ȴ�����	   
	VS_WR_Cmd(SPI_WRAM,t); 			//д�벥���ٶ�
}
//FOR WAV HEAD0 :0X7761 HEAD1:0X7665    
//FOR MIDI HEAD0 :other info HEAD1:0X4D54
//FOR WMA HEAD0 :data speed HEAD1:0X574D
//FOR MP3 HEAD0 :data speed HEAD1:ID
//������Ԥ��ֵ,�ײ�III
const u16 bitrate[2][16]=
{ 
{0,8,16,24,32,40,48,56,64,80,96,112,128,144,160,0}, 
{0,32,40,48,56,64,80,96,112,128,160,192,224,256,320,0}
};
//����Kbps�Ĵ�С
//����ֵ���õ�������
u16 VS_Get_HeadInfo(void)
{
	unsigned int HEAD0;
	unsigned int HEAD1;  
 	HEAD0=VS_RD_Reg(SPI_HDAT0); 
    HEAD1=VS_RD_Reg(SPI_HDAT1);
  	//printf("(H0,H1):%x,%x\n",HEAD0,HEAD1);
    switch(HEAD1)
    {        
        case 0x7665://WAV��ʽ
        case 0X4D54://MIDI��ʽ 
		case 0X4154://AAC_ADTS
		case 0X4144://AAC_ADIF
		case 0X4D34://AAC_MP4/M4A
		case 0X4F67://OGG
        case 0X574D://WMA��ʽ
		case 0X664C://FLAC��ʽ
        {
			////printf("HEAD0:%d\n",HEAD0);
            HEAD1=HEAD0*2/25;//�൱��*8/100
            if((HEAD1%10)>5)return HEAD1/10+1;//��С�����һλ��������
            else return HEAD1/10;
        }
        default://MP3��ʽ,�����˽ײ�III�ı�
        {
            HEAD1>>=3;
            HEAD1=HEAD1&0x03; 
            if(HEAD1==3)HEAD1=1;
            else HEAD1=0;
            return bitrate[HEAD1][HEAD0>>12];
        }
    }  
}
//�õ�ƽ���ֽ���
//����ֵ��ƽ���ֽ����ٶ�
u32 VS_Get_ByteRate(void)
{
	return VS_WRAM_Read(0X1E05);//ƽ��λ��
}
//�õ���Ҫ��������
//����ֵ:��Ҫ��������
u16 VS_Get_EndFillByte(void)
{
	return VS_WRAM_Read(0X1E06);//����ֽ�
}  
//����һ����Ƶ����
//�̶�Ϊ32�ֽ�
//����ֵ:0,���ͳɹ�
//		 1,VS10xx��ȱ����,��������δ�ɹ�����    
u8 VS_Send_MusicData(u8* buf)
{
	u8 n;
	if(VS_DQ!=0)  //�����ݸ�VS10XX
	{			   	 
		VS_XDCS=0;  
        for(n=0;n<32;n++)
		{
			VS_SPI_ReadWriteByte(buf[n]);	 			
		}
		VS_XDCS=1;     				   
	}else return 1;
	return 0;//�ɹ�������
}
//�и�
//ͨ���˺����и裬��������л���������				
void VS_Restart_Play(void)
{
	u16 temp;
	u16 i;
	u8 n;	  
	u8 vsbuf[32];
	for(n=0;n<32;n++)vsbuf[n]=0;//����
	temp=VS_RD_Reg(SPI_MODE);	//��ȡSPI_MODE������
	temp|=1<<3;					//����SM_CANCELλ
	temp|=1<<2;					//����SM_LAYER12λ,������MP1,MP2
	VS_WR_Cmd(SPI_MODE,temp);	//����ȡ����ǰ����ָ��
	for(i=0;i<2048;)			//����2048��0,�ڼ��ȡSM_CANCELλ.���Ϊ0,���ʾ�Ѿ�ȡ���˵�ǰ����
	{
		if(VS_Send_MusicData(vsbuf)==0)//ÿ����32���ֽں���һ��
		{
			i+=32;						//������32���ֽ�
   			temp=VS_RD_Reg(SPI_MODE);	//��ȡSPI_MODE������
 			if((temp&(1<<3))==0)break;	//�ɹ�ȡ����
		}	
	}
	if(i<2048)//SM_CANCEL����
	{
		temp=VS_Get_EndFillByte()&0xff;//��ȡ����ֽ�
		for(n=0;n<32;n++)vsbuf[n]=temp;//����ֽڷ�������
		for(i=0;i<2052;)
		{
			if(VS_Send_MusicData(vsbuf)==0)i+=32;//���	  
		}   	
	}else VS_Soft_Reset();  	//SM_CANCEL���ɹ�,�����,��Ҫ��λ 	  
	temp=VS_RD_Reg(SPI_HDAT0); 
    temp+=VS_RD_Reg(SPI_HDAT1);
	if(temp)					//��λ,����û�гɹ�ȡ��,��ɱ���,Ӳ��λ
	{
		VS_HD_Reset();		   	//Ӳ��λ
		VS_Soft_Reset();  		//��λ 
	} 
}
//�������ʱ��                          
void VS_Reset_DecodeTime(void)
{
	VS_WR_Cmd(SPI_DECODE_TIME,0x0000);
	VS_WR_Cmd(SPI_DECODE_TIME,0x0000);//��������
}
//�õ�mp3�Ĳ���ʱ��n sec
//����ֵ������ʱ��
u16 VS_Get_DecodeTime(void)
{ 		
	u16 dt=0;	 
	dt=VS_RD_Reg(SPI_DECODE_TIME);      
 	return dt;
} 	    					  
//vs10xxװ��patch.
//patch��patch�׵�ַ
//len��patch����
//void VS_Load_Patch(u16 *patch,u16 len) 
//{
//	u16 i; 
//	u16 addr, n, val; 	  			   
//	for(i=0;i<len;) 
//	{ 
//		addr = patch[i++]; 
//		n    = patch[i++]; 
//		if(n & 0x8000U) //RLE run, replicate n samples 
//		{ 
//			n  &= 0x7FFF; 
//			val = patch[i++]; 
//			while(n--)VS_WR_Cmd(addr, val);  
//		}else //copy run, copy n sample 
//		{ 
//			while(n--) 
//			{ 
//				val = patch[i++]; 
//				VS_WR_Cmd(addr, val); 
//			} 
//		} 
//	} 	
//} 		  	  
//�趨VS10XX���ŵ������͸ߵ���
//volx:������С(0~254)
void VS_Set_Vol(u8 volx)
{
    u16 volt=0; 			//�ݴ�����ֵ
    volt=254-volx;			//ȡ��һ��,�õ����ֵ,��ʾ���ı�ʾ 
	volt<<=8;
    volt+=254-volx;			//�õ��������ú��С
    VS_WR_Cmd(SPI_VOL,volt);//������ 
}
//�趨�ߵ�������
//bfreq:��Ƶ����Ƶ��	2~15(��λ:10Hz)
//bass:��Ƶ����			0~15(��λ:1dB)
//tfreq:��Ƶ����Ƶ�� 	1~15(��λ:Khz)
//treble:��Ƶ����  	 	0~15(��λ:1.5dB,С��9��ʱ��Ϊ����)
void VS_Set_Bass(u8 bfreq,u8 bass,u8 tfreq,u8 treble)
{
    u16 bass_set=0; //�ݴ������Ĵ���ֵ
    signed char temp=0;   	 
	if(treble==0)temp=0;	   		//�任
	else if(treble>8)temp=treble-8;
 	else temp=treble-9;  
	bass_set=temp&0X0F;				//�����趨
	bass_set<<=4;
	bass_set+=tfreq&0xf;			//��������Ƶ��
	bass_set<<=4;
	bass_set+=bass&0xf;				//�����趨
	bass_set<<=4;
	bass_set+=bfreq&0xf;			//��������    
	VS_WR_Cmd(SPI_BASS,bass_set);	//BASS 
}
//�趨��Ч
//eft:0,�ر�;1,��С;2,�е�;3,���.
void VS_Set_Effect(u8 eft)
{
	u16 temp;	 
	temp=VS_RD_Reg(SPI_MODE);	//��ȡSPI_MODE������
	if(eft&0X01)temp|=1<<4;		//�趨LO
	else temp&=~(1<<5);			//ȡ��LO
	if(eft&0X02)temp|=1<<7;		//�趨HO
	else temp&=~(1<<7);			//ȡ��HO						   
	VS_WR_Cmd(SPI_MODE,temp);	//�趨ģʽ    
}	  
///////////////////////////////////////////////////////////////////////////////
//��������,��Ч��.
void VS_Set_All(void) 				
{			 
	VS_Set_Vol(vsset.mvol);
	VS_Set_Bass(vsset.bflimit,vsset.bass,vsset.tflimit,vsset.treble);  
	VS_Set_Effect(vsset.effect);
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
