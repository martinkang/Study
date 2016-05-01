#include <stdio.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define LISTEN_BACKLOG 20
#define PORT 9090

#define MAX_FD_SOCKET 0xff

#define MAX_RECV_SIZE 255
#define SUCCESS 0
#define FAILURE -1

#define MAX(a, b) ( a>= b ? a : b )

int gIsConnect[MAX_FD_SOCKET] = {0, };

void clearClientFd()
{
	int i = 0;

	printf( "clear Client File discriptor\n" );
	for ( i = 0; i < MAX_FD_SOCKET; i++ )
	{
		if ( gIsConnect[i] == 1 )
		{
			printf( "File discriptor[%d] is close\n", i );
			close( i );
		}
	}
}


int newAccept( int aFd, fd_set *aFd_Read, int aFdMax )
{
	int sNewFd = -1;
	struct sockaddr_in sNewAddr;
	socklen_t sNewAddr_len = 0;

	int sFdMax = aFdMax;

	memset( (void*)&sNewAddr, 0, sizeof( sNewAddr ) );

	sNewFd = accept( aFd, (struct sockaddr*)&sNewAddr, &sNewAddr_len );
	if ( sNewFd == -1 )
	{
		if ( errno == EAGAIN )
		{
			perror( "No more exist new connection" );
		}
	}
	else
	{
		gIsConnect[sNewFd] = 1;

		FD_SET( sNewFd, aFd_Read );
		sFdMax = MAX( aFdMax, sNewFd );

		printf( "new File descriptor %d is accepted\n", sNewFd );
		sNewFd = -1;
	}

	return sFdMax;
}

void recvData( int aFd, fd_set * aFd_Read )
{
	int sRecv = 0;

	char sMsg[MAX_RECV_SIZE + 1] = {0, };

	sRecv = read( aFd, (void*)sMsg, MAX_RECV_SIZE );
	if ( sRecv == 0 )
	{
		gIsConnect[aFd] = 0;

		printf( "File descriptor %d is closed\n", aFd );
		FD_CLR( aFd, aFd_Read );
		close( aFd );
	}
	else if ( sRecv < 0 )
	{
		printf( "recv() error\n" );
	}
	else
	{
		sMsg[sRecv] = '\0';
		printf( "recv From File descriptor[%d] : %s\n", aFd, sMsg );
	}

}

int main( int argc, char * argv[] )
{
	int sRC = FAILURE;
	int i = 0;

	int sFd = -1;
	int sFdMax = -1;
	int sRetVal = -1;
	struct sockaddr_in sAddr;
	struct timeval sTv;

	int sRecv = 0;

	int sOptVal = 1; /* for set sockopt */

	fd_set sFd_Read;
	fd_set sFd_Read_Cpy;

	memset( (void*)&sAddr, 0, sizeof( sAddr ) );

	sFd = socket( AF_INET, SOCK_STREAM, 0 );
	if ( sFd == FAILURE )
	{
		perror( "socket() error");
		exit( EXIT_FAILURE );
	}

	sRC = fcntl( sFd, F_SETFL, O_NONBLOCK );
	if ( sRC == FAILURE )
	{
		perror( "fcntl() error" );
		exit( EXIT_FAILURE );
	}

	sRC = setsockopt( sFd, SOL_SOCKET, SO_REUSEADDR, (void*)&sOptVal, sizeof( sOptVal ) );
	if ( sRC == FAILURE )
	{
		perror( "setsockopt() error" );
		exit( EXIT_FAILURE );
	}

	sAddr.sin_family = AF_INET;
	sAddr.sin_addr.s_addr = INADDR_ANY;
	sAddr.sin_port = htons( PORT );

	sRC = bind( sFd, (struct sockaddr *)&sAddr, sizeof( sAddr) );
	if ( sRC == FAILURE )
	{
		perror( "bind() error" );
		exit( EXIT_FAILURE );
	}

	sRC = listen( sFd, LISTEN_BACKLOG );
	if ( sRC == FAILURE )
	{
		perror( "listen() error" );
		exit( EXIT_FAILURE );
	}

	FD_ZERO( &sFd_Read );
	FD_SET( sFd, &sFd_Read );
	sFdMax = sFd;

	while( 1 )
	{
		sTv.tv_sec = 5;
		sTv.tv_usec = 0;

		sFd_Read_Cpy = sFd_Read;
		sRetVal = select( sFdMax + 1, &sFd_Read_Cpy, NULL, NULL, &sTv );
		if ( sRetVal == -1 )
		{
			perror( "select() error" );
			break;
		}
		else if ( sRetVal == 0 )
		{
			fprintf( stderr, "select() timeout\n" );
			break;
		}

		printf( "select() return %d\n", sRetVal );

		if ( FD_ISSET( sFd, &sFd_Read_Cpy ) == 1 )
		{
			/* 새로운 연결 요청 감지 */
			sFdMax = newAccept( sFd, &sFd_Read, sFdMax );
			continue;
		}

		for ( i = 0; i < sFdMax + 1; i++ )
		{
			if ( FD_ISSET( i, &sFd_Read_Cpy ) == 1 )
			{
				/* client 로부터 데이터를 전송받거나 연결을 해제함  */
				recvData( i, &sFd_Read );
			}
		}
	}

	clearClientFd();

	close( sFd );

	return EXIT_SUCCESS;
}

