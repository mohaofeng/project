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


/*------------------调试宏定义------------------------*/
#define MAIN_DBG	0
#define TEST_DBG	0
#define NetDatExp_DBG	0
#define Nrf24l01TaskDBG 	0    	//nrf24l01调试宏



/*---------------------------常数定义----------------------*/
#define FLASH_DAT_LEN 50  //配置数据区长度
#define DX_LIST_MAXLEN 10  //子卡头最大数量
#define DEFAULT_TAG_FLG      0x1
//#define DX_STU_UP_GAS   500//读头状态信息上传间隔 100x10ms
#define LED_GAS_CONST   50//读头状态信息上传间隔 100x10ms
#define CONNECT_OVER_CONST 200   //300*10ms
#define QLEN_MAX 400				 //队列缓冲区最大长度
#define NET_CMD_MAXLEN  40   //上位机命令最大长度
#define L01_CMD_MAXLEN  32   //上位机命令最大长度

#define DEFAULT_DxFQ_NUM 	100 					//默认频道
#define DEFAULT_WxFQ_NUM 	100 					//默认频道

#define ReTxcntMAX      	10				//当前指令最大重发次数
#define ReGASMAX      		1					//无数据发送时接收数据时间间隔
#define TxCntMAX					6+4         //无线数据发送间隔

//宏定义
#define NRF_LNK_CONST 		10  	//NRF24L01链接间隔时间  x*10ms
#define TX_DAT_GAS				2
#define RX_DAT_GAS				5
#define	LinkGasTCONST    	300    //心跳间隔
#define LinkMAXCONST     	3     //心跳最大重发次数
#define WX_CMD 						0x01
#define DX_CMD 						0x02

#define DEFAULT_WX_ID    2

//系统配置信息
typedef struct
{
	uint16_t tagflg;											//标志
	uint16_t WxID;													//网关ID
	uint8_t DxNum;												//子读头数量
	uint8_t DxLst[DX_LIST_MAXLEN];				//子读头数量
	uint8_t Rmoteip[4];										//服务器ip
	uint8_t Localip[4];										//本机ip
	uint8_t GateW[4];											//默认网关
	uint8_t Mass[4];											//子网掩码
	uint16_t Port;												//端口号
	uint8_t Mac[6];												//端口号
	uint8_t DxFQ;													//内置频道
	uint8_t WxFQ;													//内置频道

}__ConfigVar;
extern __ConfigVar Config;//系统配置参数

//nrf24l01 控制器
typedef struct
{
	uint8_t TxFlg;						//nrf24l01发送标志
	uint8_t DatTxFlg;					//有数据待发送标志
	uint8_t ReTxcnt;					//重发计数器
	uint8_t ReRxcnt;					//接收计数器
	uint8_t TxCnt;
}__nrf24l01_ctr;
extern __nrf24l01_ctr L01_Ctr;

//系统定时信息
typedef struct{
	uint32_t tst_gas;					//测试用
	uint16_t LinkGasT;					//测试用
	uint16_t wdog_gas;					//测试用
	
}__SysDlyT;
extern __SysDlyT DlyVar;

//系统标志
typedef struct{
	uint8_t NetTxOkFlg;				//网络数据成功送达
	uint8_t NetTickCnt;				//网络数据成功送达
	uint8_t	netstaflg;
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
uint32_t app_get_rand(uint32_t max,uint32_t min);
#endif



