#define FRAME_PKG_C
#include "FramePkg.h"
#include "usart.h"	 
#include "common.h"


#define FRAME_PKG_DBG 0
extern QUEUE 	gCmpDwDxBuf;										//上位机下传命令缓冲队列
extern QUEUE 	gDxUpCmpBuf;										//卡头上传命令缓冲队列

/***********************************************
Function: 回复服务器测试上币成功
************************************************/
void AckOutCoinSta(uint8_t *sn,uint8_t sta)
{
	EnQueue(&gDxUpCmpBuf,0x90);
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>8));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>0));	
	EnQueue(&gDxUpCmpBuf,DX_UP_CONFIRM_OK_CMP);	//卡头上币
	EnQueue(&gDxUpCmpBuf,DX_CMD);								//预留字
	EnQueue(&gDxUpCmpBuf,sn[0]);								//ic卡号
	EnQueue(&gDxUpCmpBuf,sn[1]);		
	EnQueue(&gDxUpCmpBuf,sn[2]);		
	EnQueue(&gDxUpCmpBuf,sn[3]);		
	EnQueue(&gDxUpCmpBuf,sta);			
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
#if FRAME_PKG_DBG
	printf("回复服务器上币状态\r\n");
#endif
}
/***********************************************
Function: 回复测试上币成功
************************************************/
void TESTOutCoinOK(uint8_t FrmSn)
{
	EnQueue(&gDxUpCmpBuf,0x90);
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>8));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>0));	
	EnQueue(&gDxUpCmpBuf,DX_ACK_OUT_COINOK_CMP);	//卡头上币
	EnQueue(&gDxUpCmpBuf,DX_CMD);								//预留字
	EnQueue(&gDxUpCmpBuf,FrmSn);								//预留字
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
#if FRAME_PKG_DBG
	printf("回复服务器上币状态\r\n");
#endif
}
/***********************************************
Function: 回复服务器上币成功
************************************************/
void AckTest(void)
{
	EnQueue(&gDxUpCmpBuf,0x90);
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>8));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>0));	
	EnQueue(&gDxUpCmpBuf,DX_UP_ACK_TEST_CMP);	//
	EnQueue(&gDxUpCmpBuf,DX_CMD);	//
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
#if FRAME_PKG_DBG
	printf("回复服务器上币状态\r\n");
#endif
}
/***********************************************
Function: 读头上传本机参数
************************************************/
void FrameUpDxConfig(void)
{
//	uint8_t i;
	EnQueue(&gDxUpCmpBuf,0x90);
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>8));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>0));	
	EnQueue(&gDxUpCmpBuf,DX_UP_CONFIG_CMP);	//配置网关命令
	EnQueue(&gDxUpCmpBuf,DX_CMD);	//配置网关命令
	EnQueue(&gDxUpCmpBuf,Config.PromptTim/10);	
	EnQueue(&gDxUpCmpBuf,Config.Price);	
	EnQueue(&gDxUpCmpBuf,Config.CoinOutLevel);	
	EnQueue(&gDxUpCmpBuf,Config.CoinSigHod);	
	EnQueue(&gDxUpCmpBuf,Config.CoinSigGas);	
	EnQueue(&gDxUpCmpBuf,Config.DxFQ);	
	EnQueue(&gDxUpCmpBuf,Config.DxSwVer);	
	EnQueue(&gDxUpCmpBuf,Config.DxHwVer);	
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
#if FRAME_PKG_DBG
	printf("读头回复配置参数\r\n");
#endif
}


/***********************************************
Function: 读头上传用户刷卡信息到服务器
************************************************/
void FrameUpDxCoinSum(uint32_t sn,uint32_t cnt,uint8_t fsn)
{
	EnQueue(&gDxUpCmpBuf,0x90);
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>8));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>0));	
	EnQueue(&gDxUpCmpBuf,DX_UP_IC_COIN_CMP);	//读头上传用户信息
	EnQueue(&gDxUpCmpBuf,DX_CMD);	//
	EnQueue(&gDxUpCmpBuf,(uint8_t)(sn>>24));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(sn>>16));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(sn>>8));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(sn>>0));	
	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(cnt>>24));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(cnt>>16));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(cnt>>8));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(cnt>>0));	
	EnQueue(&gDxUpCmpBuf,fsn);
	
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
#if FRAME_PKG_DBG
	printf("读头上传用户彩票\r\n");
#endif
}
/***********************************************
Function: 读头上传用户刷卡信息到服务器
************************************************/
void FrameUpDxCoinCnt(uint32_t cnt,uint8_t fsn)
{
	EnQueue(&gDxUpCmpBuf,0x90);
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>8));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>0));	
	EnQueue(&gDxUpCmpBuf,DX_UP_COIN_CNT_CMP);	
	EnQueue(&gDxUpCmpBuf,DX_CMD);	//
	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(cnt>>24));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(cnt>>16));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(cnt>>8));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(cnt>>0));	
	EnQueue(&gDxUpCmpBuf,fsn);
	
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
#if FRAME_PKG_DBG
	printf("机台投币总数量\r\n");
#endif
}

/***********************************************
Function: 读头上传用户刷卡信息到服务器
************************************************/
void FrameUpDxICImfor(uint32_t sn,uint8_t cnt)
{
	EnQueue(&gDxUpCmpBuf,0x90);
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>8));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>0));	
	EnQueue(&gDxUpCmpBuf,DX_UP_IC_IMFOR_CMP);	//读头上传用户信息
	EnQueue(&gDxUpCmpBuf,DX_CMD);	//
	EnQueue(&gDxUpCmpBuf,(uint8_t)(sn>>24));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(sn>>16));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(sn>>8));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(sn>>0));	
	EnQueue(&gDxUpCmpBuf,cnt);	
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
#if FRAME_PKG_DBG
	printf("读头上传用户刷卡信息到服务器\r\n");
#endif
}





static u32 cmd_state;  //队列帧尾检测状态
static qsize cmd_pos;  //当前指令指针位置

///***********************************************
//Function: 帧解析
//************************************************/
qsize queue_find_cmd(PQUEUE Q,elementType *buffer,qsize buf_len)
{
	qsize cmd_size = 0;
	elementType _data = 0;
	
	cmd_size = QueueDatLen(Q);
	while(cmd_size>0)
	{
		DeQueue(Q,&_data);
		if((cmd_pos==0)&&(_data!=CMD_HEAD))//指令第一个字节必须是帧头，否则跳过
		{
#if FRAME_PKG_DBG	
				printf("无效帧!队列中有  ---%d---- byte数据　\r\n",cmd_size);				
#endif
		    return 0;
		}
		if(cmd_pos<buf_len)//防止缓冲区溢出
		{
			buffer[cmd_pos++] = _data;
		}
		else
		{
			cmd_pos = 0;
			cmd_size = 0;
			cmd_state = 0;
			ClearQ(Q);
			printf("帧溢出");
			return 0;
		}
		cmd_state = ((cmd_state<<8)|_data);//拼接最后4个字节，组成一个32位整数
		if(cmd_state == CMD_TAIL)//最后4个字节与帧尾匹配，得到完整帧
		{
			cmd_size = cmd_pos; //指令字节长度
			cmd_state = 0;  //重新检测帧尾巴
			cmd_pos = 0; //复位指令指针
			return cmd_size;
		}
	}
	return 0;//没有形成完整的一帧
}





