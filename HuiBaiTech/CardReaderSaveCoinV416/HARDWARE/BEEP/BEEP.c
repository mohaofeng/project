#define BEEP_C

#include "BEEP.h"
#include "usart.h"	
#include "common.h"

//extern __Sys_Var SysVar;
//extern __SysDlyT Dly_Var;								//ÏµÍ³¶¨Ê±±äÁ¿

void BEEP_Init(void)
{
}

void BeepCtr(uint16_t sta)
{
#if BEEP_ON_PWM		
	if(sta)
		TIM_SetCompare4(TIM4,500);
	else
		TIM_SetCompare4(TIM4,0);
#else
		BEEP_PIN = sta;	
#endif
}

void BeepTask(void)
{
	static uint8_t st=0;	

// 	uint8_t typ = 0;
// 	typ = SysVar.BeepTyp;
	switch(SysVar.BeepType)
	{
		case B://b¶ÌÏì
 		if(st==0)
		{
#if BEEP_DBG
		printf("b¶ÌÏì\r\n");
#endif
 			Dly_Var.BeepHodT = BEEP_LIT_CONST;
			st = 1;
			BeepCtr(1);
 		}
 		if((st == 1)&&(Dly_Var.BeepHodT == 0))
		{
 			st = 0;
 			SysVar.BeepType = NO_BEEP;
			BeepCtr(0);
 		}
		else if(st >1)
		{
 			st = 0;
 			SysVar.BeepType = NO_BEEP;
			BeepCtr(0);
		}
		break;
		case B_://b¶ÌÏì
 		if(st==0)
		{
#if BEEP_DBG
		printf("b³¤Ïì\r\n");
#endif
 			Dly_Var.BeepHodT = BEEP_LIT_CONST*4;
			st = 1;
			BeepCtr(1);
 		}
 		if((st == 1)&&(Dly_Var.BeepHodT == 0))
		{
 			st = 0;
 			SysVar.BeepType = NO_BEEP;
			BeepCtr(0);
 		}
		else if(st >1)
		{
 			st = 0;
 			SysVar.BeepType = NO_BEEP;
			BeepCtr(0);
		}
		break;
		case BB://bb¶ÌÏì
 		if(0== st)
		{
#if BEEP_DBG
		printf("bb¶ÌÏì\r\n");
#endif
 			Dly_Var.BeepHodT = BEEP_LIT_CONST;
			st = 1;
			BeepCtr(1);
 		}
 		if((1 == st)&&(0 == Dly_Var.BeepHodT))
		{
			Dly_Var.BeepStpT = BEEP_STP_CONST;
			st = 0x2;
			BeepCtr(0);
		}
 		if((0x2 == st)&&(0 == Dly_Var.BeepStpT))
		{
 			st = 0x3;
			BeepCtr(1);
 			Dly_Var.BeepHodT = BEEP_LIT_CONST;
 		}
 		if((0x3 == st)&&(0 == Dly_Var.BeepHodT))
		{
 			st = 0x0;
			BeepCtr(0);
 			SysVar.BeepType = NO_BEEP;
 		}	
		else if(st >0x3)
		{
 			st = 0;
 			SysVar.BeepType = NO_BEEP;
			BeepCtr(0);
		}
		break;
		default:
			if(0 == Dly_Var.BeepHodT)
			{
				st = 0x0;
				BeepCtr(0);
			}
			break;
	}
}
