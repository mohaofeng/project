#include "led.h"
#include "common.h"
#include "wdg.h"

uint16_t gLED_Gas = 0; 

void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOC,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);						 //PB.5 输出高
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_7);						 //PB.5 输出高
}
 
void LEDTask(void)
{
	if(gLED_Gas == 0)
	{
		gLED_Gas = LED_GAS_CONST;
		SYS_LED = !SYS_LED;
	}
}
