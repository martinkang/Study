#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#define FIFOARG 1
#define FIFO_PERMS ( S_IRWXU | S_IWGRP | S_IWOTH )

int main( int argc, char *argv[] )
{
	int requestfd;

	if ( argc != 2 )
	{
		fprintf( stderr, "Usage : %s fifoname > logfile\n", argv[0] );
	}

	if ((mkfifo(argv[FIFOARG], FIFO_PERMS ) == -1 )
			&& ( errno != EEXIST ) )
	{
		perror(" SErver failed to create a FIFO" );
		return 1;
	}

	if ( ( requestfd = open(argv[FIFOARG], O_RDWR ) ) == -1 )
	{
		perror( " Server Failed to open its FIFO" );
	}

	copyfile( requestfd, STDOUT_FILENO );
	return 1;
}
