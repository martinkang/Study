#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include "../uici.h"
#include "../uiciname.h"

#define PORT 9090
#define BUFSIZE 255

int childfun( int clntfd )
{
	int sRC = 0;
	int size = 0;
	int msgNo = 0;
	char buf[BUFSIZE] = {0, };
	char msg[BUFSIZE] = {0, };

	while ( 1 )
	{
		size = r_read( clntfd, buf, BUFSIZE );
		if ( size == 0 )
			break;

		if ( size != strlen( buf ) )
		{
			r_perror( "read error" );
			return -1;
		}
		r_printf( "p[%d] recv : %s\n", getpid(), buf );

		snprintf( msg, BUFSIZE, "p[%d] MSG[%d]\n", getpid(), msgNo );
		msgNo++;

		size = r_write( clntfd, msg, strlen( msg ) );
		if ( size != strlen( msg ) )
		{
			r_perror( "write error" );
			return -1;
		}
	}
	return 0;
}

void childwait( int signo )
{
	if ( signo == SIGCHLD )
	{
		while( waitpid( -1, NULL, WNOHANG ) >= 0 );
	}
}

int main( void )
{
	int sRC = 0;
	int i = 0;
	int j = 0;

	int pid = 0;
	int serfd;
	int clntfd = 0;

	struct sigaction act;

	act.sa_flags = 0;
	sigemptyset( &act.sa_mask );
	act.sa_handler = childwait;

	sRC = sigaction( SIGCHLD, &act, NULL );
	if ( sRC == -1 )
		return -1;

	serfd = u_open( PORT );
	if ( serfd == -1 )
	{
		perror( "socket create error" );
		return -1;
	}

	fprintf( stderr, "wait for client.....\n" );
	while ( 1 )
	{
		clntfd = u_accept( serfd );
		if ( clntfd == -1 )
		{
			r_perror( stderr, "client connect error\n" );
			continue;
		}

		r_printf( "client connected\n" );
		pid = fork();
		if ( pid < 0 )
		{
			r_fprintf( stderr, "fork error\n" );
			close( clntfd );
			continue;
		}
		else if ( pid == 0 ) // child
		{
			sRC = childfun( clntfd );

			close( clntfd );

			return sRC;
		}
	
		close( clntfd );
	}

	close( serfd );

	return 0;
}
