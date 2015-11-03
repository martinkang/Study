#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "listlib.h"

#define TRAV_INIT_SIZE 8

typedef struct list_struct
{
	data_t item;
	struct list_struct *next;
} list_t;

static list_t endlist;
static list_t *headPtr = NULL;
static list_t *tailPtr = NULL;
static list_t **travPtrs = NULL;
static int travPtrs_Size = 0;

int accData(void)
{
	int i;
	list_t **newPtrs;

	if ( headPtr == NULL )
	{
		errno = EINVAL;
		return -1;
	}

	if ( travPtrs_Size == 0 )
	{
		travPtrs = ( list_t ** )calloc( TRAV_INIT_SIZE, sizeof(list_t * ) );
		if ( travPtrs == NULL )
			return -1;

		travPtrs[0] = headPtr;
		travPtrs_Size = TRAV_INIT_SIZE;
		return 0;
	}

	for ( i = 0; i < travPtrs_Size; i++ )
	{
		if ( travPtrs[i] == NULL )
		{
			travPtrs[i] = headPtr;
			return i;
		}
	}

	newPtrs = realloc( travPtrs, 2 * travPtrs_Size * sizeof( list_t * ) );

	if ( newPtrs == NULL )
		return -1;

	travPtrs = newPtrs;
	travPtrs[travPtrs_Size] = headPtr;
	travPtrs_Size *= 2;
	return travPtrs_Size/2;
}

int addData( data_t data )
{
	list_t *newNode;
	int nodeSize;

	nodeSize = sizeof( list_t ) + strlen( data.str ) + 1;

	if (( newNode = (list_t *)(malloc(nodeSize))) == NULL )
		return -1;

	newNode->item.time = data.time;
	newNode->item.str = (char*)newNode + sizeof(list_t);
	strcpy( newNode->item.str, data.str );

	newNode->next = NULL;

	if ( headPtr == NULL )
		headPtr = newNode;
	else
		tailPtr->next = newNode;

	tailPtr = newNode;

	return 0;
}

int getData( int key, data_t *dataPtr )
{
	list_t *t;

	if ( (key < 0) || (key >= travPtrs_Size) || travPtrs[key] == NULL )
	{
		errno = EINVAL;
		return -1;
	}

	if ( travPtrs[key] == &endlist )
	{
		dataPtr->str = NULL;
		travPtrs[key] = NULL;
		return 0;
	}

	t = travPtrs[key];
	dataPtr->str = (char*)malloc(strlen(t->item.str) + 1 );
	strcpy( dataPtr->str, t->item.str );

	if ( t->next == NULL )
		travPtrs[key] = &endlist;
	else
		travPtrs[key] = t->next;
	return 0;
}

int freekey( int key )
{
	if ( (key < 0 ) || ( key >= travPtrs_Size ) )
	{
		errno = EINVAL;
		return -1;
	}

	travPtrs[key] = NULL;
	return 0;
}
