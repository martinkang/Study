#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void initlist( list_t *plist )
{
	node_t *dummy;

	dummy = (node_t*)malloc( sizeof( node_t ) );

	dummy->data = -1;
	dummy->next = NULL;

	plist->cur = NULL;
	plist->prev = NULL;
	plist->head = dummy;
	plist->comp = NULL;
	plist->numList = 0;
}

void lfirst( list_t *plist )
{
	plist->cur = plist->head;
}

int lnext( list_t *plist )
{
	if ( plist->cur->next != NULL )
	{
		plist->prev = plist->cur;
		plist->cur = plist->cur->next;
	}
	else
	{
		return -1;
	}

	return 0;
}

int ldelete( list_t *plist, int data )
{
	int sRC = 0;

	sRC = lsearch( plist, data );
	if ( sRC == -1 )
		return sRC;

	plist->prev->next = plist->cur->next;

	free( plist->cur );
	plist->cur = plist->prev;

	plist->numList = plist->numList - 1;

	return 0;
}

int lsearch( list_t *plist, int data )
{
	lfirst( plist );

	while( lnext( plist ) == 0 )
	{
		if ( plist->cur->data == data )
			return 0;
	}

	return -1;
}

void lprint( list_t *plist )
{
	lfirst( plist );

	while( lnext( plist ) == 0 )
	{
		printf( "%d\n", plist->cur->data );
	}
}

int finsert( list_t *plist, int data )
{
	node_t *newNode = NULL;

	newNode = (node_t*)malloc( sizeof( node_t ) );

	newNode->data = data;
	newNode->next = plist->head->next;

	plist->head->next = newNode;

	return 0;
}

int sinsert( list_t *plist, int data, int (*comp)(int d1, int d2 ) )
{
	node_t *newNode = NULL;

	newNode = (node_t*)malloc( sizeof( node_t ) );

	newNode->data = data;
	newNode->next = NULL;

	lfirst( plist );

	while( lnext( plist ) == 0 )
	{
		if ( comp( newNode->data, plist->cur->data ) == 0 )
		{
			newNode->next = plist->cur;
			plist->prev->next = newNode;
			return 0;
		}
	}

	plist->cur->next = newNode;

	return 0;
}

int linsert( list_t *plist, int data )
{
	int sRC = 0;

	if ( plist->comp == NULL )
	{
		sRC = finsert( plist, data );
	}
	else
	{
		sRC = sinsert( plist, data, plist->comp );
	}

	plist->numList = plist->numList + 1;
	
	return sRC;
}
