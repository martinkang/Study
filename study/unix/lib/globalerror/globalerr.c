#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "stack.h"

#define BUF_SIZE 255

static stack gStk;

static pthread_mutex_t gErrLock = PTHREAD_MUTEX_INITIALIZER;

static int isInit = 1;

int getErr( int *err )
{
	int sRC = 0;
	sRC = pthread_mutex_lock( &gErrLock );
	if ( sRC != 0 )
		return sRC;
	
	s_pop( &gStk, err );

	sRC = pthread_mutex_unlock( &gErrLock );
	return sRC;
}
int setErr( int err )
{
	int sRC = 0;
	
	sRC = pthread_mutex_lock( &gErrLock );
	if ( sRC != 0 )
		return sRC;

	s_push( &gStk, err );

	sRC = pthread_mutex_unlock( &gErrLock );
	return sRC;
}

int showErr( void )
{
	int sRC = 0;
	char result[BUF_SIZE] = {0, };
	DATA sData = 0;

	sRC = pthread_mutex_lock( &gErrLock );
	if ( sRC != 0 )
		return sRC;

	while( ( sRC = s_pop( &gStk, &sData ) ) == 1 )
	{
		snprintf( result, BUF_SIZE, "Error : %d", sData );
		write( STDOUT_FILENO, result, strlen( result ) );
	}

	if ( sRC == -1 )
	{
		pthread_mutex_unlock( &gErrLock );
		return sRC;
	}

	sRC = pthread_mutex_unlock( &gErrLock );
	return sRC;
}

void initMutex( void )
{
	if ( isInit == 0 )
	{
		pthread_mutex_init( &gErrLock, NULL );

		s_init( &gStk );

		isInit = 1;
	}
}

void destroyMutex( void )
{
	if ( isInit == 1 )
	{
		pthread_mutex_destroy( &gErrLock );

		s_finalize( &gStk );
		
		isInit = 0;
	}
}

