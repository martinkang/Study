#include <sys/shm.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h> /* getcwd */
#include "shmheader.h"

#define _MAX_PATH 255

int sysv_shmget( char  *tok,
				 key_t  shm_fixkey,
				 int    size,
				 int    user_mode )
{
	key_t shm_key;
	int shm_id;
	
	char curPath[_MAX_PATH] = {0, };

	if ( tok != NULL )
	{
		getcwd( curPath, _MAX_PATH - 1 );
		if (( shm_key = ftok( curPath, atoi( tok ) ) ) == -1 )
		{
			fprintf( stderr, "Fail create shm_key\n" );
			return -1;
		}
	}
	else
	{
		shm_key = shm_fixkey;
	}
	printf( "shm_ket get success\nshm_key : %d", shm_key );

	if (( shm_id = shmget( shm_key, size, IPC_CREAT | IPC_EXCL | user_mode )) == -1 )
	{
		if ( errno == EEXIST ) /* 이미 공유 메모리가 존재하는 경우 키를 통해 ID 를 가져옴 */
		{
			shm_id = shmget( shm_key, 0, 0 );
		}
	}
	if ( shm_id == -1 )
	{
		fprintf( stderr, "Fail : shmget() %s[%s:%d]\n", strerror( errno ), __FUNCTION__, __LINE__ );
	}

	return shm_id;
}

int sysv_shmrm( int shm_id )
{
	int ret;

	if (( ret = shmctl( shm_id, IPC_RMID, NULL )) == -1 )
	{
		fprintf( stderr, "FAIL : shmctl() : %s [%s:%d]\n", strerror( errno ), __FUNCTION__, __LINE__ );
		return -1;
	}

	return ret;
}
