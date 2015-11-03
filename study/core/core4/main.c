#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include "ide.h"
#include "server.h"
#include "client.h"
#include "logger.h"
#include "reentry.h"

#define CLIENT 4

static sig_atomic_t gIsExit = 0;

const char gAddr[] = "127.0.0.1";
const int gPort = 8080;


void *mainFunction( void );

int main()
{
	int sRC = 0;
	int ret = 0;

	pthread_t mainThread;

	sRC = pthread_create( &mainThread, NULL, (void*)mainFunction, (void*)&ret );
	if ( sRC != 0 )
	{
		r_perror( "main thread create error" );
		return -1;
	}

	sRC = pthread_join( mainThread, (void*)&ret );
	if ( sRC != 0 )
	{
		r_perror( "main thread join error" );
	}

	if ( ret != 0 )
	{
		r_fprintf( stderr, "maint thread fail\n" );
	}

	return 0;
}

void logWR( char *msg )
{
	queue_data( (void*)msg, strlen( msg ) );
}

int isExit( void )
{
	return gIsExit;
}

void setExit( void )
{
	gIsExit = 1;
}

int nonBlockRead( void )
{
	int sRC = 0;
	int flags = fcntl( STDIN_FILENO , F_GETFL, 0);
	char buf[20] = {0, };

	fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

	sRC = read( STDIN_FILENO, buf, 20 );

	fcntl(STDIN_FILENO, F_SETFL, flags );

	return sRC;
}

void *mainFunction( void )
{
	int sRC = 0;
	int step = 0;
	int i = 0;
	int j = 0;

	pthread_t serverThread;
	pthread_t clientThread[CLIENT];
	pthread_t loggerThread;
	
	sRC = pthread_create( &loggerThread, NULL, (void*)loggerFunction, NULL );
	if ( sRC != 0 )
	{
		r_perror( "loger thread create error" );
		IDE_TEST( sRC );
	}
	step = 1;

	sRC = pthread_create( &serverThread, NULL, (void*)serverFunction, NULL );
	if ( sRC != 0 )
	{
		r_perror( "server thread create error" );
		IDE_TEST( sRC );	
	}
	step = 2;

	for ( i = 0; i < CLIENT; i++ )
	{
		sRC = pthread_create( &clientThread[i], NULL, (void*)clientFunction, NULL );
		if ( sRC != 0 )
		{
			r_perror( "server thread create error" );		
			step = 3;

			IDE_TEST( sRC );
		}
	}
	step = 4;

	while ( sRC <= 0 )
	{
		sRC = nonBlockRead();
		sleep( 1 );
	}

	r_fprintf( stderr, "exit flag setting\n" );

	setExit();

	for ( i = CLIENT - 1; i >= 0; i-- )
	{
		sRC = pthread_join( clientThread[i], NULL );
		if ( sRC != 0 )
		{
			r_perror( "client thread join error" );
			step = 3;
			IDE_TEST( sRC );
		}
	}

	step = 1;
	sRC = pthread_join( serverThread, NULL );
	if ( sRC != 0 )
	{
		r_perror( "server thread join error" );
		IDE_TEST( sRC );
	}

	step = 0;
	sRC = pthread_join( loggerThread, NULL );
	if ( sRC != 0 )
	{
		r_perror( "logger thread join error" );
		IDE_TEST( sRC );
	}

	pthread_exit( (void*)sRC );
	
	EXCEPTION_END

	r_fprintf( stderr, "error exception\n" );
	setExit();

	switch ( step )
	{
		case 4:
			i = CLIENT;
		case 3:
			for ( j = 0; j < i; j++ )
			{
				pthread_join( clientThread[j], NULL );
			}
		case 2:
			pthread_join( serverThread, NULL );
		case 1:
			pthread_join( loggerThread, NULL );
		default :
			break;
	}

	pthread_exit( (void*)sRC );
}


