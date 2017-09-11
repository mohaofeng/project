#include "tcp_demo.h"
#include "sys.h"
#include "uip.h"
#include <string.h>
#include <stdio.h>	  
#include "24l01.h"
#include "queue.h"
#include "FramePkg.h"
#include "common.h"

extern QUEUE 	gCmpDwDxBuf;											//上位机下传命令缓冲队列
extern QUEUE 	gDxUpCmpBuf;										//卡头上传命令缓冲队列
extern __ConfigVar Config;								//系统配置参数
extern __SysFlg gFlg;	

u8 tcp_client_sta;											//客户端状态

//[7]:0,无连接;1,已经连接;
//[6]:0,无数据;1,收到客户端数据
//[5]:0,无数据;1,有数据需要发送

//这是一个TCP 客户端应用回调函数。
//该函数通过UIP_APPCALL(tcp_demo_appcall)调用,实现Web Client的功能.
//当uip事件发生时，UIP_APPCALL函数会被调用,根据所属端口(1400),确定是否执行该函数。
//例如 : 当一个TCP连接被创建时、有新的数据到达、数据已经被应答、数据需要重发等事件
void tcp_client_demo_appcall(void)
{		 
// 	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
//	uint8_t datlen;

	if(uip_aborted())
		tcp_client_aborted();				//连接终止	   
	if(uip_timedout())
		tcp_client_timedout();			//连接超时   
	if(uip_closed())
		tcp_client_closed();				//连接关闭	   
  if(uip_connected())
		tcp_client_connected();			//连接成功	    
	if(uip_acked())
		tcp_client_acked();					//发送的数据成功送达 
	//--------------tcp接收数据-------------------*/
	if (uip_newdata())	
	{			
		if(uip_len > 0)
		{
			EnQueuex(&gCmpDwDxBuf,uip_appdata,uip_len);		//接收到的数据入队，待转发至卡头
// #if TCP_DBG 
// 			printf("服务器----->网关 uip_len=%d>>",uip_len); 
// 			showdbg8hex(uip_appdata,uip_len);		
// #endif
		}
	}
	if(uip_rexmit()||uip_newdata()||uip_acked()||uip_connected()||uip_poll())//当需要重发、新数据到达、数据包送达、连接建立时，通知uip发送数据 
	{
		tcp_client_senddata();
	}											   
}
extern void LanInit(void);

//这里我们假定Server端的IP地址为:192.168.1.103
//这个IP必须根据Server端的IP修改.
//尝试重新连接
void tcp_client_reconnect()
{
	uip_ipaddr_t ipaddr;
	
	uip_init();				//uIP初始化	  
 	uip_ipaddr(ipaddr, Config.Localip[3],Config.Localip[2],Config.Localip[1],Config.Localip[0]);	//根据转发器编号设置本地设置IP地址
	uip_sethostaddr(ipaddr);					    
	uip_ipaddr(ipaddr, Config.GateW[3],Config.GateW[2],Config.GateW[1],Config.GateW[0]); 	//设置网关IP地址(其实就是你路由器的IP地址)
	uip_setdraddr(ipaddr);						 
	uip_ipaddr(ipaddr, Config.Mass[3],Config.Mass[2],Config.Mass[1],Config.Mass[0]);	//设置网络掩码
	uip_setnetmask(ipaddr);


	uip_ipaddr(&ipaddr,Config.Rmoteip[3],Config.Rmoteip[2],Config.Rmoteip[1],Config.Rmoteip[0]);	//设置IP为192.168.1.103
	uip_connect(&ipaddr,htons(Config.Port)); 	//端口为8888
	uip_log("重连服务器\r\n");//打印log
}
//终止连接				    
void tcp_client_aborted(void)
{
	tcp_client_sta&=(~BIT_SET7);	//标志没有连接
//	tcp_client_reconnect();		//尝试重新连接
	gFlg.netstaflg = 0;
	ClearQ(&gCmpDwDxBuf);
	ClearQ(&gDxUpCmpBuf);
	uip_log("tcp_client aborted!\r\n");//打印log
}
//连接超时
void tcp_client_timedout(void)
{
	tcp_client_sta&=(~BIT_SET7);	//标志没有连接	   
	gFlg.netstaflg = 0;
	uip_log("tcp_client timeout!\r\n");//打印log
}
//连接关闭
void tcp_client_closed(void)
{
	tcp_client_sta&=(~BIT_SET7);	//标志没有连接
	ClearQ(&gCmpDwDxBuf);
	ClearQ(&gDxUpCmpBuf);
	gFlg.netstaflg = 0;
	uip_log("tcp_client closed!\r\n");//打印log
	
}	 
//连接建立
void tcp_client_connected(void)
{
	struct tcp_demo_appstate *s=(struct tcp_demo_appstate *)&uip_conn->appstate;
 	tcp_client_sta|=BIT_SET7;		//标志连接成功
  uip_log("tcp_client connected!\r\n");//打印log
  s->state=STATE_CMD; 		//指令状态
}
//发送的数据成功送达
void tcp_client_acked(void)
{											    
	struct tcp_demo_appstate *s=(struct tcp_demo_appstate *)&uip_conn->appstate;
	s->textlen=0;//发送清零
	uip_log("tcp_client acked!\r\n");//表示成功发送		 
	//FrameAckDxSta();//报告id信息给服务器
}
//发送数据给服务端
void tcp_client_senddata(void)
{
	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
	if(s->textlen>0)
	{
		uip_send(s->textptr, s->textlen);//发送TCP数据包	 
	}
}

//打印日志用
void uip_log(char *m)
{			    
	printf("uIP log:%s\r\n",m);
}















