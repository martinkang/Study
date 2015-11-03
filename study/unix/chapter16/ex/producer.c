#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "manager.h"
#include "producer.h"
#include "buf.h"

void P_manager( void )
{
    int sRC = 0;
    int sFlag = 0;
	int ran_num = 0;

	char msg[MSGSIZE] = {0, };
	
	srand( (unsigned) time(NULL ) );

    while ( sFlag == 0 )
    {		
		ran_num = rand() % 20 + 1; 
		fprintf( stderr, "pthread[%d] rand %d\n",pthread_self(), ran_num );
		sRC = putitem( ran_num );
		if ( sRC != 0 )
			return;

        getdone( &sFlag );
		sleep( 1 );
    }

	endsignal();

    return;
}
