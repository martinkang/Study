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

#define MAX_MSG_SIZE 255

int main( int argc, char *argv[] )
{
	int sRC = 0;

	int sPort = 0;
	int sFd = 0;
	char *sAdr = NULL;

	socklen_t sAddr_len = 0;
	struct sockaddr_in sAddr;

	int sReadSize = 0;
	int sWriteSize = 0;

	char sMsg[MAX_MSG_SIZE + 1] = {0, };

	if ( argc != 3 )
	{
		fprintf( stderr, "argument error\n" );
		return -1;
	}

	sPort = atoi( argv[1] );
	sAdr = argv[2];
	
	sFd = socket( PF_INET, SOCK_STREAM, 0 );
	if ( sFd == -1 )
	{
		perror( "socket() error" );
		exit( EXIT_FAILURE );
	}

	sAddr_len = sizeof( sAddr );
	memset( &sAddr, 0, sAddr_len );
	sAddr.sin_family = AF_INET;
	sAddr.sin_port = htons( sPort );
	sAddr.sin_addr.s_addr = inet_addr( sAdr );

	sRC = connect( sFd, (struct sockaddr*)&sAddr, sAddr_len );
	if ( sRC == -1 )
	{
		perror( "connect() error" );
		exit( EXIT_FAILURE );
	}

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
	}

	close( sFd );

	return 0;
}

