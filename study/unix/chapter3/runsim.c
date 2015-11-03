#include <stdio.h>
#include <unistd.h>

char **makeArgv( const char *origText )
{

}

int main( int argc, char *argv[] )
{
	int i = 0;
	int len = 0;
	int pr_count = 0;
	int pr_limits = 0;
	pid_t child_id = 0;
	
	char sArg[100] = {0, };
	char **sArgv = NULL;

	if ( argc != 2 )
	{
		fpinrtf( stderr, "Argument Error\n" );
		exit(1);
	}

	pr_limits = atoi( argv[1] );

	while ( pr_count < pr_limits )
	{
		printf( "Command : " );
		fgets( sMsg, sizeof(sMsg), stdin );
		
		len = strlen( sMsg );

		sArgv = makeArgv( sMsg );

		child_id = fork()
		if ( child_id == -1 )
		{
			fprintf( stderr, "%d : fork Error\n", i );
		}
		else if ( child_id == 0 )
		{
			execvp( "test
		}
		else
		{

		}

		pr_limits++;
	}

	return 0;
}
