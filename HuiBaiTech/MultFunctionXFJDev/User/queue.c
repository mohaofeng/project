#include "bsp_usart1.h"
//#include<stdlib.h>
#include "queue.h"

/***********************************************
Function: Create a empty stack;
************************************************/
PQUEUE CreateLanQueue(PQUEUE Q)
{
	Q->front=0;         //?????
	Q->rear=0;
	Q->qlen=0;
	return Q;       
}


void ClearQ(PQUEUE Q)
{
	Q->front=0;         
	Q->rear=0;
	Q->qlen=0;
}

void TraverseLanQueue(PQUEUE Q)
{
	uint32_t i=Q->front;
	#if QUEUE_DBG
	printf("队列中的元素是:\r\n");
	#endif
	while(i%QLEN_MAX != Q->rear)
	{
		//printf("%d ",Q->dat[i]);
		i++;
	}
	//printf("\r\n");
}

BOOL FullLanQueue(PQUEUE Q)
{
	if(Q->front==(Q->rear+1)%QLEN_MAX)
	{    
		#if QUEUE_DBG
		printf("队列满！\r\n");
		#endif		
		return TRUE;
	}
	else
	{
		#if QUEUE_DBG
		printf("队列空！\r\n");
		#endif		
		return FALSE;
	}
}
BOOL EmptyLanQueue(PQUEUE Q)
{
	if(Q->front == Q->rear) 
	{		
		#if QUEUE_DBG
		printf("队列空！\r\n");
		#endif		
		return TRUE;
	}
	else
	{
		#if QUEUE_DBG
		printf("队列非空！\r\n");
		#endif		
		return FALSE;
	}
}

/***********************************************
Function: Print the stack element;
************************************************/
// BOOL EnQueue(PQUEUE Q,elementType val)
// {
// 	if(FullLanQueue(Q))
// 	{
// 		return FALSE;
// 	}
// 	else
// 	{
// 		Q->dat[Q->rear]=val;
// 		Q->rear=(Q->rear+1)%QLEN_MAX;
// 		Q->qlen++;
// 		#if QUEUE_DBG
// 		printf("数据入队成功val=0x%x\r\n",val);
// 		#endif		
// 		return TRUE;
// 	}
// }
BOOL EnQueue(PQUEUE Q,uint8_t val)
{
	if(Q->front!=(Q->rear+1)%QLEN_MAX)//队列非满
	{    
		Q->dat[Q->rear]=val;
		Q->rear=(Q->rear+1)%QLEN_MAX;
		Q->qlen++;
		#if QUEUE_DBG
		printf("数据入队成功val=0x%x\r\n",val);
		#endif		
		return TRUE;
	}
	else
	{
		#if QUEUE_DBG
		printf("队列满，数据入队失败\r\n");
		#endif		
		return FALSE;
	}
}
/***********************************************
Function: n个元素入队
************************************************/
BOOL EnQueuex(PQUEUE Q,uint8_t *val, uint32_t n)
{
	uint32_t i=0;
	#if QUEUE_DBG
	printf("有%d个元素需要入队\r\n",n);
	#endif			
	for(i=0;i<n;i++)
	{
		if(EnQueue(Q,val[i]) == FALSE)
		{
			return FALSE;
		}
	}
	return TRUE;
}
// BOOL EnQueuex(PQUEUE Q,elementType *val, qsize n)
// {
// 	uint16_t i=0;
// 	#if QUEUE_DBG
// 	printf("有%d个元素需要入队\r\n",n);
// 	#endif			
// 	for(i=0;i<n;i++)
// 	{
// 		EnQueue(Q,val[i]);
// 		{
// 			return FALSE;
// 		}
// 	}
// 	return TRUE;
// }

/***********************************************
Function: 队列剩余长度
************************************************/
uint32_t QueueEmptyLen(PQUEUE Q)
{
	uint32_t i;
	i= Q->front+QLEN_MAX-Q->rear;
//	i= QLEN_MAX-Q->qlen;
	
	#if QUEUE_DBG
	printf("队列剩余空间长度：%d\r\n",i);
	#endif		
	return (i);
}


/***********************************************
Function: 队列数据有效长度
************************************************/
uint32_t QueueDatLen(PQUEUE Q)
{
		//printf("队列长度为%d\r\n",i);
		return (Q->qlen);
}

BOOL DeQueue(PQUEUE Q, uint8_t *val)
{
	if(EmptyLanQueue(Q))
	{
		return FALSE;
	}
	else
	{
		*val=Q->dat[Q->front];
		Q->front=(Q->front+1)%QLEN_MAX;
		Q->qlen--;
		#if QUEUE_DBG
		printf("数据出队成功！*val=0x%x\r\n",*val);
		#endif		
		return TRUE;
	}
}
/***********************************************
Function: n个元素出队
************************************************/
BOOL DeQueuex(PQUEUE Q,uint8_t *val, uint32_t n)
{
	uint32_t i;
	for(i=0;i<n;i++)
	{
		if(DeQueue(Q,&val[i]) == FALSE)
			return FALSE;
	}
	return TRUE;
}
