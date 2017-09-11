

/*
/������ <2.4G���߶�ͷͨ��Э�� v2.7>
---------------------2016-12-09--------------------------------------------
1.�����Ϸּ��ָ��
2.���Ӳ���ָ��
3.���Ӳ����Ϸ�ָ��
4.�޸�Ϊ�ڲ�ʱ��
---------------------2017-2-04--------------------------------------------
5.���Ӳ��Կ�������Ͷ�Ҳ���
6.

*/









#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "24l01.h" 
#include "stmflash.h"
#include "rc522.h"
#include "led.h"
#include "timerx.h"
#include "BEEP.h"
#include "wdg.h"
#include "lcd.h"
#include "FramePkg.h"
#include "common.h"
#include <stdlib.h>
#include <string.h> 
#include "exti.h"

void SysReset(void);
void NetDatExp(void);
void Nrf24l01Task(void);
void RC522Task(void);
void AjstGLED(uint16_t Per);
void AjstRLED(uint16_t Per);
void AjstBLED(uint16_t Per);



QUEUE 	gCmpDwDxBuf;						//��λ���´���������
QUEUE 	gDxUpCmpBuf;						//��ͷ�ϴ���������
__ConfigVar Config;							//ϵͳ���ò���
__nrf24l01_ctr L01_Ctr;					//nrf24l01������
__SysDlyT Dly_Var;							//ϵͳ��ʱ����
__Rc522_ctr RC522Ctr;						//rc522������
__Sys_Var SysVar;								//ϵͳȫ�ֱ���
__RGB_ctr RGB_ctr;	//����LCD��Ҫ����
__net_ctr gNetCtr;
_lcd_dev lcddev;
__SysFlg gFlg;	
uint32_t ChipUniqueID[3];
uint32_t random_seed=1;
uint8_t TxMsgBuf[TX_PLOAD_WIDTH];																								//nrf24l01���ͻ�����
//led����
uint8_t leddly=0;
uint8_t stpro=0;
uint16_t rst = 1;
uint16_t gst = 2;
uint16_t bst = 3;

/*
*��ʾ�����ж�
*/
void LCDShowNo2_4gInfo(void)
{
	LCD_12864_ClearRec(0);
	LCDShowHZList(8,30,43,16,1,7);
	LCDRefresh_Gram();
}

//����ok���̵�ȫ��
void SerRxOkXfOk(void)
{	
	static uint8_t st = 0;
	//RGB_ctr.RedLightValue = rstMIN_CONST;
	if(RGB_ctr.RedFlashTim == 0){
		if(st == 0){
			st = 1;
		 	RGB_ctr.GreenLightValue = rstMAX_CONST;
			RGB_ctr.BlueLightValue = rstMIN_CONST;	
		}
		else
		{
			st = 0;
		 	RGB_ctr.GreenLightValue = rstMIN_CONST;
			RGB_ctr.BlueLightValue = rstMAX_CONST;	
		}
		RGB_ctr.RedFlashTim = RedFLASH_CONST;
	}
}
//���㣬��ɫ����
void SerRxOkNoMony(void)
{
	RGB_ctr.RedLightValue = rstMAX_CONST;
	RGB_ctr.GreenLightValue = rstMAX_CONST;
	RGB_ctr.BlueLightValue = rstMIN_CONST;	
}
//�Ƿ��������ȫ��
void SerRxOkInvalidIC(void)
{
	RGB_ctr.RedLightValue = rstMAX_CONST;
	RGB_ctr.GreenLightValue = rstMIN_CONST;
	RGB_ctr.BlueLightValue = rstMIN_CONST;	
}
//��ͷ���ϣ������˸
void ReaderErrPromt(void)
{
	static uint8_t fflg=0;
	if(Dly_Var.RED_FlashDly == 0)
	{
		Dly_Var.RED_FlashDly = RED_FLASH_CONST;
		fflg = !fflg;
		if(fflg)
		{
			RGB_ctr.RedLightValue = rstMAX_CONST;
			RGB_ctr.GreenLightValue = rstMIN_CONST;
			RGB_ctr.BlueLightValue = rstMIN_CONST;
		}
		else
		{
			RGB_ctr.RedLightValue = rstMIN_CONST;
			RGB_ctr.GreenLightValue = rstMIN_CONST;
			RGB_ctr.BlueLightValue = rstMIN_CONST;

		}
	}
}
void JianBianLightIDE_MOD(void)
{
	//��ɫled�ƿ���
	if(RGB_ctr.RedLightValue >= rstMAX_CONST)
	{
		RGB_ctr.RedLightDir = DEC_LIGHT;
	}
	else if(RGB_ctr.RedLightValue <= rstMIN_CONST)
	{
		RGB_ctr.RedLightDir = INC_LIGHT;		
	}
	RGB_ctr.RedLightDirold = RGB_ctr.RedLightDirnew;
	RGB_ctr.RedLightDirnew = RGB_ctr.RedLightDir;
	if(RGB_ctr.RedLightTim == 0)
	{
		if(RGB_ctr.RedLightDirnew != RGB_ctr.RedLightDirold)
			RGB_ctr.RedLightTim = RED_LIGHT_TIM_CONST;
		if(RGB_ctr.RedLightDir == INC_LIGHT)
		{
			if(RGB_ctr.RedLightValue> (rstMAX_CONST/2))
				RGB_ctr.RedLightStp = 2*LIGHT_STP;
			else
				RGB_ctr.RedLightStp = LIGHT_STP;
			
			RGB_ctr.RedLightValue += RGB_ctr.RedLightStp;
		}
		else
		{
			if(RGB_ctr.RedLightValue< (rstMAX_CONST/4))
			{
				RGB_ctr.RedLightStp = (LIGHT_STP/2);
			}
			else
			{
				RGB_ctr.RedLightStp = LIGHT_STP;
			}
			if(RGB_ctr.RedLightValue - RGB_ctr.RedLightStp > 0)
				RGB_ctr.RedLightValue -= RGB_ctr.RedLightStp;
		}
	}
		
	//��ɫled�ƿ���
	if(RGB_ctr.GreenLightValue >= rstMAX_CONST)
	{
		RGB_ctr.GreenLightDir = DEC_LIGHT;
	}
	else if(RGB_ctr.GreenLightValue <= rstMIN_CONST)
	{
		RGB_ctr.GreenLightDir = INC_LIGHT;		
	}
	RGB_ctr.GreenLightDirold = RGB_ctr.GreenLightDirnew;
	RGB_ctr.GreenLightDirnew = RGB_ctr.GreenLightDir;
	if(RGB_ctr.GreenLightTim == 0)
	{
		if(RGB_ctr.GreenLightDirnew != RGB_ctr.GreenLightDirold)
			RGB_ctr.GreenLightTim = GREEN_LIGHT_TIM_CONST;
		if(RGB_ctr.GreenLightDir == INC_LIGHT)
		{
			if(RGB_ctr.GreenLightValue> (rstMAX_CONST/2))
				RGB_ctr.GreenLightStp = 2*LIGHT_STP;
			else
				RGB_ctr.GreenLightStp = LIGHT_STP;
			
			RGB_ctr.GreenLightValue += RGB_ctr.GreenLightStp;
		}
		else
		{
			if(RGB_ctr.GreenLightValue< (rstMAX_CONST/4))
			{
				RGB_ctr.GreenLightStp = (LIGHT_STP/2);
			}
			else
			{
				RGB_ctr.GreenLightStp = LIGHT_STP;
			}
			if(RGB_ctr.GreenLightValue - RGB_ctr.GreenLightStp > 0)
				RGB_ctr.GreenLightValue -= RGB_ctr.GreenLightStp;
		}
	}
	
	
	//��ɫled�ƿ���
	if(RGB_ctr.BlueLightValue >= rstMAX_CONST)
	{
		RGB_ctr.BlueLightDir = DEC_LIGHT;
	}
	else if(RGB_ctr.BlueLightValue <= rstMIN_CONST)
	{
		RGB_ctr.BlueLightDir = INC_LIGHT;		
	}
	RGB_ctr.BlueLightDirold = RGB_ctr.BlueLightDirnew;
	RGB_ctr.BlueLightDirnew = RGB_ctr.BlueLightDir;
	if(RGB_ctr.BlueLightTim == 0)
	{
			
		if(RGB_ctr.BlueLightDir == INC_LIGHT)
		{
			if(RGB_ctr.BlueLightValue> (rstMAX_CONST/2))
				RGB_ctr.BlueLightStp = 2*LIGHT_STP;
			else
				RGB_ctr.BlueLightStp = LIGHT_STP;
			
			RGB_ctr.BlueLightValue += RGB_ctr.BlueLightStp;
		}
		else
		{
				if(RGB_ctr.BlueLightDirnew != RGB_ctr.BlueLightDirold)
						RGB_ctr.BlueLightTim = BLUE_LIGHT_TIM_CONST;
				if(RGB_ctr.BlueLightValue< (rstMAX_CONST/4))
				{
					RGB_ctr.BlueLightStp = (LIGHT_STP/2);
				}
				else
				{
					RGB_ctr.BlueLightStp = LIGHT_STP;
				}
				if(RGB_ctr.BlueLightValue - RGB_ctr.BlueLightStp > 0)
					RGB_ctr.BlueLightValue -= RGB_ctr.BlueLightStp;
		}
	}
		
		
	
// 	RGB_ctr.RedLightValue = 		rstMAX_CONST;
// 	RGB_ctr.GreenLightValue = 	rstMAX_CONST;
// 	RGB_ctr.BlueLightValue = 	rstMAX_CONST;
	
	
// 	if(RGB_ctr.BlueLightDir == INC_LIGHT && RGB_ctr.GreenLightDir == INC_LIGHT && RGB_ctr.RedLightDir == INC_LIGHT)
// 	{
// 		if((RGB_ctr.BlueLightValue > rstMAX_CONST/2) && ((RGB_ctr.RedLightValue > rstMAX_CONST/2) && (RGB_ctr.GreenLightValue > rstMAX_CONST/2)))
// 		{
// 			RGB_ctr.RedLightDir = DEC_LIGHT;
// 			RGB_ctr.GreenLightDir = DEC_LIGHT;
// 		}
// 	}
// 	
// 	if(RGB_ctr.BlueLightDir == DEC_LIGHT && RGB_ctr.GreenLightDir == DEC_LIGHT && RGB_ctr.RedLightDir == DEC_LIGHT)
// 	{
// 		if((RGB_ctr.BlueLightValue < rstMAX_CONST/4) && ((RGB_ctr.RedLightValue > rstMAX_CONST/4) && (RGB_ctr.GreenLightValue > rstMAX_CONST/4)))
// 		{
// 			RGB_ctr.GreenLightDir = INC_LIGHT;
// 		}
// 	}
		
// 	if(RGB_ctr.BlueLightDir == INC_LIGHT && RGB_ctr.GreenLightDir == INC_LIGHT && RGB_ctr.RedLightDir == INC_LIGHT)
// 	{
// 		if((RGB_ctr.BlueLightValue > rstMAX_CONST/2) && ((RGB_ctr.RedLightValue > rstMAX_CONST/2) && (RGB_ctr.GreenLightValue > rstMAX_CONST/2)))
// 		{
// 			RGB_ctr.RedLightDir = DEC_LIGHT;
// 			RGB_ctr.GreenLightDir = DEC_LIGHT;
// 		}
// 	}
	
	
	if(RGB_ctr.BlueLightDir == INC_LIGHT && RGB_ctr.GreenLightDir == INC_LIGHT && RGB_ctr.RedLightDir == INC_LIGHT)//��ɫͬʱ����ʱ��ɫ�ƽ���
		RGB_ctr.RedLightDir = DEC_LIGHT;
// 	if(RGB_ctr.BlueLightDir == DEC_LIGHT && RGB_ctr.GreenLightDir == DEC_LIGHT && RGB_ctr.RedLightDir == INC_LIGHT)//ֻ�к�ɫ������ʱ��ɫ��ͬʱ����
// 		RGB_ctr.BlueLightDir = INC_LIGHT;
	if(RGB_ctr.BlueLightDir == DEC_LIGHT && RGB_ctr.GreenLightDir == DEC_LIGHT && RGB_ctr.RedLightDir == DEC_LIGHT)//��ɫͬʱ����ʱ��ɫ������
		RGB_ctr.BlueLightDir = INC_LIGHT;
	
	
	
//	AjstRLED(RGB_ctr.RedLightValue);
//	AjstGLED(RGB_ctr.GreenLightValue);
//	AjstBLED(RGB_ctr.BlueLightValue);

}
/*
*��ʾˢ�������ϣ�����ϵ����Ա
*/
void LCDShowReaderErr(void)
{
	LCDShowHZList(18,20,40,16,1,5);
	LCDShowHZList(8,40,45,16,1,6);
}
/*
*��ʾˢ��ʧ��
*/
void LCDShowReaderFailed(void)
{
	LCDShowHZList(25,30,51,16,1,4);
}
/*
*��ʾ��Ƭ����
*/
void LCDShowICOverTim(void)
{
	LCDShowHZList(25,30,61,16,1,4);
}
/*
*��ʾ��Ƭ��ʧ
*/
void LCDShowICLoss(void)
{
	LCDShowHZList(25,30,57,16,1,4);
}


void LCDShowCoinSum(void)
{
// 		/* ���ڼ��� */
// 		LCDShowHZList(95,16,67,16,DIS_NORMAL,2);
// 		LCD_ShowxNum(15,16,SysVar.PlusCntSum,7,12,DIS_NORMAL|NUM_FILL_0);//���ϴ�������


// 		/* �Ѿ��������� */
// 		LCDShowHZList(95,36,65,16,DIS_NORMAL,2);//�Ѿ���������
// 		LCD_ShowxNum(15,36,SysVar.xf,7,12,DIS_NORMAL|NUM_FILL_0);//�Ѿ���������
// 	 
// 		/* ��Ʊ���� */
// 		LCDShowHZList(95,50,67,16,DIS_NORMAL,2);
// 		LCD_ShowxNum(15,50,SysVar.ye,7,12,DIS_NORMAL|NUM_FILL_0);//���ϴ�������
	
	
		/* ���ڼ��� */
		LCDShowHZList(15,20,67,16,DIS_NORMAL,2);
 		LCD_ShowStringAndBk(50,22,10,12,12,":",DIS_NORMAL);
		LCD_ShowxNum(50,16,SysVar.PlusCntSum,5,24,DIS_NORMAL);//���ϴ�������

	 
		/* ��Ʊ���� */
		LCDShowHZList(15,45,6,16,DIS_NORMAL,2);
 		LCD_ShowStringAndBk(50,47,10,12,12,":",DIS_NORMAL);
		LCD_ShowxNum(50,45,SysVar.ye,8,16,DIS_NORMAL);//���ϴ�������
}


/*
*��ʾ��������
*/
void LCDShowIDEInfo(void)
{
	switch(Config.RunMod){
		
		case RunMod_NORMAL:/* ��ͨ��ͷ */
			LCDShowHZList(100,40,90,16,DIS_NORMAL,1);
			LCD_DrawLine(105,25,90,50,DIS_NORMAL);
 			LCD_ShowxNum(6,20,Config.DecMon/100,3,32,DIS_NORMAL);
 			LCD_ShowStringAndBk(56,20,10,12,32,".",DIS_NORMAL);
 			LCD_ShowxNum(64,20,((Config.DecMon%100)/10),1,32,DIS_NORMAL);

			switch(Config.Price)
			{
				case 0:
					break;
				case BI:
					LCDShowHZList(80,25,55,16,DIS_NORMAL,1);
					break;
				case YUAN:
					LCDShowHZList(80,25,56,16,DIS_NORMAL,1);
					break;
				default:
					break;
			}
		break;
		case RunMod_CHECK:/* ��ѯ�� */
			LCDShowHZList(30,25,82,16,DIS_NORMAL,3);//��ʾ���Ѷ��

		break;
		case RunMod_SAVER:   /* ��Ʊ�� */
		LCDShowCoinSum();
		// 			LCDShowHZList(10,20,91,16,DIS_NORMAL,6);
// 			LCDShowHZList(95,45,67,16,DIS_NORMAL,2);
// 			LCD_ShowxNum(8,40,SysVar.PlusCntSum,7,24,DIS_NORMAL);

		break;
		default:
		break;
	}
}


/*
 *��ʾ�Ƿ�����Ϣ
 */
void LCDShowInvalidICInfo(void)
{
// 	uint32_t sntmp = 0;
// 	
// 	sntmp = (uint32_t)RC522Ctr.SN[3];
// 	sntmp <<= 8;
// 	sntmp |= (uint32_t)RC522Ctr.SN[2];
// 	sntmp <<= 8;
// 	sntmp |= (uint32_t)RC522Ctr.SN[1];
// 	sntmp <<= 8;
// 	sntmp |= (uint32_t)RC522Ctr.SN[0];
	
	LCDShowHZList(35,15,21,16,1,4);//��ʾ�Ƿ���
	LCD_ShowStringAndBk(10,40,40,16,16,"Sn:",DIS_NORMAL);
	
	LCD_ShowxNum(33,40,SysVar.icsn,10,16,DIS_NORMAL|NUM_FILL_0);
}
/*
*��ʾ�û������Ϣ
*/
void LCDShowICInfo(void)
{
	LCDShowHZList(23,15,70,16,DIS_NORMAL,4);//��ʾˢ���ɹ�
//	LCDShowHZList(13,32,6,16,DIS_NORMAL,3);
	
// 	LCD_ShowxNum(13,15,SysVar.xf/10,4,32,DIS_NORMAL|NUM_FILL_0);
// 	LCD_ShowStringAndBk(76,32,10,12,32,".",DIS_NORMAL);
// 	LCD_ShowxNum(82,15,SysVar.xf%10,1,32,DIS_NORMAL);
	
	
	
	LCD_ShowxNum(13,32,SysVar.ye/100,4,32,DIS_NORMAL);
	LCD_ShowStringAndBk(76,32,10,12,32,".",DIS_NORMAL);
	LCD_ShowxNum(82,32,(SysVar.ye%100)/10,1,32,DIS_NORMAL);

	switch(Config.Price)
	{
		case 0:
			break;
		case BI:
			LCDShowHZList(100,37,55,16,DIS_NORMAL,1);
			break;
		case YUAN:
			LCDShowHZList(100,37,56,16,DIS_NORMAL,1);
			break;
		default:
			break;
	}
}
/*
*��ʾ�û�����
*/
void LCDShowNoMonyInfo(void)
{
	LCDShowHZList(23,15,16,16,DIS_NORMAL,5);//��ʾ���㣡
// 	LCDShowHZList(13,40,6,16,DIS_NORMAL,1);	
// 	LCD_ShowStringAndBk(30,40,40,16,16,":",DIS_NORMAL);
	
	LCD_ShowxNum(13,32,SysVar.ye/100,4,32,DIS_NORMAL);
	LCD_ShowStringAndBk(76,32,10,12,32,".",DIS_NORMAL);
	LCD_ShowxNum(82,32,((SysVar.ye%100)/10),1,32,DIS_NORMAL);

	switch(Config.Price)
	{
		case 0:
			break;
		case BI:
			LCDShowHZList(100,37,55,16,DIS_NORMAL,1);
			break;
		case YUAN:
			LCDShowHZList(100,37,56,16,DIS_NORMAL,1);
			break;
		default:
			break;
	}
}


/*
*��ʾic�����Ѿ�¼�룬���Ʊ
*/
void LCDShowCntTickit(void)
{
	LCDShowHZList(23,15,91,16,DIS_NORMAL,5);//������¼��
	LCDShowHZList(23,38,96,16,DIS_NORMAL,5);//�뿪ʼ��Ʊ
}
/*
*ˢ��̫Ƶ�������Ժ�����
*/
void LCDShowOpTooOFent(void)
{
	LCDShowHZList(23,15,91,16,DIS_NORMAL,5);//������¼��
	LCDShowHZList(23,38,96,16,DIS_NORMAL,5);//�뿪ʼ��Ʊ
}


/*
 *��ʾID
 *
 */

void ShowSysID(void)
{
		LCD_DrawLine(0,12,127,12,DIS_NORMAL);		
		LCD_ShowStringAndBk(15,0,lcddev.width,12,12,"ID:",DIS_NORMAL);	
		LCD_ShowxNum(35,0,Config.DxID,10,12,DIS_NORMAL|NUM_FILL_0);//ϵͳid����ʾ	
}
/*
 *��ʾˢ������
 *
 */
void ShowSysICFlg(void)
{
		if(RC522Ctr.GetIcNum == 1)
		{
			RC522Ctr.GetIcNum = 0;
			LCD_DrawRectangle(104,3,109,8,DIS_NORMAL);
		}
		else
		{
			LCD_DrawRectangle(104,3,109,8,DIS_INSER);
		}
}

/*
 *��ʾ���ͽ��ռ�ͷ
 *
 */
void ShowSysTxRx(void)
{
		if(gNetCtr.netsta & NetRxOk)//���ռ�ͷ
		{
			gNetCtr.netsta &= ~NetRxOk;
			LCD_DrawLine(98,2,98,8,DIS_NORMAL);
			LCD_DrawLine(98,8,96,6,DIS_NORMAL);
		}
		else
		{
			LCD_DrawLine(98,2,98,8,DIS_INSER);
			LCD_DrawLine(98,8,96,6,DIS_INSER);
		}
		
		if(gNetCtr.netsta & NetTxOk)//���ͼ�ͷ
		{
			gNetCtr.netsta &= ~NetTxOk;
			LCD_DrawLine(100,2,100,8,DIS_NORMAL);
			LCD_DrawLine(100,2,102,6,DIS_NORMAL);
		}
		else
		{
			LCD_DrawLine(100,2,100,8,DIS_INSER);
			LCD_DrawLine(100,2,102,6,DIS_INSER);
		}
}

/*
*��ʾ�����ж�
*/
void LCDShowNoLnkInfo(void)
{
	LCDShowHZList(13,15,25,16,DIS_NORMAL,5);
	
}

void LCDDisplayTask(void)
{	
		static uint8_t tyo,tyn;
		tyo = tyn;
		tyn = SysVar.DisType;
		if(tyn != tyo)
		{
				LCD_12864_ClearRec(0);
				Dly_Var.LCDIdeT = (Config.PromptTim*100);
		}
		else if(Dly_Var.LCDIdeT == 0)
		{
			if(SysVar.DisType != ERR_PROMPT)
				SysVar.DisType = IDE_MOD;
		}
		
		switch(SysVar.DisType)
		{
				case IDE_MOD:
					LCDShowIDEInfo();
					SysVar.RGB_DisType = RGB_IDE_MOD;
					break;
				case Invalid_IC_MOD:
					LCDShowInvalidICInfo();
					SysVar.RGB_DisType = RGB_Invalid_IC_MOD;
					break;
				case NOLNK_MOD:
					LCDShowNoLnkInfo();
					SysVar.RGB_DisType = RGB_NOLNK_MOD;
					break;
				case CONI_SUM_DIS://��ʾ��������
					LCDShowCoinSum();
					break;
				case ICINFO_MOD:
					SysVar.RGB_DisType = RGB_ICINFO_MOD;
					LCDShowICInfo();
					break;
				case NOMON_MOD:
					LCDShowNoMonyInfo();
					SysVar.RGB_DisType = RGB_NOMON_MOD;
					break;
				case ERR_PROMPT:
					LCDShowReaderErr();
					SysVar.RGB_DisType = RGB_ERR_PROMPT;
					break;
				case UNKNOW_ERR:
					LCDShowReaderFailed();
					SysVar.RGB_DisType = RGB_UNKNOW_ERR;
					break;
				case ERR_LOSS_DIS:
					LCDShowICLoss();
					SysVar.RGB_DisType = RGB_ERR_LOSS_DIS;
					break;
				case ERR_OVER_DIS:
					LCDShowICOverTim();
					SysVar.RGB_DisType = RGB_ERR_OVER_DIS;
					break;
				case CNT_TEMP_DIS:
					SysVar.RGB_DisType = RGB_CNT_TEMP_DIS;
					LCDShowCntTickit();
					break;
				case OPTOO_OFENT:
					//SysVar.RGB_DisType = RGB_CNT_TEMP_DIS;
					LCDShowOpTooOFent();
					break;
				default:
					break;
		}		
		
		LCD_ShowxNum(106,0,SysVar.PlusNum,3,12,DIS_NORMAL | NUM_FILL_0);
		/* ϵͳ��������ʾ */
		if(Dly_Var.ToolBarDly == 0)
		{
			Dly_Var.ToolBarDly = 400;
			ShowSysID();
			ShowSysICFlg();
			ShowSysTxRx();
		}
		LCDRefresh_Gram();	
}



/////////////////////////////////////////////////////////////////////
//��    �ܣ�ϵͳָʾ����ʾ���
//����˵��: Dly_Var.gLED_Gas ϵͳ����ָʾ����ʱ����				
//					Dly_Var.RC522FlashDlyrc522 ����״ָ̬ʾ����ʱ����      
void LEDTask(void)
{
	if(Dly_Var.gLED_Gas == 0)
	{
		Dly_Var.gLED_Gas = LED_GAS_CONST;
		SYS_LED = !SYS_LED;
	}
}

/////////////////////////////////////////////////////////////////////
//��    �ܣ�д��ϵͳ���ò���
//����˵��: 				
//					     
void WriteParm(void)
{	
	uint16_t wtbf[FLASH_DAT_LEN];
	wtbf[0]= (uint16_t)Config.tagflg;							  
	wtbf[1] = (uint16_t)Config.Price;					//							
	wtbf[2] = (uint16_t)Config.DxID;					//����ԴĬ��Ϊ D10											
	wtbf[3] = (uint16_t)Config.CoinX;   			//Ĭ��ÿ�εο��ϱ�1��						
	wtbf[4] = (uint16_t)Config.CoinOutLevel;	//Ĭ���ϱ��źų�̬�͵�ƽ								
	wtbf[5] = (uint16_t)Config.CoinSigHod;		//Ĭ���ϱ��ź�ά��ʱ��4x10=40ms						
	wtbf[6] = (uint16_t)Config.DeviceType;		//����ѯ����־ ,Ĭ��Ϊ������ѯ��									
	wtbf[7] = (uint16_t)Config.DxSwVer;				//����ѯ����־ ,Ĭ��Ϊ������ѯ��									
	wtbf[8] = (uint16_t)Config.DxHwVer;				//����ѯ����־ ,Ĭ��Ϊ������ѯ��									
	wtbf[9] = (uint16_t)Config.disdir;				//					
	wtbf[10] = (uint16_t)Config.PromptTim;		//					
	wtbf[11] = (uint16_t)Config.CoinSigGas;		//					
	wtbf[12] = (uint16_t)Config.DxFQ;					//					
	wtbf[13] = (uint16_t)Config.WxFQ;					//					
	wtbf[14] = (uint16_t)Config.RunMod;
	wtbf[15] = (uint16_t)Config.DecMon;
	
	STMFLASH_Write(STM32_FLASH_BASE,wtbf,FLASH_DAT_LEN);
	
//  	wtbf[0] = (uint16_t)(Config.ICSn>>16);
// 	wtbf[1] = (uint16_t)(Config.ICSn>>0);
//  	wtbf[2] = (uint16_t)(Config.PlusCntSum>>16);
//  	wtbf[3] = (uint16_t)(Config.PlusCntSum>>0);
// 	STMFLASH_Write(STM32_FLASH_BASE+FLASH_DAT_LEN,wtbf,4);
	
}
/////////////////////////////////////////////////////////////////////
//��    �ܣ�ϵͳ���ò�������
//����˵��: 				
//					     
void ExplainParm(void)
{
	uint16_t expbf[FLASH_DAT_LEN];
	STMFLASH_Read(STM32_FLASH_BASE,expbf,FLASH_DAT_LEN);//��ȡϵͳ����	
	Config.tagflg = (uint8_t)expbf[0];							  
	Config.Price = (uint8_t)expbf[1];;		//����ԴĬ��Ϊ W10									
	Config.DxID = expbf[2];;		//����ԴĬ��Ϊ D10											
	Config.CoinX = (uint8_t)expbf[3];;   //Ĭ��ÿ�εο��ϱ�1��						
	Config.CoinOutLevel = (uint8_t)expbf[4];;	//Ĭ���ϱ��źų�̬�͵�ƽ								
	Config.CoinSigHod = expbf[5];;		//Ĭ���ϱ��ź�ά��ʱ��4x10=40ms						
	Config.DeviceType = (uint8_t)expbf[6];;		//����ѯ����־ ,Ĭ��Ϊ������ѯ��									
	Config.DxSwVer = (uint8_t)expbf[7];;		//����ѯ����־ ,Ĭ��Ϊ������ѯ��									
	Config.DxHwVer = (uint8_t)expbf[8];;		//����ѯ����־ ,Ĭ��Ϊ������ѯ��			
	Config.disdir = (uint8_t)expbf[9];		//					
	Config.PromptTim = (uint16_t)expbf[10];		//					
	Config.CoinSigGas = expbf[11];		//					
	COIN_SIG1 = !Config.CoinOutLevel;				//��ʼ��Ͷ�ҿڵ�ƽ״̬
	Config.DxFQ = (uint8_t)expbf[12];		//					
	Config.WxFQ = (uint8_t)expbf[13];		//					
	Config.RunMod = (uint8_t)expbf[14];
	Config.DecMon = expbf[15];
	
// 	STMFLASH_Read(STM32_FLASH_BASE+FLASH_DAT_LEN,expbf,4);//��ȡϵͳ����	
// 	
// 	/* �û�ic���� */
// 	Config.ICSn = expbf[0];
// 	Config.ICSn <<= 16;
// 	Config.ICSn |= expbf[1];
// // 	/* ��Ʊ���� */
// 	Config.PlusCntSum = expbf[2];
// 	Config.PlusCntSum <<= 16;
// 	Config.PlusCntSum |= expbf[3];
// 	SysVar.PlusCntSum = Config.PlusCntSum;
}
/////////////////////////////////////////////////////////////////////
//��    �ܣ���ӡ���ϵͳ������Ϣ
//����˵��: 				
//					     
void PrintCfgPar(void)
{
 	printf("��ȡϵͳ����---------------------------------:\r\n");	
	printf("ϵͳ��־��0x%x\r\n",Config.tagflg);
	printf("��ͷID��%d\r\n",Config.DxID);
	printf("���ѽ�%d��\r\n",Config.DecMon);
	
	printf("Ͷ�ұ��ʣ�%d\r\n",Config.CoinX);
	printf("Ͷ�ҿڳ�̬��ƽ��%d\r\n",Config.CoinOutLevel);
	printf("�ϱҿ�������ʱ�䣺%d ms\r\n",Config.CoinSigGas);
	printf("�ϱҿ��������ʱ�䣺%dms\r\n",Config.CoinSigHod);
	printf("��ͷ���ͣ�%d\r\n",Config.DeviceType);
	printf("����汾�ţ�%d  Ӳ���汾�ţ�%d \r\n",Config.DxSwVer,Config.DxHwVer);
	printf("��ʾͣ��ʱ�䣺%d s\r\n",Config.PromptTim);
	printf("��ͷƵ����%d \r\n",Config.DxFQ);
	printf("�û�ic����Ϊ��%d \r\n",Config.ICSn);
	printf("�û���Ʊ��Ϊ��%d \r\n",Config.FPlusCntSum);
	printf("�����ʾ��λ��");
	switch(Config.Price)
	{
		case 0:
			printf("��\r\n");
			break;
		case 1:
			printf("Ԫ\r\n");
			break;
		case 2:
			printf("��\r\n");
			break;
		default:
			printf("δ����\r\n");			
			break;
	}
	printf("���ۣ�%d \r\n",Config.Price);
	
	printf("��ʾ����");
	if(Config.disdir)
		printf("����\r\n");
	else
		printf("����\r\n");
}
/////////////////////////////////////////////////////////////////////
//��    �ܣ�����Ĭ��ϵͳ����
//����˵��: 				
//					     
void WriteDefaultSet(void)
{
	Config.tagflg = DEFAULT_TAG_FLG;//flash��־  
	Config.Price = DEFAULT_Price;//Ĭ������id  	120
	Config.DxID = DEFAULT_DX_ID;//Ĭ�϶�ͷid  	6
	Config.CoinX = 1;						//Ĭ���ϱұ��� 	1
	Config.CoinOutLevel = 1;		//Ĭ��Ͷ�ҿڳ�̬��ƽ ��
	Config.CoinSigHod = 80;			//Ĭ��Ͷ�������� 4*10ms=40ms
	Config.CoinSigGas = 300;			//Ĭ��Ͷ�������� 4*10ms=40ms
	
	Config.DeviceType = 0;			//Ĭ��Ϊ��������   1,Ϊ����ѯ��
	Config.DxSwVer = DxSwVerDEF;				//sw v2.0
	Config.DxHwVer = DxHwVerDEF;				//sw v2.0
	Config.disdir = 1;				//sw v2.0
	Config.PromptTim = HOLD_MOD_CONST;
	Config.DxFQ = DEFAULT_DxFQ_NUM;
	Config.WxFQ = DEFAULT_WxFQ_NUM;
	//Config.RunMod = RunMod_NORMAL;
	Config.RunMod = RunMod_SAVER;
	
	Config.DecMon = 350;
	Config.ICSn = 0;
	Config.FPlusCntSum = 0;
	WriteParm();
}
/////////////////////////////////////////////////////////////////////
//��    �ܣ�����ϵͳ���ò���
//����˵��: 				
//					     
void LoadConfigPar(void)
{
	uint16_t i;
	STMFLASH_Read(STM32_FLASH_BASE,&i,1);//��ȡ��־
	if(DEFAULT_TAG_FLG != i)//mcuδ��ʼ��������Ĭ��
	{		
#if MAIN_DBG
		printf("new mcu witting...\r\n");		
#endif
		WriteDefaultSet();
	}
	ExplainParm();
	PrintCfgPar();
}
/////////////////////////////////////////////////////////////////////
//��    �ܣ���ȡоƬid��
//����˵��: 				
//					     
void getChipID(void)
{
		ChipUniqueID[2] = *(__IO u32*)(0X1FFFF7E8);  
		ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC); 
		ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0); 
		random_seed = ChipUniqueID[2] | ChipUniqueID[1] | ChipUniqueID[0];
}

void SysClkSet(void)
{
	 RCC_DeInit(); 				/*������RCC�Ĵ�������Ϊȱʡֵ */ 
   RCC_HSICmd(ENABLE); 
   while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY)== RESET);//�ȴ�HSI���� 
   RCC_HCLKConfig(RCC_SYSCLK_Div1);   /*����AHBʱ�ӣ�HCLK�� RCC_SYSCLK_Div1����AHBʱ�� = ϵͳʱ*/  
   RCC_PCLK2Config(RCC_HCLK_Div1);   /* ���ø���AHBʱ�ӣ�PCLK2)RCC_HCLK_Div1����APB2ʱ�� = HCLK*/     
   RCC_PCLK1Config(RCC_HCLK_Div2); /*���õ���AHBʱ�ӣ�PCLK1��RCC_HCLK_Div2����APB1ʱ�� = HCLK / 2*/      
   FLASH_SetLatency(FLASH_Latency_2);   /*����FLASH�洢����ʱʱ��������FLASH_Latency_2  2��ʱ����*/   
   FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);  /*ѡ��FLASHԤȡָ�����ģ,Ԥȡָ����ʹ��*/ 
   RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);/*����PLLʱ��Դ����Ƶϵ����Ƶ��Ϊ8/2*16=64Mhz*/    
   RCC_PLLCmd(ENABLE); 	 /*ʹ��PLL */ 
   while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) ; /*���ָ����RCC��־λ(PLL׼���ñ�־)�������*/    
   RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);  /*����ϵͳʱ�ӣ�SYSCLK�� */  
   while(RCC_GetSYSCLKSource() != 0x08);     /*0x08��PLL��Ϊϵͳʱ�� */	    
}

////////////////////////////////?��???����y2�������㨬����////////////////////////////////
//??��??��???����y
//?����?2������0~RANDOM_MAX-1��????����y
//seed:??����
//max:��?�䨮?��	  		  
//����???��:0~(max-1)?D��?��????�� 		
uint16_t app_get_rand(uint16_t max,uint16_t min,uint16_t rand)
{			    	    
	rand=rand*22695477+1;

	return ((uint16_t)(rand)%(max-min+1)+min); 
}  



void PVD_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP,ENABLE);//PWR��1?��
	
	PWR_PVDLevelConfig(PWR_PVDLevel_2V9);
	PWR_PVDCmd(ENABLE);

	/* Configure EXTI Line16(PVD Output) to generate an interrupt on rising and
	 falling edges */
  EXTI_ClearITPendingBit(EXTI_Line16); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line16;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

void PVD_NVIC(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  
  /* Enable the PVD Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void PVD_IRQHandler(void)
{
// 	uint16_t wtbf[4];
// 	uint16_t rdbf[4];
// 	
// 	wtbf[0] = (uint16_t)RC522Ctr.SN[3];
// 	wtbf[0] <<= 8;
// 	wtbf[0] |= (uint16_t)RC522Ctr.SN[2];
// 	
// 	wtbf[1] = (uint16_t)RC522Ctr.SN[1];
// 	wtbf[1] <<= 8;
// 	wtbf[1] |= (uint16_t)RC522Ctr.SN[0];
	
	
// 	wtbf[2] = (uint16_t)(SysVar.PlusCntSum>>16);
// 	wtbf[3] = (uint16_t)(SysVar.PlusCntSum>>0);
// 	wtbf[2] = 5;
//  	wtbf[3] = 1;
// 	STMFLASH_WriteFast(STM32_FLASH_BASE+FLASH_DAT_LEN,wtbf,4);	
// 	
// 	STMFLASH_Read(STM32_FLASH_BASE+FLASH_DAT_LEN,rdbf,4);
// 	printf("SysVar.PlusCntSum=%d,rdbf[0]=%d,rdbf[1]=%d,rdbf[2]=%d,rdbf[3]=%d,\r\n",SysVar.PlusCntSum,rdbf[0],rdbf[1],rdbf[2],rdbf[3]);
// 	printf("wtbf[0]=%d,wtbf[1]=%d,wtbf[2]=%d,wtbf[3]=%d,\r\n",wtbf[0],wtbf[1],wtbf[2],wtbf[3]);
	EXTI_ClearITPendingBit(EXTI_Line16);
}
void SetKeyWord(void)
{
	SysVar.ICKeyA[0] = 255;
	SysVar.ICKeyA[1] = 255;
	SysVar.ICKeyA[2] = 255;
	SysVar.ICKeyA[3] = 255;
	SysVar.ICKeyA[4] = 255;
	SysVar.ICKeyA[5] = 255;
	
	SysVar.ICKeyB[5] = 0xa2;
	SysVar.ICKeyB[4] = 0x89;
	SysVar.ICKeyB[3] = 0x77;
	SysVar.ICKeyB[2] = 0xc9;
	SysVar.ICKeyB[1] = 0xd1;
	SysVar.ICKeyB[0] = 0x55;
}
int main(void)
{
	SysClkSet();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIOInit();
	delay_init();	    	 																			//��ʱ������ʼ��	  
	NVIC_Configuration(); 	 																	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	
// 	PVD_Init();
// 	PVD_NVIC();


	uart_init(115200);	 																			//���ڳ�ʼ��Ϊ115200
	CreateLanQueue(&gCmpDwDxBuf);															//������λ���·������������
	CreateLanQueue(&gDxUpCmpBuf);															//������ͷ�ϴ������������
	memset(&RC522Ctr.SN[0],0,4);
	SysVar.icsn = 0;
	LoadConfigPar();		
	InitRc522();													
 	TIM3_Int_Init(100,639);																	//100Khz����Ƶ�ʣ�������1000Ϊ10ms
	LCD_Init();
	NRF24L01_Init();    																			//��ʼ��NRF24L01 
 	while(NRF24L01_Check())																		//���NRF24L01�Ƿ���λ.	
 {						
#if MAIN_DBG 		
		printf("NRF24L01 Error\r\n");
#endif
		LCDShowNo2_4gInfo();
		BeepCtr(1);
		delay_ms(200);
		BeepCtr(0);
		delay_ms(200);
	}
	SysVar.BeepType = B;
	SysVar.DisType = IDE_MOD;
	Dly_Var.LCDIdeT = (Config.PromptTim*100);
	IWDG_Init(4,625);     //���Ƶ��Ϊ64,����ֵΪ625,���ʱ��Ϊ1s	   
	FrameUpDxConfig();
	SetKeyWord();
	delay_ms(1000);
	EXTIX_Init();
	while(1)
	{
			if(Dly_Var.gRc522RdGas == 0){
				IWDG_Feed();
				if(L01_Ctr.TxDatBufFlg == 0){
						RC522Task();								//rfid��������
				}
				Dly_Var.gRc522RdGas = Rc522Rd_GAS_CONST;
			}
		
 			LEDTask();											//ϵͳ����״ָ̬ʾ����
 			NetDatExp();
			Nrf24l01Task();									//2.4gͨ������
			LCDDisplayTask();
			BeepTask();											//��������ʾ����
			RGBCtrTask();
	}
}


#define  DEC_CNT_MAX 	5			//���λֵ
#define  IC_KEY_DATA_ADDR 	7
#define  IC_TYPE_DATA_ADDR 	6

/////////////////////////////////////////////////////////////////////
//��    �ܣ�ic����������
//����˵��: 				
//		

// uint8_t* const ictag ={"huibaitech2016"};
// uint8_t* const hzbuf ={"��ٿ�"};
// uint8_t KEYA[16]=  {0x55,0xd1,0xc9,0x77,0x89,0xa2,0xff,0x07,0x80,0x69,0x55,0xd1,0xc9,0x77,0x89,0xa2};																								//nrf24l01���ͻ�����
// uint8_t KEYAbk[16]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};																								//nrf24l01���ͻ�����
// uint8_t RFIDDAT[16]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};																								//nrf24l01���ͻ�����
// #define KEYADDR  	0x03
// #define DATAADDR  0x02

void RC522Task(void){		
	
	if(PcdRequest(PICC_REQIDL,&RC522Ctr.CT[0]) == MI_OK){
		if((PcdAnticoll(&RC522Ctr.SN[0])) == MI_OK){
			if(PcdSelect(&RC522Ctr.SN[0]) == MI_OK){
				RC522Ctr.GetIcNum = 1;
				
				if(PcdAuthState(KEYA_CHK,IC_KEY_DATA_ADDR,&SysVar.ICKeyB[0],&RC522Ctr.SN[0])== MI_OK){
					if(PcdRead(IC_TYPE_DATA_ADDR,&RC522Ctr.RFID[0]) == MI_OK){	/* ��ȡ���������ݣ��ж��Ƿ�Ϊ���� */
						//printf("ic������������:");
						//showdbg8hex(RC522Ctr.RFID,16);     
						if(RC522Ctr.RFID[0] ==0x55 && RC522Ctr.RFID[1] == 0xaa){/* ƥ�䵽��ʶ�����ǹ���,����ͷ�ϱ� 1 ��*/
								SysVar.PlusNum += 1;
								SysVar.BeepType = B;					//��ʾˢ���ɹ�
								SysVar.DisType = ICINFO_MOD;
								Dly_Var.LCDIdeT = (Config.PromptTim*100);
								SysVar.ye = 0;
								SysVar.xf = 0;
								PcdHalt();
						}else if(SysVar.DisType == IDE_MOD){//����ͣ���������ʾ���治����ˢ������
							SysVar.icsn_old = SysVar.icsn_new;
							SysVar.icsn = ((uint32_t)RC522Ctr.SN[0]<<24)+((uint32_t)RC522Ctr.SN[1]<<16)+((uint32_t)RC522Ctr.SN[2]<<8)+((uint32_t)RC522Ctr.SN[3]<<0); 
							SysVar.icsn_new = SysVar.icsn;
							L01_Ctr.ReTxcnt = ReTxcntMAX;//��ǰָ������ط�����
							
							switch(Config.RunMod){
								case RunMod_NORMAL:
									FrameUpDxICImfor(SysVar.icsn,++SysVar.UpDatCnt);//�õ�ic���ţ��ϴ��û���Ϣ
								break;
								case RunMod_CHECK:/* ��ѯ�� */
								break;
								case RunMod_SAVER:	 /* ��Ʊ�� */
									if(SysVar.CntFlg == 2){
										if(SysVar.PlusCntSum == 0){
											SysVar.DisType = CNT_TEMP_DIS;
										}
										FrameUpDxCoinSum(SysVar.icsn,SysVar.PlusCntSum,++SysVar.UpDatCnt);//�õ�ic���ţ��ϴ��û���Ϣ
										SysVar.icsn = 0;
										SysVar.CntFlg = 0;
									}
									else 
									{
										if(SysVar.PlusCntSum == 0){
											FrameUpDxCoinSum(SysVar.icsn,SysVar.PlusCntSum,++SysVar.UpDatCnt);//�õ�ic���ţ��ϴ��û���Ϣ
										}else{
											SysVar.BeepType = B_;
										}
									}
								break;
							}						
							PcdHalt();
						}else{
							PcdHalt();
							SysVar.DisType = CNT_TEMP_DIS;
							SysVar.BeepType = BB;//��ʾˢ��Ƶ��				
						}
					}
				}
				else if(SysVar.DisType == IDE_MOD){//����ͣ���������ʾ���治����ˢ������
					SysVar.icsn_old = SysVar.icsn_new;
					SysVar.icsn = ((uint32_t)RC522Ctr.SN[0]<<24)+((uint32_t)RC522Ctr.SN[1]<<16)+((uint32_t)RC522Ctr.SN[2]<<8)+((uint32_t)RC522Ctr.SN[3]<<0); 
					SysVar.icsn_new = SysVar.icsn;
					L01_Ctr.ReTxcnt = ReTxcntMAX;//��ǰָ������ط�����
					switch(Config.RunMod){
						case RunMod_NORMAL:
							FrameUpDxICImfor(SysVar.icsn,++SysVar.UpDatCnt);//�õ�ic���ţ��ϴ��û���Ϣ
						break;
						case RunMod_CHECK:/* ��ѯ�� */
						break;
						case RunMod_SAVER:	 /* ��Ʊ�� */
							if(SysVar.CntFlg == 2){
								if(SysVar.PlusCntSum == 0){
									SysVar.DisType = CNT_TEMP_DIS;
								}
								FrameUpDxCoinSum(SysVar.icsn,SysVar.PlusCntSum,++SysVar.UpDatCnt);//�õ�ic���ţ��ϴ��û���Ϣ
								SysVar.icsn = 0;
								SysVar.CntFlg = 0;
							}
							else 
							{
								if(SysVar.PlusCntSum == 0){
									FrameUpDxCoinSum(SysVar.icsn,SysVar.PlusCntSum,++SysVar.UpDatCnt);//�õ�ic���ţ��ϴ��û���Ϣ
								}else{
									SysVar.BeepType = B_;
								}
							}
						break;
					}						
					PcdHalt();
				}
				else
				{
						PcdHalt();
						SysVar.DisType = CNT_TEMP_DIS;
						SysVar.BeepType = BB;//��ʾˢ��Ƶ��				
				}
			}
		}
	}
}
//�洢��λ���´����ϱ�����
#define MAX_LEN  50       //�����������
#define COININ_CMD_LEN 22//�ϱ������

uint8_t FrameBuf[MAX_LEN][COININ_CMD_LEN];

void prtAllCmd(void)
{
	uint8_t i,j;
	printf("cmd buf is:\r\n");
	for(i=0;i<MAX_LEN;i++)
	{
		printf("[%d]",i);
		for(j=0;j<COININ_CMD_LEN;j++)
		{
			printf("0x%x,",FrameBuf[i][j]);
		}
		printf("\r\n");
	}
}
#define FindSnCmdDBG 0
//����ָ���Ƿ��Ѿ���������������ָ����ϱң�ֱ�ӻظ���λ���Ѿ��Ϸ�
uint8_t FindSnCmd(uint8_t *SnCmdbf)
{
	uint8_t i,j;
	uint8_t res=0;
#if FindSnCmdDBG >0
 	prtAllCmd();	
#endif
	for(i=0;i<MAX_LEN;i++)
	{
		for(j=0;j<COININ_CMD_LEN;j++)
		{
			if(SnCmdbf[j] != FrameBuf[i][j])
			{
				break;//�Ƚ�ָ���Ƿ���ͬ
			}
		}
		if(j == COININ_CMD_LEN)//ָ��ƥ����
		{
			res = 1;
		}
	}
	if(res == 0)
	{
#if FindSnCmdDBG >0
			printf("ָ���Ѿ�������");
#endif
	}
	else
	{
#if FindSnCmdDBG >0
			printf("ָ���Ѿ�����");
#endif
	}
	return res;
}

//����д��ָ��
void MoveCmdBf(uint8_t *SnCmdbf)
{
	uint8_t i,j;
	for(i=0;i<MAX_LEN-1;i++)
	{
		for(j=0;j<COININ_CMD_LEN;j++)
		{
			FrameBuf[i][j] = FrameBuf[i+1][j];
		}
	}	
	for(j=0;j<COININ_CMD_LEN;j++)
	{
		FrameBuf[MAX_LEN-1][j] = SnCmdbf[j];
	}
}

void NetDatExp(void)
{
		uint8_t datlen;
		uint8_t txdatlen;
		uint16_t tmpid;
		uint8_t tmbfrx[RX_PLOAD_WIDTH];
			/* ���ݽ��մ��� */
		datlen = queue_find_cmd(&gCmpDwDxBuf,tmbfrx,NET_CMD_MAXLEN);
		tmpid = (((uint16_t)tmbfrx[1])<<8)+(((uint16_t)tmbfrx[2])<<0);
#if NetDatExp_DBG > 0
		if(datlen>0)
		{
			printf("������������,tmpid=%d\r\n",tmpid);
		}
#endif	
		if((datlen>0) )//���ڱ���id�ţ����ź������ط���
		{
#if NetDatExp_DBG > 0
			printf("����----->��ͷ:");
			showdbg8hex(tmbfrx,datlen);
#endif	
			if((tmpid == Config.DxID) && (tmbfrx[4] == WX_CMD))
			{
				switch(tmbfrx[3])
				{
					case CMP_DWN_COIN_OUT:						//�������ظ��ϱ�����
	#if NetDatExp_DBG
						printf("�������ظ��ϱ�����\r\n");
	#endif
						L01_Ctr.TxDatBufFlg = 0;//���־�����Լ���ˢ��
						if(FindSnCmd(tmbfrx) == 0)				//�������޴�ָ��ϱ�һ��
//						if(1)				//�������޴�ָ��ϱ�һ��
						{
							MoveCmdBf(tmbfrx);
	//					if((memcmp(&RC522Ctr.SN[0],&tmbfrx[5],4) == 0) && (tmbfrx[16] & SHEARCH_IC_OK))//����������ic������ͬ���Ҳ�ѯok
	//					if((memcmp(&RC522Ctr.SN[0],&tmbfrx[5],4) == 0))//����������ic������ͬ���Ҳ�ѯok
	//					{
								switch(tmbfrx[16])
								{
									case SHEARCH_NO_PAY:
	#if NetDatExp_DBG
										printf("����\r\n");
	#endif
										SysVar.DisType = NOMON_MOD;
										Dly_Var.LCDIdeT = (Config.PromptTim*100);
										SysVar.BeepType = BB;						//��ʾ����
										SysVar.ye = (((uint32_t)tmbfrx[10]<<16) + ((uint32_t)tmbfrx[11]<<8) + ((uint32_t)tmbfrx[12]<<0));
										SysVar.xf = (((uint32_t)tmbfrx[13]<<16) + ((uint32_t)tmbfrx[14]<<8) + ((uint32_t)tmbfrx[15]<<0));
										break;
									case SHEARCH_IC_OK:
// 	#if NetDatExp_DBG
// 										 printf("�������޴�ָ��ϱ�\r\n");
// 	#endif								
											MoveCmdBf(tmbfrx);							//����д��һ��ָ��
											SysVar.PlusNum += (tmbfrx[9]*Config.CoinX);
											SysVar.BeepType = B;					//��ʾˢ���ɹ�
											SysVar.DisType = ICINFO_MOD;
											Dly_Var.LCDIdeT = (Config.PromptTim*100);
											SysVar.ye = (((uint32_t)tmbfrx[10]<<16) + ((uint32_t)tmbfrx[11]<<8) + ((uint32_t)tmbfrx[12]<<0));
											SysVar.xf = (((uint32_t)tmbfrx[13]<<16) + ((uint32_t)tmbfrx[14]<<8) + ((uint32_t)tmbfrx[15]<<0));
//											SysVar.bi = tmbfrx[9];
	// #if NetDatExp_DBG
	// 									prtAllCmd();
	// #endif								
										//AckOutCoinSta(&RC522Ctr.SN[0],tmbfrx[17]);
										break;
									case SHEARCH_INVALID_IC:
										SysVar.BeepType = B_;//��ʾ�Ƿ���
										SysVar.DisType = Invalid_IC_MOD;
										Dly_Var.LCDIdeT = (Config.PromptTim*100);
	#if NetDatExp_DBG
										printf("�Ƿ���\r\n");
	#endif
										break;
									case UNKNOW_ERR:
										SysVar.BeepType = BB;//ˢ��ʧ��
										if((SysVar.DisType != ICINFO_MOD) && (SysVar.DisType != NOMON_MOD))//����ͣ���������ʾ���治����ˢ������
										{
											SysVar.DisType = UNKNOW_ERR;
											Dly_Var.LCDIdeT = (Config.PromptTim*100);
										}
	#if NetDatExp_DBG
										printf("ˢ��ʧ��\r\n");
	#endif
										break;
									case LOSS_ERR:
										SysVar.BeepType = BB;//ˢ��ʧ��
										if((SysVar.DisType != ICINFO_MOD) && (SysVar.DisType != NOMON_MOD))//����ͣ���������ʾ���治����ˢ������
										{
											SysVar.DisType = ERR_LOSS_DIS;
											Dly_Var.LCDIdeT = (Config.PromptTim*100);
										}
	#if NetDatExp_DBG
										printf("��Ƭ��ʧ\r\n");
	#endif			
										break;
									case OVER_TIM_ERR:
										SysVar.BeepType = BB;//ˢ��ʧ��
										if((SysVar.DisType != ICINFO_MOD) && (SysVar.DisType != NOMON_MOD))//����ͣ���������ʾ���治����ˢ������
										{
											SysVar.DisType = ERR_OVER_DIS;
											Dly_Var.LCDIdeT = (Config.PromptTim*100);
										}
	#if NetDatExp_DBG
										printf("��Ƭ����\r\n");
	#endif			
										break;
									default:
										SysVar.BeepType = BB;//ˢ��ʧ��
										SysVar.DisType = UNKNOW_ERR;
										Dly_Var.LCDIdeT = (Config.PromptTim*100);
	#if NetDatExp_DBG
										printf("δ֪����\r\n");
	#endif
										break;
								}
						}else{
							
	#if NetDatExp_DBG
							printf("�������Ѿ��д�ָ�ֱ�ӻظ�������\r\n");
	#endif								
						}
						SysVar.icsn = 0;
//						memset(&RC522Ctr.SN[0],0,4);
						break;
					case CMP_CONFIG_DX_ID:						//���������ö�ͷID��  
	#if NetDatExp_DBG
						printf("���������ö�ͷID��\r\n");
	#endif
						Config.DxID = (((uint16_t)tmbfrx[5])<<8)+(((uint16_t)tmbfrx[6])<<0);;
						WriteParm();				//�������ò���
						ExplainParm();
						PrintCfgPar();
						FrameUpDxConfig();//�ϴ���������
						SysVar.BeepType = B;
						SysReset();
						break;
					case CMP_CONFIG_COIN_PAR:					//���������ö�ͷ�ϱҲ���
	#if NetDatExp_DBG
						printf("���������ö�ͷ�ϱҲ���\r\n");
	#endif
						Config.CoinSigHod = tmbfrx[5]*XPlus;
						Config.CoinOutLevel = tmbfrx[6];
						Config.CoinSigGas = (tmbfrx[7]*XPlus);
						WriteParm();				//�������ò���
						ExplainParm();
						PrintCfgPar();
						FrameUpDxConfig();//�ϴ���������
						SysVar.BeepType = B;
						break;
					case CMP_CONFIG_DIS_PAR:					//���������ö�ͷ�����ʾʱ��
	#if NetDatExp_DBG
						printf("���������ö�ͷ�����ʾʱ��\r\n");
	#endif
						Config.PromptTim = (tmbfrx[5]);
						WriteParm();				//�������ò���
						ExplainParm();
						PrintCfgPar();
						FrameUpDxConfig();//�ϴ���������
						SysVar.BeepType = B;
						
						break;
					case CMP_CONFIG_DIR_PAR:					//���������ö�ͷ��ʾ����
	#if NetDatExp_DBG
						printf("���������ö�ͷ��ʾ����\r\n");
	#endif
						Config.disdir = tmbfrx[5];
						WriteParm();				//�������ò���
						ExplainParm();
						PrintCfgPar();
						FrameUpDxConfig();//�ϴ���������
						SysVar.BeepType = B;
						break;
					case CMP_COIN_OT_TEST:					//�����������ϱ�
	#if NetDatExp_DBG
						printf("���������Զ�ͷ�ϱ�\r\n");
	#endif
						SysVar.PlusNum = tmbfrx[5];
						SysVar.BeepType = B;
						TESTOutCoinOK(tmbfrx[6]);//���֡���
					break;
					case CMP_CONFIG_TEST:					//����������
						AckTest();
						SysVar.BeepType = B;
						break;
					case CMP_CONFIG_PRICE:					//����������
						Config.Price = tmbfrx[5];
						WriteParm();
						ExplainParm();
						PrintCfgPar();
						FrameUpDxConfig();//����������Ϣ							
						SysVar.BeepType = B;
	#if NetDatExp_DBG
						printf("���������õ�λ\r\n");
	#endif
						break;
					case CMP_ACK_SAVED_CNT:					//�������ظ�ͳ��Ͷ������ָ��
						//Config.Price = tmbfrx[5];
						SysVar.PlusCntSum = 0;
						L01_Ctr.TxDatBufFlg = 0;//ͨ�Żָ��������������·�������
// 						WriteParm();
// 						ExplainParm();
// 						PrintCfgPar();
// 						FrameUpDxConfig();//����������Ϣ							
						SysVar.BeepType = B;
// 	#if NetDatExp_DBG
// 						printf("���������õ�λ\r\n");
// 	#endif
						break;
					case CMP_CONFIG_FQ_WX:
						if(tmbfrx[5]<125)
							Config.DxFQ = tmbfrx[5];
						else
							printf("Ƶ���ų�������\r\n");
							
						WriteParm();
						ExplainParm();
						PrintCfgPar();
						FrameUpDxConfig();//����������Ϣ							
						SysVar.BeepType = B;
					break;
					case CMP_RX_OK_TEST:					//�������ظ���ͷ����ͨ��ָ��
						if((SysVar.DisType != ICINFO_MOD) && (SysVar.DisType != NOMON_MOD))//����ͣ���������ʾ���治����ˢ������
						{
							SysVar.DisType = IDE_MOD;
						}
	//					Dly_Var.LCDIdeT = (Config.PromptTim*100);
						L01_Ctr.TxDatBufFlg = 0;//ͨ�Żָ��������������·�������
						//SysVar.BeepType = B;
						break;
					case CMP_CONFIG_PAR:					//
						
						Config.PromptTim = (tmbfrx[5]*XPlus);				//�����ʾʱ��
						Config.Price = tmbfrx[6];						//��λ
						Config.CoinOutLevel = tmbfrx[7];		//Ͷ�ҿ�״̬
						Config.CoinSigHod = (tmbfrx[8]);			//�ϱ�������
						Config.CoinSigGas = (tmbfrx[9]);			//�ϱ�������
						Config.DxFQ = tmbfrx[10];						//ͨ��Ƶ��
					
						Config.DecMon = (uint16_t)tmbfrx[11];						//ͨ��Ƶ��
						Config.DecMon <<= 8;						//ͨ��Ƶ��
						Config.DecMon |= (uint16_t)tmbfrx[12];						//ͨ��Ƶ��
// 					if(SysVar.PlusCntSum >0){/* ϵͳ�����в�Ʊδ�ϴ�*/
// 						FrameUpDxCoinSum(SysVar.icsn,SysVar.PlusCntSum,++SysVar.UpDatCnt);				
// 					}

						WriteParm();
						ExplainParm();
						PrintCfgPar();
						//FrameUpDxConfig();//����������Ϣ							
						//SysVar.BeepType = B;
				

						L01_Ctr.TxDatBufFlg = 0;//ͨ�Żָ��������������·�������
						//SysVar.BeepType = B;
						break;
					case CMP_ACK_SAVED:					//���������ö�ͷ�����ʾʱ��
	#if NetDatExp_DBG
						printf("���������ز�Ʊ����\r\n");
	#endif
							if(tmbfrx[8] == 0x01){/* ˢ���ɹ������²�Ʊ���� */
								SysVar.xf = (uint32_t)tmbfrx[9];
								SysVar.xf <<= 8;
								SysVar.xf |= (uint32_t)tmbfrx[10];
								SysVar.xf <<= 8;
								SysVar.xf |= (uint32_t)tmbfrx[11];
								SysVar.xf <<= 8;
								SysVar.xf |= (uint32_t)tmbfrx[12];
						
								SysVar.ye = (uint32_t)tmbfrx[13];
								SysVar.ye <<= 8;
								SysVar.ye |= (uint32_t)tmbfrx[14];
								SysVar.ye <<= 8;
								SysVar.ye |= (uint32_t)tmbfrx[15];
								SysVar.ye <<= 8;
								SysVar.ye |= (uint32_t)tmbfrx[16];
					
								SysVar.BeepType = B;						
//								SysVar.CntFlg = 0;
								if(SysVar.PlusCntSum >=SysVar.xf){
									SysVar.PlusCntSum -= SysVar.xf;
									if(SysVar.xf > 0){
										SysVar.RGB_DisType = RGB_ICINFO_MOD;
									}
									SysVar.DisType = CONI_SUM_DIS;
								}
								else
									printf("���������ڴ�����������\r\n");
							}else{/* ˢ��ʧ�ܣ���ʾ */
								SysVar.BeepType = BB;											
							}
							L01_Ctr.TxDatBufFlg = 0;//���־�����Լ���ˢ��
							
						break;
						
					default:
	#if NetDatExp_DBG
						printf("δ���������\r\n");
	#endif
						break;
				}	
			}
			else if(tmpid != Config.DxID)
			{
		#if NetDatExp_DBG
							printf("��ͷid��ƥ��\r\n");
		#endif
			
			}
			else if(tmbfrx[4] != WX_CMD)
			{
		#if NetDatExp_DBG
							printf("���ݷǻ�վ����\r\n");
		#endif
			
			}	
		}
		/*�����ϴ�����(�����Ѿ�������ɣ����ߴﵽ����ط���)*/
		if(L01_Ctr.TxDatBufFlg == 0){
				txdatlen = queue_find_cmd(&gDxUpCmpBuf,TxMsgBuf,NET_CMD_MAXLEN);
				//memset(&TxMsgBuf[txdatlen],0,(TX_PLOAD_WIDTH-txdatlen));
				if(txdatlen>0){	
	#if NetDatExp_DBG >0
					printf("��ͷ----->����");
					showdbg8hex(TxMsgBuf,txdatlen);
	#endif	
					L01_Ctr.TxDatBufFlg =1;
				}
		}
}




void Nrf24l01Task(void)
{
	uint8_t RxMsgBuf[NET_CMD_MAXLEN];															//nrf24l01������ʱ������	
	uint16_t tmpID=0;
	
	/*�������ݵ�������,������ǰ�ȼ���Ƿ������ݱ����յ���*/
	if(L01_Ctr.TxDatBufFlg > 0){
		NRF24L01_RX_Mode(Config.DxFQ);													//�л�ֵ����Ƶ�����ȴ���������
		if(NRF24L01_RxPacket(RxMsgBuf) == 0){										//���ݽ��ճɹ�							
#if Nrf24l01TaskDBG > 0
			printf("D%d <---- data:",Config.DxID); 
#endif  
			gNetCtr.netsta |= NetRxOk;			
			tmpID = (((uint16_t)RxMsgBuf[1])<<8) + (((uint16_t)RxMsgBuf[2])<<0);
			if(tmpID == Config.DxID){
				EnQueuex(&gCmpDwDxBuf,RxMsgBuf,NET_CMD_MAXLEN);				//������ӣ���ת����
#if Nrf24l01TaskDBG > 0
				printf("ID Mached!\r\n"); 
				showdbg8hex(RxMsgBuf,NET_CMD_MAXLEN);
#endif  				
			}else{
#if Nrf24l01TaskDBG > 0
				printf("ID is not Mach!\r\n"); 
#endif  
			}
		}
		else{
			gNetCtr.netsta &= ~NetRxOk;
		}
		
		
		/* ��������ʱ�䵽 */
		if(L01_Ctr.TxCntTim == 0){
			
			/* Ϊ����ָ�� or �ϴ���������ָ�� 3s����һ�Σ�����ָ��1s����һ�� */
			if((L01_Ctr.TxDatBufFlg == 2) || (L01_Ctr.TxDatBufFlg ==3))
				L01_Ctr.TxCntTim = TEST_TxCntMAX_CONST;
			else
 				L01_Ctr.TxCntTim = TxCntMAX_CONST;//
			
			if(L01_Ctr.ReTxcnt>0)L01_Ctr.ReTxcnt--;								//�����·�����
			/* �ﵽ����ط����� */
 			if(L01_Ctr.ReTxcnt == 0){
 				if(TxMsgBuf[3] == DX_UP_CONFIG_CMP)
 					L01_Ctr.TxDatBufFlg = 3;																//�ﵽ����ط������������������ָ��׶�
 			//	else
 			//		L01_Ctr.TxDatBufFlg = 2;
 			}
 			if(L01_Ctr.TxDatBufFlg == 2){
 				TxMsgBuf[3] = DX_UP_TEST_CMP;
 				TxMsgBuf[4] = 0x02;
 				TxMsgBuf[5] = 0xfe;
 				TxMsgBuf[6] = 0xfc;
 				TxMsgBuf[7] = 0xfe;
 				TxMsgBuf[8] = 0xfc;
 				memset(&TxMsgBuf[9],0,23);
 				if((SysVar.DisType != ICINFO_MOD) && (SysVar.DisType != NOMON_MOD))//����ͣ���������ʾ���治����ˢ������
 				{
 					SysVar.DisType = ERR_PROMPT;
 				}
 			}
			NRF24L01_TX_Mode(Config.DxFQ);					//�л�����ͷ����Ƶ��
			TxMsgBuf[4] = DX_CMD;										//��ʶ�����ɶ�ͷ����
#if Nrf24l01TaskDBG >0
			printf("L01_Ctr.ReTxcnt=%d\r\n",L01_Ctr.ReTxcnt);
#endif	
			if(NRF24L01_TxPacket(TxMsgBuf) == TX_OK){ //�·��ɹ�			
#if Nrf24l01TaskDBG >0
				printf("D%d ----> ok! data is:\r\n",Config.DxID);
				showdbg8hex(TxMsgBuf,RX_PLOAD_WIDTH);
#endif	
				gNetCtr.netsta |= NetTxOk;
			}
			else{
#if Nrf24l01TaskDBG >0
				printf("D%d ----> Failed!\r\n",Config.DxID);
#endif          
				gNetCtr.netsta &= ~NetTxOk;
			}
		}
	}
	
	else{		
				/* �л������ݽ��ս׶� */
			L01_Ctr.TxCntTim = ReGASMAX;
			NRF24L01_RX_Mode(Config.DxFQ);													//�л�ֵ����Ƶ�����ȴ���������
			if(NRF24L01_RxPacket(RxMsgBuf) == 0){										//���ݽ��ճɹ�							
#if Nrf24l01TaskDBG > 0
				printf("D%d <---- data:",Config.DxID); 
#endif  
				gNetCtr.netsta |= NetRxOk;			
				tmpID = (((uint16_t)RxMsgBuf[1])<<8)+(((uint16_t)RxMsgBuf[2])<<0);
				if(tmpID == Config.DxID){
					EnQueuex(&gCmpDwDxBuf,RxMsgBuf,NET_CMD_MAXLEN);				//������ӣ���ת����
#if Nrf24l01TaskDBG > 0
				printf("ID Mach!\r\n"); 
				showdbg8hex(RxMsgBuf,NET_CMD_MAXLEN);
#endif  
				}else{
#if Nrf24l01TaskDBG > 0
					printf("ID is not Mach!\r\n"); 
#endif  
				}
			}else{
				gNetCtr.netsta &= ~NetRxOk;			
			}
	}
}


void AjstGLED(uint16_t Per)
{
 	TIM_SetCompare1(TIM4,Per);
}
void AjstRLED(uint16_t Per)
{
 	TIM_SetCompare2(TIM4,Per);
}
void AjstBLED(uint16_t Per)
{
 	TIM_SetCompare3(TIM4,Per);
}




void RGBCtrTask(void)
{
		switch(SysVar.RGB_DisType)
		{
				case RGB_IDE_MOD:
					if(Dly_Var.gLEDScanGas == 0)
					{
						Dly_Var.gLEDScanGas = LED_SCAN_CONST;
						JianBianLightIDE_MOD();
					}
					break;
				case RGB_Invalid_IC_MOD:
					SerRxOkInvalidIC();
					break;
				case RGB_NOLNK_MOD:
					//LCDShowNoLnkInfo();
					break;
				case RGB_ICINFO_MOD:
					SerRxOkXfOk();
					break;
				case RGB_NOMON_MOD:
					SerRxOkNoMony();
					break;
				case RGB_ERR_PROMPT:
					ReaderErrPromt();
					break;
				case RGB_UNKNOW_ERR:
					//LCDShowReaderFailed();
					break;
				case RGB_ERR_LOSS_DIS:
					//LCDShowICLoss();
					break;
				case RGB_ERR_OVER_DIS:
					//LCDShowICOverTim();
					break;
				default:
					break;
		}		
		//RGB_ctr.RedLightValue = 900;
		//RGB_ctr.GreenLightValue = 900;
		//RGB_ctr.BlueLightValue = 0;
		AjstRLED(RGB_ctr.RedLightValue);
		AjstGLED(RGB_ctr.GreenLightValue);
		AjstBLED(RGB_ctr.BlueLightValue);
}



void showdbg32hex(u32 *bf,u8 len)
{
	u8 cnt;
	for(cnt=0;cnt<len;cnt++)
	{
		printf("%x,",bf[cnt]);
	}
	printf("\r\n");
}
void showdbg32dec(u32 *bf,u8 len)
{
	u8 cnt;
	for(cnt=0;cnt<len;cnt++)
	{
		printf("%d,",bf[cnt]);
	}
	printf("\r\n");
}

void showdbg8hex(u8 *bf,u8 len)
{
	u8 cnt;
	for(cnt=0;cnt<len;cnt++)
	{
		printf("%02x,",bf[cnt]);
	}
	printf("\r\n");
}
void showdbg16hex(u16 *bf,u8 len)
{
	u8 cnt;
	for(cnt=0;cnt<len;cnt++)
	{
		printf("%x,",bf[cnt]);
	}
	printf("\r\n");
}
void showdbg8dec(u8 *bf,u8 len)
{
	u8 cnt;
	for(cnt=0;cnt<len;cnt++)
	{
		printf("%d,",bf[cnt]);
	}
	printf("\r\n");
}

//ϵͳ�����λ
void SysReset(void)
{
	*((uint32_t*)0xe000ed0c)=0x05fa0004;
}




