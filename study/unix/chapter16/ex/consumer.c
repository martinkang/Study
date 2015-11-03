#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "manager.h"
#include "consumer.h"
#include "buf.h"
#include "../../ide.h"

void C_manager( void )
{
	int sRC = 0;
	int sFlag = 0;

	int sItem = 0;

	char msg[MSGSIZE] = {0, };

	while ( sFlag == 0 )
	{
		sRC = getitem( &sItem );
		if ( sRC != 0 )
			return;

		snprintf( msg, MSGSIZE, "C_thread[%d] getitem : %d\n", pthread_self(), sItem );
		write( STDOUT_FILENO, msg, strlen( msg ) );

		getdone( &sFlag );
		sleep( 1 );
	}
	
	endsignal();
	
	return;
}
