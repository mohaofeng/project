#ifndef __HC595_H
#define __HC595_H	 
#include "sys.h"   

#define 	HC595_DBG 1

#ifdef	HC595_C
#define	HC595_EXT
#else
#define	HC595_EXT extern
#endif



/***********************************************************************/
//#define  HC595_DAT	  PBout(4)
//#define  HC595_RCK	  PBout(3)
//#define  HC595_SCK	  PBout(2)

#define DISSCANGAS_CONST 4
#define DISOFFGAS_CONST 1
#define DOTGASCONST 	200
#define HOLDMODCONST 	3000


#define DELT 0xFFFFffff
#define SYSLEDGAS_CONST	50
#define DIS_DLY_CONST		100  //200X10MS
#define DIS_DLY_5sCONST		100  //200X10MS
#define IDE_MOD 0x00
#define DEC_MOD 0x01
#define RST_MOD 0x02
#define ERR_MOD 0x04
#define NOLNK_MOD 0x05
#define NOOp_MOD 0x06

#define OFFSEG  0x00  //数码管关闭短码
#define GANGSEG 0x40
#define DOT 0x10

#define HC595_SI PBout(5)
#define HC595_RCK PBout(6)
#define HC595_SCK PBout(7)


#define HC595_RCK_H()          	HC595_RCK=1;
#define HC595_RCK_L()        		HC595_RCK=0;
#define HC595_SCK_H()          	HC595_SCK=1;
#define HC595_SCK_L()        		HC595_SCK=0;
#define HC595_DAT_H()          	HC595_SI=1;
#define HC595_DAT_L()        		HC595_SI=0;

HC595_EXT uint16_t DxHwVer;//硬件版本号，为bcd码
HC595_EXT uint16_t gMony;//用户余额信息
HC595_EXT uint8_t gMonyDec;//扣款金额
HC595_EXT uint8_t gDisBuf[4];
HC595_EXT uint16_t gSysLedGas;
HC595_EXT uint16_t gDisScanGas;
HC595_EXT uint16_t gIdeFlashSpd;
HC595_EXT uint8_t gDotGas;
HC595_EXT uint16_t gLCDScanGas;//余额显示时间计时器
HC595_EXT uint16_t gDisDlyT;
HC595_EXT uint16_t gDisType;

/***********************************************************************/
 void HC595_init(void);
 //void DisAllOn(void);
 //void DisAllOFF(void);
 //void DigiTst(void);
 void DisPro(void);


#endif

