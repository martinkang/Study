#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../../lib/reentry.h"
#include "../../lib/restart.h"

#define BUFSIZE 1024

int main( int argc, char *argv[] )
{
	int sRC = 0;

	int size = 0;

	char buf[BUFSIZE] = {0, };

	int serfd = 0;
	int clntfd = 0;
	int port = 0;
	socklen_t adr_len = 0;
	socklen_t clnt_len = 0;
	struct sockaddr_in ser_addr;
	struct sockaddr_in clnt_addr;

	if ( argc != 2 )
	{
		fprintf( stderr, "input port !\n" );
		return -1;
	}

	port = atoi( argv[1] );

	serfd = socket( PF_INET, SOCK_STREAM, 0 );
	if ( sRC == -1 )
	{
		r_perror( "socket error" );
		return -1;
	}

	adr_len = sizeof( ser_addr );

	memset( &ser_addr, 0, sizeof( ser_addr ) );
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons( port );
	ser_addr.sin_addr.s_addr = htonl( INADDR_ANY );

	sRC = bind( serfd, (struct sockaddr*)&ser_addr, adr_len );
	if ( sRC == -1 )
	{
		r_perror( "bind error" );
		return -1;
	}

	r_fprintf ( stderr, "wait for client....\n" );

	sRC = listen( serfd, 5 );
	if ( sRC == -1 )
	{
		r_perror( "listen error" );
		return -1;
	}

	clntfd = accept( serfd, (struct sockaddr*)&clnt_addr, &clnt_len );
	if ( sRC == - 1)
	{
		r_perror( "accept error" );
		return -1;
	}

	r_fprintf( stderr, "connect complete\n" );
	while ( 1 )
	{
		size = r_read( clntfd, buf, BUFSIZE );

		if ( size <= 0 )
			break;

		if ( strcmp( buf, ".q" ) == 0 )
			break;

		r_write( STDOUT_FILENO, buf, sizeof( buf ) );
	}

	r_fprintf( stderr, "server end\n" );
	r_close( clntfd );
	r_close( serfd );
	return 0;
}
