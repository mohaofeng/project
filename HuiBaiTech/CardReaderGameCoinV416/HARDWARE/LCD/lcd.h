#ifndef __LCD_H
#define __LCD_H		
#include "sys.h"	 
#include "stdlib.h" 




extern u8  POINT_COLOR;//默认红色    
extern u8  BACK_COLOR; //背景颜色.默认为白色


//////////////////////////////////////////////////////////////////////////////////	 
//-----------------LCD端口定义---------------- 
//#define	LCD_LED PBout(15)  		//LCD背光    		 PB15 	    
//LCD地址结构体
typedef struct
{
	vu16 LCD_REG;
	vu16 LCD_RAM;
} LCD_TypeDef;
//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A6作为数据命令区分线 
//注意设置时STM32内部会右移一位对其! 111 1110=0X7E			    
#define LCD_BASE        ((u32)(0x6C000000 | 0x0000007E))
#define LCD             ((LCD_TypeDef *) LCD_BASE)
//////////////////////////////////////////////////////////////////////////////////
	 
//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左	 

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向

//画笔颜色
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
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

//#define DIS_NORMAL 1
//#define DIS_INSER 0
//#define NUM_FILL_0   0x80

void LCD_Init(void);													   	//初始化
void LCD_DisplayOn(void);													//开显示
void LCD_DisplayOff(void);													//关显示
void LCD_Clear(u16 Color);	 												//清屏
void LCDShowHZList(uint8_t x,uint8_t y,uint8_t hro,uint8_t size,uint8_t mode,uint8_t num);
void LCD_DrawPoint(u16 x,u16 y,u8 mod);
void LCD_12864_Clear(uint8_t dat);
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r,u8 mod);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u8 mod);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u8 mod);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);						//显示一个字符
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  						//显示一个数字
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//显示 数字
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);		//显示一个字符串,12/16字体
void LCD_ShowStringAndBk(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p,u8 mod);
void LCD_ShowNumAndBk(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mod);
void LCDRefresh_Gram(void);
void LCD_12864DrawPoint(u16 x,u16 y,u16 colr);
void Show_Font(u16 x,u16 y,u8** hor,u8 size,u8 mode);
void LCD_12864_ClearRec(uint8_t dat);
//LCD分辨率设置
#define SSD_HOR_RESOLUTION		800		//LCD水平分辨率
#define SSD_VER_RESOLUTION		480		//LCD垂直分辨率
//LCD驱动参数设置
#define SSD_HOR_PULSE_WIDTH		1		//水平脉宽
#define SSD_HOR_BACK_PORCH		46		//水平前廊
#define SSD_HOR_FRONT_PORCH		210		//水平后廊

#define SSD_VER_PULSE_WIDTH		1		//垂直脉宽
#define SSD_VER_BACK_PORCH		23		//垂直前廊
#define SSD_VER_FRONT_PORCH		22		//垂直前廊




#define SER_IP_XPOS  0
#define SER_IP_YPOS  10

#define LOCAL_IP_XPOS  0
#define LOCAL_IP_YPOS  26

#define SER_PORT_XPOS  0
#define SER_PORT_YPOS  42

#define STATUS_XPOS  35    //状态信息提示x坐标
#define STATUS_YPOS  13    //状态信息提示Y坐标
#define JINE_YPOS  30    		//金额显示y坐标
#define JINE_XPOS  0    		//金额显示y坐标


#define MON_DIS_ACM_YPOS  14    //卡金余额 y 坐标
#define MON_DIS_ACM_XPOS  53    //卡金余额 X 坐标
#define MON_DIS_ACMDOT_XPOS  82    //卡金余额 X 坐标  小数部分
#define MON_DIS_GIF_YPOS  26    //赠金余额 y 坐标
#define MON_DIS_GIF_XPOS  53    //卡金余额 X 坐标 小数部分
#define MON_DIS_GIFDOT_XPOS  82    //卡金余额 X 坐标
#define MON_DIS_DecTmp_YPOS  38    //扣费金额提示 y 坐标
#define MON_DIS_SN_YPOS  50     //ic卡号 y 坐标
#define MON_DIS_SN_XPOS  65      //ic卡号 x 坐标
#define MON_DIS_STA_YPOS  50    //状态 y 坐标
#define MON_DIS_STA_XPOS  65    //状态 x 坐标
#define DIS_G_POS 110
#define DIS_S_POS 104
#define DIS_GPRS_SIG_POS 117

#define DIS_IDE_XPOS 60
#define DIS_IDE_YPOS 20
#define DIS_STA_XPOS 0         //状态信息x坐标
#define DIS_STA_YPOS 50        //状态信息y坐标
#define DIS_STATMP_XPOS 30         //状态信息x坐标
#define DIS_STATMP_YPOS 50        //状态信息y坐标

#define CLK_DIS_YPOS      0     //时钟 y坐标
#define IC_NUM_DIS_YPOS      50 //时钟 y坐标




//显示模式定义
//#define IDE_MOD 0x01
//#define DEC_MOD 0x02
#define RST_MOD 0xff
#define ERR_MOD 0x04
#define NOLNK_MOD 0x05
#define NOOp_MOD 0x06
#define ICINFO_OK_MOD 0x07   //刷卡成功，扣费成功
#define NOMON_MOD 0x08   //刷卡成功，余额不足
#define LOGINOK_MOD 0x08   //登陆成功
#define CHANGE_XF 0x09   //登陆成功
#define SET_MOD 0x0a   //设置模式
#define CHECK_MOD 0x0b   //查询余额

//如下几个参数，自动计算
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)

#endif  
	 
	 



