#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
#define SYS_LED PCout(13)// PB5
#define NRF_TXLED PCout(14)//
#define NRF_RXLED PCout(15)//2.4G�����ϴ�
#define RST_KEY PBin(7)		//��λ����

void LED_Init(void);//��ʼ��
void LEDTask(void);

		 				    
#endif
