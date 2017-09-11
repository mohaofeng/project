#include "exti.h"
//#include "delay.h"
//#include "usart.h"
#include "SysConfig.h"



#define PLUS_WITH_DLY_T  							(4)				//高电平
#define PLUS_WITH_H_T  								(4)				//高电平
#define PLUS_WITH_L_T  								(4)				//高电平持续时间
#define PLUS_WITH_OVER_T  						(2000)				//脉冲计数超时时间

//外部中断0服务程序
void EXTIX_Init(void)
{
 
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
 		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
		GPIO_SetBits(GPIOC, GPIO_Pin_1);		
			
  //GPIOE.2 中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource1);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line1;	//ssr1
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

		NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn ;			//使能按键KEY2所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);
		
}

//ssr1输入中断
void EXTI9_5_IRQHandler(void)         		 //这里为：EXTI15_10 (外部中断号的10~15都在这里实现）   
{   
	if(EXTI_GetITStatus(EXTI_Line8) != RESET) 
	{  			
		if(DlyVar.PlusCntDly == 0)
		{
			DlyVar.PlusCntDly = PLUS_WITH_DLY_T;
			DlyVar.PlusCntOverTim = PLUS_WITH_OVER_T;
			SysVar.PlusCntSum ++;
		}
		EXTI_ClearITPendingBit(EXTI_Line8);		 	//清中断	
	} 
}  


