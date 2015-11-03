#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char *argv[] )
{
	char buf[] = "g";
	pid_t cpid = 0;
	int fd[2];
	int i, n;
	int j;

	if ( argc != 2 )
	{
		fprintf ( stderr, "Usage : %s processes\n", argv[0] );
		return 1;
	}

	n = atoi( argv[1] );
	if ( pipe(fd) == -1 )
	{
		perror( "Failed to create the synchronization pipe" );
		return 1;
	}

	for ( i = 1; i < n; i++ )
	{
		if ( ( cpid = fork() ) <= 0 )
			break;

	
	}

	sleep((i + 1));

	if ( cpid > 0 )
	{
		for ( j = 0; j < n; i++ )
			if ( write( fd[1], buf, 1 ) != 1 )
				perror( " failed to write sync chr" );
	}

	if ( read(fd[0], buf, 1 ) != 1 )
	{
		perror( " Failed to read sync char" );
	}

	fprintf( stderr,  "i:%d process ID : %ld parent ID : %ld child ID: %ld\n", i, (long)getpid(), (long)getppid(), (long)cpid );

	return (cpid == -1 );
}
