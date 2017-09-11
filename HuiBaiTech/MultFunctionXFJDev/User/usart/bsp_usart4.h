#ifndef __USART4_H
#define	__USART4_H

#include "stm32f10x.h"
#include <stdio.h>

// 串口4-UART4 二维码数据接收串口
#define  CODE_USARTx                   UART4
#define  CODE_USART_CLK                RCC_APB1Periph_UART4
#define  CODE_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  CODE_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  CODE_USART_GPIO_CLK             (RCC_APB2Periph_GPIOC)
#define  CODE_USART_GPIO_APBxClkCmd      RCC_APB2PeriphClockCmd
    
#define  CODE_USART_TX_GPIO_PORT         GPIOC   
#define  CODE_USART_TX_GPIO_PIN          GPIO_Pin_10
#define  CODE_USART_RX_GPIO_PORT       	 GPIOC
#define  CODE_USART_RX_GPIO_PIN       	 GPIO_Pin_11

#define  CODE_USART_IRQ                UART4_IRQn
#define  CODE_USART_IRQHandler         UART4_IRQHandler











void UART4_Config(void);
void Uart4Sent(uint8_t ch);
void Uart4SentStr(uint8_t *ch);
//int fputc(int ch, FILE *f);
//void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);

#endif /* __USART4_H */
