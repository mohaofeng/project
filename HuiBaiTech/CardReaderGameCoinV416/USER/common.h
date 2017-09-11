#ifndef _COMMONFUN_H
#define _COMMONFUN_H
#include <stdint.h>

#define BIT_SET0  0X0001
#define BIT_SET1  0X0002
#define BIT_SET2  0X0004
#define BIT_SET3  0X0008
#define BIT_SET4  0X0010
#define BIT_SET5  0X0020
#define BIT_SET6  0X0040
#define BIT_SET7  0X0080
#define BIT_SET8  0X0100
#define BIT_SET9 	0X0200
#define BIT_SET10  0X0400
#define BIT_SET11  0X0800
#define BIT_SET12  0X1000
#define BIT_SET13  0X2000
#define BIT_SET14  0X4000
#define BIT_SET15  0X8000
#define NetRxOk  BIT_SET0
#define NetTxOk  BIT_SET1


/*------------------调试宏定义------------------------*/
#define	TCP_DBG  0
#define	MAIN_DBG	0
#define TEST_DBG	0
#define BEEP_DBG	0
#define RC522TASK_DBG	0
#define NetDatExp_DBG	0
#define Nrf24l01TaskDBG 0   //nrf24l01调试宏
#define GateSigDBG 				0   //nrf24l01调试宏

#define DIS_NORMAL 1
#define DIS_INSER 0
#define NUM_FILL_0   0x80

#define XPlus  10
#define TimPlus  10

/*---------------------------常数定义----------------------*/
#define BI 1 					//默认读头id
#define YUAN 2 					//默认读头id

#define FLASH_DAT_LEN 20  				//配置数据区长度
#define DEFAULT_Price BI  				//默认网关id
#define DEFAULT_DX_ID 10 					//默认读头id
#define DEFAULT_TAG_FLG      8
#define ReTxcntMAX      20				//当前指令最大重发次数
#define ReGASMAX      1					//无数据发送时接收数据时间间隔
#define TxCntMAX_CONST		1000         //无线数据发送间隔
#define TEST_TxCntMAX_CONST		3000         //无线数据发送间隔
/*---------------------------状态字定义----------------------*/
#define WX_CMD 0x01
#define DX_CMD 0x02

#define SHEARCH_INVALID_IC 	0x02   //服务器处理失败
#define SHEARCH_IC_OK 			0x01   			//用户信息查询ok
#define SHEARCH_NO_PAY 			0x03   		//余额不足
#define OVER_TIM_ERR 				0x04   		//余额不足
#define LOSS_ERR 						0x05   		//余额不足

#define ACK__ERR 0x04   //err
#define ACK_SN_ERR 0x02   //服务器返回ic卡号不同
#define COIN_OUT_OK 0x00   //上币完成

#define BEEP_ON_PWM 0
#define BEEP_Q    10000
#define BEEP_P    BEEP_Q/5



#define QLEN_MAX 400				 //队列缓冲区最大长度
#define NET_CMD_MAXLEN  32   //上位机命令最大长度

/*-------------------------时间宏定义 --------------------*/
#define LED_GAS_CONST   							(50*XPlus)    		//LED 闪烁间隔时间 500ms
#define Rc522Rd_GAS_CONST   					(10*XPlus)    		//读卡间隔扫描时间  10*10ms
#define HOLD_MOD_CONST 								(3*XPlus)   			//待机界面恢复时间 20x10ms
#define NRF_LNK_CONST_MAX 						(15*XPlus)  			//NRF24L01链接间隔时间  x*10ms
#define LED_SCAN_CONST 								(1*XPlus)					//背光扫描间隔

#define PLUS_WITH_H_T  								(4)				//高电平
#define PLUS_WITH_L_T  								(4)				//高电平持续时间
#define PLUS_WITH_OVER_T  						(2000)				//脉冲计数超时时间

#define RED_LIGHT_TIM_CONST 					(15*XPlus)
#define GREEN_LIGHT_TIM_CONST 				(10*XPlus)
#define BLUE_LIGHT_TIM_CONST 					(10*XPlus)
#define BEEP_LIT_CONST 								(20*XPlus)    //5x10ms
#define BEEP_STP_CONST 								(10*XPlus)    //5x10ms
#define RedFLASH_CONST                (5*XPlus)
#define RED_FLASH_CONST 							(30*XPlus)      			//同一用户刷卡限制时间间隔




/*------------------------- 常量定义 --------------------*/
#define rstMAX_CONST  								1000
#define gstMAX_CONST  								1000
#define bstMAX_CONST  								1000

#define rstMIN_CONST 	 								2
#define gstMIN_CONST  								2
#define bstMIN_CONST  								2

#define DxSwVerDEF										44
#define DxHwVerDEF										32

#define DEFAULT_DxFQ_NUM 							100 					//默认频道
#define DEFAULT_WxFQ_NUM 							100 					//默认频道

#define LIGHT_STP   									5   
#define DEC_LIGHT   									1   					//亮度增加
#define INC_LIGHT   									0							//亮度减少

/*------------------------- 显示模式定义 --------------------*/
#define UNKNOW_ERR 					0x00   									//未知错误
#define IDE_MOD 						0x01
#define DEC_MOD 						0x02
#define Invalid_IC_MOD 			0x04     	//非法卡
#define NOLNK_MOD 					0x05
#define ICINFO_MOD 					0x07  	 	//刷卡成功，扣费成功
#define NOMON_MOD 					0x08   		//刷卡成功，余额不足
#define ERR_PROMPT 					0X09  		//提示联系管理员
#define ERR_LOSS_DIS 				0X0a 		 	//提示联系管理员
#define ERR_OVER_DIS 				0X0b  		//提示联系管理员
#define CONI_SUM_DIS 				0X0C  		//显示积分或彩票
#define CNT_TEMP_DIS 				0X0D  		//提示开始点票
#define OPTOO_OFENT 				0X0E  		//提示开始点票

#define DLY_Typ 						0Xff  		//显示积分或彩票



/*------------------------- 背光模式定义--------------------*/
#define RGB_IDE_MOD  						1
#define RGB_Invalid_IC_MOD  		2
#define RGB_NOLNK_MOD  					3
#define RGB_NOMON_MOD  					4
#define RGB_ERR_PROMPT  				5
#define RGB_UNKNOW_ERR  				6
#define RGB_ERR_LOSS_DIS  			7
#define RGB_ERR_OVER_DIS  			8
#define RGB_ICINFO_MOD					9
#define RGB_CNT_TEMP_DIS				10



#define RunMod_NORMAL 1
#define RunMod_CHECK  2
#define RunMod_SAVER  3





#define RFID_TYPE 1







//系统配置信息
typedef struct
{
	uint16_t tagflg;											//标志
	uint8_t Price;												//单价
	uint16_t DxID;												//子读头数量
	uint8_t CoinX;												//投币倍率
	uint8_t CoinOutLevel;									//服务器ip
	uint16_t CoinSigHod;										//
	uint16_t CoinSigGas;										//
	
	uint8_t DeviceType;										//默认网关
	uint8_t DxSwVer;											//软件版本
	uint8_t DxHwVer;											//硬件版本号	
	uint8_t disdir;												//显示方向
	uint16_t PromptTim;												//显示方向
	uint8_t DxFQ;													//内置频道
	uint8_t WxFQ;													//内置频道
	uint8_t RunMod;												//运行模式
	uint32_t DecMon;											//消费额度	
	uint32_t ICSn;												//ic卡号
	uint32_t FPlusCntSum;									//未存票数
}__ConfigVar;

extern __ConfigVar Config;							//系统配置参数

//LCD重要参数集
typedef struct  
{										    
	uint16_t width;			//LCD 宽度
	uint16_t height;			//LCD 高度
	uint8_t DisFrameUp;
}_lcd_dev; 	  

extern _lcd_dev lcddev;	//管理LCD重要参数

//rc522 控制器
typedef struct
{
	uint16_t RedLightValue;				//红
	uint16_t GreenLightValue;			//绿
	uint16_t BlueLightValue;				//蓝
	uint16_t RedLightTim;				//蓝
	uint16_t GreenLightTim;				//蓝
	uint16_t BlueLightTim;				//蓝
	
	uint16_t BlueLightStp;				//蓝
	uint16_t GreenLightStp;				//蓝
	uint16_t RedLightStp;				//蓝
	
	uint8_t RedLightDir;				//红
	uint8_t GreenLightDir;			//绿
	uint8_t BlueLightDir;				//蓝
	uint8_t RedLightDirold;				//红
	uint8_t GreenLightDirold;			//绿
	uint8_t BlueLightDirold;				//蓝
	uint8_t RedLightDirnew;				//红
	uint8_t GreenLightDirnew;			//绿
	uint8_t BlueLightDirnew;				//蓝
	uint16_t SignCnt;				//蓝
	uint16_t RedFlashTim;				//蓝
	uint16_t GreenFlashTim;				//蓝
	uint16_t BlueFlashTim;				//蓝
}__RGB_ctr;
extern __RGB_ctr RGB_ctr;	//管理LCD重要参数

//系统定时信息
typedef struct{
	uint8_t gNrfLnkGas;				//2.4g无线模块数据处理时间间隔定时变量
	uint8_t ICScanGastmp;			//刷卡去抖动时间
	uint8_t NETUpdateGas;			//网络状态信息更新间隔
	uint8_t RCdUpdateGas;			//消费记录上传频度
	uint16_t gLED_Gas;					//系统运行指示灯延时变量
	uint8_t gRc522RdGas;			//rc522读卡时间延时变量
	uint16_t gRc522HodGas;			//rc522读卡时间延时变量
	uint16_t BeepHodT;					//蜂鸣器响铃停顿时间变量
	uint8_t BeepStpT;					//蜂鸣器响铃按时变量
	uint16_t gPlusGas;					//上币信号脉冲定时变量
	uint16_t gPlusHold;					//上币信号脉冲定时变量
	uint8_t RC522FlashDly;		//rc522提示灯延时变量
	uint16_t LCDIdeT;			 		//lcd恢复待机界面延时变量
	uint8_t gLCDScanGas;			//lcd更新延时变量
	uint8_t gLEDScanGas;			//背光刷新延时变量
	uint16_t RED_FlashDly;		//rc522提示灯延时变量
	uint16_t ToolBarDly;
}__SysDlyT;
extern __SysDlyT Dly_Var;

//nrf24l01 控制器
typedef struct
{
	uint8_t TxFlg;								//nrf24l01发送标志
	uint8_t TxDatBufFlg;					//有数据待发送标志
	uint8_t ReTxcnt;							//重发计数器
	uint8_t ReRxcnt;							//接收计数器
	uint8_t DatLen;								//接收计数器
	uint16_t TxCntTim;						//接收计数器
}__nrf24l01_ctr;

extern __nrf24l01_ctr L01_Ctr;					//nrf24l01控制器

//rc522 控制器
typedef struct
{
	uint8_t CT[2];				//卡类型
	uint8_t RFID[16];			//存放RFID 
	uint8_t SN[4]; 				//卡号
	uint8_t KEYA[6];
	uint8_t KEYB[6];
	uint8_t GetIcNum;
	uint8_t finish_flg;
}__Rc522_ctr;
extern __Rc522_ctr RC522Ctr;						//rc522控制器

//系统全局变量
volatile typedef struct
{
	uint32_t ye;				//余额
	uint32_t xf;				//消费金额
//	uint32_t bi;				//消费金额
	uint8_t PlusNum; 		//脉冲数量
	uint8_t DisType; 	//显示模式
	uint8_t RGB_DisType;
	uint8_t UpDatCnt; 	//上传数据序号
	uint8_t BeepType; 	//蜂鸣器提示音类型
	uint16_t PlusCntT;//脉冲计数器时钟
	uint16_t PlusCntHT;//脉冲高计数器时钟
	uint16_t PlusCntLT;//脉冲计数器时钟
	uint16_t PlusCntStep;//脉冲计数器时钟
	uint32_t PlusCntSum;//脉冲计数器时钟
	
	
// 	uint16_t PlusCntTCoin;//脉冲计数器时钟
// 	uint16_t PlusCntHTCoin;//脉冲高计数器时钟
// 	uint16_t PlusCntLTCoin;//脉冲计数器时钟
// 	uint16_t PlusCntStepCoin;//脉冲计数器时钟
// 	uint32_t PlusCntSumCoin;//脉冲计数器时钟
	
	
	
	uint32_t PlusCntSumtmp;
	uint32_t PlusCntOverTim;
	uint32_t icsn;
	uint32_t icsn_old;
	uint32_t icsn_new;
	uint8_t CntFlg;
	uint8_t ICKeyA[6];
	uint8_t ICKeyB[6];
//	uint8_t In_ExtITFlg;//正在外部中断标志
//	uint8_t Saved_Flg;//彩票已经成功存入标志
	
}__Sys_Var;
extern __Sys_Var SysVar;

typedef struct
{
	uint16_t tcp_client_sta;						//nrf24l01发送标志z
	uint16_t netsta;									//有数据待发送标志
	uint8_t NetConnectRetryCnt;					//网络重连次数
	uint16_t NetChkGas;								//网络状态检查间隔
	uint16_t ReConnectGas;						//网络重连间隔
	uint16_t LinkGas;									//网络心跳间隔
	uint16_t LinkRetryCnt;						//网络心跳重发计数器
	uint8_t  StpSta;						//网络心跳重发计数器
	uint8_t csp;
	uint8_t flg;
//	uint8_t txsta;
//	uint8_t rxsta;
}__net_ctr;
extern __net_ctr gNetCtr;	//管理LCD重要参数

//系统标志
typedef struct{
	uint16_t KeyVal;				//上币脉冲数量
	uint16_t KeySta;				//上币脉冲数量
	uint8_t CoinOtstp;
}__SysFlg;
extern __SysFlg gFlg;	


void showdbg8dec(uint8_t *bf,uint8_t len);
void showdbg8hex(uint8_t *bf,uint8_t len);
void showdbg32dec(uint32_t *bf,uint8_t len);
void showdbg32hex(uint32_t *bf,uint8_t len);
void showdbg16hex(uint16_t *bf,uint8_t len);
void ExplainParm(void);
void WriteParm(void);
void PrintCfgPar(void);
void WriteDefaultSet(void);
uint8_t ChkBufSum(uint8_t *buf,uint8_t len);
uint16_t app_get_rand(uint16_t max,uint16_t min,uint16_t rand);
#endif



