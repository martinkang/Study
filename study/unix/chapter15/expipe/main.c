#include <stdio.h>
#include <errno.h>
#include "pipe.h"


int main( void )
{
	int sRC = 0;
	char buf[100] = {0, };
	pipe_t *pp;

	pp = pipe_open();

	sRC = pipe_read( pp, buf, 100);
	if ( sRC == -1 )
		perror( "pope read error" );

	sRC = pipe_write( pp, "aaaaaaaaaa", 10 );
	if ( sRC == -1 )
		perror( "pipe write error" );

	sRC = pipe_read( pp, buf, 100);
	if ( sRC == -1 )
		perror( "pope read error" );
	printf( "%s\n", buf );

	sRC = pipe_close( pp );
	return 0;
}
