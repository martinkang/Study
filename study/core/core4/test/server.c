#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/select.h>
#include "../reentry.h"
#include "../ide.h"

#define BUFSIZE 255

const char gAddr[] = "127.0.0.1";
const int gPort = 9090;

int open_socket( void )//return server socket
{
	int sRC = 0;

	int fd = 0;
	socklen_t addr_len;
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
	addr.sin_addr.s_addr = htonl( INADDR_ANY );

	sRC = bind( fd, ( struct sockaddr* )&addr, addr_len );
	if ( sRC == -1 )
	{
		r_perror( "bind error" );
		IDE_TEST( sRC );
	}

	snprintf( buf, BUFSIZE, "Server bind\nfamily = AF_INET\nport = %d\naddr = INADDR_ANY\n", gPort );

	sRC = listen( fd, 5 );
	if ( sRC == -1 )
	{
		r_perror( "listen error" );
		IDE_TEST( sRC );
	}

	return fd;

	EXCEPTION_END

	close( fd );

	return -1;
}

int acceptClient( const int serfd )
{
	int sRC = 0;

	int clntfd = 0;
	socklen_t clnt_len = 0;
	struct sockaddr_in clnt_addr;

	clntfd = accept( serfd, (struct sockaddr*)&clnt_addr, &clnt_len );
	if ( clntfd == -1 )
	{
		r_perror( "accept error" );
		return -1;
	}

	return clntfd;
}

int main( void )
{
	int sRC = 0;
	int n = 0;
	int i = 0;
	
	int size = 0;
	char msg[BUFSIZE] = {0, };

	int nfds = 0;
	fd_set cpset;
	fd_set setRD;

	int serfd = 0;
	int clntfd = 0;

	snprintf( msg, BUFSIZE, "server p[%d] Started", pthread_self() );

	serfd = open_socket();
	if ( serfd == -1 )
		return -1;

	snprintf( msg, BUFSIZE, "server socket created" );

	nfds = serfd;
	FD_ZERO( &setRD );
	FD_SET( serfd, &setRD );

	while ( 1 )
	{
		clntfd = 0;

		cpset = setRD;
		sRC = select( nfds + 1, &cpset, NULL, NULL, NULL );
		if ( sRC == -1 )
		{
			r_perror( "select error" );
			IDE_TEST( sRC );
		}
		else if ( sRC == 0 )
		{
			continue;
		}

		for ( i = 0; i < nfds + 1; i++ )
		{
			if ( FD_ISSET( i, &cpset ) == IDE_TRUE )
			{
				if ( i == serfd )
				{
					clntfd = acceptClient( serfd );
					if ( clntfd == -1 )
						continue;

					FD_SET( clntfd, &setRD );
					if ( clntfd > nfds )
						nfds = clntfd;

				}
				else
				{
					memset( msg, 0, BUFSIZE );
					size = read( i, msg, BUFSIZE );
					if ( size == 0 )
					{
						FD_CLR( i, &setRD );
						close( i );
						continue;
					}
					else if ( size != strlen( msg ) )
					{
						r_perror( "recv msg error" );
						continue;
					}

					msg[strlen( msg )] = '\0';
					r_printf( "%s\n", msg );
					n++;
				}
			}
			else
			{
				/* do nothing */
			}
		}
		
		sleep( 1 );
	}

	close( serfd );

	snprintf( msg, BUFSIZE, "server recv %d msg\nserver end\n", n );

	return 0;

	EXCEPTION_END

	close( serfd );

	return -1;
}
