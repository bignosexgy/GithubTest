#include "ht1632.h" 
#include "delay.h" 
//1302��
//void SendByte(uchardizhi,shuju)//1302�ֽڷ����ӳ�����ڲ����������֣�����
//{ 
//	  uchari;RST=0; CLK=0;RST=1;//����һ��Ҫ��ʼ��  RST=0;CLK=0;RST=1;//����ʱ���ȶ�
//	  for(i=0;i<8;i++) 
//	  { 
//	   DP=dizhi&0x01;//ȡһλ�����ݿڵ�λ��ǰ
//	   CLK=0;CLK=1;//����һ��������
//	   dizhi=dizhi>>1;
//	   } 
//	   for(i=0;i<8;i++)
//	   { DP=shuju&0x01;//ȡһλ�����ݿ�
//	   CLK=0;CLK=1;//����һ��������
//	   shuju=shuju>>1;
//	   }
//	   RST=0;
//} 
//
//ucharReadByte(uchardizhi)//1302���ֽ��ӳ�����ڲ�����������
//{//���ڲ�������ֵ 
//   uchari,dat=0;RST=0;CLK=0;RST=1;//����һ��Ҫ��ʼ��   RST=0;CLK=0;RST=1;//����ʱ�� ���ȶ� 
//   for(i=0;i<8;i++)
//    {
//	DP=dizhi&0x01;
//	CLK=0;CLK=1;
//	dizhi>>=1;
//	} 
//	for(i=0;i<8;i++)
//	{
//	CLK=1;CLK=0;
//	if(DP)
//       dat|=0x01<<i;//dat=dat|0x01<<i;
//	   }
//     RST=0;return(dat);
//     //������ֵ
//}
         
		 





 
write_command()//д�����ǰ��λ100
{ 
	 ht1632_wr=0;delay(); 
	 ht1632_data=1;delay();//д1
	 ht1632_wr=1;delay();
	  
	  ht1632_wr=0;delay();
	  ht1632_data=0;  delay();//д0
	  ht1632_wr=1;  delay();


	   ht1632_wr=0; delay();
	   ht1632_data=0; delay();//д0
	   ht1632_wr=1;  delay();
} 
   
write_data_command()//д��������ǰ��λ101 
{
    ht1632_wr=0;delay(); 
	ht1632_data=1;delay();//д1
	ht1632_wr=1;delay();

	ht1632_wr=0;delay(); 
	ht1632_data=0;delay();//д0
	ht1632_wr=1;delay();
	
	ht1632_wr=0;delay(); 
	ht1632_data=1;delay();//д1
	ht1632_wr=1;delay();
}
	
write_8bit(unsigned char x)// 
{ 
	 unsigned char i,k;k=0x80;
	  for(i=0;i<8;i++)
	  { 
	   ht1632_wr=0;
	    if((k&x)==0)
		    ht1632_data=0;
		else  
	    	ht1632_data=1;		
		ht1632_wr=1;
		k=k>>1;//��д��λ
	}


        ht1632_wr=0;
        delay();delay();ht1632_wr=1;//�м������ʱ��Ҫ����Ϊ����ھ�λ�Ǧ�
}


void INIT_1632(void)
{
          	ht1632_data=1;		
		ht1632_wr=1;
		ht1632_cs=0;//Ƭѡ//
		
		write_command();//100�����
		write_8bit(C_COMMAND1632_SYSDIS);//		SYSTEMDISABLE 0x00
		write_8bit(C_COMMAND1632_OPTION);//N-MOSOUTPUTAND16COMMONOPTION 0x24
		write_8bit(C_COMMAND1632_MASTER);//		SETMASTERMODE 0x18
		write_8bit(C_COMMAND1632_SYSEN);//		SYSTEMENABLE0 x01
		write_8bit(C_COMMAND1632_LEDON);//		LEDON 0x03
		write_8bit(C_COMMAND1632_BLINKON);//		BLINKOFF 0x08
		write_8bit(C_COMMAND1632_PWMDUTY);//		PWMDUTYSET 0xaf
		ht1632_cs=1;//ȡ��Ƭѡ//cs1=1;//cs2=1;//CS15=1;
}

write_rom_address(unsigned char y)//дROM��ַ
{
	unsigned char i,k;
	k=0x40;//Ӧʱ��Ҫ��ֻ��7λ�������λ0x40=01000000
	for(i=0;i<7;i++)
	{
		ht1632_wr=0;
		if((k&y)==0)
		  ht1632_data=0;
		else
		  ht1632_data=1;
		  ht1632_wr=1;k=k>>1;
	}
}

write_rom_data(unsigned char z)//дROM����
{
	unsigned char i,k;k=0x80;
	for(i=0;i<8;i++)
	{
	ht1632_wr=0;
	if((k&z)==0)
	   ht1632_data=0;
	 else
	   ht1632_data=1;
	ht1632_wr=1;k=k>>1;
	}
}


//write_lianxu_data(unsignedchar*pp)//д������Ϊָ����
//{
//unsignedchari;
//for(i=0;i<16;i++)//д16��ÿ��8λ
//{
//write_rom_data(*pp);pp++;
//}
//}



//wtite_8_8shi(ucharshijian)
//{uchari,r=2;
//for(i=0;i<8;i++)
//{cs=0;write_data_command();
//write_rom_address(r);
//write_rom_data(shi[shijian][i]);cs=1;r=r+4;}}



//wtite_8_8fenshiwei(ucharshijian){uchari,r=34;
//for(i=0;i<8;i++)
//{cs=0;write_data_command();
//write_rom_address(r);
//write_rom_data(fenshiwei[shijian][i]);
//
//cs=1;r=r+4;}}



//wtite_8_8fengewei(ucharshijian)
//{
//	uchari,r=66;
//	for(i=0;i<8;i++)
//	{
//		cs=0;
//		write_data_command();
//		write_rom_address(r);
//		write_rom_data(fengewei[shijian][i]);
//		cs=1;
//		r=r+4;
//	}
//}


//write_all(unsignedchar*pp)//дȫ��
//{unsignedchari;
//write_data_command();write_rom_address(0x00);for(i=0;i<48;i++){
//write_rom_data(*pp);pp++;
//}}


clean()//����
{
		unsigned char i;
		write_data_command();
		write_rom_address(0x00);
		for(i=0;i<48;i++)
		{
		  write_rom_data(0x00);
          }
}




//   00    02   04   06   08   0A   0C  0E 

//R	 0X04 0X04 0X04 0X04 0X04 0X04 0X04 0X04
//G	 0X02 0X02 0X02 0X02 0X02 0X02 0X02 0X02 
//B	 0X01 0X01 0X01 0X01 0X01 0X01 0X01 0X01
led_r()
{
   u8 i;
	   for(i=0;i<8;i++)
	   {
		ht1632_cs=0;
		write_data_command();//д��ַǰ3���ַ�101
		write_rom_address(2*i);//�׵�ַ		
		write_rom_data(0x02);
        ht1632_cs=1;
	   }

}
led_g()
{

	 u8 i;
//	   for(i=0;i<8;i++)
//	   {
		ht1632_cs=0;
		delay_us(50);
		write_data_command();//д��ַǰ3���ַ�101
		write_rom_address(0x00);//�׵�ַ
		  for(i=0;i<8;i++)
		  {
		  		
		  write_rom_data(0xFF);
		}
        ht1632_cs=1;
		delay_us(50);
//	   }
//	     write_command();//100�����
//		write_8bit(C_COMMAND1632_LEDON);//		LEDON 0x03

}
led_b()
{

       u8 i;
	   for(i=0;i<8;i++)
	   {
		ht1632_cs=0;
		write_data_command();//д��ַǰ3���ַ�101
		write_rom_address(2*i);//�׵�ַ		
		write_rom_data(0x01);
        ht1632_cs=1;
	   }


}
write_all(unsigned char *pp )
{
        unsigned char i;
 		write_data_command();//д��ַǰ3���ַ�101
		write_rom_address(0x00);//�׵�ַ
		for(i=0;i<48;i++)
		{
		  write_rom_data(*pp);
		  pp++;
		}	
} 







#define SYS_DIS 0x00 //�ر�ϵͳ��?
#define SYS_EN 0x01 //��ϵͳ��?
#define LED_OFF 0x02 //�ر�LED��ʾ?
#define LED_ON 0x03 //��LED��ʾ?
#define BLINK_OFF 0x08 //�ر���˸?
#define BLINK_ON 0x09 //����˸?
 
#define SLAVE_MODE 0x10 //��ģʽ?
 
#define RC_MASTER_MODE 0x18//0x18 //�ڲ�RCʱ��?
 
#define COM_OPTION 0x20 //24:16COM��NMOS��C��COM��PMOS��20?
#define PWM_DUTY 0xAE //PWM���ȿ���?
/****************���Ŷ���**********************/

void HT1632C_Write(unsigned char Data,unsigned char cnt) //MCU��HT1632Cд���ݺ�������λ
 {
	 unsigned char i; 
	 for(i=0;i<cnt;i++)
	  {
	  wr=0;
	  if(Data&0x80)
	  dat=1;
	  else
	  dat=0;
	  Data<<=1;
	  wr=1;
	  }
}


 
void HT1632C_Write_CMD(unsigned char cmd)  //MCU��HT1632cд����?
{
	cs1=0;
	HT1632C_Write(0x80,3); //ID:100?
	 HT1632C_Write(cmd,9); //��������λ����λΪ?
	 cs1=1; 
} 
void HT1632C_Write_DATA(unsigned char Addr,unsigned char p,unsigned char cnt)//ָ����ʼ��ַ����д�����ݣ�cntΪд�����ݸ�����ÿ�����ݰ�λ��?
{ 
  unsigned char i; 
 cs1=0; 
 
 HT1632C_Write(0xa0,3);//ID:101?
 HT1632C_Write(Addr<<1,7);
 for(i=0;i<cnt;i++)
 {
 HT1632C_Write(p,8);

 }

 cs1=1; 
} 
void HT1632C_clr(void) //��������?
{ unsigned char i;
  cs1=0; 
  HT1632C_Write(0xa0,3);
  HT1632C_Write(0x00,7);
  for(i=0;i<48;i++) 
   HT1632C_Write(0,8); 
   cs1=1; 
} 
void HT1632C_Init(void) //HT1632C��ʼ������?
{ 
  cs1=1;
//  cs2=1;
  wr=1;
  dat=1;
  HT1632C_Write_CMD(SYS_DIS);//�ر�HT1632C�ڲ�RCʱ��?
	HT1632C_Write_CMD(COM_OPTION); //ѡ��HT1632C����ģʽ��COM��NMOSģʽ??
	HT1632C_Write_CMD(RC_MASTER_MODE); //ѡ���ڲ�RCʱ��??
//	HT1632C_Write_CMD(0x18); //ѡ���ڲ�RCʱ��??
	HT1632C_Write_CMD(SYS_EN); //��HT1632C�ڲ�RCʱ��??
	HT1632C_Write_CMD(PWM_DUTY); //PWM��������??
	HT1632C_Write_CMD(BLINK_OFF); //�ر���˸??
	HT1632C_Write_CMD(LED_ON);//��LED��ʾ?

 
}

//void main(void)
//{
//	unsigned char temp=0xAA; 
//	HT1632C_Init();
//	HT1632C_clr();
//	HT1632C_Write_DATA(0x00,temp,48);
//	for(;;);
//}

void huxideng(void)
{
//     u8 i;
	 unsigned char temp=0x20; 

//	   for(i=0;i<16;i++)
//	   {
//		ht1632_cs=0;//Ƭѡ//
//		delay_us(50);		
//		write_command();//100�����
//		write_8bit(0xaf);	
//		//write_8bit(0xa0+i);//		PWMDUTYSET 0xaf
//		ht1632_cs=1;//ȡ��Ƭѡ//cs1=1;//cs2=1;//CS15=1;
//		delay_us(50);
//        }
//		 for(i=0;i<16;i++)
//	   {
//		ht1632_cs=0;//Ƭѡ//		
//		write_command();//100�����	
//		write_8bit(0xaf-i);//		PWMDUTYSET 0xaf
//		ht1632_cs=1;//ȡ��Ƭѡ//cs1=1;//cs2=1;//CS15=1;
//		delay_us(50);
//        }
		//   led_r();
 //led_g();
//	  led_b();
	  //write_all();
	
	//HT1632C_clr();
	HT1632C_Write_DATA(0x00,temp,64);

}

void LED_red(void)
{
   //  u8 i;
	 unsigned char temp=0x20; 


	HT1632C_Write_DATA(0x00,temp,64);

}
void LED_green(void)
{
  //   u8 i;
	 unsigned char temp=0x40; 


	HT1632C_Write_DATA(0x00,temp,64);

}
void LED_blue(void)
{
//     u8 i;
	 unsigned char temp=0x80; 


	HT1632C_Write_DATA(0x00,temp,64);

}


