#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include "reentry.h"

#define BUFSIZE 1024

static pthread_mutex_t gErrMutex = PTHREAD_MUTEX_INITIALIZER;

void r_printf( char *fmt, ... )
{
	int len = 0;
	int size = 0;
	char msg[BUFSIZE] = {0, };

	va_list ap;

	va_start( ap, fmt );
	vsprintf( msg, fmt, ap );

	len = strlen( msg );

	while ( ( size = write( STDOUT_FILENO, msg, len ) ) == -1 && ( errno == EINTR ) );
}

void r_fprintf( FILE *fd, char *fmt, ... )
{
	int fno = 0;

	int len = 0;
	int size = 0;
	char msg[BUFSIZE] = {0, };

	va_list ap;

	fno = fileno( fd );

	va_start( ap, fmt );
	vsprintf( msg, fmt, ap );

	len = strlen( msg );

	while ( ( size = write( fno, msg, len ) ) == -1 && ( errno == EINTR ) );
}

int r_perror( const char *log )
{
	int sRC = 0;
	sigset_t newSet;
	sigset_t oldSet;

	if ( errno == 0 )
		return 0;

	sRC = sigemptyset( &newSet );	
	if ( sRC == -1 )
		return sRC;

	sRC = sigemptyset( &oldSet );
	if ( sRC == -1 )
		return sRC;

	sRC = sigfillset( &newSet );
	if ( sRC == -1 )
		return sRC;

	sRC = sigprocmask( SIG_SETMASK, &newSet, &oldSet );
	if ( sRC == -1 )
		return sRC;

	sRC = pthread_mutex_lock( &gErrMutex );
	if ( sRC == -1 )
	{
		sigprocmask( SIG_SETMASK, &oldSet, NULL );
		return sRC;
	}

	perror( log );

	sRC = pthread_mutex_unlock( &gErrMutex );
	if ( sRC == -1 )
		return sRC;

	sRC = sigprocmask( SIG_SETMASK, &oldSet, NULL );
	if ( sRC == -1 )
		return sRC;
}

int r_strerror( int errnum, char *buf, int len )
{
	int sRC = 0;
	int err = 0;
	char *msg;
	sigset_t newSet;
	sigset_t oldSet;

	sRC = sigemptyset( &newSet );	
	if ( sRC == -1 )
		return sRC;

	sRC = sigemptyset( &oldSet );
	if ( sRC == -1 )
		return sRC;

	sRC = sigfillset( &newSet );
	if ( sRC == -1 )
		return sRC;

	sRC = sigprocmask( SIG_SETMASK, &newSet, &oldSet );
	if ( sRC == -1 )
		return sRC;

	sRC = pthread_mutex_lock( &gErrMutex );
	if ( sRC == -1 )
	{
		sigprocmask( SIG_SETMASK, &oldSet, NULL );
		return sRC;
	}

	msg = strerror( errnum );
	if ( sizeof(msg) > len )
		err = ERANGE;
	else
		strcpy( buf, msg );

	sRC = pthread_mutex_unlock( &gErrMutex );
	if ( sRC == -1 )
		return sRC;

	sRC = sigprocmask( SIG_SETMASK, &oldSet, NULL );
	if ( sRC == -1 )
		return sRC;

	return err;
}
