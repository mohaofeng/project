#include "tcp_demo.h"
#include "sys.h"
#include "uip.h"
#include <string.h>
#include <stdio.h>	  
#include "24l01.h"
#include "queue.h"
#include "FramePkg.h"
#include "common.h"

extern QUEUE 	gCmpDwDxBuf;											//��λ���´���������
extern QUEUE 	gDxUpCmpBuf;										//��ͷ�ϴ���������
extern __ConfigVar Config;								//ϵͳ���ò���
extern __SysFlg gFlg;	

u8 tcp_client_sta;											//�ͻ���״̬

//[7]:0,������;1,�Ѿ�����;
//[6]:0,������;1,�յ��ͻ�������
//[5]:0,������;1,��������Ҫ����

//����һ��TCP �ͻ���Ӧ�ûص�������
//�ú���ͨ��UIP_APPCALL(tcp_demo_appcall)����,ʵ��Web Client�Ĺ���.
//��uip�¼�����ʱ��UIP_APPCALL�����ᱻ����,���������˿�(1400),ȷ���Ƿ�ִ�иú�����
//���� : ��һ��TCP���ӱ�����ʱ�����µ����ݵ�������Ѿ���Ӧ��������Ҫ�ط����¼�
void tcp_client_demo_appcall(void)
{		 
// 	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
//	uint8_t datlen;

	if(uip_aborted())
		tcp_client_aborted();				//������ֹ	   
	if(uip_timedout())
		tcp_client_timedout();			//���ӳ�ʱ   
	if(uip_closed())
		tcp_client_closed();				//���ӹر�	   
  if(uip_connected())
		tcp_client_connected();			//���ӳɹ�	    
	if(uip_acked())
		tcp_client_acked();					//���͵����ݳɹ��ʹ� 
	//--------------tcp��������-------------------*/
	if (uip_newdata())	
	{			
		if(uip_len > 0)
		{
			EnQueuex(&gCmpDwDxBuf,uip_appdata,uip_len);		//���յ���������ӣ���ת������ͷ
// #if TCP_DBG 
// 			printf("������----->���� uip_len=%d>>",uip_len); 
// 			showdbg8hex(uip_appdata,uip_len);		
// #endif
		}
	}
	if(uip_rexmit()||uip_newdata()||uip_acked()||uip_connected()||uip_poll())//����Ҫ�ط��������ݵ�����ݰ��ʹ���ӽ���ʱ��֪ͨuip�������� 
	{
		tcp_client_senddata();
	}											   
}
extern void LanInit(void);

//�������Ǽٶ�Server�˵�IP��ַΪ:192.168.1.103
//���IP�������Server�˵�IP�޸�.
//������������
void tcp_client_reconnect()
{
	uip_ipaddr_t ipaddr;
	
	uip_init();				//uIP��ʼ��	  
 	uip_ipaddr(ipaddr, Config.Localip[3],Config.Localip[2],Config.Localip[1],Config.Localip[0]);	//����ת����������ñ�������IP��ַ
	uip_sethostaddr(ipaddr);					    
	uip_ipaddr(ipaddr, Config.GateW[3],Config.GateW[2],Config.GateW[1],Config.GateW[0]); 	//��������IP��ַ(��ʵ������·������IP��ַ)
	uip_setdraddr(ipaddr);						 
	uip_ipaddr(ipaddr, Config.Mass[3],Config.Mass[2],Config.Mass[1],Config.Mass[0]);	//������������
	uip_setnetmask(ipaddr);


	uip_ipaddr(&ipaddr,Config.Rmoteip[3],Config.Rmoteip[2],Config.Rmoteip[1],Config.Rmoteip[0]);	//����IPΪ192.168.1.103
	uip_connect(&ipaddr,htons(Config.Port)); 	//�˿�Ϊ8888
	uip_log("����������\r\n");//��ӡlog
}
//��ֹ����				    
void tcp_client_aborted(void)
{
	tcp_client_sta&=(~BIT_SET7);	//��־û������
//	tcp_client_reconnect();		//������������
	gFlg.netstaflg = 0;
	ClearQ(&gCmpDwDxBuf);
	ClearQ(&gDxUpCmpBuf);
	uip_log("tcp_client aborted!\r\n");//��ӡlog
}
//���ӳ�ʱ
void tcp_client_timedout(void)
{
	tcp_client_sta&=(~BIT_SET7);	//��־û������	   
	gFlg.netstaflg = 0;
	uip_log("tcp_client timeout!\r\n");//��ӡlog
}
//���ӹر�
void tcp_client_closed(void)
{
	tcp_client_sta&=(~BIT_SET7);	//��־û������
	ClearQ(&gCmpDwDxBuf);
	ClearQ(&gDxUpCmpBuf);
	gFlg.netstaflg = 0;
	uip_log("tcp_client closed!\r\n");//��ӡlog
	
}	 
//���ӽ���
void tcp_client_connected(void)
{
	struct tcp_demo_appstate *s=(struct tcp_demo_appstate *)&uip_conn->appstate;
 	tcp_client_sta|=BIT_SET7;		//��־���ӳɹ�
  uip_log("tcp_client connected!\r\n");//��ӡlog
  s->state=STATE_CMD; 		//ָ��״̬
}
//���͵����ݳɹ��ʹ�
void tcp_client_acked(void)
{											    
	struct tcp_demo_appstate *s=(struct tcp_demo_appstate *)&uip_conn->appstate;
	s->textlen=0;//��������
	uip_log("tcp_client acked!\r\n");//��ʾ�ɹ�����		 
	//FrameAckDxSta();//����id��Ϣ��������
}
//�������ݸ������
void tcp_client_senddata(void)
{
	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
	if(s->textlen>0)
	{
		uip_send(s->textptr, s->textlen);//����TCP���ݰ�	 
	}
}

//��ӡ��־��
void uip_log(char *m)
{			    
	printf("uIP log:%s\r\n",m);
}















