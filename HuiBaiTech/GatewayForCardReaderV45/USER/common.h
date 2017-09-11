#ifndef _COMMONFUN_H
#define _COMMONFUN_H
#include <stdint.h>

#define BIT_SET0  0X0001
#define BIT_SET1  0X0002
#define BIT_SET2  0X0004
#define BIT_SET3  0X0008
#define BIT_SET4  0X0010
#define BIT_SET5  0X0020
#define BIT_SET6  0X0040
#define BIT_SET7  0X0080
#define BIT_SET8  0X0100
#define BIT_SET9 	0X0200
#define BIT_SET10  0X0400
#define BIT_SET11  0X0800
#define BIT_SET12  0X1000
#define BIT_SET13  0X2000
#define BIT_SET14  0X4000
#define BIT_SET15  0X8000


/*------------------���Ժ궨��------------------------*/
#define MAIN_DBG	0
#define TEST_DBG	0
#define NetDatExp_DBG	0
#define Nrf24l01TaskDBG 	0    	//nrf24l01���Ժ�



/*---------------------------��������----------------------*/
#define FLASH_DAT_LEN 50  //��������������
#define DX_LIST_MAXLEN 10  //�ӿ�ͷ�������
#define DEFAULT_TAG_FLG      0x1
//#define DX_STU_UP_GAS   500//��ͷ״̬��Ϣ�ϴ���� 100x10ms
#define LED_GAS_CONST   50//��ͷ״̬��Ϣ�ϴ���� 100x10ms
#define CONNECT_OVER_CONST 200   //300*10ms
#define QLEN_MAX 400				 //���л�������󳤶�
#define NET_CMD_MAXLEN  40   //��λ��������󳤶�
#define L01_CMD_MAXLEN  32   //��λ��������󳤶�

#define DEFAULT_DxFQ_NUM 	100 					//Ĭ��Ƶ��
#define DEFAULT_WxFQ_NUM 	100 					//Ĭ��Ƶ��

#define ReTxcntMAX      	10				//��ǰָ������ط�����
#define ReGASMAX      		1					//�����ݷ���ʱ��������ʱ����
#define TxCntMAX					6+4         //�������ݷ��ͼ��

//�궨��
#define NRF_LNK_CONST 		10  	//NRF24L01���Ӽ��ʱ��  x*10ms
#define TX_DAT_GAS				2
#define RX_DAT_GAS				5
#define	LinkGasTCONST    	300    //�������
#define LinkMAXCONST     	3     //��������ط�����
#define WX_CMD 						0x01
#define DX_CMD 						0x02

#define DEFAULT_WX_ID    2

//ϵͳ������Ϣ
typedef struct
{
	uint16_t tagflg;											//��־
	uint16_t WxID;													//����ID
	uint8_t DxNum;												//�Ӷ�ͷ����
	uint8_t DxLst[DX_LIST_MAXLEN];				//�Ӷ�ͷ����
	uint8_t Rmoteip[4];										//������ip
	uint8_t Localip[4];										//����ip
	uint8_t GateW[4];											//Ĭ������
	uint8_t Mass[4];											//��������
	uint16_t Port;												//�˿ں�
	uint8_t Mac[6];												//�˿ں�
	uint8_t DxFQ;													//����Ƶ��
	uint8_t WxFQ;													//����Ƶ��

}__ConfigVar;
extern __ConfigVar Config;//ϵͳ���ò���

//nrf24l01 ������
typedef struct
{
	uint8_t TxFlg;						//nrf24l01���ͱ�־
	uint8_t DatTxFlg;					//�����ݴ����ͱ�־
	uint8_t ReTxcnt;					//�ط�������
	uint8_t ReRxcnt;					//���ռ�����
	uint8_t TxCnt;
}__nrf24l01_ctr;
extern __nrf24l01_ctr L01_Ctr;

//ϵͳ��ʱ��Ϣ
typedef struct{
	uint32_t tst_gas;					//������
	uint16_t LinkGasT;					//������
	uint16_t wdog_gas;					//������
	
}__SysDlyT;
extern __SysDlyT DlyVar;

//ϵͳ��־
typedef struct{
	uint8_t NetTxOkFlg;				//�������ݳɹ��ʹ�
	uint8_t NetTickCnt;				//�������ݳɹ��ʹ�
	uint8_t	netstaflg;
}__SysFlg;
extern __SysFlg gFlg;	


void showdbg8dec(uint8_t *bf,uint8_t len);
void showdbg8hex(uint8_t *bf,uint8_t len);
void showdbg32dec(uint32_t *bf,uint8_t len);
void showdbg32hex(uint32_t *bf,uint8_t len);
void showdbg16hex(uint16_t *bf,uint8_t len);
void ExplainParm(void);
void WriteParm(void);
void PrintCfgPar(void);
void WriteDefaultSet(void);
uint8_t ChkBufSum(uint8_t *buf,uint8_t len);
uint32_t app_get_rand(uint32_t max,uint32_t min);
#endif



