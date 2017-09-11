
#ifndef __RC522_H
#define __RC522_H	 		  
#include "SysConfig.h"

#ifdef RC522_C	
#define RC522_EXT 
#else
#define RC522_EXT extern
#endif
/////////////////////////////////////////////////////////////////////
//MF522命令字
/////////////////////////////////////////////////////////////////////
#define PCD_IDLE              0x00               //取消当前命令
#define PCD_AUTHENT           0x0E               //验证密钥
#define PCD_RECEIVE           0x08               //接收数据
#define PCD_TRANSMIT          0x04               //发送数据
#define PCD_TRANSCEIVE        0x0C               //发送并接收数据
#define PCD_RESETPHASE        0x0F               //复位
#define PCD_CALCCRC           0x03               //CRC计算

/////////////////////////////////////////////////////////////////////
//Mifare_One卡片命令字
/////////////////////////////////////////////////////////////////////
#define PICC_REQIDL           0x26               //寻天线区内未进入休眠状态
#define PICC_REQALL           0x52               //寻天线区内全部卡
#define PICC_ANTICOLL1        0x93               //防冲撞
#define PICC_ANTICOLL2        0x95               //防冲撞
#define PICC_AUTHENT1A        0x60               //验证A密钥
#define PICC_AUTHENT1B        0x61               //验证B密钥
#define PICC_READ             0x30               //读块
#define PICC_WRITE            0xA0               //写块
#define PICC_DECREMENT        0xC0               //扣款
#define PICC_INCREMENT        0xC1               //充值
#define PICC_RESTORE          0xC2               //调块数据到缓冲区
#define PICC_TRANSFER         0xB0               //保存缓冲区中数据
#define PICC_HALT             0x50               //休眠

/////////////////////////////////////////////////////////////////////
//MF522 FIFO长度定义
/////////////////////////////////////////////////////////////////////
#define DEF_FIFO_LENGTH       64                 //FIFO size=64byte
#define MAXRLEN  18

/////////////////////////////////////////////////////////////////////
//MF522寄存器定义
/////////////////////////////////////////////////////////////////////
// PAGE 0
#define     RFU00                 0x00    
#define     CommandReg            0x01    
#define     ComIEnReg             0x02    
#define     DivlEnReg             0x03    
#define     ComIrqReg             0x04    
#define     DivIrqReg             0x05
#define     ErrorReg              0x06    
#define     Status1Reg            0x07    
#define     Status2Reg            0x08    
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     RFU0F                 0x0F
// PAGE 1     
#define     RFU10                 0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     RFU1A                 0x1A
#define     RFU1B                 0x1B
#define     MifareReg             0x1C
#define     RFU1D                 0x1D
#define     RFU1E                 0x1E
#define     SerialSpeedReg        0x1F
// PAGE 2    
#define     RFU20                 0x20  
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     RFU23                 0x23
#define     ModWidthReg           0x24
#define     RFU25                 0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsCfgReg            0x28
#define     ModGsCfgReg           0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
// PAGE 3      
#define     RFU30                 0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39  
#define     TestDAC2Reg           0x3A   
#define     TestADCReg            0x3B   
#define     RFU3C                 0x3C   
#define     RFU3D                 0x3D   
#define     RFU3E                 0x3E   
#define     RFU3F		  		  0x3F

/////////////////////////////////////////////////////////////////////
//和MF522通讯时返回的错误代码
/////////////////////////////////////////////////////////////////////
#define 	MI_OK                 0
#define 	MI_NOTAGERR           (1)
#define 	MI_ERR                (2)
#define 	KEYA_CHK     0x60
#define 	KEYB_CHK     0x61
#define	SHAQU1	0X01
#define	KUAI4	0X04
#define	KUAI7	0X07
#define	REGCARD	0xa1
#define	CONSUME	0xa2
#define READCARD	0xa3
#define ADDMONEY	0xa4

//
//#define  spi_cs 1;
//sbit  spi_ck=P0^6;
//sbit  spi_mosi=P0^7;
//sbit  spi_miso=P4^1;
//sbit  spi_rst=P2^7;









#define RC522_DAT_EMPTY			0    //读卡器序号缓冲区为空，无卡号
#define RC522_DAT_SN			1    //读卡器序号缓冲区为空，无卡号


/*定义SPI3作为W5500的硬件接口*/
#define RC522_SPIx_GPIO_PORT      GPIOB								  	  	/* GPIO端口                     */
#define RC522_SPIx_GPIO_CLK       RCC_APB2Periph_GPIOB	  	  /* GPIO端口时钟                 */
#define RC522_SPIx                SPI2                   	    /* 定义W5500所用的SPI接口       */
#define RC522_SPIx_CLK_CMD        RCC_APB1PeriphClockCmd
#define RC522_SPIx_CLK            RCC_APB1Periph_SPI3    	    /* 定义W5500所用的SPI接口时钟   */
#define RC522_SPIx_SCLK           GPIO_Pin_3						   	  /* 定义W5500的时钟管脚          */
#define RC522_SPIx_MISO           GPIO_Pin_4						   	  /* 定义W5500的MISO管脚          */
#define RC522_SPIx_MOSI           GPIO_Pin_5						   	  /* 定义W5500的MOSI管脚          */


#define RC522_SPIx_SCS          GPIO_Pin_6						 	    /* 定义W5500的片选管脚          */
#define RC522_SPIx_SCS_PORT     GPIOD									 	    /* GPIO端口                     */
#define RC522_SPIx_SCS_CLK      RCC_APB2Periph_GPIOD	 	    /* GPIO端口时钟                 */

#define RC522_RESET             GPIO_Pin_0									/* 定义W5500的RESET管脚         */
#define RC522_SPIx_RESET_PORT   GPIOE									  	  /* GPIO端口                     */
#define RC522_SPIx_RESET_CLK    RCC_APB2Periph_GPIOE	 	    /* GPIO端口时钟                 */

//#define RC522_INT               GPIO_Pin_5									/* 定义W5500的INT管脚           */
//#define RC522_SPIx_INT_PORT     GPIOC									      /* GPIO端口                     */
//#define RC522_SPIx_INT_CLK      RCC_APB2Periph_GPIOE	      /* GPIO端口时钟                 */




/////////////////////////////////////////////////////////////////////
//????
/////////////////////////////////////////////////////////////////////
#define RC522_RST PBout(6) //RC522 RST
#define RC522_CS  PDout(6) //RC522 ????
//#define RC522_IRQ  PAin(11) //RC522 ????

#define SET_SPI_CS  RC522_CS=1;   //PF0 片选信号
#define CLR_SPI_CS  RC522_CS=0;
#define SET_RC522RST  RC522_RST=1;	//PF1复位信号
#define CLR_RC522RST  RC522_RST=0;





//rc522 控制器
typedef struct
{
	uint8_t CT[2];				//卡类型
	uint8_t RFID[16];			//存放RFID 
	uint8_t SN[4]; 				//卡号
	uint8_t KEYA[6];
	uint8_t KEYB[6];
	uint8_t ICScanGas;//ic卡扫描间隔
	uint8_t RC522FlashDly;
	uint8_t GetIcNum;
}__Rc522_ctr;
extern __Rc522_ctr RC522Ctr;	







void InitRc522(void);
void ClearBitMask(unsigned char   reg,unsigned char   mask);
void WriteRawRC(unsigned char   Address, unsigned char   value);
void SetBitMask(unsigned char   reg,unsigned char   mask);
char PcdComMF522(unsigned char   Command, 
                 unsigned char *pIn , 
                 unsigned char   InLenByte,
                 unsigned char *pOut , 
                 unsigned char  *pOutLenBit);
void CalulateCRC(unsigned char *pIn ,unsigned char   len,unsigned char *pOut );
unsigned char ReadRawRC(unsigned char   Address);
void PcdAntennaOn(void);

char PcdReset(void);
char PcdRequest(unsigned char req_code,unsigned char *pTagType);
void PcdAntennaOn(void);
void PcdAntennaOff(void);
char M500PcdConfigISOType(unsigned char type);
char PcdAnticoll(unsigned char *pSnr);
char PcdSelect(unsigned char *pSnr);
char PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr);
char PcdWrite(unsigned char addr,unsigned char *pData);
char PcdRead(unsigned char addr,unsigned char *pData);
char PcdHalt(void);
void Reset_RC522(void);
void RC522Task(void);
void RC522Test(void);
u8 SPI2_ReadWriteByte(u8 TxData);	
//void InitRc522Per(void);

#endif


