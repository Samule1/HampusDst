// main.c
#include "kernel.h"
#include "List.h"

TCB taskA;
TCB * Running;

void task1(void);



int main(void)
{
  
  //init_kernel();
     
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

