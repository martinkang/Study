#ifndef __HEAP_H__
#define __HEAP_H__

typedef struct node
{
	int Priority;
	int data;
} node_t;

typedef struct heap
{
	int numData;
	int heapSize;
	int (*comp)( int d1, int d2 );
	node_t **heapArr;
} heap_t;

#endif

void initHeap( heap_t *hp, int (*comp)( int d1, int d2 ) );
