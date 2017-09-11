/**
  ******************************************************************************
  * @file    bsp_usart5.c
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
  
#include "bsp_usart5.h"

 /********************************************************************************
	* @file    bsp_usart.c
	* @author  fire
	* @version V1.0
	* @date    2013-xx-xx
	* @brief   重定向c库printf函数到usart端口
	******************************************************************************	  
  * @brief  USART GPIO 配置,工作参数配置
	* @param  无
	* @retval 无
	*/
  void USART5_Config(void)
  {
	  GPIO_InitTypeDef GPIO_InitStructure;
	  USART_InitTypeDef USART_InitStructure;
  
	  // 打开串口GPIO的时钟
	  PRINTER_USART_GPIO_APBxClkCmd(PRINTER_USART_GPIO_CLK, ENABLE);
	  
	  // 打开串口外设的时钟
	  PRINTER_USART_APBxClkCmd(PRINTER_USART_CLK, ENABLE);
  
	  // 将USART Tx的GPIO配置为推挽复用模式
	  GPIO_InitStructure.GPIO_Pin = PRINTER_USART_TX_GPIO_PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(PRINTER_USART_TX_GPIO_PORT, &GPIO_InitStructure);
  
	// 将USART Rx的GPIO配置为浮空输入模式
	  GPIO_InitStructure.GPIO_Pin = PRINTER_USART_RX_GPIO_PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_Init(PRINTER_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	  
	  // 配置串口的工作参数
	  // 配置波特率
	  USART_InitStructure.USART_BaudRate = PRINTER_USART_BAUDRATE;
	  // 配置 针数据字长
	  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	  // 配置停止位
	  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	  // 配置校验位
	  USART_InitStructure.USART_Parity = USART_Parity_No ;
	  // 配置硬件流控制
	  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	  // 配置工作模式，收发一起
	  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	  // 完成串口的初始化配置
	  USART_Init(PRINTER_USARTx, &USART_InitStructure);
	  	  
	  // 使能串口接收中断
	  USART_ITConfig(PRINTER_USARTx, USART_IT_RXNE, ENABLE);	  
	  
	  // 使能串口
	  USART_Cmd(PRINTER_USARTx, ENABLE);		  
  
	// 清除发送完成标志
	  //USART_ClearFlag(USART1, USART_FLAG_TC); 	
  }
        
void USART5_Sent(uint8_t c)
{
	while((UART5->SR&0X40)==0)
	{
		;
	}//循环发送,直到发送完毕   
	UART5->DR = c;      	
}
void UART5SentStr(uint8_t *ptc)
{
	uint16_t iNum=0;
	uint8_t *pc;
	pc = ptc;
	while(pc[iNum] != 0)
	{
		USART5_Sent(pc[iNum]);
		iNum++;
	}
}





/*********************************************END OF FILE**********************/
