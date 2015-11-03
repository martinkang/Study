#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "que.h"

void initQue( que_t *que )
{
	que->numQue = 0;
	que->mHead = NULL;
	que->mTail = NULL;
}

int getQueNum( que_t *que )
{
	return que->numQue;
}

int queue( que_t *que, void *aData, int size )
{
	int sRC = 0;
	node_t *newNode = (node_t*)malloc( sizeof(node_t) );

	newNode->data = (void*)calloc( size, 1 );
	memcpy( newNode->data, aData, size );
	newNode->mNext = NULL;

	if ( que->mHead == NULL )
	{
		que->mHead = newNode;
		que->mTail = newNode;
		que->mHead->mNext = que->mTail;
	}
	else
	{
		que->mTail->mNext = newNode;
		que->mTail = newNode;
	}

	( que->numQue )++;

	return sRC;
}

int dequeue( que_t *que, void *aData, int size )
{
	int sRC = 0;
	node_t *cur = NULL;
	node_t *del = NULL;

	if ( que->mHead == NULL )
	{
		sRC = -1;
	}
	else
	{
		cur = que->mHead;
		memcpy( aData, cur->data, size );

		que->mHead = cur->mNext;

		free( cur->data );
		free( cur );

		cur = NULL;
	}

	(que->numQue)--;

	return sRC;
}
