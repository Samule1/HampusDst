#include "List.h"
#include "kernel.h"
#include "kernel_hwdep.h"
#include <limits.h>



void idle(void){
  while(1){
  
  }
}

exception init_kernel(void){
  

  //Kanske kommer det mailboxes här? 
  // Running pointer. 
  
  //skapa idle task 
   
  start_up_mode = TRUE; 
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
    set_isr(ISR_OFF);      // stänger av interrupts. 
    SaveContext(); 
    if(first){
      first = FALSE; 
      insertOnTCBDeadLine(node, ready_list);
    }
  }
  return status; 
  
	
} 





