
#ifndef _Stack_h
#define _Stack_h

#define MAX_STACK 10

typedef struct stack {
	int data[MAX_STACK];
	int size; 

}stack;

stack * InitStack(); 
void Push(int n, stack * s);
int Pop(stack * s); 

#endif 