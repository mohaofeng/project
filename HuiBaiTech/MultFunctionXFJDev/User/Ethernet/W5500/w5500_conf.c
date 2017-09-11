/*
**************************************************************************************************
* @file    		w5500_conf.c
* @author  		WIZnet Software Team 
* @version 		V1.0
* @date    		2015-02-14
* @brief  		配置MCU，移植W5500程序需要修改的文件，配置W5500的MAC和IP地址
**************************************************************************************************
*/
#include <stdio.h> 
#include <string.h>

#include "w5500_conf.h"
//#include "bsp_i2c_ee.h"
#include "utility.h"
#include "w5500.h"
#include "dhcp.h"
#include "bsp_TiMbase.h"
#include "SysConfig.h"
//#include "esp07.h"


CONFIG_MSG  ConfigMsg;																	/*配置结构体*/


/*IP配置方法选择，请自行选择*/
//uint8_t	ip_from=IP_FROM_DEFINE;				
uint8_t	ip_from=IP_FROM_DHCP;				

uint8_t   dhcp_ok   = 0;													   			/*dhcp成功获取IP*/
uint32_t	ms        = 0;															  	/*毫秒计数*/
uint32_t	dhcp_time = 0;															  	/*DHCP运行计数*/
vu8	    ntptimer  = 0;															  	/*NPT秒计数*/

/**
*@brief		配置W5500的IP地址
*@param		无
*@return	无
*/
void set_w5500_ip(void)
{	
//   /*复制定义的配置信息到配置结构体*/
	memcpy(ConfigMsg.mac, Config.Mac, 6);
	memcpy(ConfigMsg.lip,gNetCtr.LanLocalip,4);
	memcpy(ConfigMsg.sub,gNetCtr.LanMass,4);
	memcpy(ConfigMsg.gw,gNetCtr.LanGateW,4);
	memcpy(ConfigMsg.dns,Config.dns,4);
//	if(ip_from==IP_FROM_DEFINE)	
//		printf(" 使用定义的IP信息配置W5500\r\n");
//#if 0		
//	
//	/*使用EEPROM存储的IP参数*/	
//	if(ip_from==IP_FROM_EEPROM)
//	{
//    
//		/*从EEPROM中读取IP配置信息*/
//		read_config_from_eeprom();		
//		
//		/*如果读取EEPROM中MAC信息,如果已配置，则可使用*/		
//		if( *(EEPROM_MSG.mac)==0x00&& *(EEPROM_MSG.mac+1)==0x08&&*(EEPROM_MSG.mac+2)==0xdc)		
//		{
//			printf(" IP from EEPROM\r\n");
//			/*复制EEPROM配置信息到配置的结构体变量*/
//			memcpy(ConfigMsg.lip,EEPROM_MSG.lip, 4);				
//			memcpy(ConfigMsg.sub,EEPROM_MSG.sub, 4);
//			memcpy(ConfigMsg.gw, EEPROM_MSG.gw, 4);
//		}
//		else
//		{
//			printf(" EEPROM未配置,使用定义的IP信息配置W5500,并写入EEPROM\r\n");
//			write_config_to_eeprom();	/*使用默认的IP信息，并初始化EEPROM中数据*/
//		}			
//	}
//#endif
//	/*使用DHCP获取IP参数，需调用DHCP子函数*/		
//	if(ip_from==IP_FROM_DHCP)								
//	{
//		/*复制DHCP获取的配置信息到配置结构体*/
//		if(dhcp_ok==1)
//		{
//			printf(" IP from DHCP\r\n");		 
//			memcpy(Config.Localip,DHCP_GET.lip, 4);
//			memcpy(Config.Mass,DHCP_GET.sub, 4);
//			memcpy(Config.GateW,DHCP_GET.gw, 4);
//			memcpy(Config.dns,DHCP_GET.dns,4);
//		}
//		else
//		{
//			printf(" DHCP子程序未运行,或者不成功\r\n");
//			printf(" 使用定义的IP信息配置W5500\r\n");
//		}
//	}
		
	/*以下配置信息，根据需要选用*/	
//	ConfigMsg.sw_ver[0]=FW_VER_HIGH;
//	ConfigMsg.sw_ver[1]=FW_VER_LOW;	

	/*将IP配置信息写入W5500相应寄存器*/	
	setSUBR(ConfigMsg.sub);
	setGAR(ConfigMsg.gw);
	setSIPR(ConfigMsg.lip);
  //memcpy(ConfigMsg.dns,dns_server,4);
	getSIPR (gNetCtr.LanLocalip);			
	printf(" W5500 IP地址   : %d.%d.%d.%d\r\n", gNetCtr.LanLocalip[0],gNetCtr.LanLocalip[1],gNetCtr.LanLocalip[2],gNetCtr.LanLocalip[3]);
	getSUBR(gNetCtr.LanMass);
	printf(" W5500 子网掩码 : %d.%d.%d.%d\r\n", gNetCtr.LanMass[0],gNetCtr.LanMass[1],gNetCtr.LanMass[2],gNetCtr.LanMass[3]);
	getGAR(gNetCtr.LanGateW);
	printf(" W5500 网关     : %d.%d.%d.%d\r\n", gNetCtr.LanGateW[0],gNetCtr.LanGateW[1],gNetCtr.LanGateW[2],gNetCtr.LanGateW[3]);
}

/**
*@brief		配置W5500的MAC地址
*@param		无
*@return	无
*/
void set_w5500_mac(void)
{
	//memcpy(ConfigMsg.mac, mac, 6);
	setSHAR(Config.Mac);	/**/
	memcpy(DHCP_GET.mac, Config.Mac, 6);
}

/**
*@brief		配置W5500的GPIO接口
*@param		无
*@return	无
*/
void gpio_for_w5500_config(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(WIZ_SPIx_RESET_CLK|WIZ_SPIx_INT_CLK, ENABLE);
	
  /* Enable SPI1 and GPIO clocks */
  /*!< SPI_FLASH_SPI_CS_GPIO, SPI_FLASH_SPI_MOSI_GPIO, 
       SPI_FLASH_SPI_MISO_GPIO, SPI_FLASH_SPI_DETECT_GPIO 
       and SPI_FLASH_SPI_SCK_GPIO Periph clock enable */
  RCC_APB2PeriphClockCmd(WIZ_SPIx_GPIO_CLK|WIZ_SPIx_SCS_CLK, ENABLE);

  /*!< SPI_FLASH_SPI Periph clock enable */
  WIZ_SPIx_CLK_CMD(WIZ_SPIx_CLK|RCC_APB2Periph_AFIO, ENABLE);
 
  
  /*!< Configure SPI_FLASH_SPI pins: SCK */
  GPIO_InitStructure.GPIO_Pin = WIZ_SPIx_SCLK;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(WIZ_SPIx_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI pins: MISO */
  GPIO_InitStructure.GPIO_Pin = WIZ_SPIx_MISO;
  GPIO_Init(WIZ_SPIx_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI pins: MOSI */
  GPIO_InitStructure.GPIO_Pin = WIZ_SPIx_MOSI;
  GPIO_Init(WIZ_SPIx_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure SPI_FLASH_SPI_CS_PIN pin: SPI_FLASH Card CS pin */
  GPIO_InitStructure.GPIO_Pin = WIZ_SPIx_SCS;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(WIZ_SPIx_SCS_PORT, &GPIO_InitStructure);


  /* SPI1 configuration */
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(WIZ_SPIx, &SPI_InitStructure);
  SPI_Cmd(WIZ_SPIx, ENABLE);
	
  /*定义RESET引脚*/
  GPIO_InitStructure.GPIO_Pin = WIZ_RESET;					       /*选择要控制的GPIO引脚*/		 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		     /*设置引脚速率为50MHz */		
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     /*设置引脚模式为通用推挽输出*/	
  GPIO_Init(WIZ_SPIx_RESET_PORT, &GPIO_InitStructure);		 /*调用库函数，初始化GPIO*/
  GPIO_SetBits(WIZ_SPIx_RESET_PORT, WIZ_RESET);		
  /*定义INT引脚*/	
  GPIO_InitStructure.GPIO_Pin = WIZ_INT;						       /*选择要控制的GPIO引脚*/		 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		     /*设置引脚速率为50MHz*/		
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				     /*设置引脚模式为通用推挽模拟上拉输入*/		
  GPIO_Init(WIZ_SPIx_INT_PORT, &GPIO_InitStructure);			 /*调用库函数，初始化GPIO*/

//  /*用网线模式复位wifi模块*/
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;					       /*选择要控制的GPIO引脚*/		 
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		     /*设置引脚速率为50MHz */		
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     /*设置引脚模式为通用推挽输出*/	
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//  GPIO_ResetBits(GPIOA, GPIO_Pin_0);		

}

/**
*@brief		W5500片选信号设置函数
*@param		val: 为“0”表示片选端口为低，为“1”表示片选端口为高
*@return	无
*/
void wiz_cs(uint8_t val)
{
	if (val == LOW) 
	{
	  GPIO_ResetBits(WIZ_SPIx_SCS_PORT, WIZ_SPIx_SCS); 
	}
	else if (val == HIGH)
	{
	  GPIO_SetBits(WIZ_SPIx_SCS_PORT, WIZ_SPIx_SCS); 
	}
}

/**
*@brief		设置W5500的片选端口SCSn为低
*@param		无
*@return	无
*/
void iinchip_csoff(void)
{
	wiz_cs(LOW);
}

/**
*@brief		设置W5500的片选端口SCSn为高
*@param		无
*@return	无
*/
void iinchip_cson(void)
{	
   wiz_cs(HIGH);
}

/**
*@brief		W5500复位设置函数
*@param		无
*@return	无
*/
void reset_w5500(void)
{
	GPIO_ResetBits(WIZ_SPIx_RESET_PORT, WIZ_RESET);
	delay_us(2);  
	GPIO_SetBits(WIZ_SPIx_RESET_PORT, WIZ_RESET);
	delay_ms(1600);
}

uint8_t SPI_SendByte(uint8_t byte)
{
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(WIZ_SPIx, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(WIZ_SPIx, byte);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(WIZ_SPIx, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(WIZ_SPIx);
}

/**
*@brief		STM32 SPI1读写8位数据
*@param		dat：写入的8位数据
*@return	无
*/
uint8_t  IINCHIP_SpiSendData(uint8_t dat)
{
   return(SPI_SendByte(dat));
}

/**
*@brief		写入一个8位数据到W5500
*@param		addrbsb: 写入数据的地址
*@param   data：写入的8位数据
*@return	无
*/
void IINCHIP_WRITE( uint32_t addrbsb,  uint8_t data)
{
   iinchip_csoff();                              		
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);	
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4);  
   IINCHIP_SpiSendData(data);                   
   iinchip_cson();                            
}

/**
*@brief		从W5500读出一个8位数据
*@param		addrbsb: 写入数据的地址
*@param   data：从写入的地址处读取到的8位数据
*@return	无
*/
uint8_t IINCHIP_READ(uint32_t addrbsb)
{
   uint8_t data = 0;
   iinchip_csoff();                            
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8))    ;
   data = IINCHIP_SpiSendData(0x00);            
   iinchip_cson();                               
   return data;    
}

/**
*@brief		向W5500写入len字节数据
*@param		addrbsb: 写入数据的地址
*@param   buf：写入字符串
*@param   len：字符串长度
*@return	len：返回字符串长度
*/
uint16_t wiz_write_buf(uint32_t addrbsb,uint8_t* buf,uint16_t len)
{
   uint16_t idx = 0;
   if(len == 0) printf("Unexpected2 length 0\r\n");
   iinchip_csoff();                               
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4); 
   for(idx = 0; idx < len; idx++)
   {
     IINCHIP_SpiSendData(buf[idx]);
   }
   iinchip_cson();                           
   return len;  
}

/**
*@brief		从W5500读出len字节数据
*@param		addrbsb: 读取数据的地址
*@param 	buf：存放读取数据
*@param		len：字符串长度
*@return	len：返回字符串长度
*/
uint16_t wiz_read_buf(uint32_t addrbsb, uint8_t* buf,uint16_t len)
{
  uint16_t idx = 0;
  if(len == 0)
  {
    printf("Unexpected2 length 0\r\n");
  }
  iinchip_csoff();                                
  IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);
  IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);
  IINCHIP_SpiSendData( (addrbsb & 0x000000F8));    
  for(idx = 0; idx < len; idx++)                   
  {
    buf[idx] = IINCHIP_SpiSendData(0x00);
  }
  iinchip_cson();                                  
  return len;
}

/**
*@brief		STM32定时器2初始化
*@param		无
*@return	无
*/
void timer2_init(void)
{
//	TIM2_NVIC_Configuration();															/* 定时器的中断优先级 */
	TIM2_Configuration();																		/* TIM2 定时配置 */
}

/**
*@brief		dhcp用到的定时器初始化
*@param		无
*@return	无
*/
void dhcp_timer_init(void)
{
  timer2_init();																	
}

/**
*@brief		ntp用到的定时器初始化
*@param		无
*@return	无
*/
void ntp_timer_init(void)
{
  timer2_init();																	
}

/**
*@brief		定时器2中断函数
*@param		无
*@return	无
*/
void timer2_isr(void)
{
  ms++;	
  if(ms>=1000)
  {  
    ms=0;
    dhcp_time++;																					/*DHCP定时加1S*/
	  #ifndef	__NTP_H__
	  ntptimer++;																						/*NTP重试时间加1S*/
	  #endif
  }

}
/**
*@brief		STM32系统软复位函数
*@param		无
*@return	无
*/
void reboot(void)
{
  pFunction Jump_To_Application;
  uint32_t JumpAddress;
  printf(" 系统重启中……\r\n");
  JumpAddress = *(vu32*) (0x00000004);
  Jump_To_Application = (pFunction) JumpAddress;
  Jump_To_Application();
}



void W5500Init(void)
{
	gpio_for_w5500_config();						        /*初始化MCU相关引脚*/
	reset_w5500();											/*硬复位W5500*/
	set_w5500_mac();										/*配置MAC地址*/
	set_w5500_ip();											/*配置IP地址*/
  //ESP07PwrOff();
//	while(dhcp_ok == 0){
//		do_dhcp();
//	}
	//set_w5500_ip();											/*配置IP地址*/
	
	socket_buf_init(txsize, rxsize);		/*初始化8个Socket的发送接收缓存大小*/

}
