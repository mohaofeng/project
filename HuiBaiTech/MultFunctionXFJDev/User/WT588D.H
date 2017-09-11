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
/*------------ 命令字定义 ----------------------------*/
//声音调节
#define WT588D_VOL1_8        		0xE0
#define WT588D_VOL2_8        		0xE1
#define WT588D_VOL3_8        		0xE2
#define WT588D_VOL4_8        		0xE3
#define WT588D_VOL5_8        		0xE4
#define WT588D_VOL6_8        		0xE5
#define WT588D_VOL7_8        		0xE6
#define WT588D_VOL8_8        		0xE7
//循环播放
#define CLYPLAY        				0xF2
//停止播放
#define STOPPLAY        			0xFE


/* 声音模式定义 */
#define SOUND_NULL_MOD 							0
#define SOUND_0 										1   	//声音 0
#define SOUND_1 										2   	//声音 1
#define SOUND_2 										3   	//声音 2
#define SOUND_3 										4   	//声音 3
#define SOUND_4 										5   	//声音 4
#define SOUND_5											6   	//声音 5
#define SOUND_6 										7   	//声音 6 
#define SOUND_7 										8   	//声音 7
#define SOUND_8 										9   	//声音 8
#define SOUND_9 										10   	//声音 9
#define SOUND_IDE_MOD 							11		//
#define SOUND_DEC_MOD 							12		//
#define SOUND_FEIFA 								13  	//声音 非法卡
#define SOUND_ERR_MOD 							14		//声音 错误
#define SOUND_SHIJIAN 							15		//声音 时间卡	
#define SOUND_CHUZHI_CARD 					16   	//声音 储值卡
#define SOUND_GUANLI_CARD 					17   	//声音 管理卡
#define SOUND_ERR_OVER 							18  	//声音 卡片过期
#define SOUND_ERR_LOSS 							19  	//声音 卡片挂失
#define SOUND_NOMON_MOD 						20   	//声音 余额不足
#define SOUND_ICINFO_OK_MOD 				21   	//声音 刷卡成功
#define SOUND_LOGINOK_MOD 					22   	//声音 登陆成功
#define SOUND_CHANGE_XF 						23  	//声音 修改消费额度
#define SOUND_JICI 									24   	//声音 设置模式
#define SOUND_CHECK_MOD 						25   	//声音 查询余额
#define SOUND_TOO_OFTEN 						26   	//声音 刷卡太频繁
#define SOUND_IC_CARD_KEY_ERR 			27   	//声音 IC卡密码验证失败
#define SOUND_SHI 									28   	//声音 十
#define SOUND_BAI 									29   	//声音 百
#define SOUND_QIAN 									30   	//声音 千
#define SOUND_WAN 									31   	//声音 万
#define SOUND_DIAN 									32   	//声音 点
#define SOUND_F1 										33   	//声音 F1
#define SOUND_F2 										34   	//声音 F2
#define SOUND_F3 										35   	//声音 F3
#define SOUND_F4 										36   	//声音 F4
#define SOUND_QUEREN 								37   	//声音 确认
#define SOUND_QUXIAO 								38   	//声音 取消
#define SOUND_CHAXUN 								39   	//声音 查询
#define SOUND_OP_TOO_OFENT 					40   	//声音 IC卡密码验证失败
#define SOUND_SAVED_FAILED 					41   	//存票失败
#define SOUND_YU_E_CHECK 						42   	//余票查询
#define SOUND_SAVED_OK 							43   	//存票成功






#define SOUND_RST_MOD 							255


//语音地址表
//#define WT588D_LOSLNK_VOL         0x08	//链接中断

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
#define WT588D_SHI       					  11				//十
#define WT588D_BAI       					  12				//百
#define WT588D_QIAN        	    		13    		//千
#define WT588D_WAN        					14				//万
#define WT588D_FEIFA        		  	15				//非法卡
#define WT588D_NO_MONEY        		  16				//余额不足
#define WT588D_XIAOFEI        		  17				//本次消费
#define WT588D_DIAN        		  		18				//点
#define WT588D_f1        						19				//f1
#define WT588D_f2        						20				//f2
#define WT588D_f3        						21				//f3
#define WT588D_f4        						22				//f4
#define WT588D_queren        				23				//确认
#define WT588D_quxiao        				24				//取消
#define WT588D_yu_e_ck        			25				//余额查询
#define WT588D_tuikuan        			26				//退款
#define WT588D_WEL_VOL1        		 	27				//欢迎使用汇百科技智能管理系统
#define WT588D_yuan        					28				//元
#define WT588D_adminic        			29				//管理卡
#define WT588D_CountIc        			30				//记次卡
#define WT588D_DateCard        			31				//时间卡
#define WT588D_OverTimIC        		32				//卡片过期
#define WT588D_shuru        				33				//请输入金额
#define WT588D_chaoe        				34				//输入超额
#define WT588D_qingshuka        		35				//请刷卡
#define WT588D_WELCOMM        			36				//欢迎使用
#define WT588D_INVALID_IC        		37				//非法卡
#define WT588D_LOST_IC        			38				//挂失卡
#define WT588D_LOGO_IN        			39				//登陆成功
#define WT588D_NO_LINK        			40				//无链接
#define WT588D_ERR        					41				//未知错误
#define WT588D_shuakaok       			42				//刷卡成功
#define WT588D_shuakafailed       	43				//刷卡失败
#define WT588D_xiaofeiedu       		44				//消费额度
#define WT588D_chaxunok       			45				//查询成功
#define WT588D_CHUZHI_IC       			50				//储值卡
#define WT588D_IC_KEY_ERR       		46				//卡片密码验证失败
#define WT588D_TOO_OFENT       			47				//刷卡太频繁
#define WT588D_YU_E_CHECK       		48				//彩票余票查询
#define WT588D_SAVED_OK       			49				//存票成功
#define WT588D_SAVED_FAILED       	50				//存票失败
	

//Sound 控制器
typedef struct
{
	uint8_t Sta;						//状态
	uint8_t Type;						//类型
	uint8_t Typenew;						//类型
	uint8_t Typeold;						//类型
	uint8_t OutFlg;						//语音输出状态
	uint8_t OutStep;						//播音步骤
	uint8_t OutPos;						//播音位置
}__SoundCtr;
extern __SoundCtr SoundCtr;


void Wt588dInit(void);
void SoundTask(void);
void Line_1A(uint8_t add);
void Line_3A(uint8_t addr);
#endif


