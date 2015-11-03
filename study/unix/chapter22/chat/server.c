#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "packet.h"
#include "uici.h"
#include "../../lib/reentry.h"
#include "../../lib/ide.h"

#define BUFSIZE 255

int main( int argc, char *argv[] )
{
	int sRC = 0;
	int step = 0;
	int size = 0;

	int serfd = 0;
	int clntfd = 0;

	char buf[BUFSIZE] = {0, };

	if ( argc != 2 )
	{
		IDE_TEST_RAISE( 1, ARGUMENT_ERR );
	}

	serfd = u_open( atoi( argv[1] ) );
	IDE_TEST( serfd );
	step = 1;

	clntfd = u_accept( serfd );
	IDE_TEST( clntfd );
	step = 2;

	while ( 1 )
	{
		size = read( clntfd, buf, BUFSIZE );
		if ( size == 0 )
		{
			break;
		}
		buf[strlen( buf )] = '\0';

		size = write( clntfd, buf, strlen( buf ) );
	}


	close( clntfd );
	close( serfd );
	return 0;

	IDE_EXCEPTION( ARGUMENT_ERR )
	{
		r_fprintf( stderr, "port is needed\n" );
	}
	
	EXCEPTION_END

	close( serfd );

	return -1;
}
