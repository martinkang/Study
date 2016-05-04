#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <poll.h>

#define MAX_FD_SOCKET 255
#define BUF_SIZE 1024
#define LISTEN_BACKLOG 5

struct pollfd gPollFds[MAX_FD_SOCKET];
int gCntFdSocket; /* pollfds[] 배열에 저장된 소켓 파일 기술자 개수 */

int createAndBindSocket( const int aPort, sockaddr_in *aAddrMy );
void newAccept( const int aFdListener, sockaddr_in *aAddrOther );
int addSocket( int aFd );
int delSocket( int aFd );

int main( int argc, char *argv[] )
{
	int i = 0;

	int sPort = 0;
	socklen_t sLenAddr;
	struct sockaddr_in sAddrMy, sAddrOther;

	int sFdListener;
	int sNRecv;

	char sBuf[BUF_SIZE] = {0, };
	int sRetPoll;

	memset( &sAddrMy, 0x00, sizeof( struct sockaddr_in ) );
	memset( &sAddrOther, 0x00, sizeof( struct sockaddr_in ) );

	int ( argc != 2 )
	{
		printf( "Usage : %s < listen port>\n", argv[0] );
	}
	else if ( argc == 2 )
	{
		sPort = atoi( (char*)argv[1] );
	}

	for( i = 0; i < MAX_FD_SOCKET; i++ )
	{
		gPollFds[i] = -1;
	}

	sFdListener = createAndBindSocket( sPort, &sAddrMy );

	/* pollfds 의 0 번째에 리스너 소켓을 저장한다. */
	addSocket( sFdListener );
	while( 1 )
	{
		sRetPoll = poll( gPollFds, gCntFdSocket, -1 );
		if ( sRetPoll == -1 )
		{
			perror( "poll() error\n" );
			break;
		}
		printf( "poll return : %d\n", sRetPoll );

		if ( gPollFds[0].revents & POLLLIN ) /* 새로운 연결 요청이면 */
		{
			newAccept( sFdListener, &sAddrOther );
		}

		for ( i = 0; i < gCntFdSocket; i++ )
		{

#ifdef ENABLE_MSG_OOB /* OOB 데이터를 처리하는 경우만 해당되는 소스 코드다 */

			if ( gPollFds[i].revents & POLLPRI ) /* OOB 데이터 */
			{
				printf( "Urgent data detected\n" );
				while( 1 )
				{
				}
			}
#endif
			if ( gPollFds[i].revents & POLLIN ) /* 일반 데이터 수신 */
			{

			}
		}
	}
}


int createAndBindSocket( const int aPort, sockaddr_in *aAddrMy )
{
	int sFdListener = 0;
	socklen_t sLenAddr = 0;

	sFdListener = socket( AF_INET, SOCK_STREAM, IPPROTO_IP );
	if ( sFdListener == -1 )
	{
		perror( "Fail: socket() " );
		exit( EXIT_FAILURE );
	}

	/* for nonblock socket */
	if ( fcntl( sFdListener, F_SETFL, O_NONBLOCK ) == -1 )
	{
		exit( EXIT_FAILURE );
	}

	aAddrMy->sin_family = AF_INET;
	aAddrMy->sin_addr.s_addr = INADDR_ANY;
	aAddrMy->sin_port = htons( port );

	sLenAddr = sizeof( sockaddr_in );
	if ( bind( sFdListener, ( struct sockaddr * )aAddrMy, sLenAddr ) == -1 )
	{
		perror( "Fail bind()" );
		eixt( EXIT_FAILURE );
	}
	fprintf( "Port : %d\n", sPort );

	listen( sFdListener, LISTEN_BACKLOG );

	return sFdListener;
}

void newAccept( const int aFdListener, sockaddr_in *aAddrOther )
{
	int sFd = 0;
	socklen_t sLenAddr = 0;

	/* 한 번에 여러 개의 접속 요청이 있을 때 poller 가 여러번 호출되는 것을 막기 위해서
	   한번 리스너 소켓에 연결 요청이 있는 경우 모두 처리하기 위하여 while 을 이용한다.
	   non block 소켓을 지정했으므로 더 이상 받을 연결이 없으면 EAGAIN 에러가 발생한다. */
	while( 1 )
	{
		sFd = accept( aFdListener, ( struct sockaddr * )aAddrOther, &sLenAddr );
		if ( sFd == -1 )
		{
			if ( errno == EAGAIN )
			{
				/* 더 이상 연결 요청이 없으므로 루프를 빠져나간다 */
				break;
			}
			else
			{
				perror( "accept() error" );
				break;
			}
		}
		if ( addSocket( sFd ) == -1 )
		{
			fprintf( stderr, "too many client connected. force to disconnect.\n" );
			break;
		}
		printf( "Add socket %d\n", sFd );
	}
}

int addSocket( int aFd )
{
	int sRC = 0;

	if ( gCntFdSocket < MAX_FD_SOCKET )
	{
		gPollFds[gCntFdSocket].fd = aFd;
#ifdef ENABLE_NSG_OOB
		/* OOB 데이터를 처리하려면 POLLRPI 이벤트도 감시해야 한다 */
		gPollFds[gCntFdSocket].events = POLLIN | POLLPRI;
#else
		gPollFds[gCntFdScoket].events = POLLIN;
#endif
		sRC = gCntFdSocket + 1;
	}
	else
	{
		sRC = -1;
	}

	return sRC;
}

int delSocket( int aFd )
{
	int i = 0;
	int flag = -1; /* 1 : found, -1 : not found */

	for( i = 0; i < gCntFdSocket; i++ )
	{
		if ( gPollFds[i].fd == aFd )
		{
			close( aFd );
		
			gPollFds[i] = gPollFds[gCntFdSocket - 1];
			gPollFds[gCntFdSocket -1] = -1;
			flag = 1;

			gCntFdSocket--;
		}
	}

	return flag;
}

void prSocket()
{
}

