#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUF_SIZE 255

int processchar ( int fdin, int fdout, char inchar, char *outstr )
{
	int sRC = 0;
	char *msg[BUF_SIZE] = {0, };

	while ( ( sRC = read( fdin, msg, BUF_SIZE ) ) != 0 )
	{
		if ( sRC == -1 )
			return -1;

		sRC = write( fdout, msg, strlen( msg ) );
		if ( sRC < strlen( msg ) )
			return -1;
	}

	return 0;
}
int main( int argc, char *argv[] )
{
	pid_t cpid = 0;
	int error;
	int fd[2];
	int i;
	int nprocs = 4;
	int sRC = 0 ;
	int cid = 0;
	int status;
	sRC = pipe( fd );
	if ( sRC == -1 )
		return -1;

	sRC = dup2( fd[0], STDIN_FILENO );
	if ( sRC == -1 )
		return -1;

	sRC = dup2( fd[1], STDOUT_FILENO );
	if ( sRC == -1 )
		return -1;

	close( fd[0] );
	close( fd[1] );


	for ( i = 1; i < nprocs; i++ )
	{
		pipe( fd );

		cpid = fork();
		if ( cid == 0 ) //child
		{
			sRC = dup2( fd[1], STDOUT_FILENO );	
			if ( sRC == -1 )
				return -1;
		}
		else if ( cid > 0 ) //parent
		{
			dup2( fd[0], STDIN_FILENO );
			if ( sRC == -1 )
				return -1;
		}
		else
		{
			fprintf( stderr, "error\n" );
		}
		close( fd[0] );
		close( fd[1] );
	
		if ( cpid > 0 )
			break;
	}


	fprintf( stderr, "This is process %d with ID %ld and parent id %ld\n", i, (long)getpid(), (long)getppid() );

	wait( &status );
	
	return 0;
}
