#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

void s_init( stack *st )
{
	st->mNum_Stack = 0;
	st->head = NULL; 
}

int stackIsEmpty( stack *st )
{
	if ( st != NULL )
	{
		if ( st->mNum_Stack == 0 )
		{
			return 0;
		}
		else
		{
			return st->mNum_Stack;
		}
	}
	else
	{
		return -1;
	}
}

int s_pop( stack *st, DATA *aData )
{
	int sRC = 0;
	s_node *delPtr = NULL;;

	sRC = stackIsEmpty( st );
	if ( sRC == -1 )
	{
		return -1;
	}
	else if ( sRC == 0 )
	{
		fprintf( stderr, "Stack is Empty\n" );
		return 0;
	}

	*aData = st->head->mData;
	delPtr = st->head;
	st->head = st->head->mNext;
	( st->mNum_Stack )--;

	free( delPtr );

	return 1;
}

int s_push( stack *st, DATA aData )
{
	int sRC = 0;
	int slen = 0;
	s_node *newNode = NULL;

	newNode = (s_node*)malloc( sizeof( s_node ) );

	newNode->mData = aData;
	newNode->mNext = st->head;
	st->head = newNode;

	( st->mNum_Stack )++;

	return 1;	
}

void s_finalize( stack *st )
{
	int sRC = 1;
	s_node *sDel = NULL;
	s_node *sCur = NULL;

	sCur = st->head;

	while ( sCur != NULL )
	{
		sRC = stackIsEmpty( st );
		if ( sRC == 0 )
		{
			break;
		}
		else
		{
			sDel = sCur;
			sCur = sCur->mNext;
			free( sDel );
		}
	}
}

