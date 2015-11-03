#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main( void )
{
	int i = 0;
	int t = 1;

	while( 1 )
	{
		if ( i % 5 == 0 )
			printf( "i : %d\n", i );
		sleep(t);
		if ( i % 10 == 0 )
			printf("10x\n", i );

		i++;
	}
}
