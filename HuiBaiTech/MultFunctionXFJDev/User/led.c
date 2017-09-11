#include "led.h"
#include "common.h"
//#include "wdg.h"
//#include "usart.h"	 
//#include "timerx.h"

uint16_t gLED_Gas = 0; 

void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);	 
	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
 GPIO_Init(GPIOB, &GPIO_InitStructure);					
 GPIO_SetBits(GPIOB,GPIO_Pin_2);						
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				//GATE
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 
 GPIO_ResetBits(GPIOA,GPIO_Pin_15);	


 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;				//WIFI_KEY ,LAN_KEY
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 
 GPIO_SetBits(GPIOB,GPIO_Pin_7 | GPIO_Pin_8);						
	
}
 
void LEDTask(void)
{
	if(gLED_Gas == 0)
	{
		gLED_Gas = LED_GAS_CONST;
		//IWDG_Feed();
//		printf("hello world!\r\n");
//		delay_ms(500);
		SYS_LED = !SYS_LED;
	}
}
