#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ntpvm.h"
#include "proc.h"

#define BUFSIZE 1024

int newtaskproc( taskpacket_t *header, task_t *task, int *fd )
{
	int sRC = 0;
	int pid = 0;

	char pipebuf[BUFSIZE] = {0, };
	unsigned char buf[MAX_PACK_SIZE];

	pid = fork();
	if ( pid == 0 )
	{
		sRC = dup2( fd[0], STDIN_FILENO );
		if ( sRC == -1 )
			return -1;

		sRC = dup2( fd[1], STDOUT_FILENO );
		if ( sRC == -1 )
			return -1;

		close ( fd[0] );
		close ( fd[1] );

		sRC = execommand( &header, buf );
		if ( sRC == -1 )
			return -1;

		return 0;
	}
	else if ( pid < 0 )
	{
		return -1;
	}

	if ( pid > 0 )
	{
		close( fd[1] );

		while ( ( size = read( fd[0], pipebuf, BUFSIZE ) ) > 0 )
		{
			fprintf( stderr, "%s", pipebuf );
		}
		wait( NULL );

		close( fd[0] );
	}

}

int dataproc( taskpacket_t *header, task_t *task, int *fd )
{
}

int doneproc( taskpacket_t *header, task_t *task, int *fd )
{
}
