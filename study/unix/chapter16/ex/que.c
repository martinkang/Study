#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "que.h"

static que_t *gQue;

void initQue()
{
	que_t *newQueue = NULL;

	newQueue = (que_t*)malloc( sizeof( que_t ) );
	newQueue->numQue = 0;
	newQueue->mHead = NULL;
	newQueue->mTail = NULL;

	gQue = newQueue; 
}

int queue( void *aData )
{
	int sRC = 0;
	node_t *newNode = (node_t*)malloc( sizeof(node_t) );

	newNode->data = aData;
	newNode->mNext = NULL;

	if ( gQue->mHead == NULL )
	{
		gQue->mHead = newNode;
		gQue->mTail = newNode;
		gQue->mHead->mNext = gQue->mTail;
	}
	else
	{
		gQue->mTail->mNext = newNode;
		gQue->mTail = newNode;
	}

	( gQue->numQue )++;

	return sRC;
}

int dequeue( void **aData )
{
	int sRC = 0;
	node_t *cur = NULL;

	if ( gQue->mHead == NULL )
	{
		write( STDOUT_FILENO, "No Data in Queue\n", sizeof( "No Data in Queue\n" ) );
		sRC = -1;
	}
	else
	{

		cur = gQue->mHead;
		*aData = cur->data;

		if ( cur != gQue->mTail )
		{
			gQue->mHead = cur->mNext;
		}

		free( cur );
	}

	(gQue->numQue)--;

	return sRC;
}
