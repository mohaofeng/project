/**
************************************************************************************************
* @file   		tcp_demo.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief   		TCP 演示函数
* @attention  
************************************************************************************************
**/

#include <stdio.h>
#include <string.h>
#include "tcp_demo.h"
#include "W5500_conf.h"
#include "w5500.h"
#include "socket.h"
#include "SysConfig.h"
#include "SysConfig.h"
#include "queue.h"
#include <CmpDatprocess.h>
#include "FramePkg.h"




extern uint8_t gNetUpCmpCmdBuf[NET_CMD_MAXLEN];							//用于暂存上位机命令的缓冲区

static uint8 buff[2048];



#define TCP_WAIT_TO_SEND   	0x0001	/* tcp有数据等待发送 				*/
#define TCP_CLOSED			0x0002			/* tcp链接已经关闭   				*/
#define TCP_INIT			0x0004				/* tcp正在初始化     				*/
#define TCP_CONNECTTING		0x0008    /* tcp正在建立链接   				*/
#define TCP_CONNECTED		0x0010			/* tcp已经链接上     				*/
#define TCP_CLOSEING		0x0020			/* tcp等待关闭       				*/
#define TCP_RECV_DATA		0x0040			/* tcp接收到数据     				*/



void ExpNetSta(void)
{
	
	gNetCtr.tcp_sta_old = gNetCtr.tcp_sta_new;
	gNetCtr.tcp_sta_new = gNetCtr.netsta;
	
	if(gNetCtr.tcp_sta_old != gNetCtr.tcp_sta_new)
	{
		if(TCP_WAIT_TO_SEND & gNetCtr.tcp_sta_new)
			printf("有数据待发送\r\n");
		if(TCP_CLOSED & gNetCtr.tcp_sta_new)
			printf("tcp连接关闭\r\n");
		if(TCP_INIT & gNetCtr.tcp_sta_new)
			printf("tcp初始化\r\n");
		if(TCP_CONNECTTING & gNetCtr.tcp_sta_new)
			printf("tcp正在连接\r\n");
		if(TCP_CONNECTED & gNetCtr.tcp_sta_new)
			printf("tcp已经链接\r\n");
		if(TCP_CLOSEING & gNetCtr.tcp_sta_new)
			printf("tcp等待关闭\r\n");
		if(TCP_RECV_DATA & gNetCtr.tcp_sta_new)
			printf("tcp接收到数据\r\n");
	}
}
/**
*@brief		TCP Client回环演示函数。
*@param		无
*@return	无
*/
void do_tcp_client(void)
{	
   uint16 len=0;	
   uint8_t mod=1;

	switch(getSn_SR(SOCK_TCPC))								  				         /*获取socket的状态*/
	{
		case SOCK_CLOSED:											        		         /*socket处于关闭状态*/
			socket(SOCK_TCPC,Sn_MR_TCP,Config.local_port++,Sn_MR_ND);
			gNetCtr.netsta |= TCP_CLOSED;
			gNetCtr.netsta &= (~TCP_INIT);
			gNetCtr.netsta &= (~TCP_CONNECTED);
		  break;
		
		case SOCK_INIT:													        	         /*socket处于初始化状态*/
			connect(SOCK_TCPC,Config.Rmoteip,Config.RemotePort);                /*socket连接服务器*/ 
			gNetCtr.netsta |= TCP_INIT;
			gNetCtr.netsta &= (~TCP_CLOSED);
			gNetCtr.netsta &= (~TCP_CLOSEING);
			gNetCtr.netsta &= (~TCP_CONNECTED);

			
			if(gNetCtr.ReConnectGas==0)
			{
				lcd_parm.distype = DIS_INIT_MOD;
				gNetCtr.ReConnectGas = CONNECT_Gas_CONST;						
				if(gNetCtr.NetConnectRetryCnt == 0)
				{
					SetFcolor(0xffff);
					SetBcolor(0x00ff);
					LCDShowSysParm();					
				//tcp_client_reconnect();
				}
				else if(gNetCtr.NetConnectRetryCnt < RECONNECT_CNT_MAX)
				{
					
					SetScreen(SHOW_SYSPARM_SCREEN_ID);
					switch(SysVar.connect_sta)
					{
						case 0:
							DisText(300, 450,mod,SYS_TMP_FONT,"服务器连接中.				          ");
							SysVar.connect_sta = 1;
						break;
						case 1:
							SysVar.connect_sta = 2;
							DisText(300, 450,mod,SYS_TMP_FONT,"服务器连接中..				        ");
						break;
						case 2:
							SysVar.connect_sta = 0;
						DisText(300, 450,mod,SYS_TMP_FONT,"服务器连接中...				          ");
						break;
					}
				}
				else
				{
						DisText(300, 450,mod,SYS_TMP_FONT,"无法连接到服务器，请检查网络");							
				}
				if(gNetCtr.NetConnectRetryCnt++>RECONNECT_CNT_MAX*2)
				{
					gNetCtr.NetConnectRetryCnt = 0;
				}
			}
		  break;
		
		case SOCK_ESTABLISHED: 												             /*socket处于连接建立状态*/
			gNetCtr.netsta |= TCP_CONNECTED;
			gNetCtr.netsta &= (~TCP_INIT);
			gNetCtr.netsta &= (~TCP_CLOSED);
			gNetCtr.netsta &= (~TCP_CLOSEING);
		
			if(getSn_IR(SOCK_TCPC) & Sn_IR_CON)
			{
				setSn_IR(SOCK_TCPC, Sn_IR_CON);								          /*清除接收中断标志位*/
			}
			len=getSn_RX_RSR(SOCK_TCPC);									            /*定义len为已接收数据的长度*/
			if(len>0)
			{
				recv(SOCK_TCPC,buff,len);								              	/*接收来自Client的数据*/
				EnQueuex(&gNetDwDxQueu,buff,len);   										//接收到的数据入队，待转发至卡头
				gNetCtr.netsta |= TCP_RECV_DATA;		  	
			}
			if(gNetCtr.netsta & TCP_WAIT_TO_SEND)
			{
				gNetCtr.netsta &= (~TCP_WAIT_TO_SEND); 
			  send(SOCK_TCPC,gNetUpCmpCmdBuf,gNetCtr.RxFrameBufLen);											/*向Client发送数据*/
			}
		  break;
			
		case SOCK_CLOSE_WAIT: 											    	         /*socket处于等待关闭状态*/
			close(SOCK_TCPC);
			gNetCtr.netsta |= TCP_CLOSEING;
			gNetCtr.netsta &= (~TCP_INIT);
			gNetCtr.netsta &= (~TCP_CLOSED);
			gNetCtr.netsta &= (~TCP_CONNECTED);
			break;
	}
 // ExpNetSta();	
}


void NetPrintf(uint8_t *fmt, ...)
{
	va_list ap;
	
	va_start(ap,fmt);
	vsprintf((char*)TmpVar.srttmp,fmt,ap); 
	va_end(ap);
	ESP8266_SendString ( ENABLE, TmpVar.srttmp, sizeof(TmpVar.srttmp), Single_ID_0 );
	//send(SOCK_TCPC,(uint8_t*)TmpVar.srttmp,sizeof(TmpVar.srttmp));
	//FrameString(TmpVar.srttmp);  
	//gNetCtr.netsta |= TCP_WAIT_TO_SEND;
}



