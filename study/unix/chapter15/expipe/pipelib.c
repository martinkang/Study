#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>
#include "pipe.h"

#define PERMS ( S_IWUSR | S_IRUSR )

typedef union semun 
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
} semun_t;

void setSembuf( struct sembuf *sem_buf, unsigned short sem_num, short sem_op, short sem_flg )
{
	sem_buf->sem_num = sem_num;
	sem_buf->sem_op = sem_op;
	sem_buf->sem_flg = sem_flg;
}

pipe_t *pipe_open( void )
{
	int sRC = 0;
	semun_t arg;

	pipe_t *newPipe = (pipe_t*)malloc( sizeof( pipe_t ) );

	newPipe->semid = semget( IPC_PRIVATE, 2, PERMS );
	if ( newPipe->semid == -1 )
	{
		perror( "semget error" );
		return NULL;
	}

	arg.val = 1;
	semctl( newPipe->semid, 0, SETVAL, arg );

	newPipe->shmid = shmget( IPC_PRIVATE, sizeof( pipe_t ), IPC_CREAT | IPC_EXCL | PERMS );
	if ( newPipe->shmid == -1 && errno == EEXIST )
	{
		newPipe->shmid = shmget( IPC_PRIVATE, sizeof( pipe_t ), IPC_CREAT | PERMS );
		if ( newPipe->shmid == -1 )
			return NULL;
	}
	else if ( newPipe->shmid == -1 && errno != EEXIST )
	{
		perror( "shmget error" );
		return NULL;
	}

	newPipe = (pipe_t*)shmat( newPipe->shmid, NULL, PERMS );
	if ( newPipe->data == (void*)-1 )
	{
		perror( "shmat error" );
		return NULL;
	}

	newPipe->data_size = 0;
	newPipe->cur_start = NULL;
	newPipe->end_of_file = 0;

	return newPipe;	
}

int pipe_read( pipe_t *p, char *buf, int bytes )
{
	int sRC = 0;

	struct sembuf slock;
	struct sembuf sunlock;

	setSembuf( &slock, 1, -1, 0 );
	setSembuf( &sunlock, 1, 1, 0 );

	sRC = semop( p->semid, &slock, 1 );
	if ( sRC == -1 )
		return sRC;

	if ( p->data_size > 0 )
	{
		if ( bytes > p->data_size )
		{
			bytes = p->data_size;
		}
		strncpy( buf, p->cur_start, bytes );

		p->data_size = p->data_size - bytes;
		if ( p->data_size == 0 )
		{
			p->end_of_file = 1;
		}
		else
		{
			p->cur_start = p->cur_start + bytes;
		}
	}
	else if ( p->data_size == 0 && p->end_of_file == 1 )
	{
		fprintf( stderr, "pipe is empty\n" );
		sRC =  semop( p->semid, &sunlock, 1 );
		if ( sRC == -1 )
			return sRC;

		return 0;
	}

	sRC = semop( p->semid, &sunlock, 1 );
	if ( sRC == -1 )
		return sRC;

	return bytes;
}

int pipe_write( pipe_t *p, char *buf, int bytes )
{
	int sRC = 0;

	struct sembuf slock;
	struct sembuf sunlock;

	setSembuf( &slock, 1, -1, 0 );
	setSembuf( &sunlock, 1, 1, 0 );

	sRC = semop( p->semid, &slock, 1 );
	if ( sRC == -1 )
		return sRC;

	if ( bytes > _POSIX_PIPE_BUF )
		bytes = _POSIX_PIPE_BUF;

	strncpy( p->data, buf, bytes );
	p->data_size = bytes;
	p->cur_start =&( p->data[0] );

	sRC = semop( p->semid, &sunlock, 1 );
	if ( sRC == -1 )
		return sRC;

	return bytes;
}

int pipe_close( pipe_t *p )
{
	int sRC = 0;
	
	sRC = shmdt( (void*)p );
	if ( sRC == -1 )
	{
		perror( "shmdt error" );
		return -1;
	}

	sRC = shmctl( p->shmid, IPC_RMID, NULL );
	if ( sRC != 0 )
	{
		perror( "remove shmid error" );
		return -1;
	}

	semctl( p->semid, IPC_RMID, 0 );
	if ( sRC != 0 )
	{
		perror( "remove semid error" );
		return -1;
	}
}
