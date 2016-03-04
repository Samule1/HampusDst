// main.c
#include "kernel.h"
#include "List.h"
#include <assert.h>

TCB taskA;

mailbox *m; 

void task1(void);
void task2(void); 

unsigned int Get_psr(void){return NULL; }
void Set_psr(unsigned int PSR){}

int main(void)
{
  
  init_kernel();
  m = create_mailbox(10, sizeof(char)); 
  create_task(&task1, 100);
  create_task(&task2, 200);
  
  
  run(); 
     
}   
void task1(void)
{
  char x = 5;
  //wait(2); 
  send_wait(m, &x); 
  x++;
  
  terminate(); 
  
}

void task2(void)
{
  char y; 
  receive_wait(m, &y); 
  y++; 
  terminate(); 



}
