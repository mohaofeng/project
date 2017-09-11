/**
  ******************************************************************************
  * @file    bsp_usart3.c
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
  
#include "bsp_usart3.h"

 /**
  * @brief  USART3 GPIO 配置,工作模式配置。9600 8-N-1
  * @param  无
  * @retval 无
  */
void USART3_Config(uint32_t brt)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		
		/* config USART3 clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		
		/* USART3 GPIO config */
		/* Configure USART3 Tx (PA.09) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);    
		/* Configure USART3 Rx (PA.10) as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
			
		/* USART3 mode config */
		USART_InitStructure.USART_BaudRate = brt;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No ;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USART3, &USART_InitStructure); 
		USART_Cmd(USART3, ENABLE);
}

/////重定向c库函数printf到USART3
//int fputc(int ch, FILE *f)
//{
//		/* 发送一个字节数据到USART3 */
//		USART_SendData(USART3, (uint8_t) ch);
//		
//		/* 等待发送完毕 */
//		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);		
//	
//		return (ch);
//}

/////重定向c库函数scanf到USART3
//int fgetc(FILE *f)
//{
//		/* 等待串口1输入数据 */
//		while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);

//		return (int)USART_ReceiveData(USART3);
//}



void Uart3Sent(uint8_t ch)
{
	while((USART3->SR&0X40)==0)
	{
		;
	}//循环发送,直到发送完毕   
	USART3->DR = (u8)ch;      	
}

void Uart3SentStr(uint8_t *ch)
{
	uint16_t iNum;
	while(ch[iNum] != 0)
	{
		Uart3Sent(ch[iNum]);
	}
}




/*********************************************END OF FILE**********************/
