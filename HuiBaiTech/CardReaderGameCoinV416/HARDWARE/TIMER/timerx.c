#include "timerx.h"
#include "led.h"
#include "usart.h"
#include "BEEP.h"
#include "common.h"
//extern __SysDlyT Dly_Var;
#include "FramePkg.h"
#include <stdlib.h>
#include <string.h> 

void PlusCnt(void)
{
		if((SysVar.PlusCntLT == 0) && (SysVar.PlusCntStep == 2))//低电平持续时间到，脉冲数量 +1
		{
			if((SysVar.PlusCntSum == 0)  &&(SysVar.icsn_old != SysVar.icsn_new)){
				SysVar.ye = 0; 
			}
			SysVar.PlusCntStep = 0;
			SysVar.PlusCntSum ++;
		}
		
		if((SysVar.PlusCntOverTim == 0) && (SysVar.CntFlg == 1)){//脉冲周期超时，且脉冲计数周期已经完成。
			SysVar.CntFlg = 2;
			SysVar.PlusCntStep = 0;
		}
}

/////////////////////////////////////////////////////////////////////
//功    能：输出投币信号
//参数说明: Dly_Var.gPlusGas脉冲宽度x*10ms 根据系统配置参数Config.CoinSigHod可改变脉冲宽度
//					
//					SysVar.PlusNum脉冲次数         
void GateSig(void)
{
	if((SysVar.PlusNum > 0) &&(Dly_Var.gPlusGas == 0))
	{
		switch(gFlg.CoinOtstp)
		{
			case 0:
#if GateSigDBG >0
					printf("Config.CoinSigHod=%d,Config.CoinSigGas=%d\r\n",Config.CoinSigHod,Config.CoinSigGas);
#endif
					Dly_Var.gPlusGas = Config.CoinSigHod*10;
					gFlg.CoinOtstp = 1;
					COIN_SIG1 = !COIN_SIG1;
				break;
			case 1:
				COIN_SIG1 = !COIN_SIG1;
				Dly_Var.gPlusGas = Config.CoinSigGas*10;
				gFlg.CoinOtstp = 0;
				SysVar.PlusNum--;
#if GateSigDBG >0
				printf("SysVar.PlusNum=%d,输出投币信号\r\n",SysVar.PlusNum);
#endif
				break;
				default:
					break;
		}
	}
}

uint8_t cntTmp = 0;
//定时器3中断服务程序	 10ms
void TIM3_IRQHandler(void){ 	
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//检查指定的TIM中断发生与否:TIM 中断源 
	{ 
		  PlusCnt();
			GateSig();									//游戏上币信号任务

			if(Dly_Var.ToolBarDly>0)Dly_Var.ToolBarDly--;
			if(Dly_Var.BeepHodT>0)Dly_Var.BeepHodT--;
			if(Dly_Var.gLED_Gas>0)Dly_Var.gLED_Gas--;
			if(Dly_Var.gRc522RdGas>0)Dly_Var.gRc522RdGas--;
			if(Dly_Var.gNrfLnkGas>0)Dly_Var.gNrfLnkGas--;
			if(Dly_Var.BeepStpT>0)Dly_Var.BeepStpT--;
			if(Dly_Var.gPlusGas>0)Dly_Var.gPlusGas--;
			if(Dly_Var.gPlusHold>0)Dly_Var.gPlusHold--;
			if(SysVar.PlusCntLT>0)SysVar.PlusCntLT--;
			if(SysVar.PlusCntHT>0)SysVar.PlusCntHT--;
			if(SysVar.PlusCntOverTim>0)SysVar.PlusCntOverTim--;
		
		
// 			if(SysVar.PlusCntLTCoin>0)SysVar.PlusCntLTCoin--;
// 			if(SysVar.PlusCntHTCoin>0)SysVar.PlusCntHTCoin--;
		
			if(Dly_Var.RED_FlashDly>0)Dly_Var.RED_FlashDly--;
			if(Dly_Var.LCDIdeT>0)Dly_Var.LCDIdeT--;
			if(Dly_Var.gLCDScanGas>0)Dly_Var.gLCDScanGas--;
			if(Dly_Var.gLEDScanGas>0)Dly_Var.gLEDScanGas--;
			if(L01_Ctr.TxCntTim>0)L01_Ctr.TxCntTim--;
		
			if(RGB_ctr.RedLightTim>0)RGB_ctr.RedLightTim--;
			if(RGB_ctr.BlueLightTim>0)RGB_ctr.BlueLightTim--;
			if(RGB_ctr.GreenLightTim>0)RGB_ctr.GreenLightTim--;
			if(RGB_ctr.RedFlashTim>0)RGB_ctr.RedFlashTim--;
	
		} 
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 				    		  			    	    
}
 
//基本定时器6中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig( TIM3,TIM_IT_Update|TIM_IT_Trigger,ENABLE);//使能定时器6更新触发中断
 
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
 	
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 								 
}
