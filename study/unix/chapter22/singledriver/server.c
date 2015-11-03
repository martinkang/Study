#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include <sys/select.h>
#include "reentry.h"
#include "../uici.h"
#include "../uiciname.h"

typedef struct msg
{
	int pid;
	int tid;
	int msgno;
} msg_t;

#define BUFSIZE 255

static sem_t gSerSem;
static pthread_mutex_t gSerMutex = PTHREAD_MUTEX_INITIALIZER;

int gMsgNo = 0;

void *thread_fun( void *arg )
{
	int fd = 0;
	int size = 0;
	char buf[BUFSIZE] = {0, };

	fd = ((int*)arg)[0];

	while ( 1 )
	{
		size = read( fd, buf, sizeof( buf ) );
		if ( size <= 0 )
			break;

		r_fprintf( stdout, "p[%d] : %s\n", pthread_self(), buf );
		fflush( stdout );
		//	write( fd, "asdf", 4 );
	}
	close( fd );

	pthread_exit( NULL );
}

int handleresponse( int clntfd )
{
	int sRC = 0;
	int fd[0];

	pthread_t tid;
	fd[0] = clntfd;

	sRC = pthread_create( &tid, NULL, thread_fun, (void*)fd );
	if ( sRC == -1 )
	{
		close( clntfd );
		return -1;
	}
	return 0;
}

int main( int argc, char *argv[] )
{
	int sRC = 0;
	int i = 0;
	int size = 0;
	char buf[BUFSIZE] = {0, };

	int port = 0;
	int serfd = 0;
	int clntfd = 0;
	int newfd = 0;

	int nfds = 0;
	fd_set fdSet;
	fd_set fdSetRD;

	int msgno = 0;

	sRC = sem_init( &gSerSem, 1, 5 );
	if ( sRC == -1 )
		return -1;

	if ( argc != 2 )
	{
		fprintf( stderr, "argument error\n" );
		return -1;
	}

	port = atoi( argv[1] );
	
	serfd = u_open( port );
	if ( serfd == -1 )
	{
		r_fprintf( stderr, "socket open error\n" );
	}

	FD_ZERO( &fdSet );
	FD_SET( serfd, &fdSet );
	nfds = serfd + 1;

	fdSetRD = fdSet;

	fprintf( stderr, "server start\nwait client....\n" );
	while( 1 )
	{
		newfd = select( nfds, &fdSet, NULL, NULL, NULL );
		if ( newfd == serfd )
		{
			clntfd = u_accept( serfd );

			FD_SET( clntfd, &fdSetRD );
			nfds++;

			fprintf( stderr, "client connected! fd %d\n", clntfd );
		}
		else
		{
			size = read( newfd, buf, sizeof( buf ) );
			r_fprintf( stderr, "%s\n", buf );
	//		write( newfd, &msgno, sizeof(msgno) );
			msgno++;
		}
		fdSet = fdSetRD;
	}

	close( serfd );

	sem_destory( &gSerSem );
	return 0;
}
