#include "List.h"
#include "kernel.h"
#include "kernel_hwdep.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/*
                             REMINDERS
i en subrutin där du slår på och sedan av ett interrupt ska du köra:
int x = set_isr(ISR_OFF), do stuff, set_isr(x)


Skriv in interrupt disable/enable på de ställen som behövs..



*/

/*Globals*/
unsigned int tickcounter;                      // Global tick counter. 
bool start_up_mode;                            // the kernel is in start-up              
TCB * Running; bool running_mode;              // the kernel is in running-mode
List * waiting_list; 
List * timer_list;
List * ready_list;

void idle(void){
  while(1){
   SaveContext(); 
   TimerInt(); 
   LoadContext(); 
  
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
  timer_list   = ListInitialize(); 
  ready_list   = ListInitialize(); 
  
  if(waiting_list == NULL || timer_list == NULL || ready_list == NULL){
   
    free(waiting_list->head);
    free(waiting_list->tail);
    free(waiting_list); 
    
    free(timer_list->head);
    free(timer_list->tail); 
    free(timer_list); 
    
    free(ready_list->head);
    free(ready_list->tail);
    free(ready_list);
    
    return FAIL; 
  }
  
  create_task(&idle, UINT_MAX); 
  
  return OK; 
}

exception create_task(void(*task_body)(), uint deadline){
  
  volatile int first = TRUE; // Denna ska antagligen inte ligga här sen men.. 
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
  task->SP = &task->StackSeg[STACK_SIZE-1]; // Sätter pekaren på sin egen stack. 
  
  node->pTask = task; 
  
  if(start_up_mode){
    insertOnTCBDeadLine(node, ready_list); 
    Running = ready_list->head->pNext->pTask; 
    return status; 
  }
  else{ 
    set_isr(ISR_OFF);      // Behöver inte köra hacket här.. 
    SaveContext(); 
    if(first){
      first = FALSE; 
      insertOnTCBDeadLine(node, ready_list);
      Running = ready_list->head->pNext->pTask;
      LoadContext(); 
    }
  }
  return status; 
  
	
} 

void run(void){
  // !!!initailize interuupt timer?!!! 
  start_up_mode = FALSE; 
  running_mode = TRUE; 
  
  set_isr(ISR_ON); 
  Running = ready_list->head->pNext->pTask; 
  
  
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

exception wait( uint nTicks ){
  volatile bool first = TRUE;
  exception status; 
  listobj * temp; 
  SaveContext(); 
  if(first){
    first = FALSE; 
    
    temp = getFirst(ready_list);
    temp->nTCnt = ticks() + nTicks; 
    insertonTCnt(temp, timer_list);
    Running = ready_list->head->pNext->pTask; 
    LoadContext(); 
  }
  else{
    if(ticks() >= Running->DeadLine){
      status = DEADLINE_REACHED;        
    }
    else{
      status = OK;
    }
    
  }
  return status; 
  
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
  
  volatile bool first = TRUE; 
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
  
  
  while(timer_list->head->pNext->nTCnt <= ticks() && timer_list->head->pNext != timer_list->tail){
    node = getFirst(timer_list); 
    insertOnTCBDeadLine(node, ready_list);
    Running = ready_list->head->pNext->pTask;
  
  }
  
  while(waiting_list->head->pNext->pTask->DeadLine <= ticks()){
    node = getFirst(waiting_list); 
    message = node->pMessage; 
    
    extractThisMsg(message); 
    free(message); 
    insertOnTCBDeadLine(node, ready_list); 
    Running = ready_list->head->pNext->pTask; 
    
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
  volatile bool first = TRUE; 
  listobj * temp; 
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
      Running = ready_list->head->pNext->pTask; // LÄgg till på fler ställen! 
      
    
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
      message->pData = pData;
     
      
      //Inseting the message in the mailbox. 
      insertInMailBox(mBox, message); 
      mBox->nMessages++; 
      mBox->nBlockedMsg++;
      
      
      //Moving from ready to waiting
      temp = getFirst(ready_list); 
      insertOnTCBDeadLine(temp, waiting_list); // Kolla om den ska in på NTCNT
      Running = ready_list->head->pNext->pTask;
      
    }
    
    LoadContext(); 
  }  
  else{
    if(ticks() >= Running->DeadLine){
      //Stäng av interrupt!
      message = ready_list->head->pNext->pMessage; 
      extractThisMsg(message);  // Rimligen är detta det första meddelandet i lådan 
      mBox->nBlockedMsg--;      //Ska vi räkna ner båda här?
      mBox->nMessages--; 
      ready_list->head->pNext->pMessage = NULL; 
      free(message); 
      
      //Toggle interrupt
      return DEADLINE_REACHED; 
      
    }
    else{
      return OK; 
    }
  
  }
}

exception receive_wait( mailbox* mBox, void* pData ){
  //Stäng av interrupt! 
  volatile bool first = TRUE; 
  listobj * temp; 
  msg * message; 
  SaveContext();
  if(first){
    first = FALSE; 
    if(mBox->nMessages > 0){
      
      message = getFirstFromMailBox(mBox); 
      memcpy(pData, message->pData, sizeof(pData)); 
      
      
      if(mBox->nBlockedMsg > 0){
        
        //fixing the mailbox counters. 
        mBox->nBlockedMsg--; 
        mBox->nMessages--; 
        
        //move to ready list 
        temp = freeThis(message->pBlock); 
        insertOnTCBDeadLine(temp, ready_list); 
        
        //time to free the msg struct.. 
        free(message); 
        
        //Running pointer! 
        Running = ready_list->head->pNext->pTask; 
        
      }
      else{
        free(message->pData); 
        free(message);
        mBox->nMessages--; 
      
      } 
    }
    else{
        msg * m = (msg *)calloc(1, sizeof(msg)); 
        if(m == NULL){
          free(m); 
          return FAIL; 
          
        }
        m->pData = pData; 
        m->pBlock = ready_list->head->pNext;
        
        ready_list->head->pNext->pMessage = m; 
        insertInMailBox(mBox, m); 
        
        mBox->nBlockedMsg--; 
        mBox->nMessages--; 
        
        temp = getFirst(ready_list); 
        insertOnTCBDeadLine(temp, waiting_list); 
        
        //Flyttar min running 
        Running = ready_list->head->pNext->pTask; 
        
    }
    LoadContext(); 
  
  }
  else{
    if(ticks() >= Running->DeadLine){
      //Stäng av interrupt
      message = ready_list->head->pNext->pMessage;
      extractThisMsg(message); 
      mBox->nMessages++; 
      mBox->nMessages++; 
      
      //Tror vi kan tabort meddelandet här.
      ready_list->head->pNext->pMessage = NULL; // eller?
      free(message);
      
      return DEADLINE_REACHED; 
    }
    else{
      return OK; 
    
    }
  
  
  }
}
exception send_no_wait( mailbox* mBox, void* pData ){
  //Stäng av interrupt ! 
  volatile bool first = TRUE;
  listobj * temp; 
  msg * message; 
  SaveContext();
  if(first){
    first = FALSE;
    if(mBox->nMessages < 0){
      
      //Copy data..
      message = getFirstFromMailBox(mBox); 
      memcpy(message->pData, pData, sizeof(pData)); 
      
      //Move the waiting task..
      temp = freeThis(message->pBlock); 
      insertOnTCBDeadLine(temp, ready_list); 
      
      //Fix counters and destroy the waiting message 
      message->pBlock->pMessage = NULL; 
      free(message); 
      mBox->nMessages++; 
      mBox->nBlockedMsg++; 
      
      LoadContext(); 
    }
    else{
      msg * m = (msg*)calloc(1, sizeof(msg)); 
      if(m == NULL){
        free(m); 
        return FAIL; 
      }
      m->pBlock = ready_list->head->pNext;
      ready_list->head->pNext->pMessage = m; 
      
      
      //copy data to the message.. 
      m->pData = pData; 
      
      //if the mailbox is full then.. 
      if(mBox->nMessages >= mBox->nMaxMessages){
        //Kopplar bara loss meddelandet så länge..
        mBox->nMessages--; 
        message = getFirstFromMailBox(mBox); 
        message->pBlock->pMessage = NULL; 
        free(message); 
        
        // Känns som vi behöver hantera detta bättre??
        
        
      }
      // Inserting the new message.. 
      insertInMailBox(mBox, m); 
    
    }
  
  
  }
  return OK; 


}

int receive_no_wait( mailbox* mBox, void* pData ){
  //Stäng av interrupt!
  exception status = FAIL; 
  volatile bool first = TRUE; 
  SaveContext(); 
  msg * message; 
  listobj * temp;
  if(first){
    first = FALSE; 
    if(mBox->nMessages > 0){
      
      /*this would mean that a message has indeed been 
      found and we can change the return status*/
      
      status = OK; 
      
      /*Coping the the senders data to the 
      recivers data area...*/
      message = getFirstFromMailBox(mBox); 
      memcpy(pData, message->pData, sizeof(pData));
      
      mBox->nMessages--; 
      
      if(mBox->nBlockedMsg > 0){
        mBox->nBlockedMsg--; 
        
        //Extracting and moving to the readylist..
        temp = freeThis(message->pBlock); 
        insertOnTCBDeadLine(temp, ready_list); 
        
        //Running pointer update.. 
        Running = ready_list->head->pNext->pTask; 
        
        //Destroying the message..? I guess.. 
        message->pBlock->pMessage = NULL; 
        free(message); 
      
      }
      else{
        /*Freeing the senders data area and destroying 
        the message*/
        
        free(message->pData); 
        free(message); 
      
      }
    
    }
    LoadContext(); 
    
  }
  return status; 



}



