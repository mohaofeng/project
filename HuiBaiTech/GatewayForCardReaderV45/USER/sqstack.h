#ifndef __SQSTACK_H
#define __SQSTACK_H	 

#define STACK_SIZE     50
#define DATLEN     		 20

#define STACK_DBG 0


typedef struct Stack{
    unsigned char top;
    unsigned char array[STACK_SIZE+1][DATLEN];
}Stack;



void Init_Stack(Stack *sp);
unsigned char IsEmpty(Stack *sp);
unsigned char IsFull(Stack *sp);
unsigned char Push(Stack *sp,unsigned char *bf,unsigned char len);
unsigned char Pop(Stack *sp,unsigned char *bf,unsigned char len);
unsigned char StRead(Stack *sp,unsigned char *bf,unsigned char len);
//Stack S;
#endif


