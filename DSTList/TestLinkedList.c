
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "TestLinkedList.h"

int sortedAfterGet(){
	List * ls = randomTwentyDeadline();
	getFirst(ls); 
	if (testDeadlineSorting(ls) != 1){
		return 0;
	}
	else{
		return 1; 
	}
}


int testIndexFetch(){
	List * test = ListInitialize();
	listobj * nodeToInsert = (listobj*)calloc(1, sizeof(listobj));
	listobj * compareNode;
	int i;
	for (i = 0; i < 10; i++){
		nodeToInsert = (listobj*)calloc(1, sizeof(listobj));
		nodeToInsert->nTCnt = i; 
		insertonTCnt(nodeToInsert, test);
		
	}
	for (i = 0; i < 10; i++){
		compareNode = nodeAtindex(test, i); 
		if (compareNode->nTCnt != i){
			return 0; 
		
		}

	}
	return 1; 


}

int testNTCNTOrder(List* ls){
	
	
	int i;
	listobj *node1; 
	listobj *node2; 
	//Fill the list with random numbers. 
	for (i = 0; i < 100; i++){
		listobj *a = (listobj *)calloc(1, sizeof(listobj));
		a->nTCnt = rand() % 100;
		insertonTCnt(a, ls);
	}
	for (i = 1; i < 100; i++){
		node1 = nodeAtindex(ls, i);
		node2 = nodeAtindex(ls, i-1);
		if (!(node1->nTCnt >= node2->nTCnt)){

			return 0;
		}

	
	}

	return 1; 
}


int testGetWhenListIsone(List * ls){
	listobj * node = getFirst(ls);
	return isListEmpty(ls); 

}

int isListEmpty(List * ls){
	if (ls->head->pNext != ls->tail || ls->tail->pPrevious != ls->head){

		return 0;
	}
	else{
		return 1;
	}

}


int testDeadlineSorting(List * ls){
	listobj * node = ls->head->pNext->pNext;
	listobj * prev = node->pPrevious;
	while (node->pNext != NULL){
		if (node->pTask->DeadLine < prev->pTask->DeadLine){
			return 0; 
		}
		prev = node; 
		node = node->pNext; 
	}

	return 1; 


}
//Byter från 20 till 6 för att kolla. 
List * randomTwentyTCnt(){
	int i;
	int nRandomTCnt;
	int nRandomTCBDeadline;
	listobj * nodes[6];
	listobj * node;
	TCB * tcb;
	List * ls;
	srand(time(NULL));
	for (i = 0; i < 6; i++){
		node = (listobj *)calloc(1, sizeof(listobj));
		tcb = (TCB *)calloc(1, sizeof(TCB));
		nRandomTCnt = (rand() % 20) + 1;
		nRandomTCBDeadline = (rand() % 20) + 1;
		node->pTask = tcb;
		node->nTCnt = nRandomTCnt;
		tcb->DeadLine = nRandomTCBDeadline;
		nodes[i] = node;


	}
	ls = ListInitialize();

	for (i = 0; i < 6; i++){
		insertonTCnt(nodes[i], ls);
	}
	return ls;



};

List * randomTwentyDeadline(){
	int i;
	int nRandomTCnt;
	int nRandomTCBDeadline;
	listobj * nodes[20];
	listobj * node;
	TCB * tcb;
	List * ls;
	srand(time(NULL));
	for (i = 0; i < 20; i++){
		node = (listobj *)calloc(1, sizeof(listobj));
		tcb = (TCB *)calloc(1, sizeof(TCB));
		nRandomTCnt = (rand() % 20) + 1;
		nRandomTCBDeadline = (rand() % 20) + 1;
		node->pTask = tcb;
		node->nTCnt = nRandomTCnt;
		tcb->DeadLine = nRandomTCBDeadline;
		nodes[i] = node;


	}
	ls = ListInitialize();

	for (i = 0; i < 20; i++){
		insertOnTCBDeadLine(nodes[i], ls);
	}
	return ls;



};

int freeNodeIsFree(listobj * node){
	freeThis(node);
	if (node->pNext != NULL || node->pPrevious != NULL){
		return 0;

	}
	else{

		return 1;
	}

}



int sizecheckAfterFree(List * ls){
	
	int nodeIndex = (rand() % 10) + 1;
	listobj * node = nodeAtindex(ls, nodeIndex);
	int firstCount = 0;
	int afterCount = 0; 
	listobj * temp = ls->head->pNext;

	while (temp->pNext != NULL){
		firstCount++; 
		temp = temp->pNext;
	}

	freeThis(node); 
	temp = ls->head->pNext; 


	while (temp->pNext != NULL){
		afterCount++; 
		temp = temp->pNext;
	}

	if (afterCount != (firstCount - 1)){
		return 0;
	}
	else {
		return 1; 
	}
	

}
int testPreviousnTCnt(List * ls){
	listobj * node = ls->tail->pPrevious; 
	while (node->pPrevious != NULL){
		if (node->nTCnt < node->pPrevious->nTCnt){
			return 0; 
		}
		node = node->pPrevious; 

	}
	return 1; 

}

int testPreviousnDeadline(List * ls){
	listobj * node = ls->tail->pPrevious;
	int s = 0;
	while (node->pPrevious != ls->head->pNext){
	
		if (node->pTask->DeadLine < node->pPrevious->pTask->DeadLine){

			return 0;
		}
		if (node == ls->head->pPrevious){
			break;
		}
		node = node->pPrevious; 
	}
	return 1; 

}

