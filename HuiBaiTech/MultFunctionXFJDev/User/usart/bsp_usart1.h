#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>




/** 
  * 串口宏定义，不同的串口挂载的总线和IO不一样，移植时需要修改这几个宏
	* 1-修改总线时钟的宏，uart1挂载到apb2总线，其他uart挂载到apb1总线
	* 2-修改GPIO的宏
  */
	
// 串口1-USART1
#define  SERIAL_LCD_USARTx                   USART1
#define  SERIAL_LCD_USART_CLK                RCC_APB2Periph_USART1
#define  SERIAL_LCD_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  SERIAL_LCD_USART_BAUDRATE           115200

// USART GPIO 引脚宏定义
#define  SERIAL_LCD_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  SERIAL_LCD_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  SERIAL_LCD_USART_TX_GPIO_PORT       GPIOA   
#define  SERIAL_LCD_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  SERIAL_LCD_USART_RX_GPIO_PORT       GPIOA
#define  SERIAL_LCD_USART_RX_GPIO_PIN        GPIO_Pin_10

#define  SERIAL_LCD_USART_IRQ                USART1_IRQn
#define  SERIAL_LCD_USART_IRQHandler         USART1_IRQHandler


void USART1_Config(void);
void Uart1Sent(uint8_t ch);
void Uart1SentStr(uint8_t *ch);
//int fputc(int ch, FILE *f);
//void USART1_printf(USART_TypeDef* USARTx, uint8_t *Data,...);

#endif /* __USART1_H */
