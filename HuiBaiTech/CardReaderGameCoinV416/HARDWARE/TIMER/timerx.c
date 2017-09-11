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
		if((SysVar.PlusCntLT == 0) && (SysVar.PlusCntStep == 2))//�͵�ƽ����ʱ�䵽���������� +1
		{
			if((SysVar.PlusCntSum == 0)  &&(SysVar.icsn_old != SysVar.icsn_new)){
				SysVar.ye = 0; 
			}
			SysVar.PlusCntStep = 0;
			SysVar.PlusCntSum ++;
		}
		
		if((SysVar.PlusCntOverTim == 0) && (SysVar.CntFlg == 1)){//�������ڳ�ʱ����������������Ѿ���ɡ�
			SysVar.CntFlg = 2;
			SysVar.PlusCntStep = 0;
		}
}

/////////////////////////////////////////////////////////////////////
//��    �ܣ����Ͷ���ź�
//����˵��: Dly_Var.gPlusGas������x*10ms ����ϵͳ���ò���Config.CoinSigHod�ɸı�������
//					
//					SysVar.PlusNum�������         
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
				printf("SysVar.PlusNum=%d,���Ͷ���ź�\r\n",SysVar.PlusNum);
#endif
				break;
				default:
					break;
		}
	}
}

uint8_t cntTmp = 0;
//��ʱ��3�жϷ������	 10ms
void TIM3_IRQHandler(void){ 	
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//���ָ����TIM�жϷ������:TIM �ж�Դ 
	{ 
		  PlusCnt();
			GateSig();									//��Ϸ�ϱ��ź�����

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
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 				    		  			    	    
}
 
//������ʱ��6�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig( TIM3,TIM_IT_Update|TIM_IT_Trigger,ENABLE);//ʹ�ܶ�ʱ��6���´����ж�
 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
 	
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 								 
}
