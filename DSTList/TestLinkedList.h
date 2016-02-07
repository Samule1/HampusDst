/*Hampus Carlsson DST II TEST HEADER
*					
*				TESTAT F�R:
*	Sortering sortering med avseende TCnt 
*	Sortering med avseende p� deadline 
*	Om man kan koppla bort noder.
*	Att en lista som har f�tt alla sina noder bortkopplade faktiskt �r tom.
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

//H�r hittar du f�rdiga listor med slumpade noder.
List * randomTwentyTCnt(); 
List * randomTwentyDeadline();

//Returnernar storleken p� en lista. 
int listLength(List * ls); 

//Testar om listan �r tom 
int isListEmpty(List * ls); 


//Testar om objekten �r sorterade i stigande ordning.
int testIndexFetch(); 
int testNTCNTOrder(List* ls);
int testPreviousnTCnt(List * ls); 
int testPreviousnDeadline(List * ls);

//Testar get first
//int firsIsFirst(List * ls); 
int testGetWhenListIsone(List * ls);
int sortedAfterGet();

//Testar om listan �r korrekt sorterad efter deadline. 
int testDeadlineSorting(List * ls); 

//Testar kan koppla bort en nod och bevara strukturen efter �t. 
int freeNodeIsFree(listobj * node); 
int sizecheckAfterFree(List * ls);

//Tar tv� stycken listor, en med 20st objekt en �r tom. Alla objekt flyttas fr�n den fulla listan till den tomma. 
void moveBetweenLists(List * one, List * two);

//Testar flytt mellan listor (anv�nder moveBetweenLists)
int testMoveNodes(List * one, List * two);

#endif
