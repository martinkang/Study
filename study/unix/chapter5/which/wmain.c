#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "which.h"

int main( int argc, char *argv[] )
{
	int sRC = 0;
	int sNumPath = 0;
	int sIdx = 0;
	int *sFindPath = NULL;
	const char *sFname = NULL;
	const char *getPath = NULL;
	char **sPath = NULL;

	if ( ( argc != 2 ) && ( argc != 3 ) )
	{
		error_handling( "Argument Error" );
		return -1;
	}

	sFname = argv[1];

	if ( argc == 3 )
	{
		getPath = argv[2];
	}
	else
	{
		getPath = getenv( "PATH" );
		if ( getPath == NULL )
		{
			error_handling( "Wrong ENV" );
			return -1;
		}
	}

	sRC = sepPath( getPath, &sPath, &sNumPath );
	if ( sRC == -1 )
	{
		error_handling( "Path Separation is Error" );
		return -1;
	}
	else
	{
		for ( sIdx = 0; sIdx < sNumPath; sIdx++ )
		{
			printf( "Path[%d] : %s\n", sIdx + 1, sPath[sIdx] );
		}

		sFindPath = (int*)malloc( sizeof(int) * sNumPath );
	}

	sRC = findFile( sFname, sPath, sFindPath );
	if ( sRC == 0 )
	{
		printf ( "do not find %s\n", sFname );
	}
	else
	{
		printf( "File : %s\n", sFname );
		for ( sIdx = 0; sIdx < sNumPath; sIdx ++ )
		{
			if ( sFindPath[sIdx] == 1 )
			{
				printf( "PATH : %s\n", sPath[sIdx] );
				sRC = checkexecutable( sFname, sPath[sIdx] );
				if ( sRC == 1 )
				{
					printf( "%s/%s is executable\n", sPath[sIdx], sFname );
				}
				else
				{
					printf( "%s/%s is not executable\n", sPath[sIdx], sFname );
				}
			}
		}


	}

	free( sFindPath );
	free( sPath );

	return 0;
}
