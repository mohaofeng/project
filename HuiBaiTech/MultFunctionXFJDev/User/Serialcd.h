#ifndef __SERIAL_LCD_H
#define __SERIAL_LCD_H		
//#include "sys.h"	 
#include "stdlib.h" 
#include <stdint.h>
//#include "types.h"




//LCD重要参数集
typedef struct  
{										    
	uint16_t  width;								//LCD 宽度
	uint16_t 	height;								//LCD 高度
	uint16_t current_screen_id;
	uint16_t Fcolorv;
	uint16_t Bcolorv;
	uint16_t new_screen_id;
	uint16_t old_screen_id;
	uint8_t distype;
	uint8_t disstaold;
	uint8_t disstanew;
	uint32_t test_value;
	uint32_t LcdTaskRunFlg;
}__LCDParm; 	  
extern __LCDParm lcd_parm;




#define SHIFT_LINE 1
#define SIGLE_LINE  0



#define DIS_NORMAL 1
#define DIS_INSER 0
#define NUM_FILL_0   0x80

void LCDDisplayTask(void);
void LCDShowSysParm(void);
void TFT_LCD_Init(void);
//void DBG_LCD_PRINTF(uint8_t *buff,uint8_t type);
void Lcd_Show(uint16_t x, uint16_t y,uint8_t back,uint8_t font,char *fmt, ...);
void Update_LcdText(uint16_t screen_id,uint16_t control_id,char *fmt, ...);

void DisCardcx(void);
//void UpdateUI(void);
void ToolBar(void);



#endif  /* __SERIAL_LCD_H */
	 







