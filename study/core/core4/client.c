#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "client.h"
#include "ide.h"
#include "reentry.h"

#define RANDMAX 100

extern const char gAddr[];
extern const int gPort;

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
	logWR( buf );

	return fd;

	EXCEPTION_END
	
	close( fd );
	return -1;
}

void *clientFunction( void )
{
	int sRC = 0;
	int n = 0;
	int r = 0;

	int fd = 0;

	int size = 0;
    char msg[BUFSIZE] = {0, };

    snprintf( msg, BUFSIZE, "client[%d] Started\n", pthread_self() );
	logWR( msg );

	fd = connectServer();
	if ( fd == -1 )
	{
		return;
	}

	memset( msg, 0, BUFSIZE );
	snprintf( msg, BUFSIZE, "client[%d] Connected\n", pthread_self() );
	logWR( msg );

	srand( time( NULL ) );

	while ( isExit() == 0 )
	{
		r = rand() % RANDMAX;
		
		memset( msg, 0, BUFSIZE );
		snprintf( msg, BUFSIZE, "client[%d] %d\n", pthread_self(), r );

		size = write( fd, msg, strlen( msg ) );
		if ( size != strlen( msg ) )
		{
			r_perror( "send to msg error" );
			IDE_TEST( -1 );
		}

		n++;

		sRC = queue_data( (void*)msg, strlen( msg ) );
		if ( sRC == -1 )
		{
			r_perror( "que error" );
			IDE_TEST( -1 );
		}

		sleep( 1 );
	}

	close( fd );

	memset( msg, 0, BUFSIZE );	
    snprintf( msg, BUFSIZE, "client[%d] send %d msg\nclient function end\n", pthread_self(), n );
	logWR( msg );

	r_printf( "%s", msg );

	return;

	EXCEPTION_END

	close( fd );

	return;
}
