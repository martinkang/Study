#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MAX_CANON
#define MAX_CANON 8192
#endif

int runproc( char *cmd );
void showhistory( FILE *f );

int main( int argc, char *argv[] )
{
	char cmd[MAX_CANON];
	int history = 1;
	int len = 0;
	int sRC = 0;

	if ( argc == 1 )
	{
		history = 0;
	}
	else if ( argc > 2 )
	{
		IDE_TEST_RAISE( 0, TOO_MANY_ARG );		
		fprintf( stderr, "Too many argument\n" );
		return 1;
	}

	while ( fgets( cmd, MAX_CANON, stdin ) != NULL )
	{
		len = strlen(cmd);
		if ( cmd[len - 1]  == '\n' )
		{
			cmd[len - 1] = '\0';
		}
		else
		{
			/* do nothing */
		}

		sRC = strcmp( cmd, ".exit" );
		if ( sRC == 0 )
		{
			printf("Program exit\n");
			break;
		}
		sRC = strcmp( cmd, "history" );
		
		if ( history != 0 && sRC == 0 )
		{
			showhistory( stdout );
		}
		else
		{ 
			sRC = runproc(cmd);
			if ( sRC == -1 )
			{
				perror( "Failed to execute command" );
				break;
			}
			else
			{
				history++;
			}
		}
	}
	printf ("\n\n>>>>>>> The list of command executed is : \n" );
	showhistory(stdout);
	return 0;

	TOO_MANY_ARG
}
