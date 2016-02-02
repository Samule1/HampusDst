
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

int firsIsFirst(List * ls){
	listobj * node = ls->head->pNext; 
	listobj * cmp; 
	listobj * fetchedNode; 
	listobj * prevState;
	while (node->pNext != NULL){
		cmp = node->pNext; 
		prevState = ls->head->pNext; 
		fetchedNode = getFirst(ls);
		if (prevState != fetchedNode){
			return 0; 
		}
		 else if (ls->head->pNext != cmp){
			return 0; 

		}
		node = node->pNext; 
	}
	/*Omringar returnen med en emoty check här..*/

	if (isListEmpty(ls) != 1){
		return 0; 
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

List * randomTwentyTCnt(){
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


