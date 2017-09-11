/**
******************************************************************************
* @file    			main.c
* @author  			WIZnet Software Team
* @version 			V1.0
* @date    			2015-02-14
* @brief   			用3.5.0版本库建的工程模板
* @attention  	实验平台：秉火 iSO-V3 STM32 开发板 + 秉火W5500网络适配板
*
*               默认使用秉火开发板的SPI2接口
*							
*               内网测试，请保证W5500的IP与测试PC机在同一网段内，且不冲突
*               如果是用网线跟PC机直连，请设置PC机本地连接地址IP为静态IP
*
* 实验平台:秉火 iSO-V3 STM32 开发板 
* 论坛    :http://www.firebbs.cn
* 淘宝    :https://fire-stm32.taobao.com
******************************************************************************
*/ 
#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "bsp_led.h"

#include "w5500.h"
#include "W5500_conf.h"
#include "socket.h"
#include "utility.h"
/*app函数头文件*/
#include "tcp_demo.h"
#include "SysConfig.h"
#include "utility.h"
#include "led.h"
#include "Serialcd.h"
#include "rc522.h"
//#include "WT588D.H"
#include <CmpDatprocess.h>

#include "dhcp.h"
#include <printer.h>
#include "FramePkg.h"

#include "WT588D.H"
#include "bsp_led.h"   
#include "./ESP8266/bsp_esp8266.h" 


//#include "./pvd/bsp_pvd.h" 






__ConfigVar Config;//系统配置参数
__SysFlg g_tFlg;	
__net_ctr gNetCtr;
__Sys_Var SysVar;
__SysDlyT DlyVar;


void NetDatExp(void);





void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);	 
	
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
 GPIO_Init(GPIOB, &GPIO_InitStructure);					
 GPIO_SetBits(GPIOB,GPIO_Pin_2);						
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				//GATE
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 
 GPIO_ResetBits(GPIOA,GPIO_Pin_15);	


// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;				//WIFI_KEY ,LAN_KEY
// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		
// GPIO_Init(GPIOB, &GPIO_InitStructure);					 
// GPIO_SetBits(GPIOB,GPIO_Pin_7 | GPIO_Pin_8);						
	
}
 
void LEDTask(void)
{
	if(DlyVar.gLED_Gas == 0)
	{
		DlyVar.gLED_Gas = LED_GAS_CONST;
		//IWDG_Feed();
		SYS_LED = !SYS_LED;
	}
}




void UpCmd2Cmp(void)
{
	if(SysVar.GetVipIdFlg == 1)
	{
		switch(Config.runmod)
		{
			case RunMod_KEYINPUT:
					RequestDecMon(SysVar.CmdID,g_tFlg.VipIDString,g_tFlg.gDecMon); 
//					g_tFlg.gDecMon = 0;
				break;
			case RunMod_NORMAL_FIX:
					RequestDecMon(SysVar.CmdID,g_tFlg.VipIDString,g_tFlg.gDecMon); 
//					g_tFlg.gDecMon = 0;
				break;
			case RunMod_NORMAL_DANGWEI:
					RequestDecMon(SysVar.CmdID,g_tFlg.VipIDString,g_tFlg.gDecMon); 
//					g_tFlg.gDecMon = 0;
				break;
			case RunMod_CHECK:
					RequestDecMon(SysVar.CmdID,g_tFlg.VipIDString,g_tFlg.gDecMon); 
//					g_tFlg.gDecMon = 0;
				break;
			case RunMod_SAVER_COIN:
					g_tFlg.gDecMon =SysVar.WaitUpPlusCntSum;
					RequestDecMon(SysVar.CmdID,g_tFlg.VipIDString,g_tFlg.gDecMon); 
//					g_tFlg.gDecMon = 0;									
				break;
			case RunMod_SALE_COIN:
				break;
			case RunMod_SALE_SAVER_COIN:
					g_tFlg.gDecMon =SysVar.WaitUpPlusCntSum;
					RequestDecMon(SysVar.CmdID,g_tFlg.VipIDString,g_tFlg.gDecMon); 
//					g_tFlg.gDecMon = 0;									
				break;
			case RunMod_GATE_CTRL:
					RequestDecMon(SysVar.CmdID,g_tFlg.VipIDString,g_tFlg.gDecMon); 
//					g_tFlg.gDecMon = 0;
				break;
			default:
				break;
		}
		SysVar.GetVipIdFlg = 0;
		DBG_PRINTF("上传扣费指令，会员id:%s\r\n",g_tFlg.VipIDString);
	}
}

int main(void) 
 { 	
	SysInit();
	LED_Init();
	lcd_parm.current_screen_id = 255;
	LCDShowSysParm();	
	while(1)                            /*循环执行的函数*/ 
	{
//		if(DlyVar.tst_gas == 0)
//		{
//			DlyVar.tst_gas = 100; 
//			SysVar.WaitUpPlusCntSum++;
//		}
		//ESP8266_StaTcpClient_UnvarnishTest(); 
		if(RunMod_SAVER_COIN == Config.runmod )
		{
			if(SysVar.WaitUpPlusCntSum >0 && DlyVar.UpdateCoinCntDly == 0)
			{
				DlyVar.UpdateCoinCntDly = 200;
				Lcd_Show(250, 180,1,GBK_64X64,"系统未存：%d\r\n",SysVar.WaitUpPlusCntSum);
			}
		}
		LEDTask();
		SoundCtr.Typeold = SoundCtr.Typenew;
		SoundCtr.Typenew = SoundCtr.Type;
		if((SoundCtr.Sta == 0) && (SoundCtr.Typenew != SoundCtr.Typeold))
		{
			SoundTask();
		}
		if(RC522Ctr.ICScanGas == 0)
		{
			RC522Task();
			RC522Ctr.ICScanGas = ICScanGas_CONST;
		}
		ScomTftCmdExp();
		if(gNetCtr.netsta & TCP_CONNECTED && (DlyVar.LcdDisTaskDly == 0)) //网络已经连接
		{
			NetDatExp();
			CodeDatExp();
			gNetCtr.NetConnectRetryCnt = 0;
			UpCmd2Cmp();
			if(lcd_parm.LcdTaskRunFlg == 0)
				LCDDisplayTask();
		}
		else //服务器重连
		{
			gNetCtr.flg = 0;
		}
		
		if(DlyVar.NetScanGas == 0)
		{
			do_tcp_client();                  /*TCP_Client 数据回环测试程序*/ 
			DlyVar.NetScanGas = 100;
		}
		/* 如果为网线连接时先让网线建立连接后在连接wifi */
		switch(Config.NetMod){
			case NET_RJ45:
				if(gNetCtr.netsta & TCP_CONNECTED)
					//ESP8266_StaTcpClient_UnvarnishTest();
				break;
			case NET_WIFI:
				//ESP8266_StaTcpClient_UnvarnishTest();
				break;
			default:
				break;
		}
	}
} 




