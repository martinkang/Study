#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../restart.h"
#include "../reentry.h"
#include "../uici.h"
#include "../uiciname.h"
#include "../ide.h"

#define BUFSIZE 256
#define PORT 9090

char gAddr[] = "127.0.0.1";
int gNumProc = 100;
int gNumMsg = 1000;

int childfun( void )
{
	int i = 0;
	int n = 0;
	int isGetFd = 0;

	int fd = -1;
	
	int len = 0;
	char msg[BUFSIZE] = {0, };

	while ( fd <= 0 )
	{
		sleep( 1 );
		fd = u_connect( gAddr, PORT );
		
		n++;
		if ( n > 10 ) break;
	}

	IDE_TEST_RAISE( fd, CONNECT_ERR );
	isGetFd = 1;

	r_printf( "PID[%d] Connect Success\n", getpid() );

	snprintf( msg, BUFSIZE, "PID[%d] MSG", getpid() );
	
	len = r_write( fd, msg, strlen( msg ) );
	if ( len <= 0 )
		IDE_TEST( -1 );

	for ( i = 0; i < gNumMsg; i++ )
	{
		len = r_write( fd, msg, strlen( msg ) );
		if ( len != strlen( msg ) )
		{
			r_perror( "write error" );
			IDE_TEST( -1 );
		}

		len = r_read( fd, msg, BUFSIZE - 1 );
		if ( len <= 0 )
			break;

		msg[len] = '\0';
	}

	close( fd );
	r_printf( "PID[%d] Close[%d]\n", getpid(), fd );

	return 0;

	IDE_EXCEPTION( CONNECT_ERR )
	{
		r_perror( "Connect error" );
	}

	EXCEPTION_END
	
	if ( isGetFd == 1 )
		close( fd );

	return -1;
}

int main( void )
{
	int sRC = 0;
	int pid = 0;
	int i = 0;

	int status = 0;
	int n = 0;

	for ( i = 0; i < gNumProc; i++ )
	{
		pid = fork();
		if ( pid == 0 )
		{
			return childfun();
		}
		else if ( pid < 0 )
		{
			r_perror( "fork error" );
		}
		sleep(1);
	}

	while ( n < gNumProc )
	{
		while( waitpid( -1, &status, WNOHANG ) == 0 );
		r_printf( "child process return %d\n", status );
		n++;
	}

	return 0;

	EXCEPTION_END
	
	return -1;
}

