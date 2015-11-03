#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "list.h"

int sorting( int d1, int d2 )
{
	if ( d1 < d2 )
		return 0;
	else
		return 1;
}

void setRule( list_t *plist, int (*comp)(int d1, int d2 ) )
{
	plist->comp = comp;
}

int main()
{
	int data = 0;
	list_t sList;

	initlist( &sList );

	setRule( &sList, sorting );

	while ( scanf( "%d", &data ) != EOF )
	{
		linsert( &sList, data );
	}

	lprint( &sList );
}
