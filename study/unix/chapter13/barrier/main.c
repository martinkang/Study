#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

#define NUM_CHILD 4
#define BUF_SIZE 256

int main()
{
	int sRC = 0;
	int i = 0;

	int fd[2];

	int pid = 0;
	char buf[BUF_SIZE] = {0, };
	char msg[BUF_SIZE] = {0, };

	pipe( fd );
	
	sRC = initBarrier( NUM_CHILD );
	if ( sRC != 0 )
	{
		fprintf( stderr, "initBarrier error\n" );
		close( fd[0] );
		close (fd[1] );

		return -1;
	}

	for ( i = 0; i < NUM_CHILD; i++ )
	{
		pid = fork();
		if ( pid < 0 )
		{
			perror( "child process create fail" );
			break;
		}
		else if ( pid == 0 )
		{
			break;
		}
	}

	if ( pid == 0 )
	{
		read ( fd[0], buf, 1 );
	
		sRC = waitBarrier();
		if ( sRC == 0 )
		{
			snprintf( msg, BUF_SIZE, "pid[%d] : %s", getpid(), buf );
			write( STDOUT_FILENO, msg, strlen( msg ) );
		}
		else
		{
			write( STDERR_FILENO, "wait barrier fail\n", BUF_SIZE );
		}

	}
	else if ( pid > 0 )
	{
		for ( i = 0; i < NUM_CHILD; i++ )
			write( fd[1], "r", 1 );

		write( STDOUT_FILENO, "parent[%d]", getpid() );
		
		for ( i = 0 ; i < NUM_CHILD; i++ )
			wait( NULL );
	}

	close ( fd[0] );
	close ( fd[1] );

	return 0;
}
