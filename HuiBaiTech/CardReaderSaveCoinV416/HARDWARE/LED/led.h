#ifndef __LED_H
#define __LED_H	 
#include "sys.h"



#ifdef	LED_C
#define	LED_EXT
#else
#define	LED_EXT extern
#endif

#define SYS_LED 	PBout(2)				//系统运行指示灯控制端口
#define COIN_SIG3 	PBout(4)			//上币信号控制端口
#define COIN_SIG2 	PBout(3)			//上币信号控制端口
#define COIN_SIG1 	PAout(15)			//上币信号控制端口

void GPIOInit(void);
void RGBCtrTask(void);
			    
#endif
