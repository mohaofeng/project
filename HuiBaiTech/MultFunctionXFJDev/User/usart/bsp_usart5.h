#ifndef __USART5_H
#define	__USART5_H

#include "stm32f10x.h"
#include <stdio.h>






// ¡ä??¨²5-UART5
#define  PRINTER_USARTx                   UART5
#define  PRINTER_USART_CLK                RCC_APB1Periph_UART5
#define  PRINTER_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  PRINTER_USART_BAUDRATE           9600

// USART GPIO ¨°y??o¨º?¡§¨°?
#define  PRINTER_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)
#define  PRINTER_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  PRINTER_USART_TX_GPIO_PORT         GPIOC   
#define  PRINTER_USART_TX_GPIO_PIN          GPIO_Pin_12
#define  PRINTER_USART_RX_GPIO_PORT       GPIOD
#define  PRINTER_USART_RX_GPIO_PIN        GPIO_Pin_2

#define  PRINTER_USART_IRQ                UART5_IRQn
#define  PRINTER_USART_IRQHandler         UART5_IRQHandler

void USART5_Config(void);
void USART5_Sent(uint8_t c);
void UART5SentStr(uint8_t *ptc);

#endif /* __USART5_H */
