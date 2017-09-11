/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "w5500_conf.h"
#include "utility.h"
#include "SysConfig.h"
#include "queue.h"
#include <string.h> 


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
		printf("HardFault_Handler\r\n");
		delay_ms(500);
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
	/**
  * @brief  PVD中断请求
  * @param  None
  * @retval None
  */
void PVD_IRQHandler(void)
{
		/*检测是否产生了PVD警告信号*/
		if(PWR_GetFlagStatus (PWR_FLAG_PVDO)==SET)			
		{
			/* 亮红灯，实际应用中应进入紧急状态处理 */
//			LED_RED; 
			printf("system power down\r\n");
		}
    /* 清除中断信号*/
    EXTI_ClearITPendingBit(EXTI_Line16);

}




/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
/**
  * @brief  This function handles TIM2 interrupt request.
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{	
		timer2_isr();
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);  		 
	}		 	
}


extern void GateSig(void);
extern void CoinSig(void);
//extern void PlusCnt(void);

/**
  * @brief  This function handles TIM3 interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void){ 	
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//检查指定的TIM中断发生与否:TIM 中断源 
	{ 
		GateSig();
		CoinSig();
		//PlusCnt();
		if(DlyVar.tst_gas1>0)DlyVar.tst_gas1--;
		if(DlyVar.tst_gas>0)DlyVar.tst_gas--;
		if(gNetCtr.LinkGas>0)gNetCtr.LinkGas--;
		if(DlyVar.LCDIdeT>0)DlyVar.LCDIdeT--;
		if(gNetCtr.ReConnectGas>0)gNetCtr.ReConnectGas--;
		if(RC522Ctr.ICScanGas>0)RC522Ctr.ICScanGas--;
		if(DlyVar.gGateSigDly>0)DlyVar.gGateSigDly--;
		if(DlyVar.gCoinSigDly>0)DlyVar.gCoinSigDly--;
		if(DlyVar.gLED_Gas>0)DlyVar.gLED_Gas--;
		if(DlyVar.LcdDisTaskDly>0)DlyVar.LcdDisTaskDly--;
		if(DlyVar.UpdateCoinCntDly>0)DlyVar.UpdateCoinCntDly--;
		if(DlyVar.NetScanGas>0)DlyVar.NetScanGas--;
		if(DlyVar.LCDIdeTUpdatGas>0)DlyVar.LCDIdeTUpdatGas--;
		
		//SYS_LED =!SYS_LED; 
		if(DlyVar.PlusCntDly>0)DlyVar.PlusCntDly--;
		/* 统计脉冲数量 */
		if(DlyVar.PlusCntOverTim > 0)
		{
			DlyVar.PlusCntOverTim--;
		}
		else if(SysVar.PlusCntSum > 0 && SysVar.CntFlg == 0)//有脉冲计数，待用户刷卡
		{
			SysVar.CntFlg = 2;
			printf("SysVar.PlusCntSum=%d\r\n",SysVar.PlusCntSum);
			SysVar.WaitUpPlusCntSum += SysVar.PlusCntSum;
			SysVar.PlusCntSum = 0;
		}
	
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 		
	}		
}





// 串口彩屏串口中断服务函数
void USART1_IRQHandler(void)
{
  uint8_t ucTemp;
	if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
	{		
		ucTemp = USART_ReceiveData(USART1);
    	EnQueue(&gExtTFTLCDQueu,ucTemp);    
	}	 
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
/**
  * @brief  This function handles macESP8266_USARTx Handler.
  * @param  None
  * @retval None
  */
void macESP8266_USART_INT_FUN ( void )
{	
	uint8_t ucCh;
	
	if ( USART_GetITStatus ( macESP8266_USARTx, USART_IT_RXNE ) != RESET )
	{
		ucCh  = USART_ReceiveData( macESP8266_USARTx );
		
		if ( strEsp8266_Fram_Record .InfBit .FramLength < ( RX_BUF_MAX_LEN - 1 ) )                       //预留1个字节写结束符
			strEsp8266_Fram_Record .Data_RX_BUF [ strEsp8266_Fram_Record .InfBit .FramLength ++ ]  = ucCh;

	}
	 	 
	if ( USART_GetITStatus( macESP8266_USARTx, USART_IT_IDLE ) == SET )                                         //数据帧接收完毕
	{
    strEsp8266_Fram_Record .InfBit .FramFinishFlag = 1;
		
		ucCh = USART_ReceiveData( macESP8266_USARTx );                                                              //由软件序列清除中断标志位(先读USART_SR，然后读USART_DR)
	
		SysVar.ucTcpClosedFlag = strstr ( strEsp8266_Fram_Record .Data_RX_BUF, "CLOSED\r\n" ) ? 1 : 0;
		
  }	

}
// 串口中断服务函数
void UART3_IRQHandler(void)
{
  //uint8_t ucTemp;
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
	{		
		//ucTemp = USART_ReceiveData(USART3);
    //USART_SendData(USART2,ucTemp);    
	}	 
}
// 二维码读头串口中断服务函数
void CODE_USART_IRQHandler(void)
{
  uint8_t ucTemp;
	if(USART_GetITStatus(CODE_USARTx,USART_IT_RXNE)!=RESET)
	{		
		ucTemp = USART_ReceiveData(CODE_USARTx);
		if(gNetCtr.netsta & TCP_CONNECTED/* && lcd_parm.distype == DIS_IDE_MOD*/)
			EnQueue(&gExtCodeQueu,ucTemp);    
    //USART_SendData(CODE_USARTx,ucTemp);    
	}	 
}
// 打印机串口中断服务函数
void PRINTER_USART_IRQHandler(void)
{
//  uint8_t ucTemp;
	if(USART_GetITStatus(UART5,USART_IT_RXNE)!=RESET)
	{		
		//ucTemp = USART_ReceiveData(PRINTER_USARTx);
    //USART_SendData(USART2,ucTemp);    
	}	 
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
