#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "sys.h"  

//////////////////////////////////////////////////////////////////////////////////////////////////////
//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 512 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08010000 	//STM32 FLASH����ʼ��ַ
#define OFFSETWQ 								2
#define IN_COIN_CNT_OFFSET 			4  		//ÿ�εο��ϱҴ���
#define IN_COIN_SIGSTA_OFFSET 	6			//�ϱ��źų�̬
#define IN_COIN_SIGHOD_OFFSET 	8			//�ϱ��ź�ά��ʱ��
#define Dx_CHK_FLG_OFFSET 			10			//����ѯ����־�����ַ
#define OFFSETDQ 								12		//��ͷid��

//FLASH������ֵ
 

u16 STMFLASH_ReadHalfWord(u32 faddr);		  //��������  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);   

//����д��
void Test_Write(u32 WriteAddr,u16 WriteData);								   
#endif

















