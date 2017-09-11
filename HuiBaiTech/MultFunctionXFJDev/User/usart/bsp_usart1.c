/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   重现c库printf函数到usart端口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO-MINI STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "bsp_usart1.h"

 /**
  * @brief  USART1 GPIO 配置,工作模式配置。9600 8-N-1
  * @param  无
  * @retval 无
  */
void USART1_Config(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
	
	  // 打开串口GPIO的时钟
	  SERIAL_LCD_USART_GPIO_APBxClkCmd(SERIAL_LCD_USART_GPIO_CLK, ENABLE);
	  
	  // 打开串口外设的时钟
	  SERIAL_LCD_USART_APBxClkCmd(SERIAL_LCD_USART_CLK, ENABLE);
	
	  // 将USART Tx的GPIO配置为推挽复用模式
	  GPIO_InitStructure.GPIO_Pin = SERIAL_LCD_USART_TX_GPIO_PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(SERIAL_LCD_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	
	// 将USART Rx的GPIO配置为浮空输入模式
	  GPIO_InitStructure.GPIO_Pin = SERIAL_LCD_USART_RX_GPIO_PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_Init(SERIAL_LCD_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	  
	  // 配置串口的工作参数
	  // 配置波特率
	  USART_InitStructure.USART_BaudRate = SERIAL_LCD_USART_BAUDRATE;
	  // 配置 针数据字长
	  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	  // 配置停止位
	  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	  // 配置校验位
	  USART_InitStructure.USART_Parity = USART_Parity_No ;
	  // 配置硬件流控制
	  USART_InitStructure.USART_HardwareFlowControl = 
	  USART_HardwareFlowControl_None;
	  // 配置工作模式，收发一起
	  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	  // 完成串口的初始化配置
	  USART_Init(SERIAL_LCD_USARTx, &USART_InitStructure);
	  	  
	  // 使能串口接收中断
	  USART_ITConfig(SERIAL_LCD_USARTx, USART_IT_RXNE, ENABLE);	  
	  
	  // 使能串口
	  USART_Cmd(SERIAL_LCD_USARTx, ENABLE);		  
}



void Uart1Sent(uint8_t ch)
{
	while((USART1->SR&0X40)==0)
	{
		;
	}//循环发送,直到发送完毕   
	USART1->DR = (u8)ch;      	
}


void Uart1SentStr(uint8_t *ch)
{
	uint16_t iNum;
	while(ch[iNum] != 0)
	{
		Uart1Sent(ch[iNum]);
	}
}



/*********************************************END OF FILE**********************/
