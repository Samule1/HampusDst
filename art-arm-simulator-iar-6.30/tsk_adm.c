#include "List.h"
#include "kernel.h"
#include "kernel_hwdep.h"
#include <limits.h>

/*
                             REMINDERS
i en subrutin där du slår på och sedan av ett interrupt ska du köra:
int x = set_isr(ISR_OFF), do stuff, set_isr(x)






*/



void idle(void){
  while(1){
  
  }
}

exception init_kernel(void){
  

  //Kanske kommer det mailboxes här? 
  // Running pointer. 
  
 
   
  start_up_mode = TRUE; 
  running_mode = FALSE; 
  tickcounter = 0; // Done manually for now.  
  
  //Creating the three fundamental listst. 
  waiting_list = ListInitialize(); 
  timer_list = ListInitialize(); 
  ready_list = ListInitialize(); 
  
  if(waiting_list == NULL || timer_list == NULL || ready_list == NULL){
    free(waiting_list); 
    free(timer_list); 
    free(ready_list); 
    return FAIL; 
  }
  
  create_task(&idle, UINT_MAX); 
  
  return OK; 
}

exception create_task(void(*task_body)(), uint deadline){
  
  int first = TRUE; // Denna ska antagligen inte ligga här sen men.. 
  exception status = OK; 
  listobj * node = (listobj *)calloc(1, sizeof(listobj)); 
  if(node == NULL){
    status = FAIL; 
    free(node); 
  }
  
  TCB * task = (TCB*)calloc(1, (sizeof(TCB)));
  if(task == NULL){
    status = FAIL; 
    free(node); 
    free(task);
  }
  
  task->PC = task_body; 
  task->DeadLine = deadline; 
  task->SP = task->StackSeg; // Sätter pekaren på sin egen stack. 
  
  node->pTask = task; 
  
  if(start_up_mode){
    insertOnTCBDeadLine(node, ready_list); 
    return status; 
  }
  else{ 
    set_isr(ISR_OFF);      // Behöver inte köra hacket här.. 
    SaveContext(); 
    if(first){
      first = FALSE; 
      insertOnTCBDeadLine(node, ready_list);
    }
  }
  return status; 
  
	
} 

void run(void){
  // !!!initailize interuupt timer?!!! 
  start_up_mode = FALSE; 
  running_mode = TRUE; 
  
  set_isr(ISR_ON); 
  
  LoadContext(); 

}

/*Här anatar jag att vi alltid termininerar från readylist och vi kommer ju alltid att 
  terminera "the running task" så freeFirst ska kunna funka ganska bra här.. 
*/
void terminate(void){
  //Här kan man tänka sig en check för idle.. 
  listobj * node = getFirst(ready_list);
  
  //OM mailbox är kopplad hit så bör vi rimligen köra en free på den också...  
  free(node->pTask); 
  free(node); 
  
  Running = ready_list->head->pNext->pTask; 
  LoadContext(); 
  

}


mailbox* create_mailbox( uint nMessages, uint nDataSize ){
  
  mailbox * mb = (mailbox *)calloc(1, sizeof(mailbox)); 
  if(mb == NULL){
    free(mb); 
    return NULL; 
  }
  mb->nDataSize = nDataSize; 
  mb->nMaxMessages = nMessages; 
  
  mb->pHead = (msg *)calloc(1, sizeof(msg)); 
  mb->pTail = (msg *)calloc(1, sizeof(msg)); 
  if(mb->pHead == NULL || mb->pTail == NULL){
     free(mb->pHead); 
     free(mb->pTail); 
     free(mb); 
     return NULL; 
  }
  
  mb->pHead->pNext = mb->pTail; 
  return mb; 

}

exception remove_mailbox(mailbox* mBox){
  
  //Kollar om den är tom
  if(mBox->pHead->pNext == mBox->pTail){
    free(mBox->pHead); 
    free(mBox->pTail); 
    free(mBox); 
    return OK; 
  
  }
  else{
    return NOT_EMPTY; 
  }
  


}
//TODO
exception wait( uint nTicks ){
    return NULL; 
}

void set_ticks( uint no_of_ticks ){
  tickcounter = no_of_ticks; 
}

uint ticks( void ){
  
  int count = tickcounter; 
  return count; 
}

uint deadline( void ){
  return Running->DeadLine; 

}
//Kolla igenom detta, tror du är lite trött.. 
void set_deadline( uint nDeadline ){
  set_isr(ISR_OFF);
  
  bool first = TRUE; 
  listobj * node;  
  
  SaveContext();
  
  if(first){
    first = FALSE; 
    node = getFirst(ready_list);
    node->pTask->DeadLine = nDeadline; 
    insertOnTCBDeadLine(node, ready_list); 
    Running = ready_list->head->pNext->pTask; 
    LoadContext(); 
      
    
  }
  

}



