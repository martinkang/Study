#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main( int argc, char *argv[] )
{
	int sRC = 0;
	int fd[2];
	int cPid = 0;

	sRC = pipe( fd );

	cPid = fork();
	if ( cPid == 0 ) //child
	{
		sRC = dup2( fd[1], STDOUT_FILENO );
		if ( sRC == -1 )
			return -1;

		execl( "/bin/ls", "ls", NULL );
		
		close( fd[0] );
		close( fd[1] );
	}
	else if ( cPid > 0 ) //parent
	{
		sRC = dup2( fd[0], STDIN_FILENO );
		if ( sRC == -1 )
			return -1;

		execl( "/bin/sort", "sort", NULL );

		close( fd[0] );
		close( fd[1] );
	}

	return 0;
}

