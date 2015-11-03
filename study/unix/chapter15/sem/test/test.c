#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void)
{
	int sRC = 0;
	int i = 0;
	int pid = 0;

	int c = 0;
	char sname[] = "sem";
	sem_t *sem;

	sem_unlink( sname );

	sem = sem_open( sname, O_CREAT | O_EXCL );
	sRC = sem_init( sem, 1, 1 );
	if ( sRC != 0 )
	{
		fprintf( stderr, "sem init error\n" );
	}


	pid = fork();
	if ( pid == 0 )
	{
		while ( c < 10 )
		{
			sem_wait( sem );
			fprintf( stderr, "im child %d\n", getpid() );
			sem_post( sem );
			c++;
			sleep( 1 );
		}
		return 0;
	}
	else if ( pid > 0 )
	{
		while ( c < 15 )
		{
			sem_wait( sem );
			fprintf( stderr, "im parent %d\n", getpid() );
			sem_post( sem );
			c++;
			sleep( 1 );
		}

		wait( NULL );
	}

	sRC = sem_close( sem );
	if ( sRC != 0 )
	{
		fprintf( stderr, "sem close error\n" );
	}

	sRC = sem_unlink( sname );
	if ( sRC != 0 )
	{
		fprintf( stderr, "sem unlink error\n" );
	}

	return 0;

}
