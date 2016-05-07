#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#define MMAP_FILENAME "mmap.log"
#define MMAP_SIZE	  1024
#define INPUT_SIZE    64
void err_handling( char *msg, int errno )
{
	printf( "%s\nerrno( %d : %s )\n", msg, errno, strerror( errno ) );
}

int main( int argc, char **argv )
{
	int fd;
	int mapCursor = 0;

	int flag_mmap;
	char *mmapPtr = NULL;

	int len = 0;
	char a_input[INPUT_SIZE] = {0, };

	if ( argc != 2 )
	{
		printf( "Usage : %s < s / p > \n", argv[0] );
		return -1;
	}

	fd = open( MMAP_FILENAME, O_RDWR | O_CREAT | O_TRUNC, 0664 );

	if ( fd == -1 )
	{
		err_handling( "Fail : open()", errno );
		return -1;
	}

	if ( *argv[1] == 's' )
	{
		flag_mmap = MAP_SHARED;
		printf( "SHARED MMAP\n" );
	}
	else
	{
		flag_mmap = MAP_PRIVATE;		
		printf( "PRIVATE MMAP\n" );
	}

	mmapPtr = mmap( ( void *)0, MMAP_SIZE, PROT_READ | PROT_WRITE, flag_mmap, fd, 0 );

	if ( mmapPtr == MAP_FAILED )
	{
		err_handling( "Fail : mmap()", errno );
		return -1;
	}

	if ( ftruncate( fd, (off_t)MMAP_SIZE ) == -1 )
	{
		err_handling( "ftruncate() error", errno );
		return -1;
	}

	while( 1 )
	{
		printf( "'*' printf current mmap otherwise input text to mmap. >> " );

		if ( fgets( a_input, INPUT_SIZE, stdin ) == NULL )
		{
			err_handling( "Fail : fgets()", errno );
			return -1;
		}
		len = strlen( a_input );

		a_input[len] = 0;
		if ( strcmp( a_input, "*\n" ) == 0 )
		{
			/* %.* 폭 지정 * 는 숫자와 대응한다. */
			printf( "Current mmap\n'%.*s'\n", MMAP_SIZE, mmapPtr );
		}
		else if ( strcmp( a_input, ".exit\n" ) == 0 )
		{
			printf( "Current mmap\n'%.*s'\n", MMAP_SIZE, mmapPtr );
			fprintf( stderr, "exit\n" );		
			break;
		}
		else
		{
			memcpy( mmapPtr + mapCursor, a_input, strlen( a_input ) );
			mapCursor += strlen( a_input );
		}
	
		if ( msync( mmapPtr, mapCursor, MS_SYNC ) == -1 )
		{
			err_handling( "Fail : msync()", errno );
			return -1;
		}
	}

	munmap( (void*)mmapPtr, MMAP_SIZE );

	close( fd );
	return 0;
}

