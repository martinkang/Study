#include <stdio.h>
#include <pthread.h>
#include "manager.h"
#include "loger.h"
#include "../../ide.h"
#include "que.h"

static que_t gQue;

static pthread_mutex_t gQueMutex = PTHREAD_MUTEX_INITIALIZER;

static char path[255] = "log.txt";

static void writelog( void )
{
	int sRC = 0;
	int islock = 0;
	int isopen = 0;

	char sData[256] = {0, };
	
	FILE *fd = NULL;

	sRC = pthread_mutex_lock( &gQueMutex );
	IDE_TEST( sRC );
	islock = 1;

	fd = fopen( path, "a+" );
	isopen = 1;

	while( sRC == 0 )
	{
		sRC = dequeue( (void**)&sData );
		if ( sRC == 0 )
		{
			fputs( sData, fd );
		}
	}

	isopen = 0;
	fclose( fd );

	islock = 0;
	sRC = pthread_mutex_unlock( &gQueMutex );
	IDE_TEST( sRC );

	return;

	EXCEPTION_END

	if ( isopen == 1 )
		fclose( fd );

	if ( islock == 1 )
		 pthread_mutex_unlock( &gQueMutex );
	
	return;

}

static void flushLog( void )
{
	int sRC = 0;
	char sData[256] = {0, };
	int isopen = 0;
	
	FILE *fd = NULL;

	fd = fopen( path, "a+" );
	isopen = 1;

	while( sRC == 0 )
	{
		sRC = dequeue( (void**)&sData );
		if ( sRC == 0 )
		{
			fputs( sData, fd );
		}
		else
		{
			IDE_TEST( sRC );
		}
	}

	isopen = 0;
	fclose( fd );

	return;

	EXCEPTION_END
	
	if ( isopen == 1 )
		fclose( fd );

	return;
}

void L_manager( void )
{
    int sRC = 0;
    int sFlag = 0;

	initQue();

    while ( sFlag == 0 )
    {
        getdone( &sFlag );
		writelog();
		sleep( 2);
    }
    return;

	fprintf( stderr, "log flush\n" );
	flushLog();
}

int Inlog( char *msg )
{
	int sRC = 0;
	int islock = 0;

	sRC = pthread_mutex_lock( &gQueMutex );
	IDE_TEST( sRC );
	islock = 1;

	sRC = queue( (void*)msg );
	IDE_TEST( sRC );

	islock = 0;
	sRC = pthread_mutex_unlock( &gQueMutex );
	IDE_TEST( sRC );

	return 0;

	EXCEPTION_END

	if ( islock == 1 )
		 pthread_mutex_unlock( &gQueMutex );

	return sRC;
}


