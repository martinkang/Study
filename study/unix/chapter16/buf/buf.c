#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include "buf.h"

static pthread_mutex_t gBufLock = PTHREAD_MUTEX_INITIALIZER;

static buf_t gBuf[BUFSIZE];
static int gBufIn = 0;
static int gBufOut = 0;

static int gIsInit = 0;
static sem_t gSemGet;
static sem_t gSemPut;

int initBuf( void )
{
	int sRC = 0;

	if ( gIsInit == 0 )
	{
		sRC = sem_init( &gSemGet, 0, 0 );
		if ( sRC == 0 )
		{
			sRC = sem_init( &gSemPut, 0, BUFSIZE );
			if ( sRC == 0 )
			{
				gIsInit = 1;
				return 0;
			}
			else
			{
				sem_destroy( &gSemGet );
				return errno;
			}
		}
	}
	else
	{
		return 0;
	}
	return errno;
}

void finalBuf( void )
{
	if ( gIsInit == 1 )
	{
		sem_destroy( &gSemGet );

		sem_destroy( &gSemPut );

		gIsInit = 0;
	}
}

int getItem( buf_t *aData )
{
	int sRC = 0;
	
	sRC = sem_wait( &gSemGet );
	if ( sRC != 0 )
		return errno;

	sRC = pthread_mutex_lock( &gBufLock );
	if ( sRC == -1 )
	{
		sem_post( &gSemGet );
		return sRC;
	}

	*aData = gBuf[gBufOut];
	gBufOut++;
	gBufOut = ( gBufOut + 1 ) % BUFSIZE;

	pthread_mutex_unlock( &gBufLock );

	sRC = sem_post( &gSemPut );
	return sRC;
}

int putItem( buf_t aData )
{
	int sRC = 0;

	sRC = sem_wait( &gSemPut );
	if ( sRC != 0 )
		return errno;

	sRC = pthread_mutex_lock( &gBufLock );
	if ( sRC == -1 )
	{
		sem_post( &gSemPut );
		return errno;
	}

	
	gBuf[gBufIn] = aData;
	gBufIn++;
	gBufIn = ( gBufIn + 1 ) % BUFSIZE;

	pthread_mutex_unlock( &gBufLock );

	sRC = sem_post( &gSemGet );

	return sRC;

}

