#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static int *gData = NULL;

int mbsearch( int data, int sIdx, int eIdx )
{
	int mIdx = ( sIdx + eIdx ) / 2;

	if ( gData[mIdx] == data )
		return mIdx;

	if ( sIdx + 1 == eIdx )
		return -1;

	if ( gData[mIdx] > data )
		return mbsearch( data, sIdx, mIdx );

	if ( gData[mIdx] < data )
		return mbsearch( data, mIdx, eIdx );
}

int main( void )
{
	int i = 0;
	int n = 0;
	int snum = 0;

	while( scanf( "%d", &n ) != EOF )
	{
		gData = (int*)malloc( sizeof( int ) * n );
		for ( i = 0; i < n; i++ )
		{
			scanf( "%d", &gData[i] );
		}
		scanf( "%d", &snum );

		printf( "%d Idx : %d\n", snum, mbsearch( snum, 0, n ) );

		free( gData );
	}

	return 0;
}
