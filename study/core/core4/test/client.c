#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>
#include "../reentry.h"
#include "../ide.h"

#define BUFSIZE 255
#define RANDMAX 100

const char gAddr[] = "127.0.0.1";
const int gPort = 9090;

int connectServer( void )
{
	int sRC = 0;

	int fd = 0;
	socklen_t addr_len = 0;
	struct sockaddr_in addr;

	char buf[BUFSIZE] = {0, };

	fd = socket( PF_INET, SOCK_STREAM, 0 );
	if ( fd == -1 )
	{
		r_perror( "socket error" );
		return -1;
	}

	addr_len = sizeof( addr );
	memset( &addr, 0, addr_len );
	addr.sin_family = AF_INET;
	addr.sin_port = htons( gPort );
	addr.sin_addr.s_addr = inet_addr( gAddr );

	sRC = connect( fd, (struct sockaddr*)&addr, addr_len );
	if ( sRC == -1 )
	{
		r_perror( "connect error" );
		IDE_TEST( sRC );
	}

	snprintf( buf, BUFSIZE, "client connected\nfamily = AF_INET\nport = %d\naddr = %s\n", gPort, gAddr );

	return fd;
	
	EXCEPTION_END
	
	close( fd );
	return -1;
}

int main( void )
{
	int sRC = 0;
	int n = 0;
	int r = 0;

	int fd = 0;

	int size = 0;
    char msg[BUFSIZE] = {0, };

    snprintf( msg, BUFSIZE, "client[%d] Started\n", pthread_self() );

	fd = connectServer();
	if ( fd == -1 )
	{
		return -1;
	}

	snprintf( msg, BUFSIZE, "client[%d] Connected\n", pthread_self() );

	srand( time( NULL ) );

	while ( 1 )
	{
		r = rand() % RANDMAX;

		memset( msg, 0, BUFSIZE );
		snprintf( msg, BUFSIZE, "client[%d] : %d", getpid(), r );

		size = write( fd, msg, strlen( msg ) );
		printf( "size : %d\nstrlen : %d\nmsg : %s$\n", size, strlen( msg ), msg );
		if ( size > strlen( msg ) )
		{
			r_perror( "send to msg error" );
			IDE_TEST( -1 );
		}

		n++;

		sleep( 1 );
	}

    snprintf( msg, BUFSIZE, "client[%d] send %d msg\nclient function end\n", pthread_self(), n );

	return 0;

	close( fd );

	EXCEPTION_END

	close( fd );

	return -1;
}
