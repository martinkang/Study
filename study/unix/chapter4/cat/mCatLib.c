#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "cat.h"
#include "../../ide.h"
#include "../../log.h"

int dupCheck( int aIdx, int *aDupCheck)
{
	if ( aDupCheck[aIdx] == 0 )
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int getOpt( const int argc, 
		    const char **argv,
			int *aOpt )
{
	int sRC = 0;
	int sIdx = 0;
	int sLen = 0;
	int sDupNum = 0;
	const char *sOpt = argv[1];
	
	if ( argc == 2 )
	{
		if ( sOpt[0] == '-' )
		{
			fputs( "Option Error\n", stderr );
			ideLog( IDE_DE, "Option Error" );

			IDE_TEST( IDE_TRUE );
		}
		else
		{
			aOpt[0] = 1;
			return 0;
		}
	}

	sLen = strlen( sOpt );

	for( sIdx = 1; sIdx < sLen; sIdx++ )
	{
		switch ( sOpt[sIdx] )
		{
			case 's' :  /* show start */
				sDupNum = 1;		
				break;
			case 'e' : /* show ends */
				sDupNum = 2;
				break;
			case 'n' : /* show number */
				sDupNum = 3;
				break;
			default :
				fputs( "Option Error\n", stderr );
				ideLog( IDE_DE, "Option Error" );

				IDE_TEST( IDE_TRUE );
		}
		
		sRC = dupCheck( sDupNum, aOpt );
		if ( sRC == 0 )
		{
			if ( aOpt[1] + aOpt[2] > 1 )
			{
				fputs( "Invalid Option Set\n", stderr );
				ideLog( IDE_DE, "Invalid Option Set" );

				IDE_TEST( IDE_TRUE );
			}

			aOpt[sDupNum] = 1;
		}
		else
		{
			fputs( "Option Duplicated\n", stderr );
			ideLog( IDE_DE, "Option Duplicated" );

			IDE_TEST( IDE_TRUE );
		}
	}

	return ( sLen - 1 );

	EXCEPTION_END

	return -1;
}

int printCat( const char *fName, int *aOpt )
{
	int n = 1;
	FILE *sFile = NULL;
	char sPtr = NULL;

	sFile = fopen( fName, "r" );

	if ( sFile == NULL )
	{
		fputs( strerror( errno ), stderr );                      
		fputc( '\n', stderr );
		ideLog( IDE_DE, strerror( errno ) );

		IDE_TEST( IDE_TRUE );
	}

	if ( aOpt[3] == 1 )
		printf( "\n%d : ", n );

	while ( ( sPtr = fgetc( sFile ) ) != -1 )
	{
		printf( "%c", sPtr );

		if ( sPtr == '\n' )
		{
			n++;

			if ( aOpt[1] == 1 )
			{
				if ( n > 10 )
					break;
			}

			if ( aOpt[3] == 1 )
				printf( "%d : ", n );
		}
	}
	printf( "\n" );

	fclose( sFile );

	return 0;

	EXCEPTION_END

	return -1;
}

