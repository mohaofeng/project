#ifndef __FRAME_PKG_H
#define __FRAME_PKG_H	 		  
#include "sys.h"   

#ifdef FRAME_PKG_C	
#define FRAME_PKG_EXT 
#else
#define FRAME_PKG_EXT extern
#endif
#include "queue.h"
#include "common.h"


#define CMD_HEAD 0x90//帧头
#define CMD_TAIL 0XFeFCFeFc //帧尾
#define CMD_TAIL_Nbyte 1 //帧尾
#define CMD_POS 1							//命令字位置

/***********************************************
简述：协议命令字定义    Sn -> Dn
************************************************/
#define CMP_DWN_COIN_OUT   		 0xff	//服务器回复上币命令
#define CMP_CONFIG_DX_ID   		 0xfe	//服务器配置读头
#define CMP_CONFIG_COIN_PAR    0xfd	//服务器配置读头上币参数

/***********************************************
简述：协议命令字定义    Sn -> Wn
************************************************/
#define CMP_CONFIG_WX_ID   		 0xfc	//服务器配置网关id
#define CMP_CONFIG_WX_IP   		 0xfb	//服务器配置网关iP
#define CMP_RESET_WX   		 		 0xf9	//服务器软件复位网关
#define CMP_ACK_Link_WX   		 0x9f	//服务器回复心跳指令
#define CMP_TEST_WX   		 		 0x9e	//服务器测试网关
#define CMP_CONFIG_MAC_WX   	 0x9d	//服务器配置mac
#define CMP_CONFIG_FQ_WX   		 0xe9	//服务器配置网关通信频道

#define CMP_DWN_COIN_OUT   		 0xff	//服务器回复上币命令
#define CMP_CONFIG_DX_ID   		 0xfe	//服务器配置读头ID号
#define CMP_CONFIG_COIN_PAR    0xfd	//服务器配置读头上币参数
#define CMP_CONFIG_DIS_PAR     0xed	//服务器配置读头金额提示时间
#define CMP_CONFIG_DIR_PAR     0xee	//服务器配置读头显示方向
#define CMP_CONFIG_TEST    		 0xec	//服务器测试指令
#define CMP_COIN_OT_TEST    	 0xeb	//服务器测试上币指令
#define CMP_RX_OK_TEST    		 0xea	//服务器回复测试通信指令
#define CMP_CONFIG_FQ_DX    	 0xe8	//服务器回复测试通信指令
#define CMP_CONFIG_PRICE    	 0xe7	//服务器回复测试通信指令
#define CMP_CONFIG_PAR    		 0xe5	//服务器回复测试通信指令

/***********************************************
简述：协议命令字定义    Dn -> Sn
************************************************/
#define DX_UP_IC_IMFOR_CMP   		 0x02	//读头上传用户信息
#define DX_UP_CONFIRM_OK_CMP   	 0x04	//读头回复服务器确认扣款
#define DX_UP_CONFIG_CMP     		 0x08	//读头回复本机参数

/***********************************************
简述：协议命令字定义    Wn -> Sn
************************************************/
#define WxUP_CMP_CONFIG_PAR     0xfa	//回复服务器配置ID命令
#define WxUP_CMP_LINK   			 	0xa0	//回复服务器读头状态
#define WxUP_CMP_MAC   		 			0x9e	//回复服务器读头状态

void AckDXImfor(uint8_t dxid,uint8_t wxid,uint8_t *sn);
//void FrameUpDxSta(void);
void FrameUpWxConfig(void);
void FrameUpWxLink(void);
void FrameUpCMPTEST(void);
qsize queue_find_cmd(PQUEUE Q,elementType *buffer,qsize buf_len);


#endif






