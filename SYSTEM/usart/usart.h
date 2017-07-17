#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "stm32f10x.h"

//extern u8 USART_RX_BUF[64];     //接收缓冲,最大63个字节.末字节为换行符 
//extern u8 USART_RX_STA;         //接收状态标记	
void Mac_TX_State(void)	;
void time_sound(void);
void USART5_TX_State1(void)	;
void USART5_TX_State2(void);
void USART5_TX_State3(void)	;
void USART5_TX_State4(void);
void USART5_TX_State(void);
void USART5_TX_ceshi_1(void);
void USART5_TX_ceshi_2(void);
u8 send_msg_to_wifi(u8 cmd_data,u8 *p_data,u32 i_data_len);
u8 send_msg_to_wifi_1(u8 cmd_data,u8* p_data,u32 i_data_len);
void uart1_init(u32 bound);
void USART3_Config(u32 bound);
void USART2_Config(u32 bound);
void Internet_TX_State(void);
char *itoa(int value, char *string, int radix) ;
void ID_information(void);
void receive_mac_from_wifi(void);
u8 receive_msg_from_wifi(void);
u8 send_msg_to_usb(u8 cmd_data,u8* p_data,u32 i_data_len);
void receive_msg_from_usb(void);
// u8 send_msg_to_usb(u8 cmd_data,u8 *p_data,u32 i_data_len);
extern u8 flash_write(u32 StartAddr,u32 *p_data,u32 size);
extern int flash_read(u32 StartAddr,u32 *p_data,u32 size);
#endif
