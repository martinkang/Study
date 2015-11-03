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
	int serfd = 0;
	int clntfd = 0;

	int size = 0;
	char buf[BUFSIZE] = {0, };

	if ( argc != 2 )
	{
		fprintf( stderr, "argument error\n" );
		return -1;
	}

	port = atoi( argv[1] );
	serfd = u_open( port );
	if ( serfd == -1 )
	{
		perror( "server open error" );
		return -1;
	}
	fprintf( stderr, "server open success\nwait client.....\n" );

	clntfd = u_accept( serfd );
	if ( clntfd == -1 )
	{
		r_perror( "client accept error" );
		return -1;
	}
	r_fprintf( stderr, "accept complete\n" );

	while ( 1 )
	{
		size = r_read( clntfd, buf, sizeof( buf ) );
		if ( size <= 0 )
		{
			r_fprintf( stderr, "server end\n" );
			break;
		}

		size = r_write( clntfd, buf, strlen( buf ) );
		if ( size != strlen( buf ) )
		{
			r_fprintf( stderr, "send msg error\n" );
			break;
		}
	}

	close( clntfd );
	close( serfd );

	return 0;
}
