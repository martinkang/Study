#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/stat.h>

#define ERROR_CHAR 'e'
#define OK_CHAR 'g'
#define REPEAT 100
#define REQUEST_FIFO 1
#define SEQUENCE_FIFO 2
#define SLEEP_MAX 5

int main( int argc, char *argv[] )
{
	int i;
	char reqbuf[1];
	int reqfd, seqfd;
	long seqnum;

	if ( argc != 3 )
	{
		fprintf( stderr, "Usage : %s\n", argv[0] );
	}

	if ( ( ( reqfd = open( argv[REQUEST_FIFO], O_WRONLY ) ) == -1 ||
				(( seqfd = open( argv[SEQUENCE_FIFO], O_RDONLY ) ) == -1 )))
	{
		perror( "client failed to open FIFO" );
		return 1;
	}

	for ( i = 0; i < REPEAT; i++ )
	{
		reqbuf[0] = OK_CHAR;
		sleep((int)(SLEEP_MAX * drand48() ) );

		if ( write(reqfd, reqbuf, 1 ) == -1 )
		{
			perror( " client failed to write request" );
			break;
		}

		if ( read(seqfd, &seqnum, sizeof(seqnum)) != sizeof(seqnum ) )
		{
			fprintf( stderr, "client failed to read full sequence number\n" );
			write( reqfd, reqbuf, 1 );
			break;
		}

		fprintf( stderr, "[%ld]: received sequence number %ld\n", (long)getpid(), seqnum );

	}

	return 0;
}

