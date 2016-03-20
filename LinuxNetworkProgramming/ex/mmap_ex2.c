#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>

#define MMAP_FILENAME "shared_mmap"
#define MMAP_SIZE	  64

char contents[] = "(*)What`s differents between Shared mmap and Private mmap? -end-\n";

void err_handling( char *msg, int errno )
{
	printf( "%s\nerrno( %d : %s )\n", msg, errno, strerror( errno ) );
}

int main( int argc, char **argv )
{
	int fd, flag_mmap, n_write;
	char *p_map, a_input[100] = {0, };

	if ( argc != 2 )
	{
		printf( "Usage : %s < s / p > \n", argv[0] );
		return -1;
	}

	fd = open( MMAP_FILENAME, O_RDWR | O_CREAT, 0664 );

	if ( fd == -1 )
	{
		err_handling( "Fail : open()", errno );
		return -1;
	}

	if ( ( n_write = write( fd, contents, sizeof( contents ) - 1 ) ) == -1 )
	{
		err_handling( "Fail : write()", errno );
		return -1;
	}

	printf( "%d byte writing... to file -> %s\n", n_write, MMAP_FILENAME );

	if ( *argv[1] == 's' )
	{
		flag_mmap = MAP_SHARED;
	}
	else
	{
		flag_mmap = MAP_PRIVATE;
	}

	p_map = mmap( ( void *)0, MMAP_SIZE, PROT_READ | PROT_WRITE, flag_mmap, fd, 0 );

	if ( p_map == MAP_FAILED )
	{
		err_handling( "Fail : mmap()", errno );
		return -1;
	}

	while( 1 )
	{
		printf( "'*' printf current mmap otherwise input text to mmap. >> " );

		if ( fgets( a_input, sizeof( a_input ), stdin ) == NULL )
		{
			err_handling( "Fail : fgets()", errno );
			return -1;
		}

		a_input[strlen( a_input )] = 0;

		if ( a_input[0] == '*' )
		{
			printf( "Current mmap -> '%.*s'\n", MMAP_SIZE, p_map );
		}
		else
		{
			memcpy( p_map, a_input, strlen( a_input ) );
		}
	
		if ( msync( p_map, MMAP_SIZE, MS_SYNC ) == -1 )
		{
			err_handling( "Fail : msync()", errno );
			return -1;
		}
	}

	return 0;
}

