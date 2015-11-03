#include <stdio.h>
#include <unistd.h>
#include "clnt.h"
#include "buf.h"
#include <string.h>

int child( void )
{
	int loop = 0;
	int sRC = 0;

	int sData = 0;
	char msg[100] = {0, };

	fprintf( stderr, "pid[%d] is started\n", getpid() );

	while ( loop == 0 )
	{
		sRC = getItem( &sData );
		if ( sRC != 0 )
		{
			perror( "get item error" );
		}
		else
		{
			if ( sData == ENDMSG )
			{
				loop = 1;
			}
			else
			{
				snprintf( msg, 100, "pid[%d] : %d\n", getpid(), sData );
				write( STDOUT_FILENO, msg, strlen( msg ) );
			}
		}
	}

	fprintf( stderr, "client[%d] is ended\n", getpid() );

	return 0;
}

