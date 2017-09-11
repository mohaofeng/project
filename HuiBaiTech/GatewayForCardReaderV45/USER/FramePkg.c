#define FRAME_PKG_C
#include "FramePkg.h"
#include "usart.h"	 
#include "common.h"


#define FRAME_PKG_DBG  0
extern QUEUE 	gCmpDwDxBuf;											//上位机下传命令缓冲队列
extern QUEUE 	gDxUpCmpBuf;										//卡头上传命令缓冲队列

extern __ConfigVar Config;//系统配置参数

#if TEST_DBG	

/***********************************************
Function: 服务器回复卡头上币
************************************************/
void AckDXImfor(uint8_t dxid,uint8_t wxid,uint8_t *sn)
{
	EnQueue(&gCmpDwDxBuf,0x90);
	EnQueue(&gCmpDwDxBuf,dxid);	
	EnQueue(&gCmpDwDxBuf,wxid);	
	EnQueue(&gCmpDwDxBuf,CMP_DWN_COIN_OUT);	//卡头上币
	EnQueue(&gCmpDwDxBuf,WX_CMD);								//预留字
	EnQueue(&gCmpDwDxBuf,sn[0]);								//ic卡号
	EnQueue(&gCmpDwDxBuf,sn[1]);		
	EnQueue(&gCmpDwDxBuf,sn[2]);		
	EnQueue(&gCmpDwDxBuf,sn[3]);		
	
	EnQueue(&gCmpDwDxBuf,1);	//投币次数
	
	EnQueue(&gCmpDwDxBuf,0);	//用户余额
	EnQueue(&gCmpDwDxBuf,100);	
	EnQueue(&gCmpDwDxBuf,100);	
	
	EnQueue(&gCmpDwDxBuf,0);	//本次消费金额
	EnQueue(&gCmpDwDxBuf,0);	
	EnQueue(&gCmpDwDxBuf,100);
	
	EnQueue(&gCmpDwDxBuf,1);//处理结果
	
	EnQueue(&gCmpDwDxBuf,0xfe);
	EnQueue(&gCmpDwDxBuf,0xfc);
	EnQueue(&gCmpDwDxBuf,0xfe);
	EnQueue(&gCmpDwDxBuf,0xfc);
#if FRAME_PKG_DBG
	printf("回复卡头扣费命令\r\n");
#endif
}
#endif

/***********************************************
Function: 回复配置帧
************************************************/
void FrameUpWxConfig(void)
{
//	uint8_t i;
	EnQueue(&gDxUpCmpBuf,0x90);
	EnQueue(&gDxUpCmpBuf,(u8)(Config.WxID>>8));	//子读头个数
	EnQueue(&gDxUpCmpBuf,(u8)(Config.WxID));	//网关本机ID
	EnQueue(&gDxUpCmpBuf,WxUP_CMP_CONFIG_PAR);	//配置网关命令	
	EnQueue(&gDxUpCmpBuf,DX_CMD);	//配置网关命令	
	EnQueue(&gDxUpCmpBuf,Config.Localip[3]);	//网关本机ID
	EnQueue(&gDxUpCmpBuf,Config.Localip[2]);	//网关本机ID
	EnQueue(&gDxUpCmpBuf,Config.Localip[1]);	//网关本机ID
	EnQueue(&gDxUpCmpBuf,Config.Localip[0]);	//网关本机ID
	
	EnQueue(&gDxUpCmpBuf,Config.Rmoteip[3]);	//服务器ip
	EnQueue(&gDxUpCmpBuf,Config.Rmoteip[2]);	//服务器ip
	EnQueue(&gDxUpCmpBuf,Config.Rmoteip[1]);	//服务器ip
	EnQueue(&gDxUpCmpBuf,Config.Rmoteip[0]);	//服务器ip
	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.Port>>8));	//端口号
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.Port>>0));	//端口号
	
	EnQueue(&gDxUpCmpBuf,Config.GateW[3]);	//默认网关
	EnQueue(&gDxUpCmpBuf,Config.GateW[2]);	//默认网关
	EnQueue(&gDxUpCmpBuf,Config.GateW[1]);	//默认网关
	EnQueue(&gDxUpCmpBuf,Config.GateW[0]);	//默认网关

	EnQueue(&gDxUpCmpBuf,Config.Mass[3]);	//子网掩码
	EnQueue(&gDxUpCmpBuf,Config.Mass[2]);	//子网掩码
	EnQueue(&gDxUpCmpBuf,Config.Mass[1]);	//子网掩码
	EnQueue(&gDxUpCmpBuf,Config.Mass[0]);	//子网掩码
	EnQueue(&gDxUpCmpBuf,Config.WxFQ);		//通信频道
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
	#if FRAME_PKG_DBG
	printf("回复配置网关id帧\r\n");
	#endif
}
/***********************************************
Function: 发送心跳帧
************************************************/
void FrameUpWxLink(void)
{
	EnQueue(&gDxUpCmpBuf,0x90);
	EnQueue(&gDxUpCmpBuf,(u8)(Config.WxID>>8));	//子读头个数
	EnQueue(&gDxUpCmpBuf,(u8)(Config.WxID));	//网关本机ID
	EnQueue(&gDxUpCmpBuf,WxUP_CMP_LINK);	//配置网关命令	
	EnQueue(&gDxUpCmpBuf,DX_CMD);	//配置网关命令	
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
	#if FRAME_PKG_DBG
	printf("发送心跳帧\r\n");
	#endif
}
/***********************************************
Function: 网关回复服务器测试指令
************************************************/
void FrameUpCMPTEST(void)
{
	EnQueue(&gDxUpCmpBuf,0x90);
	EnQueue(&gDxUpCmpBuf,(u8)(Config.WxID>>8));	//子读头个数
	EnQueue(&gDxUpCmpBuf,(u8)(Config.WxID));	//网关本机ID
	EnQueue(&gDxUpCmpBuf,WxUP_CMP_MAC);		//回复测试	
	EnQueue(&gDxUpCmpBuf,DX_CMD);		//回复测试	
	EnQueue(&gDxUpCmpBuf,Config.Mac[0]);	//配置网关命令	
	EnQueue(&gDxUpCmpBuf,Config.Mac[1]);	//配置网关命令	
	EnQueue(&gDxUpCmpBuf,Config.Mac[2]);	//配置网关命令	
	EnQueue(&gDxUpCmpBuf,Config.Mac[3]);	//配置网关命令	
	EnQueue(&gDxUpCmpBuf,Config.Mac[4]);	//配置网关命令	
	EnQueue(&gDxUpCmpBuf,Config.Mac[5]);	//配置网关命令	
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
	#if FRAME_PKG_DBG
	printf("回复服务器测试网关命令\r\n");
	#endif
}

// /***********************************************
// Function: 上传读头状态信息
// ************************************************/
// void FrameUpDxSta(void)
// {
// 	uint8_t i;
// 	EnQueue(&gDxUpCmpBuf,0x90);
// 	EnQueue(&gDxUpCmpBuf,Config.DxNum);	//子读头个数
// 	EnQueue(&gDxUpCmpBuf,Config.WxID);	//网关本机ID
// 	EnQueue(&gDxUpCmpBuf,WxACK_CMP_Dx_STA);	//配置网关命令
// 	if(Config.DxNum>DX_LIST_MAXLEN)Config.DxNum = DX_LIST_MAXLEN;
// 	for(i=0;i<Config.DxNum;i++)
// 		EnQueue(&gDxUpCmpBuf,Config.DxLst[i]);	//子读头列表
// 	EnQueue(&gDxUpCmpBuf,0xfe);
// 	EnQueue(&gDxUpCmpBuf,0xfc);
// 	EnQueue(&gDxUpCmpBuf,0xfe);
// 	EnQueue(&gDxUpCmpBuf,0xfc);
// 	#if FRAME_PKG_DBG
// 	printf("上传卡头状态帧\r\n");
// 	#endif
// }












static u32 cmd_state=0;  //队列帧尾检测状态
static qsize cmd_pos=0;  //当前指令指针位置

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
		if(cmd_pos==0&&_data!=CMD_HEAD)//指令第一个字节必须是帧头，否则跳过
		{
#if FRAME_PKG_DBG	
				printf("无效帧!队列中有  ---%d---- byte数据　\r\n",cmd_size);				
#endif
				//cmd_size = 0;
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





