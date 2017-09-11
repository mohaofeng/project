#ifndef _UTILITY_H
#define _UTILITY_H
#include "stdio.h"
#include <stdint.h>
//#include "types.h"
#include "stm32f10x.h"







//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

 #define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
 #define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

 #define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
 #define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

 #define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
 #define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

 #define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
 #define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入



void systick_init (uint8_t SYSCLK);
void TIM3_Int_Init(u16 arr,u16 psc);




void delay_s( uint32_t time_s );
void delay_us(uint32_t time_us);
void delay_ms(uint32_t time_ms);


uint16_t atoi16(char* str,uint16_t base); 			/* Convert a string to integer number */
uint32_t atoi32(char* str,uint16_t base); 			/* Convert a string to integer number */
void itoa(uint16_t n,uint8_t* str, uint8_t len);
int validatoi(char* str, int base, int* ret); 		/* Verify character string and Convert it to (hexa-)decimal. */
char c2d(uint8_t c); 					/* Convert a character to HEX */

uint16_t swaps(uint16_t i);
uint32_t swapl(uint32_t l);

void replacetochar(char * str, char oldchar, char newchar);

void mid(char* src, char* s1, char* s2, char* sub);
void inet_addr_(unsigned char* addr,unsigned char *ip);


char* inet_ntoa(unsigned long addr);			/* Convert 32bit Address into Dotted Decimal Format */
char* inet_ntoa_pad(unsigned long addr);

uint32_t inet_addr(unsigned char* addr);		/* Converts a string containing an (Ipv4) Internet Protocol decimal dotted address into a 32bit address */

char verify_ip_address(char* src, uint8_t * ip);/* Verify dotted notation IP address string */

uint16_t htons( unsigned short hostshort);	/* htons function converts a unsigned short from host to TCP/IP network byte order (which is big-endian).*/

uint32_t htonl(uint32_t hostlong);		/* htonl function converts a unsigned long from host to TCP/IP network byte order (which is big-endian). */

uint32_t ntohs(unsigned short netshort);		/* ntohs function converts a unsigned short from TCP/IP network byte order to host byte order (which is little-endian on Intel processors). */

uint32_t ntohl(unsigned long netlong);		/* ntohl function converts a u_long from TCP/IP network order to host byte order (which is little-endian on Intel processors). */

uint16_t checksum(unsigned char * src, unsigned int len);		/* Calculate checksum of a stream */

uint8_t check_dest_in_local(uint32_t destip);			/* Check Destination in local or remote */


#endif
