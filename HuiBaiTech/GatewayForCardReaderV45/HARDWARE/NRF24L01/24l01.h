#ifndef __24L01_H
#define __24L01_H	 		  
#include "sys.h"   
#include "sqstack.h"


#ifdef NRF24L01_C
#define NRF24L01_EXT 
#else
#define NRF24L01_EXT extern
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//NRF24L01寄存器操作命令
#define READ_REG_NRF        0x00  //读配置寄存器,低5位为寄存器地址
#define WRITE_REG_NRF       0x20  //写配置寄存器,低5位为寄存器地址
#define RD_RX_PLOAD     0x61  //读RX有效数据,1~32字节
#define WR_TX_PLOAD     0xA0  //写TX有效数据,1~32字节
#define FLUSH_TX        0xE1  //清除TX FIFO寄存器.发射模式下用
#define FLUSH_RX        0xE2  //清除RX FIFO寄存器.接收模式下用
#define REUSE_TX_PL     0xE3  //重新使用上一包数据,CE为高,数据包被不断发送.
#define NOP             0xFF  //空操作,可以用来读状态寄存器	 
//SPI(NRF24L01)寄存器地址
#define CONFIG          0x00  //配置寄存器地址;bit0:1接收模式,0发射模式;bit1:电选择;bit2:CRC模式;bit3:CRC使能;
                              //bit4:中断MAX_RT(达到最大重发次数中断)使能;bit5:中断TX_DS使能;bit6:中断RX_DR使能
#define EN_AA           0x01  //使能自动应答功能  bit0~5,对应通道0~5
#define EN_RXADDR       0x02  //接收地址允许,bit0~5,对应通道0~5
#define SETUP_AW        0x03  //设置地址宽度(所有数据通道):bit1,0:00,3字节;01,4字节;02,5字节;
#define SETUP_RETR      0x04  //建立自动重发;bit3:0,自动重发计数器;bit7:4,自动重发延时 250*x+86us
#define RF_CH           0x05  //RF通道,bit6:0,工作通道频率;
#define RF_SETUP        0x06  //RF寄存器;bit3:传输速率(0:1Mbps,1:2Mbps);bit2:1,发射功率;bit0:低噪声放大器增益
#define STATUS          0x07  //状态寄存器;bit0:TX FIFO满标志;bit3:1,接收数据通道号(最大:6);bit4,达到最多次重发
                              //bit5:数据发送完成中断;bit6:接收数据中断;
#define MAX_TX  		0x10  //达到最大发送次数中断
#define TX_OK   		0x20  //TX发送完成中断
#define RX_OK   		0x40  //接收到数据中断

#define OBSERVE_TX      0x08  //发送检测寄存器,bit7:4,数据包丢失计数器;bit3:0,重发计数器
#define CD              0x09  //载波检测寄存器,bit0,载波检测;
#define RX_ADDR_P0      0x0A  //数据通道0接收地址,最大长度5个字节,低字节在前
#define RX_ADDR_P1      0x0B  //数据通道1接收地址,最大长度5个字节,低字节在前
#define RX_ADDR_P2      0x0C  //数据通道2接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P3      0x0D  //数据通道3接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P4      0x0E  //数据通道4接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define RX_ADDR_P5      0x0F  //数据通道5接收地址,最低字节可设置,高字节,必须同RX_ADDR_P1[39:8]相等;
#define TX_ADDR         0x10  //发送地址(低字节在前),ShockBurstTM模式下,RX_ADDR_P0与此地址相等
#define RX_PW_P0        0x11  //接收数据通道0有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P1        0x12  //接收数据通道1有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P2        0x13  //接收数据通道2有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P3        0x14  //接收数据通道3有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P4        0x15  //接收数据通道4有效数据宽度(1~32字节),设置为0则非法
#define RX_PW_P5        0x16  //接收数据通道5有效数据宽度(1~32字节),设置为0则非法
#define NRF_FIFO_STATUS 0x17  //FIFO状态寄存器;bit0,RX FIFO寄存器空标志;bit1,RX FIFO满标志;bit2,3,保留
                              //bit4,TX FIFO空标志;bit5,TX FIFO满标志;bit6,1,循环发送上一数据包.0,不循环;
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//24L01操作线
#define NRF24L01_CE   PAout(8) //24L01片选信号
#define NRF24L01_CSN  PBout(12) //SPI片选信号	   
#define NRF24L01_IRQ  PAin(11)  //IRQ主机数据输入
//24L01发送接收数据宽度定义
#define TX_ADR_WIDTH    5   	//5字节的地址宽度
#define RX_ADR_WIDTH    5   	//5字节的地址宽度
#define TX_PLOAD_WIDTH  22  	//32字节的用户数据宽度
#define RX_PLOAD_WIDTH  22  	//32字节的用户数据宽度
	



#define  NET_ID 0X04   //上位机id
/*===========================================================================
															缓冲区状态字定义
============================================================================*/
//状态标记
#define RDER_STAON          0x01			//dx链路存在
#define RDER_DATUP_SVD      0x02			//dx有待上传数据到上位机

#define RDER_NETRCV         0x04			//WX有带下传命令


// /*===========================================================================
// 														DX与WX  2.4G无线通信协议
// ============================================================================*/
// //2.4g 下传命令字
// #define WDW_NOTHIN_DX 	    	0X00						//WX下传空信息到   dx
// #define WDW_DAT_DX 	    			0X01						//WX下传投币信息到DX
// #define WDW_NOMON_DX 	    		0X02						//WX下传余额不足信息到DX
// #define WDW_VALIDIC_DX 	    	0X04						//WX下传非法卡信息到DX
// #define WDW_CONICFG_DX 	    	0X08						//WX下传上币配置信息到DX
// #define WDW_CHKMONY_DX 	    	0X20						//WX下传响应余额查询信息DX
// #define WDW_CHKCFG_DX 	    	0X40						//WX下传余额查询机配置信息到DX

// //2.4g 上传配置字
// #define DUP_LNK_WX 	    			0X02						//dx上传链路信号到wx
// #define DUP_SNDAT_WX 	    		0X04						//dx上传卡号信息到wx
// #define DUP_CONICFG_WX 	    	0X08						//dx上传上币配置信息到wx
// #define DUP_CHKMONY_WX 	    	0X20						//dx上传余额查询信息到wx
// #define DUP_CHKCFG_WX 	    	0X40						//dx上传本机余额机配置成功信息到wx
// #define DUP_COINOK_WX 	    	0X80						//dx上传本次上币成功信息到wx

// /*===========================================================================
// 													wx与上位机通信协议
// ============================================================================*/
// //上位机下传命令字wx
// #define CMPDW_NOTHIN_WX   		0X00				//上位机下传空信息帧   wx
// #define CMPDW_MSG_COIN_WX   	0X01				//上位机下传投币信息到WX
// #define CMPDW_MSG_NOMON_WX   	0X02				//上位机下传余额不足信息到WX
// #define CMPDW_MSG_VALIDIC_WX  0X04				//上位机下传非法卡信息到WX
// #define CMPDW_COINCFG_WX   		0X08				//上位机下传DX上币配置信息到WX
// #define CMPDW_IDCONFIG_WX   	0X10				//上位机下传 WX id配置信息到WX
// #define CMPDW_CHKMONY_WX 	    0X20			  //上位机下传响应DX余额查询。
// #define CMPDW_CHKCFG_WX 	    0X40			  //上位机下传将DX配置为余额查询机。
// #define CMPDW_CHKVER_WX 	    0X80			  //上位机查询网关软硬件版本号


// //上位机下传命令字信息位置定义
// #define DXlST_POSITION  			5    				//子读头id列表信息位置
// #define WXID_POSITION  				4    				//发送源信息位置
// #define CMD_POSITION  				3    				//帧类型位置
// #define RXID_POSITION  				2    				//接收源地址信息
// #define DxLEN_POSITION  			1    				//读头个数信息位置
// #define DxID_POSITION  				1    				//读头个数信息位置

// //wx上传命令字到上位机
// #define WUP_DxSTU_CMP 	    	0X02				//wx 上传 DX... 状态信息到上位机
// #define WUP_ICSN_CMP 	    		0X04				//WX 上传 DX 卡号信息到上位机
// #define WUP_COINCFG_CMP 	    0X08				//WX 上传 DX上币配置信息到上位机
// #define WUP_IDCONFIG_CMP 	    0X10				//WX 上传 WX ID配置信息到上位机
// #define WUP_CHKMONY_CMP 	    0X20			  //wx 上传dx查询余额信息
// #define WUP_CHKCFG_CMP 	    	0X40			  //Wx上传响应上位机配置dx为余额机。
// #define WUP_DWDXOK_CMP 	    	0X80			  //Wx回复上位机投币信息下传成功
// #define WUP_WXVER_CMP 	    	0X01			  //Wx回复上位机软硬件版本信息








/*===========================================================================
															WX所属DX数量定义
============================================================================*/



void NRF24L01_Init(void);//初始化
void NRF24L01_RX_Mode(uint8_t ch);//配置为接收模式
void NRF24L01_TX_Mode(uint8_t ch);//配置为发送模式
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf,u8 len);//写数据区
uint8_t NRF24L01_Read_Buf(uint8_t reg, uint8_t *pBuf,u8 len);//读数据区		  
uint8_t NRF24L01_Read_Reg(uint8_t reg);			//读寄存器
uint8_t NRF24L01_Write_Reg(uint8_t reg, uint8_t value);//写寄存器
uint8_t NRF24L01_Check(void);//检查24L01是否存在
u8 NRF24L01_TxPacket(u8 *txbuf);
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf);//接收一个包的数据
void Nrf24l01Task(void);
uint8_t ChkBufSum(uint8_t *buf,uint8_t len);
#endif











