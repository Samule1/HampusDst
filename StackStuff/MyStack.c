#include <stdio.h>
#include <stdlib.h>
#include "MyStack.h"


stack * InitStack(){

	stack s;
	s.size = 0;
	return &s;
}

void Push(int n, stack * s){
	s->data[s->size] = n; 
	s->size++; 

}

int Pop(stack * s){
	int address = s->data[s->size]; 
	s->size--; 
	return address; 

}