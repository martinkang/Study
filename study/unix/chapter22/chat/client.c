#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "packet.h"
#include "uici.h"
#include "../../lib/reentry.h"
#include "../../lib/ide.h"

#define BUFSIZE 255

int sendMsg( int fd, char *msg )
{
	int size = 0;
	char buf[BUFSIZE] = {0, };

	strcpy( buf, msg );

	size = write( fd, buf, strlen( buf ));
	return size;
}

int main( int argc, char *argv[] )
{
	int sRC = 0;
	int step = 0;
	int size = 0;

	int fd = 0;

	char buf[BUFSIZE] = {0, };

	if ( argc != 3 )
	{
		IDE_TEST_RAISE( 1, ARGUMENT_ERR );
	}

	fd = u_connect( argv[1], atoi( argv[2] ) );
	IDE_TEST( fd );
	step = 1;

	while ( 1 )
	{
		r_printf( ">>" );
		gets( buf );
		if ( strcmp( buf, ".q\n" ) == 0 )
			break;

		size = sendMsg( fd, buf );
		read( fd, buf, BUFSIZE );
		r_printf( "%s\n", buf );
	}
	close( fd );
	return 0;

	IDE_EXCEPTION( ARGUMENT_ERR )
	{
		r_fprintf( stderr, "port is needed\n" );
	}
	
	EXCEPTION_END

	close( fd );

	return -1;
}
