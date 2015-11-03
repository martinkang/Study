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

#define KEY 5678

typedef struct msg
{
	int mFlag;
	char mMsg[1];
} msg_t;

int main( int argc, char *argv[] )
{
	int sRC = 0;
	int i = 0;
	int n = 0;
	int loop = 1;
	char msg[BUF_SIZE - 4] = {0, };

	msg_t *sMsg = NULL;
	char in_msg[BUF_SIZE];

	int pid = 0;
	char c_fname[] = "child\0";

	int shmid = 0;
	char c_key[20] = {0, };
	sem_t *sSem;
	char s_name[] = "sem";


	if ( argc != 2 )
	{
		fprintf( stderr, "Argument error\n" );
		return -1;
	}
	
	sem_unlink( s_name );

	n = atoi( argv[1] );
	if ( n <= 0 )
	{
		fprintf ( stderr, "n is smaller than zero\n" );
		return -1;
	}

	sRC = initSem( &sSem, s_name, FLAG, PERMS, 1 );
	if ( sRC != 0 )
	{
		perror( "init sem error" );
		return -1;
	}

	sRC = initShm( KEY, &shmid, (void**)&sMsg, sizeof( msg_t ) + BUF_SIZE );
	if ( sRC != 0 )
	{
		perror( "init shm error" );
		return -1;
	}
	fprintf( stderr, "Shared memory[%d] is created\n", shmid );

	for ( i = 0; i < n; i++ )
	{
		pid = fork();
		if ( pid == 0 )
		{
			fprintf( stderr, "child[%d] is created\n", getpid() );

			snprintf( c_key, 20, "%d\0", KEY );
			sRC = execl( c_fname, s_name, c_key, NULL );
			if ( sRC == -1 )
			{
				perror( "execl error" );
				return -1;
			}

			return 0;
		}
		else if ( pid < 0 )
		{
			return -1;
		}
	}

	sleep( 1 );

	sMsg->mFlag = 0;
	while( loop != 0 )
	{
		if ( sRC == -1 )
			break;

		sRC = sem_wait( sSem );
		if ( sMsg->mFlag == 0 )
		{
			sRC = fprintf( stdout,  "prompt : " );

			if ( gets( msg ) == NULL )
			break;

			strcpy( sMsg->mMsg, msg );
			sMsg->mFlag = 1;
		}
		sRC = sem_post( sSem );
		if ( sRC != 0 )
			break;

		loop = strcmp( msg, "q" );
	}
	
	if ( sRC == 0 )
	{
		for ( i = 0; i < n; i++ )
		wait( NULL );
	}

	fprintf( stderr, "Shared memory[%d] is destroyed\n", shmid );
	sRC = destroyShm( shmid, (void*)sMsg );
	if ( sRC != 0 )
		return -1;

	sRC = destroySem( sSem, s_name );
	if ( sRC != 0 )
		return -1;

	fprintf( stderr, "Shared memory[%s] is detached\n", s_name );
	return 0;
}
