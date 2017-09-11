#ifndef __MCU_CONFIG_H
#define __MCU_CONFIG_H	 

#include "sys.h"
#define 	SYS_DBG 0	



//pwm config
#define TIM1_CH1_EN 1
#define TIM1_CH2_EN 0
#define TIM1_CH3_EN 0
#define TIM1_CH4_EN 0

#define TIM2_CH1_EN 0
#define TIM2_CH2_EN 0
#define TIM2_CH3_EN 0
#define TIM2_CH4_EN 0

#define TIM3_CH1_EN 0
#define TIM3_CH2_EN 0
#define TIM3_CH3_EN 1
#define TIM3_CH4_EN 1

#define TIM4_CH1_EN 1
#define TIM4_CH2_EN 1
#define TIM4_CH3_EN 0
#define TIM4_CH4_EN 0

//
#define LED0 PBout(5)
#define WORK_LED PBout(2)// PB5
#define GUNHR PAout(2)
#define GUNHL PAout(3)
#define GUNVUP PAout(6)
#define GUNVDOWN PAout(7)
#define WLED PBout(9)
#define BLED PCout(13)// PB5
#define FLED PCout(15)// PB5
#define LED_SHOT PAout(15)// PB5
#define RDATA PAin(11)	 //红外数据输入脚
#define RDATA2 PAin(1)	 //红外数据输入脚
#define REMOTE_ID	  0
#define CH_NUM  1           //要使用的ADC通道数

#define TSEQ    1000
#define PSC   	0
#define T1SEQ    1891
#define T1PSC   	0
#define T2SEQ    10000
#define T2PSC   	71
#define T3SEQ    1000
#define T3PSC   	0
#define T4SEQ    1000
#define T4PSC   	0
#define QIRT 		T1SEQ/2
//uart
#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
//extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
//extern u16 USART_RX_STA;         		//接收状态标记	

void MCUConfig(void);
u8 Remote_Scan(void);
#endif

