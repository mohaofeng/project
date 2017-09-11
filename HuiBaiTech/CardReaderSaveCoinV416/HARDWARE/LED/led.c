
#define LED_C


#include "led.h"
#include "common.h"
//extern __SysDlyT Dly_Var;



static void TIM4_PWM_Init(u16 arr,u16 psc)
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;	
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); 
	TIM_OC2Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); 
	TIM_OC1Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); 
	TIM_ARRPreloadConfig(TIM4, ENABLE); 	
#if BEEP_ON_PWM		
	TIM_OC4Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); 
 	TIM_SetCompare3(TIM4,0);
#endif	
 	TIM_SetCompare3(TIM4,0);
 	TIM_SetCompare2(TIM4,0);
 	TIM_SetCompare1(TIM4,0);
	TIM_Cmd(TIM4, ENABLE); 
}

void IOInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	
	
	
	/* ---------------------------GPIOA--------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_11 | GPIO_Pin_12;						//KEY_T	,EXIT1,EXIT2		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);				
	GPIO_SetBits(GPIOA,GPIO_Pin_0 | GPIO_Pin_11 | GPIO_Pin_12);		
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_8 |GPIO_Pin_15;					//SDA,RC522_SS,BELL,COIN1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);				
	GPIO_SetBits(GPIOA,GPIO_Pin_1 | GPIO_Pin_4);		
	GPIO_ResetBits(GPIOA,GPIO_Pin_8 | GPIO_Pin_15);		//关闭蜂鸣器 ,上币信号1 常开状态
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_9; //USART2_TX,spi1_sck ,spi1_mi,spi1_mo,USART1_TX
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_10;			//USART2_RX,USART1_RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

	/* ---------------------------GPIOB--------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;	//lcd_SCL,lcd_A0,BOOT1,COIN2,COIN3,lcd_RST,wifi_rst,rc522_rst,lcd_cs,ZK_CS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(GPIOB, &GPIO_InitStructure);					
	GPIO_SetBits(GPIOB,GPIO_Pin_0 |GPIO_Pin_1|GPIO_Pin_11|GPIO_Pin_5);						 
	GPIO_ResetBits(GPIOB,GPIO_Pin_3 |GPIO_Pin_4);				//上币信号2,3  常开状态		 
	
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8 |GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//LED_B,LED_G,LED_R,spi2_sck ,spi2_mi,spi2_mo	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);					
  TIM4_PWM_Init(1000,0);	
	
	
	/* ---------------------------GPIOC--------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;				 //SPI2_NSS,NRF_CE
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	
 	GPIO_SetBits(GPIOC,GPIO_Pin_13 | GPIO_Pin_14);
	
	

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //NRF_IRQ
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	
 	GPIO_SetBits(GPIOC,GPIO_Pin_15);

}
void LCDIOInit(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	 	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1|GPIO_Pin_11|GPIO_Pin_5;				 								//lcd_A0 lcd_SCL  lcd_SDA
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
 GPIO_Init(GPIOB, &GPIO_InitStructure);					
 GPIO_SetBits(GPIOB,GPIO_Pin_0 |GPIO_Pin_1|GPIO_Pin_11|GPIO_Pin_5);						 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;					//SDA
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
 GPIO_Init(GPIOA, &GPIO_InitStructure);				
 GPIO_SetBits(GPIOA,GPIO_Pin_1);		
	

	
//  GPIO_ResetBits(GPIOA,GPIO_Pin_0);		
//  GPIO_SetBits(GPIOA,GPIO_Pin_0);		
//  GPIO_ResetBits(GPIOA,GPIO_Pin_0);		
//  GPIO_SetBits(GPIOA,GPIO_Pin_0);		
// 	
//  GPIO_ResetBits(GPIOB,GPIO_Pin_11);		
//  GPIO_SetBits(GPIOB,GPIO_Pin_11);		
//  GPIO_ResetBits(GPIOB,GPIO_Pin_11);		
//  GPIO_SetBits(GPIOB,GPIO_Pin_11);		
//  GPIO_ResetBits(GPIOB,GPIO_Pin_11);		
//  GPIO_SetBits(GPIOB,GPIO_Pin_11);		
// 	
	
}
void ReserveIO_Init(void)
{
// 	GPIO_InitTypeDef  GPIO_InitStructure;
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;			
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
// 	GPIO_Init(GPIOC, &GPIO_InitStructure);					 
// 	GPIO_SetBits(GPIOC,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);						 
}



void RGB_LEDInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);					
	GPIO_ResetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8);
  TIM4_PWM_Init(1000,0);	
}
void BeepIOInit(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟
#if BEEP_ON_PWM		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	 GPIO_Init(GPIOB, &GPIO_InitStructure);					
	 GPIO_ResetBits(GPIOB,GPIO_Pin_9);	
	 TIM4_PWM_Init(BEEP_Q,0);	
#else
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;		
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	 GPIO_Init(GPIOA, &GPIO_InitStructure);					
	 GPIO_ResetBits(GPIOA,GPIO_Pin_8);
#endif
}
void CoinIOInit(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				//COIN2
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 
 GPIO_ResetBits(GPIOB,GPIO_Pin_3);						 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				//COIN1
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 
 GPIO_ResetBits(GPIOB,GPIO_Pin_4);			

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				//COIN1
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 
 GPIO_ResetBits(GPIOA,GPIO_Pin_15);						 

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				//SSR1
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 
 GPIO_SetBits(GPIOA,GPIO_Pin_12);						 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				//SSR2
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 
 GPIO_SetBits(GPIOA,GPIO_Pin_11);						 
	
}
void Nrf24L01IOInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //NSS
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	
 	GPIO_SetBits(GPIOC,GPIO_Pin_13);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;		//	CE	
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOC,GPIO_Pin_14);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;    //IRQ 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_15);//
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;//spi2_sck ,spi2_mi,spi2_mo
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  
}
void RC522IOInit(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;						//rc522_irq
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,GPIO_Pin_4);  
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;					//rc522_rst
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_ResetBits(GPIOB, GPIO_Pin_10);  
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;//spi1_sck ,spi1_mi,spi1_mo
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);
}
void LED_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PE端口时钟
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 
 GPIO_ResetBits(GPIOB,GPIO_Pin_2);						 
}
void UARTIOInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟
 //USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
 
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10

}

void GPIOInit(void)
{
	//LCDIOInit();
	//ReserveIO_Init();
	//RGB_LEDInit();
	//BeepIOInit();
	//CoinIOInit();
	//Nrf24L01IOInit();
	//RC522IOInit();
	//LED_Init();
	//UARTIOInit();
	IOInit();
}
