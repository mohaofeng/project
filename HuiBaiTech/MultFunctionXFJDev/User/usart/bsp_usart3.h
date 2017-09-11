#ifndef __USART3_H
#define	__USART3_H

#include "stm32f10x.h"
#include <stdio.h>

void USART3_Config(uint32_t brt);
void Uart3Sent(uint8_t ch);
void Uart3SentStr(uint8_t *ch);
	//int fputc(int ch, FILE *f);
//void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);

#endif /* __USART3_H */
