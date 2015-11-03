#include <stdio.h>
#include <string.h>
#include "/home/mk/lib/reentry.h"
#include "/home/mk/lib/restart.h"
#include "uici.h"

#define BUFSIZE 1024

int main( int argc, char *argv[] )
{
	int sRC = 0;

	int port = 0;
	int fd = 0;
	char *adr = NULL;

	int size = 0;
	char buf[BUFSIZE] = {0, };

	if ( argc != 3 )
	{
		fprintf( stderr, "argument error\n" );
		return -1;
	}

	port = atoi( argv[1] );
	adr = argv[2];

	fd = u_connect( adr, port );
	if ( fd == -1 )
	{
		perror( "client open error" );
		return -1;
	}
	fprintf( stderr, "client open success\n" );

	while ( 1 )
	{
		r_printf( "message : " );
		size = r_read( STDIN_FILENO, buf, sizeof( buf ) );
		if ( size <= 0 )
		{
			r_fprintf( stderr, "client end\n" );
			break;
		}

		size = r_write( fd, buf, strlen( buf ) );
		if ( size != strlen( buf ) )
		{
			r_fprintf( stderr, "send msg error\n" );
			break;
		}
		r_printf( "message from serv : %s\n" , buf );
	}

	close( fd );

	return 0;
}
