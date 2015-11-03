#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <errno.h>	

int initSem( sem_t **aSem, const char *aName, int oflag, mode_t aMode, unsigned aValue )
{
	int sRC = 0;

	*aSem = sem_open( aName, oflag, aMode, aValue );
	if ( *aSem == SEM_FAILED && errno == EEXIST )
	{
		*aSem = sem_open( aName, 0 );
		if ( *aSem == SEM_FAILED )
			return errno;
	}
	
	if ( sRC == -1 )
		return errno;

	return 0;
}

int destroySem( sem_t *aSem, char *aName )
{
	int sRC = 0;

	sRC = sem_close( aSem );
	if ( sRC == -1 )
		return errno;

	sRC = sem_unlink( aName );
	if ( sRC == -1 )
		return errno;

}
