#define FRAME_PKG_C
#include "FramePkg.h"
#include "SysConfig.h"
//#include "usart.h"	 
#include "SysConfig.h"


#define FRAME_PKG_DBG  0

#if TEST_DBG	
void CmpAckCheckBalence(uint16_t x,uint16_t y,uint16_t fsize,uint16_t Fcolor,uint16_t dtim,uint16_t Bcolor,uint8_t soundtype,uint8_t coincnt,char *pname)
{
	uint8_t i=0;
	EnQueue(&gNetDwDxQueu,0x90);
	EnQueue(&gNetDwDxQueu,Sn_ACK_CHECK_BALENCE);	
	EnQueue(&gNetDwDxQueu,(uint8_t)(Config.deviceid>>24));
	EnQueue(&gNetDwDxQueu,(uint8_t)(Config.deviceid>>16));
	EnQueue(&gNetDwDxQueu,(uint8_t)(Config.deviceid>>8));
	EnQueue(&gNetDwDxQueu,(uint8_t)(Config.deviceid>>0));
	EnQueue(&gNetDwDxQueu,(uint8_t)(x>>8));
	EnQueue(&gNetDwDxQueu,(uint8_t)(x>>0));
	EnQueue(&gNetDwDxQueu,(uint8_t)(y>>8));
	EnQueue(&gNetDwDxQueu,(uint8_t)(y>>0));
	EnQueue(&gNetDwDxQueu,(uint8_t)(Fcolor>>8));
	EnQueue(&gNetDwDxQueu,(uint8_t)(Fcolor>>0));
	EnQueue(&gNetDwDxQueu,(uint8_t)(Bcolor>>8));
	EnQueue(&gNetDwDxQueu,(uint8_t)(Bcolor>>0));
	EnQueue(&gNetDwDxQueu,(uint8_t)(fsize>>0));
	EnQueue(&gNetDwDxQueu,(uint8_t)(coincnt>>0));
	EnQueue(&gNetDwDxQueu,(uint8_t)(soundtype>>0));
	EnQueue(&gNetDwDxQueu,(uint8_t)(dtim>>0));
	EnQueue(&gNetDwDxQueu,(uint8_t)(0>>0));
	while(pname[i] != 0)
	{
		EnQueue(&gNetDwDxQueu,pname[i]);
		i++;
	}
	EnQueue(&gNetDwDxQueu,0);
	EnQueue(&gNetDwDxQueu,0xfe);
	EnQueue(&gNetDwDxQueu,0xfc);
	EnQueue(&gNetDwDxQueu,0xfe);
	EnQueue(&gNetDwDxQueu,0xfc);
#if FRAME_PKG_DBG
	printf("回复卡头扣费命令\r\n");
#endif
}

void CmpTestPrint(char *pname)
{
	uint8_t i;
	EnQueue(&gNetDwDxQueu,0x90);
	EnQueue(&gNetDwDxQueu,Sn_TEST_PRINTER);	
	EnQueue(&gNetDwDxQueu,(uint8_t)(Config.deviceid>>24));
	EnQueue(&gNetDwDxQueu,(uint8_t)(Config.deviceid>>16));
	EnQueue(&gNetDwDxQueu,(uint8_t)(Config.deviceid>>8));
	EnQueue(&gNetDwDxQueu,(uint8_t)(Config.deviceid>>0));
	while(pname[i] != 0)
	{
		EnQueue(&gNetDwDxQueu,pname[i]);
		i++;
	}
	EnQueue(&gNetDwDxQueu,0);
	EnQueue(&gNetDwDxQueu,0xfe);
	EnQueue(&gNetDwDxQueu,0xfc);
	EnQueue(&gNetDwDxQueu,0xfe);
	EnQueue(&gNetDwDxQueu,0xfc);
#if FRAME_PKG_DBG
	printf("服务器测试打印机命令\r\n");
#endif
}

#endif



/***********************************************
Function: 帧头
************************************************/
static void CMDHead(void)
{
	EnQueue(&gNetUpCmpQueu,0x90);
}
/***********************************************
Function: 帧尾
************************************************/
static void CMDTail(void)
{
	EnQueue(&gNetUpCmpQueu,0xfe);
	EnQueue(&gNetUpCmpQueu,0xfc);
	EnQueue(&gNetUpCmpQueu,0xfe);
	EnQueue(&gNetUpCmpQueu,0xfc);
}




/***********************************************
Function: 回复测试指令
************************************************/
void FrameRequTest(void)
{
	CMDHead();
	EnQueue(&gNetUpCmpQueu,XnREPOT_TEST_CMD);
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>24));//登陆密码
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>16));//登陆密码
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>8));//登陆密码
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>0));//登陆密码
	CMDTail();
	#if FRAME_PKG_DBG
	printf("回复测试指令\r\n");
	#endif
}



/*
 *Function: 申请扣费   彩屏
 *type:1/2/3  购币/取币/查询
 */
void RequestDecMon(uint8_t cmdid,uint8_t *vipstring,uint32_t deccnt) 
{
	uint32_t i=0;
	CMDHead();
	EnQueue(&gNetUpCmpQueu,RequestCMD_MULTFUNC);
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>24));
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>16));
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>8));
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>0));
	
	EnQueue(&gNetUpCmpQueu,cmdid);
	EnQueue(&gNetUpCmpQueu,(uint8_t)(deccnt>>24));//登陆密码
	EnQueue(&gNetUpCmpQueu,(uint8_t)(deccnt>>16));//登陆密码
	EnQueue(&gNetUpCmpQueu,(uint8_t)(deccnt>>8));//登陆密码
	EnQueue(&gNetUpCmpQueu,(uint8_t)(deccnt>>0));//登陆密码

	while(vipstring[i] != 0)
	{
		EnQueue(&gNetUpCmpQueu,vipstring[i]);
		i++;
	}	
	EnQueue(&gNetUpCmpQueu,0);
	CMDTail();
	#if FRAME_PKG_DBG
	printf("申请扣费-彩屏\r\n");
	#endif
}
/***********************************************
Function: 申请登录
************************************************/
void FrameLogin(void)
{
	CMDHead();
	EnQueue(&gNetUpCmpQueu,LOGIN_CMD_MULFUN_XFJ);

	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>24));//设备id
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>16));//设备id
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>8));//设备id
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>0));//设备id
	
	EnQueue(&gNetUpCmpQueu,Config.hwver);//硬件版本
	EnQueue(&gNetUpCmpQueu,Config.swver);//软件版本
	EnQueue(&gNetUpCmpQueu,Config.runmod);//软件版本
	CMDTail();
	#if FRAME_PKG_DBG
	printf("申请登录\r\n");
	#endif
}

/***********************************************
Function: 链路帧
************************************************/
void FrameLink(void)
{
	CMDHead();
	EnQueue(&gNetUpCmpQueu,XnLINK_CMD);
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>24));//设备id
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>16));//设备id
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>8));//设备id
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>0));//设备id
	CMDTail();
	#if FRAME_PKG_DBG
	printf("发送链路帧\r\n");
	#endif
}

/***********************************************
Function: 回复配置帧
************************************************/
void FrameAckConfig(void)
{
	CMDHead();
	EnQueue(&gNetUpCmpQueu,XnREPOT_InFOR_CMD);
	
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>24));	//本机id
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>16));	//本机id
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>8));	//本机id
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>0));	//本机id
	
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.Rmoteip[3]));	//服务器ip
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.Rmoteip[2]));	//服务器ip
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.Rmoteip[1]));	//服务器ip
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.Rmoteip[0]));	//服务器ip
	
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.GateW[3]));	//网关
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.GateW[2]));	//网关
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.GateW[1]));	//网关
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.GateW[0]));	//网关
	
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.Mass[3]));	//子网掩码
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.Mass[2]));	//子网掩码
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.Mass[1]));	//子网掩码
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.Mass[0]));	//子网掩码
	
	EnQueue(&gNetUpCmpQueu,Config.Localip[3]);	//本机ip
	EnQueue(&gNetUpCmpQueu,Config.Localip[2]);	//本机ip
	EnQueue(&gNetUpCmpQueu,Config.Localip[1]);	//本机ip
	EnQueue(&gNetUpCmpQueu,Config.Localip[0]);	//本机ip
	
	EnQueue(&gNetUpCmpQueu,(u8)(Config.RemotePort>>8));	//服务器端口号
	EnQueue(&gNetUpCmpQueu,(u8)(Config.RemotePort>>0));	//服务器端口号
	
// 	EnQueue(&gNetUpCmpQueu,(u8)(Config.DecMon>>16));	//消费额度
// 	EnQueue(&gNetUpCmpQueu,(u8)(Config.DecMon>>8));	
// 	EnQueue(&gNetUpCmpQueu,(u8)(Config.DecMon>>0));	
	
	EnQueue(&gNetUpCmpQueu,(u8)(Config.NetMod>>0));	
	
	EnQueue(&gNetUpCmpQueu,(u8)(Config.PlusWith>>0));	//脉冲宽度
	
	EnQueue(&gNetUpCmpQueu,(u8)(Config.PromptTim>>8));	
	EnQueue(&gNetUpCmpQueu,(u8)(Config.PromptTim>>0));	
	
	EnQueue(&gNetUpCmpQueu,(u8)(Config.runmod>>0));	//运行方式
	
	CMDTail();
	#if FRAME_PKG_DBG
	printf("回复系统配置帧\r\n");
	#endif

}


/***********************************************
Function: 申请余额查询
************************************************/
void FrameApply(u32 sn,uint32_t id)
{
	CMDHead();
	EnQueue(&gNetUpCmpQueu,XnCHK_MON_CMD);
	EnQueue(&gNetUpCmpQueu,(uint8_t)(sn>>24));//??
	EnQueue(&gNetUpCmpQueu,(uint8_t)(sn>>16));//??
	EnQueue(&gNetUpCmpQueu,(uint8_t)(sn>>8));//??
	EnQueue(&gNetUpCmpQueu,(uint8_t)(sn>>0));//??
	EnQueue(&gNetUpCmpQueu,(uint8_t)(id>>24));	//??id
	EnQueue(&gNetUpCmpQueu,(uint8_t)(id>>16));	//??id
	EnQueue(&gNetUpCmpQueu,(uint8_t)(id>>8));	//??id
	EnQueue(&gNetUpCmpQueu,(uint8_t)(id>>0));	//??id	
	CMDTail();
	#if FRAME_PKG_DBG
	printf("??????\r\n");
	#endif
}
/***********************************************
Function: 申请扣费
************************************************/
void FrameDecMon(u32 sn,u32 xf,uint32_t id)
{
	CMDHead();
	EnQueue(&gNetUpCmpQueu,XnDEC_MON_CMD);
	EnQueue(&gNetUpCmpQueu,(uint8_t)(sn>>24));//卡号
	EnQueue(&gNetUpCmpQueu,(uint8_t)(sn>>16));//卡号
	EnQueue(&gNetUpCmpQueu,(uint8_t)(sn>>8));//卡号
	EnQueue(&gNetUpCmpQueu,(uint8_t)(sn>>0));//卡号
	
	EnQueue(&gNetUpCmpQueu,(xf>>16));//消费卡金
	EnQueue(&gNetUpCmpQueu,(xf>>8));
	EnQueue(&gNetUpCmpQueu,(xf>>0));
	EnQueue(&gNetUpCmpQueu,(uint8_t)(id>>24));	//本机id
	EnQueue(&gNetUpCmpQueu,(uint8_t)(id>>16));	//本机id
	EnQueue(&gNetUpCmpQueu,(uint8_t)(id>>8));	//本机id
	EnQueue(&gNetUpCmpQueu,(uint8_t)(id>>0));	//本机id	
	CMDTail();
	#if FRAME_PKG_DBG
	printf("申请扣费\r\n");
	#endif
}


/***********************************************
Function: 申请扣费 扫描头
************************************************/
void FrameDecMonEXT(u8 imforlen,u8 *imfor,u32 xf,uint32_t id)
{
	uint8_t i=0;
	CMDHead();
	EnQueue(&gNetUpCmpQueu,XnEXT_CMD);	
	EnQueue(&gNetUpCmpQueu,(uint8_t)(id>>24));	//本机id
	EnQueue(&gNetUpCmpQueu,(uint8_t)(id>>16));	//本机id
	EnQueue(&gNetUpCmpQueu,(uint8_t)(id>>8));	//本机id
	EnQueue(&gNetUpCmpQueu,(uint8_t)(id>>0));	//本机id	
	EnQueue(&gNetUpCmpQueu,(xf>>16));//消费卡金
	EnQueue(&gNetUpCmpQueu,(xf>>8));
	EnQueue(&gNetUpCmpQueu,(xf>>0));
	EnQueue(&gNetUpCmpQueu,(imforlen>>0));
	
	for(i=0;i<imforlen;i++)//
		EnQueue(&gNetUpCmpQueu,imfor[i]);
	
	CMDTail();
	#if FRAME_PKG_DBG
	printf("申请扣费\r\n");
	#endif
}
/***********************************************
Function: 发送格式化字符串
************************************************/
void FrameString(char *imfor) 
{
	uint16_t i=0;
//	CMDHead();	
	while(imfor[i] != 0)
	{
		EnQueue(&gNetUpCmpQueu,imfor[i]);
		i++;
	}	
	EnQueue(&gNetUpCmpQueu,0);	
//	CMDTail();
}


/***********************************************
Function: 获取服务器时间
************************************************/
void FrameResTim(void)
{
	CMDHead();
	EnQueue(&gNetUpCmpQueu,XnREQ_TIM_CMD);
	CMDTail();
	#if FRAME_PKG_DBG
	printf("获取服务器时间\r\n");
	#endif
}

/***********************************************
Function: 读头上传用户刷卡信息到服务器
************************************************/
void FrameUpDxCoinSum(uint32_t sn,uint32_t cnt)
{
	EnQueue(&gNetUpCmpQueu,0x90);
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>24));	//??id
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>16));	//??id
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>8));	//??id
	EnQueue(&gNetUpCmpQueu,(uint8_t)(Config.deviceid>>0));	//??id	
	EnQueue(&gNetUpCmpQueu,XnREPOT_COIN_CMP);	//????????
	EnQueue(&gNetUpCmpQueu,(uint8_t)(sn>>24));//??
	EnQueue(&gNetUpCmpQueu,(uint8_t)(sn>>16));//??
	EnQueue(&gNetUpCmpQueu,(uint8_t)(sn>>8));//??
	EnQueue(&gNetUpCmpQueu,(uint8_t)(sn>>0));//??
	EnQueue(&gNetUpCmpQueu,(uint8_t)(cnt>>24));	
	EnQueue(&gNetUpCmpQueu,(uint8_t)(cnt>>16));	
	EnQueue(&gNetUpCmpQueu,(uint8_t)(cnt>>8));	
	EnQueue(&gNetUpCmpQueu,(uint8_t)(cnt>>0));	
	
	EnQueue(&gNetUpCmpQueu,0xfe);
	EnQueue(&gNetUpCmpQueu,0xfc);
	EnQueue(&gNetUpCmpQueu,0xfe);
	EnQueue(&gNetUpCmpQueu,0xfc);
#if FRAME_PKG_DBG
	printf("????????\r\n");
#endif
}





static u32 Cmpcmd_state=0;  //队列帧尾检测状态
static qsize Cmpcmd_pos=0;  //当前指令指针位置

///***********************************************
//Function: 帧解析上位机下传数据
//head：帧头
//帧尾
//************************************************/
qsize queue_find_cmdForCmp(PQUEUE Q,elementType *buffer,qsize buf_len,uint8_t head,uint32_t tail)
{
	qsize cmd_size = 0;
	elementType _data = 0;
	
//	cmd_size = ;
	while(QueueDatLen(Q)>0)
	{
		DeQueue(Q,&_data);
		//showdbg8hex(&_data,1);
		//printf("0x%x,",_data);
		if(Cmpcmd_pos==0&&_data!= head)//指令第一个字节必须是帧头，否则跳过
		{
#if FRAME_PKG_DBG	
				printf("无效帧!队列中有  ---%d---- byte数据　\r\n",cmd_size);				
#endif
		    return 0;
		}
		if(Cmpcmd_pos<buf_len)//防止缓冲区溢出
		{
			buffer[Cmpcmd_pos++] = _data;
		}
		else
		{
			Cmpcmd_pos = 0;
//			cmd_size = 0;
			Cmpcmd_state = 0;
			ClearQ(Q);
			printf("帧溢出");
			return 0;
		}
		Cmpcmd_state = ((Cmpcmd_state<<8)|_data);//拼接最后4个字节，组成一个32位整数
		if(Cmpcmd_state == tail)//最后4个字节与帧尾匹配，得到完整帧
		{
			cmd_size = Cmpcmd_pos; //指令字节长度
			Cmpcmd_state = 0;  //重新检测帧尾
			Cmpcmd_pos = 0; //复位指令指针
			return cmd_size;
		}
	}
	return 0;//没有形成完整的一帧
}
static u32 Upcmd_state=0;  //队列帧尾检测状态
static qsize Upcmd_pos=0;  //当前指令指针位置

///***********************************************
//Function: 帧解析上位机下传数据
//head：帧头
//帧尾
//************************************************/
qsize queue_find_cmdForUp(PQUEUE Q,elementType *buffer,qsize buf_len,uint8_t head,uint32_t tail)
{
	qsize cmd_size = 0;
	elementType _data = 0;
	
//	cmd_size = ;
	while(QueueDatLen(Q)>0)
	{
		DeQueue(Q,&_data);
		//showdbg8hex(&_data,1);
		//printf("0x%x,",_data);
		if(Upcmd_pos==0&&_data!= head)//指令第一个字节必须是帧头，否则跳过
		{
#if FRAME_PKG_DBG	
				printf("无效帧!队列中有  ---%d---- byte数据　\r\n",cmd_size);				
#endif
		    return 0;
		}
		if(Upcmd_pos<buf_len)//防止缓冲区溢出
		{
			buffer[Upcmd_pos++] = _data;
		}
		else
		{
			Upcmd_pos = 0;
//			cmd_size = 0;
			Upcmd_state = 0;
			ClearQ(Q);
			printf("帧溢出");
			return 0;
		}
		Upcmd_state = ((Upcmd_state<<8)|_data);//拼接最后4个字节，组成一个32位整数
		if(Upcmd_state == tail)//最后4个字节与帧尾匹配，得到完整帧
		{
			cmd_size = Upcmd_pos; //指令字节长度
			Upcmd_state = 0;  //重新检测帧尾
			Upcmd_pos = 0; //复位指令指针
			return cmd_size;
		}
	}
	return 0;//没有形成完整的一帧
}




static u32 Codecmd_state=0;  //队列帧尾检测状态
static qsize Codecmd_pos=0;  //当前指令指针位置

///***********************************************
//Function: 帧解析
//head：帧头
//帧尾
//************************************************/
qsize queue_find_cmdForCode(PQUEUE Q,elementType *buffer,qsize buf_len,uint8_t head,uint32_t tail)
{
	qsize cmd_size = 0;
	elementType _data = 0;
	
//	cmd_size = ;
	while(QueueDatLen(Q)>0)
	{
		DeQueue(Q,&_data);
		//showdbg8hex(&_data,1);
		//printf("0x%x,",_data);
		if(Codecmd_pos==0&&_data!= head)//指令第一个字节必须是帧头，否则跳过
		{
#if FRAME_PKG_DBG	
				printf("无效帧!队列中有  ---%d---- byte数据　\r\n",cmd_size);				
#endif
		    return 0;
		}
		if(Codecmd_pos<buf_len)//防止缓冲区溢出
		{
			buffer[Codecmd_pos++] = _data;
		}
		else
		{
			Codecmd_pos = 0;
//			cmd_size = 0;
			Codecmd_state = 0;
			ClearQ(Q);
			printf("帧溢出");
			return 0;
		}
		Codecmd_state = ((Codecmd_state<<8)|_data);//拼接最后4个字节，组成一个32位整数
		if(Codecmd_state == tail)//最后4个字节与帧尾匹配，得到完整帧
		{
			cmd_size = Codecmd_pos; //指令字节长度
			Codecmd_state = 0;  //重新检测帧尾
			Codecmd_pos = 0; //复位指令指针
			return cmd_size;
		}
	}
	return 0;//没有形成完整的一帧
}

static u32 TFTcmd_state=0;  //队列帧尾检测状态
static qsize TFTcmd_pos=0;  //当前指令指针位置

///***********************************************
//Function: 帧解析
//head：帧头
//帧尾
//************************************************/
qsize queue_find_cmdForTFT(PQUEUE Q,elementType *buffer,qsize buf_len,uint8_t head,uint32_t tail)
{
	qsize cmd_size = 0;
	elementType _data = 0;
	
//	cmd_size = ;
	while(QueueDatLen(Q)>0)
	{
		DeQueue(Q,&_data);
		//showdbg8hex(&_data,1);
		//printf("0x%x,",_data);
		if(TFTcmd_pos==0&&_data!= head)//指令第一个字节必须是帧头，否则跳过
		{
#if FRAME_PKG_DBG	
				printf("无效帧!队列中有  ---%d---- byte数据　\r\n",cmd_size);				
#endif
		    return 0;
		}
		if(TFTcmd_pos<buf_len)//防止缓冲区溢出
		{
			buffer[TFTcmd_pos++] = _data;
		}
		else
		{
			TFTcmd_pos = 0;
//			cmd_size = 0;
			TFTcmd_state = 0;
			ClearQ(Q);
			printf("帧溢出");
			return 0;
		}
		TFTcmd_state = ((TFTcmd_state<<8)|_data);//拼接最后4个字节，组成一个32位整数
		if(TFTcmd_state == tail)//最后4个字节与帧尾匹配，得到完整帧
		{
			cmd_size = TFTcmd_pos; //指令字节长度
			TFTcmd_state = 0;  //重新检测帧尾
			TFTcmd_pos = 0; //复位指令指针
			return cmd_size;
		}
	}
	return 0;//没有形成完整的一帧
}
