#include "led.h"
#include "common.h"
#include "wdg.h"

uint16_t gLED_Gas = 0; 

void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOC,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);						 //PB.5 �����
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_7);						 //PB.5 �����
}
 
void LEDTask(void)
{
	if(gLED_Gas == 0)
	{
		gLED_Gas = LED_GAS_CONST;
		SYS_LED = !SYS_LED;
	}
}
