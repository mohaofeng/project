#ifndef __24L01_H
#define __24L01_H	 		  
#include "sys.h"   
#include "sqstack.h"


#ifdef NRF24L01_C
#define NRF24L01_EXT 
#else
#define NRF24L01_EXT extern
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//NRF24L01�Ĵ�����������
#define READ_REG_NRF        0x00  //�����üĴ���,��5λΪ�Ĵ�����ַ
#define WRITE_REG_NRF       0x20  //д���üĴ���,��5λΪ�Ĵ�����ַ
#define RD_RX_PLOAD     0x61  //��RX��Ч����,1~32�ֽ�
#define WR_TX_PLOAD     0xA0  //дTX��Ч����,1~32�ֽ�
#define FLUSH_TX        0xE1  //���TX FIFO�Ĵ���.����ģʽ����
#define FLUSH_RX        0xE2  //���RX FIFO�Ĵ���.����ģʽ����
#define REUSE_TX_PL     0xE3  //����ʹ����һ������,CEΪ��,���ݰ������Ϸ���.
#define NOP             0xFF  //�ղ���,����������״̬�Ĵ���	 
//SPI(NRF24L01)�Ĵ�����ַ
#define CONFIG          0x00  //���üĴ�����ַ;bit0:1����ģʽ,0����ģʽ;bit1:��ѡ��;bit2:CRCģʽ;bit3:CRCʹ��;
                              //bit4:�ж�MAX_RT(�ﵽ����ط������ж�)ʹ��;bit5:�ж�TX_DSʹ��;bit6:�ж�RX_DRʹ��
#define EN_AA           0x01  //ʹ���Զ�Ӧ����  bit0~5,��Ӧͨ��0~5
#define EN_RXADDR       0x02  //���յ�ַ����,bit0~5,��Ӧͨ��0~5
#define SETUP_AW        0x03  //���õ�ַ���(��������ͨ��):bit1,0:00,3�ֽ�;01,4�ֽ�;02,5�ֽ�;
#define SETUP_RETR      0x04  //�����Զ��ط�;bit3:0,�Զ��ط�������;bit7:4,�Զ��ط���ʱ 250*x+86us
#define RF_CH           0x05  //RFͨ��,bit6:0,����ͨ��Ƶ��;
#define RF_SETUP        0x06  //RF�Ĵ���;bit3:��������(0:1Mbps,1:2Mbps);bit2:1,���书��;bit0:�������Ŵ�������
#define STATUS          0x07  //״̬�Ĵ���;bit0:TX FIFO����־;bit3:1,��������ͨ����(���:6);bit4,�ﵽ�����ط�
                              //bit5:���ݷ�������ж�;bit6:���������ж�;
#define MAX_TX  		0x10  //�ﵽ����ʹ����ж�
#define TX_OK   		0x20  //TX��������ж�
#define RX_OK   		0x40  //���յ������ж�

#define OBSERVE_TX      0x08  //���ͼ��Ĵ���,bit7:4,���ݰ���ʧ������;bit3:0,�ط�������
#define CD              0x09  //�ز����Ĵ���,bit0,�ز����;
#define RX_ADDR_P0      0x0A  //����ͨ��0���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P1      0x0B  //����ͨ��1���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P2      0x0C  //����ͨ��2���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P3      0x0D  //����ͨ��3���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P4      0x0E  //����ͨ��4���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P5      0x0F  //����ͨ��5���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define TX_ADDR         0x10  //���͵�ַ(���ֽ���ǰ),ShockBurstTMģʽ��,RX_ADDR_P0��˵�ַ���
#define RX_PW_P0        0x11  //��������ͨ��0��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P1        0x12  //��������ͨ��1��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P2        0x13  //��������ͨ��2��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P3        0x14  //��������ͨ��3��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P4        0x15  //��������ͨ��4��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P5        0x16  //��������ͨ��5��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define NRF_FIFO_STATUS 0x17  //FIFO״̬�Ĵ���;bit0,RX FIFO�Ĵ����ձ�־;bit1,RX FIFO����־;bit2,3,����
                              //bit4,TX FIFO�ձ�־;bit5,TX FIFO����־;bit6,1,ѭ��������һ���ݰ�.0,��ѭ��;
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//24L01������
#define NRF24L01_CE   PAout(8) //24L01Ƭѡ�ź�
#define NRF24L01_CSN  PBout(12) //SPIƬѡ�ź�	   
#define NRF24L01_IRQ  PAin(11)  //IRQ������������
//24L01���ͽ������ݿ�ȶ���
#define TX_ADR_WIDTH    5   	//5�ֽڵĵ�ַ���
#define RX_ADR_WIDTH    5   	//5�ֽڵĵ�ַ���
#define TX_PLOAD_WIDTH  22  	//32�ֽڵ��û����ݿ��
#define RX_PLOAD_WIDTH  22  	//32�ֽڵ��û����ݿ��
	



#define  NET_ID 0X04   //��λ��id
/*===========================================================================
															������״̬�ֶ���
============================================================================*/
//״̬���
#define RDER_STAON          0x01			//dx��·����
#define RDER_DATUP_SVD      0x02			//dx�д��ϴ����ݵ���λ��

#define RDER_NETRCV         0x04			//WX�д��´�����


// /*===========================================================================
// 														DX��WX  2.4G����ͨ��Э��
// ============================================================================*/
// //2.4g �´�������
// #define WDW_NOTHIN_DX 	    	0X00						//WX�´�����Ϣ��   dx
// #define WDW_DAT_DX 	    			0X01						//WX�´�Ͷ����Ϣ��DX
// #define WDW_NOMON_DX 	    		0X02						//WX�´�������Ϣ��DX
// #define WDW_VALIDIC_DX 	    	0X04						//WX�´��Ƿ�����Ϣ��DX
// #define WDW_CONICFG_DX 	    	0X08						//WX�´��ϱ�������Ϣ��DX
// #define WDW_CHKMONY_DX 	    	0X20						//WX�´���Ӧ����ѯ��ϢDX
// #define WDW_CHKCFG_DX 	    	0X40						//WX�´�����ѯ��������Ϣ��DX

// //2.4g �ϴ�������
// #define DUP_LNK_WX 	    			0X02						//dx�ϴ���·�źŵ�wx
// #define DUP_SNDAT_WX 	    		0X04						//dx�ϴ�������Ϣ��wx
// #define DUP_CONICFG_WX 	    	0X08						//dx�ϴ��ϱ�������Ϣ��wx
// #define DUP_CHKMONY_WX 	    	0X20						//dx�ϴ�����ѯ��Ϣ��wx
// #define DUP_CHKCFG_WX 	    	0X40						//dx�ϴ������������óɹ���Ϣ��wx
// #define DUP_COINOK_WX 	    	0X80						//dx�ϴ������ϱҳɹ���Ϣ��wx

// /*===========================================================================
// 													wx����λ��ͨ��Э��
// ============================================================================*/
// //��λ���´�������wx
// #define CMPDW_NOTHIN_WX   		0X00				//��λ���´�����Ϣ֡   wx
// #define CMPDW_MSG_COIN_WX   	0X01				//��λ���´�Ͷ����Ϣ��WX
// #define CMPDW_MSG_NOMON_WX   	0X02				//��λ���´�������Ϣ��WX
// #define CMPDW_MSG_VALIDIC_WX  0X04				//��λ���´��Ƿ�����Ϣ��WX
// #define CMPDW_COINCFG_WX   		0X08				//��λ���´�DX�ϱ�������Ϣ��WX
// #define CMPDW_IDCONFIG_WX   	0X10				//��λ���´� WX id������Ϣ��WX
// #define CMPDW_CHKMONY_WX 	    0X20			  //��λ���´���ӦDX����ѯ��
// #define CMPDW_CHKCFG_WX 	    0X40			  //��λ���´���DX����Ϊ����ѯ����
// #define CMPDW_CHKVER_WX 	    0X80			  //��λ����ѯ������Ӳ���汾��


// //��λ���´���������Ϣλ�ö���
// #define DXlST_POSITION  			5    				//�Ӷ�ͷid�б���Ϣλ��
// #define WXID_POSITION  				4    				//����Դ��Ϣλ��
// #define CMD_POSITION  				3    				//֡����λ��
// #define RXID_POSITION  				2    				//����Դ��ַ��Ϣ
// #define DxLEN_POSITION  			1    				//��ͷ������Ϣλ��
// #define DxID_POSITION  				1    				//��ͷ������Ϣλ��

// //wx�ϴ������ֵ���λ��
// #define WUP_DxSTU_CMP 	    	0X02				//wx �ϴ� DX... ״̬��Ϣ����λ��
// #define WUP_ICSN_CMP 	    		0X04				//WX �ϴ� DX ������Ϣ����λ��
// #define WUP_COINCFG_CMP 	    0X08				//WX �ϴ� DX�ϱ�������Ϣ����λ��
// #define WUP_IDCONFIG_CMP 	    0X10				//WX �ϴ� WX ID������Ϣ����λ��
// #define WUP_CHKMONY_CMP 	    0X20			  //wx �ϴ�dx��ѯ�����Ϣ
// #define WUP_CHKCFG_CMP 	    	0X40			  //Wx�ϴ���Ӧ��λ������dxΪ������
// #define WUP_DWDXOK_CMP 	    	0X80			  //Wx�ظ���λ��Ͷ����Ϣ�´��ɹ�
// #define WUP_WXVER_CMP 	    	0X01			  //Wx�ظ���λ����Ӳ���汾��Ϣ








/*===========================================================================
															WX����DX��������
============================================================================*/



void NRF24L01_Init(void);//��ʼ��
void NRF24L01_RX_Mode(uint8_t ch);//����Ϊ����ģʽ
void NRF24L01_TX_Mode(uint8_t ch);//����Ϊ����ģʽ
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf,u8 len);//д������
uint8_t NRF24L01_Read_Buf(uint8_t reg, uint8_t *pBuf,u8 len);//��������		  
uint8_t NRF24L01_Read_Reg(uint8_t reg);			//���Ĵ���
uint8_t NRF24L01_Write_Reg(uint8_t reg, uint8_t value);//д�Ĵ���
uint8_t NRF24L01_Check(void);//���24L01�Ƿ����
u8 NRF24L01_TxPacket(u8 *txbuf);
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf);//����һ����������
void Nrf24l01Task(void);
uint8_t ChkBufSum(uint8_t *buf,uint8_t len);
#endif











