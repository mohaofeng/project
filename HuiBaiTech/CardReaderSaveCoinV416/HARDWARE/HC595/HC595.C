#define HC595_C


#include "HC595.H"
#include "delay.h"



const uint8_t  seg7codeP[19]={0x3f,0x06,0x5b,0x4f, 0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c, 0x39,0x5e,0x79,0x71,OFFSEG,GANGSEG,DOT};					
//const uint8_t seg7codeN[16]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};

static void DataLaunch(void){
	HC595_RCK_L();
	HC595_RCK_H();
}
static void HC595_shift_byte(uint8_t byte)
{
    uint8_t i = 0;
    for (i = 0; i < 8; i++){
        if (byte & 0x80){
            HC595_DAT_L();
				}
        else{
            HC595_DAT_H();
				}
        HC595_SCK_L();
        HC595_SCK_H();
        byte = byte << 1;
    }
}
void HC595_init(void)
{
		uint8_t i,k=0x00;
		GPIO_InitTypeDef  GPIO_InitStructure; 	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PE端口时钟

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;				 //LED0-->PB.5 端口配置
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
		GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
		GPIO_SetBits(GPIOB,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);						 //PB.5 输出高

		for(i=0;i<=8;i++){
			HC595_shift_byte(k);		
			HC595_shift_byte(0x00);			
			DataLaunch();
			delay_ms(200);			
			k <<=1;
			k |= 0x01;
		}
		delay_ms(200);			
}
void DisAllOFF(void){
	HC595_shift_byte(0x00);		 //??  
	HC595_shift_byte(0x00);		 //??					
	DataLaunch();
}
void DisAllOn(void){
	//??
	HC595_shift_byte(0x00);		 //??  
	HC595_shift_byte(0xff);		 //??					
	DataLaunch();
}
void DigiTst(void){
	HC595_shift_byte(0xff);		  //????
	HC595_shift_byte(0xef);		 //????			
	DataLaunch();	
}
static void Disrefresh(uint8_t *buf){
	static uint8_t tmp=0xfe;
	static uint8_t st=0;	
	static uint8_t disct=0;	
	uint8_t segcd=0;
	buf = gDisBuf;
	
	segcd = seg7codeP[buf[disct]];
	HC595_shift_byte(segcd);		  //????
	HC595_shift_byte(tmp);							  //????
	DataLaunch();
	
	if(st == 0){
		tmp = 0xfe;
		st = 1;
		disct = 0;
	}
	else if(st == 1){
		tmp = 0xfd;
		st = 2;
		disct = 1;
	}
	else if(st == 2){
		tmp = 0xfb;
		st = 3;
		disct = 2;
	}
	else if(st == 3){
		tmp = 0xf7;
		st = 0;
		disct = 3;
	}	
}

void DisPro(void){
	static uint8_t st=0;
	
 	if(gDisType == IDE_MOD){						//系统待机，显示用户余额信息XXXX
					//	gMony = 678;
		gDisBuf[3] = gMony%10;						//个
		gDisBuf[2] = (gMony%100)/10;			//十
		gDisBuf[1] = (gMony%1000)/100;		//百
		gDisBuf[0] = gMony/1000;					//千
		gDisDlyT = DIS_DLY_CONST;
		if(0 == gHoldYET){
			gMony =0;
		}		
	}
 	else if(DEC_MOD == gDisType){				//扣款成功，显示扣款金额 --XX
		gDisBuf[3] = gMonyDec%10;					//个
		gDisBuf[2] = (gMonyDec%100)/10;		//十
		gDisBuf[1] = (gMonyDec%1000)/100;		//百
		gDisBuf[0] = 17;									//-
		if(0 == gDisDlyT){
			gDisType = IDE_MOD;
			gHoldYET = DIS_DLY_5sCONST;//余额显示5s
		}		
	}
 	else if(RST_MOD == gDisType){				//显示读头id 转发器id				xxxx
		if(gDisDlyT>(DIS_DLY_CONST*2)){//显示id信息
			gDisBuf[3] = (uint8_t)(gMony)&0x0f;			
			gDisBuf[2] = (uint8_t)((gMony)&0xf0)>>4;			
			gDisBuf[1] = (uint8_t)((gMony>>8)&0x0f);		
			gDisBuf[0] = (uint8_t)(((gMony>>8)&0xf0)>>4);	
		}
		else if(gDisDlyT<(DIS_DLY_CONST*2) && (gDisDlyT >0)){
			gDisBuf[0] =(DxSwVer/10);			
			gDisBuf[1] =(DxSwVer%10);			
			gDisBuf[2] =(DxHwVer/10);			
			gDisBuf[3] =(DxHwVer%10);			
		}
		if(0 == gDisDlyT){
			gDisType = IDE_MOD;
			gMony =0;
		}
	}
 	else if(ERR_MOD == gDisType){				//显示非法卡信息 		EEEE
		gDisBuf[3] = 14;			
		gDisBuf[2] = 14;			
		gDisBuf[1] = 14;		
		gDisBuf[0] = 14;
		if(0 == gDisDlyT){
			gDisType = IDE_MOD;
			gMony =0;
		}
	}	
 	else if(NOLNK_MOD == gDisType){				//链接超时 		AAAA
		gDisBuf[3] = 10;			
		gDisBuf[2] = 10;			
		gDisBuf[1] = 10;		
		gDisBuf[0] = 10;
//		gMony = 0;
//		gMonyDec = 0;
//		gDisDlyT = DIS_DLY_CONST;
	}	
	else{
			gDisType = IDE_MOD;
	}
	
	
	if(st == 0 && gDisOFFGas == 0){		//刷新数码管
	    if(gDisScanGas == 0){
			gDisScanGas = DISSCANGAS_CONST;
			Disrefresh(gDisBuf);
			st = 1;
		}
	}
	if(st == 1 && gDisScanGas == 0){
		DisAllOFF();
		gDisOFFGas = DISOFFGAS_CONST;
		st = 0;	
	}
	
}










