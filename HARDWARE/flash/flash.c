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
flash��д������
���룺	
		u32 StartAddr	flash��ʼ��ַ
		u32 *p_data	��д������ָ��
		u32 size	д�����ݵ�����
�����	
		0����ȷִ��		
		��0������
ע�⣺��������һ����u32 ��ָ�룬������һ���ǰ���4�ֽڶ���д��ġ�
���ԣ�sizeҲ��u32�ĸ������ֽ�����4��֮һ��
*******************************************************************************/

u8 flash_write(u32 StartAddr,u32 *p_data,u32 size)
{	
	volatile FLASH_Status FLASHStatus;	
	u32 EndAddr=StartAddr+size*4;	
	vu32 NbrOfPage = 0;	
	u32 EraseCounter = 0x0, Address = 0x0;
	int i;
	int MemoryProgramStatus=1;
	//Ϊһ��ͨ��
	FLASH_Unlock();          //��������  //duan
	NbrOfPage=((EndAddr-StartAddr)>>10)+1;	//�ж��ٸ�ҳ������	//����������б�־
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	//��ҳ
	FLASHStatus=FLASH_COMPLETE;
	for(EraseCounter=0;(EraseCounter<NbrOfPage)&&(FLASHStatus==FLASH_COMPLETE);EraseCounter++)
	{		
		//FLASHStatus=FLASH_ErasePage(StartAddr+(PageSize*EraseCounter));	
		FLASHStatus=FLASH_ErasePage(StartAddr+(size*EraseCounter));	
	}
	//��ʼд����
	Address = StartAddr;
	i=0;
	while((Address<EndAddr)&&(FLASHStatus==FLASH_COMPLETE))	
	{	FLASHStatus=FLASH_ProgramWord(Address,p_data[i++]);  //duan
		Address=Address+4;
	}
	//���������Ƿ����
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
			 //��Ϣͷ��	DeviceID
//	 1	�豸�������	01	02	03	04	05
//		ˮ	��	��	��	��
//2	�豸����	01	02	03
//		�����豸	�����豸	���ݲɼ���
//		02	���������	02	��������		
//		03	���������	03			
//		04	���������	04			
//		05	�������ͯ��	05			
//4	����ʱ��	20150724	������
//5	�����������	0100	ͬ���Ʒ�����������
//6	����汾��	10	V1.0
//7	Ӳ���汾��	22	V2.2
//8	Ԥ��	0000
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
