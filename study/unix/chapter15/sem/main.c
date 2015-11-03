#include <stdio.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
#include <unistd.h>

#define NUM_THREAD 4
#define BUF_SIZE 1024

void setsembuf( struct sembuf *sem, unsigned short aNum,  short aOp, short aFlg )
{
	sem->sem_num = aNum;
	sem->sem_op = aOp;
	sem->sem_flg = aFlg;
}

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
} arg;

int main( void )
{
	int sRC = 0;
	int i = 0;
	int j = 0;
	int delay = 1000000;

	int pid = 0;
	int cid = 0;

	int len = 0;
	char msg[BUF_SIZE] = {0, };
	char *ch = NULL;

	int semID = 0;
	struct sembuf semwait;
	struct sembuf semsignal;


	semID = semget( IPC_PRIVATE, 1, IPC_CREAT | S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH | S_IRGRP | S_IWGRP );

	setsembuf( &semwait, 0, -1, 0 );
	setsembuf( &semsignal, 0, 1, 0 );

	arg.val = 1;

	semctl( semID, 0, SETVAL,  arg );
	for ( i = 0; i < NUM_THREAD; i++ )
	{
		pid = fork();
		if ( pid == 0 )
		{
			break;
		}
		else if ( pid < 0 )
		{
			return -1;
		
		}
	}

	if ( pid > 0 )
	{
		for ( i = 0; i < NUM_THREAD; i++ )
			wait( NULL );

		semctl( semID, 0, IPC_RMID );
	}
	else
	{
		snprintf( msg, BUF_SIZE, "PID : %d PPID : %d MSG : semaphore test\n", getpid(), getppid() );
		ch = msg;

		semop( semID, &semwait, 1 );
		
		while( *ch != '\0' )	
		{
			fputc( *ch, stderr );
			ch = ch + 1;
			for ( j = 0; j < delay; j++ );
		}
		semop( semID, &semsignal, 1 );

	}
	return 0;
}

