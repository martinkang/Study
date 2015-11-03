#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include "buf.h"
#include "../../ide.h"

#define BUFSIZE 10

static pthread_mutex_t gBufLock  = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  gItemCond = PTHREAD_COND_INITIALIZER;
static pthread_cond_t  gLotCond  = PTHREAD_COND_INITIALIZER;

static buf_t gBuf[BUFSIZE];
static int gNumBuf;
static int gIn = 0;
static int gOut = 0;

int getitem( buf_t *aData )
{
	int sRC = 0;
	int islock = 0;

	sRC = pthread_mutex_lock ( &gBufLock );
	IDE_TEST( sRC );
	islock = 1;

	if ( gNumBuf <= 0 )
	{
		sRC = pthread_cond_wait( &gItemCond , &gBufLock );
		IDE_TEST( sRC );
	}

	*aData = gBuf[gOut];
	gOut++;
	gOut = ( gOut + 1 ) % BUFSIZE;
	gNumBuf--;

	sRC = pthread_cond_signal( &gLotCond );
	IDE_TEST( sRC );

	islock = 0;
	sRC = pthread_mutex_unlock( &gBufLock );
	IDE_TEST( sRC );

	return 0;

	EXCEPTION_END

	if ( islock == 1 )
		pthread_mutex_unlock( &gBufLock );

	return errno;
}

int putitem( buf_t aData )
{
	int sRC = 0;
	int islock = 0;

	sRC = pthread_mutex_lock ( &gBufLock );
	IDE_TEST( sRC );
	islock = 1;

	if ( gNumBuf + 1 >= BUFSIZE )
	{
		sRC = pthread_cond_wait( &gLotCond , &gBufLock );
		IDE_TEST( sRC );
	
		if ( gNumBuf == 0 )
			IDE_TEST( -1 );
	}

	gBuf[gIn] = aData;
	gIn++;
	gIn = ( gIn + 1 ) % BUFSIZE;
	gNumBuf++;

	if ( gNumBuf > 5 )
	{
		sRC = pthread_cond_signal( &gItemCond );
		IDE_TEST( sRC );
	}

	islock = 0;
	sRC = pthread_mutex_unlock( &gBufLock );
	IDE_TEST( sRC );

	return 0;

	EXCEPTION_END

	if ( islock == 1 )
		pthread_mutex_unlock( &gBufLock );

	return errno;

}

int getNumBuf( void )
{
	return gNumBuf;
}

void endsignal( void )
{
	pthread_cond_signal( &gItemCond );

	pthread_cond_signal( &gLotCond );
}
