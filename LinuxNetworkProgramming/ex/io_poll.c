#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define MAX_FD_SOCKET 255
#define BUF_SIZE 1024
#define LISTEN_BACKLOG 5

struct pollfd gPollFds[MAX_FD_SOCKET];
int gCntFdSocket = 0; /* pollfds[] 배열에 저장된 소켓 파일 기술자 개수 */

int recvData( const int aIdx );
int createAndBindSocket( const int aPort, struct sockaddr_in *aAddrMy );
void newAccept( const int aFdListener, struct sockaddr_in *aAddrOther );
int addSocket( int aFd );
int delSocket( int aFd );

int main( int argc, char *argv[] )
{
	int sRC = 0;

	int i = 0;

	int sPort = 0;
	socklen_t sLenAddr;
	struct sockaddr_in sAddrMy, sAddrOther;

	int sFdListener;
	int sNRecv;

	int sRetPoll;

	memset( (void*)(&sAddrMy), 0, sizeof( struct sockaddr_in ) );
	memset( (void*)(&sAddrOther), 0, sizeof( struct sockaddr_in ) );

	if ( argc != 2 )
	{
		printf( "Usage : %s < listen port>\n", argv[0] );
	}
	else if ( argc == 2 )
	{
		sPort = atoi( (char*)argv[1] );
	}

	for( i = 0; i < MAX_FD_SOCKET; i++ )
	{
		gPollFds[i].fd = -1;
	}

	sFdListener = createAndBindSocket( sPort, &sAddrMy );
	fprintf( stderr,"sFdListener : %d\n", sFdListener );

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
		else if ( sRetPoll > 0 )
		{
			printf( "poll return : %d\n", sRetPoll );

			if ( gPollFds[0].revents & POLLIN ) /* 새로운 연결 요청이면 */
			{
				newAccept( sFdListener, &sAddrOther );
			}

			for ( i = 1; i < gCntFdSocket; i++ )
			{
				if ( gPollFds[i].revents & POLLIN ) /* 일반 데이터 수신 */
				{
					sRC = recvData( i );
					if ( sRC == 0 )
					{
						delSocket( gPollFds[i].fd );
						i--; /* 맨 마지막 소켓과 바꿔치기 했으므로 i 를 -- 해줘야 한다 */
					}
				}
				if ( gPollFds[i].revents & POLLERR || gPollFds[i].revents & POLLNVAL ) 
				{
					fprintf( stderr, "ERROR received\n" );
				}
			}
		}
	}

	return 0;
}


int createAndBindSocket( const int aPort, struct sockaddr_in *aAddrMy )
{
	int sFdListener = 0;
	socklen_t sLenAddr = 0;

	sFdListener = socket( AF_INET, SOCK_STREAM, IPPROTO_IP );
	if ( sFdListener == -1 )
	{
		perror( "Fail: socket()" );
		exit( EXIT_FAILURE );
	}

	/* for nonblock socket */
	if ( fcntl( sFdListener, F_SETFL, O_NONBLOCK ) == -1 )
	{
		exit( EXIT_FAILURE );
	}

	aAddrMy->sin_family = AF_INET;
	aAddrMy->sin_addr.s_addr = INADDR_ANY;
	aAddrMy->sin_port = htons( aPort );

	sLenAddr = sizeof( (*aAddrMy) );
	if ( bind( sFdListener, ( struct sockaddr * )aAddrMy, sLenAddr ) == -1 )
	{
		perror( "Fail bind()" );
		exit( EXIT_FAILURE );
	}
	fprintf( stderr, "Port : %d\n", aPort );

	listen( sFdListener, LISTEN_BACKLOG );

	return sFdListener;
}

void newAccept( const int aFdListener, struct sockaddr_in *aAddrOther )
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
		else
		{
			send( sFd, "accepted", 8, 0 );	
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
		gPollFds[gCntFdSocket].events = POLLIN;
		sRC = gCntFdSocket + 1;
		gCntFdSocket++;
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
			gPollFds[gCntFdSocket -1].fd = -1;
			flag = 1;

			gCntFdSocket--;
		}
	}

	return flag;
}

int recvData( const int aIdx )
{
	int sRecv = 0;
	char sBuf[BUF_SIZE] = {0, };

	sRecv = recv( gPollFds[aIdx].fd, sBuf, sizeof( sBuf ), 0 );
	if ( sRecv == -1 )
	{
		perror( "recv Data error" );
		return;
	}

	if ( sRecv == 0 )
	{
		fprintf( stderr, "fd(%d) was closed by foriegn host\n", gPollFds[aIdx].fd );
	}
	else
	{
		sBuf[sRecv] = '\0';
		printf( "fd(%d) %d bytes received\n%s", gPollFds[aIdx].fd, sRecv, sBuf );
		send( gPollFds[aIdx].fd, "ack", 3, 0 );
	}

	return sRecv;
}
