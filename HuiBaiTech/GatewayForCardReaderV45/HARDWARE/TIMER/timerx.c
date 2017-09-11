#include "timerx.h"
#include "led.h"
#include "usart.h"
#include "common.h"

extern uint8_t gNrfLnkGas; 
extern uint16_t gLED_Gas; //
extern uint16_t gTCPReConnectT;
extern uint8_t gNrfLnkGas;  																		//NRF24L01 ��ʱ����

//extern __Tst_ctr tst_ctr;

u32 uip_timer=0;//uip ��ʱ����ÿ10ms����1.

//��ʱ��3�жϷ������	 10ms
void TIM3_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//���ָ����TIM�жϷ������:TIM �ж�Դ 
	{ 
			uip_timer++;//uip��ʱ������1	
			if(gNrfLnkGas>0)gNrfLnkGas--;
			
			if(DlyVar.wdog_gas>0)DlyVar.wdog_gas--;
			if(DlyVar.tst_gas>0)DlyVar.tst_gas--;
			else 
				DlyVar.tst_gas = 0xffffffff;
			if(L01_Ctr.TxCnt>0)L01_Ctr.TxCnt--;
			if(DlyVar.LinkGasT>0)DlyVar.LinkGasT--;
		 
			if(gLED_Gas>0)gLED_Gas--;
			if(gTCPReConnectT>0)gTCPReConnectT--;
//			if(tst_ctr.AckDxGas>0)tst_ctr.AckDxGas--;
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
