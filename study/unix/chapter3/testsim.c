#include <stdio.h>
#include <unistd.h>

int main( int argc, char *argv[] )
{
	int sNum = 0;
	int sSleepTime = 0;
	int i = 0;

	if ( argc != 3 )
	{
		fprintf( stderr, "Argument Error\n" );
	}

	sNum = atoi( argv[1] );
	sSleepTime = atoi( argv[2] );

	for ( i = 0; i < sNum; i++ )
	{
		sleep( sSleepTime );
		fprintf( stderr, "My PID : %ld My Parent ID : %ld\n", (long)getpid(), (long)getppid );
	}

	return 0;
}
