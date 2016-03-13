// main.c
#include "kernel.h"
#include "List.h"
#include <assert.h>

#define EXPECTING_EMPTY 1

TCB taskA;

mailbox *m; 
mailbox *fullbox; 

void Simple_send_wait(void); 
void Simple_recive_wait(void);
void Three_time_recive_wait(void); 
void sw_deadline_fail(void);
void rw_deadline_fail(void);
void wait_til_break_dl10(void);
void deadline_change_receive(void); 
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

void deadline_change_receive(void){
  exception status = OK; 
  char giveMeSeven; 
  create_task(&Simple_send_wait, (deadline()+1));
  set_deadline((deadline()+5));
  
  receive_no_wait(m, &giveMeSeven); 
  if(giveMeSeven != 7){
    status = FAIL; 
  }
  
  terminate(); 
  

}

void wait_til_break_dl10(void){
  exception e;
  exception status = OK; 
  e = wait(11); 
  
  if(e != DEADLINE_REACHED){
    
      status = FAIL;
  }
  
  terminate(); 

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


void rw_deadline_fail(void){
  exception e; 
  char x; 
  e = receive_wait(m, &x); 
  if(e != DEADLINE_REACHED){
    x = x; 
  }
  terminate(); 

}

void Three_time_recive_wait(void){
  
  exception status; 
  char a; 
  char b; 
  char c; 
  
  receive_wait(m, &a);
  if(a != 1){
    status = FAIL;
  }
  receive_wait(m, &b); 
  if(b != 2){
    status = FAIL;
  }
  receive_wait(m, &c);
  if(c != 3){
    status = FAIL;
  }
  
  terminate(); 
}

//för compile


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
  //This is the simple send one, wait, receive 
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
  
/*===========================================================================*/
  //Let's do some fast messageing, shall we?
  
  char x,y,z;
  create_task(&Three_time_recive_wait, (ticks()+10)); 
  
  x = 1; 
  y = 2; 
  z = 3; 
  
  send_no_wait(m, &x);
  send_no_wait(m, &y);
  send_no_wait(m, &z);
  
  if(x == NULL){
    status = FAIL; 
  }
  
  if(y == NULL){
    status = FAIL; 
  }
  
  if(z == NULL){
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
  //Let's do some rapid messaging between tasks with send- receive_no_wait 
  char a,b,c,d,e,A,B,C,D,E;
  
  a = 1; 
  b = 2; 
  c = 3; 
  d = 4; 
  e = 5; 
  
  send_no_wait(m, &a);
  send_no_wait(m, &b);
  send_no_wait(m, &c);
  send_no_wait(m, &d);
  send_no_wait(m, &e); 
  
  if(mailbox_status_compare(m, 0, 5, !EXPECTING_EMPTY) != OK){
    status = FAIL; 
  } 
    
  receive_no_wait(m, &A);
  
  if(A != 1){
    status = FAIL; 
  }
  receive_no_wait(m, &B);
  
  if(B != 2){
    status = FAIL; 
  }
  receive_no_wait(m, &C);
  if(C != 3){
    status = FAIL; 
  }
  receive_no_wait(m, &D);
  
  if(D != 4){
    status = FAIL; 
  }
  receive_no_wait(m, &E);
  
  if(E != 5){
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
  // Doing some receive no wait with empty mailbox 
  
  //just checking the state of this local one.  
  if(a != 1){
    status = FAIL; 
  }
  
  //Nothing should happen here.. 
  receive_no_wait(m, &a);
/*===========================================================================*/
  
  //After we complete a section we check the state of the system.. 
  if(condition_check(2) != OK){
    status = FAIL;
  }
  if(mailbox_status_compare(m, 0, 0, EXPECTING_EMPTY) != OK){
    status = FAIL; 
  } 
  
/*===========================================================================*/
  //receive_no_wait is now to be tested on send_wait msg. 
  
  //Creating the send_wait task.
  create_task(&Simple_send_wait, (ticks()+10));
  
  //Now let's fetch that seven 
  receive_no_wait(m, &a); 
  
  if(a != 7){
    status = FAIL; 
  }
  
  //reseting the a to one.
  a = 1; 

/*===========================================================================*/
  
  //After we complete a section we check the state of the system.. 
  if(condition_check(2) != OK){
    status = FAIL;
  }
  if(mailbox_status_compare(m, 0, 0, EXPECTING_EMPTY) != OK){
    status = FAIL; 
  } 
  
/*===========================================================================*/
  // Testing case: recieve wait deadline fail. 
  create_task(&rw_deadline_fail, (ticks() + 3)); 
  
  if(length_of_list(waiting_list) != 1){
    status = FAIL; 
  }
  
  wait(4); 
  
  
/*===========================================================================*/

  //After we complete a section we check the state of the system.. 
  if(condition_check(2) != OK){
    status = FAIL;
  }
  if(mailbox_status_compare(m, 0, 0, EXPECTING_EMPTY) != OK){
    status = FAIL; 
  } 
  
/*===========================================================================*/  
  //send no wait -> picked up by receive_wait
  
  create_task(&Simple_recive_wait, (ticks() + 3)); 
  a = 7; 
  send_no_wait(m, &a); 
  
/*===========================================================================*/
  //After we complete a section we check the state of the system.. 
  if(condition_check(2) != OK){
    status = FAIL;
  }
  if(mailbox_status_compare(m, 0, 0, EXPECTING_EMPTY) != OK){
    status = FAIL; 
  } 
  
/*===========================================================================*/
    //Putting a task into the timing list and letting it expire.. 
  create_task(&wait_til_break_dl10, (ticks() + 10)); 
  
  //Putting main thread to sleep here so it wont terminate first. 
  wait(15); 
  
/*===========================================================================*/
  //After we complete a section we check the state of the system.. 
  if(condition_check(2) != OK){
    status = FAIL;
  }
  if(mailbox_status_compare(m, 0, 0, EXPECTING_EMPTY) != OK){
    status = FAIL; 
  } 
  
/*===========================================================================*/
  //changing deadlines in run time, two tasks are created, the first one 
  //with the lowest deadline will reschedule it self and next time it is running
  //a receive no wait will be performed.
  create_task(&deadline_change_receive, (ticks() + 10)); 
  
  
  
/*===========================================================================*/
  //After we complete a section we check the state of the system.. 
  if(condition_check(2) != OK){
    status = FAIL;
  }
  if(mailbox_status_compare(m, 0, 0, EXPECTING_EMPTY) != OK){
    status = FAIL; 
  } 
  
/*===========================================================================*/
  //Testing adding message to a full mailbox. 
  
  //A new mailbox, just for this test. 
  fullbox = create_mailbox(3, sizeof(char)); 
  
  //Check the status of the new mailbox. 
  if(mailbox_status_compare(fullbox, 0, 0, EXPECTING_EMPTY) != OK){
    status = FAIL; 
  } 
  
  a = 1; 
  b = 2; 
  c = 3;
  d = 4; 
  
  A = B = C = 0; 
  
  send_no_wait(fullbox, &a);
  send_no_wait(fullbox, &b);
  send_no_wait(fullbox, &c);
  
  //Checking the mailbox state. 
  if(mailbox_status_compare(fullbox, 0, 3, !EXPECTING_EMPTY) != OK){
    status = FAIL; 
  } 
  
  //Overfull mailbox after this send action. 
  send_no_wait(fullbox, &d);
  
  //Checking the mailbox state. 
  if(mailbox_status_compare(fullbox, 0, 3, !EXPECTING_EMPTY) != OK){
    status = FAIL; 
  }
  
  //fetching the messages. 
  receive_no_wait(fullbox, &A);
  receive_no_wait(fullbox, &B);
  receive_no_wait(fullbox, &C);
  
  if(A != 2){
    status = FAIL; 
  }
  if(B != 3){
    status = FAIL; 
  }
  if(C != 4){
    status = FAIL; 
  }
  
  //Final check of the mailbox state. 
  if(mailbox_status_compare(fullbox, 0, 0, EXPECTING_EMPTY) != OK){
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
  
  
//Add interrupts.. 
//My calloc 
  
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

