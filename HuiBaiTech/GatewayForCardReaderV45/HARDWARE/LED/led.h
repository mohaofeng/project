#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
#define SYS_LED PCout(13)// PB5
#define NRF_TXLED PCout(14)//
#define NRF_RXLED PCout(15)//2.4G数据上传
#define RST_KEY PBin(7)		//复位引脚

void LED_Init(void);//初始化
void LEDTask(void);

		 				    
#endif
