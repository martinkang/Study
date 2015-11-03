#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

void initHeap( heap_t *hp, int (*comp)( int d1, int d2 ) )
{
	hp->numData = 0;
	hp->comp = comp;
	hp->heapSize = 1;
	hp->heapArr = (node_t**)malloc( sizeof( node_t*) );
}

int getParentIdx( int idx )
{
	return idx / 2;
}

int getLChildIdx( int idx )
{
	return idx * 2;
}

int getRChildIdx( int idx )
{
	return idx * 2 + 1;
}

int HInsert( heap_t *hp, int data )
{
	node_t **reHeap = NULL;
	node_t *newNode = NULL;

	newNode = (node_t*)malloc( sizeof(node_t) );
	newNode->data = data;
	newNode->Priority = hp->numData;

	hp->heapArr[hp->numData] = newNode;

}
