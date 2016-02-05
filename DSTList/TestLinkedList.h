/*Hampus Carlsson DST II TEST HEADER
*					
*				TESTAT FÖR:
*	Sortering sortering med avseende TCnt 
*	Sortering med avseende på deadline 
*	Om man kan koppla bort noder.
*	Att en lista som har fått alla sina noder bortkopplade faktiskt är tom.
*	+ ett par undantags fall. 
*
*
*
*
*/


#ifndef _TestLinkedList_h_
#define _TestLinkedList_h_
#include "List.h"
#include <time.h>

//Här hittar du färdiga listor med slumpade noder.
List * randomTwentyTCnt(); 
List * randomTwentyDeadline();

//Testar om listan är tom 
int isListEmpty(List * ls); 


//Testar om objekten är sorterade i stigande ordning.
int testIndexFetch(); 
int testNTCNTOrder(List* ls);
int testPreviousnTCnt(List * ls); 
int testPreviousnDeadline(List * ls);

//Testar get first
//int firsIsFirst(List * ls); 
int testGetWhenListIsone(List * ls);
int sortedAfterGet();

//Testar om listan är korrekt sorterad efter deadline. 
int testDeadlineSorting(List * ls); 

//Testar kan koppla bort en nod och bevara strukturen efter åt. 
int freeNodeIsFree(listobj * node); 
int sizecheckAfterFree(List * ls);

#endif
