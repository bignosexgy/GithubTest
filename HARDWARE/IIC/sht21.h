#ifndef __SHT21_H_
#define __SHT21_H_

#define SHT21ADDR 0x80 
#define ACK 1
#define NACK 0
#define self_flag  1 
#define ABS(a,b)  (((a)-(b))>0?((a)-(b)):((b)-(a))) 


//#if (self_flag == 0)
//void SHT21Init(void);  
//int Sht21Work(unsigned char); 
//#endif  

#if (self_flag == 1)
void SHT21Init(void);
float Sht21Work(u8 order);
int16_t TP_data_lvbo( int16_t data1,int16_t* data_in,int16_t* data_out);
#endif  

#endif
