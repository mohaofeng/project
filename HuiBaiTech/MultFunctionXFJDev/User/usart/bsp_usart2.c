/**
  ******************************************************************************
  * @file    bsp_usart2.c
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
  
#include "bsp_usart2.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "SysConfig.h"

 /**
  * @brief  USART GPIO 配置,工作参数配置
  * @param  无
  * @retval 无
  */
void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数 
	// 配置波特率
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
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
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	
	// 串口中断优先级配置
//	NVIC_Configuration();
	
	// 使能串口接收中断
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(DEBUG_USARTx, ENABLE);		

  // 清除发送完成标志
	//USART_ClearFlag(USART1, USART_FLAG_TC);     
}

/////重定向c库函数printf到USART2
//int fputc(int ch, FILE *f)
//{
//		/* 发送一个字节数据到USART2 */
//		USART_SendData(USART2, (uint8_t) ch);
//		
//		/* 等待发送完毕 */
//		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);		
//	
//		return (ch);
//}

/////重定向c库函数scanf到USART2
//int fgetc(FILE *f)
//{
//		/* 等待串口1输入数据 */
//		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);

//		return (int)USART_ReceiveData(USART2);
//}



void Uart2Sent(uint8_t ch)
{
	while((USART2->SR&0X40)==0)
	{
		;
	}//循环发送,直到发送完毕   
	USART2->DR = (u8)ch;      	
}


void Uart2SentStr(uint8_t *ch)
{
	uint16_t iNum;
	while(ch[iNum] != 0)
	{
		Uart2Sent(ch[iNum]);
	}
}

//串口2,printf 函数
//确保一次发送数据不超过USART2_MAX_SEND_LEN字节
void u2_printf(char* fmt,...)  
{  
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)TmpVar.srttmp,fmt,ap);
	va_end(ap);
}

//向sim900a发送命令
//cmd:发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 AT_send_cmd(char *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	//USART2_RX_STA=0;
//	if((u32)cmd<=0XFF)
//	{
//		while(DMA1_Channel7->CNDTR!=0);	//等待通道7传输完成   
//		USART2->DR=(u32)cmd;
//	}
//	else 
//	{
//		u2_printf("%s\r\n",cmd);//发送命令
//	}
//	
//	if(ack&&waittime)		//需要等待应答
//	{
//		while(--waittime)	//等待倒计时
//		{
//			delay_us(1000);
////			if(USART2_RX_STA&0X8000)//接收到期待的应答结果
////			{
////				if(AT_check_cmd(ack))break;//得到有效数据 
////				USART2_RX_STA=0;
////			} 
//		}
//		if(waittime==0)res=1;
//	}
	
	return res;
} 

/*********************************************END OF FILE**********************/
