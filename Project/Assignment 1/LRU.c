#include <stdio.h>
#include <stdlib.h>


#define END_OF_STACK (-1)
#define NOT_IN_STACK (-2)

typedef struct DLLNode
{
	int buf_id;
	int next;
	int prev;

} DLLNode;

typedef struct
{

	int LRUStackHead;
	int LRUStackTail;

} BufferStrategyControl;

/* Pointers to shared state */
static DLLNode *LRUStack = NULL;
static BufferStrategyControl *StrategyControl = NULL;

void printStack(){
	// Print the stack trace from head to tail
  if (StrategyControl->LRUStackHead == END_OF_STACK){
    return;
  }
	DLLNode *current = &LRUStack[StrategyControl->LRUStackHead];
	while (current->next != END_OF_STACK){
    printf("%d -> ",current->buf_id);
		current = &LRUStack[current->next];
  }
  printf("%d -> ",current->buf_id);
	printf("\n\n");
}


void StrategyUpdateAccessedBuffer(int buf_id, int delete)
{
	DLLNode *current_node = &LRUStack[buf_id];
  printf("%d, %d", StrategyControl->LRUStackHead, StrategyControl->LRUStackTail);
  printf("\n\n");


	if (delete) /* case 4 */
	{
    printf("delete %d prev %d next %d ",current_node->buf_id, current_node->prev, current_node->next);
    printf("\n\n");
		if (current_node->next == NOT_IN_STACK && current_node->prev == NOT_IN_STACK) /*Not in LRUStack*/
		{
			return;
		}
		else if (StrategyControl->LRUStackHead == buf_id && StrategyControl->LRUStackTail == buf_id) /*Head = Tail => 1 Node on LRUStack*/
		{
			StrategyControl->LRUStackHead = END_OF_STACK;
			StrategyControl->LRUStackTail = END_OF_STACK;
		}
		else if (StrategyControl->LRUStackHead == buf_id) /* remove Head */
		{
			StrategyControl->LRUStackHead = current_node->next;
			DLLNode *next_node = &LRUStack[current_node->next];
			next_node->prev = END_OF_STACK; /*Head of Stack prev also points to END_OF_STACK*/
		}
		else if (StrategyControl->LRUStackTail == buf_id) /* remove Tail */
		{
			StrategyControl->LRUStackTail = current_node->prev;
			DLLNode *prev_node = &LRUStack[current_node->prev];
			prev_node->next = END_OF_STACK;
		}
		else /* remove middle Node */
		{
			DLLNode *next_node = &LRUStack[current_node->next];
			DLLNode *prev_node = &LRUStack[current_node->prev];
			prev_node->next = current_node->next;
			next_node->prev = current_node->prev;
		}

		current_node->next = NOT_IN_STACK;
		current_node->prev = NOT_IN_STACK;
	}

	else
	{
		if (current_node->next == NOT_IN_STACK && current_node->prev == NOT_IN_STACK) /* case 2 */
		{
      printf("not in stack %d prev %d next %d ",current_node->buf_id, current_node->prev, current_node->next);
      printf("\n\n");
			if (StrategyControl->LRUStackHead == END_OF_STACK && StrategyControl->LRUStackHead == END_OF_STACK) /* Empty LRUStack, become new Head & Tail */
			{
        printf("empty stack");
        printf("\n\n");
				StrategyControl->LRUStackHead = current_node->buf_id;
				StrategyControl->LRUStackTail = current_node->buf_id;
				current_node->next = END_OF_STACK;
				current_node->prev = END_OF_STACK;
			}
			else /* current becomes new Head, update pointers*/
			{
        printf("new head");
        printf("\n\n");
				DLLNode *old_head_node = &LRUStack[StrategyControl->LRUStackHead];
				StrategyControl->LRUStackHead = current_node->buf_id;
				current_node->prev = END_OF_STACK;
				current_node->next = old_head_node->buf_id;
				old_head_node->prev = current_node->buf_id;
			}
		}
		else /* case 1 & 3*/
		{
      printf("move to front %d prev %d next %d ",current_node->buf_id, current_node->prev, current_node->next);
      printf("\n\n");
			if (StrategyControl->LRUStackHead != current_node->buf_id && StrategyControl->LRUStackTail == current_node->buf_id) /* current at Tail but not at Head*/
			{
        printf("at tail");
        printf("\n\n");
				DLLNode *new_tail_node = &LRUStack[current_node->prev];
				StrategyControl->LRUStackTail = new_tail_node->buf_id;
				new_tail_node->next = END_OF_STACK;

				DLLNode *old_head_node = &LRUStack[StrategyControl->LRUStackHead];
				StrategyControl->LRUStackHead = current_node->buf_id;
				current_node->prev = END_OF_STACK;
				current_node->next = old_head_node->buf_id;
				old_head_node->prev = current_node->buf_id;
			}
			else if (StrategyControl->LRUStackHead != current_node->buf_id) /* current not at Head or Tail*/
			{
        printf("in middle");
        printf("\n\n");
				DLLNode *next_node = &LRUStack[current_node->next];
				DLLNode *prev_node = &LRUStack[current_node->prev];
				prev_node->next = current_node->next;
				next_node->prev = current_node->prev;

				DLLNode *old_head_node = &LRUStack[StrategyControl->LRUStackHead];
				StrategyControl->LRUStackHead = current_node->buf_id;
				current_node->prev = END_OF_STACK;
				current_node->next = old_head_node->buf_id;
				old_head_node->prev = current_node->buf_id;
			}
			/* current at Head, no need to move */
		}
	}
  printStack();

}

void LRUReplacement(){
  printf("start of LRU");
  printf("\n\n");
  DLLNode *replacement_node = &LRUStack[StrategyControl->LRUStackTail];
	while(1)
	{
    printf(" %d prev %d next %d ",replacement_node->buf_id, replacement_node->prev, replacement_node->next);
    printf("\n\n");

		if (replacement_node->buf_id == StrategyControl->LRUStackHead)
		{
      break;
		}
		replacement_node = &LRUStack[replacement_node->prev];
	}
	printf("no unpinned buffers");
	printf("\n\n");
}

int main() {
  LRUStack = (DLLNode *)malloc(16 * sizeof(DLLNode));
  StrategyControl = (BufferStrategyControl *)malloc(sizeof(BufferStrategyControl));
  StrategyControl->LRUStackHead = END_OF_STACK;
	StrategyControl->LRUStackTail = END_OF_STACK;
  //printf("%d, %d", StrategyControl->LRUStackHead, StrategyControl->LRUStackTail);
  for (int i = 0; i < 16; i++) {
    DLLNode *node = &LRUStack[i];
    node->buf_id = i;
    node->next = NOT_IN_STACK;
    node->prev = NOT_IN_STACK;
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
