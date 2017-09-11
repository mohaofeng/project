

/*
/适用于 <2.4G无线读头通信协议 v2.7>
---------------------2016-12-09--------------------------------------------
1.增加上分间隔指令
2.增加测试指令
3.增加测试上分指令
4.修改为内部时钟
---------------------2017-2-04--------------------------------------------
5.增加测试卡，用于投币测试
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



QUEUE 	gCmpDwDxBuf;						//上位机下传命令缓冲队列
QUEUE 	gDxUpCmpBuf;						//卡头上传命令缓冲队列
__ConfigVar Config;							//系统配置参数
__nrf24l01_ctr L01_Ctr;					//nrf24l01控制器
__SysDlyT Dly_Var;							//系统定时变量
__Rc522_ctr RC522Ctr;						//rc522控制器
__Sys_Var SysVar;								//系统全局变量
__RGB_ctr RGB_ctr;	//管理LCD重要参数
__net_ctr gNetCtr;
_lcd_dev lcddev;
__SysFlg gFlg;	
uint32_t ChipUniqueID[3];
uint32_t random_seed=1;
uint8_t TxMsgBuf[TX_PLOAD_WIDTH];																								//nrf24l01发送缓冲区
//led控制
uint8_t leddly=0;
uint8_t stpro=0;
uint16_t rst = 1;
uint16_t gst = 2;
uint16_t bst = 3;

/*
*显示连接中断
*/
void LCDShowNo2_4gInfo(void)
{
	LCD_12864_ClearRec(0);
	LCDShowHZList(8,30,43,16,1,7);
	LCDRefresh_Gram();
}

//消费ok，绿灯全亮
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
//余额不足，黄色灯亮
void SerRxOkNoMony(void)
{
	RGB_ctr.RedLightValue = rstMAX_CONST;
	RGB_ctr.GreenLightValue = rstMAX_CONST;
	RGB_ctr.BlueLightValue = rstMIN_CONST;	
}
//非法卡，红灯全亮
void SerRxOkInvalidIC(void)
{
	RGB_ctr.RedLightValue = rstMAX_CONST;
	RGB_ctr.GreenLightValue = rstMIN_CONST;
	RGB_ctr.BlueLightValue = rstMIN_CONST;	
}
//卡头故障，红灯闪烁
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
	//红色led灯控制
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
		
	//绿色led灯控制
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
	
	
	//蓝色led灯控制
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
	
	
	if(RGB_ctr.BlueLightDir == INC_LIGHT && RGB_ctr.GreenLightDir == INC_LIGHT && RGB_ctr.RedLightDir == INC_LIGHT)//三色同时渐亮时红色灯渐灭
		RGB_ctr.RedLightDir = DEC_LIGHT;
// 	if(RGB_ctr.BlueLightDir == DEC_LIGHT && RGB_ctr.GreenLightDir == DEC_LIGHT && RGB_ctr.RedLightDir == INC_LIGHT)//只有红色灯增亮时蓝色灯同时增亮
// 		RGB_ctr.BlueLightDir = INC_LIGHT;
	if(RGB_ctr.BlueLightDir == DEC_LIGHT && RGB_ctr.GreenLightDir == DEC_LIGHT && RGB_ctr.RedLightDir == DEC_LIGHT)//三色同时渐灭时蓝色灯亮起
		RGB_ctr.BlueLightDir = INC_LIGHT;
	
	
	
//	AjstRLED(RGB_ctr.RedLightValue);
//	AjstGLED(RGB_ctr.GreenLightValue);
//	AjstBLED(RGB_ctr.BlueLightValue);

}
/*
*显示刷卡器故障，请联系管理员
*/
void LCDShowReaderErr(void)
{
	LCDShowHZList(18,20,40,16,1,5);
	LCDShowHZList(8,40,45,16,1,6);
}
/*
*显示刷卡失败
*/
void LCDShowReaderFailed(void)
{
	LCDShowHZList(25,30,51,16,1,4);
}
/*
*显示卡片过期
*/
void LCDShowICOverTim(void)
{
	LCDShowHZList(25,30,61,16,1,4);
}
/*
*显示卡片挂失
*/
void LCDShowICLoss(void)
{
	LCDShowHZList(25,30,57,16,1,4);
}


void LCDShowCoinSum(void)
{
// 		/* 正在计数 */
// 		LCDShowHZList(95,16,67,16,DIS_NORMAL,2);
// 		LCD_ShowxNum(15,16,SysVar.PlusCntSum,7,12,DIS_NORMAL|NUM_FILL_0);//待上传的数量


// 		/* 已经存入数量 */
// 		LCDShowHZList(95,36,65,16,DIS_NORMAL,2);//已经传的数量
// 		LCD_ShowxNum(15,36,SysVar.xf,7,12,DIS_NORMAL|NUM_FILL_0);//已经传的数量
// 	 
// 		/* 彩票总数 */
// 		LCDShowHZList(95,50,67,16,DIS_NORMAL,2);
// 		LCD_ShowxNum(15,50,SysVar.ye,7,12,DIS_NORMAL|NUM_FILL_0);//待上传的数量
	
	
		/* 正在计数 */
		LCDShowHZList(15,20,67,16,DIS_NORMAL,2);
 		LCD_ShowStringAndBk(50,22,10,12,12,":",DIS_NORMAL);
		LCD_ShowxNum(50,16,SysVar.PlusCntSum,5,24,DIS_NORMAL);//待上传的数量

	 
		/* 彩票总数 */
		LCDShowHZList(15,45,6,16,DIS_NORMAL,2);
 		LCD_ShowStringAndBk(50,47,10,12,12,":",DIS_NORMAL);
		LCD_ShowxNum(50,45,SysVar.ye,8,16,DIS_NORMAL);//待上传的数量
}


/*
*显示待机界面
*/
void LCDShowIDEInfo(void)
{
	switch(Config.RunMod){
		
		case RunMod_NORMAL:/* 普通卡头 */
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
		case RunMod_CHECK:/* 查询机 */
			LCDShowHZList(30,25,82,16,DIS_NORMAL,3);//显示消费额度

		break;
		case RunMod_SAVER:   /* 彩票机 */
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
 *显示非法卡信息
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
	
	LCDShowHZList(35,15,21,16,1,4);//显示非法卡
	LCD_ShowStringAndBk(10,40,40,16,16,"Sn:",DIS_NORMAL);
	
	LCD_ShowxNum(33,40,SysVar.icsn,10,16,DIS_NORMAL|NUM_FILL_0);
}
/*
*显示用户余额信息
*/
void LCDShowICInfo(void)
{
	LCDShowHZList(23,15,70,16,DIS_NORMAL,4);//显示刷卡成功
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
*显示用户余额不足
*/
void LCDShowNoMonyInfo(void)
{
	LCDShowHZList(23,15,16,16,DIS_NORMAL,5);//显示余额不足！
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
*显示ic卡号已经录入，请点票
*/
void LCDShowCntTickit(void)
{
	LCDShowHZList(23,15,91,16,DIS_NORMAL,5);//卡号已录入
	LCDShowHZList(23,38,96,16,DIS_NORMAL,5);//请开始点票
}
/*
*刷卡太频繁，请稍后重试
*/
void LCDShowOpTooOFent(void)
{
	LCDShowHZList(23,15,91,16,DIS_NORMAL,5);//卡号已录入
	LCDShowHZList(23,38,96,16,DIS_NORMAL,5);//请开始点票
}


/*
 *显示ID
 *
 */

void ShowSysID(void)
{
		LCD_DrawLine(0,12,127,12,DIS_NORMAL);		
		LCD_ShowStringAndBk(15,0,lcddev.width,12,12,"ID:",DIS_NORMAL);	
		LCD_ShowxNum(35,0,Config.DxID,10,12,DIS_NORMAL|NUM_FILL_0);//系统id号显示	
}
/*
 *显示刷卡方框
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
 *显示发送接收箭头
 *
 */
void ShowSysTxRx(void)
{
		if(gNetCtr.netsta & NetRxOk)//接收箭头
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
		
		if(gNetCtr.netsta & NetTxOk)//发送箭头
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
*显示连接中断
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
				case CONI_SUM_DIS://显示积分数量
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
		/* 系统工具栏显示 */
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
//功    能：系统指示灯提示输出
//参数说明: Dly_Var.gLED_Gas 系统运行指示灯延时变量				
//					Dly_Var.RC522FlashDlyrc522 运行状态指示灯延时变量      
void LEDTask(void)
{
	if(Dly_Var.gLED_Gas == 0)
	{
		Dly_Var.gLED_Gas = LED_GAS_CONST;
		SYS_LED = !SYS_LED;
	}
}

/////////////////////////////////////////////////////////////////////
//功    能：写入系统配置参数
//参数说明: 				
//					     
void WriteParm(void)
{	
	uint16_t wtbf[FLASH_DAT_LEN];
	wtbf[0]= (uint16_t)Config.tagflg;							  
	wtbf[1] = (uint16_t)Config.Price;					//							
	wtbf[2] = (uint16_t)Config.DxID;					//发射源默认为 D10											
	wtbf[3] = (uint16_t)Config.CoinX;   			//默认每次滴卡上币1数						
	wtbf[4] = (uint16_t)Config.CoinOutLevel;	//默认上币信号常态低电平								
	wtbf[5] = (uint16_t)Config.CoinSigHod;		//默认上币信号维持时间4x10=40ms						
	wtbf[6] = (uint16_t)Config.DeviceType;		//余额查询机标志 ,默认为非余额查询机									
	wtbf[7] = (uint16_t)Config.DxSwVer;				//余额查询机标志 ,默认为非余额查询机									
	wtbf[8] = (uint16_t)Config.DxHwVer;				//余额查询机标志 ,默认为非余额查询机									
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
//功    能：系统配置参数解释
//参数说明: 				
//					     
void ExplainParm(void)
{
	uint16_t expbf[FLASH_DAT_LEN];
	STMFLASH_Read(STM32_FLASH_BASE,expbf,FLASH_DAT_LEN);//读取系统参数	
	Config.tagflg = (uint8_t)expbf[0];							  
	Config.Price = (uint8_t)expbf[1];;		//接收源默认为 W10									
	Config.DxID = expbf[2];;		//发射源默认为 D10											
	Config.CoinX = (uint8_t)expbf[3];;   //默认每次滴卡上币1数						
	Config.CoinOutLevel = (uint8_t)expbf[4];;	//默认上币信号常态低电平								
	Config.CoinSigHod = expbf[5];;		//默认上币信号维持时间4x10=40ms						
	Config.DeviceType = (uint8_t)expbf[6];;		//余额查询机标志 ,默认为非余额查询机									
	Config.DxSwVer = (uint8_t)expbf[7];;		//余额查询机标志 ,默认为非余额查询机									
	Config.DxHwVer = (uint8_t)expbf[8];;		//余额查询机标志 ,默认为非余额查询机			
	Config.disdir = (uint8_t)expbf[9];		//					
	Config.PromptTim = (uint16_t)expbf[10];		//					
	Config.CoinSigGas = expbf[11];		//					
	COIN_SIG1 = !Config.CoinOutLevel;				//初始化投币口电平状态
	Config.DxFQ = (uint8_t)expbf[12];		//					
	Config.WxFQ = (uint8_t)expbf[13];		//					
	Config.RunMod = (uint8_t)expbf[14];
	Config.DecMon = expbf[15];
	
// 	STMFLASH_Read(STM32_FLASH_BASE+FLASH_DAT_LEN,expbf,4);//读取系统参数	
// 	
// 	/* 用户ic卡号 */
// 	Config.ICSn = expbf[0];
// 	Config.ICSn <<= 16;
// 	Config.ICSn |= expbf[1];
// // 	/* 彩票数量 */
// 	Config.PlusCntSum = expbf[2];
// 	Config.PlusCntSum <<= 16;
// 	Config.PlusCntSum |= expbf[3];
// 	SysVar.PlusCntSum = Config.PlusCntSum;
}
/////////////////////////////////////////////////////////////////////
//功    能：打印输出系统配置信息
//参数说明: 				
//					     
void PrintCfgPar(void)
{
 	printf("读取系统参数---------------------------------:\r\n");	
	printf("系统标志：0x%x\r\n",Config.tagflg);
	printf("读头ID：%d\r\n",Config.DxID);
	printf("消费金额：%d分\r\n",Config.DecMon);
	
	printf("投币倍率：%d\r\n",Config.CoinX);
	printf("投币口常态电平：%d\r\n",Config.CoinOutLevel);
	printf("上币口脉冲间隔时间：%d ms\r\n",Config.CoinSigGas);
	printf("上币口脉冲持续时间：%dms\r\n",Config.CoinSigHod);
	printf("读头类型：%d\r\n",Config.DeviceType);
	printf("软件版本号：%d  硬件版本号：%d \r\n",Config.DxSwVer,Config.DxHwVer);
	printf("显示停留时间：%d s\r\n",Config.PromptTim);
	printf("读头频道：%d \r\n",Config.DxFQ);
	printf("用户ic卡号为：%d \r\n",Config.ICSn);
	printf("用户彩票数为：%d \r\n",Config.FPlusCntSum);
	printf("金额显示单位：");
	switch(Config.Price)
	{
		case 0:
			printf("无\r\n");
			break;
		case 1:
			printf("元\r\n");
			break;
		case 2:
			printf("币\r\n");
			break;
		default:
			printf("未定义\r\n");			
			break;
	}
	printf("单价：%d \r\n",Config.Price);
	
	printf("显示方向：");
	if(Config.disdir)
		printf("正向\r\n");
	else
		printf("反向\r\n");
}
/////////////////////////////////////////////////////////////////////
//功    能：设置默认系统参数
//参数说明: 				
//					     
void WriteDefaultSet(void)
{
	Config.tagflg = DEFAULT_TAG_FLG;//flash标志  
	Config.Price = DEFAULT_Price;//默认网关id  	120
	Config.DxID = DEFAULT_DX_ID;//默认读头id  	6
	Config.CoinX = 1;						//默认上币倍率 	1
	Config.CoinOutLevel = 1;		//默认投币口常态电平 高
	Config.CoinSigHod = 80;			//默认投币脉冲宽度 4*10ms=40ms
	Config.CoinSigGas = 300;			//默认投币脉冲宽度 4*10ms=40ms
	
	Config.DeviceType = 0;			//默认为消费类型   1,为余额查询机
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
//功    能：载入系统配置参数
//参数说明: 				
//					     
void LoadConfigPar(void)
{
	uint16_t i;
	STMFLASH_Read(STM32_FLASH_BASE,&i,1);//读取标志
	if(DEFAULT_TAG_FLG != i)//mcu未初始化，设置默认
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
//功    能：读取芯片id号
//参数说明: 				
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
	 RCC_DeInit(); 				/*将外设RCC寄存器重设为缺省值 */ 
   RCC_HSICmd(ENABLE); 
   while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY)== RESET);//等待HSI就绪 
   RCC_HCLKConfig(RCC_SYSCLK_Div1);   /*设置AHB时钟（HCLK） RCC_SYSCLK_Div1——AHB时钟 = 系统时*/  
   RCC_PCLK2Config(RCC_HCLK_Div1);   /* 设置高速AHB时钟（PCLK2)RCC_HCLK_Div1——APB2时钟 = HCLK*/     
   RCC_PCLK1Config(RCC_HCLK_Div2); /*设置低速AHB时钟（PCLK1）RCC_HCLK_Div2——APB1时钟 = HCLK / 2*/      
   FLASH_SetLatency(FLASH_Latency_2);   /*设置FLASH存储器延时时钟周期数FLASH_Latency_2  2延时周期*/   
   FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);  /*选择FLASH预取指缓存的模,预取指缓存使能*/ 
   RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);/*设置PLL时钟源及倍频系数，频率为8/2*16=64Mhz*/    
   RCC_PLLCmd(ENABLE); 	 /*使能PLL */ 
   while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) ; /*检查指定的RCC标志位(PLL准备好标志)设置与否*/    
   RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);  /*设置系统时钟（SYSCLK） */  
   while(RCC_GetSYSCLKSource() != 0x08);     /*0x08：PLL作为系统时钟 */	    
}

////////////////////////////////?±???úêy2úéú°ì·¨////////////////////////////////
//??è??±???úêy
//?éò?2úéú0~RANDOM_MAX-1μ????úêy
//seed:??×ó
//max:×?′ó?μ	  		  
//·μ???μ:0~(max-1)?Dμ?ò????μ 		
uint16_t app_get_rand(uint16_t max,uint16_t min,uint16_t rand)
{			    	    
	rand=rand*22695477+1;

	return ((uint16_t)(rand)%(max-min+1)+min); 
}  



void PVD_Init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP,ENABLE);//PWRê1?ü
	
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
	delay_init();	    	 																			//延时函数初始化	  
	NVIC_Configuration(); 	 																	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
	
// 	PVD_Init();
// 	PVD_NVIC();


	uart_init(115200);	 																			//串口初始化为115200
	CreateLanQueue(&gCmpDwDxBuf);															//创建上位机下发命令缓冲区队列
	CreateLanQueue(&gDxUpCmpBuf);															//创建读头上传命令缓冲区队列
	memset(&RC522Ctr.SN[0],0,4);
	SysVar.icsn = 0;
	LoadConfigPar();		
	InitRc522();													
 	TIM3_Int_Init(100,639);																	//100Khz计数频率，计数到1000为10ms
	LCD_Init();
	NRF24L01_Init();    																			//初始化NRF24L01 
 	while(NRF24L01_Check())																		//检查NRF24L01是否在位.	
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
	IWDG_Init(4,625);     //与分频数为64,重载值为625,溢出时间为1s	   
	FrameUpDxConfig();
	SetKeyWord();
	delay_ms(1000);
	EXTIX_Init();
	while(1)
	{
			if(Dly_Var.gRc522RdGas == 0){
				IWDG_Feed();
				if(L01_Ctr.TxDatBufFlg == 0){
						RC522Task();								//rfid读卡任务
				}
				Dly_Var.gRc522RdGas = Rc522Rd_GAS_CONST;
			}
		
 			LEDTask();											//系统运行状态指示任务
 			NetDatExp();
			Nrf24l01Task();									//2.4g通信任务
			LCDDisplayTask();
			BeepTask();											//蜂鸣器提示任务
			RGBCtrTask();
	}
}


#define  DEC_CNT_MAX 	5			//最大档位值
#define  IC_KEY_DATA_ADDR 	7
#define  IC_TYPE_DATA_ADDR 	6

/////////////////////////////////////////////////////////////////////
//功    能：ic卡读卡任务
//参数说明: 				
//		

// uint8_t* const ictag ={"huibaitech2016"};
// uint8_t* const hzbuf ={"汇百科"};
// uint8_t KEYA[16]=  {0x55,0xd1,0xc9,0x77,0x89,0xa2,0xff,0x07,0x80,0x69,0x55,0xd1,0xc9,0x77,0x89,0xa2};																								//nrf24l01发送缓冲区
// uint8_t KEYAbk[16]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};																								//nrf24l01发送缓冲区
// uint8_t RFIDDAT[16]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};																								//nrf24l01发送缓冲区
// #define KEYADDR  	0x03
// #define DATAADDR  0x02

void RC522Task(void){		
	
	if(PcdRequest(PICC_REQIDL,&RC522Ctr.CT[0]) == MI_OK){
		if((PcdAnticoll(&RC522Ctr.SN[0])) == MI_OK){
			if(PcdSelect(&RC522Ctr.SN[0]) == MI_OK){
				RC522Ctr.GetIcNum = 1;
				
				if(PcdAuthState(KEYA_CHK,IC_KEY_DATA_ADDR,&SysVar.ICKeyB[0],&RC522Ctr.SN[0])== MI_OK){
					if(PcdRead(IC_TYPE_DATA_ADDR,&RC522Ctr.RFID[0]) == MI_OK){	/* 读取数据区数据，判定是否为管理卡 */
						//printf("ic卡数据区内容:");
						//showdbg8hex(RC522Ctr.RFID,16);     
						if(RC522Ctr.RFID[0] ==0x55 && RC522Ctr.RFID[1] == 0xaa){/* 匹配到标识符，是管理卡,给卡头上币 1 个*/
								SysVar.PlusNum += 1;
								SysVar.BeepType = B;					//提示刷卡成功
								SysVar.DisType = ICINFO_MOD;
								Dly_Var.LCDIdeT = (Config.PromptTim*100);
								SysVar.ye = 0;
								SysVar.xf = 0;
								PcdHalt();
						}else if(SysVar.DisType == IDE_MOD){//界面停留在余额显示界面不可以刷卡操作
							SysVar.icsn_old = SysVar.icsn_new;
							SysVar.icsn = ((uint32_t)RC522Ctr.SN[0]<<24)+((uint32_t)RC522Ctr.SN[1]<<16)+((uint32_t)RC522Ctr.SN[2]<<8)+((uint32_t)RC522Ctr.SN[3]<<0); 
							SysVar.icsn_new = SysVar.icsn;
							L01_Ctr.ReTxcnt = ReTxcntMAX;//当前指令最大重发次数
							
							switch(Config.RunMod){
								case RunMod_NORMAL:
									FrameUpDxICImfor(SysVar.icsn,++SysVar.UpDatCnt);//得到ic卡号，上传用户信息
								break;
								case RunMod_CHECK:/* 查询机 */
								break;
								case RunMod_SAVER:	 /* 彩票机 */
									if(SysVar.CntFlg == 2){
										if(SysVar.PlusCntSum == 0){
											SysVar.DisType = CNT_TEMP_DIS;
										}
										FrameUpDxCoinSum(SysVar.icsn,SysVar.PlusCntSum,++SysVar.UpDatCnt);//得到ic卡号，上传用户信息
										SysVar.icsn = 0;
										SysVar.CntFlg = 0;
									}
									else 
									{
										if(SysVar.PlusCntSum == 0){
											FrameUpDxCoinSum(SysVar.icsn,SysVar.PlusCntSum,++SysVar.UpDatCnt);//得到ic卡号，上传用户信息
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
							SysVar.BeepType = BB;//提示刷卡频繁				
						}
					}
				}
				else if(SysVar.DisType == IDE_MOD){//界面停留在余额显示界面不可以刷卡操作
					SysVar.icsn_old = SysVar.icsn_new;
					SysVar.icsn = ((uint32_t)RC522Ctr.SN[0]<<24)+((uint32_t)RC522Ctr.SN[1]<<16)+((uint32_t)RC522Ctr.SN[2]<<8)+((uint32_t)RC522Ctr.SN[3]<<0); 
					SysVar.icsn_new = SysVar.icsn;
					L01_Ctr.ReTxcnt = ReTxcntMAX;//当前指令最大重发次数
					switch(Config.RunMod){
						case RunMod_NORMAL:
							FrameUpDxICImfor(SysVar.icsn,++SysVar.UpDatCnt);//得到ic卡号，上传用户信息
						break;
						case RunMod_CHECK:/* 查询机 */
						break;
						case RunMod_SAVER:	 /* 彩票机 */
							if(SysVar.CntFlg == 2){
								if(SysVar.PlusCntSum == 0){
									SysVar.DisType = CNT_TEMP_DIS;
								}
								FrameUpDxCoinSum(SysVar.icsn,SysVar.PlusCntSum,++SysVar.UpDatCnt);//得到ic卡号，上传用户信息
								SysVar.icsn = 0;
								SysVar.CntFlg = 0;
							}
							else 
							{
								if(SysVar.PlusCntSum == 0){
									FrameUpDxCoinSum(SysVar.icsn,SysVar.PlusCntSum,++SysVar.UpDatCnt);//得到ic卡号，上传用户信息
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
						SysVar.BeepType = BB;//提示刷卡频繁				
				}
			}
		}
	}
}
//存储上位机下传的上币命令
#define MAX_LEN  50       //最大命令条数
#define COININ_CMD_LEN 22//上币命令长度

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
//查找指令是否已经处理过，处理过的指令不再上币，直接回复上位机已经上分
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
				break;//比较指令是否相同
			}
		}
		if(j == COININ_CMD_LEN)//指令匹配上
		{
			res = 1;
		}
	}
	if(res == 0)
	{
#if FindSnCmdDBG >0
			printf("指令已经不存在");
#endif
	}
	else
	{
#if FindSnCmdDBG >0
			printf("指令已经存在");
#endif
	}
	return res;
}

//覆盖写入指令
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
			/* 数据接收处理 */
		datlen = queue_find_cmd(&gCmpDwDxBuf,tmbfrx,NET_CMD_MAXLEN);
		tmpid = (((uint16_t)tmbfrx[1])<<8)+(((uint16_t)tmbfrx[2])<<0);
#if NetDatExp_DBG > 0
		if(datlen>0)
		{
			printf("解析接收数据,tmpid=%d\r\n",tmpid);
		}
#endif	
		if((datlen>0) )//属于本机id号，且信号由网关发出
		{
#if NetDatExp_DBG > 0
			printf("网关----->读头:");
			showdbg8hex(tmbfrx,datlen);
#endif	
			if((tmpid == Config.DxID) && (tmbfrx[4] == WX_CMD))
			{
				switch(tmbfrx[3])
				{
					case CMP_DWN_COIN_OUT:						//服务器回复上币命令
	#if NetDatExp_DBG
						printf("服务器回复上币命令\r\n");
	#endif
						L01_Ctr.TxDatBufFlg = 0;//清标志，可以继续刷卡
						if(FindSnCmd(tmbfrx) == 0)				//缓冲中无此指令，上币一次
//						if(1)				//缓冲中无此指令，上币一次
						{
							MoveCmdBf(tmbfrx);
	//					if((memcmp(&RC522Ctr.SN[0],&tmbfrx[5],4) == 0) && (tmbfrx[16] & SHEARCH_IC_OK))//服务器返回ic卡号相同，且查询ok
	//					if((memcmp(&RC522Ctr.SN[0],&tmbfrx[5],4) == 0))//服务器返回ic卡号相同，且查询ok
	//					{
								switch(tmbfrx[16])
								{
									case SHEARCH_NO_PAY:
	#if NetDatExp_DBG
										printf("余额不足\r\n");
	#endif
										SysVar.DisType = NOMON_MOD;
										Dly_Var.LCDIdeT = (Config.PromptTim*100);
										SysVar.BeepType = BB;						//提示余额不足
										SysVar.ye = (((uint32_t)tmbfrx[10]<<16) + ((uint32_t)tmbfrx[11]<<8) + ((uint32_t)tmbfrx[12]<<0));
										SysVar.xf = (((uint32_t)tmbfrx[13]<<16) + ((uint32_t)tmbfrx[14]<<8) + ((uint32_t)tmbfrx[15]<<0));
										break;
									case SHEARCH_IC_OK:
// 	#if NetDatExp_DBG
// 										 printf("缓冲区无此指令，上币\r\n");
// 	#endif								
											MoveCmdBf(tmbfrx);							//覆盖写入一条指令
											SysVar.PlusNum += (tmbfrx[9]*Config.CoinX);
											SysVar.BeepType = B;					//提示刷卡成功
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
										SysVar.BeepType = B_;//提示非法卡
										SysVar.DisType = Invalid_IC_MOD;
										Dly_Var.LCDIdeT = (Config.PromptTim*100);
	#if NetDatExp_DBG
										printf("非法卡\r\n");
	#endif
										break;
									case UNKNOW_ERR:
										SysVar.BeepType = BB;//刷卡失败
										if((SysVar.DisType != ICINFO_MOD) && (SysVar.DisType != NOMON_MOD))//界面停留在余额显示界面不可以刷卡操作
										{
											SysVar.DisType = UNKNOW_ERR;
											Dly_Var.LCDIdeT = (Config.PromptTim*100);
										}
	#if NetDatExp_DBG
										printf("刷卡失败\r\n");
	#endif
										break;
									case LOSS_ERR:
										SysVar.BeepType = BB;//刷卡失败
										if((SysVar.DisType != ICINFO_MOD) && (SysVar.DisType != NOMON_MOD))//界面停留在余额显示界面不可以刷卡操作
										{
											SysVar.DisType = ERR_LOSS_DIS;
											Dly_Var.LCDIdeT = (Config.PromptTim*100);
										}
	#if NetDatExp_DBG
										printf("卡片挂失\r\n");
	#endif			
										break;
									case OVER_TIM_ERR:
										SysVar.BeepType = BB;//刷卡失败
										if((SysVar.DisType != ICINFO_MOD) && (SysVar.DisType != NOMON_MOD))//界面停留在余额显示界面不可以刷卡操作
										{
											SysVar.DisType = ERR_OVER_DIS;
											Dly_Var.LCDIdeT = (Config.PromptTim*100);
										}
	#if NetDatExp_DBG
										printf("卡片过期\r\n");
	#endif			
										break;
									default:
										SysVar.BeepType = BB;//刷卡失败
										SysVar.DisType = UNKNOW_ERR;
										Dly_Var.LCDIdeT = (Config.PromptTim*100);
	#if NetDatExp_DBG
										printf("未知错误\r\n");
	#endif
										break;
								}
						}else{
							
	#if NetDatExp_DBG
							printf("缓冲区已经有此指令，直接回复服务器\r\n");
	#endif								
						}
						SysVar.icsn = 0;
//						memset(&RC522Ctr.SN[0],0,4);
						break;
					case CMP_CONFIG_DX_ID:						//服务器配置读头ID号  
	#if NetDatExp_DBG
						printf("服务器配置读头ID号\r\n");
	#endif
						Config.DxID = (((uint16_t)tmbfrx[5])<<8)+(((uint16_t)tmbfrx[6])<<0);;
						WriteParm();				//保存配置参数
						ExplainParm();
						PrintCfgPar();
						FrameUpDxConfig();//上传本机参数
						SysVar.BeepType = B;
						SysReset();
						break;
					case CMP_CONFIG_COIN_PAR:					//服务器配置读头上币参数
	#if NetDatExp_DBG
						printf("服务器配置读头上币参数\r\n");
	#endif
						Config.CoinSigHod = tmbfrx[5]*XPlus;
						Config.CoinOutLevel = tmbfrx[6];
						Config.CoinSigGas = (tmbfrx[7]*XPlus);
						WriteParm();				//保存配置参数
						ExplainParm();
						PrintCfgPar();
						FrameUpDxConfig();//上传本机参数
						SysVar.BeepType = B;
						break;
					case CMP_CONFIG_DIS_PAR:					//服务器配置读头金额提示时间
	#if NetDatExp_DBG
						printf("服务器配置读头金额提示时间\r\n");
	#endif
						Config.PromptTim = (tmbfrx[5]);
						WriteParm();				//保存配置参数
						ExplainParm();
						PrintCfgPar();
						FrameUpDxConfig();//上传本机参数
						SysVar.BeepType = B;
						
						break;
					case CMP_CONFIG_DIR_PAR:					//服务器配置读头显示方向
	#if NetDatExp_DBG
						printf("服务器配置读头显示方向\r\n");
	#endif
						Config.disdir = tmbfrx[5];
						WriteParm();				//保存配置参数
						ExplainParm();
						PrintCfgPar();
						FrameUpDxConfig();//上传本机参数
						SysVar.BeepType = B;
						break;
					case CMP_COIN_OT_TEST:					//服务器测试上币
	#if NetDatExp_DBG
						printf("服务器测试读头上币\r\n");
	#endif
						SysVar.PlusNum = tmbfrx[5];
						SysVar.BeepType = B;
						TESTOutCoinOK(tmbfrx[6]);//填充帧序号
					break;
					case CMP_CONFIG_TEST:					//服务器点名
						AckTest();
						SysVar.BeepType = B;
						break;
					case CMP_CONFIG_PRICE:					//服务器点名
						Config.Price = tmbfrx[5];
						WriteParm();
						ExplainParm();
						PrintCfgPar();
						FrameUpDxConfig();//本机配置信息							
						SysVar.BeepType = B;
	#if NetDatExp_DBG
						printf("服务器配置单位\r\n");
	#endif
						break;
					case CMP_ACK_SAVED_CNT:					//服务器回复统计投币数量指令
						//Config.Price = tmbfrx[5];
						SysVar.PlusCntSum = 0;
						L01_Ctr.TxDatBufFlg = 0;//通信恢复正常，可以重新发送数据
// 						WriteParm();
// 						ExplainParm();
// 						PrintCfgPar();
// 						FrameUpDxConfig();//本机配置信息							
						SysVar.BeepType = B;
// 	#if NetDatExp_DBG
// 						printf("服务器配置单位\r\n");
// 	#endif
						break;
					case CMP_CONFIG_FQ_WX:
						if(tmbfrx[5]<125)
							Config.DxFQ = tmbfrx[5];
						else
							printf("频道号超出限制\r\n");
							
						WriteParm();
						ExplainParm();
						PrintCfgPar();
						FrameUpDxConfig();//本机配置信息							
						SysVar.BeepType = B;
					break;
					case CMP_RX_OK_TEST:					//服务器回复读头测试通信指令
						if((SysVar.DisType != ICINFO_MOD) && (SysVar.DisType != NOMON_MOD))//界面停留在余额显示界面不可以刷卡操作
						{
							SysVar.DisType = IDE_MOD;
						}
	//					Dly_Var.LCDIdeT = (Config.PromptTim*100);
						L01_Ctr.TxDatBufFlg = 0;//通信恢复正常，可以重新发送数据
						//SysVar.BeepType = B;
						break;
					case CMP_CONFIG_PAR:					//
						
						Config.PromptTim = (tmbfrx[5]*XPlus);				//金额提示时间
						Config.Price = tmbfrx[6];						//金额单位
						Config.CoinOutLevel = tmbfrx[7];		//投币口状态
						Config.CoinSigHod = (tmbfrx[8]);			//上币脉冲宽度
						Config.CoinSigGas = (tmbfrx[9]);			//上币脉冲间隔
						Config.DxFQ = tmbfrx[10];						//通信频道
					
						Config.DecMon = (uint16_t)tmbfrx[11];						//通信频道
						Config.DecMon <<= 8;						//通信频道
						Config.DecMon |= (uint16_t)tmbfrx[12];						//通信频道
// 					if(SysVar.PlusCntSum >0){/* 系统中尚有彩票未上传*/
// 						FrameUpDxCoinSum(SysVar.icsn,SysVar.PlusCntSum,++SysVar.UpDatCnt);				
// 					}

						WriteParm();
						ExplainParm();
						PrintCfgPar();
						//FrameUpDxConfig();//本机配置信息							
						//SysVar.BeepType = B;
				

						L01_Ctr.TxDatBufFlg = 0;//通信恢复正常，可以重新发送数据
						//SysVar.BeepType = B;
						break;
					case CMP_ACK_SAVED:					//服务器配置读头金额提示时间
	#if NetDatExp_DBG
						printf("服务器返回彩票数量\r\n");
	#endif
							if(tmbfrx[8] == 0x01){/* 刷卡成功，跟新彩票数量 */
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
									printf("返回数大于存入数，错误！\r\n");
							}else{/* 刷卡失败，提示 */
								SysVar.BeepType = BB;											
							}
							L01_Ctr.TxDatBufFlg = 0;//清标志，可以继续刷卡
							
						break;
						
					default:
	#if NetDatExp_DBG
						printf("未定义此数据\r\n");
	#endif
						break;
				}	
			}
			else if(tmpid != Config.DxID)
			{
		#if NetDatExp_DBG
							printf("卡头id不匹配\r\n");
		#endif
			
			}
			else if(tmbfrx[4] != WX_CMD)
			{
		#if NetDatExp_DBG
							printf("数据非基站发送\r\n");
		#endif
			
			}	
		}
		/*数据上传处理(数据已经发送完成，或者达到最大重发数)*/
		if(L01_Ctr.TxDatBufFlg == 0){
				txdatlen = queue_find_cmd(&gDxUpCmpBuf,TxMsgBuf,NET_CMD_MAXLEN);
				//memset(&TxMsgBuf[txdatlen],0,(TX_PLOAD_WIDTH-txdatlen));
				if(txdatlen>0){	
	#if NetDatExp_DBG >0
					printf("读头----->网关");
					showdbg8hex(TxMsgBuf,txdatlen);
	#endif	
					L01_Ctr.TxDatBufFlg =1;
				}
		}
}




void Nrf24l01Task(void)
{
	uint8_t RxMsgBuf[NET_CMD_MAXLEN];															//nrf24l01接收临时缓冲区	
	uint16_t tmpID=0;
	
	/*发送数据到服务器,（发送前先检查是否有数据被接收到）*/
	if(L01_Ctr.TxDatBufFlg > 0){
		NRF24L01_RX_Mode(Config.DxFQ);													//切换值本机频道，等待接收数据
		if(NRF24L01_RxPacket(RxMsgBuf) == 0){										//数据接收成功							
#if Nrf24l01TaskDBG > 0
			printf("D%d <---- data:",Config.DxID); 
#endif  
			gNetCtr.netsta |= NetRxOk;			
			tmpID = (((uint16_t)RxMsgBuf[1])<<8) + (((uint16_t)RxMsgBuf[2])<<0);
			if(tmpID == Config.DxID){
				EnQueuex(&gCmpDwDxBuf,RxMsgBuf,NET_CMD_MAXLEN);				//数据入队，待转处理
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
		
		
		/* 发送数据时间到 */
		if(L01_Ctr.TxCntTim == 0){
			
			/* 为测试指令 or 上传本机配置指令 3s发送一次，其他指令1s发送一次 */
			if((L01_Ctr.TxDatBufFlg == 2) || (L01_Ctr.TxDatBufFlg ==3))
				L01_Ctr.TxCntTim = TEST_TxCntMAX_CONST;
			else
 				L01_Ctr.TxCntTim = TxCntMAX_CONST;//
			
			if(L01_Ctr.ReTxcnt>0)L01_Ctr.ReTxcnt--;								//数据下发计数
			/* 达到最大重发次数 */
 			if(L01_Ctr.ReTxcnt == 0){
 				if(TxMsgBuf[3] == DX_UP_CONFIG_CMP)
 					L01_Ctr.TxDatBufFlg = 3;																//达到最大重发次数，进入网络测试指令阶段
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
 				if((SysVar.DisType != ICINFO_MOD) && (SysVar.DisType != NOMON_MOD))//界面停留在余额显示界面不可以刷卡操作
 				{
 					SysVar.DisType = ERR_PROMPT;
 				}
 			}
			NRF24L01_TX_Mode(Config.DxFQ);					//切换到读头本机频道
			TxMsgBuf[4] = DX_CMD;										//标识数据由读头发出
#if Nrf24l01TaskDBG >0
			printf("L01_Ctr.ReTxcnt=%d\r\n",L01_Ctr.ReTxcnt);
#endif	
			if(NRF24L01_TxPacket(TxMsgBuf) == TX_OK){ //下发成功			
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
				/* 切换到数据接收阶段 */
			L01_Ctr.TxCntTim = ReGASMAX;
			NRF24L01_RX_Mode(Config.DxFQ);													//切换值本机频道，等待接收数据
			if(NRF24L01_RxPacket(RxMsgBuf) == 0){										//数据接收成功							
#if Nrf24l01TaskDBG > 0
				printf("D%d <---- data:",Config.DxID); 
#endif  
				gNetCtr.netsta |= NetRxOk;			
				tmpID = (((uint16_t)RxMsgBuf[1])<<8)+(((uint16_t)RxMsgBuf[2])<<0);
				if(tmpID == Config.DxID){
					EnQueuex(&gCmpDwDxBuf,RxMsgBuf,NET_CMD_MAXLEN);				//数据入队，待转处理
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

//系统软件复位
void SysReset(void)
{
	*((uint32_t*)0xe000ed0c)=0x05fa0004;
}




