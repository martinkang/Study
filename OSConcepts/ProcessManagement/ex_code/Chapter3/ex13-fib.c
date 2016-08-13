#include <stdio.h>
#include <unistd.h>

int main( void )
{
	int n = 0;
	int fib1 = 0;
	int fib2 = 1;
	int temp = 0;
	int i = 0;

	pid_t sPid;

	printf( "Input n : " );
	scanf( "%d", &n );

	fprintf( stderr, "%d ", fib1 );
	
	for ( i = 0; i < n; i++ )
	{
		sPid = fork();
		if ( sPid == 0 )
		{
			fprintf( stderr, "%d ", fib2 );
			temp = fib2;
			fib2 = fib1 + fib2;

			fib1 = temp;
		}
		else if ( sPid > 0 )
		{
			wait( sPid );
			break;
		}
		else
		{
			fprintf( stderr, "fork() error\n" );
			return -1;
		}
	}
	
	fprintf( stderr, "\n" );

	return 0;
}
