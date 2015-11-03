#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <errno.h>
#include "../restart.h"
#include "../reentry.h"
#include "../uici.h"
#include "../uiciname.h"
#include "../ide.h"

#define BUFSIZE 256
#define THREAD_NUM 4 
#define MAX_BACKLOG 10
#define PORT 9090

static pthread_mutex_t gMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t gCond = PTHREAD_COND_INITIALIZER;
static pthread_cond_t gCondP = PTHREAD_COND_INITIALIZER;

enum { NONE = 0, RUN = 1, EXIT = 2 };

static sig_atomic_t gExitFlag = 0;

typedef struct ThreadStruct
{
	pthread_t mID;
	int mFd;
	int mStatus;
	int mNumEndFd;
	int mNumFail;
} ThreadStruct;

ThreadStruct gThread[THREAD_NUM];

static int gQue[MAX_BACKLOG];
static int gQueNum = 0;
static int gQueIn = 0;
static int gQueOut = 0;

int getFdFromQue( int idx )
{
	int sRC = 0;
	int fd = 0;

	sRC = pthread_mutex_lock( &gMutex );
	IDE_TEST( sRC );

	while( gQueNum == 0 )
	{
		pthread_cond_wait( &gCond, &gMutex );
	}

	gQueNum--;

	IDE_TEST_RAISE( gQue[gQueOut], INVALID_FD );
	gThread[idx].mFd = gQue[gQueOut];
	gThread[idx].mStatus = RUN;

	gQue[gQueOut] = -1;

	gQueOut++;
	gQueOut = gQueOut % MAX_BACKLOG;

	pthread_cond_signal( &gCondP );

	IDE_TEST( pthread_mutex_unlock( &gMutex ) );

	r_printf( "Thread[%d] - FD[%d] Assigned\n", idx, fd );

	return 0; 
	
	IDE_EXCEPTION( INVALID_FD )
	{
		pthread_mutex_unlock( &gMutex );

		r_perror( "Wrong FD!!" );
		gExitFlag = 1;
	}

	EXCEPTION_END;

	return -1;
}

void *thread_fun( void* arg )
{
	int sRC = 0;
	int Idx = (int)arg;
	int fd = 0;

	int len = 0;
	char msg[BUFSIZE] = {0, };

	int isGetFd = 0;

	while( gExitFlag == 0 )
	{
		sRC = getFdFromQue( Idx );
		IDE_TEST_RAISE( sRC, GET_FD );
		isGetFd = 1;

		fd = gThread[Idx].mFd;

		len = read( fd, msg, BUFSIZE - 1 );
		if ( len <= 0 )
			IDE_TEST( -1 );

		msg[len] = '\0';
		r_printf( "Thread[%d] Recv Msg : %s\n", Idx, msg );

		while( 1 )
		{
			len = r_read( fd, msg, BUFSIZE - 1 );
			if ( len <= 0 )
				break;

			msg[len] = '\0';

			len = r_write( fd, msg, len );
			if ( len != strlen( msg ) )
			{
				r_perror( "write error" );
			}
		}

		close( fd );

		pthread_mutex_lock( &gMutex );

		gThread[Idx].mFd = -1;
		gThread[Idx].mStatus = EXIT;
		(gThread[Idx].mNumEndFd)++;

		pthread_mutex_unlock( &gMutex );
	}

	r_printf( "Thread[%d] normal exited\n", Idx );

	r_printf( "Close FD[%d]\n", fd );

	pthread_exit( (void*)0 );

	IDE_EXCEPTION( GET_FD )
	{
		r_perror( "Get Fd Error" );
	}

	IDE_EXCEPTION( LOCK_ERR )
	{
		r_perror( "Lock error" );
	}

	IDE_EXCEPTION( UNLOCK_ERR )
	{
		r_perror( "UnLock error" );
	}

	EXCEPTION_END;

	if ( isGetFd == 1 )
	{
		close( fd );

		pthread_mutex_lock( &gMutex );

		gThread[Idx].mFd = -1;
		gThread[Idx].mStatus = EXIT;
		(gThread[Idx].mNumFail)++;
	
		pthread_mutex_unlock( &gMutex );
	}

	pthread_exit( (void*)-1 );
}

void createThreadPool( void )
{
	int i = 0;

	for ( i = 0; i < THREAD_NUM; i++ )
	{
		while( pthread_create( &gThread[i].mID, NULL, thread_fun, (void*)i ) == -1 );

		gThread[i].mFd = -1;
		gThread[i].mStatus = NONE;
		gThread[i].mNumEndFd = 0;
		gThread[i].mNumFail = 0;
	}
}

void destroyThreadPool( void )
{
	int i = 0;

	r_printf( "join thread pool\n" );

	for ( i = 0; i < THREAD_NUM; i++ )
	{
		pthread_join( gThread[i].mID, NULL );

		gThread[i].mFd = -1;
		gThread[i].mStatus = NONE;
		gThread[i].mNumEndFd = 0;
		gThread[i].mNumFail = 0;
	}
}

int main( void )
{
	int sRC = 0;
	int i = 0;
	int serfd = 0;
	int clntfd = 0;

	int n =0;

	struct timeval startTime;
	struct timeval endTime;
	struct timespec ts;

	ts.tv_sec = 5;

	fprintf( stderr, "main module start....\n" );
	
	for ( i = 0; i < MAX_BACKLOG; i++ )
	{
		gQue[i] = -1;
	}

	createThreadPool();
	fprintf( stderr, "create ThreadPool Sucssess\n" );

	sRC = setBackLog( MAX_BACKLOG );
	IDE_TEST_RAISE( sRC, BACKLOG_ERROR );

	serfd = u_open( PORT );
	IDE_TEST_RAISE( serfd, SOCKET_ERROR );

	fprintf( stderr, "create Socket\nPORT : %d\n", PORT );

	gettimeofday( &startTime, NULL );

	while( gExitFlag == 0 )
	{
		pthread_mutex_lock( &gMutex );

		while ( gQueNum >= MAX_BACKLOG )
		{
			sRC = pthread_cond_timedwait( &gCondP, &gMutex, &ts );
			if ( sRC == -1 )
			{
				r_printf( "Main Module accept time out!\nProgram exit step is started\n" );
				gExitFlag = 1;
				break;
			}
		}

		if ( gExitFlag == 1 )
			break;

		clntfd = u_accept( serfd );
		if ( clntfd == -1 )
		{
			r_perror( "client socket accept error" );
			pthread_mutex_unlock( &gMutex );
			continue;
		}
		r_printf( "new socket accepted\n" );

		gQue[gQueIn] = clntfd;
		gQueIn++;
		gQueIn = gQueIn % MAX_BACKLOG;
		
		gQueNum++;

		pthread_cond_signal( &gCond );

		pthread_mutex_unlock( &gMutex );

		n++;

		if ( n % THREAD_NUM == 0 )
		{
			pthread_mutex_lock( &gMutex );

			for ( i = 0; i < THREAD_NUM; i++ )
			{
				r_printf( "THREAD[%d]\n", i );
				r_printf( "THREAD Status : %d\n", gThread[i].mStatus );
				r_printf( "THREAD Fd     : %d\n", gThread[i].mFd );
				r_printf( "THREAD EndFd  : %d\n", gThread[i].mNumEndFd );
				r_printf( "THREAD NumFail: %d\n", gThread[i].mNumFail );

			}
			pthread_mutex_unlock( &gMutex );

			sleep( 1 );
		}
	}

	gettimeofday( &endTime, NULL );

	r_printf( "Number of Accept is %d\n", n );

	r_printf( "main manager exit step start\n" );

	destroyThreadPool();

	r_printf( "main manager exited\n" );

	return 0;

	IDE_EXCEPTION( BACKLOG_ERROR )
	{
		r_fprintf( stderr, "back log must bigger than zero\n" );
	}

	IDE_EXCEPTION( SOCKET_ERROR )
	{
		r_perror( "socket create error" );
	}

	EXCEPTION_END;

	return -1;
}
