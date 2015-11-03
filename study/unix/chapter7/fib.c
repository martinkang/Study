#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define N_PROCS 20
#define BUF_MAX 1024

void sep( char *aStr, int *f1, int *f2 )
{
	int i = 0;
	int idx = 0;
	int len = 0;

	char *cpy = NULL;
	
	len = strlen( aStr );
	cpy = (char*)malloc( sizeof(char) * (len + 1) );
	strcpy( cpy, aStr );
	cpy[len] = '\0';

	
	for ( i = 0; i < len; i++ )
	{
		if ( cpy[i] == ' ' )
		{
			cpy[i] = '\0';
			idx = i + 1;
		}
	}

	*f1 = atoi( aStr );
	*f2 = atoi( ( aStr + idx ) );
}

int main( int argc, char *argv[] )
{
	int sRC = 0;
	char fib[BUF_MAX] = {0, };
	int pid = 0;
	int fd[2];
	int i = 0;
	int f1 = 1;
	int f2 = 1;
	int temp = 0;
	int root = getpid();

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
			dup2( fd[1], STDOUT_FILENO );

		}
		else if ( pid == 0 )
		{
			dup2( fd[0], STDIN_FILENO );
			
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
		snprintf( fib, BUF_MAX, "%d %d", f1, f2 );

		write( STDOUT_FILENO, fib, sizeof(fib) );
		fprintf( stderr, "ROOT : %ld\n", (long)root );
	}
	else
	{
		read( STDIN_FILENO, fib, sizeof(fib) );

		sep( fib, &f1, &f2 );

		temp = f1;

		f1 = f2;

		f2 = temp + f1;

		snprintf( fib, BUF_MAX, "%d %d", f1, f2 );

		write( STDOUT_FILENO, fib, sizeof(fib) );
		//alread child process is exited
		//so pipe is brocken
	}

	wait();
	
	fprintf( stderr, "[i : %d %ld] FIB : %s  F2 : %d\n", i, (long)getpid(), fib, f2 );

	return 0;
}
