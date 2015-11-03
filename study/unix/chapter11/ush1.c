#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_BUF 256
#define QUIT_STRING "q"

int makeArgv( const char *aOrigText, const char *aDelimiters, char ***argvp, int *aArgc );

int main ( void )
{
	int argc = 0;
	int pid = 0;
	int sRC = 0;
	char **chargv = NULL;
	char inbuf[MAX_BUF] = {0, };

	while ( 1 )
	{
		gets( inbuf );
		if ( strcmp( inbuf, QUIT_STRING ) == 0 )
			return 0;

		pid = fork();
		if ( pid < 0 )
			return -1;

		if ( pid == 0 )
		{
			sRC = makeArgv( inbuf, " ", &chargv, &argc );
			if ( sRC == 0 && argc > 0 )
			{
				execvp( chargv[0], chargv );
			}
		}
		else
		{
			wait( NULL );
		}
	}

	return 0;
}
