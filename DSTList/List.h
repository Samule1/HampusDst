#ifndef _List_h
#define _List_h




typedef struct TCB
{
	void(*PC)();
	unsigned int	*SP;
	//unsigned int	Context[CONTEXT_SIZE];
	//unsigned int	StackSeg[STACK_SIZE];
	unsigned int	DeadLine;
} TCB;


typedef struct l_obj {
	TCB *pTask;
	unsigned int nTCnt;
	//msg *pMessage;
	struct l_obj *pPrevious;
	struct l_obj *pNext;

} listobj;

typedef struct List {
	listobj *head;
	listobj *tail;

}List;

List * ListInitialize();
void insertOnTCBDeadLine(listobj* node, List* ls);
void insertonTCnt(listobj* node, List* ls);
void printList(List *ls);
listobj * nodeAtindex(List* ls, int index); 
listobj * getFirst(List * ls); 

// freeTheList har ännu inte skrivits. 
void freeTheList(List * ls); 
#endif 