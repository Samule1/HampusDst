#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "TestLinkedList.h"
#include <assert.h>






void main(void){



	List * ls = ListInitialize();
	listobj * temp;

	int nodeCount = 0;
	int nodeCountAgain = 0;
	while (nodeCount != 100){
		listobj * node = (listobj *)calloc(1, sizeof(listobj));

		if (node == NULL){
			break;
		}

		node->nTCnt = 2;
		insertonTCnt(node, ls);
		nodeCount++;
	}

	while (ls->head->pNext->pNext->pNext != ls->tail){
		temp = getFirst(ls);
		free(temp);
	}
	while (nodeCountAgain != 100){
		listobj * node = (listobj *)calloc(1, sizeof(listobj));

		if (node == NULL){
			break;
		}

		node->nTCnt = 2;
		insertonTCnt(node, ls);
		nodeCountAgain++;
	}
	nodeCountAgain++;

	/*
	List * ls = ListInitialize();
	listobj * temp;
	int nodeCount = 0;
	int nodeCountAgain = 0;
	while (nodeCount != 4){
		listobj * node = (listobj *)calloc(1, sizeof(listobj));

		if (node == NULL){
			break;
		}

		node->nTCnt = 2;
		insertonTCnt(node, ls);
		nodeCount++;
	}



	
	List *test = ListInitialize();
	listobj *a = (listobj *)calloc(1, sizeof(listobj));
	int i;
	for (i = 0; i < 100; i++){
		listobj *a = (listobj *)calloc(1, sizeof(listobj));
		a->nTCnt = rand() % 100;
		insertonTCnt(a, test);

	};
	List * sizeIsOne = ListInitialize(); 
	listobj *b = (listobj *)calloc(1, sizeof(listobj));
	insertonTCnt(b,sizeIsOne); 

	List * empty = ListInitialize();
	List * hmm = randomTwentyTCnt(); 
	List * tcblist = randomTwentyDeadline();
	List * what = randomTwentyDeadline();
	List * backDead = randomTwentyDeadline();
	List * backCNT = randomTwentyTCnt();
	List * full = randomTwentyTCnt(); 
	


	assert(testPreviousnTCnt(backCNT));
	assert(testPreviousnDeadline(backDead)); 

	assert(testDeadlineSorting(tcblist)); 
	assert(isListEmpty(empty)); 
	assert(testIndexFetch());
	assert(testNTCNTOrder(hmm));

	assert(testMoveNodes(full, empty));
	
	assert(testGetWhenListIsone(sizeIsOne)); 
	assert(sortedAfterGet());

	
	assert(sizecheckAfterFree(what));
	assert(testDeadlineSorting(what));
	assert(testPreviousnDeadline(what));
	
	
	
	*/



}