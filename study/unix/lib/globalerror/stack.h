#ifndef __STACK_H__
#define __STACK_H__

#define DATA int

typedef struct stack_node
{
	DATA mData;
	struct stack_node *mNext;
} s_node;

typedef struct stack
{
	int mNum_Stack;
	s_node *head;
} stack;

#endif

void s_init( stack *st );

int s_pop( stack *st, DATA *aData );

int s_push( stack *st, DATA aData );

void s_finalize( stack *st );
