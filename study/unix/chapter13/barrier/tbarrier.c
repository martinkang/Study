#include <errno.h>
#include <unistd.h>
#include <pthread.h>

static pthread_mutex_t bmutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t bcond = PTHREAD_COND_INITIALIZER;
static int gCount = 0;
static int gLimit = 0;

int initBarrier( int n )
{
	int sRC = 0;

	sRC = pthread_mutex_lock( &bmutex );
	if ( sRC != 0 )
		return sRC;

	if ( gLimit == 0 )
	{
		gLimit = n;
	}
	else
	{
		write( STDOUT_FILENO, "already initialized\n", 20 );
		sRC = EINVAL;
	}

	sRC = pthread_mutex_unlock( &bmutex );
	return sRC;
}

int waitBarrier( void )
{
	int sRC = 0;

	sRC = pthread_mutex_lock( &bmutex );
	if ( sRC != 0 )
		return sRC;

	if ( gLimit <= 0 )
	{
		return EINVAL;
	}

	gCount++;
	
	while ( gCount < gLimit && sRC == 0 )
		sRC =pthread_cond_wait( &bcond, &bmutex );

	if ( sRC == 0 )
		sRC = pthread_cond_broadcast( &bcond );
	if ( sRC == 0 )
	{
		sRC = pthread_mutex_unlock( &bmutex );
		return sRC;
	}
	else
	{
		pthread_mutex_unlock( &bmutex );
		return sRC;
	}
}
