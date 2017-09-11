#include "exti.h"
#include "delay.h"
#include "usart.h"
#include "common.h"

//�ⲿ�ж�0�������
void EXTIX_Init(void)
{
 
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;


  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

  //GPIOE.2 �ж����Լ��жϳ�ʼ������   �½��ش���
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource11);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line11;	//ssr1
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

  //GPIOE.2 �ж����Լ��жϳ�ʼ������   �½��ش���
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line12;	//ssr1
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

		NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn ;			//ʹ�ܰ���KEY2���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);
		
}

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

//ssr1�����ж�
void EXTI15_10_IRQHandler(void)         		 //����Ϊ��EXTI15_10 (�ⲿ�жϺŵ�10~15��������ʵ�֣�   
{   
	if(EXTI_GetITStatus(EXTI_Line11) != RESET) 
	{  																					//                                                   1  2  1  2   
		if(!SSR1)																	//�ߵ�ƽ����,��ʱ�ⲿ�˿ڵ�ƽΪ�͵�ƽ�����벽��1   -----|__|--|__|----
		{
			SysVar.PlusCntStep = 1;
			SysVar.PlusCntHT = PLUS_WITH_H_T;
		}																					//                                                   2  1  2  1
		else if(SysVar.PlusCntHT == 0)						//�͵�ƽ��������ʱ�ⲿ�˿ڵ�ƽΪ�ߵ�ƽ�����벽��2   ____|--|__|--|____
		{
			SysVar.PlusCntStep = 2; 
			SysVar.PlusCntLT = PLUS_WITH_L_T; 
		}
		SysVar.CntFlg = 1;
		SysVar.PlusCntOverTim = PLUS_WITH_OVER_T;
		EXTI_ClearITPendingBit(EXTI_Line11);		 	//���ж�	
	} 
}  

	



// //ssr2�����ж�
// void EXTI9_5_IRQHandler(void)		 //����Ϊ��EXTI15_10 (�ⲿ�жϺŵ�10~15��������ʵ�֣�	
// {		   
// 	if(EXTI_GetITStatus(EXTI_Line8) != RESET) //����Ϊ�ж���Ӧ���жϺ��Ƿ�����жϣ�����ж���жϵĻ���	 
// 	{	
// 		if(!SSR2)//�ߵ�ƽ����
// 		{
// 		  //printf("Line8 H\r\n");
// 			SysVar.PlusCntStep = 1;
// 			SysVar.PlusCntHT = PLUS_WITH_H_T;
// 		}
// 		else
// 		{
// 			SysVar.PlusCntStep = 2;
// 		  //printf("Line8 L\r\n");
// 			SysVar.PlusCntLT = PLUS_WITH_L_T;
// 		}
// 		EXTI_ClearITPendingBit(EXTI_Line8);		  //���ж�	 
// 	} 
// }	

