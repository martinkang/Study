#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include "../uici.h"
#include "../uiciname.h"

#define PORT 9090
#define BUFSIZE 255

static int msgNo = 0;

static pthread_mutex_t gMutex = PTHREAD_MUTEX_INITIALIZER;

static int getnum()
{
	int sRC = 0;

	sRC = pthread_mutex_lock( &gMutex );
	if ( sRC == -1 )
		return sRC;

	msgNo++;

	sRC = pthread_mutex_unlock( &gMutex);
	if ( sRC == -1 )
		return sRC;
	
	return msgNo;
}

void *childfun( void *arg )
{
	int sRC = 0;
	int clntfd;
	int size = 0;

	int no = 0;

	char buf[BUFSIZE] = {0, };
	char msg[BUFSIZE] = {0, };

	clntfd = (int)arg;
	while ( 1 )
	{
		size = r_read( clntfd, buf, BUFSIZE );
		if ( size == 0 )
			break;

		if ( size > 0 )
			buf[size] = '\0';

		if ( size != strlen( buf ) )
		{
			r_printf( "size : %d\nstrlen(buf) : %d\n", size, strlen(buf) );
			r_perror( "read error" );
			close( clntfd );
			return;
		}
		r_printf( "Pthread : %d\n recv : %s\n", pthread_self(), buf );

		no = getnum();		
		snprintf( msg, BUFSIZE, "p[%d] MSG[%d]\n", pthread_self(), no );
		size = r_write( clntfd, msg, strlen( msg ) );
		r_printf( "Msg No : %d\n", no );

		if ( size > 0 )
			msg[size] = '\0';

		if ( size != strlen( msg ) )
		{
			r_perror( "write error" );
			close( clntfd );
			return;
		}
	}

	close( clntfd );
	return ;
}

pthread_t *creatTh()
{
	pthread_t *newTh = (pthread_t*)malloc( sizeof( pthread_t ) );
	return newTh;
}

int main( void )
{
	int sRC = 0;
	int i = 0;
	int j = 0;
	int num = 0;
	int n = 0;

	int pid = 0;
	int serfd;
	int clntfd;

	pthread_t *th = NULL;

	serfd = u_open( PORT );
	if ( serfd == -1 )
	{
		fprintf( stderr, "socket create error\n" );
		return -1;
	}

	fprintf( stderr, "wait for client.....\n" );
	while ( 1 )
	{
		clntfd = u_accept( serfd );
		if ( clntfd == -1 )
		{
			r_fprintf( stderr, "client connect error\n" );
			continue;
		}

		r_printf( "client connected\n" );
		
		th = creatTh();

		sRC = pthread_create( th, NULL, childfun, (void*)clntfd );
		if ( sRC < 0 )
		{
			r_fprintf( stderr, "fork error\n" );
			close( clntfd );
			continue;
		}
	}

	close( serfd );

	return 0;
}
