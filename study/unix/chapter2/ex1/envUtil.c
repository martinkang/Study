#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "../../ide.h"

int ignoreOpt( const char **argv, const int argc );
int utilityOpt( const char **argv, const int argc );
int getOpt( const char *opt );
int copyEnv();
int freeEnv();
void printGetEnv();

int main( const int argc, const char *argv[] )
{
	int sRC = 0;

	IDE_TEST( copyEnv() != IDE_TRUE );

	if ( argc == 1 )
	{
		printGetEnv();
		return 0;
	}
	else
	{
		IDE_TEST_RAISE( argc == 2, ARG_ERROR );
	}


	sRC = getOpt( argv[1] );
	switch( sRC )
	{
		case 1:
			IDE_TEST( ( sRC = ignoreOpt( argv, argc ) ) != IDE_TRUE);
			break;
		case 2:
			IDE_TEST( utilityOpt( argv, argc ) != IDE_TRUE );
			break;
		default :
			printf( "Invalid Option Error\n" );
			return 0;
	}

	IDE_TEST( freeEnv() != IDE_TRUE );

	return IDE_SUCCESS;

	IDE_EXCEPTION( ARG_ERROR )
	{
		printf( "Argurent Error\n" );
	}
	EXCEPTION_END

	printf( "Exception END\n" );

	return IDE_FAILURE;
}

