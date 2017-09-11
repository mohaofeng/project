#ifndef __CMPDATAPROCESS_H
#define	__CMPDATAPROCESS_H
#include "SysConfig.h"
#include "cmd_process.h"





/* 网络数据帧帧头帧尾定义*/
#define NET_CMD_MAXLEN  		1024   				//TCP指令最大长度
#define NET_CMD_HEAD 			0x90				//帧头
#define NET_CMD_TAIL 			0xfefcfefc 			//帧尾

//7寸串口彩屏串口帧头帧尾
#define TFT_CMD_MAXLEN  		500   				//TCP指令最大长度
#define TFT_CMD_HEAD 			0xee				//帧头
#define TFT_CMD_TAIL 			0xfffcffff 			//帧尾

//二维码读头帧头帧尾
#define CODE_CMD_MAXLEN  		200   				//二维码指令做大长度
#define CODE_CMD_HEAD 			0x61				//帧头
#define CODE_CMD_TAIL 			0x7778797A 			//帧尾






void CodeDatExp(void);
void ScomTftCmdExp(void);
void NetDatExp(void);
void ProcessMessage( PCTRL_MSG msg, uint16_t size );
int CheckStr(uint8_t min,uint8_t max,char c,uint8_t cnt,uint8_t *tmpbuf,char *mstrpc);
void TestSrintInput(uint8_t min,uint8_t max,char c,uint8_t cnt,char *mstrpc);
#endif /* __CMPDATAPROCESS_H */

