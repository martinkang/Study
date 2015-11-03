#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <async.h>

#define BLKSIZE 1024
#define MODE ( S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH )

int main( int argc, char *argv[] )
{
	int sRC = 0;
	int fdin = 0;
	int fdout = 0;
	int done = 0;

	char buf[BLKSIZE] = {0, };

	if ( argc != 3 )
	{
		fprintf( stderr, "Argument error\n" );
		return -1;
	}

	fdin = open( argv[1], O_RDONLY );
	if ( fdin == -1 )
	{
		perror( "Input File open error" );
		return -1;
	}

	fdout = open( argv[1], MODE );
	if ( fdout == -1 )
	{
		perror( "Output File open error" );
		return -1;
	}

	sRC = initsignal(SIGRTMAX);
	if ( sRC == -1 )
	{
		perror( "Failed to initialize signal" );
		return -1;
	}

	sRC = initread( fdin, fdout, SIGRTMAX, buf, BLKSIZE );
	if ( sRC == -1 )
	{
		perror( "Failed to initiread read" );
		return -1;
	}

	while ( 1 )
	{
		dowork();
		if ( done == 0 )
		{
			done = getdone();
			if ( done == 1 )
			{
			}
		}
	}

}
