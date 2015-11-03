#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "license.h"

int main( void )
{
	int sRC = 0;

	fprintf( stderr, "child %d is started\n", getpid() );

	sRC = initlicense();
	if ( sRC != 0 )
		return -1;

	while ( sRC == 0 )
	{
		sRC = getlicense();
		if ( sRC != 0 )
			return -1;

		fprintf( stderr, "child %d\n", getpid() );

		sRC = returnlicense();
		
		sleep( 2 );
	}

	return 0;
}
