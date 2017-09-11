#include "Serialcd.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	   
#include "hmi_driver.h"
//#include "printer.h"
#include "SysConfig.h"
#include "utility.h"
#include "rc522.h"
#include "WT588D.H"


__LCDParm lcd_parm;




#define H_GAS   20



void Lcd_Show(uint16_t x, uint16_t y,uint8_t back,uint8_t font,char *fmt, ...)
{
	va_list ap;
	char string[512];
	
	va_start(ap,fmt);
	vsprintf(string,fmt,ap);
	va_end(ap);

	DisText(x,y,back,font,(uint8_t*)string);
}

void Update_LcdText(uint16_t screen_id,uint16_t control_id,char *fmt, ...)
{
	va_list ap;
	char string[512];
	
	va_start(ap,fmt);
	vsprintf(string,fmt,ap);
	va_end(ap);

	SetTextValue(screen_id,control_id,(uint8_t*)string);
} 

   

void TFT_LCD_Init(void)
{
	USART1_Config();										/*初始化串口通信:115200@8-n-1*/
	lcd_parm.width 		= 800;
	lcd_parm.height 	= 400;
	lcd_parm.LcdTaskRunFlg = 0;
	DlyVar.LcdDisTaskDly = 5000;
	lcd_parm.Fcolorv = 0xffff;
	lcd_parm.Bcolorv = 0xffff;
	VipImfor.ucFcolor = 0xffff;
	VipImfor.ucBcolor = 0xffff;
	
	VipImfor.ucIdeFcolor = 0xffff;
	VipImfor.ucIdeBcolor = 0xffff;
	VipImfor.wIdexpos = 250;
	VipImfor.wIdeypos = 200;
	VipImfor.ucIdeFontsize = GBK_64X64;
//	SetFcolor(lcd_parm.Fcolorv);
//	SetBcolor(lcd_parm.Bcolorv);
	ClearLayer(0);
	

}

/*
void LCDShowIDEInfo(void)
{
	switch(Config.runmod){
		
		case RunMod_NORMAL:						
			SetScreen(0);
//			ClearLayer(0);
			Lcd_Show(250, 180,0,GBK_64X64,"%d.%d 元/次\r\n\r\n ",g_tFlg.gDecMon/10,g_tFlg.gDecMon%10);
		break;
		case RunMod_CHECK:				
		break;
		case RunMod_SAVER:   			
		break;
		case RunMod_SALE_COIN:  
		break;
		default:
		break;
	}
}
*/
#define XEND_pos   600


void LCDShowSysParm(void)
{
	uint16_t mod = 1;
	
		/*   远端主机ip地址   */
	//ClearLayer(0);
	SetScreen(SHOW_SYSPARM_SCREEN_ID);
	Lcd_Show(10, 2,mod,TOOL_BAR_FONT,"设备编号：%d ",Config.deviceid);
	Update_LcdText(SHOW_SYSPARM_SCREEN_ID,SHOW_SYSPARM_SCREEN_REMOTE_IP_TEXT_ID,"%03d.%03d.%03d.%03d",Config.Rmoteip[0],Config.Rmoteip[1],Config.Rmoteip[2],Config.Rmoteip[3]);
	Update_LcdText(SHOW_SYSPARM_SCREEN_ID,SHOW_SYSPARM_SCREEN_SUB_NET_TEXT_ID,"%03d.%03d.%03d.%03d",Config.Mass[0],Config.Mass[1],Config.Mass[2],Config.Mass[3]);
	Update_LcdText(SHOW_SYSPARM_SCREEN_ID,SHOW_SYSPARM_SCREEN_GATE_WAY_TEXT_ID,"%03d.%03d.%03d.%03d",Config.GateW[0],Config.GateW[1],Config.GateW[2],Config.GateW[3]);
	Update_LcdText(SHOW_SYSPARM_SCREEN_ID,SHOW_SYSPARM_SCREEN_LOCAL_IP_TEXT_ID,"%03d.%03d.%03d.%03d",Config.Localip[0],Config.Localip[1],Config.Localip[2],Config.Localip[3]);
	Update_LcdText(SHOW_SYSPARM_SCREEN_ID,SHOW_SYSPARM_SCREEN_MAC_ADDR_TEXT_ID,"%03d.%03d.%03d.%03d.%03d.%03d",Config.Mac[0],Config.Mac[1],Config.Mac[2],Config.Mac[3],Config.Mac[4],Config.Mac[5]); 
	Update_LcdText(SHOW_SYSPARM_SCREEN_ID,SHOW_SYSPARM_SCREEN_WIFI_NAME_TEXT_ID,"%s",Config.APName);
	Update_LcdText(SHOW_SYSPARM_SCREEN_ID,SHOW_SYSPARM_SCREEN_WIFI_PWD_TEXT_ID,"%s",Config.APPassword);
	Update_LcdText(SHOW_SYSPARM_SCREEN_ID,SHOW_SYSPARM_SCREEN_REMOTE_PORT_TEXT_ID,"%d",Config.RemotePort);
	Update_LcdText(SHOW_SYSPARM_SCREEN_ID,SHOW_SYSPARM_SCREEN_GATE_TIM_TEXT_ID,"%d",Config.GateWith); 
	Update_LcdText(SHOW_SYSPARM_SCREEN_ID,SHOW_SYSPARM_SCREEN_GATE_GAS_TEXT_ID,"%d",Config.GateGas); 
	Update_LcdText(SHOW_SYSPARM_SCREEN_ID,SHOW_SYSPARM_SCREEN_PULS_TIM_TEXT_ID,"%d",Config.PlusWith); 
	Update_LcdText(SHOW_SYSPARM_SCREEN_ID,SHOW_SYSPARM_SCREEN_PULS_GAS_TEXT_ID,"%d",Config.PlusGas); 
	switch(Config.NetMod)
	{
		case NET_RJ45:
			Lcd_Show(675,75,mod,ASCII_12X24,"网线 "); 
			break;
		case NET_WIFI:
			Lcd_Show(675,75,mod,ASCII_12X24,"WiFi "); 
			break;
		default:
			Lcd_Show(675,75,mod,ASCII_12X24,"未定义"); 
			break;
	}
	/*DHCP开关*/
	switch(Config.dhcpsw)
	{
		case DHCP_OFF:
			Lcd_Show(680,130,mod,ASCII_12X24,"OFF"); 
			break;
		case DHCP_ON:
			Lcd_Show(680,130,mod,ASCII_12X24,"ON "); 
			break;
		default:
			Lcd_Show(680,130,mod,ASCII_12X24,"ERR"); 
			break;
	}

	/*开门信号状态*/
	switch(Config.GateSta)
	{
		case GATE_NC:
			Lcd_Show(147,365,mod,ASCII_12X24,"NC "); 
			break;
		case GATE_NO:
			Lcd_Show(147,365,mod,ASCII_12X24,"NO "); 
			break;
		default:
			Lcd_Show(147,365,mod,ASCII_12X24,"ERR"); 
			break;
	}
	/*投币口状态*/
	switch(Config.ttlLe)
	{
		case TTL_NC:
			Lcd_Show(147,405,mod,ASCII_12X24,"NC "); 
			break;
		case TTL_NO:
			Lcd_Show(147,405,mod,ASCII_12X24,"NO "); 
			break;
		default:
			Lcd_Show(147,405,mod,ASCII_12X24,"ERR"); 
			break;
	}
	
	/*运行模式*/
	switch(Config.runmod)
	{
		case RunMod_KEYINPUT:
			Lcd_Show(626,180,mod,ASCII_12X24,"按键消费模式 "); 
			break;
		case RunMod_NORMAL_FIX:
			Lcd_Show(626,180,mod,ASCII_12X24,"固定消费模式 "); 
			break;
		case RunMod_NORMAL_DANGWEI:
			Lcd_Show(626,180,mod,ASCII_12X24,"档位消费模式 "); 
			break;
		case RunMod_CHECK:
			Lcd_Show(626,180,mod,ASCII_12X24,"  查询机模式 "); 
			break;
		case RunMod_SAVER_COIN:
			Lcd_Show(626,180,mod,ASCII_12X24,"   存币模式  "); 
			break;
		case RunMod_SALE_COIN:
			Lcd_Show(626,180,mod,ASCII_12X24,"   售币模式  "); 
			break;
		case RunMod_SALE_SAVER_COIN:
			Lcd_Show(626,180,mod,ASCII_12X24,"存取一体模式 "); 
			break;
		case RunMod_GATE_CTRL:
			Lcd_Show(626,180,mod,ASCII_12X24,"道闸控制模式 "); 
			break;
		default:
			Lcd_Show(626,180,mod,ASCII_12X24,"  未定义模式 "); 
			break;
	}
}



/*  显示id号 */
void IdDis(void)
{
	Lcd_Show(700, 468,0,ASCII_06X12,"%d ",Config.deviceid);
}
/*  显示id号 */
void VerDis(void)
{
	Lcd_Show(700, 452,0,ASCII_06X12,"%02d.%02d.%02d.%02d",Config.swver/10,Config.swver%10,Config.hwver/10,Config.hwver%10);
}

/*  状态栏 */
void ToolBar(void)
{
	SetFcolor(lcd_parm.Fcolorv);
	IdDis();
	VerDis();
}

/* 显示服务器下传指令 */
void LCDShowCmpCmd(char *pc)
{
	//ClearLayer(0);
	switch(Config.runmod)
	{
		case RunMod_KEYINPUT:
			SetScreen(KEY_INPUT_MON_SCREEN_ID);
			break;
		case RunMod_NORMAL_FIX:
			SetScreen(FIXED_MON_SCREEN_ID);
			break;
		case RunMod_NORMAL_DANGWEI:
			SetScreen(STALL_MON_SCREEN_ID);
			break;
		case RunMod_CHECK:
			SetScreen(CHECK_SCREEN_ID);
			break;
		case RunMod_SAVER_COIN:
			SetScreen(VIP_CUN_SCREEN_ID);
			break;
		case RunMod_SALE_COIN:
			SetScreen(VIP_QUBI_SCREEN_ID);
			break;
		case RunMod_SALE_SAVER_COIN:
			SetScreen(VIP_QUBI_SCREEN_ID);
			break;
		case RunMod_GATE_CTRL:
			SetScreen(GATE_CTRL_SCREEN_ID);
			break;
		default:
			SetScreen(RESERVER_SCREEN_ID);
			break;
	}
	SetFcolor(VipImfor.ucFcolor);
	if(VipImfor.ucBcolor)
	{
		SetBcolor(VipImfor.ucBcolor);
		Lcd_Show(VipImfor.wxpos,VipImfor.wypos,1,VipImfor.ucFontsize, "%s ",pc); //把整数转换为字符串
	}		
	else
		Lcd_Show(VipImfor.wxpos,VipImfor.wypos,0,VipImfor.ucFontsize, "%s ",pc); //把整数转换为字符串		'
	memset(pc,0,strlen(pc));
}


/* 待机界面 */
void LCDShowIDEInfo(char *pc)
{
	//ClearLayer(0);
	switch(Config.runmod)
	{
		case RunMod_KEYINPUT:
			SetScreen(IDE_KEY_INPUT_MON_SCREEN_ID);
			break;
		case RunMod_NORMAL_FIX:
			SetScreen(IDE_FIXED_MON_SCREEN_ID);
			ClearLayer(0);
			SetScreen(IDE_FIXED_MON_SCREEN_ID);
			
			break;
		case RunMod_NORMAL_DANGWEI:
			SetScreen(IDE_STALL_MON_SCREEN_ID);
			ClearLayer(0);
			SetScreen(IDE_STALL_MON_SCREEN_ID);
			Lcd_Show(250, 180,0,GBK_64X64,"%d.%d 元/次\r\n ",g_tFlg.gDecMon/10,g_tFlg.gDecMon%10);
			break;
		case RunMod_CHECK:
			SetScreen(IDE_CHECK_SCREEN_ID);
			ClearLayer(0);
			SetScreen(IDE_CHECK_SCREEN_ID);
//			Lcd_Show(250, 180,0,GBK_64X64,"系统未存：%d\r\n所属人：hello ",SysVar.PlusCntSum);
			break;
		case RunMod_SAVER_COIN:
			SetScreen(IDE_VIP_CUN_SCREEN_ID);
			ClearLayer(0);
			SetScreen(IDE_VIP_CUN_SCREEN_ID);
			Lcd_Show(250, 180,0,GBK_64X64,"系统未存：%d\r\n所属人：hello ",SysVar.WaitUpPlusCntSum);
			break;
		case RunMod_SALE_COIN:
			SetScreen(IDE_VIP_QUBI_SCREEN_ID);
			break;
		case RunMod_SALE_SAVER_COIN:
			SetScreen(IDE_SAVE_SALE_SCREEN_ID);
			break;
		case RunMod_GATE_CTRL:
			SetScreen(IDE_GATE_CTRL_SCREEN_ID);
			break;
		default:
			SetScreen(IDE_RESERVER_SCREEN_ID);
			break;
	}
	SetFcolor(VipImfor.ucIdeFcolor);
	
	if(VipImfor.ucIdeBcolor) 
	{
		SetBcolor(VipImfor.ucBcolor);
		Lcd_Show(VipImfor.wIdexpos,VipImfor.wIdeypos,1,VipImfor.ucIdeFontsize,"%s",VipImfor.IdeShowStr);
	}		
	else
	{
		Lcd_Show(VipImfor.wIdexpos,VipImfor.wIdeypos,0,VipImfor.ucIdeFontsize,"%s",VipImfor.IdeShowStr);
	}
}
void LCDShowLoginCmd(char *pc)
{
	SetScreen(LOGIN_SCREEN_ID);
	SetFcolor(VipImfor.ucFcolor);
	SetBcolor(VipImfor.ucBcolor);
	Lcd_Show(300,200,0,GBK_32X32, "%s ",pc); //把整数转换为字符串		
}

void LCDShowErro(char *pc)
{
	SetFcolor(lcd_parm.Fcolorv);
	SetBcolor(lcd_parm.Bcolorv);
	Lcd_Show(VipImfor.wxpos,VipImfor.wypos,0,VipImfor.ucFontsize, "erro cmd"); //把整数转换为字符串		
	
}



void LCDShowIcAction(void)
{
	if(RC522Ctr.GetIcNum == 1)
	{
		GUI_Circle(600,400,20);
		RC522Ctr.GetIcNum = 0;
	}
	else
	{
		//GUI_Circle(600,400,20);		
	}
}
void LCDDisplayTask(void)
{	
	lcd_parm.disstaold = lcd_parm.disstanew;					//
	lcd_parm.disstanew = lcd_parm.distype;						//
		
// 	if(lcd_parm.disstaold != lcd_parm.disstanew)//显示模式有变化
// 	{
//		DlyVar.LCDIdeT = Config.PromptTim;
// 	}
	if(DlyVar.LCDIdeT == 0)
	{
		lcd_parm.distype = DIS_IDE_MOD;
	}
	if(lcd_parm.disstaold != lcd_parm.disstanew)//显示模式有变化
	{
		//DlyVar.LCDIdeT = Config.PromptTim;
		
		switch(lcd_parm.distype)
		{					
			/* 待机模式 */
			case DIS_IDE_MOD:
					LCDShowIDEInfo(VipImfor.IdeShowStr);
				break;
			case DIS_Cmp_MOD:
					LCDShowCmpCmd(VipImfor.name);
				break;
			case DIS_LOGIN_MOD:
					LCDShowLoginCmd("登录成功");
				break;
					
			default:
				LCDShowErro("erro mode");
			break;
		}
		ToolBar();
	}
//	if((DIS_IDE_MOD == lcd_parm.distype) && (0 ==DlyVar.LCDIdeTUpdatGas))
//	{
//		LCDShowIDEInfo(VipImfor.IdeShowStr);	
//		LCDShowIcAction();
//		DlyVar.LCDIdeTUpdatGas = 1000;
//		ToolBar();
//	}
}








