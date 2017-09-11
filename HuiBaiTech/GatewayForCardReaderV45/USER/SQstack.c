#include "sqstack.h"
#include "usart.h"	 




void Init_Stack(Stack *sp){
    sp->top = 0;
}
unsigned char IsEmpty(Stack *sp){
    if(sp->top==0)
			return (1);
    else 
			return (0);
}
unsigned char IsFull(Stack *sp){
    if(sp->top== STACK_SIZE)
			return (1);
    else 
			return (0);
}

unsigned char Push(Stack *sp,unsigned char *bf,unsigned char len){
		unsigned char i;
		if(((sp->top)+1) > STACK_SIZE){
// 			#if STACK_DBG
// 			printf("stack overflow!\r\n");
// 			#endif
			return(0);
		}
		else{
			sp->top++;
			for(i=0;i<len;i++){
				sp->array[sp->top][i] = bf[i];
			}
			#if STACK_DBG 
			printf("pop sp->top=%d dat>>:",sp->top); 
			for(i = 0;i<len;i++){
				printf("0x%x,",sp->array[sp->top][i]);
			}
			printf("\r\n");
			#endif  
			return (1);
		}
}

unsigned char Pop(Stack *sp,unsigned char *bf,unsigned char len){
		unsigned char i;
		if((sp->top) == 0){
// 			#if STACK_DBG
// 			printf("stack empyt!\r\n");
// 			#endif
			return(0);
		}
		else{
			for(i=0;i<len;i++){
				bf[i] = sp->array[sp->top][i];
			}
			#if STACK_DBG 
			printf("pop sp->top=%d dat>>:",sp->top); 
			for(i = 0;i<len;i++){
				printf("0x%x,",bf[i]);
			}
			printf("\r\n");
			#endif  
			sp->top--;
			#if STACK_DBG
			printf("pop ok!sp->top=%d\r\n",sp->top);
			#endif
			return (1);
		}
}

unsigned char StRead(Stack *sp,unsigned char *bf,unsigned char len){
		unsigned char i;
		if((sp->top) == 0){
// 			#if STACK_DBG
// 			printf("stack empyt!\r\n");
// 			#endif
			return(0);
		}
		else{
			for(i=0;i<len;i++){
				bf[i] = sp->array[sp->top][i];
			}
			return (1);
		}
}


