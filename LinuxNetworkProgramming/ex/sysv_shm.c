#include <stdio.h>
#include <sys/shm.h>
#include "shmheader.h"

#define SZ_SHM_SEGMENT 1024

int shm_id;
char *shm_ptr;

int main( int argc, char *argv[] )
{
	char a_input[0xff] = {0, };
	struct shmid_ds shmds_buf;

	printf ( "c : create shared memory with IPC_PRIVATE.\n" );
	printf( "number : attach shared memory with IPC id number.\n>>" );

	fgets( a_input, sizeof( a_input ), stdin );

	if ( a_input[0] == 'c' )
	{
		shm_id = sysv_shmget( NULL, IPC_PRIVATE, sizeof( char) * SZ_SHM_SEGMENT, 0664 );
	}
	else
	{
		shm_id = sysv_shmget( a_input, 0, sizeof( char) * SZ_SHM_SEGMENT, 0664 );
	}

	if ( shm_id == -1 )
	{
		fprintf( stderr, "FAIL : sysv_shsmget() \n" );
		return -1;
	}

	printf( "* SHM IPC id : %d\n", shm_id );

	shm_ptr = (char*)shmat( shm_id, 0, 0 );
	if ( shm_ptr == NULL )
	{
		fprintf( stderr, " FAIL : shmat()\n" );
		return -1;
	}

	printf( "attached shm pointer : %p\n", shm_ptr );

	printf( "'*' Print current shm.\n'.' Exit. Otherwise input text to shm.\n" );
	printf( "'?' print shm info\n" );

	printf( "start shared memory example\n" );
	while(1)
	{
		printf( "\n>>");

		if ( fgets( a_input, sizeof( a_input ), stdin ) == NULL )
		{
			fprintf( stderr, "fget() error\n" );
			return -1;
		}

		if ( a_input[0] == '.' )
		{
			break;
		}
		else if ( a_input[0] == '?' )
		{
			if ( shmctl( shm_id, IPC_STAT, &shmds_buf ) == -1 )
			{
				fprintf( stderr, "shmctl() error\n" );
			}
			else
			{
				printf( "SHM : size(%d) # of attach (%ld)\n", shmds_buf.shm_segsz, shmds_buf.shm_nattch );
			}
		}
		else if ( a_input[0] == '*' )
		{
			printf( "shm -> '%.*s'\n", SZ_SHM_SEGMENT, shm_ptr );
		}
		else
		{
			memcpy( shm_ptr, a_input, strlen( a_input ) );
		}
	}

	printf( "* Would you remove shm ( IPC id : %d ) ( y/n ) ", shm_id );
	fgets( a_input, sizeof( a_input ), stdin );

	if ( a_input[0] != 'n' )
	{
		sysv_shmrm( shm_id );
	}
	
	return 0;
}
