#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>                                                
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 9090
#define ADDR "127.0.0.1"

#define BUF_SIZE 1024

#define ERR_TEST( a, b ) \
    if ( a ) \
{       \
	perror( b ); \
	return -1; \
}

int main( void )
{
	int fd = 0;

	socklen_t addrlen = 0;                                       
	struct sockaddr_in addr;

	char buf[BUF_SIZE] = { 0 , };
	char readBuf[BUF_SIZE] = { 0 , };

	ssize_t sSize = 0;
	ssize_t sWriteSize = 0;
	ssize_t sReadSize = 0;

	fd = socket( PF_INET, SOCK_STREAM, 0 );
    	ERR_TEST( fd == -1, "sock() error" );
	printf( "client socket() success\n" );

	addrlen = sizeof( addr ); 
	memset( &addr, 0, addrlen );

	addr.sin_family = AF_INET;
	addr.sin_port = htons( PORT );
	addr.sin_addr.s_addr = inet_addr( ADDR );
	ERR_TEST( connect( fd, ( struct sockaddr *)&addr, addrlen ) == -1, "connect() error" );
	printf( "connect() to server success\n" );

	fprintf( stderr, "input : " );
	(void*)gets( buf );
	sSize = strlen( buf );

	sWriteSize = write( fd, buf, sSize );
	ERR_TEST( sWriteSize < 0, "write() error" );

	sReadSize = read( fd, readBuf, sSize );
	ERR_TEST( sReadSize < 0, "read() error" );

	readBuf[sReadSize] = '\0';
	fprintf( stderr, "echo : %s", readBuf );
	sSize = sReadSize;

	while( sSize < sWriteSize )
	{
		sReadSize = read( fd, readBuf, sSize );
		ERR_TEST( sReadSize < 0, "read() error" );

		readBuf[sReadSize] = '\0';
		fprintf( stderr, "%s", readBuf );
		sSize += sReadSize;
	}		

	fprintf( stderr, "\n" );

	close( fd );

	return 0;
}
