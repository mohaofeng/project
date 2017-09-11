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
#define NetRxOk  BIT_SET0
#define NetTxOk  BIT_SET1


/*------------------���Ժ궨��------------------------*/
#define	TCP_DBG  0
#define	MAIN_DBG	0
#define TEST_DBG	0
#define BEEP_DBG	0
#define RC522TASK_DBG	0
#define NetDatExp_DBG	0
#define Nrf24l01TaskDBG 0   //nrf24l01���Ժ�
#define GateSigDBG 				0   //nrf24l01���Ժ�

#define DIS_NORMAL 1
#define DIS_INSER 0
#define NUM_FILL_0   0x80

#define XPlus  10
#define TimPlus  10

/*---------------------------��������----------------------*/
#define BI 1 					//Ĭ�϶�ͷid
#define YUAN 2 					//Ĭ�϶�ͷid

#define FLASH_DAT_LEN 20  				//��������������
#define DEFAULT_Price BI  				//Ĭ������id
#define DEFAULT_DX_ID 10 					//Ĭ�϶�ͷid
#define DEFAULT_TAG_FLG      8
#define ReTxcntMAX      20				//��ǰָ������ط�����
#define ReGASMAX      1					//�����ݷ���ʱ��������ʱ����
#define TxCntMAX_CONST		1000         //�������ݷ��ͼ��
#define TEST_TxCntMAX_CONST		3000         //�������ݷ��ͼ��
/*---------------------------״̬�ֶ���----------------------*/
#define WX_CMD 0x01
#define DX_CMD 0x02

#define SHEARCH_INVALID_IC 	0x02   //����������ʧ��
#define SHEARCH_IC_OK 			0x01   			//�û���Ϣ��ѯok
#define SHEARCH_NO_PAY 			0x03   		//����
#define OVER_TIM_ERR 				0x04   		//����
#define LOSS_ERR 						0x05   		//����

#define ACK__ERR 0x04   //err
#define ACK_SN_ERR 0x02   //����������ic���Ų�ͬ
#define COIN_OUT_OK 0x00   //�ϱ����

#define BEEP_ON_PWM 0
#define BEEP_Q    10000
#define BEEP_P    BEEP_Q/5



#define QLEN_MAX 400				 //���л�������󳤶�
#define NET_CMD_MAXLEN  32   //��λ��������󳤶�

/*-------------------------ʱ��궨�� --------------------*/
#define LED_GAS_CONST   							(50*XPlus)    		//LED ��˸���ʱ�� 500ms
#define Rc522Rd_GAS_CONST   					(10*XPlus)    		//�������ɨ��ʱ��  10*10ms
#define HOLD_MOD_CONST 								(3*XPlus)   			//��������ָ�ʱ�� 20x10ms
#define NRF_LNK_CONST_MAX 						(15*XPlus)  			//NRF24L01���Ӽ��ʱ��  x*10ms
#define LED_SCAN_CONST 								(1*XPlus)					//����ɨ����

#define PLUS_WITH_H_T  								(4)				//�ߵ�ƽ
#define PLUS_WITH_L_T  								(4)				//�ߵ�ƽ����ʱ��
#define PLUS_WITH_OVER_T  						(2000)				//���������ʱʱ��

#define RED_LIGHT_TIM_CONST 					(15*XPlus)
#define GREEN_LIGHT_TIM_CONST 				(10*XPlus)
#define BLUE_LIGHT_TIM_CONST 					(10*XPlus)
#define BEEP_LIT_CONST 								(20*XPlus)    //5x10ms
#define BEEP_STP_CONST 								(10*XPlus)    //5x10ms
#define RedFLASH_CONST                (5*XPlus)
#define RED_FLASH_CONST 							(30*XPlus)      			//ͬһ�û�ˢ������ʱ����




/*------------------------- �������� --------------------*/
#define rstMAX_CONST  								1000
#define gstMAX_CONST  								1000
#define bstMAX_CONST  								1000

#define rstMIN_CONST 	 								2
#define gstMIN_CONST  								2
#define bstMIN_CONST  								2

#define DxSwVerDEF										44
#define DxHwVerDEF										32

#define DEFAULT_DxFQ_NUM 							100 					//Ĭ��Ƶ��
#define DEFAULT_WxFQ_NUM 							100 					//Ĭ��Ƶ��

#define LIGHT_STP   									5   
#define DEC_LIGHT   									1   					//��������
#define INC_LIGHT   									0							//���ȼ���

/*------------------------- ��ʾģʽ���� --------------------*/
#define UNKNOW_ERR 					0x00   									//δ֪����
#define IDE_MOD 						0x01
#define DEC_MOD 						0x02
#define Invalid_IC_MOD 			0x04     	//�Ƿ���
#define NOLNK_MOD 					0x05
#define ICINFO_MOD 					0x07  	 	//ˢ���ɹ����۷ѳɹ�
#define NOMON_MOD 					0x08   		//ˢ���ɹ�������
#define ERR_PROMPT 					0X09  		//��ʾ��ϵ����Ա
#define ERR_LOSS_DIS 				0X0a 		 	//��ʾ��ϵ����Ա
#define ERR_OVER_DIS 				0X0b  		//��ʾ��ϵ����Ա
#define CONI_SUM_DIS 				0X0C  		//��ʾ���ֻ��Ʊ
#define CNT_TEMP_DIS 				0X0D  		//��ʾ��ʼ��Ʊ
#define OPTOO_OFENT 				0X0E  		//��ʾ��ʼ��Ʊ

#define DLY_Typ 						0Xff  		//��ʾ���ֻ��Ʊ



/*------------------------- ����ģʽ����--------------------*/
#define RGB_IDE_MOD  						1
#define RGB_Invalid_IC_MOD  		2
#define RGB_NOLNK_MOD  					3
#define RGB_NOMON_MOD  					4
#define RGB_ERR_PROMPT  				5
#define RGB_UNKNOW_ERR  				6
#define RGB_ERR_LOSS_DIS  			7
#define RGB_ERR_OVER_DIS  			8
#define RGB_ICINFO_MOD					9
#define RGB_CNT_TEMP_DIS				10



#define RunMod_NORMAL 1
#define RunMod_CHECK  2
#define RunMod_SAVER  3





#define RFID_TYPE 1







//ϵͳ������Ϣ
typedef struct
{
	uint16_t tagflg;											//��־
	uint8_t Price;												//����
	uint16_t DxID;												//�Ӷ�ͷ����
	uint8_t CoinX;												//Ͷ�ұ���
	uint8_t CoinOutLevel;									//������ip
	uint16_t CoinSigHod;										//
	uint16_t CoinSigGas;										//
	
	uint8_t DeviceType;										//Ĭ������
	uint8_t DxSwVer;											//����汾
	uint8_t DxHwVer;											//Ӳ���汾��	
	uint8_t disdir;												//��ʾ����
	uint16_t PromptTim;												//��ʾ����
	uint8_t DxFQ;													//����Ƶ��
	uint8_t WxFQ;													//����Ƶ��
	uint8_t RunMod;												//����ģʽ
	uint32_t DecMon;											//���Ѷ��	
	uint32_t ICSn;												//ic����
	uint32_t FPlusCntSum;									//δ��Ʊ��
}__ConfigVar;

extern __ConfigVar Config;							//ϵͳ���ò���

//LCD��Ҫ������
typedef struct  
{										    
	uint16_t width;			//LCD ���
	uint16_t height;			//LCD �߶�
	uint8_t DisFrameUp;
}_lcd_dev; 	  

extern _lcd_dev lcddev;	//����LCD��Ҫ����

//rc522 ������
typedef struct
{
	uint16_t RedLightValue;				//��
	uint16_t GreenLightValue;			//��
	uint16_t BlueLightValue;				//��
	uint16_t RedLightTim;				//��
	uint16_t GreenLightTim;				//��
	uint16_t BlueLightTim;				//��
	
	uint16_t BlueLightStp;				//��
	uint16_t GreenLightStp;				//��
	uint16_t RedLightStp;				//��
	
	uint8_t RedLightDir;				//��
	uint8_t GreenLightDir;			//��
	uint8_t BlueLightDir;				//��
	uint8_t RedLightDirold;				//��
	uint8_t GreenLightDirold;			//��
	uint8_t BlueLightDirold;				//��
	uint8_t RedLightDirnew;				//��
	uint8_t GreenLightDirnew;			//��
	uint8_t BlueLightDirnew;				//��
	uint16_t SignCnt;				//��
	uint16_t RedFlashTim;				//��
	uint16_t GreenFlashTim;				//��
	uint16_t BlueFlashTim;				//��
}__RGB_ctr;
extern __RGB_ctr RGB_ctr;	//����LCD��Ҫ����

//ϵͳ��ʱ��Ϣ
typedef struct{
	uint8_t gNrfLnkGas;				//2.4g����ģ�����ݴ���ʱ������ʱ����
	uint8_t ICScanGastmp;			//ˢ��ȥ����ʱ��
	uint8_t NETUpdateGas;			//����״̬��Ϣ���¼��
	uint8_t RCdUpdateGas;			//���Ѽ�¼�ϴ�Ƶ��
	uint16_t gLED_Gas;					//ϵͳ����ָʾ����ʱ����
	uint8_t gRc522RdGas;			//rc522����ʱ����ʱ����
	uint16_t gRc522HodGas;			//rc522����ʱ����ʱ����
	uint16_t BeepHodT;					//����������ͣ��ʱ�����
	uint8_t BeepStpT;					//���������尴ʱ����
	uint16_t gPlusGas;					//�ϱ��ź����嶨ʱ����
	uint16_t gPlusHold;					//�ϱ��ź����嶨ʱ����
	uint8_t RC522FlashDly;		//rc522��ʾ����ʱ����
	uint16_t LCDIdeT;			 		//lcd�ָ�����������ʱ����
	uint8_t gLCDScanGas;			//lcd������ʱ����
	uint8_t gLEDScanGas;			//����ˢ����ʱ����
	uint16_t RED_FlashDly;		//rc522��ʾ����ʱ����
	uint16_t ToolBarDly;
}__SysDlyT;
extern __SysDlyT Dly_Var;

//nrf24l01 ������
typedef struct
{
	uint8_t TxFlg;								//nrf24l01���ͱ�־
	uint8_t TxDatBufFlg;					//�����ݴ����ͱ�־
	uint8_t ReTxcnt;							//�ط�������
	uint8_t ReRxcnt;							//���ռ�����
	uint8_t DatLen;								//���ռ�����
	uint16_t TxCntTim;						//���ռ�����
}__nrf24l01_ctr;

extern __nrf24l01_ctr L01_Ctr;					//nrf24l01������

//rc522 ������
typedef struct
{
	uint8_t CT[2];				//������
	uint8_t RFID[16];			//���RFID 
	uint8_t SN[4]; 				//����
	uint8_t KEYA[6];
	uint8_t KEYB[6];
	uint8_t GetIcNum;
	uint8_t finish_flg;
}__Rc522_ctr;
extern __Rc522_ctr RC522Ctr;						//rc522������

//ϵͳȫ�ֱ���
volatile typedef struct
{
	uint32_t ye;				//���
	uint32_t xf;				//���ѽ��
//	uint32_t bi;				//���ѽ��
	uint8_t PlusNum; 		//��������
	uint8_t DisType; 	//��ʾģʽ
	uint8_t RGB_DisType;
	uint8_t UpDatCnt; 	//�ϴ��������
	uint8_t BeepType; 	//��������ʾ������
	uint16_t PlusCntT;//���������ʱ��
	uint16_t PlusCntHT;//����߼�����ʱ��
	uint16_t PlusCntLT;//���������ʱ��
	uint16_t PlusCntStep;//���������ʱ��
	uint32_t PlusCntSum;//���������ʱ��
	
	
// 	uint16_t PlusCntTCoin;//���������ʱ��
// 	uint16_t PlusCntHTCoin;//����߼�����ʱ��
// 	uint16_t PlusCntLTCoin;//���������ʱ��
// 	uint16_t PlusCntStepCoin;//���������ʱ��
// 	uint32_t PlusCntSumCoin;//���������ʱ��
	
	
	
	uint32_t PlusCntSumtmp;
	uint32_t PlusCntOverTim;
	uint32_t icsn;
	uint32_t icsn_old;
	uint32_t icsn_new;
	uint8_t CntFlg;
	uint8_t ICKeyA[6];
	uint8_t ICKeyB[6];
//	uint8_t In_ExtITFlg;//�����ⲿ�жϱ�־
//	uint8_t Saved_Flg;//��Ʊ�Ѿ��ɹ������־
	
}__Sys_Var;
extern __Sys_Var SysVar;

typedef struct
{
	uint16_t tcp_client_sta;						//nrf24l01���ͱ�־z
	uint16_t netsta;									//�����ݴ����ͱ�־
	uint8_t NetConnectRetryCnt;					//������������
	uint16_t NetChkGas;								//����״̬�����
	uint16_t ReConnectGas;						//�����������
	uint16_t LinkGas;									//�����������
	uint16_t LinkRetryCnt;						//���������ط�������
	uint8_t  StpSta;						//���������ط�������
	uint8_t csp;
	uint8_t flg;
//	uint8_t txsta;
//	uint8_t rxsta;
}__net_ctr;
extern __net_ctr gNetCtr;	//����LCD��Ҫ����

//ϵͳ��־
typedef struct{
	uint16_t KeyVal;				//�ϱ���������
	uint16_t KeySta;				//�ϱ���������
	uint8_t CoinOtstp;
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
uint16_t app_get_rand(uint16_t max,uint16_t min,uint16_t rand);
#endif



