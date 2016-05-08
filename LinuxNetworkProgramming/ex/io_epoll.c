#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 9090
#define DFL_OPEN_MAX_FILES 8192
#define LISTEN_BACKLOG 20

#define BUF_SIZE 1024

#define MAX(a, b) ( a >= b ? a : b )
#define MIN(a, b) ( a < b ? a : b )

int newClient( int aEpollFd, int aFdListener );
int createAndInitializeEpoll( const int aMaxOpenFiles, struct epoll_event **aEpEvent );

int createAndBindSocket( const int aPort, struct sockaddr_in *aAddrMy );

int main( int argc, char *argv[] )
{
    int sRC = 0;

    int i = 0;

    int sPort = 0;
    socklen_t sLenAddr;
    struct sockaddr_in sAddrMy;

	int sFd;
    int sFdListener;
    int sNRecv;

	int sMaxOpenFiles = 0;
	
	int sRetPoll = 0;
	int sEpollFd = 0;
	struct epoll_event *sEpEvents = NULL; /* epoll_wait 의 반환 이벤트를 저장할 공강 */

	char sBuf[BUF_SIZE] = {0, };

    memset( (void*)(&sAddrMy), 0, sizeof( struct sockaddr_in ) );

	sFdListener = createAndBindSocket( PORT, &sAddrMy );
	sMaxOpenFiles = MIN( (int)sysconf(_SC_OPEN_MAX), (int)DFL_OPEN_MAX_FILES );
	printf( "Max Open Files : %d\n", sMaxOpenFiles );

	sEpollFd = createAndInitializeEpoll( sMaxOpenFiles, &sEpEvents );
	addEv( sEpollFd, sFdListener );	

	while( 1 )
	{
		printf( "Epoll Waiting...\n" );
		sRetPoll = epoll_wait( sEpollFd, sEpEvents, sMaxOpenFiles, -1 );
		if ( sRetPoll < 0 )
		{
			fprintf( stderr, "epoll_wait error\n" );
		}
		else
		{
			printf( "Epoll return : %d\n", sRetPoll );

			for ( i = 0; i < sRetPoll; i++ )
			{
				if ( sEpEvents[i].events & EPOLLIN )
				{
					if ( sEpEvents[i].data.fd == sFdListener )
					{
						sRC = newClient( sEpollFd, sFdListener );
						if ( sRC == -1 )
						{
							fprintf( stderr, "accept() error\n%s\n", strerror( errno ) );
							continue;
						}
					}
					else
					{
						sNRecv = recv( sEpEvents[i].data.fd, sBuf, BUF_SIZE, 0 );
						if ( sNRecv == -1 )
						{
							fprintf( stderr, "recv() error : %s\n", strerror(errno ) );
							continue;
						}
						if ( sNRecv == 0 )
						{
							printf( "fd(%d) was closed\n", sEpEvents[i].data.fd );
							delEv( sEpollFd, sEpEvents[i].data.fd );
						}
						else
						{
							printf( "fd(%d) : %d bytes received\n%s", sEpEvents[i].data.fd, sNRecv, sBuf );
							send( sEpEvents[i].data.fd, "ack", 3, 0 );
						}
					}
				}
				else if (sEpEvents[i].events & EPOLLERR )
				{
					fprintf( stderr, "fd(%d) epoll event(%d) Error( %d:%s)\n", 
							sEpEvents[i].data.fd, sEpEvents[i].events, errno, strerror(errno) );
				}
			}
		}
	}

	return 0;
}

int createAndInitializeEpoll( const int aMaxOpenFiles, struct epoll_event **aEpEvent )
{
	int sFd = 0;

	sFd = epoll_create( aMaxOpenFiles );
	if ( sFd == -1 )
	{
		perror( "Epoll create() error" );
		exit( EXIT_FAILURE );
	}

	(*aEpEvent) = calloc( 1024, sizeof( struct epoll_event ) );
	if ( (*aEpEvent) == NULL )
	{
		perror( "calloc error" );
		exit( EXIT_FAILURE );
	}
	return sFd;
}

int createAndBindSocket( const int aPort, struct sockaddr_in *aAddrMy )
{
	int sRC = 0;

    int sFdListener = 0;
    socklen_t sLenAddr = 0;
	int sOptVal = 1;

    sFdListener = socket( AF_INET, SOCK_STREAM, IPPROTO_IP );
    if ( sFdListener == -1 )
    {
        perror( "Fail: socket()" );
        exit( EXIT_FAILURE );
    }

	sRC = setsockopt( sFdListener, SOL_SOCKET, SO_REUSEADDR, (void*)&sOptVal, sizeof( sOptVal ) );
    if ( sRC == -1 )
    {
        perror( "setsockopt() error" );
        exit( EXIT_FAILURE );
    }

   /* for nonblock socket */
    sRC = fcntl( sFdListener, F_SETFL, O_NONBLOCK );
	if ( sRC == -1 )
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
    listen( sFdListener, LISTEN_BACKLOG );

    return sFdListener;
}

int addEv(int aEpollFd, int aFd )
{
	int sRC = 0;
	struct epoll_event sEv;

	sEv.events = EPOLLIN ;	/* check inbound data */
	sEv.data.fd = aFd;
	sRC = epoll_ctl(aEpollFd, EPOLL_CTL_ADD, aFd, &sEv);
	if ( sRC == -1) 
	{
		fprintf( stderr, "fd(%d) EPOLL_CTL_ADD  Error(%d:%s)", aFd, errno, strerror(errno) );
		return -1;
	}

	return 0;
}

int delEv(int aEpollFd, int aFd)
{
	int sRC = 0;

	sRC = epoll_ctl( aEpollFd, EPOLL_CTL_DEL, aFd, NULL);
	if ( sRC == -1 )
	{
		fprintf( stderr, "fd(%d) EPOLL_CTL_DEL Error(%d:%s)", aFd, errno, strerror(errno) );
		return -1;
	}

	close(aFd);
	return 0;
}
int newClient( int aEpollFd, int aFdListener  )
{
	int sRC = 1;

	int sFd = 0;
	struct sockaddr_in sAddr;
	socklen_t sLenAddr;

    memset( (void*)(&sAddr), 0, sizeof( struct sockaddr_in ) );

	while(1)
	{
		sFd = accept( aFdListener, (struct sockaddr*)&sAddr, &sLenAddr );	
		if ( sFd == -1 )
		{
			if ( errno == EAGAIN ) /* no more new connection */
			{
				break;
			}
			else
			{
				sRC = -1;
			}
		}
		addEv( aEpollFd, sFd );
	}

	return sRC;
}
