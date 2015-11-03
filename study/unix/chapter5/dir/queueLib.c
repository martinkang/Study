#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"

void q_init( que *aQue )
{
	aQue->mHead = NULL;
	aQue->mTail = NULL;
	aQue->mNum_que = 0;
}

int queIsEmpty( que *aQue )
{
	if ( aQue == NULL)
	{
		return -1;
	}

	if ( aQue->mNum_que == 0 )
	{
		return 0;
	}
	else
	{
		return aQue->mNum_que;
	}
}

void q_finalize( que *aQue )
{
	int sRC = 0;
	q_node *sCur = NULL;
	q_node *sDel = NULL;

	sRC = queIsEmpty( aQue );

	if ( sRC >= 0 )
	{
		while ( sCur != NULL )
		{
			sDel = sCur;
			free ( sDel );
			sDel = NULL;
			sCur = sCur->mPrev;
		}
	}
}

int q_pop ( que *aQue, char **aData )
{
	int sRC = 0;
	q_node *sDel = NULL;

	sRC = queIsEmpty( aQue );
	if ( sRC > 0 )
	{
		*aData = aQue->mHead->mStr;

		sDel = aQue->mHead;
		aQue->mHead = aQue->mHead->mPrev;

		( aQue->mNum_que )--;
		free( sDel );
	}
	else if ( sRC == 0 )
	{
		*aData = "\0";
		fprintf( stderr, "Que is Empty\n" );
		return 0;
	}
	else
	{
		fprintf (stderr, "Que error\n" );
		return -1;
	}
}

int q_push ( que *aQue, char *aData )
{
	int sRC = 0;
	int slen = 0;
	q_node *newNode = NULL;

	if ( aQue != NULL )
	{
		slen = strlen( aData );

		newNode = (q_node*)malloc( sizeof(q_node) );
		newNode->mStr = (char*)malloc( sizeof(char) );
		newNode->mPrev = NULL;
		strcpy( newNode->mStr, aData );

		if ( aQue->mHead == NULL )
		{
			aQue->mHead = newNode;
			aQue->mTail = aQue->mHead;

			aQue->mHead->mPrev = aQue->mTail;
			
			( aQue->mNum_que ) ++;
		}
		else
		{
			aQue->mTail->mPrev = newNode;
			aQue->mTail = newNode;
			( aQue->mNum_que ) ++;
		}
	}
	else
	{
		fprintf( stderr, "Queue is not initailized\n" );
		return -1;
	}
}

/*
int main()
{
	que qu;
	char *data = NULL;

	init( &qu );
	q_push( &qu, "asdf" );
	q_push( &qu, "aaa" );

	q_pop( &qu, &data );
	printf( "%s\n", data );
	
	q_pop( &qu, &data );
	printf( "%s\n", data );
	
	q_pop( &qu, &data );
	printf( "%s\n", data );

	finalize( &qu );
}
*/
