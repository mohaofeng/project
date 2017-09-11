#ifndef _W5500_CONF_H_
#define _W5500_CONF_H_

#include "stm32f10x.h"
#include "stdio.h"
#include <stdint.h>
//#include "types.h"
#define __GNUC__

typedef  void (*pFunction)(void);

extern uint8_t    remote_ip[4];                            	  /* 远端IP地址                   */
extern uint8_t    use_dhcp;                              	    /* 是否使用DHCP获取IP           */
extern uint8_t    use_eeprom;                             	  /* 是否使用EEPROM中的IP配置信息 */

/*
 * 通过宏定义选择目标开发板
 * STM32F103ZET6 -> 秉火ISO-V3开发板
 * STM32F103VET6 -> 秉火ISO-MINI-V3开发板
 */
/*使用秉火ISO-MINI-V3开发板*/
#define STM32F103VET6
#undef  STM32F103ZET6

/*定义SPI1作为W5500的硬件接口*/
#define WIZ_SPIx_GPIO_PORT      GPIOA								  	  	/* GPIO端口                     */
#define WIZ_SPIx_GPIO_CLK       RCC_APB2Periph_GPIOA	  	  /* GPIO端口时钟                 */
#define WIZ_SPIx                SPI1                   	    /* 定义W5500所用的SPI接口       */
#define WIZ_SPIx_CLK_CMD        RCC_APB2PeriphClockCmd
#define WIZ_SPIx_CLK            RCC_APB2Periph_SPI1    	    /* 定义W5500所用的SPI接口时钟   */
#define WIZ_SPIx_SCLK           GPIO_Pin_5						   	  /* 定义W5500的时钟管脚          */
#define WIZ_SPIx_MISO           GPIO_Pin_6						   	  /* 定义W5500的MISO管脚          */
#define WIZ_SPIx_MOSI           GPIO_Pin_7						   	  /* 定义W5500的MOSI管脚          */



#ifdef  STM32F103ZET6 
  #define WIZ_SPIx_SCS          GPIO_Pin_9						   	  /* 定义W5500的片选管脚          */
  #define WIZ_SPIx_SCS_PORT     GPIOG									   	  /* GPIO端口                     */
  #define WIZ_SPIx_SCS_CLK      RCC_APB2Periph_GPIOG	   	  /* GPIO端口时钟                 */
   
	#define WIZ_RESET             GPIO_Pin_4									/* 定义W5500的RESET管脚         */
  #define WIZ_SPIx_RESET_PORT   GPIOA									   	  /* GPIO端口                     */
  #define WIZ_SPIx_RESET_CLK    RCC_APB2Periph_GPIOA	  	  /* GPIO端口时钟                 */
  
	#define WIZ_INT               GPIO_Pin_4									/* 定义W5500的INT管脚           */
  #define WIZ_SPIx_INT_PORT     GPIOC									      /* GPIO端口                     */
  #define WIZ_SPIx_INT_CLK      RCC_APB2Periph_GPIOC	      /* GPIO端口时钟                 */
#endif


#ifdef STM32F103VET6 
  #define WIZ_SPIx_SCS          GPIO_Pin_3						 	    /* 定义W5500的片选管脚          */
  #define WIZ_SPIx_SCS_PORT     GPIOC									 	    /* GPIO端口                     */
  #define WIZ_SPIx_SCS_CLK      RCC_APB2Periph_GPIOC	 	    /* GPIO端口时钟                 */
  
	#define WIZ_RESET             GPIO_Pin_4									/* 定义W5500的RESET管脚         */
  #define WIZ_SPIx_RESET_PORT   GPIOA									  	  /* GPIO端口                     */
  #define WIZ_SPIx_RESET_CLK    RCC_APB2Periph_GPIOA	 	    /* GPIO端口时钟                 */
  
	#define WIZ_INT               GPIO_Pin_4									/* 定义W5500的INT管脚           */
  #define WIZ_SPIx_INT_PORT     GPIOC									      /* GPIO端口                     */
  #define WIZ_SPIx_INT_CLK      RCC_APB2Periph_GPIOC	      /* GPIO端口时钟                 */
#endif

#define FW_VER_HIGH  						1               
#define FW_VER_LOW    					0
#define ON	                 		1
#define OFF	                 		0
#define HIGH	           	 			1
#define LOW		             			0

#define MAX_BUF_SIZE		 				1460       			            /*定义每个数据包的大小*/
#define KEEP_ALIVE_TIME	     		30	// 30sec
#define TX_RX_MAX_BUF_SIZE      2048							 
#define EEPROM_MSG_LEN        	sizeof(EEPROM_MSG)

#define IP_FROM_DEFINE	        0       			              /*使用初始定义的IP信息*/
#define IP_FROM_DHCP	          1       			              /*使用DHCP获取IP信息*/
#define IP_FROM_EEPROM	        2       			              /*使用EEPROM定义的IP信息*/




//extern uint8_t	ip_from;											  


///*选择IP信息配置源*/
/*此结构体定义了W5500可供配置的主要参数*/
typedef struct _CONFIG_MSG											
{
  uint8_t mac[6];																							/*MAC地址*/
  uint8_t lip[4];																							/*local IP本地IP地址*/
  uint8_t sub[4];																							/*子网掩码*/
  uint8_t gw[4];																							/*网关*/	
  uint8_t dns[4];																							/*DNS服务器地址*/
  uint8_t rip[4];																							/*remote IP远程IP地址*/
	uint8_t sw_ver[2];																					/*软件版本号*/
}CONFIG_MSG;
extern CONFIG_MSG  	ConfigMsg;




extern uint8_t dhcp_ok;																				/*DHCP获取成功*/
extern uint32_t	dhcp_time;																		/*DHCP运行计数*/
extern vu8	ntptimer;																				/*NPT秒计数*/

/*MCU配置相关函数*/
void gpio_for_w5500_config(void);														/*SPI接口reset 及中断引脚*/
void timer2_init(void);																			/*STM32定时器2初始化*/
void timer2_isr(void);																			/*定时器中断执行的函数*/
void reboot(void);																					/*STM32软复位*/
void write_config_to_eeprom(void);													/*写配置信息到EEPROM中*/
void read_config_from_eeprom(void);													/*从EEPROM中读出信息*/

/*W5500SPI相关函数*/
void IINCHIP_WRITE( uint32_t addrbsb,  uint8_t data);						/*写入一个8位数据到W5500*/
uint8_t IINCHIP_READ(uint32_t addrbsb);													/*从W5500读出一个8位数据*/
uint16_t wiz_write_buf(uint32_t addrbsb,uint8_t* buf,uint16_t len);	/*向W5500写入len字节数据*/
uint16_t wiz_read_buf(uint32_t addrbsb, uint8_t* buf,uint16_t len);	/*从W5500读出len字节数据*/

/*W5500基本配置相关函数*/
void reset_w5500(void);																			/*硬复位W5500*/
void set_w5500_mac(void);																		/*配置W5500的MAC地址*/
void set_w5500_ip(void);																		/*配置W5500的IP地址*/

/*需要用定时的的应用函数*/
void dhcp_timer_init(void);																	/*dhcp用到的定时器初始化*/
void ntp_timer_init(void);																	/*npt用到的定时器初始化*/
void W5500Init(void);

#endif
