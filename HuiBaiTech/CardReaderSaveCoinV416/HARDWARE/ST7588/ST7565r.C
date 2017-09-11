   

#define ST7565R_C


#include "st7565R.h"
#include "delay.h"
#include "usart.h"
#include "common.h"

//A0=0 ��������
//***************************************************************************SPI4-B 
void Write_ST7565R_COM(uint8_t COMDADA_4SPI)
{
	uint8_t bit8;
	A0=0;
	CS0=0;
	for(bit8=0;bit8<8;bit8++)
	{
		SCL=0;
		if ((COMDADA_4SPI << bit8) & 0x80)
			SDI=1;
		else		
			SDI=0;
		SCL=1;
	}
  CS0=1;
}
//�����ƶ�ʱ�����A0=1 ��������
void Write_ST7565R_DISPLAY_DATA_serial_4SPI(uint8_t DADA_4SPI)
{   
	uint8_t bit8;
	A0=1;
	CS0=0;				
	for(bit8=0;bit8<8;bit8++)
	{
		SCL=0;
		if ((DADA_4SPI << bit8) & 0x80)
			SDI=1;
		else
			SDI=0;
		SCL=1;
	}
	CS0=1;
}

void HDReset(void){
	RSTB=0;
	delay_ms(200);
	RSTB=1;
	delay_ms(200);
}


void SetStartPage(u8 StartPageAddress)//������ʼҳ���ַ
{
  Write_ST7565R_COM(SET_YADDR|StartPageAddress);
}
void SetStartColumn(u8 StartColumnAddress)//������ʼ�е�ַ
{
  Write_ST7565R_COM(SET_XADDH|StartColumnAddress);
  Write_ST7565R_COM(SET_XADDL|StartColumnAddress);
}
void SetStartLine(u8 StartLineAddress)//������ʼ�е�ַ
{
  Write_ST7565R_COM(SET_START_lINE|StartLineAddress);
}


void Init_ST7565r(void)
{	

		HDReset();

		Write_ST7565R_COM(0xE2);   //����λ
    delay_ms(200);
		Write_ST7565R_COM(0x2C);  //��ѹ����1������ѹ
    delay_ms(200);
    Write_ST7565R_COM(0x2E);  //��ѹ����2������ѹ����ѹ������
    delay_ms(200);
    Write_ST7565R_COM(0x2F);  //��ѹ����3������ѹ����ѹ����������ѹ������
     
    Write_ST7565R_COM(0x23);  //�ֵ��Աȶȣ������÷�Χ0x20��0x27
    Write_ST7565R_COM(0x81);  //΢���Աȶ�
    Write_ST7565R_COM(0x30);  //0x1A,΢���Աȶȵ�ֵ�������÷�Χ0x00��0x3f
     
		Write_ST7565R_COM(0x10);  /*���õ�ѹ�Ĳ���RRֵ*/
		Write_ST7565R_COM(0xA2);  //1/9ƫѹ�ȣ�bias��
   
    Write_ST7565R_COM(0xA0);  //��ɨ��˳�򣺴�����
    Write_ST7565R_COM(0xC8);  //��ɨ��˳�򣺴��ϵ���
    Write_ST7565R_COM(0x60);  //ɨ����ʼ�У�0X40��ʾ�ӵ�1�п�ʼ��0x41:��2��...0x60:��33�У�����LCD��������ƣ���LCD����ش�33�п�ʼɨ�衣
    Write_ST7565R_COM(0xAF);  //����ʾ

// 		if(Config.disdir)
// 			Write_ST7565R_COM(FUNC_SET_Y|MIRROR_X|MIRROR_Y);	  //function set//h1=h0=0  38
// 		else
			

//      Write_ST7565R_COM(0xe2);                  //�Ĵ�����λ
//      delay_ms(10);     
//      //  ��Դ������ʼ 
//      Write_ST7565R_COM(0xaf);                  //0xaf��ʾ����,0xae��ʾ���� 
//      Write_ST7565R_COM(0x2f);                  //0x2f��ѹ��·,��ѹ������·,
//      																				//��ѹ�����·����(0x28-0x2fΪ�����ϵ����ģʽ) 
//       Write_ST7565R_COM(0x26);                  //0x20-0x27ΪV5��ѹ�ڲ������������ 
//       Write_ST7565R_COM(0x81);                  //��������ģʽ 
//       Write_ST7565R_COM(0x0f);                  //0x01-0x3f�����Ĵ�������ģʽ 

//      // �������ķ����ʼ����װ����(��λ��Ĭ����������)

//      Write_ST7565R_COM(0xa0);                  //0xa0ΪSegment����,0xa1 ΪSegment����
// //     																				//(0xa0-0xa1ΪADC����ѡ��) 
//       Write_ST7565R_COM(0xc8);                  //0xc0����ɨ��,0xc8����ɨ�� 
//      // ��ʾ������ʼ(��λ��Ĭ����������)
//      Write_ST7565R_COM(0xa6);                  //0xa6������ʾ,0xa7������ʾ         
//      Write_ST7565R_COM(0xa4);                  //0xa4������ʾ,0xa5ȫ������
//     // ָʾ������
//      Write_ST7565R_COM(0xac);                  //
//      Write_ST7565R_COM(0x00);                  //
//     //��ѹ������
//      Write_ST7565R_COM(0xf8);                  //
//      Write_ST7565R_COM(0x00);                  //		

}
