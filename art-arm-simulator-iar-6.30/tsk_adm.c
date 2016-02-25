#include "List.h"
#include "kernel.h"
#include "kernel_hwdep.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>

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
  mb->pTail->pPrevious = mb->pHead; 
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
void TimerInt (void)
{
  listobj * node; 
  msg * message; 
  tickcounter++; 
  
  while(timer_list->head->pNext->nTCnt <= tickcounter){
    node = getFirst(timer_list); 
    insertOnTCBDeadLine(node, ready_list);
  
  }
  
  while(waiting_list->head->pNext->pTask->DeadLine <= tickcounter){
    node = getFirst(waiting_list); 
    message = node->pMessage; 
    
    extractThisMsg(message); 
    free(message); 
    insertOnTCBDeadLine(node, ready_list); 
    
  }
}
/*Ser bra ut, inte testad doc.*/
msg * getFirstFromMailBox(mailbox * mb){
  msg * m = mb->pHead->pNext; 
  mb->pHead->pNext = m->pNext; 
  mb->pHead->pNext->pPrevious = mb->pHead; 
  
  m->pNext = NULL; 
  m->pPrevious = NULL; 
  
  return m; 

}
void insertInMailBox(mailbox * mb, msg * m){
  m->pPrevious = mb->pTail->pPrevious; 
  m->pNext = mb->pTail; 
  mb->pTail->pPrevious->pNext = m; 
}


void extractThisMsg(msg * m){
  m->pPrevious->pNext = m->pNext; 
  m->pNext->pPrevious = m->pPrevious;
  
  m->pNext = NULL; 
  m->pPrevious = NULL; 
 
}
/*Flyttar vi running i denna?*/
exception send_wait( mailbox* mBox, void* pData ){
  //Stäng av interrupt! 
  bool first = TRUE; 
  listobj * temp; 
  listobj * callingTask = ready_list->head->pNext; 
  msg * message; 
  SaveContext(); 
  if(first){
    first = FALSE; 
    if(mBox->nMessages < 0){
      memcpy(mBox->pHead->pNext->pData, pData, sizeof(pData)); 
      
      //Remove recivers message from mb 
      message = getFirstFromMailBox(mBox); 
      mBox->nMessages++; 
      mBox->nBlockedMsg++;
      //Remove from waiting list and insert in ready list
      temp = freeThis(message->pBlock); 
      insertOnTCBDeadLine(temp, ready_list); 
      
    
    }
    else{
      //Creating a message
      message = (msg *)calloc(1, sizeof(msg)); 
      
      //Check for fail..
      if(message == NULL){
        free(message); 
        return FAIL; 
      }
      
      message->pBlock = ready_list->head->pNext;
      ready_list->head->pNext->pMessage = message; 
      memcpy(message->pData, pData, sizeof(pData));
      
      //Inseting the message in the mailbox. 
      insertInMailBox(mBox, message); 
      mBox->nMessages++; 
      mBox->nBlockedMsg++;
      
      
      //Moving from ready to waiting
      temp = getFirst(ready_list); 
      insertonTCnt(temp, waiting_list); // Kolla om den ska in på NTCNT
    }
    LoadContext(); 
  }  
  else{
    if((ticks() - callingTask->nTCnt) >= callingTask->pTask->DeadLine){
      //Stäng av interrupt!
      //!gör klart!
      
    }
    else{
      return OK; 
    }
  
  }
}

exception receive_wait( mailbox* mBox, void* pData ){
  //Stäng av interrupt! 
  bool first = TRUE; 
  listobj * temp; 
  listobj * callingTask = ready_list->head->pNext; 
  msg * message; 
  SaveContext();
  if(first){
    first = FALSE; 
    if(mBox->nMessages > 0){
      message = mBox->pHead->pNext; 
      memcpy(pData, message->pData, sizeof(pData)); 
      //Fixing the mailbox 
      extractThisMsg(message);
      if(mBox->nBlockedMsg > 0){
        
        //fixing the mailbox counters. 
        mBox->nBlockedMsg--; 
        mBox->nMessages--; 
        
        //move to ready list 
        temp = freeThis(message->pBlock); 
        insertOnTCBDeadLine(temp, ready_list); 
        
      }
      else{
        free(message->pData); 
        mBox->nMessages--; 
      
      } 
    }
    else{
        /*
          Allocate a Message structure
          Add Message to the Mailbox
          Move receiving task from Readylist to
          Waitinglist
      
        */
    }
  
  }
  




}




