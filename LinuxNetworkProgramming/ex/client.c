#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>

#define MAX_MSG_SIZE 255

int createAndConnectSocket( const int aPort, const char *aAddr, struct sockaddr_in *aAddrMy );

int main( int argc, char *argv[] )
{
	int sPort = 0;
	int sFd = 0;
	char *sAdr = NULL;

	struct sockaddr_in sAddr;

	int sReadSize = 0;
	int sWriteSize = 0;

	char sMsg[MAX_MSG_SIZE + 1] = {0, };
	
	if ( argc != 3 )
	{
		fprintf( stderr, "argument error\n" );
		return -1;
	}

	memset( &sAddr, 0, sizeof( sAddr ) );
	sPort = atoi( argv[1] );
	sAdr = argv[2];

	sFd = createAndConnectSocket( sPort, sAdr, &sAddr );

	while( 1 )
	{
		fprintf( stderr, "message : " );
		sReadSize = read( STDIN_FILENO, sMsg, MAX_MSG_SIZE );
		if ( sReadSize < 0 )
		{
			fprintf( stderr, "client error\n" );
			break;
		}

		if ( strncmp( sMsg, ".exit", 4 ) == 0 )
		{
			fprintf( stderr, "disconnect server and exit\n" );
			break;
		}

		sWriteSize = write( sFd, sMsg, sReadSize );
		if ( sReadSize != sWriteSize )
		{
			fprintf( stderr, "send msg error\n" );
			break;
		}

		sReadSize = read( sFd, sMsg, MAX_MSG_SIZE );
		if ( sReadSize == 0 )
		{
			fprintf( stderr, "server is disconnected\n" );
			break;
		}	

	}

	close( sFd );
	return 0;
}

int createAndConnectSocket( const int aPort, const char *aAddr, struct sockaddr_in *aAddrMy )
{
	int sRC = 0;

	int sFd = 0;
	socklen_t sLenAddr = 0;

	sFd = socket( AF_INET, SOCK_STREAM, IPPROTO_IP );
	if ( sFd == -1 )
	{
		perror( "Fail: socket()" );
		exit( EXIT_FAILURE );
	}

	fprintf( stderr, "Port : %d\n", aPort );
	fprintf( stderr, "IP   : %s\n", aAddr );

	aAddrMy->sin_family = AF_INET;
	aAddrMy->sin_port = htons( aPort );
    aAddrMy->sin_addr.s_addr = inet_addr( aAddr );

    sLenAddr = sizeof( (*aAddrMy) );

	sRC = connect( sFd, (struct sockaddr*)aAddrMy, sLenAddr );
	if ( sRC == -1 )
	{
		perror( "connect() error" );
		exit( EXIT_FAILURE );
	}

    return sFd;
}

