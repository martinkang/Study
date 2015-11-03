#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include "../uici.h"
#include "../uiciname.h"
#include "../reentry.h"
#include "../restart.h"

typedef struct msg
{
	int pid;
	int tid;
	int msgno;
} msg_t;

#define PORT 9090
#define BUFSIZE 255
#define NUMMSG 4

char *gMsg[4] = { "hi", "message", "response", "everyone" };

int gNumProc = 0;
int gNumMsg = 0;
char gAddr[] = "127.0.0.1";

int connectToServer( void )
{
	int sRC = -1;
	int size = 0;
	int i = 0;
	int j = 0;
	int r = 0;
	int n = 1;
	char buf[BUFSIZE] = {0, };
	char sMsg[BUFSIZE] = {0, };

	int fd = 0;

	msg_t msg;

	srand( time(NULL ) );

	for ( i = 0; i < 3; i++ )
	{
		fd = u_connect( gAddr, PORT );
		if ( fd == -1 )
		{
			r_fprintf( stderr, "retry %d\n", n );
			n++;
		}
		else
		{
			r_printf( "p[%d] connect success\n", getpid() );		

			for ( j = 0; j < gNumMsg; j++ )
			{
				r = rand() % NUMMSG;
				snprintf( sMsg, BUFSIZE, "PID : %d Msg : %s\n", getpid(), gMsg[r] );
				size = r_write( fd, sMsg, strlen( sMsg ) ); // request msg
				if ( size > 0 )
					sMsg[size] = '\0';

				if ( size != strlen( sMsg ) )
				{
					r_printf( "size : %d\n strlen : %d\n", size, strlen( gMsg[r] ) );
					r_perror( "write error" );
				}
				else
				{
					size = r_read( fd, buf, BUFSIZE );

					if ( size > 0 )
						buf[size] = '\0';
					if ( size != strlen( buf ) )
					{
						r_printf( "size : %d\n strlen : %d\n", size, strlen( buf ) );
						r_perror( "read error" );
					}

					r_printf( "p[%d] : %s\n\n", getpid(), buf );
				}
			}	
			close( fd );

			break;
		}
	}
	return sRC;
}

int main( int argc, char *argv[] )
{
	int sRC = 0;
	int i = 0;

	int pid = 0;

	gNumProc = atoi( argv[1] );
	gNumMsg = atoi( argv[2] );

	for ( i = 0; i < gNumProc; i++ )
	{
		pid = fork();
		if ( pid == 0 )
		{
			r_printf( "fork success\n" );
			sRC = connectToServer();
			return sRC;
		}
		else if ( pid < 0 )
		{
			fprintf( stderr, "fork error\n" );
			return -1;
		}
	}

	fprintf( stderr, "wait for child\n" );
	while( waitpid( -1, NULL, WNOHANG ) >= 0 );

	r_fprintf( stderr, "end\n" );

	return 0;
}
