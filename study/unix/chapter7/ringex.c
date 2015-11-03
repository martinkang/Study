#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define N_PROCS 4
#define BUF_MAX 1024

int main( int argc, char *argv[] )
{
	int sRC = 0;
	int pid = 0;
	int fd[2];
	int i = 0;
	int root = getpid();
	int procs[N_PROCS] = {0, };
	int recvid = 0;
	int myid = 0;
	int rel = 0;

	pipe( fd );

	dup2( fd[0], STDIN_FILENO );
	dup2( fd[1], STDOUT_FILENO );

	close( fd[0] );
	close( fd[1] );

	for ( i = 1; i < N_PROCS; i++ )
	{
		pipe( fd );
		pid = fork();

		if ( pid > 0 )
		{
			dup2( fd[0], STDIN_FILENO );

		}
		else if ( pid == 0 )
		{
			dup2( fd[1], STDOUT_FILENO );
			
		}
		else
		{
			return -1;
		}

		close( fd[0] );
		close( fd[1] );

		if ( pid > 0 )
			break;
	}

	if ( getpid() == root )
	{
		procs[0] = getpid();

		write( STDOUT_FILENO, procs, sizeof(procs) );
	}
	else
	{
		read( STDIN_FILENO, procs, sizeof(procs) );

		procs[i-1] = getpid();

		write( STDOUT_FILENO, procs, sizeof(procs) );
	}

	read( STDIN_FILENO, procs, sizeof(procs) );

	write( STDOUT_FILENO, procs, sizeof(procs) );

	myid = getpid();
	while ( 1 )
	{
		fcntl( fd[0], O_NONBLOCK );

		sRC = read( STDOUT_FILENO, &recvid, sizeof( recvid ) );

		if ( myid > recvid )
		{
			write( STDOUT_FILENO, &recvid, sizeof( recvid ) );
		}
		else if ( myid < recvid )
		{
			write( STDOUT_FILENO, &myid, sizeof( myid ) );
		}
		else if ( sRC == 0 )
		{
			write( STDOUT_FILENO, &myid, sizeof( myid ) );
		}
	
		sRC = read( STDOUT_FILENO, &recvid, sizeof( recvid ) );

		if ( recvid == myid )
		{
			break;
		}

	}

	fprintf( stderr, "[i : %d][%d] %d %d %d %d\n", i, getpid(), procs[0], procs[1], procs[2], procs[3] );


	wait();
	
	return 0;
}
