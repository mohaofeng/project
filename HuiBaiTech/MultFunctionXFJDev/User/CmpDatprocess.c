
#include <CmpDatprocess.h>
#include "tcp_demo.h"
#include "FramePkg.h"
#include <string.h>
#include "WT588D.H"
#include "SysConfig.h"
#include <printer.h>




uint8_t gNetDwCmdBuff[NET_CMD_MAXLEN];								//用于暂存上位机命令的缓冲区
uint8_t gNetUpCmpCmdBuf[NET_CMD_MAXLEN];							//用于暂存上位机命令的缓冲区
uint8_t gExtTFTLCDCmdBuff[TFT_CMD_MAXLEN];							//串口彩屏命令缓冲区
uint8_t gExtCodeBuff[NET_CMD_MAXLEN];								//二维码读头命令接收缓冲区




QUEUE 	gNetDwDxQueu;										//上位机下传命令缓冲队列
QUEUE 	gNetUpCmpQueu;										//设备上传命令缓冲队列
QUEUE 	gExtCodeQueu;										//外接二维码读头命令缓冲队列
QUEUE 	gExtTFTLCDQueu;										//外接串口彩屏命令缓冲队列


//static uint32_t test_value = 0;//测试值
//static uint8_t update_en = 1;//更新标记
//static uint32_t timer_tick_last_update = 0; //上一次更新的时间
//volatile uint32_t  timer_tick_count = 0; //定时器节拍
//static uint16_t current_screen_id = 0;//当前画面ID

__VipImfor VipImfor;
__TmpVar TmpVar;	

/*
 *二维码读头数据解析，与处理
 *
 *
 **/
void CodeDatExp(void)
{
	uint16_t CodeDatlen;
	
	CodeDatlen = queue_find_cmdForCode(&gExtCodeQueu,gExtCodeBuff,CODE_CMD_MAXLEN,CODE_CMD_HEAD,CODE_CMD_TAIL);//取出一帧命令
	if(CodeDatlen  >0)
	{
		 DBG_PRINTF("串口1-->服务器：%s\r\n",gExtCodeBuff);
		//FrameDecMonEXT((CodeDatlen-5),&gExtCodeBuff[1],g_tFlg.gDecMon,Config.deviceid);
		gExtCodeBuff[CodeDatlen-4] = 0;
		memcpy(g_tFlg.VipIDString,&gExtCodeBuff[1],(CodeDatlen-5)); 
		SysVar.GetVipIdFlg = 1;								
		sprintf((char*)g_tFlg.VipIDString,"%s",&gExtCodeBuff[1]); //把整数转换为字符串
		//DBG_PRINTF("codeString is：%s\r\n",(char*)g_tFlg.VipIDString);  
//		  printDat(g_tFlg.pString);
	}
	
}







/*
 *服务器回复余额查询指令
 *
 *
 **/
void ExpAmountCmd(uint8_t *pbuff)
{
}
/*
 *服务器回复扣费指令
 *
 *
 **/
void ExpDecMonCmd(uint8_t *pbuff)
{
}

/*
 *服务器回复登录指令
 *
 *
 **/
void ExpLoginCmd(uint8_t *pbuff)
{
	
	Config.deviceid = ((u32)pbuff[2]<<24)+((u32)pbuff[3]<<16)+((u32)pbuff[4]<<8)+((u32)pbuff[5]<<0);
	Config.PromptTim = ((u32)pbuff[6]<<8)+((u32)pbuff[7]<<0);

	Config.DecMon[0] = ((uint16_t)pbuff[8] << 8) + ((uint16_t)pbuff[9]<<0);
	Config.DecMon[1] = ((uint16_t)pbuff[10] << 8) + ((uint16_t)pbuff[11]<<0);
	Config.DecMon[2] = ((uint16_t)pbuff[12] << 8) + ((uint16_t)pbuff[13]<<0);
	Config.DecMon[3] = ((uint16_t)pbuff[14] << 8) + ((uint16_t)pbuff[15]<<0);
	Config.DecMon[4] = ((uint16_t)pbuff[16] << 8) + ((uint16_t)pbuff[17]<<0);
	g_tFlg.gDecMon = Config.DecMon[0];
	
	Config.ICKey[5] = pbuff[18];
	Config.ICKey[4] = pbuff[19];
	Config.ICKey[3] = pbuff[20];
	Config.ICKey[2] = pbuff[21];
	Config.ICKey[1] = pbuff[22];
	Config.ICKey[0] = pbuff[23];
	

	VipImfor.wDecNum = ((u32)pbuff[24]<<24)+((u32)pbuff[25]<<16)+((u32)pbuff[26]<<8)+((u32)pbuff[27]<<0);

	VipImfor.wIdexpos = ((uint16_t)pbuff[28] << 8) + ((uint16_t)pbuff[29]<<0);				//显示x坐标
	VipImfor.wIdeypos = ((uint16_t)pbuff[30] << 8) + ((uint16_t)pbuff[31]<<0);				//显示y坐标
	VipImfor.ucIdeFcolor = ((uint16_t)pbuff[32] << 8) + ((uint16_t)pbuff[33]<<0); 			//显示前景色
	VipImfor.ucIdeBcolor = ((uint16_t)pbuff[34] << 8) + ((uint16_t)pbuff[35]<<0); 			//显示背景色
	
	VipImfor.ucIdeFontsize = pbuff[36];														//显示字体大小
	
	SoundCtr.Type = SOUND_LOGINOK_MOD;
	DlyVar.LCDIdeT = Config.PromptTim *1000;
	lcd_parm.distype = DIS_LOGIN_MOD;

	
	sprintf(VipImfor.IdeShowStr,"%s",&pbuff[39]); //保留待机字符
	DBG_PRINTF("收到登录指令,x坐标为:%u,y坐标为:%u,前景色:%u,背景色:%u,字体大小:%u,消费额度:%u,待机字符为:%s\r\n",VipImfor.wIdexpos,VipImfor.wIdeypos,VipImfor.ucIdeFcolor,VipImfor.ucIdeBcolor,VipImfor.ucIdeFontsize,VipImfor.wDecNum,VipImfor.IdeShowStr);


	SysParmWrite();
	SysParmExplain();
	SysParmPrint();
	ClearQ(&gNetUpCmpQueu);
	ClearQ(&gNetDwDxQueu);
	//FrameAckConfig();
}
/*
 *服务器配置消费额度
 *
 *
 **/
void ExpSetCostCmd(uint8_t *pbuff)
{
	Config.DecMon[1] = (((uint32_t)pbuff[6])<<16)+(((uint32_t)pbuff[7])<<8)+(((uint32_t)pbuff[8])<<0);					
	SysParmWrite();
	SysParmExplain();
	SysParmPrint();
	FrameAckConfig();
}
/*
 *服务器配置设备ID
 *
 *
 **/
void ExpSetIDCmd(uint8_t *pbuff)
{
	uint32_t tmp;
	tmp = ((u32)pbuff[2]<<24)+((u32)pbuff[3]<<16)+((u32)pbuff[4]<<8)+((u32)pbuff[5]<<0);
	if((Config.deviceid == tmp))//检查目标id是否正确
	{
		Config.deviceid = ((u32)pbuff[6]<<24)+((u32)pbuff[7]<<16)+((u32)pbuff[8]<<8)+((u32)pbuff[9]<<0);
		SysParmWrite();
		SysParmExplain();
		SysParmPrint();
		SysReset();
	}
}
/*
 *服务器配置设备运行模式
 *
 *
 **/
void ExpSetRunModeCmd(uint8_t *pbuff)
{
	uint32_t tmp;
	tmp = ((u32)pbuff[2]<<24)+((u32)pbuff[3]<<16)+((u32)pbuff[4]<<8)+((u32)pbuff[5]<<0);
	if((Config.deviceid == tmp))//检查目标id是否正确
	{
		Config.runmod = pbuff[6];
		if(Config.runmod == RunMod_CHECK)
		{
			g_tFlg.gDecMon = 0;
		}
		SysParmWrite();
		SysParmExplain();
		SysParmPrint();
		FrameAckConfig();
	}
}
/*
 *服务器配置设备通信方式
 *
 *
 **/
void ExpSetNetModeCmd(uint8_t *pbuff)
{
	uint32_t tmp;
	tmp = ((u32)pbuff[2]<<24)+((u32)pbuff[3]<<16)+((u32)pbuff[4]<<8)+((u32)pbuff[5]<<0);
	if((Config.deviceid == tmp))//检查目标id是否正确
	{
		Config.NetMod = pbuff[6];
		SysParmWrite();
		SysParmExplain();
		SysParmPrint();
		SysReset();
	}
}
/*
 *服务器配置待机界面恢复时间
 *
 *
 **/
void ExpSetPromptTimCmd(uint8_t *pbuff)
{
	uint32_t tmp;
	tmp = ((u32)pbuff[2]<<24)+((u32)pbuff[3]<<16)+((u32)pbuff[4]<<8)+((u32)pbuff[5]<<0);
	if((Config.deviceid == tmp))//检查目标id是否正确
	{
		Config.PromptTim = ((u16)pbuff[6]<<8)+((u16)pbuff[7]<<0);
		SysParmWrite();
		SysParmExplain();
		SysParmPrint();
		FrameAckConfig();
	}
}

/*
 *服务器配置设备上币参数
 *
 *
 **/
void ExpSetPlusPramCmd(uint8_t *pbuff)
{
	uint32_t tmp;
	tmp = ((u32)pbuff[2]<<24)+((u32)pbuff[3]<<16)+((u32)pbuff[4]<<8)+((u32)pbuff[5]<<0);
	if((Config.deviceid == tmp))//检查目标id是否正确
	{
		Config.PlusWith = pbuff[6];
		SysParmWrite();
		SysParmExplain();
		SysParmPrint();
		FrameAckConfig();
	}
}
/*
 *服务器配置设备ip信息
 *
 *
 **/
void ExpSetIPPramCmd(uint8_t *pbuff)
{
	uint32_t tmp;
	tmp = ((u32)gNetDwCmdBuff[2]<<24)+((u32)pbuff[3]<<16)+((u32)pbuff[4]<<8)+((u32)pbuff[5]<<0);
	if((Config.deviceid == tmp))//检查目标id是否正确
	{
		Config.Localip[3] = pbuff[6];//服务器ip
		Config.Localip[2] = pbuff[7];//服务器ip
		Config.Localip[1] = pbuff[8];//服务器ip
		Config.Localip[0] = pbuff[9];//服务器ip
		Config.Rmoteip[1] = Config.Localip[1];//服务器ip网段跟随本机ip网段做更改
		
		Config.GateW[3] = pbuff[10];//服务器网关
		Config.GateW[2] = pbuff[11];//服务器网关
		Config.GateW[1] = pbuff[12];//服务器网关
		Config.GateW[0] = pbuff[13];//服务器网关
		
		Config.Mass[3] = pbuff[14];//子网掩码
		Config.Mass[2] = pbuff[15];//子网掩码
		Config.Mass[1] = pbuff[16];//子网掩码
		Config.Mass[0] = pbuff[17];//子网掩码										
		Config.dhcpsw = 0;
		SysParmWrite();
		SysParmExplain();
		SysParmPrint();
		SysReset();
	}
}
/*
 *服务器配置服务器ip信息
 *
 *
 **/
void ExpSetRemoteIPPramCmd(uint8_t *pbuff)
{
	uint32_t tmp;
	tmp = ((u32)gNetDwCmdBuff[2]<<24)+((u32)pbuff[3]<<16)+((u32)pbuff[4]<<8)+((u32)pbuff[5]<<0);
	if((Config.deviceid == tmp))//检查目标id是否正确
	{
		Config.Rmoteip[3] = pbuff[6];//服务器ip
		Config.Rmoteip[2] = pbuff[7];//服务器ip
		Config.Rmoteip[1] = pbuff[8];//服务器ip
		Config.Rmoteip[0] = pbuff[9];//服务器ip
		
		Config.RemotePort = (((uint16_t)pbuff[10])<<8)+(((uint16_t)pbuff[11])<<0);//服务器端口
		Config.Localip[1] = Config.Rmoteip[1];	  //本机ip网段跟随服务器ip网段做更改
		Config.GateW[1]   = Config.Rmoteip[1];		//服务器网关
						
		SysParmWrite();
		SysParmExplain();
		SysParmPrint();
		SysReset();
	}
}
/*
 *服务器配置wifi无线参数
 *
 *
 **/
void ExpSetWiFiPramCmd(uint8_t *pbuff)
{
	uint8_t i;
	uint32_t tmp;
	
	tmp = ((u32)gNetDwCmdBuff[2]<<24)+((u32)pbuff[3]<<16)+((u32)pbuff[4]<<8)+((u32)pbuff[5]<<0);
	if((Config.deviceid == tmp))//检查目标id是否正确
	{
		for(i=0;i<gNetDwCmdBuff[6];i++)//ap名称和密码
		{
			Config.APName[i]= (uint8_t)gNetDwCmdBuff[8+i];//名称
		}
		Config.APName[i] = 0;
		for(i=0;i<gNetDwCmdBuff[7];i++)//ap名称和密码
		{
			Config.APPassword[i] = (uint8_t)gNetDwCmdBuff[8+gNetDwCmdBuff[6]+i];//密码
		}
		Config.APPassword[i] = 0;
#if NetDatExp_DBG >0 
			printf("服务器配置热点名称和密码:%s,%s\r\n",Config.APName,Config.APPassword);
#endif
		SysParmWrite();
		SysParmExplain();
		SysParmPrint();
		SysReset();
	}
}
/*
 *服务器回复存币成功
 *
 *
 **/
void ExpACKSavedCoinCmd(uint8_t *pbuff)
{
}



/*
 *服务器配置dhcp开关
 *
 *
 **/
void ExpSetDHCPSwCmd(uint8_t *pbuff)
{
	Config.dhcpsw = pbuff[6];
	SysParmWrite();
	SysParmExplain();
	SysParmPrint();
}





void ExpSnUpdateGUI(uint8_t *pbuff){
	char *pname;
	uint32_t ucNum=0;
	uint32_t ucI=0;
	uint32_t tmp=0;
	
	tmp = ((u32)pbuff[2]<<24)+((u32)pbuff[3]<<16)+((u32)pbuff[4]<<8)+((u32)pbuff[5]<<0);
	if(Config.deviceid == tmp)//检查目标id是否正确 
	{
		VipImfor.wDecNum        = ((u32)pbuff[8]<<24)+((u32)pbuff[9]<<16)+((u32)pbuff[10]<<8)+((u32)pbuff[11]<<0);
		VipImfor.cmdid          = pbuff[6];
		switch(Config.runmod)
		{
			case RunMod_KEYINPUT:
				break;
			case RunMod_NORMAL_FIX:
				break;
			case RunMod_NORMAL_DANGWEI:
				break;
			case RunMod_CHECK:
				break;
			case RunMod_SALE_COIN:
				break;
			case RunMod_SALE_SAVER_COIN:
				if((SAVE_CAI_PIAO == VipImfor.cmdid) || ((SAVE_COIN_DAI == VipImfor.cmdid)))//存彩票或者代币减去已存的彩票或者代币
				{
					if(SysVar.WaitUpPlusCntSum >= VipImfor.wDecNum)//如果当前系统存币数量大于存入系统的数量，则减去已经存入上位机的数量
					{
						SysVar.WaitUpPlusCntSum -= VipImfor.wDecNum;
					}
				}
				break;
			case RunMod_SAVER_COIN:
				if((SAVE_CAI_PIAO == VipImfor.cmdid) || ((SAVE_COIN_DAI == VipImfor.cmdid)))//存彩票或者代币减去已存的彩票或者代币
				{
					if(SysVar.WaitUpPlusCntSum >= VipImfor.wDecNum)//如果当前系统存币数量大于存入系统的数量，则减去已经存入上位机的数量
					{
						SysVar.WaitUpPlusCntSum -= VipImfor.wDecNum;
					}
				}
				break;
			case RunMod_GATE_CTRL:
				break;
			default:
				break;
		}
		
		
		VipImfor.wCoinCnt 			= ((u16)pbuff[12]<<8)+((u16)pbuff[13]<<0);
		VipImfor.wGateCnt 			= ((u16)pbuff[14]<<8)+((u16)pbuff[15]<<0);
		VipImfor.wSountType 		= pbuff[16];
		VipImfor.ScreenWaitTim 	= ((u16)pbuff[17]<<8)+((u16)pbuff[18]<<0);
		VipImfor.wxpos 					= ((u16)pbuff[19]<<8)+((u16)pbuff[20]<<0);
		VipImfor.wypos 					= ((u16)pbuff[21]<<8)+((u16)pbuff[22]<<0);
		VipImfor.ucFcolor 			= ((u16)pbuff[23]<<8)+((u16)pbuff[24]<<0);
		VipImfor.ucBcolor 			= ((u16)pbuff[25]<<8)+((u16)pbuff[26]<<0);
		VipImfor.ucFontsize 		= pbuff[27];
				
		g_tFlg.CoinSigNum += VipImfor.wCoinCnt;
		g_tFlg.GateSigNum += VipImfor.wGateCnt;
		SoundCtr.Type = VipImfor.wSountType;
		if(VipImfor.ScreenWaitTim)
			DlyVar.LCDIdeT = VipImfor.ScreenWaitTim*1000;
		else
			DlyVar.LCDIdeT = Config.PromptTim;
		lcd_parm.distype = DIS_Cmp_MOD;								
		
		pname = (char*)&pbuff[28];
		ucNum = strlen(pname);
		if(ucNum >0 && ucNum<400)
		{
			while(pname[ucI] != 0)
			{
				VipImfor.name[ucI] = pname[ucI];
				ucI++;
			}
		}

		//sprintf(VipImfor.IdeShowStr,"%s",&pbuff[39]); //保留待机字符
		//DBG_PRINTF("上币数量:%u\r\n开门次数:%u\r\n声音编号:%u\r\n恢复时间:%u\r\nx坐标:%u\r\ny坐标:%u\r\n前景色:%u\r\n背景色:%u\r\n字体大小:%u\r\n待机字符为:%s\r\n",VipImfor.wCoinCnt,VipImfor.wGateCnt,VipImfor.wSountType,VipImfor.ScreenWaitTim,VipImfor.wxpos,VipImfor.wypos,VipImfor.ucFcolor,VipImfor.ucBcolor,VipImfor.ucFontsize,VipImfor.name);
		
	}
}

/*
 *服务器测试打印
 *
 *
 **/
void ExpTestPrintCmd(uint8_t *pbuff)
{
//	sprintf((char*)TmpVar.srttmp,"%s\r\n",&pbuff[6]); //把整数转换为字符串
	printDat("%s\r\n",&pbuff[6]);
	DBG_PRINTF("测试打印的内容为：%s\r\n",&pbuff[6]);
	memset(&pbuff[6],0,strlen((char*)&pbuff[6]));
	//printDat((uint8_t*)TmpVar.srttmp);
}

/*
 *以太网数据解析，与处理
 *
 *
 **/
void NetDatExp(void)
{
	//char stringbuf[100];
	uint16_t NetRxbuffLen;
	uint16_t NetTxbuffLen;
	/*--------------tcp接收数据-------------------*/
	NetRxbuffLen = queue_find_cmdForCmp(&gNetDwDxQueu,gNetDwCmdBuff,NET_CMD_MAXLEN,NET_CMD_HEAD,NET_CMD_TAIL);//取出一帧命令
	if(NetRxbuffLen >0)
	{
		printf("服务器下传指令：");
		showdbg8hex(gNetDwCmdBuff,NetRxbuffLen);
		gNetCtr.LinkGas = LinkGas_CONST;
		gNetCtr.LinkRetryCnt = 0;
		gNetCtr.NetConnectRetryCnt = 0;
		//Arr2String(stringbuf,gNetDwCmdBuff,NetRxbuffLen);
	 	//DBG_PRINTF("%s\r\n",gNetDwCmdBuff);
		switch(gNetDwCmdBuff[CMD_POS])
		{
			case SnRE_AMOUNT_CMD:			
				ExpAmountCmd(gNetDwCmdBuff);
				break;
			case SnRE_DEC_MON_CMD:
				ExpDecMonCmd(gNetDwCmdBuff);
				break;											
			case Sn_REQXnIMFo_CMD:
				//FrameAckConfig(gNetDwCmdBuff);
				break;
			case Sn_Login_CMD:
				ExpLoginCmd(gNetDwCmdBuff);
				break;
			case Sn_SETDECMON_CMD:
				ExpSetCostCmd(gNetDwCmdBuff);
				break;
			case Sn_LINK_CMD:
				if(gNetCtr.flg ==0)
				{
					gNetCtr.flg = 1;
					DlyVar.LCDIdeT = Config.PromptTim;
					FrameLogin();
				}
				break;
			case Sn_CONFIG_ID_CMD:
				ExpSetIDCmd(gNetDwCmdBuff);
				break;	
			case Sn_SET_RUMMOD_CMD:
				ExpSetRunModeCmd(gNetDwCmdBuff);
				break;	
			case Sn_SET_Netmod:
				ExpSetNetModeCmd(gNetDwCmdBuff);
				break;	
			case Sn_PromptTIM:
				ExpSetPromptTimCmd(gNetDwCmdBuff);
				break;	
			case Sn_CONFIG_PLUS_CMD:
				ExpSetPlusPramCmd(gNetDwCmdBuff);
			break;	
			case Sn_CONFIG_IP_CMD:
				ExpSetIPPramCmd(gNetDwCmdBuff);
				break;	
			case Sn_SET_RemotIP:
				ExpSetRemoteIPPramCmd(gNetDwCmdBuff);
				break;	
			case Sn_SET_AP_NAME_PASSWORD:
				ExpSetWiFiPramCmd(gNetDwCmdBuff);
				break;	
			case Sn_ACK_COIN:
				ExpACKSavedCoinCmd(gNetDwCmdBuff);
				break;		
			case Sn_TEST:
				break;
			
			case Sn_UPDATE_GUI:
				ExpSnUpdateGUI(gNetDwCmdBuff);
				memset(gNetDwCmdBuff,0,NetRxbuffLen);
				break;
			case Sn_OUT_TEST:
				g_tFlg.CoinSigNum += gNetDwCmdBuff[6];
				break;
			case Sn_Config_DHCP:
				ExpSetDHCPSwCmd(gNetDwCmdBuff);
				break;
			case Sn_TEST_PRINTER:
				ExpTestPrintCmd(gNetDwCmdBuff);
				memset(gNetDwCmdBuff,0,NetRxbuffLen);
				break;
			
			default:			
				break;
		}
	}
	/*--------------tcp发送数据-------------------*/
	NetTxbuffLen = queue_find_cmdForUp(&gNetUpCmpQueu,gNetUpCmpCmdBuf,NET_CMD_MAXLEN,NET_CMD_HEAD,NET_CMD_TAIL);
	if(NetTxbuffLen>0){
//	 DBG_PRINTF("%s\r\n",gNetUpCmpCmdBuf);
	printf("验票机上传指令：");
	showdbg8hex(gNetUpCmpCmdBuf,NetTxbuffLen);
	gNetCtr.netsta |= TCP_WAIT_TO_SEND;
	gNetCtr.RxFrameBufLen = NetTxbuffLen;
	}
	else if(gNetCtr.LinkGas == 0){
		gNetCtr.LinkGas = LinkGas_CONST;
		FrameLink();
		if(++gNetCtr.LinkRetryCnt>LINK_MAX_NUM){
			DBG_PRINTF("网络已经断开\r\n");
			gNetCtr.LinkRetryCnt = 0;
			gNetCtr.flg = 0;
			//gNetCtr.netsta &=(~TCP_CONNECTED);//标记链接failed,重连服务器
		}
	}
}







/*
 *7寸串口彩屏数据解析，与处理
 *
 *
 **/
void ScomTftCmdExp(void)
{
	uint16_t STFTCmdLen;
/*--------------tcp接收数据-------------------*/
	STFTCmdLen = queue_find_cmdForTFT(&gExtTFTLCDQueu,gExtTFTLCDCmdBuff,TFT_CMD_MAXLEN,TFT_CMD_HEAD,TFT_CMD_TAIL);//取出一帧命令
	if(STFTCmdLen>0)
	{
		//DBG_PRINTF("%s\r\n",gExtTFTLCDCmdBuff);
		ProcessMessage((PCTRL_MSG)gExtTFTLCDCmdBuff, STFTCmdLen);//指令处理
		
//		/****************************************************************************************************************
//		特别注意
//		MCU不要频繁向串口屏发送数据，否则串口屏的内部缓存区会满，从而导致数据丢失(缓冲区大小：标准型8K，基本型4.7K)
//		1) 一般情况下，控制MCU向串口屏发送数据的周期大于100ms，就可以避免数据丢失的问题；
//		2) 如果仍然有数据丢失的问题，请判断串口屏的BUSY引脚，为高时不能发送数据给串口屏。
//		******************************************************************************************************************/

//		//TODO: 添加用户代码
//		//数据有更新时，每100毫秒刷新一次
//		if(update_en&&timer_tick_count-timer_tick_last_update>=TIME_100MS)
//		{
//			update_en = 0;
//			timer_tick_last_update = timer_tick_count;			

//			UpdateUI();
//		}

//		//这里为了演示方便，每100毫秒自动更新一次
//		if(timer_tick_count-timer_tick_last_update>=TIME_100MS)
//		{
//			timer_tick_last_update = timer_tick_count;
//			//UpdateUI();
//		}
//		//FrameDecMonEXT((SLen-5),&ExtDev[1],gFlg.gDecMon,Config.deviceid);
	}
}


/*! 
 *  \brief  消息处理流程
 *  \param msg 待处理消息
 *  \param size 消息长度
 *  \消息的类型：
			0xB1 :
 */
void ProcessMessage( PCTRL_MSG msg, uint16_t size )
{
	uint8_t cmd_type = msg->cmd_type;//指令类型
	uint8_t ctrl_msg = msg->ctrl_msg;   //消息的类型
	uint8_t control_type = msg->control_type;//控件类型
	uint16_t screen_id = PTR2U16(&msg->screen_id);//画面ID
	uint16_t control_id = PTR2U16(&msg->control_id);//控件ID
	uint32_t value = PTR2U32(msg->param);//数值


	lcd_parm.current_screen_id = screen_id;
	lcd_parm.old_screen_id = lcd_parm.new_screen_id;
	lcd_parm.new_screen_id= lcd_parm.current_screen_id;
	
	switch(cmd_type)
	{		
	case NOTIFY_TOUCH_PRESS://触摸屏按下
	case NOTIFY_TOUCH_RELEASE://触摸屏松开
		NotifyTouchXY(gExtTFTLCDCmdBuff[1],PTR2U16(gExtTFTLCDCmdBuff+2),PTR2U16(gExtTFTLCDCmdBuff+4));
		break;	
	case NOTIFY_WRITE_FLASH_OK://写FLASH成功
		NotifyWriteFlash(1);
		break;
	case NOTIFY_WRITE_FLASH_FAILD://写FLASH失败
		NotifyWriteFlash(0);
		break;
	case NOTIFY_READ_FLASH_OK://读取FLASH成功
		NotifyReadFlash(1,gExtTFTLCDCmdBuff+2,size-6);//去除帧头帧尾
		break;
	case NOTIFY_READ_FLASH_FAILD://读取FLASH失败
		NotifyReadFlash(0,0,0);
		break;
	case NOTIFY_READ_RTC://读取RTC时间
		NotifyReadRTC(gExtTFTLCDCmdBuff[1],gExtTFTLCDCmdBuff[2],gExtTFTLCDCmdBuff[3],gExtTFTLCDCmdBuff[4],gExtTFTLCDCmdBuff[5],gExtTFTLCDCmdBuff[6],gExtTFTLCDCmdBuff[7]);
		break;
	case NOTIFY_CONTROL:
		{
			if(ctrl_msg==MSG_GET_CURRENT_SCREEN)//画面ID变化通知
			{
				NotifyScreen(screen_id);
			}
			else
			{
				switch(control_type)
				{
				case kCtrlButton: //按钮控件
					NotifyButton(screen_id,control_id,msg->param[1]);
					break;
				case kCtrlText://文本控件
					NotifyText(screen_id,control_id,msg->param);
					break;
				case kCtrlProgress: //进度条控件
					NotifyProgress(screen_id,control_id,value);
					break;
				case kCtrlSlider: //滑动条控件
					NotifySlider(screen_id,control_id,value);
					break;
				case kCtrlMeter: //仪表控件
					NotifyMeter(screen_id,control_id,value);
					break;
				case kCtrlMenu://菜单控件
					NotifyMenu(screen_id,control_id,msg->param[0],msg->param[1]);
					break;
				case kCtrlSelector://选择控件
					NotifySelector(screen_id,control_id,msg->param[0]);
					break;
				case kCtrlRTC://倒计时控件
					NotifyTimer(screen_id,control_id);
					break;
				default:
					break;
				}
			}			
		}
		break;
	default:
		break;
	}
}

/*! 
 *  \brief  画面切换通知
 *  \details  当前画面改变时(或调用GetScreen)，执行此函数
 *  \param screen_id 当前画面ID
 */
void NotifyScreen(uint16_t screen_id)
{
	//TODO: 添加用户代码
	lcd_parm.current_screen_id = screen_id;//在工程配置中开启画面切换通知，记录当前画面ID

	if(lcd_parm.current_screen_id==4)//温度曲线
	{
		uint16_t i = 0;
		uint8_t dat[100] = {0};

		//生成方波
		for (i=0;i<100;++i)
		{
			if((i%20)>=10)
				dat[i] = 200;
			else
				dat[i] = 20;
		}
		GraphChannelDataAdd(4,1,0,dat,100);//添加数据到通道0

		//生成锯齿波
		for (i=0;i<100;++i)
		{
			dat[i] = 16*(i%15);
		}
		GraphChannelDataAdd(4,1,1,dat,100);//添加数据到通道1
	}
	else if(lcd_parm.current_screen_id==9)//二维码
	{
		//二维码控件显示中文字符时，需要转换为UTF8编码，
		//通过“指令助手”，转换“广州大彩123” ，得到字符串编码如下
		uint8_t dat[] = {0xE5,0xB9,0xBF,0xE5,0xB7,0x9E,0xE5,0xA4,0xA7,0xE5,0xBD,0xA9,0x31,0x32,0x33};
		SetTextValue(9,1,dat);
	}
}

/*! 
 *  \brief  触摸坐标事件响应
 *  \param press 1按下触摸屏，3松开触摸屏
 *  \param x x坐标
 *  \param y y坐标
 */
void NotifyTouchXY(uint8_t press,uint16_t x,uint16_t y)
{
	//TODO: 添加用户代码
}

void SetTextValueInt32(uint16_t screen_id, uint16_t control_id,uint32_t value)
{
	uint8_t buffer[12] = {0};
	sprintf((char*)buffer,"%ld",value); //把整数转换为字符串
	SetTextValue(screen_id,control_id,buffer);
}

void SetTextValueFloat(uint16_t screen_id, uint16_t control_id,float value)
{
	uint8_t buffer[12] = {0};
	sprintf((char*)buffer,"%.1f",value);//把浮点数转换为字符串(保留一位小数)
	SetTextValue(screen_id,control_id,buffer);
}

/*! 
 *  \brief  按钮控件通知
 *  \details  当按钮状态改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param state 按钮状态：0弹起，1按下
 */
void NotifyButton(uint16_t screen_id, uint16_t control_id, uint8_t  state)
{
	switch(screen_id){					
/*   显示系统参数画面   */
			case SHOW_SYSPARM_SCREEN_ID:
				
				switch(control_id){
					//设置开关
					case SHOW_SYSPARM_SCREEN_SET_BUTTON_ID:
						lcd_parm.LcdTaskRunFlg = 1;
						break;
					//退出开关
					case SHOW_SYSPARM_SCREEN_QUIT_BUTTON_ID:
						lcd_parm.LcdTaskRunFlg = 0;
						break;
					case SHOW_SYSPARM_SCREEN_COIN_STA_BUTTON_ID:
						/*投币口状态*/
						switch(Config.ttlLe)
						{
							case TTL_NC:
								Config.ttlLe = TTL_NO;
								//Lcd_Show(147,415,1,ASCII_12X24,"NO "); 
								break;
							case TTL_NO:
								Config.ttlLe = TTL_NC;
								//Lcd_Show(147,415,1,ASCII_12X24,"NC "); 
								break;
							default:
								Config.ttlLe = TTL_NC;
								//Lcd_Show(147,415,1,ASCII_12X24,"NC "); 
								break;
						}
						break;
					case SHOW_SYSPARM_SCREEN_GATE_STA_BUTTON_ID:
						/*开门信号状态*/
						switch(Config.GateSta)
						{
							case GATE_NC:
								Config.GateSta = GATE_NO;
								//Lcd_Show(147,375,1,ASCII_12X24,"NO "); 
								break;
							case GATE_NO:
								Config.GateSta = GATE_NC;
								//Lcd_Show(147,375,1,ASCII_12X24,"NC "); 
								break;
							default:
								Config.GateSta = GATE_NO;
								//Lcd_Show(147,375,1,ASCII_12X24,"NO "); 
								break;
						}
						break;
					case SHOW_SYSPARM_SCREEN_DHCP_SW_BUTTON_ID:
						/*DHCP开关*/
						switch(Config.dhcpsw)
						{
							case DHCP_OFF:
								Config.dhcpsw = DHCP_ON;
								//Lcd_Show(680,130,1,ASCII_12X24,"ON "); 
								break;
							case DHCP_ON:
								Config.dhcpsw = DHCP_OFF;
								//Lcd_Show(680,130,1,ASCII_12X24,"OFF"); 
								break;
							default:
								Config.dhcpsw = DHCP_ON;
								//Lcd_Show(680,130,1,ASCII_12X24,"ON "); 
								break;
						}
					break;
					case SHOW_SYSPARM_SCREEN_NET_MOD_BUTTON_ID:
						switch(Config.NetMod)
						{
							case NET_RJ45:
								Config.NetMod = NET_WIFI;
								//Lcd_Show(675,75,1,ASCII_12X24,"WiFi "); 
								break;
							case NET_WIFI:
								Config.NetMod = NET_RJ45;
								//Lcd_Show(675,75,1,ASCII_12X24,"网线 "); 
								break;
							default:
								Config.NetMod = NET_RJ45;
								//Lcd_Show(675,75,1,ASCII_12X24,"网线 "); 
								break;
						}
					break;
					case SHOW_SYSPARM_SCREEN_RUN_MOD_BUTTON_ID:
						/*运行模式*/
						switch(Config.runmod)
						{
							case RunMod_KEYINPUT:
								Config.runmod = RunMod_NORMAL_FIX;
								//Lcd_Show(626,180,1,ASCII_12X24,"固定消费模式 "); 
								break;
							case RunMod_NORMAL_FIX:
								Config.runmod = RunMod_NORMAL_DANGWEI;
								//Lcd_Show(626,180,1,ASCII_12X24,"档位消费模式 "); 
								break;
							case RunMod_NORMAL_DANGWEI:
								Config.runmod = RunMod_CHECK;
								//Lcd_Show(626,180,1,ASCII_12X24,"  查询机模式 "); 
								break;
							case RunMod_CHECK:
								Config.runmod = RunMod_SAVER_COIN;
								//Lcd_Show(626,180,1,ASCII_12X24,"   存币模式  "); 
								break;
							case RunMod_SAVER_COIN:
								Config.runmod = RunMod_SALE_COIN;
								//Lcd_Show(626,180,1,ASCII_12X24,"   售币模式  "); 
								break;
							case RunMod_SALE_COIN:
								Config.runmod = RunMod_SALE_SAVER_COIN;
								//Lcd_Show(626,180,1,ASCII_12X24,"存取一体模式 "); 
								break;
							case RunMod_SALE_SAVER_COIN:
								Config.runmod = RunMod_GATE_CTRL;
								//Lcd_Show(626,180,1,ASCII_12X24,"道闸控制模式 "); 
								break;
							case RunMod_GATE_CTRL:
								Config.runmod = RunMod_KEYINPUT;
								//Lcd_Show(626,180,1,ASCII_12X24,"按键消费模式 "); 
								break;
							default:
								Config.runmod = RunMod_NORMAL_FIX;
								//Lcd_Show(626,180,1,ASCII_12X24,"固定消费模式 "); 
								break;
						}
						break;
					default:
						break;
			}
			SysParmWrite();
			SysParmExplain();
			LCDShowSysParm();
			SysParmPrint();
	break;
/*   配置系统网络参数画面    */
			case SET_SYSPARM_FOR_NET_SCREEN_ID:
			break;
			
/*   固定金额消费机界面*/
		case FIXED_MON_SCREEN_ID:
			switch(control_id){
			}							
		break;
/*   可变档位消费机界面 */
			case STALL_MON_SCREEN_ID:
				switch(control_id)
				{ 
					case BT_STALL_MON_SCREEN_ID:
					break;
				}
				break;
/*   可变档位消费机界面 */
			case IDE_STALL_MON_SCREEN_ID:
				switch(control_id)
				{
					case BT_STALL_MON_SCREEN_ID:
						if(++g_tFlg.gDecMonCnt >= DEC_CNT_MAX)
							g_tFlg.gDecMonCnt = 0;
						g_tFlg.gDecMon = Config.DecMon[g_tFlg.gDecMonCnt];
						ClearLayer(0);
						SetScreen(IDE_STALL_MON_SCREEN_ID);
						Lcd_Show(250, 180,0,GBK_64X64,"%d.%d 元/次\r\n ",g_tFlg.gDecMon/10,g_tFlg.gDecMon%10);
					break;
					default:
					break;
				}
			break;
/*  按键输入消费机界面 */
		case KEY_INPUT_MON_SCREEN_ID:
				switch(control_id){
				}
		break;
/*  会员取币界面  */
		case VIP_QUBI_SCREEN_ID:
		break;
/*  会员存币界面  */
		case VIP_CUN_SCREEN_ID:
		break;
/* 会员购币界面下的操作 */	
		case VIP_GOUBI_SCREEN_ID:
			/* 若顾客已经查询过卡余额信息，则可以进行购币操作 */
			if((SysVar.ALLFlg && CHECK_MON_FLG)){
				switch(control_id){
				}				
			}
		break;
		default:
		break;
	}
	//TODO: 添加用户代码
	if(screen_id==3)//按钮、图标、动画控制
	{
		if(control_id==3)//运行按钮
		{
			if(state==0)//停止运行
			{				
				AnimationPlayFrame(3,1,1);//显示停止图标
				AnimationStop(3,2);//动画停止播放
			}
			else//开始运行
			{				
				SetControlVisiable(3,1,1);//显示图标
				SetControlVisiable(3,2,1);//显示动画

				AnimationPlayFrame(3,1,0);//显示运行图标
				AnimationStart(3,2);//动画开始播放
			}		
		}
		else if(control_id==4)//复位按钮
		{
			SetControlVisiable(3,1,0);//隐藏图标
			SetControlVisiable(3,2,0);//隐藏动画
			SetButtonValue(3,3,0);//显示开始运行
		}
	}
}


uint8_t CheckCharNum(char c,char *str)
{
	uint8_t inum = 0;
	uint8_t i=0;
	while(str[i])
	{
		if(str[i] == c)inum++;
		i++;
	}
	return inum;
}



/*
 *检查字符串是否符合规范，字符串长度必须<max >min 且包含 cnt个字符 c
 *
 **/
int CheckStr(uint8_t min,uint8_t max,char c,uint8_t cnt,uint8_t *tmpbuf,char *mstrpc)
{
	uint8_t i=0;
	
	/* 检查字符串长度 */
	if(strlen(mstrpc)<= max && strlen(mstrpc)>= min)
	{
		/* 检查包含制定字符串数量 */
		if(CheckCharNum(c,mstrpc) == cnt)
		{
			strcpy(TmpVar.srttmp,mstrpc);
			TmpVar.pc[0] = TmpVar.srttmp;
			for(i=0;i<cnt;i++) 
			{
				TmpVar.pc[i+1]=strchr(TmpVar.pc[i],c);
				*TmpVar.pc[i+1] = 0;
				TmpVar.pc[i+1] += 1;
				
			}
			for(i=0;i<cnt+1;i++) 
			{
				/* 检查字符串是否有效 */
				if(strlen(TmpVar.pc[i]) <1)
				{
					DBG_PRINTF("输入字符不符合规范，退出检查步骤\r\n");
					return -1;
				}
				tmpbuf[i] = atoi(TmpVar.pc[i]);
				DBG_PRINTF("%d",tmpbuf[i]);
			}
			return 0;
		}
		else
		{
			DBG_PRINTF("未包含制定数量的字符\r\n");
			return -1;
		}
	}
	else
	{
		DBG_PRINTF("字符长度不正确\r\n");
		return -1;
	}
}


uint32_t GetStrValue(char *tmpbuf)
{
	uint32_t itmp=0;
	strcpy(TmpVar.srttmp,tmpbuf);
	TmpVar.pc[0] = TmpVar.srttmp;
	if(strlen(TmpVar.srttmp)>0 && strlen(TmpVar.srttmp) <=8)
	{
		TmpVar.pc[1] = strchr(TmpVar.pc[0],'.');//整数部分
		if(TmpVar.pc[1] != NULL)//检查数据是否有小数部分
		{
			(*TmpVar.pc[1]) = 0;
			TmpVar.pc[2] = (TmpVar.pc[1]+1);
			if(strlen(TmpVar.pc[2]) > 0 && strlen(TmpVar.pc[2]) < 2 )//小数部分为一位数
			{
				itmp = atoi(TmpVar.pc[0])*10 + atoi(TmpVar.pc[2])*1;	
				Lcd_Show(400,450,0,ASCII_12X24,"               "); 
			}
			else
			{
				Lcd_Show(400,450,0,ASCII_12X24,"小数部分需为一位 "); 
				//Update_LcdText(IDE_KEY_INPUT_MON_SCREEN_ID,KEY_INPUT_MON_SCREEN_TEXT_ID,"错误");				
				itmp = 0;
			}
			//return(atoi(TmpVar.pc[0])*10 + atoi(TmpVar.pc[2])*1);//包含小数部分，返回小数加整数部分		
		}
		else{
			itmp = atoi(TmpVar.pc[0])*10;	
			//return(atoi(TmpVar.pc[0])*10);//数据只有整数部分，直接转换
		}
		return(itmp);
	}
	return(0);
}

/*! 
 *  \brief  文本控件通知
 *  \details  当文本通过键盘更新(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param str 文本控件内容
 */
void NotifyText(uint16_t screen_id, uint16_t control_id, uint8_t *str)
{
	char *pc;
	uint8_t ucbuf[100];
	uint16_t ucNum;
	uint32_t uiNum;
	//TODO: 添加用户代码
//	uint32_t value = 0; 
	pc = (char*)str;
	switch(screen_id){					
/*   显示系统参数画面   */
			case SHOW_SYSPARM_SCREEN_ID:
				switch(control_id){
//服务器ip文本框
					case SHOW_SYSPARM_SCREEN_REMOTE_IP_TEXT_ID:
						if(CheckStr(7,15,'.',3,ucbuf,pc) == 0 )
						{
							Config.Rmoteip[0] = ucbuf[0];
							Config.Rmoteip[1] = ucbuf[1];
							Config.Rmoteip[2] = ucbuf[2];
							Config.Rmoteip[3] = ucbuf[3];
						}
						break;
//子网掩码文本框
					case SHOW_SYSPARM_SCREEN_SUB_NET_TEXT_ID:
						if(CheckStr(7,15,'.',3,ucbuf,pc) == 0 )
						{
							Config.Mass[0] = ucbuf[0];
							Config.Mass[1] = ucbuf[1];
							Config.Mass[2] = ucbuf[2];
							Config.Mass[3] = ucbuf[3];
						}
						break;
//默认网关文本框
					case SHOW_SYSPARM_SCREEN_GATE_WAY_TEXT_ID:
						if(CheckStr(7,15,'.',3,ucbuf,pc) == 0 )
						{
							Config.GateW[0] = ucbuf[0];
							Config.GateW[1] = ucbuf[1];
							Config.GateW[2] = ucbuf[2];
							Config.GateW[3] = ucbuf[3];
						}
					break;
//本机ip文本框
					case SHOW_SYSPARM_SCREEN_LOCAL_IP_TEXT_ID:
						if(CheckStr(7,15,'.',3,ucbuf,pc) == 0 )
						{
							Config.Localip[0] = ucbuf[0];
							Config.Localip[1] = ucbuf[1];
							Config.Localip[2] = ucbuf[2];
							Config.Localip[3] = ucbuf[3];
						}
					break;
//WIFI名称文本框
					case SHOW_SYSPARM_SCREEN_WIFI_NAME_TEXT_ID:
						if(strlen(pc)>0 && strlen(pc) <=64)
						{
							strcpy((char*)Config.APName,pc);
						}
						break;
//WIFI密码文本框
					case SHOW_SYSPARM_SCREEN_WIFI_PWD_TEXT_ID:
						if(strlen(pc)>0 && strlen(pc) <=64)
						{
							strcpy((char*)Config.APPassword,pc);
						}
						break;
//服务器端口号文本框
					case SHOW_SYSPARM_SCREEN_REMOTE_PORT_TEXT_ID:
						if(strlen(pc)>0 && strlen(pc) <=5)
						{
							ucNum = atoi(pc);
							Config.RemotePort = (uint16_t)ucNum;
						}
						break;
//开门时间
					case SHOW_SYSPARM_SCREEN_GATE_TIM_TEXT_ID:
						if(strlen(pc)>0 && strlen(pc) <=10)
						{
							uiNum = atoi(pc);
							Config.GateWith = (uint32_t)uiNum;
						}
						break;
//上币时间
					case SHOW_SYSPARM_SCREEN_PULS_TIM_TEXT_ID:
						if(strlen(pc)>0 && strlen(pc) <=10)
						{
							uiNum = atoi(pc);
							Config.PlusWith = (uint32_t)uiNum;
						}
						break;
//开门间隔
					case SHOW_SYSPARM_SCREEN_GATE_GAS_TEXT_ID:
						if(strlen(pc)>0 && strlen(pc) <=10)
						{
							uiNum = atoi(pc);
							Config.GateGas = (uint32_t)uiNum;
						}
						break;
//上币间隔
					case SHOW_SYSPARM_SCREEN_PULS_GAS_TEXT_ID:
						if(strlen(pc)>0 && strlen(pc) <=10)
						{
							uiNum = atoi(pc);
							Config.PlusGas = (uint32_t)uiNum;
						}
						break;
//本机mac文本框
					case SHOW_SYSPARM_SCREEN_MAC_ADDR_TEXT_ID:
						if(CheckStr(11,23,'.',5,ucbuf,pc) == 0 )
						{
							Config.Mac[0] = ucbuf[0];
							Config.Mac[1] = ucbuf[1];
							Config.Mac[2] = ucbuf[2];
							Config.Mac[3] = ucbuf[3];
							Config.Mac[4] = ucbuf[4];
							Config.Mac[5] = ucbuf[5];
						}
						break;					
					default:
						break;
			}
			SysParmWrite();
			SysParmExplain();
			LCDShowSysParm();
			SysParmPrint();
			W5500Init();
	break;
/*   配置系统网络参数画面    */
			case SET_SYSPARM_FOR_NET_SCREEN_ID:
			break;
			
/*   固定金额消费机界面*/
		case FIXED_MON_SCREEN_ID:
			switch(control_id){
			}							
		break;
/*   可变档位消费机界面 */
			case STALL_MON_SCREEN_ID:
				switch(control_id){
					case 1:
						if(++g_tFlg.gDecMonCnt >= DEC_CNT_MAX)
							g_tFlg.gDecMonCnt = 0;
						g_tFlg.gDecMon = Config.DecMon[g_tFlg.gDecMonCnt];
						ClearLayer(0);
						Lcd_Show(250, 180,0,GBK_64X64,"%d.%d 元/次\r\n\r\n ",g_tFlg.gDecMon/10,g_tFlg.gDecMon%10);
					break;
					default:
					break;
			}
			break;
/*  按键输入消费机界面 */
		case IDE_KEY_INPUT_MON_SCREEN_ID:
				switch(control_id){
					case KEY_INPUT_MON_SCREEN_TEXT_ID:
						g_tFlg.gDecMon =GetStrValue(pc);
						Lcd_Show(350,50,0,GBK_24X24,"消费金额为：%d.%d\r\n   请刷卡",g_tFlg.gDecMon / 10,g_tFlg.gDecMon % 10); 		
						break;
						
				}
		break;
/*  会员取币界面  */
		case VIP_QUBI_SCREEN_ID:
		break;
/*  会员存币界面  */
		case VIP_CUN_SCREEN_ID:
		break;
/* 会员购币界面下的操作 */	
		case VIP_GOUBI_SCREEN_ID:
			/* 若顾客已经查询过卡余额信息，则可以进行购币操作 */
			if((SysVar.ALLFlg && CHECK_MON_FLG)){
				switch(control_id){
				}				
			}
		break;
		default:
		break;
	}
}

/*! 
 *  \brief  进度条控件通知
 *  \details  调用GetControlValue时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyProgress(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  滑动条控件通知
 *  \details  当滑动条改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifySlider(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
	//TODO: 添加用户代码
	if(screen_id==5&&control_id==2)//滑块控制
	{
		lcd_parm.test_value = value;

		SetProgressValue(5,1,lcd_parm.test_value); //更新进度条数值
	}
}

/*! 
 *  \brief  仪表控件通知
 *  \details  调用GetControlValue时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param value 值
 */
void NotifyMeter(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  菜单控件通知
 *  \details  当菜单项按下或松开时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param item 菜单项索引
 *  \param state 按钮状态：0松开，1按下
 */
void NotifyMenu(uint16_t screen_id, uint16_t control_id, uint8_t  item, uint8_t  state)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  选择控件通知
 *  \details  当选择控件变化时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param item 当前选项
 */
void NotifySelector(uint16_t screen_id, uint16_t control_id, uint8_t  item)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  定时器超时通知处理
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 */
void NotifyTimer(uint16_t screen_id, uint16_t control_id)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  读取用户FLASH状态返回
 *  \param status 0失败，1成功
 *  \param _data 返回数据
 *  \param length 数据长度
 */
void NotifyReadFlash(uint8_t status,uint8_t *_data,uint16_t length)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  写用户FLASH状态返回
 *  \param status 0失败，1成功
 */
void NotifyWriteFlash(uint8_t status)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  读取RTC时间，注意返回的是BCD码
 *  \param year 年（BCD）
 *  \param month 月（BCD）
 *  \param week 星期（BCD）
 *  \param day 日（BCD）
 *  \param hour 时（BCD）
 *  \param minute 分（BCD）
 *  \param second 秒（BCD）
 */
void NotifyReadRTC(uint8_t year,uint8_t month,uint8_t week,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second)
{
}

