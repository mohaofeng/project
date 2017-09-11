#ifndef __FRAME_PKG_H
#define __FRAME_PKG_H	 		  

#ifdef FRAME_PKG_C	
#define FRAME_PKG_EXT 
#else
#define FRAME_PKG_EXT extern
#endif
#include "queue.h"
#include "SysConfig.h"






#define CMD_TAIL_Nbyte 1 					//帧尾
#define CMD_POS 1							//命令字位置


/*
*
*多功能消费机指令号
*/
#define LOGIN_CMD_MULFUN_XFJ   		 0x37					//申请登录指令


/***********************************************
简述：协议命令字定义    Xn -> Sn
************************************************/
   
#define XnLOGIN_CMD   		 0x00					//申请登录指令
#define XnLINK_CMD    		 0x01					//链路，心跳指令
#define XnCHK_MON_CMD    	 0x02					//申请余额查询指令
#define XnDEC_MON_CMD    	 0x03					//申请扣费
#define XnDEC_MON_CMD    	 0x03					//申请扣费
#define XnEXT_CMD   			 0x04					//上传扫描头信息
#define XnREQ_TIM_CMD      0x05					//获取服务器时间
#define XnREPOT_InFOR_CMD  0x06					//上传设备信息
#define XnREPOT_TEST_CMD   0x07					//回复服务器测试指令
#define XnREPOT_COIN_CMP   0x08					//上传彩票数量到服务器
#define XnRequestCMD   		 0x09					//申请扣费指令，彩屏用
#define RequestCMD_MULTFUNC   		 0x36					//申请扣费指令，彩屏用
/***********************************************
简述：协议命令字定义    Sn -> Xn
************************************************/
#define SnRE_TIM_CMD   		 			0xff		//服务器返回时间
#define SnRE_AMOUNT_CMD    		 	0xfe		//返回余额
#define SnRE_DEC_MON_CMD    	 	0xfd		//返回扣费
#define SnRE_REFUND_MON_CMD    	0xfc		//返回退款
#define Sn_REQXnIMFo_CMD   			0xfb		//获取设备信息
#define Sn_REQXnSTA_CMD      		0xfa		//获取设备运行状态
#define Sn_CONFIG_IP_CMD  			0xf9		//服务器配置消费机id号
#define Sn_Login_CMD  					0x33		//回复登陆成功
#define Sn_SETDECMON_CMD  			0xf7		//服务器配置消费额度
#define Sn_CONFIG_ID_CMD  			0xf5		//服务器配置消费机id号
#define Sn_LINK_CMD  						0xf4		//回复信条指令
#define Sn_SET_RUMMOD_CMD  			0xf3		//服务器配置消费机运行模式
#define Sn_CONFIG_PLUS_CMD  		0xf2		//服务器配置脉冲宽度
#define Sn_SET_AP_NAME_PASSWORD 0xf1		//服务器配置热点名称和密码
#define Sn_SET_RemotIP 					0xf0		//服务器配置远端ip地址
#define Sn_SET_Netmod 					0xef		//服务器配置联网方式
#define Sn_PromptTIM 						0xee		//金额提示停留时间
#define Sn_TEST 								0xed		//服务器测试指令
#define Sn_OUT_TEST 						0xec		//服务器测试上分指令
#define Sn_Config_DHCP					0xeb		//服务器配置dhcp开关
#define Sn_ACK_COIN							0xea		//服务器回复存票命令
#define Sn_UPDATE_GUI						0x31		//服务器回复余额查询指令
//#define Sn_ACK_DEC_OK						0x32		//服务器回复扣费成功指令
#define Sn_TEST_PRINTER					0x32		//服务器回复扣费成功指令


void FrameLogin(void);
void FrameLink(void);


void FrameAckConfig(void);
void AckDXImfor(uint8_t dxid,uint8_t wxid,uint8_t *sn);
void FrameUpWxConfig(void);
qsize queue_find_cmdForCmp(PQUEUE Q,elementType *buffer,qsize buf_len,uint8_t head,uint32_t tail);
qsize queue_find_cmdForCode(PQUEUE Q,elementType *buffer,qsize buf_len,uint8_t head,uint32_t tail);
qsize queue_find_cmdForTFT(PQUEUE Q,elementType *buffer,qsize buf_len,uint8_t head,uint32_t tail);
qsize queue_find_cmdForUp(PQUEUE Q,elementType *buffer,qsize buf_len,uint8_t head,uint32_t tail);
void FrameDecMon(u32 sn,u32 xf,uint32_t id);
void FrameDecMonEXT(u8 imforlen,u8 *imfor,u32 xf,uint32_t id);
void FrameApply(u32 sn,uint32_t id);
void FrameRequTest(void);
void FrameUpDxCoinSum(uint32_t sn,uint32_t cnt);
void RequestDecMon(uint8_t cmdid,uint8_t *vipstring,uint32_t deccnt);
void CmpTestPrint(char *pname);
void FrameString(char *imfor);

#if TEST_DBG	
void CmpAckCheckBalence(uint16_t x,uint16_t y,uint16_t fsize,uint16_t Fcolor,uint16_t dtim,uint16_t Bcolor,uint8_t soundtype,uint8_t coincnt,char *pname);
//void CmpAckCheckBalence(uint8_t sta,uint32_t jifen,uint32_t caipiao,uint32_t daibi,uint32_t benjin,uint32_t zengjin,uint32_t sn,uint32_t snid,uint8_t *pname);
#endif



#endif






