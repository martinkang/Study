#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../../lib/ide.h"
#include "../../lib/reentry.h"

int u_open( int port )
{
	int sRC = 0;
	int sockIsCreat = 0;

	int fd = 0;
	socklen_t addr_len = 0;
	struct sockaddr_in addr;

	int opt = 1;
	socklen_t optlen = 0;

	fd = socket( PF_INET, SOCK_STREAM, 0 );
	IDE_TEST_RAISE( fd, SOCK_CREATE_ERR );
	sockIsCreat = 1;

	optlen = sizeof( opt );
	sRC = setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, &opt, optlen );
	IDE_TEST_RAISE( sRC, SET_SOCKET_ERR );

	addr_len = sizeof( addr );
	memset( &addr, 0, addr_len );
	addr.sin_family = AF_INET;
	addr.sin_port = htons( port );
	addr.sin_addr.s_addr = htonl( INADDR_ANY );

	sRC = bind( fd, (struct sockaddr*)&addr, addr_len );
	IDE_TEST_RAISE( sRC, SOCK_BIND_ERR );

	sRC = listen( fd, 5 );
	IDE_TEST_RAISE( sRC, SOCK_LISTEN_ERR );

	return fd;

	IDE_EXCEPTION( SOCK_CREATE_ERR )
	{
		r_perror( "socket() error" );
	}

	IDE_EXCEPTION( SET_SOCKET_ERR )
	{
		r_perror( "setsockopt() error" );
	}

	IDE_EXCEPTION( SOCK_BIND_ERR )
	{
		r_perror( "bind() error" );
	}

	IDE_EXCEPTION( SOCK_LISTEN_ERR )
	{
		r_perror( "listen() error" );
	}

	EXCEPTION_END

	if ( sockIsCreat == 1 )
		close( fd );

	return -1;
	
}

int u_accept( int fd )
{
	int clntfd = 0;
	socklen_t addr_len = 0;
	struct sockaddr_in addr;

	clntfd = accept( fd, (struct sockaddr*)&addr, &addr_len );
	IDE_TEST_RAISE( clntfd, ACCEPT_ERR );

	return clntfd;

	IDE_EXCEPTION( ACCEPT_ERR )
	{
		r_perror( "accept() error" );
	}

	EXCEPTION_END

	return -1;
}

int u_connect( char *addr, int port )
{
	int sRC = 0;
	int sockIsCreat = 0;

	int fd = 0;
	socklen_t addr_len = 0;
	struct sockaddr_in sockaddr;

	fd = socket( PF_INET, SOCK_STREAM, 0 );
	IDE_TEST_RAISE( fd, SOCK_CREATE_ERR );
	sockIsCreat = 1;

	addr_len = sizeof( sockaddr );
	memset( &sockaddr, 0, addr_len );
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons( port );
	sockaddr.sin_addr.s_addr = inet_addr( addr );
	sRC = connect( fd, (struct sockaddr*)&sockaddr, addr_len );
	IDE_TEST_RAISE( sRC, SOCK_CONNECT_ERR );

	return fd;

	IDE_EXCEPTION( SOCK_CREATE_ERR )
	{
		r_perror( "socket() error" );
	}

	IDE_EXCEPTION( SOCK_CONNECT_ERR )
	{
		r_perror( "bind() error" );
	}

	EXCEPTION_END

	if ( sockIsCreat == 1 )
		close( fd );

	return -1;
	
}
	

