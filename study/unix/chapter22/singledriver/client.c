#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include "../uici.h"
#include "../uiciname.h"
#include "reentry.h"
#include "restart.h"

typedef struct msg
{
	int pid;
	int tid;
	int msgno;
} msg_t;

#define BUFSIZE 255
#define NUMMSG 4

char *gMsg[4] = { "hi", "message", "response", "everyone" };

int gNumProc = 0;
int gNumMsg = 0;

int gPort = 9090;
char gAddr[] = "127.9.9.1";

int connectToServer( void )
{
	int sRC = -1;
	int size = 0;
	int i = 0;
	int j = 0;
	int r = 0;
	char buf[BUFSIZE] = {0, };
	char sMsg[BUFSIZE] = {0, };

	int fd = 0;

	msg_t msg;

	srand( time(NULL ) );

	for ( i = 0; i < gNumConn; i++ )
	{
		r = rand() % NUMMSG;
		snprintf( sMsg, BUFSIZE, "p[%d] : %s", getpid(), gMsg[r] );
		size = r_write( fd, sMsg, strlen( sMsg ) ); // request msg
		if ( size != strlen( sMsg ) )
		{
			r_fprintf( stderr, "write error\n" );
		}
		r_fprintf( stderr, "p[%d] request success\n", getpid() );		

		size = r_read( fd, &msg, sizeof( msg ) );
		if ( size != sizeof( msg ) )
		{
			r_perror( "read error");
		}
		r_fprintf( stderr, "p[%d] pid : %d tid : %d msgno : %d\n", getpid(), msg.pid, msg.tid, msg.msgno );

		close( fd );
	}
	return sRC;
}

int main( int argc, char *argv[] )
{
	int sRC = 0;
	int i = 0;
	int j = 0;

	int fd = 0;
	int pid = 0;

	if ( argc != 3 )
	{
		fprintf( stderr, "argument error\n" );
		return -1;
	}

	gNumProc = atoi( argv[1] );
	gNumMsg = atoi( argv[2] );

	for ( i = 0; i < gNumProc; i++ )
	{
		fd = u_connect( gAddr, gPort );
		if ( fd == -1 )
		{
			r_perror( "connect fail" );
			continue;	
		}

		pid = fork();
		if ( pid == 0 )
		{
			r_fprintf( stderr, "fork success\n" );
			sRC = connectToServer();
			return sRC;
		}
		else if ( pid < 0 )
		{
			r_fprintf( stderr, "fork error\n" );
			return -1;
		}
		close( fd );
		
		waitpid( -1, NULL, WNOHANG );
	}

	r_fprintf( stderr, "wait for child\n" );
	while( waitpid( -1, NULL, WNOHANG ) >= 0 );

	r_fprintf( stderr, "end\n" );

	return 0;
}
