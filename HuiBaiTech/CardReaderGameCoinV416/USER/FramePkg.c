#define FRAME_PKG_C
#include "FramePkg.h"
#include "usart.h"	 
#include "common.h"


#define FRAME_PKG_DBG 0
extern QUEUE 	gCmpDwDxBuf;										//��λ���´���������
extern QUEUE 	gDxUpCmpBuf;										//��ͷ�ϴ���������

/***********************************************
Function: �ظ������������ϱҳɹ�
************************************************/
void AckOutCoinSta(uint8_t *sn,uint8_t sta)
{
	EnQueue(&gDxUpCmpBuf,0x90);
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>8));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>0));	
	EnQueue(&gDxUpCmpBuf,DX_UP_CONFIRM_OK_CMP);	//��ͷ�ϱ�
	EnQueue(&gDxUpCmpBuf,DX_CMD);								//Ԥ����
	EnQueue(&gDxUpCmpBuf,sn[0]);								//ic����
	EnQueue(&gDxUpCmpBuf,sn[1]);		
	EnQueue(&gDxUpCmpBuf,sn[2]);		
	EnQueue(&gDxUpCmpBuf,sn[3]);		
	EnQueue(&gDxUpCmpBuf,sta);			
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
#if FRAME_PKG_DBG
	printf("�ظ��������ϱ�״̬\r\n");
#endif
}
/***********************************************
Function: �ظ������ϱҳɹ�
************************************************/
void TESTOutCoinOK(uint8_t FrmSn)
{
	EnQueue(&gDxUpCmpBuf,0x90);
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>8));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>0));	
	EnQueue(&gDxUpCmpBuf,DX_ACK_OUT_COINOK_CMP);	//��ͷ�ϱ�
	EnQueue(&gDxUpCmpBuf,DX_CMD);								//Ԥ����
	EnQueue(&gDxUpCmpBuf,FrmSn);								//Ԥ����
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
	EnQueue(&gDxUpCmpBuf,0xfe);
	EnQueue(&gDxUpCmpBuf,0xfc);
#if FRAME_PKG_DBG
	printf("�ظ��������ϱ�״̬\r\n");
#endif
}
/***********************************************
Function: �ظ��������ϱҳɹ�
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
	printf("�ظ��������ϱ�״̬\r\n");
#endif
}
/***********************************************
Function: ��ͷ�ϴ���������
************************************************/
void FrameUpDxConfig(void)
{
//	uint8_t i;
	EnQueue(&gDxUpCmpBuf,0x90);
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>8));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>0));	
	EnQueue(&gDxUpCmpBuf,DX_UP_CONFIG_CMP);	//������������
	EnQueue(&gDxUpCmpBuf,DX_CMD);	//������������
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
	printf("��ͷ�ظ����ò���\r\n");
#endif
}


/***********************************************
Function: ��ͷ�ϴ��û�ˢ����Ϣ��������
************************************************/
void FrameUpDxCoinSum(uint32_t sn,uint32_t cnt,uint8_t fsn)
{
	EnQueue(&gDxUpCmpBuf,0x90);
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>8));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>0));	
	EnQueue(&gDxUpCmpBuf,DX_UP_IC_COIN_CMP);	//��ͷ�ϴ��û���Ϣ
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
	printf("��ͷ�ϴ��û���Ʊ\r\n");
#endif
}
/***********************************************
Function: ��ͷ�ϴ��û�ˢ����Ϣ��������
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
	printf("��̨Ͷ��������\r\n");
#endif
}

/***********************************************
Function: ��ͷ�ϴ��û�ˢ����Ϣ��������
************************************************/
void FrameUpDxICImfor(uint32_t sn,uint8_t cnt)
{
	EnQueue(&gDxUpCmpBuf,0x90);
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>8));	
	EnQueue(&gDxUpCmpBuf,(uint8_t)(Config.DxID>>0));	
	EnQueue(&gDxUpCmpBuf,DX_UP_IC_IMFOR_CMP);	//��ͷ�ϴ��û���Ϣ
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
	printf("��ͷ�ϴ��û�ˢ����Ϣ��������\r\n");
#endif
}





static u32 cmd_state;  //����֡β���״̬
static qsize cmd_pos;  //��ǰָ��ָ��λ��

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
		if((cmd_pos==0)&&(_data!=CMD_HEAD))//ָ���һ���ֽڱ�����֡ͷ����������
		{
#if FRAME_PKG_DBG	
				printf("��Ч֡!��������  ---%d---- byte���ݡ�\r\n",cmd_size);				
#endif
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





