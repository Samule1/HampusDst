// main.c
#include "kernel.h"
#include "List.h"

TCB taskA;
TCB * Running;

void task1(void);

void TimerInt (void)
{
}

int main(void)
{
  
  List * ls = ListInitialize();
  listobj * temp; 
  
  int nodeCount = 0; 
  int nodeCountAgain = 0; 
     while(1){
        listobj * node = (listobj *)calloc(1, sizeof(listobj)); 
        
        if(node == NULL){
          break; 
        }
        
        node->nTCnt = 2; 
        insertonTCnt(node, ls); 
        nodeCount++;
     }
     
     while(ls->head->pNext != ls->tail){
       temp = getFirst(ls); 
       free(temp); 
     }
     while(1){
        listobj * node = (listobj *)calloc(1, sizeof(listobj)); 
        
        if(node == NULL){
          break; 
        }
        
        node->nTCnt = 2; 
        insertonTCnt(node, ls); 
        nodeCountAgain++; 
     }      
     nodeCountAgain++;
    
     
}   
void task1(void)
{
	register int reg_var = 1;
	volatile int vol_var = 1;

	SaveContext();
	reg_var++;
	vol_var++;

	SaveContext();
	reg_var++;
	vol_var++;

	LoadContext();
}

