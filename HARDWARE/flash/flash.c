#include "flash.h"

u32 Flash_Write_Address=0x0801FC00;
//u32 *p_data_write;
//u32 *p_data_read;
//                        model      Sample(-30) Coldtrap   Temp1_B    Temp1_C    Temp2_B    Temp2_C    Temp2_B    Temp2_C    Temp2_B    Temp2_C
//int32_t p_data_write[12]={0x00000008,0xFFFFFFFE2,0x00000028,0x00000000,0x00000001,0x00000000,0x00000001,0x00000000,0x00000001,0x00000000,0x00000001,0x00000005};
u32 p_data_write[56]={0x0000,0x00000000,0x00000000,0x00000000,0x00000001,0x00000000,0x00000001,0x00000002,0x00000001,0x00000003,0x00000001,0x00000005};
u32 p_data_read[56];
u8 Flash_Data_Length=56;
//p_data_write=&p_data_write_temp[0];
//p_data_read=&p_data_read_temp[0];
extern int16_t Sample_Value_Set;
extern int16_t Coldtrap_Value_Set;
extern u8 Model;
extern u16 PM25_Vs;
extern u16 PM25_k;
extern u8 Temp1_B,Temp1_C,Temp2_B,Temp2_C,Temp3_B,Temp3_C,Temp4_B,Temp4_C;
extern u16 re_frequency;
/*******************************************************************************
flash的写函数：
输入：	
		u32 StartAddr	flash起始地址
		u32 *p_data	待写入数据指针
		u32 size	写入数据的数量
输出：	
		0：正确执行		
		非0：出错
注意：输入数据一定是u32 的指针，即数据一定是按照4字节对齐写入的。
所以：size也是u32的个数（字节数的4分之一）
*******************************************************************************/

u8 flash_write(u32 StartAddr,u32 *p_data,u32 size)
{	
	volatile FLASH_Status FLASHStatus;	
	u32 EndAddr=StartAddr+size*4;	
	vu32 NbrOfPage = 0;	
	u32 EraseCounter = 0x0, Address = 0x0;
	int i;
	int MemoryProgramStatus=1;
	//为一是通过
	FLASH_Unlock();          //解锁函数  //duan
	NbrOfPage=((EndAddr-StartAddr)>>10)+1;	//有多少个页被擦除	//清除所有已有标志
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	//擦页
	FLASHStatus=FLASH_COMPLETE;
	for(EraseCounter=0;(EraseCounter<NbrOfPage)&&(FLASHStatus==FLASH_COMPLETE);EraseCounter++)
	{		
		//FLASHStatus=FLASH_ErasePage(StartAddr+(PageSize*EraseCounter));	
		FLASHStatus=FLASH_ErasePage(StartAddr+(size*EraseCounter));	
	}
	//开始写数据
	Address = StartAddr;
	i=0;
	while((Address<EndAddr)&&(FLASHStatus==FLASH_COMPLETE))	
	{	FLASHStatus=FLASH_ProgramWord(Address,p_data[i++]);  //duan
		Address=Address+4;
	}
	//检验数据是否出错
	Address = StartAddr;
	i=0;
	while((Address < EndAddr) && (MemoryProgramStatus != 0))
	{	
		if((*(vu32*) Address) != p_data[i++])
		{		MemoryProgramStatus = 0;
				return 1;
		}
		Address += 4;
	}
	FLASH_LockBank1(); //add by xgy
	return 0;
}

int flash_read(u32 StartAddr,u32 *p_data,u32 size)
{
	u32 EndAddr=StartAddr+size*4;
	int MemoryProgramStatus=1;
	u32 Address = 0x0;
	int i=0;
	Address = StartAddr;
	while((Address < EndAddr) && (MemoryProgramStatus != 0))
	{
		p_data[i++]=(*(vu32*) Address);
		Address += 4;	
	}
	return 0;	
}
void Parameter_Flash_write(void)
{
//	int Flash_Temp;
	p_data_write[0] =PM25_Vs&0xffff;
	//p_data_write[0]=Model;
//	p_data_write[1]=PM25_k&0xffff;;
	p_data_write[2]=re_frequency&0xffff;


	p_data_write[3]=0x02;
	p_data_write[4]=0x01;
	p_data_write[5]=0x01;

	p_data_write[6]=0x0D;
	p_data_write[7]=0X7F;
	p_data_write[8]=0X07;
	p_data_write[9]=0X18;
	p_data_write[10]=0X00;
	p_data_write[11]=0X05;
	p_data_write[12]=0X10;
	p_data_write[13]=0X10;
	p_data_write[14]=0X00;
	p_data_write[15]=0X00;
//	Flash_Temp=
			 //消息头体	DeviceID
//	 1	设备检测类型	01	02	03	04	05
//		水	气	土	声	风
//2	设备类型	01	02	03
//		室内设备	室外设备	数据采集器
//		02	分体机客厅	02	室外噪音		
//		03	分体机厨房	03			
//		04	分体机卧室	04			
//		05	分体机儿童房	05			
//4	生产时间	20150724	年月日
//5	当日生产序号	0100	同类产品当日生产序号
//6	软件版本号	10	V1.0
//7	硬件版本号	22	V2.2
//8	预留	0000
//	wifi_msg_temp_1[6]=0x02;
//	wifi_msg_temp_1[7]=0x01;
//	wifi_msg_temp_1[8]=0x01;
//
//	wifi_msg_temp_1[9]=0x0D;
//	wifi_msg_temp_1[10]=0x7F;
//	wifi_msg_temp_1[11]=0x07;
//	wifi_msg_temp_1[12]=0x18;
//
//	wifi_msg_temp_1[13]=0x00;		
//	wifi_msg_temp_1[14]=0x05;
//
//	wifi_msg_temp_1[15]=0x10;
//	wifi_msg_temp_1[16]=0x10;
//
//	wifi_msg_temp_1[17]=0x00;
//	wifi_msg_temp_1[18]=0x00;

	flash_write(Flash_Write_Address,&p_data_write[0],(u32)Flash_Data_Length);

}
