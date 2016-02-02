#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "List.h"

//Hur tänker vi här? Kan ju inte gärna ligga en free här?
listobj * getFirst(List * ls){

	listobj * node = ls->head->pNext;
	node->pNext->pPrevious = ls->head; 
	ls->head->pNext = node->pNext; 
	return node; 
}

listobj * nodeAtindex(List* ls, int index){
	listobj * current = (listobj*)calloc(1, sizeof(listobj));
	current = ls->head->pNext; 
	int i = 0; 
	while (current->pNext != NULL){ 
		if (i == index){
			return  current; 
		}
		current = current->pNext; 
		i++; 
	
	}


}

List * ListInitialize(){
	List *ptr = (List *)calloc(1,sizeof(List)); 
	//listobj *node = (listobj *)calloc(1, sizeof(listobj));
	
	ptr->head = (listobj *)calloc(1, sizeof(listobj));;
	ptr->tail = (listobj *)calloc(1, sizeof(listobj));;
	ptr->head->pNext = ptr->tail; 
	ptr->tail->pPrevious = ptr->head; 

	

	return ptr; 


}

void insertOnTCBDeadLine(listobj* node, List* ls){
	
	/*if the list is empty*/
	if (ls->head->pNext == NULL){
		ls->head->pNext = node;
		ls->tail->pPrevious = node;
		node->pNext = ls->tail;
		node->pPrevious = ls->head;
		return;

	};


	listobj *temp = ls->head->pNext;

	while (temp->pNext != NULL){
		if (node->pTask->DeadLine <= temp->pTask->DeadLine){
			temp->pPrevious->pNext = node;
			node->pPrevious = temp->pPrevious;
			temp->pPrevious = node;
			node->pNext = temp;

			return;
		};

		temp = temp->pNext;

	}

	/*If the new node is to be put at the end of the list.*/
	node->pPrevious = temp->pPrevious;
	node->pNext = temp;
	temp->pPrevious->pNext = node;
	temp->pPrevious = node;


}

void insertonTCnt(listobj* node, List* ls){
	
	/*if the list is empty*/
	if (ls->head->pNext == NULL){
		ls->head->pNext = node; 
		ls->tail->pPrevious = node; 
		node->pNext = ls->tail;
		node->pPrevious = ls->head; 
		return;
	
	}; 
	
	
	listobj *temp = ls->head->pNext;
	
	while (temp->pNext != NULL){
		if (node->nTCnt <= temp->nTCnt){
			temp->pPrevious->pNext = node;
			node->pPrevious = temp->pPrevious;
			temp->pPrevious = node; 
			node->pNext = temp; 
			
			return; 
		};

		temp = temp->pNext; 
	
	}
	/*If the new node is to be put at the end of the list.*/
	node->pPrevious = temp->pPrevious;
	node->pNext = temp; 
	temp->pPrevious->pNext = node;
	temp->pPrevious = node;
	

	
}

void printList(List *ls){
	listobj *temp = ls->head->pNext; 
	while (temp->pNext != NULL){
		printf("%i\n", temp->nTCnt);
		temp = temp->pNext;
	};



}




