#include <stdio.h>
#include "heap.h"


int compMax( int d1, int d2 )
{
	if ( d1 > d2 )
		return 0;
	else
		return 1;
}

int compMin( int d1, int d2 )
{
	if ( d1 < d2 )
		return 0;
	else
		return 1;
}

int main( void )
{
    heap_t newHeap;
    int sRC = 0;

    initHeap( &newHeap, compMax );


    return 0;
}

