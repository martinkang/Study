#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/select.h>
#include "uici.h"
#include "uiciname.h"

static int u_ignore_sigpipe()
{
	int sRC = 0;

	struct sigaction act;

	sRC = sigaction( SIGPIPE, NULL, &act );
	
	if ( act.sa_handler == SIG_DFL )
		act.sa_handler = SIG_IGN;

	sRC = sigaction( SIGPIPE, &act, NULL );
	return sRC;
}

int u_open( int port )
{
	int sRC = 0;
	int err = 0;

	int fd = 0;
	int opt = 1;
	socklen_t optlen = 0;
	socklen_t addr_len = 0;
	struct sockaddr_in addr;

	sRC = u_ignore_sigpipe();
	if ( sRC != 0 )
		return -1;

	fd = socket( PF_INET, SOCK_STREAM, 0 );
	if ( fd == -1 )
		return -1;

	optlen = sizeof( opt );
	sRC = setsockopt( fd, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, optlen );
	if ( sRC == -1 )
	{
		err = errno;
		while ( ( close( fd ) == -1 ) && ( errno == EINTR ) );
		errno = err;
		return -1;
	}

	addr_len = sizeof( addr );
	memset( &addr, 0, addr_len );
	addr.sin_family = AF_INET;
	addr.sin_port = htons( port );
	addr.sin_addr.s_addr = htonl( INADDR_ANY );
	
	sRC = bind( fd, (struct sockaddr*)&addr, addr_len );
	if ( sRC == -1 )
		return -1;

	sRC = listen( fd, 5 );
	if ( sRC == -1 )
		return -1;

	return fd;
}

int u_accept( int ser_fd )
{
	int clnt_fd = 0;
	socklen_t addr_len;
	struct sockaddr_in addr;

	clnt_fd = accept( ser_fd, (struct sockaddr*)&addr, &addr_len );
	if ( clnt_fd == -1 )
		return -1;

	return clnt_fd;
}

int u_connect( char *adr, int port )
{
	int sRC = 0;
	int ret = 0;

	int fd = 0;
	fd_set fset;
	socklen_t addr_len = 0;
	struct sockaddr_in addr;
	
	sRC = u_ignore_sigpipe();
	if ( sRC != 0 )
		return -1;

	fd = socket( PF_INET, SOCK_STREAM, 0 );
	if ( fd == -1 )
		return -1;

	
	addr_len = sizeof( addr );
	memset( &addr, 0, addr_len );
	addr.sin_family = AF_INET;
	addr.sin_port = htons( port );
	sRC = name2addr( adr, &(addr.sin_addr.s_addr) );
	if ( sRC == -1 )
		return -1;

	sRC = connect( fd, (struct sockaddr*)&addr, addr_len );
	if ( sRC == -1 && ( errno == EINTR || errno == EALREADY ))
	{
		FD_ZERO( &fset );
		FD_SET( fd, &fset );
		while ( ( ( ret = select( fd + 1, NULL, &fset, NULL, NULL ) ) == -1 ) && ( errno == EINTR )  )
		{
			FD_ZERO( &fset );
			FD_SET( fd, &fset );
		}
	}
	else if ( sRC == -1 )
	{
		return -1;
	}

	return fd;
}

