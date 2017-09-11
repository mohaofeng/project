#include "exti.h"
#include "delay.h"
#include "usart.h"
#include "common.h"

//外部中断0服务程序
void EXTIX_Init(void)
{
 
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;


  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

  //GPIOE.2 中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource11);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line11;	//ssr1
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

  //GPIOE.2 中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line12;	//ssr1
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn ;			//使能按键KEY2所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);
		
}

void PlusCnt(void)
{
		if((SysVar.PlusCntLT == 0) && (SysVar.PlusCntStep == 2))//低电平持续时间到，脉冲数量 +1
		{
			if((SysVar.PlusCntSum == 0)  &&(SysVar.icsn_old != SysVar.icsn_new)){
				SysVar.ye = 0; 
			}
			SysVar.PlusCntStep = 0;
			SysVar.PlusCntSum ++;
		}
		
		if((SysVar.PlusCntOverTim == 0) && (SysVar.CntFlg == 1)){//脉冲周期超时，且脉冲计数周期已经完成。
			SysVar.CntFlg = 2;
			SysVar.PlusCntStep = 0;
		}
}

//ssr1输入中断
void EXTI15_10_IRQHandler(void)         		 //这里为：EXTI15_10 (外部中断号的10~15都在这里实现）   
{   
	if(EXTI_GetITStatus(EXTI_Line11) != RESET) 
	{  																					//                                                   1  2  1  2   
		if(!SSR1)																	//高电平触发,此时外部端口电平为低电平，进入步骤1   -----|__|--|__|----
		{
			SysVar.PlusCntStep = 1;
			SysVar.PlusCntHT = PLUS_WITH_H_T;
		}																					//                                                   2  1  2  1
		else if(SysVar.PlusCntHT == 0)						//低电平触发，此时外部端口电平为高电平，进入步骤2   ____|--|__|--|____
		{
			SysVar.PlusCntStep = 2; 
			SysVar.PlusCntLT = PLUS_WITH_L_T; 
		}
		SysVar.CntFlg = 1;
		SysVar.PlusCntOverTim = PLUS_WITH_OVER_T;
		EXTI_ClearITPendingBit(EXTI_Line11);		 	//清中断	
	} 
}  

	



// //ssr2输入中断
// void EXTI9_5_IRQHandler(void)		 //这里为：EXTI15_10 (外部中断号的10~15都在这里实现）	
// {		   
// 	if(EXTI_GetITStatus(EXTI_Line8) != RESET) //这里为判断相应的中断号是否进入中断，如果有多个中断的话。	 
// 	{	
// 		if(!SSR2)//高电平触发
// 		{
// 		  //printf("Line8 H\r\n");
// 			SysVar.PlusCntStep = 1;
// 			SysVar.PlusCntHT = PLUS_WITH_H_T;
// 		}
// 		else
// 		{
// 			SysVar.PlusCntStep = 2;
// 		  //printf("Line8 L\r\n");
// 			SysVar.PlusCntLT = PLUS_WITH_L_T;
// 		}
// 		EXTI_ClearITPendingBit(EXTI_Line8);		  //清中断	 
// 	} 
// }	

