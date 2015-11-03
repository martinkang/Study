#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define BUF_SIZE 255

int processchar ( int fdin, int fdout, char *inchar, char *outstr )
{
	int sRC = 0;
	char msg[BUF_SIZE] = {0, };
	char cpymsg[BUF_SIZE] = {0, };
	char wrmsg[BUF_SIZE] = {0, };
	char *idx = NULL;

	while ( ( sRC = read( fdin, msg, BUF_SIZE ) ) != 0 )
	{
		if ( sRC == -1 )
		{
			perror( "read error" );
			return -1;
		}

		strcpy( cpymsg, msg );
	
		if ( inchar != NULL )
			idx = strtok( cpymsg, inchar );
	
		if ( idx != NULL)
		{
			while ( idx != NULL )
			{
				snprintf( wrmsg, BUF_SIZE, "%s%s", idx, outstr );

				idx = strtok( NULL, inchar );
				
				sRC = write( fdout, wrmsg, strlen( wrmsg ) );
				if ( sRC < strlen( wrmsg ) )
				{
					perror( "write error" );
					return -1;
				}
			}
		}
		else
		{
			sRC = write( fdout, msg, strlen( msg ) );
			if ( sRC < strlen( msg ) )
			{
				perror( "write error" );
				return -1;
			}
		}
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
	int root = getpid();
	int last = 0;

	char inPath[] = "file.in";
	char outPath[] = "file.out";
	int fdin;
	int fdout;

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

	fdin = open( inPath, O_RDONLY );
	if ( fdin == -1 )
	{
		perror( "fdin open error" );
		return -1;
	}

	fdout = open( outPath, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IWGRP | S_IWOTH );
	if ( fdout == -1 )
	{
		perror( "fdout open error" );
		return -1;
	}

	for ( i = 1; i < nprocs; i++ )
	{
		pipe( fd );

		cpid = fork();
		if ( cid == 0 ) //child
		{
			sRC = dup2( fd[1], STDOUT_FILENO );	
			if ( sRC == -1 )
				return -1;

			if ( i == nprocs - 1 )
				last = getpid();
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

	if ( getpid() == root )
	{
		sRC = processchar( fdin, STDOUT_FILENO, " ", "-!" );
		if ( sRC == -1 )
		{
			fprintf( stderr, "processchar error\n" );
			return -1;
		}
	}
	else if ( getpid() == last )
	{
		sRC = processchar( STDIN_FILENO, fdout, NULL, NULL );
		if ( sRC == -1 )
		{
			fprintf( stderr, "processchar error\n" );
			return -1;
		}
	}
	else
	{
		sRC = processchar( STDIN_FILENO, STDOUT_FILENO, NULL, NULL );
		if ( sRC == -1 )
		{
			fprintf( stderr, "processchar error\n" );
			return -1;
		}
	}

	close( fdout );
	close( fdin );

	wait( &status );
	
	return 0;
}
