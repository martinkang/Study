#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "../../ide.h"
#include "../../log.h"
#include "signalthread.h"
#include "../../globalerror/globalerr.h"

#define MAX_THREAD 8 
#define BUF_SIZE 255

void *threadfunc( void *arg )
{
	int sRC = 0;
	int sDone = 0;

	int len = 0;
	char msg[BUF_SIZE] = {0, };

	snprintf( msg, BUF_SIZE, "t_id[%ld], printf until signal is comming\n", (long)pthread_self() );
	len = strlen( msg );

	ideLog( IDE_SG, "pthread created" );

	ideLog( IDE_SG, "getDone" );
	while ( sDone == 0  )
	{
		ideLog( IDE_SG, msg );
		sRC = write( STDOUT_FILENO, msg, len );
		if ( sRC == -1 )
		{
			setErr( errno );
			return;
		}

		ideLog( IDE_SG, "sleep 1 sec" );
		sleep( 1 );
		sRC = getDone( &sDone );
		fprintf( stderr, "sDone : %d\n", sDone );
	}

	ideLog ( IDE_SG, "signal is received" );

	return;
}

int main()
{
	int sRC = 0;
	int step = 0;
	int i = 0;

	pthread_t th[MAX_THREAD + 1];

	ideLog( IDE_DE, "========== program is started ==========" );

	sRC = initSignal( SIGUSR1, &th[0] );
	if ( sRC == -1 )
	{
		ideLog( IDE_DE, "init siganl is fail" );
		IDE_TEST( 1 );
	}
	ideLog( IDE_DE, "init signal is success" );

	for ( i = 1; i < MAX_THREAD + 1; i++ )
	{
		sRC = pthread_create( &th[i], NULL, threadfunc, NULL );
		if ( sRC == -1 )
			setErr( errno );
	}

	fprintf( stderr, "Send SIGUSR1(%d) signal to proc %ld to stop msg\n", SIGUSR1, (long)getpid() );

	for ( i = 0; i < MAX_THREAD + 1; i++ )
	{
		sRC = pthread_join( th[i], NULL );
		if ( sRC == -1 )
			setErr( errno );

	}

	sRC = showErr();
	if ( sRC == -1 )
		IDE_TEST( 1 );

	ideLog( IDE_DE, "========== program is ended ==========" );

	return 0;

	EXCEPTION_END;

	if ( errno != 0 )
		ideLog( IDE_DE, strerror( errno ) );

	return sRC;

}


