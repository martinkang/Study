#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 1024

#define PORT 9090

int recvData( int aServFd );

int main( int argc, char *argv[] )
{
	int sRC = 0;

	int servFd;
	struct sockaddr_in sAddr;

	int sRecv = 0;
	char sBuf[BUF_SIZE] = {0, };

	memset( &sAddr, 0, sizeof( sAddr ) );

	servFd = socket( AF_INET, SOCK_DGRAM, 0 );
	if ( servFd == -1 )
	{
		perror( "socket() error" );
		exit( EXIT_FAILURE );
	}

	sAddr.sin_family = AF_INET;
	sAddr.sin_addr.s_addr = INADDR_ANY;
	sAddr.sin_port = htons( PORT);
	sRC = bind(  servFd, (struct sockaddr *)&sAddr, sizeof( sAddr ) );
	if ( sRC == -1 )
	{
		perror( "bind() error" );
		exit( EXIT_FAILURE );
	}

	while( 1 )
	{
		sRC = recvData( servFd );
		
		if ( sRC == -1 )
			break;
	}

}

int recvData( int aServFd )
{
	int sRC = 0;

	int sNRecv = 0;
	socklen_t sAddrLen = 0;
	struct sockaddr_in sAddr;

	char sBuf[BUF_SIZE] = {0, };

	sAddrLen = sizeof( sAddr );
	memset( &sAddr, 0, sAddrLen );

	sNRecv = 0;

	sNRecv = recvfrom( aServFd, sBuf, BUF_SIZE,
			0, (struct sockaddr *)&sAddr, &sAddrLen );
	if ( sNRecv == -1 )
	{
		perror( "recvFrom() error" );
		sRC = -1;	
	}
	else
	{
		sBuf[sNRecv] = '\0';
		printf( "[recvfrom IP : %s] ( %d byte )\n%s", 
				inet_ntoa( sAddr.sin_addr ), sNRecv, sBuf ); 
	}

	return sRC;
}

