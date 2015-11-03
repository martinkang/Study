#include <stdio.h>
#include <string.h>
#include <fcntl.h>

int main( int argc, char *argv[] )
{
	int len = 0;
	int fd = 0;
	int sRC = 0;
	len = strlen(argv[1] );
	
	sRC = mkfifo ( "asdf", S_IRUSR | S_IWUSR );
	printf ( "mkfifo : %d\n", sRC );

	fd = open( "asdf", O_RDWR );
	printf ( "fd : %d\n", fd );


	close( fd );
	unlink( "asdf" );

	printf( "%s len : %d \n",argv[1], len );
}
