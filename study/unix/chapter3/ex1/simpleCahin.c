#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char *argv[] )
{
	int n = 0;
	int i = 0;
	pid_t cPid = 0;

	if ( argc != 2 )
	{
		fprintf( stderr, "Argument Error\n" );
		exit(1);
	}

	n =atoi( argv[1] );
	if ( n < 1 )
	{
		fprintf( stderr, "Arg num is error\n" );
		exit(1);
	}

	for ( i = 0; i < n; i++ )
	{
		cPid = fork();
		if ( cPid > 0 )
		{
			break;
		}
	}

	wait(NULL);
	fprintf( stderr, "i : %d process ID : %ld  parent ID : %ld child ID : %ld\n",
			i, (long)getpid(), (long)getppid(), (long)cPid );
}
