
/*--------------------------log---------------------------------
1.协议栈定时器更改为tim3 
2.通过系统复位解决服务器断开连接后数据反复重发的问题
3.数据处理周期更改为200ms（之前为500ms）
4.增加读头刷卡成功后数据恢复命令，解决无刷卡问题
5.未配置读头ID号未0
6.解决网络连接超时后无法重连问题20160225
-------------------2017-05-06
1.喂看门狗，解决网关长期不断电程序意外跑飞
-------------------2017-05-27
1.解决晶振初始化失败导致网管死机问题
*/
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "enc28j60.h"
#include "uip.h"
#include "uip_arp.h"
#include "tapdev.h"
#include "timer.h"				   
#include "timerx.h"				   
#include "math.h" 	
#include "string.h"	 
#include "stmflash.h"
#include "24l01.h"
#include "wdg.h"
#include <stdlib.h>
#include "common.h"
#include "queue.h"
#include "FramePkg.h"

uint16_t Readtmp[FLASH_DAT_LEN];
uint8_t gSysRstFlg=0;
extern uint8_t tcp_client_sta;											//客户端状态
uint8_t NetRetry = 0;
uint16_t gTCPReConnectT = 0;
void SysReset(void);

QUEUE 	gCmpDwDxBuf;											//上位机下传命令缓冲队列
QUEUE 	gDxUpCmpBuf;										//卡头上传命令缓冲队列

uint8_t WxSwVer;//软件版本
uint8_t WxHwVer;//硬件版本
uint8_t TxMsgBuf[NET_CMD_MAXLEN];										  	//nrf24l01发送缓冲区
uint32_t ChipUniqueID[3];
uint32_t random_seed=1;
uint8_t Wnettxbf[NET_CMD_MAXLEN];//tcp 发送到服务器缓冲区
__SysFlg gFlg;	


__nrf24l01_ctr L01_Ctr;
//__Tst_ctr tst_ctr;
uint8_t gNrfLnkGas;  																		//NRF24L01 定时变量
__SysDlyT DlyVar;

__ConfigVar Config;//系统配置参数


#define BUF ((struct uip_eth_hdr *)&uip_buf[0])	 		  

void uip_polling(void);

u32 LCD_Pow(uint8_t m,uint8_t n);								//m^n函数
void num2str(u16 num,uint8_t *buf,uint8_t len);	//数字->字符串转换函数
void NetDatExp(void);



/***********************************************
Function: 写入系统配置参数
************************************************/
void WriteParm(void)
{	
	uint16_t wtbf[FLASH_DAT_LEN];
	wtbf[0]= (uint16_t)Config.tagflg;							  
	wtbf[1] = (uint16_t)Config.WxID;											
	wtbf[2] = (uint16_t)Config.DxNum;													
	
	wtbf[3] = (uint16_t)Config.DxLst[0];							
	wtbf[4] = (uint16_t)Config.DxLst[1];									
	wtbf[5] = (uint16_t)Config.DxLst[2];								
	wtbf[6] = (uint16_t)Config.DxLst[3];										
	wtbf[7] = (uint16_t)Config.DxLst[4];							
	wtbf[8] = (uint16_t)Config.DxLst[5];									
	wtbf[9] = (uint16_t)Config.DxLst[6];								
	wtbf[10] = (uint16_t)Config.DxLst[7];										
	wtbf[11] = (uint16_t)Config.DxLst[8];								
	wtbf[12] = (uint16_t)Config.DxLst[9];								
	
	wtbf[13] = (uint16_t)Config.Rmoteip[3];									
	wtbf[14] = (uint16_t)Config.Rmoteip[2];									
	wtbf[15] = (uint16_t)Config.Rmoteip[1];									
	wtbf[16] = (uint16_t)Config.Rmoteip[0];	
	
	wtbf[17] = (uint16_t)Config.Localip[3];									
	wtbf[18] = (uint16_t)Config.Localip[2];									
	wtbf[19] = (uint16_t)Config.Localip[1];									
	wtbf[20] = (uint16_t)Config.Localip[0];
	
	wtbf[21] = (uint16_t)Config.GateW[3];									
	wtbf[22] = (uint16_t)Config.GateW[2];									
	wtbf[23] = (uint16_t)Config.GateW[1];									
	wtbf[24] = (uint16_t)Config.GateW[0];			
	
	wtbf[25] = (uint16_t)Config.Mass[3];									
	wtbf[26] = (uint16_t)Config.Mass[2];									
	wtbf[27] = (uint16_t)Config.Mass[1];									
	wtbf[28] = (uint16_t)Config.Mass[0];		
	
	wtbf[29] = (uint16_t)Config.Port;		
	wtbf[30] = (uint16_t)Config.Mac[0];
	wtbf[31] = (uint16_t)Config.Mac[1];
	wtbf[32] = (uint16_t)Config.Mac[2];
	wtbf[33] = (uint16_t)Config.Mac[3];
	wtbf[34] = (uint16_t)Config.Mac[4];
	wtbf[35] = (uint16_t)Config.Mac[5];
	
	wtbf[36] = (uint16_t)Config.DxFQ;									
	wtbf[37] = (uint16_t)Config.WxFQ;									
		
	STMFLASH_Write(STM32_FLASH_BASE,wtbf,FLASH_DAT_LEN);	
}
/***********************************************
Function: 系统配置参数解释
************************************************/
void ExplainParm(void)
{
	uint16_t expbf[FLASH_DAT_LEN];
	STMFLASH_Read(STM32_FLASH_BASE,expbf,FLASH_DAT_LEN);//读取系统参数
	
	Config.tagflg = expbf[0];							  
  Config.WxID = (u8)expbf[1];													
	Config.DxNum = (u8)expbf[2];												
	
	Config.DxLst[0] = (u8)expbf[3];									
	Config.DxLst[1] = (u8)expbf[4];								
	Config.DxLst[2] = (u8)expbf[5];										
	Config.DxLst[3] = (u8)expbf[6];									
	Config.DxLst[4] = (u8)expbf[7];										
	Config.DxLst[5] = (u8)expbf[8];									
	Config.DxLst[6] = (u8)expbf[9];										
	Config.DxLst[7] = (u8)expbf[10];									
	Config.DxLst[8] = (u8)expbf[11];									
	Config.DxLst[9] = (u8)expbf[12];		

	Config.Rmoteip[3] = (u8)expbf[13];									
	Config.Rmoteip[2] = (u8)expbf[14];									
	Config.Rmoteip[1] = (u8)expbf[15];									
	Config.Rmoteip[0] = (u8)expbf[16];	
	
	Config.Localip[3] = (u8)expbf[17];									
	Config.Localip[2] = (u8)expbf[18];									
	Config.Localip[1] = (u8)expbf[19];									
	Config.Localip[0] = (u8)expbf[20];
	
	Config.GateW[3] = (u8)expbf[21];									
	Config.GateW[2] = (u8)expbf[22];									
	Config.GateW[1] = (u8)expbf[23];									
	Config.GateW[0] = (u8)expbf[24];			
	
	Config.Mass[3] = (u8)expbf[25];									
	Config.Mass[2] = (u8)expbf[26];									
	Config.Mass[1] = (u8)expbf[27];									
	Config.Mass[0] = (u8)expbf[28];		

	Config.Port = expbf[29];	
	
	Config.Mac[0] = (uint8_t)expbf[30];
	Config.Mac[1] = (uint8_t)expbf[31];
	Config.Mac[2] = (uint8_t)expbf[32];
	Config.Mac[3] = (uint8_t)expbf[33];
	Config.Mac[4] = (uint8_t)expbf[34];
	Config.Mac[5] = (uint8_t)expbf[35];

	Config.DxFQ = expbf[36];									
	Config.WxFQ = expbf[37];									
}

void PrintCfgPar(void)
{
 	printf("读取系统参数---------------------------------:\r\n");	
	printf("系统标志：%d\r\n",Config.tagflg);
	printf("网关ID：%d\r\n",Config.WxID);
	printf("子卡头个数：%d\r\n",Config.DxNum);
	printf("子卡头列表：");
	showdbg8hex(&Config.DxLst[0],Config.DxNum);
	
	printf("服务器ip地址：%d.%d.%d.%d\r\n",Config.Rmoteip[3],Config.Rmoteip[2],Config.Rmoteip[1],Config.Rmoteip[0]);
	printf("本机ip地址：%d.%d.%d.%d\r\n",Config.Localip[3],Config.Localip[2],Config.Localip[1],Config.Localip[0]);
	printf("默认网关：%d.%d.%d.%d\r\n",Config.GateW[3],Config.GateW[2],Config.GateW[1],Config.GateW[0]);
	printf("子网掩码：%d.%d.%d.%d\r\n",Config.Mass[3],Config.Mass[2],Config.Mass[1],Config.Mass[0]);
	printf("端口号：%d\r\n",Config.Port);
	printf("本机mac地址为：%d:%d:%d:%d:%d:%d\r\n",Config.Mac[0],Config.Mac[1],Config.Mac[2],Config.Mac[3],Config.Mac[4],Config.Mac[5]);
	printf("读头频道：%d\r\n",Config.DxFQ);
	printf("网关频道：%d\r\n",Config.WxFQ);
}
/***********************************************
Function: 设置默认系统参数
************************************************/
void WriteDefaultSet(void)
{
	uint8_t i;
	Config.tagflg = DEFAULT_TAG_FLG;
	Config.WxID = DEFAULT_WX_ID;
	Config.DxNum = 1;
	for(i=0;i<Config.DxNum;i++)
		Config.DxLst[i] = i+6;
	
	Config.Rmoteip[3] = 192;
	Config.Rmoteip[2] = 168;
	Config.Rmoteip[1] = 1;
	Config.Rmoteip[0] = 250;
	Config.Localip[3] = 192;
	Config.Localip[2] = 168;
	Config.Localip[1] = 1;
	Config.Localip[0] = 254;
	Config.GateW[3] = 192;
	Config.GateW[2] = 168;
	Config.GateW[1] = 1;
	Config.GateW[0] = 1;
	Config.Mass[3] = 255;
	Config.Mass[2] = 255;
	Config.Mass[1] = 255;
	Config.Mass[0] = 0;
	Config.Port = 8888;
	Config.Mac[0] = (uint8_t)(ChipUniqueID[0]>>0);
	Config.Mac[1] = (uint8_t)(ChipUniqueID[0]>>8);
	Config.Mac[2] = (uint8_t)(ChipUniqueID[0]>>16);
	Config.Mac[3] = (uint8_t)(ChipUniqueID[0]>>24);
	Config.Mac[4] = (uint8_t)(ChipUniqueID[1]>>0);
	Config.Mac[5] = (uint8_t)(ChipUniqueID[1]>>8);
	Config.DxFQ = DEFAULT_DxFQ_NUM;
	Config.WxFQ = DEFAULT_WxFQ_NUM;
	WriteParm();
}

void FlashChk(void)
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
void LanInit(void){
	uip_ipaddr_t ipaddr;
 	while(tapdev_init())
	{	//初始化ENC28J60错误
		printf("ENC28J60 Init Error!\r\n");
		delay_ms(1000);
		SYS_LED=!SYS_LED;    //nrf24l01初始化失败，指示灯慢闪
	}		
	uip_init();				//uIP初始化	  
 	uip_ipaddr(ipaddr, Config.Localip[3],Config.Localip[2],Config.Localip[1],Config.Localip[0]);	//根据转发器编号设置本地设置IP地址
	uip_sethostaddr(ipaddr);					    
	uip_ipaddr(ipaddr, Config.GateW[3],Config.GateW[2],Config.GateW[1],Config.GateW[0]); 	//设置网关IP地址(其实就是你路由器的IP地址)
	uip_setdraddr(ipaddr);						 
	uip_ipaddr(ipaddr, Config.Mass[3],Config.Mass[2],Config.Mass[1],Config.Mass[0]);	//设置网络掩码
	uip_setnetmask(ipaddr);
//	tcp_client_reconnect();
}
extern uint8_t mymac[6];




void getChipID(void)
{
		ChipUniqueID[2] = *(__IO u32*)(0X1FFFF7E8);  
		ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC); 
		ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0); 
		random_seed = (ChipUniqueID[2]>>1) | (ChipUniqueID[1]>>1) | (ChipUniqueID[0]>>1);
}
////////////////////////////////伪随机数产生办法////////////////////////////////
//获取伪随机数
//可以产生0~RANDOM_MAX-1的随机数
//seed:种子
//max:最大值	  		  
//返回值:0~(max-1)中的一个值 		
uint32_t app_get_rand(uint32_t max,uint32_t min)
{			    	    
	random_seed=random_seed*22695477+1;
//  rand(random_seed);
	return ((uint32_t)(random_seed)%(max-min+1)+min); 
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

int main(void)
{
	SysClkSet();
	NVIC_Configuration(); 	 
	delay_init();	    	 			
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	getChipID();
	uart_init(115200);	 		
	FlashChk();								
 	LED_Init();			  			
 	NRF24L01_Init();  			
	LanInit();							
	gTCPReConnectT = CONNECT_OVER_CONST;	//3s钟重连一次
	if(RST_KEY == 0)
	{
		delay_ms(10);
		if(RST_KEY == 0)
		{
			WriteDefaultSet();
		}
	}
	TIM3_Int_Init(1000,639);//100Khz计数频率，计数到1000为10ms
	CreateLanQueue(&gCmpDwDxBuf);//创建上位机下发命令缓冲区队列
	CreateLanQueue(&gDxUpCmpBuf);//创建读头上传命令缓冲区队列
	ClearQ(&gCmpDwDxBuf);
	ClearQ(&gDxUpCmpBuf);
	delay_ms((Config.WxID%50)+1);
	IWDG_Init(4,625);   
		
	while (1)
	{
			LEDTask();			
			if(tcp_client_sta & BIT_SET7 && (gFlg.netstaflg !=0))	//网络已经连接//--------------tcp发送数据-------------------*/
			{
				Nrf24l01Task();
			}
			uip_polling();
			NetDatExp();
			if(DlyVar.wdog_gas == 0)//100ms 喂看门狗一次
			{
				IWDG_Feed();
				DlyVar.wdog_gas = 10;
			}
	}  
} 

void NetDatExp(void)
{
		uint16_t tmpdt = 0;
		uint8_t Lcmdlen = 0;
		uint8_t Wdatlen=0;
		struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
		
	
		Lcmdlen = queue_find_cmd(&gCmpDwDxBuf,TxMsgBuf,NET_CMD_MAXLEN);//取出一帧命令
		if(Lcmdlen >0)
		{
#if NetDatExp_DBG 
			printf("服务器----->网关：");
			showdbg8hex(TxMsgBuf,Lcmdlen);
#endif	
			tmpdt = ((uint16_t)TxMsgBuf[1]<<8)+(((uint16_t)TxMsgBuf[2])<<0);
			switch(TxMsgBuf[3])
			{
				case CMP_CONFIG_WX_ID://本机网关id号
					DlyVar.LinkGasT = LinkGasTCONST;				
					gFlg.NetTickCnt = 0;
					NetRetry = 0;
					if(tmpdt == Config.WxID)
					{
						Config.WxID = ((uint16_t)TxMsgBuf[4]<<8)+(((uint16_t)TxMsgBuf[5])<<0);
						WriteParm();
						ExplainParm();
						PrintCfgPar();
						SysReset();
					}
			break;
			case CMP_CONFIG_WX_IP://服务器配置网关iP
					NetRetry = 0;
					DlyVar.LinkGasT = LinkGasTCONST;				
					gFlg.NetTickCnt = 0;
				if(tmpdt == Config.WxID)
				{
					Config.Localip[3] =  TxMsgBuf[4];
					Config.Localip[2] =  TxMsgBuf[5];
					Config.Localip[1] =  TxMsgBuf[6];
					Config.Localip[0] =  TxMsgBuf[7];
					
					
					Config.Rmoteip[3] =  TxMsgBuf[8];
					Config.Rmoteip[2] =  TxMsgBuf[9];
					Config.Rmoteip[1] =  TxMsgBuf[10];
					Config.Rmoteip[0] =  TxMsgBuf[11];
					
					Config.Port =  (((uint16_t)TxMsgBuf[12])<<8) + (((uint16_t)TxMsgBuf[13])<<0);

					Config.GateW[3] =  TxMsgBuf[14];
					Config.GateW[2] =  TxMsgBuf[15];
					Config.GateW[1] =  TxMsgBuf[16];
					Config.GateW[0] =  TxMsgBuf[17];
					
					Config.Mass[3] =  TxMsgBuf[18];
					Config.Mass[2] =  TxMsgBuf[19];
					Config.Mass[1] =  TxMsgBuf[20];
					Config.Mass[0] =  TxMsgBuf[21];						
					WriteParm();
					ExplainParm();
					PrintCfgPar();
					SysReset();
				}
			break;
				
			case CMP_CONFIG_FQ_WX:
					NetRetry = 0;
					DlyVar.LinkGasT = LinkGasTCONST;				
					gFlg.NetTickCnt = 0;
				if(tmpdt == Config.WxID)
				{
					if(TxMsgBuf[5]<125)
						Config.WxFQ = TxMsgBuf[5];
					else
						printf("频道号超出限制\r\n");
					WriteParm();
					ExplainParm();
					PrintCfgPar();
					FrameUpWxConfig();//本机配置信息
				}
					
			break;
			case CMP_RESET_WX:
					NetRetry = 0;
					DlyVar.LinkGasT = LinkGasTCONST;				
					gFlg.NetTickCnt = 0;
					SysReset();
			break;
			case CMP_TEST_WX://服务器测试网关
					NetRetry = 0;
					DlyVar.LinkGasT = LinkGasTCONST;				
					gFlg.NetTickCnt = 0;
					FrameUpCMPTEST();
				//SysReset();
			break;
			case CMP_CONFIG_MAC_WX:
					NetRetry = 0;
					DlyVar.LinkGasT = LinkGasTCONST;				
					gFlg.NetTickCnt = 0;
				if(tmpdt == Config.WxID)
				{
					Config.Mac[0] = TxMsgBuf[5];
					Config.Mac[1] = TxMsgBuf[6];
					Config.Mac[2] = TxMsgBuf[7];;
					Config.Mac[3] = TxMsgBuf[8];;
					Config.Mac[4] = TxMsgBuf[9];;
					Config.Mac[5] = TxMsgBuf[10];;
					WriteParm();
					ExplainParm();
					PrintCfgPar();
					SysReset();
				}
			break;
			
			case CMP_ACK_Link_WX:
					NetRetry = 0;
					DlyVar.LinkGasT = LinkGasTCONST;		
					if(gFlg.netstaflg == 0)
					{
						gFlg.netstaflg = 1;
						ClearQ(&gCmpDwDxBuf);
						ClearQ(&gDxUpCmpBuf);
						FrameUpWxConfig();//本机配置信息
					}
					gFlg.NetTickCnt = 0;
			break;
// 				case CMP_DWN_COIN_OUT:
// 						NetRetry = 0;
// 					L01_Ctr.DatTxFlg =1;
// 					break;
// 				case CMP_CONFIG_DX_ID:
// 						NetRetry = 0;
// 					L01_Ctr.DatTxFlg =1;
// 					break;
// 				case CMP_CONFIG_COIN_PAR:
// 						NetRetry = 0;
// 					L01_Ctr.DatTxFlg =1;
// 					break;
// 				case CMP_CONFIG_DIS_PAR:
// 						NetRetry = 0;
// 					L01_Ctr.DatTxFlg =1;
// 					break;
// 				case CMP_CONFIG_DIR_PAR:
// 						NetRetry = 0;
// 					L01_Ctr.DatTxFlg =1;
// 					break;
// 				case CMP_CONFIG_TEST:
// 						NetRetry = 0;
// 					L01_Ctr.DatTxFlg =1;
// 					break;
// 				case CMP_COIN_OT_TEST:
// 						NetRetry = 0;
// 					L01_Ctr.DatTxFlg =1;
// 					break;
// 				case CMP_RX_OK_TEST:
// 						NetRetry = 0;
// 					L01_Ctr.DatTxFlg =1;
// 					break;
// 				case CMP_CONFIG_FQ_DX:
// 						NetRetry = 0;
// 					L01_Ctr.DatTxFlg =1;
// 					break;
// 				case CMP_CONFIG_PRICE:
// 						NetRetry = 0;
// 					L01_Ctr.DatTxFlg =1;
// 					break;
// 				case CMP_CONFIG_PAR:
// 						NetRetry = 0;
// 					L01_Ctr.DatTxFlg =1;
// 					break;
			default:
			memset(&TxMsgBuf[Lcmdlen],0,(TX_PLOAD_WIDTH-Lcmdlen));
			L01_Ctr.DatTxFlg =1;
			gFlg.NetTickCnt = 0;
			break;
		}
	}
	
	if(tcp_client_sta & BIT_SET7)	//网络已经连接//--------------tcp发送数据-------------------*/
	{
			Wdatlen = queue_find_cmd(&gDxUpCmpBuf,Wnettxbf,NET_CMD_MAXLEN);
			if((Wdatlen>0) && (Wnettxbf[4] == DX_CMD)){//数据由读头上传	
#if NetDatExp_DBG 
					printf("网关----->服务器>>"); 
					showdbg8hex(Wnettxbf,Wdatlen);		
#endif
					s->textptr=Wnettxbf;
					s->textlen=Wdatlen;   														//发送数据长度  momo
			}else if(Wdatlen == 0){
				if(DlyVar.LinkGasT == 0){/* 队列中已经无数据待发送*/
					DlyVar.LinkGasT = LinkGasTCONST;
					if(++gFlg.NetTickCnt>LinkMAXCONST)
					{
						gFlg.NetTickCnt = 0;
						tcp_client_sta &= (~BIT_SET7);
					}
					else
					{
							FrameUpWxLink();
					}
				}
			}
	}
	if((gTCPReConnectT == 0) && ((tcp_client_sta & BIT_SET7) == 0))
	{
		s->textlen=0;   														//发送数据长度  momo
		s->state=STATE_CMD; 		//指令状态
		tcp_client_reconnect();	   		    		//链接断开重连
		//uip_polling();
		gTCPReConnectT = 300;	//3s钟重连一次
		//printf("gTCPReConnectT=%d",gTCPReConnectT);
		if(++NetRetry>3)SysReset();
	}
}
void Nrf24l01Task(void)
{
	uint8_t RxMsgBuf[TX_PLOAD_WIDTH];															//nrf24l01接收临时缓冲区	
	
	if(L01_Ctr.DatTxFlg > 0)/*=================================发送数据到卡头===================================*/
	{
			L01_Ctr.DatTxFlg = 0;								
			NRF24L01_RX_Mode(Config.WxFQ);													//切换值本机频道，等待接收数据
			if(NRF24L01_RxPacket(RxMsgBuf) == 0)										//数据接收成功
			{							
#if Nrf24l01TaskDBG >0
				printf("1W%d  <<---- data is:\r\n",Config.WxID); 
				showdbg8hex(TxMsgBuf,L01_CMD_MAXLEN);
#endif 
				NRF_RXLED =!NRF_RXLED;
				if(RxMsgBuf[4] == DX_CMD)//数据有读头发出
				{
					EnQueuex(&gDxUpCmpBuf,RxMsgBuf,RX_PLOAD_WIDTH);				//数据入队，待转处理
				}
				else
				{
#if Nrf24l01TaskDBG > 0
					printf("is not DX_CMD!\r\n"); 
#endif  
				}
			}
			else
			{
				;
			}
			
			NRF24L01_TX_Mode(Config.WxFQ);					//切换到读头本机频道
			TxMsgBuf[4] = WX_CMD;										//标识数据由读头发出
// #if Nrf24l01TaskDBG >0
// 			printf("L01 tx data is:");
// 			showdbg8hex(TxMsgBuf,L01_CMD_MAXLEN);
// #endif				
			if(NRF24L01_TxPacket(TxMsgBuf) == TX_OK)//下发成功		
			{ 	
#if Nrf24l01TaskDBG >0
				printf("2W%d ---->> ok!\r\n",Config.WxID);
#endif	
				NRF_TXLED =!NRF_TXLED;
			}
			else
			{
// #if Nrf24l01TaskDBG >0
// 				printf("W%d ---->> failed!\r\n",Config.WxID);
// #endif          
			}
	}
	else
	{
		if(L01_Ctr.TxCnt == 0)//切换到数据接收阶段
		{
			L01_Ctr.TxCnt = ReGASMAX;
			NRF24L01_RX_Mode(Config.WxFQ);													//切换值本机频道，等待接收数据
			if(NRF24L01_RxPacket(RxMsgBuf) == 0)										//数据接收成功
			{							
#if Nrf24l01TaskDBG >0
				printf("W%d <<---- ok  data is:\r\n",Config.WxID); 
				showdbg8hex(TxMsgBuf,L01_CMD_MAXLEN);
#endif 
				NRF_RXLED =!NRF_RXLED;
				if(RxMsgBuf[4] == DX_CMD)//数据有读头发出
				{
					EnQueuex(&gDxUpCmpBuf,RxMsgBuf,RX_PLOAD_WIDTH);				//数据入队，待转处理
				}
				else
				{
#if Nrf24l01TaskDBG > 0
					printf("is not DX_CMD!\r\n"); 
#endif  
				}
			}
			else
			{
				;
			}
		}
	}
}













//uip事件处理函数
//必须将该函数插入用户主循环,循环调用.
void uip_polling(void)
{
	uint8_t i;
	static struct timer periodic_timer, arp_timer;
	static uint8_t timer_ok=0;	 
	if(timer_ok==0)//仅初始化一次
	{
		timer_ok = 1;
		timer_set(&periodic_timer,CLOCK_SECOND/5);  //创建1个0.5秒的定时器 
		timer_set(&arp_timer,CLOCK_SECOND*2);	   	//创建1个10秒的定时器 
	}				 
	uip_len=tapdev_read();	//从网络设备读取一个IP包,得到数据长度.uip_len在uip.c中定义
	if(uip_len>0)//有数据
	{ 			
		//处理IP数据包(只有校验通过的IP包才会被接收) 
		if(BUF->type == htons(UIP_ETHTYPE_IP)){//是否是IP包? 
			uip_arp_ipin();	//去除以太网头结构，更新ARP表
			uip_input();   	//IP包处理
			//当上面的函数执行后，如果需要发送数据，则全局变量 uip_len > 0
			//需要发送的数据在uip_buf, 长度是uip_len  (这是2个全局变量)		    
			if(uip_len>0){//需要回应数据
				uip_arp_out();//加以太网头结构，在主动连接时可能要构造ARP请求
				tapdev_send();//发送数据到以太网
			}
		}
		else if (BUF->type==htons(UIP_ETHTYPE_ARP))//处理arp报文,是否是ARP请求包?
		{
			uip_arp_arpin();
 			//当上面的函数执行后，如果需要发送数据，则全局变量uip_len>0
			//需要发送的数据在uip_buf, 长度是uip_len(这是2个全局变量)
 			if(uip_len>0)
				tapdev_send();//需要发送数据,则通过tapdev_send发送	 
		}
	}
	else if(timer_expired(&periodic_timer))	//0.5秒定时器超时
	{
		timer_reset(&periodic_timer);		//复位0.5秒定时器 
		//轮流处理每个TCP连接, UIP_CONNS缺省是40个  
		for(i=0;i<UIP_CONNS;i++)
		{
			uip_periodic(i);	//处理TCP通信事件  
	 		//当上面的函数执行后，如果需要发送数据，则全局变量uip_len>0
			//需要发送的数据在uip_buf, 长度是uip_len (这是2个全局变量)
	 		if(uip_len>0)
			{
				uip_arp_out();//加以太网头结构，在主动连接时可能要构造ARP请求
				tapdev_send();//发送数据到以太网
			}
		}
#if UIP_UDP	//UIP_UDP 
		//轮流处理每个UDP连接, UIP_UDP_CONNS缺省是10个
		for(i=0;i<UIP_UDP_CONNS;i++)
		{
			uip_udp_periodic(i);	//处理UDP通信事件
	 		//当上面的函数执行后，如果需要发送数据，则全局变量uip_len>0
			//需要发送的数据在uip_buf, 长度是uip_len (这是2个全局变量)
			if(uip_len > 0)
			{
				uip_arp_out();//加以太网头结构，在主动连接时可能要构造ARP请求
				tapdev_send();//发送数据到以太网
			}
		}
#endif 
		//每隔10秒调用1次ARP定时器函数 用于定期ARP处理,ARP表10秒更新一次，旧的条目会被抛弃
		if(timer_expired(&arp_timer))
		{		
			timer_reset(&arp_timer);
			uip_arp_timer();
		}
	}
}





//m^n函数
//返回值:m^n次方.
u32 LCD_Pow(uint8_t m,uint8_t n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 


//数字->字符串转换函数
//将num数字(位数为len)转为字符串,存放在buf里面
//num:数字,整形
//buf:字符串缓存
//len:长度
void num2str(u16 num,uint8_t *buf,uint8_t len){
	u16 i;
	for(i=0;i<len;i++)
	{
		buf[i]=(num/LCD_Pow(10,len-i-1))%10+'0';
	}
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
		printf("%x,",bf[cnt]);
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

