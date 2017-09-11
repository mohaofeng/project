#ifndef __LED_H
#define __LED_H	 
#include "sys.h"



#ifdef	LED_C
#define	LED_EXT
#else
#define	LED_EXT extern
#endif

#define SYS_LED 	PBout(2)				//ϵͳ����ָʾ�ƿ��ƶ˿�
#define COIN_SIG3 	PBout(4)			//�ϱ��źſ��ƶ˿�
#define COIN_SIG2 	PBout(3)			//�ϱ��źſ��ƶ˿�
#define COIN_SIG1 	PAout(15)			//�ϱ��źſ��ƶ˿�

void GPIOInit(void);
void RGBCtrTask(void);
			    
#endif
