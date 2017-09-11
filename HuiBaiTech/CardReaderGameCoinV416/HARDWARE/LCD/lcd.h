#ifndef __LCD_H
#define __LCD_H		
#include "sys.h"	 
#include "stdlib.h" 




extern u8  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern u8  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ


//////////////////////////////////////////////////////////////////////////////////	 
//-----------------LCD�˿ڶ���---------------- 
//#define	LCD_LED PBout(15)  		//LCD����    		 PB15 	    
//LCD��ַ�ṹ��
typedef struct
{
	vu16 LCD_REG;
	vu16 LCD_RAM;
} LCD_TypeDef;
//ʹ��NOR/SRAM�� Bank1.sector4,��ַλHADDR[27,26]=11 A6��Ϊ�������������� 
//ע������ʱSTM32�ڲ�������һλ����! 111 1110=0X7E			    
#define LCD_BASE        ((u32)(0x6C000000 | 0x0000007E))
#define LCD             ((LCD_TypeDef *) LCD_BASE)
//////////////////////////////////////////////////////////////////////////////////
	 
//ɨ�跽����
#define L2R_U2D  0 //������,���ϵ���
#define L2R_D2U  1 //������,���µ���
#define R2L_U2D  2 //���ҵ���,���ϵ���
#define R2L_D2U  3 //���ҵ���,���µ���

#define U2D_L2R  4 //���ϵ���,������
#define U2D_R2L  5 //���ϵ���,���ҵ���
#define D2U_L2R  6 //���µ���,������
#define D2U_R2L  7 //���µ���,���ҵ���	 

#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��

//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

//#define DIS_NORMAL 1
//#define DIS_INSER 0
//#define NUM_FILL_0   0x80

void LCD_Init(void);													   	//��ʼ��
void LCD_DisplayOn(void);													//����ʾ
void LCD_DisplayOff(void);													//����ʾ
void LCD_Clear(u16 Color);	 												//����
void LCDShowHZList(uint8_t x,uint8_t y,uint8_t hro,uint8_t size,uint8_t mode,uint8_t num);
void LCD_DrawPoint(u16 x,u16 y,u8 mod);
void LCD_12864_Clear(uint8_t dat);
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r,u8 mod);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u8 mod);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u8 mod);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						//��ʾһ���ַ�
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						//��ʾһ������
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//��ʾ ����
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		//��ʾһ���ַ���,12/16����
void LCD_ShowStringAndBk(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p,u8 mod);
void LCD_ShowNumAndBk(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mod);
void LCDRefresh_Gram(void);
void LCD_12864DrawPoint(u16 x,u16 y,u16 colr);
void Show_Font(u16 x,u16 y,u8** hor,u8 size,u8 mode);
void LCD_12864_ClearRec(uint8_t dat);
//LCD�ֱ�������
#define SSD_HOR_RESOLUTION		800		//LCDˮƽ�ֱ���
#define SSD_VER_RESOLUTION		480		//LCD��ֱ�ֱ���
//LCD������������
#define SSD_HOR_PULSE_WIDTH		1		//ˮƽ����
#define SSD_HOR_BACK_PORCH		46		//ˮƽǰ��
#define SSD_HOR_FRONT_PORCH		210		//ˮƽ����

#define SSD_VER_PULSE_WIDTH		1		//��ֱ����
#define SSD_VER_BACK_PORCH		23		//��ֱǰ��
#define SSD_VER_FRONT_PORCH		22		//��ֱǰ��




#define SER_IP_XPOS  0
#define SER_IP_YPOS  10

#define LOCAL_IP_XPOS  0
#define LOCAL_IP_YPOS  26

#define SER_PORT_XPOS  0
#define SER_PORT_YPOS  42

#define STATUS_XPOS  35    //״̬��Ϣ��ʾx����
#define STATUS_YPOS  13    //״̬��Ϣ��ʾY����
#define JINE_YPOS  30    		//�����ʾy����
#define JINE_XPOS  0    		//�����ʾy����


#define MON_DIS_ACM_YPOS  14    //������� y ����
#define MON_DIS_ACM_XPOS  53    //������� X ����
#define MON_DIS_ACMDOT_XPOS  82    //������� X ����  С������
#define MON_DIS_GIF_YPOS  26    //������� y ����
#define MON_DIS_GIF_XPOS  53    //������� X ���� С������
#define MON_DIS_GIFDOT_XPOS  82    //������� X ����
#define MON_DIS_DecTmp_YPOS  38    //�۷ѽ����ʾ y ����
#define MON_DIS_SN_YPOS  50     //ic���� y ����
#define MON_DIS_SN_XPOS  65      //ic���� x ����
#define MON_DIS_STA_YPOS  50    //״̬ y ����
#define MON_DIS_STA_XPOS  65    //״̬ x ����
#define DIS_G_POS 110
#define DIS_S_POS 104
#define DIS_GPRS_SIG_POS 117

#define DIS_IDE_XPOS 60
#define DIS_IDE_YPOS 20
#define DIS_STA_XPOS 0         //״̬��Ϣx����
#define DIS_STA_YPOS 50        //״̬��Ϣy����
#define DIS_STATMP_XPOS 30         //״̬��Ϣx����
#define DIS_STATMP_YPOS 50        //״̬��Ϣy����

#define CLK_DIS_YPOS      0     //ʱ�� y����
#define IC_NUM_DIS_YPOS      50 //ʱ�� y����




//��ʾģʽ����
//#define IDE_MOD 0x01
//#define DEC_MOD 0x02
#define RST_MOD 0xff
#define ERR_MOD 0x04
#define NOLNK_MOD 0x05
#define NOOp_MOD 0x06
#define ICINFO_OK_MOD 0x07   //ˢ���ɹ����۷ѳɹ�
#define NOMON_MOD 0x08   //ˢ���ɹ�������
#define LOGINOK_MOD 0x08   //��½�ɹ�
#define CHANGE_XF 0x09   //��½�ɹ�
#define SET_MOD 0x0a   //����ģʽ
#define CHECK_MOD 0x0b   //��ѯ���

//���¼����������Զ�����
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)

#endif  
	 
	 



