#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "main.h"
#include "logger.h"
#include "que.h"

static que_t gQue;

static pthread_mutex_t gQueMutex = PTHREAD_MUTEX_INITIALIZER;

int getNumQue( void )
{
	int sRC = 0;
	int num = -1;

	sRC = pthread_mutex_lock( &gQueMutex );
	if ( sRC == -1 )
		return -1;

	num = getQueNum( &gQue );

	pthread_mutex_unlock( &gQueMutex );

	return num;
}

int queue_data( void *aData, int size )
{
	int sRC = 0;

	sRC = pthread_mutex_lock( &gQueMutex );
	if ( sRC == -1 )
		return -1;

	sRC = queue( &gQue, aData, size );
	if ( sRC == -1 )
	{
		pthread_mutex_unlock( &gQueMutex );
		return -1;
	}

	sRC = pthread_mutex_unlock( &gQueMutex );

	return sRC;
}

int dequeue_data( void *aData, int size )
{
	int sRC = 0;
	
	pthread_mutex_lock( &gQueMutex );
	if ( sRC == -1 )
		return -1;

	sRC = dequeue( &gQue, aData, size );
	if ( sRC == -1 )
	{
		pthread_mutex_unlock( &gQueMutex );
		return 1;
	}

	sRC = pthread_mutex_unlock( &gQueMutex );

	return sRC;
}

int logflush( FILE *fd )
{
	int n = 0;
	char msg[BUFSIZE] = {0, };

	while ( dequeue_data( (void*)msg, sizeof( msg ) ) == 0 ) 
	{
		n++;
		fputs( (char*)msg, fd );
	}

	return n;
}

void *loggerFunction( void )
{
	int sRC = 0;
	int n = 0;
	char msg[BUFSIZE] = {0, };

	FILE *fd;
	char path[] = "log.txt";

	fd = fopen( path, "a+" );
	if ( fd == NULL )
	{
		r_perror( "log file create error\n" );
		return;
	}

	initQue( &gQue );

	while ( isExit() == 0 )
	{
		if ( getNumQue() > 0 )
		{
			memset( msg, 0, BUFSIZE );
			while ( dequeue_data( (void*)msg, sizeof( msg ) ) == 0 ) 
			{
				n++;
				fputs( (char*)msg, fd );
			}
		}
		sleep( 1 );
	}

	n = n + logflush( fd );

	fclose( fd );

	printf( "loger recieved %d msg\nlogger end\n", n );

	return;
}
