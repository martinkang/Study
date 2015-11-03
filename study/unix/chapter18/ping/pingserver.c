#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <string.h>
#include "../UICI/uici.h"
#include "../UICI/uiciname.h"

#define PORT 9090
#define BUFSIZE 1024

int process_ping( int communfd )
{
	int sRC = 0;
	int size = 0;
	char buf[BUFSIZE];
	struct utsname utname;

	sRC = uname( &utname );
	snprintf( buf, BUFSIZE, "%s %s %s %s %s", 
			utname.sysname, utname.nodename, utname.release, utname.version, utname.machine );

	write( communfd, buf, strlen( buf ) );
	return 0;
}

int child_fun( int clntfd )
{
	int sRC = 0;
	char buf[BUFSIZE] = {0, };

	read( clntfd, buf, BUFSIZE );
	fprintf( stderr, "process[%s] is connected\n", buf );

	process_ping( clntfd );

	close( clntfd );

	return sRC;
}

int main( void )
{
	int sRC = 0;
	int numchild = 0;
	int i = 0;
	int num = 0;

	int pid = 0;
	int serfd = 0;
	int clntfd = 0;
	serfd = u_open( PORT );
	fprintf( stderr, "server socket create success\n" );

	while( 1 )
	{
		fprintf( stderr, "wait for client ping\n" );

		clntfd = u_accept( serfd );

		pid = fork();
		if ( pid == 0 ) //child
		{
			sRC = child_fun( clntfd );
			return 0;
		}
		else if ( pid < 0 ) // error
		{
			perror( "fork error" );

		}
		else // parent
		{
			numchild++;
		}

		for ( i = 0; i < num; i++ )
		{
			 sRC = waitpid( -1, NULL, WNOHANG );
			 if ( sRC > 0 )
				 numchild--;
		}

		close( clntfd );
	}

	while ( waitpid( -1, NULL, WNOHANG ) > 0 );

	fprintf( stderr, "server end" );
	
	close( serfd );
	return 0;
}
