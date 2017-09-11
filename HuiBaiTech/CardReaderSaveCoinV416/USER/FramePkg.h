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


#define CMD_HEAD 0x90//֡ͷ
#define CMD_TAIL 0XFeFCFeFc //֡β
#define CMD_TAIL_Nbyte 1 //֡β
#define CMD_POS 1							//������λ��

/***********************************************
������Э�������ֶ���    Sn -> Dn
************************************************/
#define CMP_DWN_COIN_OUT   		 0xff	//�������ظ��ϱ�����
#define CMP_CONFIG_DX_ID   		 0xfe	//���������ö�ͷID��
#define CMP_CONFIG_COIN_PAR    0xfd	//���������ö�ͷ�ϱҲ���
#define CMP_CONFIG_DIS_PAR    	0xed	//���������ö�ͷ�����ʾʱ��
#define CMP_CONFIG_DIR_PAR    	0xee	//���������ö�ͷ��ʾ����
#define CMP_CONFIG_TEST    			0xec	//����������ָ��
#define CMP_COIN_OT_TEST    			0xeb	//�����������ϱ�ָ��
#define CMP_RX_OK_TEST    			0xea	//�������ظ�����ͨ��ָ��
#define CMP_CONFIG_FQ_WX    			0xe8	//�������ظ�����ͨ��ָ��
#define CMP_CONFIG_PRICE    			0xe7	//�������ظ�����ͨ��ָ��
#define CMP_CONFIG_PAR    				0xe5	//�������ظ�����ͨ��ָ��
#define CMP_ACK_SAVED    					0xe4	//�������ظ�����ͨ��ָ��
#define CMP_ACK_SAVED_CNT    			0xB1	//�������ظ�ͳ��Ͷ������ָ��

/***********************************************
������Э�������ֶ���    Sn -> Wn
************************************************/
#define CMP_CONFIG_WX_ID   		 0xfc	//��������������id
#define CMP_CONFIG_WX_IP   		 0xfb	//��������������iP

/***********************************************
������Э�������ֶ���    Dn -> Sn
************************************************/
#define DX_UP_IC_IMFOR_CMP   		 0x02	//��ͷ�ϴ��û���Ϣ
#define DX_UP_CONFIRM_OK_CMP   	 0x04	//��ͷ�ظ�������ȷ�Ͽۿ�
#define DX_UP_CONFIG_CMP     		 0x08	//��ͷ�ظ���������
#define DX_UP_ACK_TEST_CMP     	 0x10	//��ͷ�ظ�����ָ��
#define DX_UP_TEST_CMP     		 	 0x20	//��ͷ����ָ��
#define DX_ACK_OUT_COINOK_CMP    0x40	//��ͷ�ظ������������ϱ�ok
#define DX_UP_IC_COIN_CMP   		 0x80	//��ͷ�ϴ����ֻ��Ʊ
#define DX_UP_COIN_CNT_CMP   		 0x81	//��̨Ͷ��������

/***********************************************
������Э�������ֶ���    Wn -> Sn
************************************************/
#define WxACK_CMP_CONFIG_ID    0xfa	//�ظ�����������ID����
#define WxUP_CMP_Dx_STA   		 0xf9	//�ظ���������ͷ״̬
void AckDXImfor(uint8_t dxid,uint8_t wxid,uint8_t *sn);
void AckOutCoinSta(uint8_t *sn,uint8_t sta);
void FrameUpDxICImfor(uint32_t sn,uint8_t cnt);
void FrameUpDxConfig(void);
void AckTest(void);
void TESTOutCoinOK(uint8_t FrmSn);
void FrameUpDxCoinSum(uint32_t sn,uint32_t cnt,uint8_t fsn);
void FrameUpDxCoinCnt(uint32_t cnt,uint8_t fsn);
qsize queue_find_cmd(PQUEUE Q,elementType *buffer,qsize buf_len);


#endif






