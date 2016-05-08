#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 1024

#define PORT 9090

int sendData( int aServFd, char *aMsg );

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

	while( 1 )
	{
		fprintf( stderr, "input msg : " );	
		sRecv = read( STDIN_FILENO, sBuf, BUF_SIZE );
		if ( sRecv == -1 )
		{
			perror( "read() error" );
			break;
		}	
		else
		{
			sBuf[sRecv] = '\0';
		}

		sRC = sendData( servFd, sBuf );
		if ( sRC == -1 )
		{
			perror( "sendto() error" );
			break;
		}
	}
	return 0;
}

int sendData( int aServFd, char *aMsg )
{
    int sRC = 0;
    
    socklen_t sAddrLen = 0;
    struct sockaddr_in sAddr;
    
    int sPort = 9090;
    char *sIP = "127.0.0.1";
    
    sAddrLen = sizeof( sAddr );
    
    sAddr.sin_family = AF_INET;
    sAddr.sin_addr.s_addr = inet_addr( sIP );
    sAddr.sin_port = htons( sPort );
    
    sRC = sendto( aServFd, aMsg, strlen( aMsg ), 0, (struct sockaddr *)&sAddr, sAddrLen );
    
    return sRC;
}


