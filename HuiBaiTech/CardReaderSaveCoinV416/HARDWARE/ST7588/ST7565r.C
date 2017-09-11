   

#define ST7565R_C


#include "st7565R.h"
#include "delay.h"
#include "usart.h"
#include "common.h"

//A0=0 发送命令
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
//动画移动时间变量A0=1 发送数据
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


void SetStartPage(u8 StartPageAddress)//设置起始页面地址
{
  Write_ST7565R_COM(SET_YADDR|StartPageAddress);
}
void SetStartColumn(u8 StartColumnAddress)//设置起始列地址
{
  Write_ST7565R_COM(SET_XADDH|StartColumnAddress);
  Write_ST7565R_COM(SET_XADDL|StartColumnAddress);
}
void SetStartLine(u8 StartLineAddress)//设置起始行地址
{
  Write_ST7565R_COM(SET_START_lINE|StartLineAddress);
}


void Init_ST7565r(void)
{	

		HDReset();

		Write_ST7565R_COM(0xE2);   //软复位
    delay_ms(200);
		Write_ST7565R_COM(0x2C);  //升压步聚1：打开升压
    delay_ms(200);
    Write_ST7565R_COM(0x2E);  //升压步聚2：打开升压及电压调整器
    delay_ms(200);
    Write_ST7565R_COM(0x2F);  //升压步聚3：打开升压、电压调整器、电压跟随器
     
    Write_ST7565R_COM(0x23);  //粗调对比度，可设置范围0x20～0x27
    Write_ST7565R_COM(0x81);  //微调对比度
    Write_ST7565R_COM(0x30);  //0x1A,微调对比度的值，可设置范围0x00～0x3f
     
		Write_ST7565R_COM(0x10);  /*设置电压的参数RR值*/
		Write_ST7565R_COM(0xA2);  //1/9偏压比（bias）
   
    Write_ST7565R_COM(0xA0);  //列扫描顺序：从左到右
    Write_ST7565R_COM(0xC8);  //行扫描顺序：从上到下
    Write_ST7565R_COM(0x60);  //扫描起始行：0X40表示从第1行开始，0x41:第2行...0x60:第33行，由于LCD的走线设计，本LCD特殊地从33行开始扫描。
    Write_ST7565R_COM(0xAF);  //打开显示

// 		if(Config.disdir)
// 			Write_ST7565R_COM(FUNC_SET_Y|MIRROR_X|MIRROR_Y);	  //function set//h1=h0=0  38
// 		else
			

//      Write_ST7565R_COM(0xe2);                  //寄存器复位
//      delay_ms(10);     
//      //  电源管理初始 
//      Write_ST7565R_COM(0xaf);                  //0xaf显示器开,0xae显示器关 
//      Write_ST7565R_COM(0x2f);                  //0x2f升压电路,电压管理电路,
//      																				//电压跟随电路均开(0x28-0x2f为设置上电控制模式) 
//       Write_ST7565R_COM(0x26);                  //0x20-0x27为V5电压内部电阻调整设置 
//       Write_ST7565R_COM(0x81);                  //电量设置模式 
//       Write_ST7565R_COM(0x0f);                  //0x01-0x3f电量寄存器设置模式 

//      // 根据屏的方向初始化安装方向(复位后默认下面配置)

//      Write_ST7565R_COM(0xa0);                  //0xa0为Segment正向,0xa1 为Segment反向
// //     																				//(0xa0-0xa1为ADC方向选择) 
//       Write_ST7565R_COM(0xc8);                  //0xc0正向扫描,0xc8反射扫描 
//      // 显示管理初始(复位后默认下面配置)
//      Write_ST7565R_COM(0xa6);                  //0xa6正向显示,0xa7反向显示         
//      Write_ST7565R_COM(0xa4);                  //0xa4正常显示,0xa5全屏点亮
//     // 指示器设置
//      Write_ST7565R_COM(0xac);                  //
//      Write_ST7565R_COM(0x00);                  //
//     //背压比设置
//      Write_ST7565R_COM(0xf8);                  //
//      Write_ST7565R_COM(0x00);                  //		

}

