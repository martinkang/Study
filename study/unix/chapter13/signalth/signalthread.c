#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include "../../ide.h"
#include "../../log.h"
#include "../../globalerror/globalerr.h"

static int gIsInit = 0;
static int gDoneFlag = 0;
static int gSigno = 0;

static pthread_mutex_t gSigMutex;

static int setDone( void )
{
	int sRC = 0;
	int done = 0;

	fprintf( stderr, "setdone start\n" );
	sRC = pthread_mutex_lock( &gSigMutex );
	if ( sRC == -1 )
	{
		setErr( errno );
		return sRC;
	}

	gDoneFlag = 1;
	ideLog( IDE_DE, "set doneflag" );

	sRC = pthread_mutex_unlock( &gSigMutex );	
	return sRC;
}

int getDone( int *aDoneFlag )
{
	int sRC = 0;

	sRC = pthread_mutex_lock( &gSigMutex );
	if ( sRC == -1 )
	{
		setErr( errno );
		return sRC;
	}

	*aDoneFlag = gDoneFlag;

	sRC = pthread_mutex_unlock( &gSigMutex );
	return sRC;
}

static void *signalThread( void *arg )
{
	int sRC = 0;

	int signo = gSigno;
	int sigNum = 0;
	sigset_t sigSet;

	int policy;
	struct sched_param param;
	
	int ss = 0;

	ideLog( IDE_DE, "signal thread is started" );

	sRC = pthread_getschedparam(pthread_self(), &policy, &param);
	if ( sRC == -1 )
	{
		setErr( errno );
		IDE_TEST( 1 );
	}
	
	fprintf(stderr, "Signal thread entered with policy %d and priority %d\n",
		policy,  param.sched_priority);

	sRC = sigemptyset( &sigSet );
	if ( sRC == -1 )
		IDE_TEST( 1 );

	sRC = sigaddset( &sigSet, signo );
	if ( sRC == -1 )
		IDE_TEST( 1 );

	ideLog( IDE_DE, "sigwait..." );
	
	sRC = sigwait( &sigSet, &sigNum );
	if ( sRC == -1 )
		IDE_TEST( 1 );

	ideLog( IDE_DE, "signal received" );

	sRC = setDone();
	if ( sRC == - 1 )
		IDE_TEST( 1 );

	write( STDERR_FILENO, "setDone!\n", 10 );

	ideLog( IDE_DE, "setDone 1\nsignal thread is ended" );

	getDone( &ss );

	fprintf( stderr, "getdone %d\n", ss );

	return;

	EXCEPTION_END

	setErr( errno );
	ideLog( IDE_DE, strerror( errno ) );

	return;
}

int initSignal( int signo, pthread_t *sigth)
{
	int sRC = 0;
	char *errMsg = NULL;

	sigset_t sigSet;

	pthread_attr_t th_Attr;
	struct sched_param th_Param;
	int policy = 0;

	if ( gIsInit == 1 )
	{
		ideLog( IDE_DE, "already initialized" );
		fprintf( stderr, "already initialized" );
		IDE_TEST( 1 );
	}
	ideLog( IDE_DE, "signal initialize begin" );

	sRC = sigemptyset( &sigSet );
	if ( sRC == -1 )
		IDE_TEST( 1 );

	sRC = sigaddset( &sigSet, signo );
	if ( sRC == - 1)
		IDE_TEST( 1 );

	sRC = sigprocmask( SIG_BLOCK, &sigSet, NULL );
	if ( sRC == -1 )
		IDE_TEST( 1 );
	ideLog( IDE_DE, "signal mask block success" );

	sRC = pthread_attr_init( &th_Attr );
	if ( sRC == -1 )
		IDE_TEST( 1 );

	sRC = pthread_attr_getschedparam( &th_Attr, &th_Param );
	if ( sRC == -1 )
		IDE_TEST( 1 );

	sRC = pthread_attr_getschedpolicy( &th_Attr, &policy );
	if ( sRC == -1 )
		IDE_TEST( 1 );

	if ( th_Param.sched_priority < sched_get_priority_max( policy ) )
		th_Param.sched_priority++;
	ideLog( IDE_DE, "priority higher success" );

	sRC = pthread_attr_setschedparam( &th_Attr, &th_Param );
	if ( sRC == - 1 )
		IDE_TEST( 1 );

	gSigno = signo;

	sRC = pthread_create( sigth, &th_Attr, signalThread, NULL );
	if ( sRC == -1 )
		IDE_TEST( 1 );
	ideLog( IDE_DE, "signal thread create success" );

	gIsInit = 1;

	return sRC;

	EXCEPTION_END

	setErr( errno );
	ideLog( IDE_DE, strerror( errno ) );

	return sRC;
}


