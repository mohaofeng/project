#ifndef __WT588D_H
#define __WT588D_H	 
#include "SysConfig.h"



#ifdef WT588D_C
#define WT588D_EXT 
#else
#define WT588D_EXT extern
#endif

#define WT588D_CLK_H()          	GPIO_SetBits(GPIOE,GPIO_Pin_2);
#define WT588D_CLK_L()        		GPIO_ResetBits(GPIOE,GPIO_Pin_2);
#define WT588D_DAT_H()          	GPIO_SetBits(GPIOE,GPIO_Pin_4);
#define WT588D_DAT_L()        		GPIO_ResetBits(GPIOE,GPIO_Pin_4);
#define WT588D_CS_H()          		GPIO_SetBits(GPIOE,GPIO_Pin_3);
#define WT588D_CS_L()        			GPIO_ResetBits(GPIOE,GPIO_Pin_3);
		


//#define SCL PBout(7)
//#define SDA PBout(9)
//#define CS PBout(8)
/*------------ �����ֶ��� ----------------------------*/
//��������
#define WT588D_VOL1_8        		0xE0
#define WT588D_VOL2_8        		0xE1
#define WT588D_VOL3_8        		0xE2
#define WT588D_VOL4_8        		0xE3
#define WT588D_VOL5_8        		0xE4
#define WT588D_VOL6_8        		0xE5
#define WT588D_VOL7_8        		0xE6
#define WT588D_VOL8_8        		0xE7
//ѭ������
#define CLYPLAY        				0xF2
//ֹͣ����
#define STOPPLAY        			0xFE


/* ����ģʽ���� */
#define SOUND_NULL_MOD 							0
#define SOUND_0 										1   	//���� 0
#define SOUND_1 										2   	//���� 1
#define SOUND_2 										3   	//���� 2
#define SOUND_3 										4   	//���� 3
#define SOUND_4 										5   	//���� 4
#define SOUND_5											6   	//���� 5
#define SOUND_6 										7   	//���� 6 
#define SOUND_7 										8   	//���� 7
#define SOUND_8 										9   	//���� 8
#define SOUND_9 										10   	//���� 9
#define SOUND_IDE_MOD 							11		//
#define SOUND_DEC_MOD 							12		//
#define SOUND_FEIFA 								13  	//���� �Ƿ���
#define SOUND_ERR_MOD 							14		//���� ����
#define SOUND_SHIJIAN 							15		//���� ʱ�俨	
#define SOUND_CHUZHI_CARD 					16   	//���� ��ֵ��
#define SOUND_GUANLI_CARD 					17   	//���� ������
#define SOUND_ERR_OVER 							18  	//���� ��Ƭ����
#define SOUND_ERR_LOSS 							19  	//���� ��Ƭ��ʧ
#define SOUND_NOMON_MOD 						20   	//���� ����
#define SOUND_ICINFO_OK_MOD 				21   	//���� ˢ���ɹ�
#define SOUND_LOGINOK_MOD 					22   	//���� ��½�ɹ�
#define SOUND_CHANGE_XF 						23  	//���� �޸����Ѷ��
#define SOUND_JICI 									24   	//���� ����ģʽ
#define SOUND_CHECK_MOD 						25   	//���� ��ѯ���
#define SOUND_TOO_OFTEN 						26   	//���� ˢ��̫Ƶ��
#define SOUND_IC_CARD_KEY_ERR 			27   	//���� IC��������֤ʧ��
#define SOUND_SHI 									28   	//���� ʮ
#define SOUND_BAI 									29   	//���� ��
#define SOUND_QIAN 									30   	//���� ǧ
#define SOUND_WAN 									31   	//���� ��
#define SOUND_DIAN 									32   	//���� ��
#define SOUND_F1 										33   	//���� F1
#define SOUND_F2 										34   	//���� F2
#define SOUND_F3 										35   	//���� F3
#define SOUND_F4 										36   	//���� F4
#define SOUND_QUEREN 								37   	//���� ȷ��
#define SOUND_QUXIAO 								38   	//���� ȡ��
#define SOUND_CHAXUN 								39   	//���� ��ѯ
#define SOUND_OP_TOO_OFENT 					40   	//���� IC��������֤ʧ��
#define SOUND_SAVED_FAILED 					41   	//��Ʊʧ��
#define SOUND_YU_E_CHECK 						42   	//��Ʊ��ѯ
#define SOUND_SAVED_OK 							43   	//��Ʊ�ɹ�






#define SOUND_RST_MOD 							255


//������ַ��
//#define WT588D_LOSLNK_VOL         0x08	//�����ж�

#define WT588D_0        		  			1					//0
#define WT588D_1        		  			2					//1
#define WT588D_2        		  			3					//2
#define WT588D_3        		  			4					//3
#define WT588D_4        		  			5					//4
#define WT588D_5       		  				6					//5
#define WT588D_6        		  			7					//6
#define WT588D_7        		  			8					//7
#define WT588D_8        		  			9					//8
#define WT588D_9        		  			10				//9
#define WT588D_SHI       					  11				//ʮ
#define WT588D_BAI       					  12				//��
#define WT588D_QIAN        	    		13    		//ǧ
#define WT588D_WAN        					14				//��
#define WT588D_FEIFA        		  	15				//�Ƿ���
#define WT588D_NO_MONEY        		  16				//����
#define WT588D_XIAOFEI        		  17				//��������
#define WT588D_DIAN        		  		18				//��
#define WT588D_f1        						19				//f1
#define WT588D_f2        						20				//f2
#define WT588D_f3        						21				//f3
#define WT588D_f4        						22				//f4
#define WT588D_queren        				23				//ȷ��
#define WT588D_quxiao        				24				//ȡ��
#define WT588D_yu_e_ck        			25				//����ѯ
#define WT588D_tuikuan        			26				//�˿�
#define WT588D_WEL_VOL1        		 	27				//��ӭʹ�û�ٿƼ����ܹ���ϵͳ
#define WT588D_yuan        					28				//Ԫ
#define WT588D_adminic        			29				//������
#define WT588D_CountIc        			30				//�Ǵο�
#define WT588D_DateCard        			31				//ʱ�俨
#define WT588D_OverTimIC        		32				//��Ƭ����
#define WT588D_shuru        				33				//��������
#define WT588D_chaoe        				34				//���볬��
#define WT588D_qingshuka        		35				//��ˢ��
#define WT588D_WELCOMM        			36				//��ӭʹ��
#define WT588D_INVALID_IC        		37				//�Ƿ���
#define WT588D_LOST_IC        			38				//��ʧ��
#define WT588D_LOGO_IN        			39				//��½�ɹ�
#define WT588D_NO_LINK        			40				//������
#define WT588D_ERR        					41				//δ֪����
#define WT588D_shuakaok       			42				//ˢ���ɹ�
#define WT588D_shuakafailed       	43				//ˢ��ʧ��
#define WT588D_xiaofeiedu       		44				//���Ѷ��
#define WT588D_chaxunok       			45				//��ѯ�ɹ�
#define WT588D_CHUZHI_IC       			50				//��ֵ��
#define WT588D_IC_KEY_ERR       		46				//��Ƭ������֤ʧ��
#define WT588D_TOO_OFENT       			47				//ˢ��̫Ƶ��
#define WT588D_YU_E_CHECK       		48				//��Ʊ��Ʊ��ѯ
#define WT588D_SAVED_OK       			49				//��Ʊ�ɹ�
#define WT588D_SAVED_FAILED       	50				//��Ʊʧ��
	

//Sound ������
typedef struct
{
	uint8_t Sta;						//״̬
	uint8_t Type;						//����
	uint8_t Typenew;						//����
	uint8_t Typeold;						//����
	uint8_t OutFlg;						//�������״̬
	uint8_t OutStep;						//��������
	uint8_t OutPos;						//����λ��
}__SoundCtr;
extern __SoundCtr SoundCtr;


void Wt588dInit(void);
void SoundTask(void);
void Line_1A(uint8_t add);
void Line_3A(uint8_t addr);
#endif


