#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>

#define MMAP_FILENAME "shared_mmap"
#define MMAP_SIZE	  64

char contents[] = "(*)What`s differents between Shared mmap and Private mmap? -end-";

int main( int argc, char **argv )
{
	int fd, flag_mmap, n_write;
	char *p_map, a_input[100];

	if ( argc != 2 )
	{
		printf( "Usage : %s < s / p > \n", argv[0] );
		return -1;
	}

	fd = open( MMAP_FILENAME, O_RDWR | O_CREAT, 0664 );

	if ( fd == -1 )
	{
		printf ( "Fail : open(): ( %ㅇ : %s ) \n", errno, strerror( errno ) );
		return -1;
	}

	if ( ( n_write = write( fd, contents, sizeof( contents ) ) ) == -1 )
	{
		printf( "Fail : write() : ( %d : %s ) \n", errno, strerror( errno ) );
		return -1;
	}

	printf( "%d byte writing... to file -> $s\n", n_write, MMAP_FILENAME );

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
		printf( "Fail : mmap() : ( %d : %s ) \n", errno, strerror( errno ) );
		return -1;
	}

}
