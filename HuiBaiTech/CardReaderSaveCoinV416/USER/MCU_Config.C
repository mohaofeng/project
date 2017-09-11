#define MCU_CONFIG_C

#include "MCU_CONFIG.H"

static void GPIOInit(void){
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);   	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);    
	/*-----------------------------GPIOA--------------------------------------*/
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;    
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
	GPIO_SetBits(GPIOA,GPIO_Pin_1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_2 | GPIO_Pin_3);

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
	GPIO_SetBits(GPIOA,GPIO_Pin_4 | GPIO_Pin_5);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6 | GPIO_Pin_7);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
	GPIO_SetBits(GPIOA,GPIO_Pin_11);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_15;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_12 | GPIO_Pin_15);

	/*-----------------------------GPIOB--------------------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_2);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_3);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_4 | GPIO_Pin_5);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_8);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);

 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_10);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_11 | GPIO_Pin_12);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*-----------------------------GPIOC--------------------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
}

static void TIM4_PWM_Init(u16 arr,u16 psc){  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;	
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
	
#if TIM4_CH1_EN > 0	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable); 
#endif	
#if TIM4_CH2_EN > 0	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); 
#endif	
#if TIM4_CH3_EN > 0	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable); 
#endif	
#if TIM4_CH4_EN > 0	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable); 
#endif	
	TIM_ARRPreloadConfig(TIM4, ENABLE); 		
 	
	TIM_SetCompare1(TIM4,0);
	TIM_SetCompare2(TIM4,0);
	TIM_Cmd(TIM4, ENABLE); 
}

static void TIM3_PWM_Init(u16 arr,u16 psc){  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	
#if TIM3_CH1_EN > 0	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); 
#endif	
#if TIM3_CH2_EN > 0	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM3, &TIM_OCInitStructure); 
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); 
#endif	
#if TIM3_CH3_EN > 0	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure); 
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); 
#endif	
#if TIM3_CH4_EN > 0	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM3, &TIM_OCInitStructure); 
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); 
#endif	
	TIM_ARRPreloadConfig(TIM3, ENABLE); 		   // 使能TIM3重载寄存器ARR
 	
//	TIM_SetCompare1(TIM3,0);
//	TIM_SetCompare2(TIM3,0);
	TIM_SetCompare3(TIM3,0);
	TIM_SetCompare4(TIM3,0);
	TIM_Cmd(TIM3, ENABLE); 

}
static void TIM2_PWM_Init(u16 arr,u16 psc){  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;

	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;  
	
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
	
#if TIM2_CH1_EN > 0	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM2, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); 
#endif	
#if TIM2_CH2_EN > 0	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure); 
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); 
#endif	
#if TIM2_CH3_EN > 0	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM2, &TIM_OCInitStructure); 
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable); 
#endif	
#if TIM2_CH4_EN > 0	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM2, &TIM_OCInitStructure); 
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable); 
#endif	
/*------------------------*/
  	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  	TIM_ICInitStructure.TIM_ICFilter = 0x03;
  	TIM_ICInit(TIM2, &TIM_ICInitStructure);
   	TIM_Cmd(TIM2,ENABLE ); 	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig( TIM2,TIM_IT_Update|TIM_IT_CC2,ENABLE);							 
/*------------------------*/
	TIM_ARRPreloadConfig(TIM2, ENABLE); 		
	TIM_SetCompare1(TIM2,0);
	TIM_Cmd(TIM2, ENABLE); 

}

static void TIM1_PWM_Init(u16 arr,u16 psc){  

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
//	TIM_ICInitTypeDef  TIM_ICInitStructure;  
//	NVIC_InitTypeDef NVIC_InitStructure;
	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	
	TIM_DeInit(TIM1);
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); 
	
#if TIM1_CH1_EN > 0	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); 
#endif	
#if TIM1_CH2_EN > 0	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure); 
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable); 
#endif	
#if TIM1_CH3_EN > 0	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM1, &TIM_OCInitStructure); 
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable); 
#endif	
#if TIM1_CH4_EN > 0	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4Init(TIM1, &TIM_OCInitStructure); 
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable); 
#endif	
	TIM_Cmd(TIM1, ENABLE); 
	TIM_SetCompare1(TIM1,0);
	TIM_ARRPreloadConfig(TIM1, ENABLE); 		   // 使能TIM3重载寄存器ARR
	TIM_CtrlPWMOutputs(TIM1, ENABLE);  
}

static void SPI2_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,  ENABLE ); 
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;	
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure); 
	SPI_Cmd(SPI2, ENABLE); 
}   

 
#if EN_USART1_RX  
  
void uart_init(u32 bound){
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

    USART_Init(USART1, &USART_InitStructure); 
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);                    
}

void USART1_IRQHandler(void)                	
	{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) { 
		Res =USART_ReceiveData(USART1);
		Res = Res;
     } 
#ifdef OS_TICKS_PER_SEC	 
	OSIntExit();  											 
#endif
} 

#endif	



void PWMInit(void){
	TIM4_PWM_Init(T4SEQ,T4PSC);
	TIM3_PWM_Init(T3SEQ,T3PSC);
	TIM1_PWM_Init(T1SEQ,T1PSC);
	TIM2_PWM_Init(T2SEQ,T2PSC);
}

void SPIInit(void){
	SPI2_Init();
}




void TIM4_IRQHandler(void){
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  ); 
	}
}
void TIM3_IRQHandler(void){
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET){
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  ); 
	}
}
//void TIM2_IRQHandler(void){
//	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  ); 
//	}
//}
#if 0
static void EXTIX_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line10;	//NRF24L01 IRQ
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;				
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							
  	NVIC_Init(&NVIC_InitStructure); 
 
}
#endif
void EXTI15_10_IRQHandler (void){
//	if(EXTI_GetITStatus(EXTI_Line10) == RESET){
		EXTI_ClearITPendingBit(EXTI_Line10); 
		printf("EXTI15_10_IRQHandler\r\n");
//	}
}

////处理红外键盘
////返回值:
////	 0,没有任何按键按下
////其他,按下的按键键值.
//u8 Remote_Scan(void)
//{        
//	u8 sta=0;       
//    u8 t1,t2;  
//	if(RmtSta&(1<<6))//得到一个按键的所有信息了
//	{ 
//	    t1=RmtRec>>24;			//得到地址码
//	    t2=(RmtRec>>16)&0xff;	//得到地址反码 
// 	    if((t1==(u8)~t2)&&t1==REMOTE_ID)//检验遥控识别码(ID)及地址 
//	    { 
//	        t1=RmtRec>>8;
//	        t2=RmtRec; 	
//	        if(t1==(u8)~t2)sta=t1;//键值正确	 
//		}   
//		if((sta==0)||((RmtSta&0X80)==0))//按键数据错误/遥控已经没有按下了
//		{
//		 	RmtSta&=~(1<<6);//清除接收到有效按键标识
//			RmtCnt=0;		//清除按键次数计数器
//		}
//	}  
//    return sta;
//}
//
//
////定时器1中断服务程序	 
//void TIM1_IRQHandler(void)
//{ 		    	 
//    if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET){
//		if(RmtSta&0x80){//上次有数据被接收到了
//			RmtSta&=~0X10;						//取消上升沿已经被捕获标记
//			if((RmtSta&0X0F)==0X00)
//				RmtSta|=1<<6;//标记已经完成一次按键的键值信息采集
//			if((RmtSta&0X0F)<14)RmtSta++;
//			else
//			{
//				RmtSta&=~(1<<7);//清空引导标识
//				RmtSta&=0XF0;	//清空计数器	
//			}						 	   	
//		}							    
//	}
// 	if(TIM_GetITStatus(TIM1,TIM_IT_CC4)!=RESET)
//	{	  
//		if(RDATA)//上升沿捕获
//		{
//			TIM_OC4PolarityConfig(TIM1,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获				
//	    	TIM_SetCounter(TIM1,0);	   	//清空定时器值
//			RmtSta|=0X10;					//标记上升沿已经被捕获
//		}else //下降沿捕获
//		{			
//  			 Dval=TIM_GetCapture4(TIM1);//读取CCR1也可以清CC1IF标志位
//			 TIM_OC4PolarityConfig(TIM1,TIM_ICPolarity_Rising); //CC4P=0	设置为上升沿捕获
// 			
//			if(RmtSta&0X10)					//完成一次高电平捕获 
//			{
// 				if(RmtSta&0X80)//接收到了引导码
//				{
//					
//					if(Dval>300&&Dval<800)			//560为标准值,560us
//					{
//						RmtRec<<=1;	//左移一位.
//						RmtRec|=0;	//接收到0	   
//					}else if(Dval>1400&&Dval<1800)	//1680为标准值,1680us
//					{
//						RmtRec<<=1;	//左移一位.
//						RmtRec|=1;	//接收到1
//					}else if(Dval>2200&&Dval<2600)	//得到按键键值增加的信息 2500为标准值2.5ms
//					{
//						RmtCnt++; 		//按键次数增加1次
//						RmtSta&=0XF0;	//清空计时器		
//					}
// 				}else if(Dval>4200&&Dval<4700)		//4500为标准值4.5ms
//				{
//					RmtSta|=1<<7;	//标记成功接收到了引导码
//					RmtCnt=0;		//清除按键次数计数器
//				}						 
//			}
//			RmtSta&=~(1<<4);
//		}				 		     	    					   
//	}
// TIM_ClearFlag(TIM1,TIM_IT_Update|TIM_IT_CC4);	    
//}
//
void ADCInit(void){
	ADC_InitTypeDef ADC_InitStructure; 

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	 
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   					
	
	ADC_DeInit(ADC1);  										
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);                     
    
    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode                = ADC_Mode_Independent;     
    ADC_InitStructure.ADC_ContinuousConvMode  = ENABLE;                 
    ADC_InitStructure.ADC_DataAlign           = ADC_DataAlign_Right;  
    ADC_InitStructure.ADC_ExternalTrigConv    = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_NbrOfChannel        = CH_NUM ;                    
    ADC_InitStructure.ADC_ScanConvMode        = ENABLE;               
    ADC_Init(ADC1,&ADC_InitStructure);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
    ADC_DMACmd(ADC1,ENABLE);
    ADC_Cmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1);  
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1)); 
	
}

void MCUConfig(void){
	GPIOInit();
	PWMInit();
	SPIInit();
#if SYS_DBG
	uart_init(115200);
#else 
	uart_init(9600);
#endif
	
}
