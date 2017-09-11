
/*--------------------------log---------------------------------
1.Э��ջ��ʱ������Ϊtim3 
2.ͨ��ϵͳ��λ����������Ͽ����Ӻ����ݷ����ط�������
3.���ݴ������ڸ���Ϊ200ms��֮ǰΪ500ms��
4.���Ӷ�ͷˢ���ɹ������ݻָ���������ˢ������
5.δ���ö�ͷID��δ0
6.����������ӳ�ʱ���޷���������20160225
-------------------2017-05-06
1.ι���Ź���������س��ڲ��ϵ���������ܷ�
-------------------2017-05-27
1.��������ʼ��ʧ�ܵ���������������
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
extern uint8_t tcp_client_sta;											//�ͻ���״̬
uint8_t NetRetry = 0;
uint16_t gTCPReConnectT = 0;
void SysReset(void);

QUEUE 	gCmpDwDxBuf;											//��λ���´���������
QUEUE 	gDxUpCmpBuf;										//��ͷ�ϴ���������

uint8_t WxSwVer;//����汾
uint8_t WxHwVer;//Ӳ���汾
uint8_t TxMsgBuf[NET_CMD_MAXLEN];										  	//nrf24l01���ͻ�����
uint32_t ChipUniqueID[3];
uint32_t random_seed=1;
uint8_t Wnettxbf[NET_CMD_MAXLEN];//tcp ���͵�������������
__SysFlg gFlg;	


__nrf24l01_ctr L01_Ctr;
//__Tst_ctr tst_ctr;
uint8_t gNrfLnkGas;  																		//NRF24L01 ��ʱ����
__SysDlyT DlyVar;

__ConfigVar Config;//ϵͳ���ò���


#define BUF ((struct uip_eth_hdr *)&uip_buf[0])	 		  

void uip_polling(void);

u32 LCD_Pow(uint8_t m,uint8_t n);								//m^n����
void num2str(u16 num,uint8_t *buf,uint8_t len);	//����->�ַ���ת������
void NetDatExp(void);



/***********************************************
Function: д��ϵͳ���ò���
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
Function: ϵͳ���ò�������
************************************************/
void ExplainParm(void)
{
	uint16_t expbf[FLASH_DAT_LEN];
	STMFLASH_Read(STM32_FLASH_BASE,expbf,FLASH_DAT_LEN);//��ȡϵͳ����
	
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
 	printf("��ȡϵͳ����---------------------------------:\r\n");	
	printf("ϵͳ��־��%d\r\n",Config.tagflg);
	printf("����ID��%d\r\n",Config.WxID);
	printf("�ӿ�ͷ������%d\r\n",Config.DxNum);
	printf("�ӿ�ͷ�б�");
	showdbg8hex(&Config.DxLst[0],Config.DxNum);
	
	printf("������ip��ַ��%d.%d.%d.%d\r\n",Config.Rmoteip[3],Config.Rmoteip[2],Config.Rmoteip[1],Config.Rmoteip[0]);
	printf("����ip��ַ��%d.%d.%d.%d\r\n",Config.Localip[3],Config.Localip[2],Config.Localip[1],Config.Localip[0]);
	printf("Ĭ�����أ�%d.%d.%d.%d\r\n",Config.GateW[3],Config.GateW[2],Config.GateW[1],Config.GateW[0]);
	printf("�������룺%d.%d.%d.%d\r\n",Config.Mass[3],Config.Mass[2],Config.Mass[1],Config.Mass[0]);
	printf("�˿ںţ�%d\r\n",Config.Port);
	printf("����mac��ַΪ��%d:%d:%d:%d:%d:%d\r\n",Config.Mac[0],Config.Mac[1],Config.Mac[2],Config.Mac[3],Config.Mac[4],Config.Mac[5]);
	printf("��ͷƵ����%d\r\n",Config.DxFQ);
	printf("����Ƶ����%d\r\n",Config.WxFQ);
}
/***********************************************
Function: ����Ĭ��ϵͳ����
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
void LanInit(void){
	uip_ipaddr_t ipaddr;
 	while(tapdev_init())
	{	//��ʼ��ENC28J60����
		printf("ENC28J60 Init Error!\r\n");
		delay_ms(1000);
		SYS_LED=!SYS_LED;    //nrf24l01��ʼ��ʧ�ܣ�ָʾ������
	}		
	uip_init();				//uIP��ʼ��	  
 	uip_ipaddr(ipaddr, Config.Localip[3],Config.Localip[2],Config.Localip[1],Config.Localip[0]);	//����ת����������ñ�������IP��ַ
	uip_sethostaddr(ipaddr);					    
	uip_ipaddr(ipaddr, Config.GateW[3],Config.GateW[2],Config.GateW[1],Config.GateW[0]); 	//��������IP��ַ(��ʵ������·������IP��ַ)
	uip_setdraddr(ipaddr);						 
	uip_ipaddr(ipaddr, Config.Mass[3],Config.Mass[2],Config.Mass[1],Config.Mass[0]);	//������������
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
////////////////////////////////α����������취////////////////////////////////
//��ȡα�����
//���Բ���0~RANDOM_MAX-1�������
//seed:����
//max:���ֵ	  		  
//����ֵ:0~(max-1)�е�һ��ֵ 		
uint32_t app_get_rand(uint32_t max,uint32_t min)
{			    	    
	random_seed=random_seed*22695477+1;
//  rand(random_seed);
	return ((uint32_t)(random_seed)%(max-min+1)+min); 
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
	gTCPReConnectT = CONNECT_OVER_CONST;	//3s������һ��
	if(RST_KEY == 0)
	{
		delay_ms(10);
		if(RST_KEY == 0)
		{
			WriteDefaultSet();
		}
	}
	TIM3_Int_Init(1000,639);//100Khz����Ƶ�ʣ�������1000Ϊ10ms
	CreateLanQueue(&gCmpDwDxBuf);//������λ���·������������
	CreateLanQueue(&gDxUpCmpBuf);//������ͷ�ϴ������������
	ClearQ(&gCmpDwDxBuf);
	ClearQ(&gDxUpCmpBuf);
	delay_ms((Config.WxID%50)+1);
	IWDG_Init(4,625);   
		
	while (1)
	{
			LEDTask();			
			if(tcp_client_sta & BIT_SET7 && (gFlg.netstaflg !=0))	//�����Ѿ�����//--------------tcp��������-------------------*/
			{
				Nrf24l01Task();
			}
			uip_polling();
			NetDatExp();
			if(DlyVar.wdog_gas == 0)//100ms ι���Ź�һ��
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
		
	
		Lcmdlen = queue_find_cmd(&gCmpDwDxBuf,TxMsgBuf,NET_CMD_MAXLEN);//ȡ��һ֡����
		if(Lcmdlen >0)
		{
#if NetDatExp_DBG 
			printf("������----->���أ�");
			showdbg8hex(TxMsgBuf,Lcmdlen);
#endif	
			tmpdt = ((uint16_t)TxMsgBuf[1]<<8)+(((uint16_t)TxMsgBuf[2])<<0);
			switch(TxMsgBuf[3])
			{
				case CMP_CONFIG_WX_ID://��������id��
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
			case CMP_CONFIG_WX_IP://��������������iP
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
						printf("Ƶ���ų�������\r\n");
					WriteParm();
					ExplainParm();
					PrintCfgPar();
					FrameUpWxConfig();//����������Ϣ
				}
					
			break;
			case CMP_RESET_WX:
					NetRetry = 0;
					DlyVar.LinkGasT = LinkGasTCONST;				
					gFlg.NetTickCnt = 0;
					SysReset();
			break;
			case CMP_TEST_WX://��������������
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
						FrameUpWxConfig();//����������Ϣ
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
	
	if(tcp_client_sta & BIT_SET7)	//�����Ѿ�����//--------------tcp��������-------------------*/
	{
			Wdatlen = queue_find_cmd(&gDxUpCmpBuf,Wnettxbf,NET_CMD_MAXLEN);
			if((Wdatlen>0) && (Wnettxbf[4] == DX_CMD)){//�����ɶ�ͷ�ϴ�	
#if NetDatExp_DBG 
					printf("����----->������>>"); 
					showdbg8hex(Wnettxbf,Wdatlen);		
#endif
					s->textptr=Wnettxbf;
					s->textlen=Wdatlen;   														//�������ݳ���  momo
			}else if(Wdatlen == 0){
				if(DlyVar.LinkGasT == 0){/* �������Ѿ������ݴ�����*/
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
		s->textlen=0;   														//�������ݳ���  momo
		s->state=STATE_CMD; 		//ָ��״̬
		tcp_client_reconnect();	   		    		//���ӶϿ�����
		//uip_polling();
		gTCPReConnectT = 300;	//3s������һ��
		//printf("gTCPReConnectT=%d",gTCPReConnectT);
		if(++NetRetry>3)SysReset();
	}
}
void Nrf24l01Task(void)
{
	uint8_t RxMsgBuf[TX_PLOAD_WIDTH];															//nrf24l01������ʱ������	
	
	if(L01_Ctr.DatTxFlg > 0)/*=================================�������ݵ���ͷ===================================*/
	{
			L01_Ctr.DatTxFlg = 0;								
			NRF24L01_RX_Mode(Config.WxFQ);													//�л�ֵ����Ƶ�����ȴ���������
			if(NRF24L01_RxPacket(RxMsgBuf) == 0)										//���ݽ��ճɹ�
			{							
#if Nrf24l01TaskDBG >0
				printf("1W%d  <<---- data is:\r\n",Config.WxID); 
				showdbg8hex(TxMsgBuf,L01_CMD_MAXLEN);
#endif 
				NRF_RXLED =!NRF_RXLED;
				if(RxMsgBuf[4] == DX_CMD)//�����ж�ͷ����
				{
					EnQueuex(&gDxUpCmpBuf,RxMsgBuf,RX_PLOAD_WIDTH);				//������ӣ���ת����
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
			
			NRF24L01_TX_Mode(Config.WxFQ);					//�л�����ͷ����Ƶ��
			TxMsgBuf[4] = WX_CMD;										//��ʶ�����ɶ�ͷ����
// #if Nrf24l01TaskDBG >0
// 			printf("L01 tx data is:");
// 			showdbg8hex(TxMsgBuf,L01_CMD_MAXLEN);
// #endif				
			if(NRF24L01_TxPacket(TxMsgBuf) == TX_OK)//�·��ɹ�		
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
		if(L01_Ctr.TxCnt == 0)//�л������ݽ��ս׶�
		{
			L01_Ctr.TxCnt = ReGASMAX;
			NRF24L01_RX_Mode(Config.WxFQ);													//�л�ֵ����Ƶ�����ȴ���������
			if(NRF24L01_RxPacket(RxMsgBuf) == 0)										//���ݽ��ճɹ�
			{							
#if Nrf24l01TaskDBG >0
				printf("W%d <<---- ok  data is:\r\n",Config.WxID); 
				showdbg8hex(TxMsgBuf,L01_CMD_MAXLEN);
#endif 
				NRF_RXLED =!NRF_RXLED;
				if(RxMsgBuf[4] == DX_CMD)//�����ж�ͷ����
				{
					EnQueuex(&gDxUpCmpBuf,RxMsgBuf,RX_PLOAD_WIDTH);				//������ӣ���ת����
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













//uip�¼�������
//���뽫�ú��������û���ѭ��,ѭ������.
void uip_polling(void)
{
	uint8_t i;
	static struct timer periodic_timer, arp_timer;
	static uint8_t timer_ok=0;	 
	if(timer_ok==0)//����ʼ��һ��
	{
		timer_ok = 1;
		timer_set(&periodic_timer,CLOCK_SECOND/5);  //����1��0.5��Ķ�ʱ�� 
		timer_set(&arp_timer,CLOCK_SECOND*2);	   	//����1��10��Ķ�ʱ�� 
	}				 
	uip_len=tapdev_read();	//�������豸��ȡһ��IP��,�õ����ݳ���.uip_len��uip.c�ж���
	if(uip_len>0)//������
	{ 			
		//����IP���ݰ�(ֻ��У��ͨ����IP���Żᱻ����) 
		if(BUF->type == htons(UIP_ETHTYPE_IP)){//�Ƿ���IP��? 
			uip_arp_ipin();	//ȥ����̫��ͷ�ṹ������ARP��
			uip_input();   	//IP������
			//������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ��� uip_len > 0
			//��Ҫ���͵�������uip_buf, ������uip_len  (����2��ȫ�ֱ���)		    
			if(uip_len>0){//��Ҫ��Ӧ����
				uip_arp_out();//����̫��ͷ�ṹ������������ʱ����Ҫ����ARP����
				tapdev_send();//�������ݵ���̫��
			}
		}
		else if (BUF->type==htons(UIP_ETHTYPE_ARP))//����arp����,�Ƿ���ARP�����?
		{
			uip_arp_arpin();
 			//������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ���uip_len>0
			//��Ҫ���͵�������uip_buf, ������uip_len(����2��ȫ�ֱ���)
 			if(uip_len>0)
				tapdev_send();//��Ҫ��������,��ͨ��tapdev_send����	 
		}
	}
	else if(timer_expired(&periodic_timer))	//0.5�붨ʱ����ʱ
	{
		timer_reset(&periodic_timer);		//��λ0.5�붨ʱ�� 
		//��������ÿ��TCP����, UIP_CONNSȱʡ��40��  
		for(i=0;i<UIP_CONNS;i++)
		{
			uip_periodic(i);	//����TCPͨ���¼�  
	 		//������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ���uip_len>0
			//��Ҫ���͵�������uip_buf, ������uip_len (����2��ȫ�ֱ���)
	 		if(uip_len>0)
			{
				uip_arp_out();//����̫��ͷ�ṹ������������ʱ����Ҫ����ARP����
				tapdev_send();//�������ݵ���̫��
			}
		}
#if UIP_UDP	//UIP_UDP 
		//��������ÿ��UDP����, UIP_UDP_CONNSȱʡ��10��
		for(i=0;i<UIP_UDP_CONNS;i++)
		{
			uip_udp_periodic(i);	//����UDPͨ���¼�
	 		//������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ���uip_len>0
			//��Ҫ���͵�������uip_buf, ������uip_len (����2��ȫ�ֱ���)
			if(uip_len > 0)
			{
				uip_arp_out();//����̫��ͷ�ṹ������������ʱ����Ҫ����ARP����
				tapdev_send();//�������ݵ���̫��
			}
		}
#endif 
		//ÿ��10�����1��ARP��ʱ������ ���ڶ���ARP����,ARP��10�����һ�Σ��ɵ���Ŀ�ᱻ����
		if(timer_expired(&arp_timer))
		{		
			timer_reset(&arp_timer);
			uip_arp_timer();
		}
	}
}





//m^n����
//����ֵ:m^n�η�.
u32 LCD_Pow(uint8_t m,uint8_t n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 


//����->�ַ���ת������
//��num����(λ��Ϊlen)תΪ�ַ���,�����buf����
//num:����,����
//buf:�ַ�������
//len:����
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
//ϵͳ�����λ
void SysReset(void)
{
	*((uint32_t*)0xe000ed0c)=0x05fa0004;
}

