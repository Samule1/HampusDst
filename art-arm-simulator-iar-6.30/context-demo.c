// main.c
#include "kernel.h"
#include "List.h"
#include <assert.h>

#define EXPECTING_EMPTY 1

TCB taskA;

mailbox *m; 

void Simple_send_wait(void); 
void Simple_recive_wait(void);
void MainTestSW(void);

exception condition_check();
bool list_is_empty(List* ls);
int length_of_list(List* ls); 
exception mailbox_status_compare(mailbox* m, int blocked, int total, bool empty);

unsigned int Get_psr(void){return NULL; }
void Set_psr(unsigned int PSR){}

int main(void)
{
  
  init_kernel();
  

  
  m = create_mailbox(10, sizeof(char)); 
  create_task(&MainTestSW, 10000);  
  run(); 
     
} 

void Simple_send_wait(void){
  char n = 7; 
  send_wait(m, &n);
  terminate(); 
  
  

}
void Simple_recive_wait(void){
  
  exception status = OK; 
  char giveMeSeven; 
  receive_wait(m, &giveMeSeven);
  if(giveMeSeven != 7){
    status = FAIL; 
  }
  terminate(); 

}

void sw_deadline_fail(void)
{
  exception e;
  char y = 5;
  e = send_wait(m, &y);
  if(m->nBlockedMsg != 0){
    y = y;
  }
  if(e != DEADLINE_REACHED){
    y = y;   
  }
  
  terminate(); 
}



void MainTestSW(void){
  
  exception status = OK; 
/*===========================================================================*/ 
  
  //creating task to test a deadline fail.. 
  create_task(&sw_deadline_fail, 10);
  
  wait(9);
  
  //sw_deadline_fail should still be in the waiting list here.. 
  if(waiting_list->head->pNext == waiting_list->tail ){
    status = FAIL; 
  }
  
  wait(100);
  
 /* sw_deadline_fail should have terminated here due to deadline.. 
  * this mean that the waiting list should be empty at this point.  
  */
  
  if(waiting_list->head->pNext != waiting_list->tail ){
    status = FAIL; 
  
  }
  
/*===========================================================================*/
  
  //After we complete a section we check the state of the system.. 
  if(condition_check(2) != OK){
    status = FAIL;
  }
  if(mailbox_status_compare(m, 0, 0, EXPECTING_EMPTY) != OK){
    status = FAIL; 
  }
  
/*===========================================================================*/
  
  create_task(&Simple_send_wait, (ticks()+10)); 
  
  if(length_of_list(waiting_list) != 1){
    status = FAIL; 
  } 
  
  if(mailbox_status_compare(m, 1, 1, !EXPECTING_EMPTY) != OK){
    status = FAIL; 
  }
  
  create_task(&Simple_recive_wait, (ticks()+10)); 
  
  wait(5); 
  
  if(length_of_list(waiting_list) != 0){
    status = FAIL; 
  }
  
/*===========================================================================*/
  
   //After we complete a section we check the state of the system.. 
  if(condition_check(2) != OK){
    status = FAIL;
  }
  if(mailbox_status_compare(m, 0, 0, EXPECTING_EMPTY) != OK){
    status = FAIL; 
  } 
  
  
  terminate(); 

}



exception condition_check(int expected_number_of_ready_objects){
  if(!list_is_empty(waiting_list)){
    return FAIL; 
  }
  if(!list_is_empty(timer_list)){
    return FAIL; 
  }
  
  if(length_of_list(ready_list) != expected_number_of_ready_objects){
    return FAIL; 
  }
  return OK; 

}
bool list_is_empty(List* ls){
  if(ls->head->pNext == ls->tail){
    return TRUE; 
  }
  else{
    return FALSE; 
  }

}

int length_of_list(List* ls){
  
  listobj* temp = ls->head; 
  int length = 0; 
  while(temp->pNext != ls->tail){
    length++; 
    temp = temp->pNext; 
  }
  return length; 


}

exception mailbox_status_compare(mailbox* m, int blocked, int total, bool empty){

  if(m->nBlockedMsg != blocked){
    return FAIL;
  }
  if(m->nMessages != total){
    return FAIL; 
  }
  if(empty && m->pHead->pNext != m->pTail){
    return FAIL; 
  }
  if(!empty && m->pHead->pNext == m->pTail ){
    return FAIL; 
  }
  return OK; 
  

}

