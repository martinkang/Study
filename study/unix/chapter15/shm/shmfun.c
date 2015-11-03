#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "shmfun.h"

#define PERMS ( S_IRUSR | S_IWUSR )

int initShm( key_t aKey, int *shmid, void **addr, int size )
{
	int sRC = 0;
	int id = 0;

	id = shmget( aKey, size, IPC_CREAT | PERMS );
	if ( id == -1 )
	{
		perror( "shm get error" );
		return -1;
	}

	*shmid = id;

	*addr = shmat( id, NULL, PERMS );
	if ( *addr == (void *)-1 )
		perror( "shm attach error" );

	return sRC;
}

int destroyShm( int shmid, void *addr )
{
	int sRC = 0;

	sRC = shmdt( addr );
	if ( sRC == -1 )
	{
		perror( "shm detach error" );
		return -1;
	}

	sRC = shmctl( shmid, IPC_RMID, NULL );
	if ( sRC == -1 )
		perror ( "shm destroy error" );

	return sRC;
}
