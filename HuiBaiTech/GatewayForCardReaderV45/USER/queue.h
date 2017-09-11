#ifndef __QUEUE_H_
#define __QUEUE_H_
#include "common.h"


typedef unsigned char elementType;
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


typedef unsigned char elementType;
typedef unsigned char BOOL;
typedef unsigned int qsize;

typedef struct queue 
{
	qsize qlen;
	elementType front;    //?????????
	elementType rear;    //????????????????
	elementType dat[QLEN_MAX]; //???????????
}QUEUE,*PQUEUE;


void CreateLanQueue(PQUEUE Q);
void TraverseQueue(PQUEUE Q);
BOOL FullLanQueue(PQUEUE Q);
BOOL EmptyQueue(PQUEUE Q);
BOOL EnQueue(PQUEUE Q,elementType val);
BOOL EnQueuex(PQUEUE Q,elementType *val, qsize n);
qsize QueueEmptyLen(PQUEUE Q);
qsize QueueDatLen(PQUEUE Q);
BOOL DeQueue(PQUEUE Q, elementType *val);
BOOL DeQueuex(PQUEUE Q,elementType *val, qsize n);
void ClearQ(PQUEUE Q);
#endif


