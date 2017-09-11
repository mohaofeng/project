#ifndef _SYS_CONFIG_H
#define _SYS_CONFIG_H



//#include "types.h"
#include <stdint.h>
#include "./common/common.h" 
#include "utility.h"
#include "bsp_usart1.h"
#include "bsp_usart2.h"
#include "bsp_usart3.h"
#include "bsp_usart4.h"
#include "bsp_usart5.h"
#include "W5500_conf.h"
#include "Serialcd.h"
#include "rc522.h"
#include "WT588D.H"
#include "bsp_led.h"   
#include "stmflash.h"
#include <printer.h>
#include "bsp_usart2.h"
#include "exti.h"
#include "./pvd/bsp_pvd.h" 
#include "./ESP8266/bsp_esp8266.h" 



#define MAX_APNAME_LEN    				64   				/*  wifi名称最大长度  */
#define MAX_APPASSWORD_LEN  			64   				/*  wifi密码最大长度  */
#define FLASH_DAT_LEN 						250  			 	/*  配置数据区长度    */

#define FLASH_STORE_BASE          0            /*  外部存储器基地址  */


#define QUBI   1
#define GOUBI  2
#define CHAXUN 3
#define CUNBI  4

#define SUCCEED   1
#define FAILED  	2



#define  DEC_CNT_MAX 	5			//最大档位值
#define  RECONNECT_CNT_MAX 	10			//最大档位值







#define TCP_WAIT_TO_SEND   	0x0001			/* tcp有数据等待发送 				*/
#define TCP_CLOSED			0x0002			/* tcp链接已经关闭   				*/
#define TCP_INIT			0x0004			/* tcp正在初始化     				*/
#define TCP_CONNECTTING		0x0008    		/* tcp正在建立链接   				*/
#define TCP_CONNECTED		0x0010			/* tcp已经链接上     				*/
#define TCP_CLOSEING		0x0020			/* tcp等待关闭       				*/
#define TCP_RECV_DATA		0x0040			/* tcp接收到数据     				*/


#define WiFi_CONNECTED		0x0080			/* wifi已经建立连接    				*/









/* 输入输出io口定义 */

#define GATE_CTR_SIG 	PDout(4)		//继电器输出
#define COIN_SIG 		PCout(2)		//投币脉冲输出







/* 运行模式定义 */
#define RunMod_KEYINPUT 						0X01   	//按键项目消费机模式
#define RunMod_NORMAL_FIX  						0X02	 	//固定不可变金额模式
#define RunMod_NORMAL_DANGWEI  					0X03	 	//固定带档位切换模式
#define RunMod_CHECK							0X04   	//查询模式
#define RunMod_SAVER_COIN      					0X05   	//存币机模式
#define RunMod_SALE_COIN  						0x06	 	//取币机模式
#define RunMod_SALE_SAVER_COIN  				0x07	 	//存取币机模式
#define RunMod_GATE_CTRL  						0x08	 	//道闸控制器模式

/* 指令号定义 1=扣费，2=查余额，3=存代币，4=存彩票，5=取代币，6=购买代币，7=扫描支付*/
#define DEC_MON_CMD 												1   		//扣费
#define CHECK_MON_CMD  											2	 			//查余额
#define SAVE_COIN_DAI					  						3	 			//存代币
#define SAVE_CAI_PIAO												4   		//存彩票
#define GET_COIN_DAI      									5   		//取代币
#define BUY_COIN_DAI 												6	 			//购买代币
#define SCAN_PAY  													7	 			//扫描支付







/* 高低电平定义 */
#define TTL_NC 0 										//默认读头id
#define TTL_NO 1 										//默认读头id
/* 高低电平定义 */
#define DHCP_OFF 0 										//默认读头id
#define DHCP_ON 1 										//默认读头id
/* 高低电平定义 */
#define GATE_NC 0 										//默认读头id
#define GATE_NO 1 										//默认读头id

/* 显示单位定义 */
#define BI 1 										//默认读头id
#define YUAN 2 									//默认读头id
/* 通信方式定 */
#define NET_RJ45  			1   					//网线联网
#define NET_WIFI  			2   					//WIFI联网
#define NET_GPRS  			4   					//GPRS联网
#define NET_2_4G  			8   					//2.4G联网

/* 设备类型定义 */
#define XN     		    0X01						//消费机
#define ZN     		    0X02						//轧机
#define CCGN     			0X03						//存储柜

/* 卡类别定义 */
#define CHUZHI     		0X01						//储值卡
#define SHIJIAN     	0X02						//时间卡
#define JICI     			0X03						//记次卡
#define GUANLI     		0X04						//管理卡
#define FEIFA     		0X05						//消费成功


#define BIT_SET0  	0X0001
#define BIT_SET1  	0X0002
#define BIT_SET2  	0X0004
#define BIT_SET3  	0X0008
#define BIT_SET4  	0X0010
#define BIT_SET5  	0X0020
#define BIT_SET6  	0X0040
#define BIT_SET7  	0X0080
#define BIT_SET8  	0X0100
#define BIT_SET9 	0X0200
#define BIT_SET10  	0X0400
#define BIT_SET11  	0X0800
#define BIT_SET12 	0X1000
#define BIT_SET13  	0X2000
#define BIT_SET14  	0X4000
#define BIT_SET15  	0X8000



/*------------------调试宏定义------------------------*/
#define MAIN_DBG	0
#define TEST_DBG	0
#define NetDatExp_DBG	0
#define TcpClientTaskDBG 0
#define ESP07_DBG  0
#define LCDDisplayTaskDBG 0
#define a6_moduleDBG 0
#define esp8266_moduleDBG 0
#define AT_CMD_DBG 0





/*---------------------------常数定义----------------------*/



#define LED_GAS_CONST   500//读头状态信息上传间隔 100x10ms
#define LinkGas_CONST 3000  //心跳间隔常数
#define CONNECT_Gas_CONST 500  //心跳间隔常数
#define ToolBarDlyCONST 200  //

#define RC522FLASH_CONST					200       	//rc522指示灯点亮持续时间
#define ICScanGas_CONST 					100        //同一用户刷卡限制时间间隔
#define KEY_SCAN_CONST 						400        //同一用户刷卡限制时间间隔

#define LINK_MAX_NUM 3				 		//链路最大重发次数
#define CONNECT_MAX_NUM 3				 //链路最大重发次数
#define QLEN_MAX 2048				 			//队列缓冲区最大长度






/* 彩屏显示模式定义 */


#define SERIAL_LCD          1         			//开启彩屏功能
#define DIS_NULL_MOD 				0x00
#define DIS_IDE_MOD 				0x01
#define DIS_Cmp_MOD 				0x02
#define DIS_LOGIN_MOD 			0x03
#define DIS_INIT_MOD 				0xff


#define CHECK_MON_FLG 				0x0001     //余额查询成功标志


#define MAX_APNAME_LEN    	64
#define MAX_APPASSWORD_LEN  64








#define SYS_LED PBout(2)// PB5 








/*  系统默认配置参数 */
#define DEFAULT_TAG_FLG    		2
#define DEFAULT_ID      	 	1
#define DEFAULT_COIN_TIM    	30
#define DEFAULT_COIN_GAS    	100
#define DEFAULT_GATE_TIM    	1000
#define DEFAULT_GATE_GAS    	1000
#define DEFAULT_DecMon_10   	100
#define DEFAULT_DecMon_20   	200
#define DEFAULT_DecMon_30   	300
#define DEFAULT_DecMon_40   	400
#define DEFAULT_DecMon_50   	500
#define DEFAULT_PromptTim   	1000
#define DEFAULT_KEY      		123456
#define DEFAULT_Hwver      		27
#define DEFAULT_Swver      		27
#define DEFAULT_DecMon_10   	100
#define DEFAULT_DecMon_20   	200
#define DEFAULT_DecMon_30   	300
#define DEFAULT_DecMon_40   	400
#define DEFAULT_DecMon_50   	500
#define DEFAULT_DHCP_STA   		DHCP_ON

//#define IP_FROM_DEFINE	        0       			              /*使用初始定义的IP信息*/
//#define IP_FROM_DHCP	        1       			              /*使用DHCP获取IP信息*/

#pragma pack(1)

//系统配置信息
typedef struct
{
	uint8_t  NetMod;													//联网方式
	uint32_t deviceid;
	uint32_t LoginKey;
	
	uint32_t PlusWith;
	uint32_t PlusGas;												//上分间隔时间
	uint32_t GateWith;
	uint32_t GateGas;
	uint32_t GateSta;
	uint32_t PromptTim;    									//金额显示停留时间
	
	uint8_t  Rmoteip[4];											//服务器ip
	uint8_t  Localip[4];											//本机ip
	uint8_t  GateW[4];												//默认网关
	uint8_t  Mass[4];												//子网掩码
	uint16_t RemotePort;													//端口号
	uint16_t tagflg;												//标志
	uint16_t DeviceType;
	uint8_t  hwver;
	uint8_t  swver;
	uint16_t DecMon[5];											//档位信息
	uint8_t  ICKey[6];											//卡片验证密码
	uint8_t  runmod;
	uint8_t  ttlLe;													//常态电平
	uint8_t  dhcpsw;													//dhcp开关
	uint8_t  Mac[6];
	uint32_t SysSn[3];
	uint8_t  APName[MAX_APNAME_LEN+2];//ap名称
	uint8_t  APPassword[MAX_APPASSWORD_LEN+2];//ap密码
	uint8_t  dns[4];
	uint16_t local_port;
}__ConfigVar;
#pragma pack()

extern __ConfigVar Config;								//系统配置参数



typedef struct
{
	uint8_t 	ucisOk;				//
	uint8_t 	cmdid;				//
	
	
	uint16_t 	wxpos;				//显示x坐标
	uint16_t 	wypos;				//显示y坐标
	uint16_t 	ucFcolor; 		//显示前景色
	uint16_t	ucBcolor; 		//显示背景色
	uint8_t 	ucFontsize;		//显示字体大小

	uint16_t 	ucIdeFcolor; 		//显示前景色
	uint16_t	ucIdeBcolor; 		//显示背景色
	uint16_t 	wIdexpos;				//显示x坐标
	uint16_t 	wIdeypos;				//显示y坐标
	uint8_t 	ucIdeFontsize;		//显示字体大小
	
	uint32_t 	wCoinCnt;  		//投币次数
	uint32_t 	wGateCnt;  		//开门次数
	uint32_t 	wDecNum;  		//消耗数量
	uint16_t 	wSountType;		//语音编号
	uint8_t 	ScreenWaitTim;//界面恢复时间
	uint8_t 	SerParm;			//预留字
	char name[400];					//显示内容
	char IdeShowStr[800];					//显示内容
}__VipImfor;
extern __VipImfor VipImfor;


//网络控制器
typedef struct
{
	uint8_t  WiFiRmoteip[4];											//服务器ip
	uint8_t  WiFiLocalip[4];											//本机ip
	uint8_t  WiFiGateW[4];												//默认网关
	uint8_t  WiFiMass[4];												//子网掩码
	uint16_t WiFiRemotePort;											//端口号

	uint8_t  LanRmoteip[4];											//服务器ip
	uint8_t  LanLocalip[4];											//本机ip
	uint8_t  LanGateW[4];												//默认网关
	uint8_t  LanMass[4];												//子网掩码
	uint16_t LanRemotePort;											//端口号

	uint16_t TxFrameBufLen;						//发送缓冲区长度
	uint16_t RxFrameBufLen;						//接收缓冲区长度

	uint16_t tcp_sta_old;						//nrf24l01发送标志z
	uint16_t tcp_sta_new;						//nrf24l01发送标志z
	
	
	uint16_t netsta;									//有数据待发送标志
	uint8_t NetConnectRetryCnt;					//网络重连次数
	uint16_t NetChkGas;								//网络状态检查间隔
	uint16_t ReConnectGas;						//网络重连间隔
	uint16_t LinkGas;									//网络心跳间隔
	uint16_t LinkRetryCnt;						//网络心跳重发计数器
	uint8_t  WiFiStpSta;									//网络心跳重发计数器
	
	uint8_t csp;
	uint8_t flg;
}__net_ctr;
extern __net_ctr gNetCtr;

//系统定时信息
typedef struct{
	uint32_t tst_gas1;			//测试用
	uint32_t LCDIdeT;				//待机界面恢复时间
	uint32_t LCDIdeTUpdatGas;				//待机界面刷屏间隔
	uint32_t gGateSigDly;			//上币脉冲宽度
	uint32_t gCoinSigDly;			//上币脉冲宽度
	uint32_t gKeyScanGas;			//按键扫描间隔
	uint32_t SysStartDly;
//	uint16_t ToolBarDly;    //工具栏更新间隔时间
	uint32_t tst_gas;
	uint32_t gLED_Gas;
	uint32_t wifi_mod_key_Dly;
	uint32_t lan_mod_key_Dly;
	uint32_t LcdDisTaskDly;
	uint32_t PlusCntOverTim;
	uint32_t PlusCntLT;
	uint32_t PlusCntHT;
	uint32_t PlusCntDly;
	uint32_t UpdateCoinCntDly;
	uint32_t NetScanGas;
	
}__SysDlyT;
extern __SysDlyT DlyVar;

//系统标志
typedef struct{
	char *pc[10];
	char srttmp[1024];
}__TmpVar;
extern __TmpVar TmpVar;	 




//系统全局变量
volatile typedef struct
{
	uint8_t PlusNum; 		//脉冲数量
	uint8_t DisType; 	//显示模式
	uint8_t UpDatCnt; 	//上传数据序号
	uint32_t PlusCntSum;//脉冲计数器时钟
	uint32_t WaitUpPlusCntSum;//脉冲计数器时钟
	uint32_t icsn;
	uint32_t icsn_old;
	uint32_t icsn_new;
	uint8_t CntFlg;
	uint8_t CmdID;
	uint16_t ALLFlg;
	uint16_t connect_sta;	
	uint32_t GetVipIdFlg;
	uint8_t ucTcpClosedFlag;
}__Sys_Var;
extern __Sys_Var SysVar;


//会员信息集合
typedef struct
{
	uint8_t *name;		//姓名
	uint8_t *call;		//电话
	uint32_t kajin;		//本机
	uint32_t zengjin;	//赠金余额
	uint32_t jifen;		//积分余额
	uint32_t daibi;		//代币余额
	uint32_t caipiao;	//彩票余额
	uint32_t cardsn;	//卡序号
	uint32_t cardid;	//卡编号
	
}__VIPImfor;
extern __VIPImfor VIPImfor;

//系统标志
typedef struct{
	uint16_t KeyVal;				//上币脉冲数量
	uint16_t PlusSt;				//上币脉冲数量
	uint16_t KeySta;				//上币脉冲数量
	uint32_t gDecMon;				//当前消费金额
	uint16_t gDecMonCnt;				//当前档位状态
	uint32_t GateSigNum;
	uint32_t CoinSigNum;				//上币脉冲数量
	uint8_t  dhcp_ok;
	uint32_t ChipUniqueID[3];
	uint8_t VipIDString[400];					//显示内容
	uint8_t pString[200];
}__SysFlg;
extern __SysFlg g_tFlg;	


void SysParmSet(void);
void SysParmWrite(void);
void SysParmExplain(void);
void SysParmRestorDefault(void);
void SysParmPrint(void);
void SysInit(void);
void SysReset(void);
void NetInit(void);

void showdbg32hex(u32 *bf,u8 len);
void showdbg32dec(u32 *bf,u8 len);
void showdbg8hex(u8 *bf,u8 len);
void showdbg16hex(u16 *bf,u8 len);
void showdbg8dec(u8 *bf,u8 len);


#endif




