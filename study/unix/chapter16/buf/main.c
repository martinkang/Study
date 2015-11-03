#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "buf.h"
#include "clnt.h"

#define CLNT_NUM 3

int main( void )
{
	int sRC = 0;
	int i = 0;
	int loop = 0;

	char prompt[] = "Input Number ( .Q is Exit ) : ";
	pthread_t th[CLNT_NUM];

	int sData = 0;

	char msg[5] = {0, };

	while ( initBuf() != 0 )
		perror( "init buf error" );

	for ( i = 0; i < CLNT_NUM; i++ )
	{
		sRC = pthread_create( &( th[i] ), NULL, (void*)child, NULL );
	}

	sleep( 1 );

	while( loop == 0 )
	{
		fflush( stdout );
		write( STDOUT_FILENO,  prompt, strlen( prompt ) );
		if ( gets( msg ) == NULL )
			loop = 1;

		if ( strcmp( msg, ".Q" ) == 0 )
		{
			loop = 1;
			for ( i = 0; i < CLNT_NUM; i++ )
				putItem( ENDMSG );
		}
		else
		{
			sData = atoi( msg );
			sRC = putItem( sData );
			if ( sRC != 0 )
				loop = 1;
		}
	}

	for ( i = 0; i < CLNT_NUM; i++ )
		pthread_join( th[i], NULL );

	fprintf( stderr, "finalize\n" );
	finalBuf();
	
	return 0;
}
