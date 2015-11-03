#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char *argv[] )
{
	char buf[] = "g";
	pid_t cpid = 0;
	int fd[2];
	int i, n;

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

	for ( i = 0; i < n; i++ )
	{
		if ( ( cpid = fork() ) <= 0 )
		{
			break;
		}


	}

	if ( cpid > 0 )
	{
		for ( i = 0; i < n; i++ )
			if ( write( fd[1], buf, 1 ) != 1 )
			{
				perror( " failed to write sync chr" );
			}
	}

	printf( "%d sleep %d sec\n", getpid(), i + 1 );
	sleep( i + 1);

	if ( cpid == 0 )
	{
		if ( read(fd[0], buf, 1 ) != 1 )
		{
			fprintf( stderr, "error\n" );
		}
	}
	fprintf( stderr, " %d is synced\n", getpid() );
}
