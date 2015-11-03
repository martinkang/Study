#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFSIZE 10

int main( void )
{
	char bufin[BUFSIZE] = "empty";
	char bufout[] = "hello";

	int bytein;
	pid_t childpid;
	int fd[2];

	int sRC = 0;

	sRC = pipe(fd);
	if ( sRC == -1 )
	{
		perror( " failed to create th pipe" );
		return -1;
	}

	bytein = strlen(bufin);
	childpid = fork();

	if ( childpid == -1 )
	{
		perror( "Failed to fork" );
		return -1;
	}

	if ( childpid > 0 )
	{
		dup2( fd[1], STDOUT_FILENO );

		execl( "/bin/sort", "sort", NULL );
		perror( "faile exec sort" );
		close (fd[0] );
		close (fd[1] );


	}
	else 
	{
		dup2( fd[0], STDIN_FILENO );
		close( fd[0] );
		close( fd[1] );

		execl( "/bin/ls", "ls", "-l", NULL );
		perror( "Failed ls" );
	}
	return 1;
}

