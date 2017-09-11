



#include <printer.h>
#include "bsp_usart5.h"
#include "SysConfig.h"


void printDat(char *fmt, ...)
{
	va_list ap;
	
	va_start(ap,fmt);
	vsprintf(TmpVar.srttmp,fmt,ap);
	va_end(ap);

	UART5SentStr((uint8_t*)TmpVar.srttmp); 
	
}
	
void PrinterInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD,GPIO_Pin_3);						
	
	USART5_Config();
}




