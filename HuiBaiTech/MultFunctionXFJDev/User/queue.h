#ifndef __QUEUE_H_
#define __QUEUE_H_
#include "SysConfig.h"


//typedef unsigned char elementType;
typedef unsigned char BOOL;



#define QUEUE_DBG 0

#ifndef true
	#define TRUE 1
#endif
	
#ifndef false
	#define FALSE 0
#endif

//typedef struct queue 
//{
//	elementType *pBase;
//	elementType front;    //?????????
//	elementType rear;    //????????????????
//	int maxsize; //???????????
//}QUEUE,*PQUEUE;


//typedef unsigned char elementType;
//typedef unsigned char BOOL;
//typedef unsigned int uint32_t;

typedef struct queue 
{
	uint32_t qlen;
	uint32_t front;    //?????????
	uint32_t rear;    //????????????????
	uint8_t	dat[QLEN_MAX]; //???????????
}QUEUE,*PQUEUE;

extern QUEUE 	gNetDwDxQueu;										//上位机下传命令缓冲队列
extern QUEUE 	gNetUpCmpQueu;										//设备上传命令缓冲队列
extern QUEUE 	gExtCodeQueu;											//外接二维码读头
extern QUEUE 	gExtTFTLCDQueu;											//外接串口彩屏


PQUEUE CreateLanQueue(PQUEUE Q);
void TraverseQueue(PQUEUE Q);
BOOL FullLanQueue(PQUEUE Q);
BOOL EmptyQueue(PQUEUE Q);
//void EnQueue(PQUEUE Q,elementType val);
BOOL EnQueue(PQUEUE Q,uint8_t val);
BOOL EnQueuex(PQUEUE Q,uint8_t *val, uint32_t n);
uint32_t QueueEmptyLen(PQUEUE Q);
uint32_t QueueDatLen(PQUEUE Q);
BOOL DeQueue(PQUEUE Q, uint8_t *val);
BOOL DeQueuex(PQUEUE Q,uint8_t *val, uint32_t n);
void ClearQ(PQUEUE Q);
#endif


