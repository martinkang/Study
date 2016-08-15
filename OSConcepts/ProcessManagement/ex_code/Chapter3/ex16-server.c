#include <stdio.h>
#include <sys/types.h>                                            
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 9090
#define BACKLOG 5
#define BUF_SIZE 1024

#define ERR_TEST( a, b ) \
	if ( a ) \
	{		\
		perror( b ); \
		return -1; \
	}

int echoLoop( int aSerFd )
{
	char buf[BUF_SIZE] = { 0, };

	int clnt_fd = 0;
	socklen_t addrlen = 0;
	struct sockaddr_in addr;

	ssize_t sReadSize = 0;
	ssize_t sWriteSize = 0;

	while( 1 )
	{
		printf( "wait client accept()\n" );
		clnt_fd = accept( aSerFd, ( struct sockaddr*)&addr, &addrlen );
		ERR_TEST( clnt_fd == -1, "accept() error" );

		printf( "accept File descriptor %d\n", clnt_fd );

		sReadSize = read( clnt_fd, buf, BUF_SIZE - 1 );
		ERR_TEST( sReadSize < 0, "read() error" );
		
		printf( "recv Buf : " );
		while ( sReadSize > 0 )
		{
			buf[sReadSize] = '\0';
			printf( "%s", buf );
			
			sWriteSize = write( clnt_fd, buf, sReadSize );
			ERR_TEST( sReadSize != sWriteSize, "wirte() error" );

			sReadSize = read( clnt_fd, buf, BUF_SIZE - 1 );
			ERR_TEST( sReadSize < 0, "read() error" );
		}
		ERR_TEST( sReadSize < 0, "read() error" );
		printf( "\n" );

		close( clnt_fd );
	}
}

int main( void )
{
	int ser_fd = 0;

	socklen_t addrlen = 0;
	struct sockaddr_in serv_addr;

	ser_fd = socket( PF_INET, SOCK_STREAM, 0 );
	ERR_TEST( ser_fd == -1, "sock() error" );
	printf( "server socket() create success\n" );

	addrlen = sizeof( serv_addr );
	memset( &serv_addr, 0, addrlen );

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons( PORT );
	serv_addr.sin_addr.s_addr = htonl( INADDR_ANY );
	ERR_TEST( bind( ser_fd, (struct sockaddr *)&serv_addr, addrlen ) == -1,
			"bind() error" );
	printf( "bind() success\n" );

	ERR_TEST( listen( ser_fd, BACKLOG ) == -1, "listen() error" );
	printf( "listen() success\n" );

	ERR_TEST( echoLoop( ser_fd ) == -1, "echoLoop() Error" )
		
	return 0;
}
