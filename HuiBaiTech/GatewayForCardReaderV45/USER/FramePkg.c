#define FRAME_PKG_C
#include "FramePkg.h"
#include "usart.h"	 
#include "common.h"


#define FRAME_PKG_DBG  0
extern QUEUE 	gCmpDwDxBuf;											//��λ���´���������
extern QUEUE 	gDxUpCmpBuf;										//��ͷ�ϴ���������

extern __ConfigVar Config;//ϵͳ���ò���

#if TEST_DBG	

/***********************************************
Function: �������ظ���ͷ�ϱ�
************************************************/
void AckDXImfor(uint8_t dxid,uint8_t wxid,uint8_t *sn)
{
	EnQueue(&gCmpDwDxBuf,0x90);
	EnQueue(&gCmpDwDxBuf,dxid);	
	EnQueue(&gCmpDwDxBuf,wxid);	
	EnQueue(&gCmpDwDxBuf,CMP_DWN_COIN_OUT);	//��ͷ�ϱ�
	EnQueue(&gCmpDwDxBuf,WX_CMD);								//Ԥ����
	EnQueue(&gCmpDwDxBuf,sn[0]);								//ic����
	EnQueue(&gCmpDwDxBuf,sn[1]);		
	EnQueue(&gCmpDwDxBuf,sn[2]);		
	EnQueue(&gCmpDwDxBuf,sn[3]);		
	
	EnQueue(&gCmpDwDxBuf,1);	//Ͷ�Ҵ���
	
	EnQueue(&gCmpDwDxBuf,0);	//�û����
	EnQueue(&gCmpDwDxBuf,100);	
	EnQueue(&gCmpDwDxBuf,100);	
	
	EnQueue(&gCmpDwDxBuf,0);	//�������ѽ��
	EnQueue(&gCmpDwDxBuf,0);	
	EnQueue(&gCmpDwDxBuf,100);
	
	EnQueue(&gCmpDwDxBuf,1);//������
	
	EnQueue(&gCmpDwDxBuf,0xfe);
	EnQueue(&gCmpDwDxBuf,0xfc);
	EnQueue(&gCmpDwDxBuf,0xfe);
	EnQueue(&gCmpDwDxBuf,0xfc);
#if FRAME_PKG_DBG
	printf("�ظ���ͷ�۷�����\r\n");
#endif
}
#endif

/***********************************************
Function: �ظ�����֡
************************************************/
void FrameUpWxConfig(void)
{
//	uint8_t i;
	EnQueue(&gDxUpCmpBuf,0x90);
	EnQueue(&gDxUpCmpBuf,(u8)(Config.WxID>>8));	//�Ӷ�ͷ����
	EnQueue(&gDxUpCmpBuf,(u8)(Config.WxID));	//���ر���ID
	EnQueue(&gDxUpCmpBuf,WxUP_CMP_CONFIG_PAR);	//������������	
	EnQueue(&gDxUpCmpBuf,DX_CMD);	//������������	
	EnQueue(&gDxUpCmpBuf,Config.Localip[3]);	//���ر���ID
	EnQueue(&gDxUpCmpBuf,Config.Localip[2]);	//���ر���ID
	EnQueue(&gDxUpCmpBuf,Config.Localip[1]);	//���ر���ID
	EnQueue(&gDxUpCmpBuf,Config.Localip[0]);	//���ر���ID
	
	EnQueue(&gDxUpCmpBuf,Config.Rmoteip[3]);	//������ip
	EnQueue(&gDxUpCmpBuf,Config.Rmoteip[2]);	//������ip
	EnQueue(&gDxUpCmpBuf,Config.Rmoteip[1]);	//������ip
	EnQueue(&gDxUpCmpBuf,Config.Rmoteip[0]);	//������ip
	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.Port>>8));	//�˿ں�
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.Port>>0));	//�˿ں�
	
	EnQueue(&gDxUpCmpBuf,Config.GateW[3]);	//Ĭ������
	EnQueue(&gDxUpCmpBuf,Config.GateW[2]);	//Ĭ������
	EnQueue(&gDxUpCmpBuf,Config.GateW[1]);	//Ĭ������
	EnQueue(&gDxUpCmpBuf,Config.GateW[0]);	//Ĭ������

	EnQueue(&gDxUpCmpBuf,Config.Mass[3]);	//��������
	EnQueue(&gDxUpCmpBuf,Config.Mass[2]);	//��������
	EnQueue(&gDxUpCmpBuf,Config.Mass[1]);	//��������
	EnQueue(&gDxUpCmpBuf,Config.Mass[0]);	//��������
	EnQueue(&gDxUpCmpBuf,Config.WxFQ);		//ͨ��Ƶ��
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
	#if FRAME_PKG_DBG
	printf("�ظ���������id֡\r\n");
	#endif
}
/***********************************************
Function: ��������֡
************************************************/
void FrameUpWxLink(void)
{
	EnQueue(&gDxUpCmpBuf,0x90);
	EnQueue(&gDxUpCmpBuf,(u8)(Config.WxID>>8));	//�Ӷ�ͷ����
	EnQueue(&gDxUpCmpBuf,(u8)(Config.WxID));	//���ر���ID
	EnQueue(&gDxUpCmpBuf,WxUP_CMP_LINK);	//������������	
	EnQueue(&gDxUpCmpBuf,DX_CMD);	//������������	
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
	#if FRAME_PKG_DBG
	printf("��������֡\r\n");
	#endif
}
/***********************************************
Function: ���ػظ�����������ָ��
************************************************/
void FrameUpCMPTEST(void)
{
	EnQueue(&gDxUpCmpBuf,0x90);
	EnQueue(&gDxUpCmpBuf,(u8)(Config.WxID>>8));	//�Ӷ�ͷ����
	EnQueue(&gDxUpCmpBuf,(u8)(Config.WxID));	//���ر���ID
	EnQueue(&gDxUpCmpBuf,WxUP_CMP_MAC);		//�ظ�����	
	EnQueue(&gDxUpCmpBuf,DX_CMD);		//�ظ�����	
	EnQueue(&gDxUpCmpBuf,Config.Mac[0]);	//������������	
	EnQueue(&gDxUpCmpBuf,Config.Mac[1]);	//������������	
	EnQueue(&gDxUpCmpBuf,Config.Mac[2]);	//������������	
	EnQueue(&gDxUpCmpBuf,Config.Mac[3]);	//������������	
	EnQueue(&gDxUpCmpBuf,Config.Mac[4]);	//������������	
	EnQueue(&gDxUpCmpBuf,Config.Mac[5]);	//������������	
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
	#if FRAME_PKG_DBG
	printf("�ظ�������������������\r\n");
	#endif
}

// /***********************************************
// Function: �ϴ���ͷ״̬��Ϣ
// ************************************************/
// void FrameUpDxSta(void)
// {
// 	uint8_t i;
// 	EnQueue(&gDxUpCmpBuf,0x90);
// 	EnQueue(&gDxUpCmpBuf,Config.DxNum);	//�Ӷ�ͷ����
// 	EnQueue(&gDxUpCmpBuf,Config.WxID);	//���ر���ID
// 	EnQueue(&gDxUpCmpBuf,WxACK_CMP_Dx_STA);	//������������
// 	if(Config.DxNum>DX_LIST_MAXLEN)Config.DxNum = DX_LIST_MAXLEN;
// 	for(i=0;i<Config.DxNum;i++)
// 		EnQueue(&gDxUpCmpBuf,Config.DxLst[i]);	//�Ӷ�ͷ�б�
// 	EnQueue(&gDxUpCmpBuf,0xfe);
// 	EnQueue(&gDxUpCmpBuf,0xfc);
// 	EnQueue(&gDxUpCmpBuf,0xfe);
// 	EnQueue(&gDxUpCmpBuf,0xfc);
// 	#if FRAME_PKG_DBG
// 	printf("�ϴ���ͷ״̬֡\r\n");
// 	#endif
// }












static u32 cmd_state=0;  //����֡β���״̬
static qsize cmd_pos=0;  //��ǰָ��ָ��λ��

///***********************************************
//Function: ֡����
//************************************************/
qsize queue_find_cmd(PQUEUE Q,elementType *buffer,qsize buf_len)
{
	qsize cmd_size = 0;
	elementType _data = 0;
	
	cmd_size = QueueDatLen(Q);
	while(cmd_size>0)
	{
		DeQueue(Q,&_data);
		if(cmd_pos==0&&_data!=CMD_HEAD)//ָ���һ���ֽڱ�����֡ͷ����������
		{
#if FRAME_PKG_DBG	
				printf("��Ч֡!��������  ---%d---- byte���ݡ�\r\n",cmd_size);				
#endif
				//cmd_size = 0;
		    return 0;
		}
		if(cmd_pos<buf_len)//��ֹ���������
		{
			buffer[cmd_pos++] = _data;
		}
		else
		{
			cmd_pos = 0;
			cmd_size = 0;
			cmd_state = 0;
			ClearQ(Q);
			printf("֡���");
			return 0;
		}
		cmd_state = ((cmd_state<<8)|_data);//ƴ�����4���ֽڣ����һ��32λ����
		if(cmd_state == CMD_TAIL)//���4���ֽ���֡βƥ�䣬�õ�����֡
		{
			cmd_size = cmd_pos; //ָ���ֽڳ���
			cmd_state = 0;  //���¼��֡β��
			cmd_pos = 0; //��λָ��ָ��
			return cmd_size;
		}
	}
	return 0;//û���γ�������һ֡
}





