#include <stdio.h>
#include <string.h>
#include "../UICI/uici.h"

#define PORT 80
#define BUFSIZE 255

int main( int argc, char *argv[] )
{
	int sRC = 0;
	int size = 0;

	int clntfd = 0;

	char buf[BUFSIZE] = {0, };

	if ( argc != 2 )
	{
		fprintf( stderr, "argument error\n" );
		return -1;
	}

	clntfd = u_connect( argv[1], PORT );
	if ( clntfd == -1 )
	{
		perror( "connect error" );
		return -1;
	}

	snprintf( buf, BUFSIZE, "%d", getpid() );
	size = write( clntfd, buf, strlen( buf ) );
	if ( size != strlen( buf ) )
	{
		fprintf( stderr, "send to server msg error\n" );
		close( clntfd );
		return -1;
	}

	read( clntfd, buf, sizeof( buf ) );

	fprintf( stderr, "%s\n", buf );

	close( clntfd );

	return 0;
}
