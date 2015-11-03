#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../../lib/restart.h"
#include "../../lib/reentry.h"

#define BUFSIZE 1024

int main( int argc, char *argv[] )
{
	int sRC = 0;
	int size = 0;

	char buf[1024] = {0, };

	int clntfd = 0;
	int port = 0;
	char *adr  = NULL; 
	socklen_t addr_len = 0;;
	struct sockaddr_in addr;

	if ( argc != 3 )
	{
		fprintf( stderr, "input ip and port\n" );
		return -1;
	}

	port = atoi( argv[2] );
	adr = argv[1];

	printf( "port : %d\nip : %s\n", port, adr );

	clntfd = socket( PF_INET, SOCK_STREAM, 0 );
	if ( clntfd == -1 )
	{
		r_perror( "socket error" );
		return -1;
	}

	memset( &addr, 0, sizeof( addr ) );
	addr.sin_family = AF_INET;
	addr.sin_port = htons( port );
	addr.sin_addr.s_addr = inet_addr( adr );

	addr_len = sizeof( addr );

	sRC = connect( clntfd, (struct sockaddr*)&addr, addr_len );
	if ( sRC == -1 )
	{
		r_perror( "connect error" );
		return -1;
	}

	while ( 1 )
	{
//		r_printf( "send to server msg : " );
		size = r_read( STDIN_FILENO, buf, BUFSIZE );

		if ( size <= 0 )
		{
			break;
		}

		r_write( clntfd, buf, sizeof( buf ) );
		
		if ( strcmp( buf, ".q" ) == 0 )
			break;

	}


	r_printf( "client end\n" );

	r_close( clntfd );

	return 0;
}
	
