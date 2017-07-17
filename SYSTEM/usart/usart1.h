#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

#define Compressor_Close 0
#define Compressor_Open 1
#define Vacuumpump_Close 0
#define Vacuumpump_Open 1
#define	Vacuometer_Close 0
#define	Vacuometer_Open 1

void USART1_Config(void);
void NVIC_Configuration(void);
int fputc(int ch, FILE *f);

#endif /* __USART1_H */
