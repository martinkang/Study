#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "../../ide.h"
#include "../../log.h"

#define BUF_SIZE 3

void readAndWrite( FILE *aRFile, FILE *aWFile )
{
	char sReadMsg[BUF_SIZE] = {0, };
	char *sRC = NULL;
	char *errMsg = NULL;

	ideLog( IDE_DE, "Read And Write is Start" );
	while( IDE_TRUE  )
	{
		while ( ( ( sRC = fgets( sReadMsg, BUF_SIZE, aRFile ) ) == NULL ) &&
				( errno == EINTR ) );
		if ( sRC == NULL )
		{
			if ( strerror( errno ) == NULL )
			{
				ideLog( IDE_DE, "Copy is Complete" );
				break;
			}
			else
			{
				ideLog( IDE_DE, "fgets Error" );
				break;
			}
		}
		else
		{
			fputs( sReadMsg, aWFile );
		}
		sleep(1);
	}
}

int main( int argc, char *argv[] )
{
	FILE *sReadFile = NULL;
	FILE *sNewFile = NULL;
	
	int sRC = 0;
	
	char *errMsg = NULL;

	if ( argc != 3 )
	{
		ideLog( IDE_DE, "Wrong Argument" );
		IDE_TEST( IDE_TRUE );
	}

	sReadFile = fopen( argv[1], "r" );
	if ( sReadFile != NULL )
	{
		ideLog( IDE_DE, " === sReadFile Open is Success ===" );
	}
	else
	{
		errMsg = strerror( errno );
		
		fprintf( stderr, "%s\n", errMsg );
		ideLog( IDE_DE, errMsg );
		IDE_TEST( IDE_TRUE );
	}

	while ( ( sNewFile = fopen( argv[2], "a" ) ) != NULL )
	{
		if ( sReadFile != NULL )
		{
			ideLog( IDE_DE, " === sNewFile Open is Success ===" );
			break;
		}
		else
		{
			errMsg = strerror( errno );

			fprintf( stderr, "%s\n", errMsg );
			ideLog( IDE_DE, errMsg );
		}
	}

	readAndWrite( sReadFile, sNewFile );
	
	fclose( sNewFile );
	fclose( sReadFile );

	return 0;

	EXCEPTION_END

	ideLog( IDE_DE, "EXCEPTION_END" );
	(void)saveLog( IDE_DE );

	return -1;
}
