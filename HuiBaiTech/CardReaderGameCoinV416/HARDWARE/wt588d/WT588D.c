#define WT588D_C



#include "WT588D.H"





#define WT588DCS_CONST  6
#define WT588DCLK_CONST 2500
#define WT588DDAT_CONST 1000




uint8_t WT588D_Dly=0;			//wt588dʼ����ʱʱ�����


void Dlyn(uint32_t dt){
  while(dt--);
}
void Wt588dInit(void){
	GPIO_InitTypeDef  GPIO_InitStructure; 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					
	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);			
	GPIO_SetBits(GPIOA,GPIO_Pin_15);				
	
}
#if 0
/*-------------------------------------- 
;ģ������:Line_1A(void)
;��    ��:ʵ��һ�ߵ��ֽڵ�λ��ǰ����ͨ�ź���
;��    ��:��
;��    ��:0	   ����������
;-------------------------------------*/
void Line_1A(uint8_t add)
{
	uint8_t i;
	static uint8_t st=0;
	
//********************************************************

	P_DATA = 0;

	Delay_1ms(5);   				//��������5ms

	B_DATA = dat&0X01;
	for(i=0;i<8;i++)
	{
		P_DATA = 1;				//�������ݴ����ߣ�׼����������
		if(B_DATA==0)
		{	/*��ʾ�߼���ƽ0*/
			Delay_10us(20);     // ��ʱ200us
			P_DATA = 0;
			Delay_10us(20);	    // ��ʱ600us
			Delay_10us(20);
			Delay_10us(20);	    
		}
		else
		{  /*��ʾ�߼���ƽ1*/
			Delay_10us(20);
			Delay_10us(20);
			Delay_10us(20);
			P_DATA = 0;
			Delay_10us(20);
		}
		dat = dat>>1;
		B_DATA = dat&0X01;
	}
	P_DATA = 1;					//���ߵ�ƽ
}
#else 
void Sound_Ply(void)
{
	static uint8_t st=0;
	uint8_t i = 0x00;
	uint8_t dat;
	if(gWt588dTxFlg == 1){
		dat = gWt588dDat;
		if(st == 0){
			WT588D_Dly= WT588DCS_CONST;
			WT588D_CS_L();
			st = 1;
		}
		if((st == 1) && (WT588D_Dly == 0)){
	//		WT588D_CS_H();
			st =2;
		}
		if(st == 2){
			for(i=0;i<8;i++){
				if((dat&0x01) != 0){
					WT588D_DAT_H();
				}
				else{
					WT588D_DAT_L();			
				}
				Dlyn(WT588DDAT_CONST);
				WT588D_CLK_L();	
				Dlyn(WT588DCLK_CONST);
				WT588D_CLK_H();					
				Dlyn(WT588DCLK_CONST);
				dat >>= 1;	 			
			}
			st = 0;
			gWt588dTxFlg = 0;
			WT588D_CS_H();
//			WT588D_DAT_H();
//			WT588D_CLK_H(); 				
		}
	}
}
#endif

