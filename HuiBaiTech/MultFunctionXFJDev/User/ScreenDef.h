#ifndef ScreenDef_H
#define ScreenDef_H


// typedef unsigned          char uint8_t;
// typedef unsigned          char uchar;
// typedef unsigned short     int uint16_t;
// typedef unsigned           int uint32_t;
// typedef unsigned       __int64 uint64;
#include <stdint.h>

/*   显示系统参数画面   */
#define SHOW_SYSPARM_SCREEN_ID    											0 	 
#define SHOW_SYSPARM_SCREEN_REMOTE_IP_TEXT_ID 					18  		//服务器ip文本框
#define SHOW_SYSPARM_SCREEN_SUB_NET_TEXT_ID 						1  			//子网掩码文本框
#define SHOW_SYSPARM_SCREEN_GATE_WAY_TEXT_ID 						2  			//默认网关文本框
#define SHOW_SYSPARM_SCREEN_LOCAL_IP_TEXT_ID 						19  		//本机ip文本框
#define SHOW_SYSPARM_SCREEN_MAC_ADDR_TEXT_ID 						20  		//本机mac文本框
#define SHOW_SYSPARM_SCREEN_WIFI_NAME_TEXT_ID 					21  		//WIFI名称文本框
#define SHOW_SYSPARM_SCREEN_WIFI_PWD_TEXT_ID 						22  		//WIFI密码文本框
#define SHOW_SYSPARM_SCREEN_REMOTE_PORT_TEXT_ID 				23  		//服务器端口号文本框

#define SHOW_SYSPARM_SCREEN_GATE_TIM_TEXT_ID 						27  		//开门时间
#define SHOW_SYSPARM_SCREEN_PULS_TIM_TEXT_ID 						28  		//上币时间
#define SHOW_SYSPARM_SCREEN_GATE_GAS_TEXT_ID 						29  		//开门间隔
#define SHOW_SYSPARM_SCREEN_PULS_GAS_TEXT_ID 						30  		//上币间隔

#define SHOW_SYSPARM_SCREEN_GATE_STA_BUTTON_ID 					25  		//开门信号nc/no开关
#define SHOW_SYSPARM_SCREEN_COIN_STA_BUTTON_ID 					26  		//投币口信号nc/no开关
#define SHOW_SYSPARM_SCREEN_DHCP_SW_BUTTON_ID 					41  		//DHCP开关
#define SHOW_SYSPARM_SCREEN_NET_MOD_BUTTON_ID 					24  		//联网方式开关
#define SHOW_SYSPARM_SCREEN_RUN_MOD_BUTTON_ID 					32  		//运行模式开关
#define SHOW_SYSPARM_SCREEN_QUIT_BUTTON_ID 							34  		//退出开关
#define SHOW_SYSPARM_SCREEN_SET_BUTTON_ID 							44  		//配置开关

/*   配置系统网络参数画面    */
#define SET_SYSPARM_FOR_NET_SCREEN_ID    									1 	

/*   固定金额消费机界面*/
#define FIXED_MON_SCREEN_ID    														2  		

/*   可变档位消费机界面 */
#define STALL_MON_SCREEN_ID    														3  		
#define BT_STALL_MON_SCREEN_ID 														2  		//档位切换按钮

/*  按键输入消费机界面 */
#define KEY_INPUT_MON_SCREEN_ID    												4  	
#define KEY_INPUT_MON_SCREEN_TEXT_ID     									1    //文本输入框ID  	





/*  会员取币界面  */
#define VIP_QUBI_SCREEN_ID    						5 

/*  会员存币界面  */
#define VIP_CUN_SCREEN_ID    						6  

/*  系统返回信息界面  */
#define SERVER_ACK_IMFOR_SCREEN_ID    				7 

/*  会员购币界面  */
#define VIP_GOUBI_SCREEN_ID    						8 

/*  会员存取一体机界面  */
#define VIP_SAVE_SALE_SCREEN_ID    						9



/* VIP固定消费模式待机界面  */
#define IDE_FIXED_MON_SCREEN_ID						10

/* 可切换档位待机界面  */
#define IDE_STALL_MON_SCREEN_ID						11

/* VIP按键模式待机界面  */
#define IDE_KEY_INPUT_MON_SCREEN_ID					12


/* VIP取币机待机界面  */
#define IDE_VIP_QUBI_SCREEN_ID						13

/* VIP存币机待机界面  */
#define IDE_VIP_CUN_SCREEN_ID						14

/*  会员购币待机界面 */
#define IDE_GOUBI_SCREEN_ID    						15 

/* VIP存取一体机待机界面  */
#define IDE_SAVE_SALE_SCREEN_ID						16

/* VIP道闸控制器待机界面  */
#define IDE_GATE_CTRL_SCREEN_ID						17

/* VIP查询机待机界面  */
#define IDE_CHECK_SCREEN_ID							18

/* 道闸控制界面 */
#define GATE_CTRL_SCREEN_ID    						19 

/* 查询机机界面  */
#define CHECK_SCREEN_ID    							20 

/* VIP未定义待机界面  */
#define LOGIN_SCREEN_ID						21


/* VIP未定义待机界面  */
#define IDE_RESERVER_SCREEN_ID						22



/* 预留界面  */
#define RESERVER_SCREEN_ID    						50 




#define CMD_MAX_SIZE 100        /*!<单条指令大小，根据需要调整，尽量设置大一些*/
//#define QUEUE_MAX_SIZE 1000   /*!< 指令接收缓冲区大小，根据需要调整，尽量设置大一些*/


#define TIME_100MS 10  //100毫秒(10个单位)


//void UpdateUI(void);



#endif



