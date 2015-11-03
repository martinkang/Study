#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "shmfun.h"
#include "semfun.h"

#define BUF_SIZE 256
#define FLAG ( O_CREAT | O_EXCL )
#define PERMS (mode_t)( S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH | S_IWGRP | S_IWOTH )

typedef struct msg
{
    int mFlag;
    char mMsg[1];
} msg_t;

int main( int argc, char *argv[] )
{
	int sRC = 0;
	char *s_name = NULL;

	int shmid = 0;
	key_t sKey = 0;
	sem_t *sSem = NULL;

	msg_t *sMsg = NULL;
	if ( argc != 2 )
	{
		fprintf( stderr, "Argument error\n" );
		return -1;
	}

	fprintf( stderr, "child[%d] is started\n", getpid() );

	s_name = argv[0];	
	sKey = (key_t)atoi( argv[1] );

	fprintf( stderr, "child[%d] received s_name : %s key : %d\n", getpid(), s_name, sKey );

	sRC = initShm( sKey, &shmid, (void**)&sMsg, sizeof( msg_t ) + BUF_SIZE );
	if ( sMsg == (void*)-1 )
	{
		fprintf( stderr, "child[%d] shm init error\n", getpid() );
		return -1;
	}
	fprintf( stderr, "child[%d] is attach shm is success\n", getpid() );

	sRC = initSem( &sSem, s_name, FLAG , PERMS, 1 );
	if ( sRC != 0 )
	{
		fprintf( stderr, "child[%d] init sem error\n", getpid() );
	}
	else
	{
		while ( 1 )
		{
			sem_wait( sSem );
			if ( sMsg->mFlag == 1 )
			{
				fprintf( stderr, "child[%d] read shm msg : %s\n", sMsg->mMsg );
				sMsg->mFlag = 0;
			}
			sem_post( sSem );
			if ( strcmp( sMsg->mMsg, "q" ) == 0 )
				break;
		}

	}

	destroySem( sSem, s_name );

	shmdt( (void*)sMsg );

	fprintf( stderr, "child[%d] is ended\n", getpid() );

	return 0;
}
