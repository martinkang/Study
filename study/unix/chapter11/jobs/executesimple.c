#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BLANK_STRING " "

int makeArg( const char *aOrigText, const char *aDelimiters, char ***argvp );

void executecmd( char *incmd )
{
	int argc = 0;
	int sRC = 0;
	char **chargv = NULL;
	
	argc = makeArgv( incmd, BLANK_STRING, &chargv, &argc );
	if ( sRC < 0 || argc == 0 )
	{
		fprintf( stderr, "Failed to parse command line\n" );
		exit(1);
	}

	execvp( chargv[0], chargv );
	perror( "Failed to execute command" );
	exit(1);
}
