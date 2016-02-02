#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "TestLinkedList.h"
#include <assert.h>






void main(void){


	// Lägger in en lite comment.. k
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
	

	assert(testDeadlineSorting(tcblist)); 
	assert(isListEmpty(empty)); 
	assert(testIndexFetch());
	assert(testNTCNTOrder(hmm));
	assert(firsIsFirst(hmm));
	assert(testGetWhenListIsone(sizeIsOne)); 
	assert(sortedAfterGet());
	
	




}