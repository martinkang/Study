#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "which.h"

int main( int argc, char *argv[] )
{
	int sRC = 0;
	char *getPath = NULL;
	char *sPath = NULL;

	if ( argc != 2 )
	{
		fprintf( stderr, "Argument Error\n" );
		return -1;
	}

	getPath = getenv( "a" );
	return 0;
}
