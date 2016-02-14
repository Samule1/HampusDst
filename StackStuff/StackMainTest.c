#include "MyStack.h"

void main(void){
	stack *s = InitStack();
	int i;
	for (i = 0; i < 10; i++){
		Push(i, s);
	}
	for (i = 0; i < 10; i++){
		printf("%d", Pop(s)); 
	}
	getch(); 


}