#include <stdio.h>
#include <stdlib.h>


#define NOT_IN_STACK (-1)

typedef struct
{
	int buf_id;
	int stack_next;
	int stack_prev;

} StackEntry;

typedef struct
{

	int stackTop;
	int stackBottom;

} BufferStrategyControl;

/* Pointers to shared state */
static StackEntry *LRU_Stack = NULL;
static BufferStrategyControl *StrategyControl = NULL;

void printStack(){
	// Print the stack trace from head to tail
  if (StrategyControl->stackTop == NOT_IN_STACK){
    return;
  }
	StackEntry *current = &LRU_Stack[StrategyControl->stackTop];
	while (current->stack_next != NOT_IN_STACK){
    printf("%d -> ",current->buf_id);
		current = &LRU_Stack[current->stack_next];
  }
  printf("%d -> ",current->buf_id);
	printf("\n\n");
}


void
StrategyUpdateAccessedBuffer(int buf_id, int delete)
{
	if (delete) {          //delete = true, delete the buffer with buf_id from the stack
		StackEntry *current;
		current = &LRU_Stack[buf_id];
		if (StrategyControl->stackTop == buf_id) {             //if buf_id is at the top of the stack
			StrategyControl->stackTop = current->stack_next;
			StackEntry *stackNext;
			stackNext = &LRU_Stack[current->stack_next];
			stackNext->stack_prev = NOT_IN_STACK;
		} else {												//buf_id not at top of stack
			StackEntry *stackPrev;
			stackPrev = &LRU_Stack[current->stack_prev];
			stackPrev->stack_next = current->stack_next;
			if (current->stack_next != NOT_IN_STACK) {
				StackEntry *stackNext;
				stackNext = &LRU_Stack[current->stack_next];
				stackNext->stack_prev = current->stack_prev;
			}
		}
		if (StrategyControl->stackBottom == buf_id) {			//if buf_id is at the bottom of the stack
			StrategyControl->stackBottom = current->stack_prev;
			StackEntry *stackPrev;
			stackPrev = &LRU_Stack[current->stack_prev];
			stackPrev->stack_next = NOT_IN_STACK;
		} else {
			StackEntry *stackNext;
			stackNext = &LRU_Stack[current->stack_next];
			stackNext->stack_prev = current->stack_prev;
			if (current->stack_prev != NOT_IN_STACK) {
				StackEntry *stackPrev;
				stackPrev = &LRU_Stack[current->stack_prev];
				stackPrev->stack_next = current->stack_next;
			}
		}
		current->stack_next = NOT_IN_STACK;
		current->stack_prev = NOT_IN_STACK;
	} else {			   										//delete = false, adjust the position of buffer buf_id
		StackEntry *current;
		current = &LRU_Stack[buf_id];
		if (StrategyControl->stackTop != buf_id) {				//buffer buf_id is not at the top of the stack
			StackEntry *tempStack;
			tempStack = &LRU_Stack[buf_id];

			if (tempStack->stack_next == NOT_IN_STACK && tempStack->stack_prev == NOT_IN_STACK)
			{
			/*
			 * The buffer is not on the stack
			 */
				tempStack->stack_next = StrategyControl->stackTop;
				if (StrategyControl->stackTop != NOT_IN_STACK)
					((StackEntry *)&LRU_Stack[StrategyControl->stackTop])->stack_prev = buf_id;
				else
					StrategyControl->stackBottom = buf_id;
				StrategyControl->stackTop = buf_id;
			}
			else
			{
				/*
			 	* The buffer is already on the stack
			 	* if buffer does not need to remove from the stack,
			 	* we move the buffer to the top of the stack and update
			 	* StrategyControl's fields
			 	*/

				((StackEntry *)&LRU_Stack[StrategyControl->stackTop])->stack_prev = buf_id;
				((StackEntry *)&LRU_Stack[tempStack->stack_prev])->stack_next = tempStack->stack_next;
				if(tempStack->stack_next != NOT_IN_STACK)
				{
					((StackEntry *)&LRU_Stack[tempStack->stack_next])->stack_prev = tempStack->stack_prev;
				}
				// if the interested buffer is at bottom of the stack
				if (StrategyControl->stackBottom == buf_id)
				{
					StrategyControl->stackBottom = tempStack->stack_prev;
				}
				// update the current buffer pointer
				tempStack->stack_prev = NOT_IN_STACK;
				tempStack->stack_next = StrategyControl->stackTop;
				StrategyControl->stackTop = buf_id;
		}

		}
	}
	printStack();
}

void LRUReplacement(){
  printf("start of LRU");
  printf("\n\n");
  StackEntry *replacement_node = &LRU_Stack[StrategyControl->stackBottom];
	while(1)
	{
    printf(" %d prev %d next %d ",replacement_node->buf_id, replacement_node->stack_prev, replacement_node->stack_next);
    printf("\n\n");

		if (replacement_node->buf_id == StrategyControl->stackTop)
		{
      break;
		}
		replacement_node = &LRU_Stack[replacement_node->stack_prev];
	}
	printf("no unpinned buffers");
	printf("\n\n");
}

int main() {
  LRU_Stack = (StackEntry *)malloc(16 * sizeof(StackEntry));
  StrategyControl = (BufferStrategyControl *)malloc(sizeof(BufferStrategyControl));
  StrategyControl->stackTop = NOT_IN_STACK;
	StrategyControl->stackBottom = NOT_IN_STACK;
  //printf("%d, %d", StrategyControl->LRUStackHead, StrategyControl->LRUStackTail);
  for (int i = 0; i < 16; i++) {
    StackEntry *node = &LRU_Stack[i];
    node->buf_id = i;
    node->stack_next = NOT_IN_STACK;
    node->stack_prev = NOT_IN_STACK;
    //printf(" %d prev %d next %d ",node->buf_id, node->prev, node->next);
  }
	StrategyUpdateAccessedBuffer(1, 1);
  StrategyUpdateAccessedBuffer(5, 0);
  StrategyUpdateAccessedBuffer(4, 0);
  StrategyUpdateAccessedBuffer(3, 0);
  StrategyUpdateAccessedBuffer(2, 0);
  StrategyUpdateAccessedBuffer(1, 0);
  StrategyUpdateAccessedBuffer(1, 0);
  StrategyUpdateAccessedBuffer(5, 0);
  StrategyUpdateAccessedBuffer(4, 0);
  StrategyUpdateAccessedBuffer(1, 0);
  StrategyUpdateAccessedBuffer(3, 1);
  StrategyUpdateAccessedBuffer(1, 1);
  StrategyUpdateAccessedBuffer(5, 1);
  StrategyUpdateAccessedBuffer(3, 0);
  StrategyUpdateAccessedBuffer(2, 0);
  StrategyUpdateAccessedBuffer(1, 0);
  LRUReplacement();





  return 0;
}
