#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

#define BUF_SIZE 255
#define QUIT_CHAR "Q"

void executecmd( char *incmd );

int main( void )
{
	int pid = 0;

	int terfd = 0;
	char termP[L_ctermid] = {0, };
	int len = 0;
	char buf[BUF_SIZE] = {0, };

	while ( 1 )
	{
		fputs( "Prompt : ", stdout );
		gets( buf );
		len = strlen( buf );

		if ( buf[len - 1] == '\n' )
			buf[len - 1] = '\0';

		if ( strcmp( buf, QUIT_CHAR ) == 0 )
			break;

		if ( strcmp( buf, "bg" ) == 0 )
		{
			pid = fork();
			if ( pid == -1 )
			{
				fprintf( stderr, "fork error\n" );
				continue;
			}
			else if ( pid == 0 )
			{
				pid = setsid();
				fprintf( stderr, "background process %d is created\n", pid );
				execvp( "b", NULL );
			}
		}
		else if ( strncmp( buf, "fg", 2 ) == 0 )
		{
			ctermid( termP );
			terfd = open( termP, O_RDWR );
			pid = tcgetpgrp(terfd);
			close( terfd );

			setpgid( getpid(), pid );
		}
		else
		{
			executecmd( buf );
		}
	}

	return 0;
}
