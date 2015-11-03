#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "sys/stat.h"

#define ERROR_CHAR 'e'
#define OK_CHAR 'g'
#define REQUEST_FIFO 1
#define REQ_PERMS ( S_IRUSR | S_IWGRP | S_IWOTH )
#define SEQUENCE_FIFO 2
#define SEQ_PERMS ( S_IRUSR | S_IWUSR | S_IROTH )

int main ( int argc, char *argv[] )
{
	char buf[1];
	int reqfd, seqfd;
	long seqnum = 1;

	if ( argc != 3 )
	{
		fprintf( stderr, "Usage %s requestfifo sequencefifo\n", argv[0] );
		return 1;
	}

	if ( ( mkfifo( argv[REQUEST_FIFO], REQ_PERMS ) == -1 ) &&
			( errno != EEXIST ) )
	{
		perror( " server failed to create request FIFO" );
		if ( unlink( argv[REQUEST_FIFO] ) == -1 )
		{
			perror( "server failed to unlink request FIFO" );
			return 1;
		}

	}

	if ( ( mkfifo( argv[SEQUENCE_FIFO], SEQ_PERMS ) == -1 ) &&
			( errno != EEXIST ) )
	{
		perror( " server failed to create sequence FIFO" );
		if ( unlink( argv[REQUEST_FIFO] ) == -1 )
		{
			perror( "server failed to unlink sequence FIFO" );
			return 1;
		}

	}


	if (( reqfd = open( argv[REQUEST_FIFO], O_RDONLY ) ) == -1 )
	{
		perror( "Server failed to open request of the FIFOs" );
		return 1;
	}

	if (( seqfd = open( argv[SEQUENCE_FIFO], O_RDWR )) == -1 )
	{
		perror( "Server failed to open sequence of the FIFOs" );
		return 1;
	}


	while( 1 )	
	{
		if ( read( reqfd, buf, 1 ) == 1 )
		{
			if ( ( buf[0] == OK_CHAR ) &&
				( write( seqfd, &seqnum, sizeof(seqnum ) ) == sizeof( seqnum ) ) )
			{
				seqnum++;
			}
			else if ( buf[0] == ERROR_CHAR )
			{
				break;
			}
		}
	}

	if ( unlink( argv[REQUEST_FIFO] ) == -1 )
	{
		perror( "Server failed to unlink request FIFO" );
	}

	if ( unlink( argv[SEQUENCE_FIFO] ) == -1 )
	{
		perror( "Server failed to unlink sequence FIFO" );
	}
	return 0;
}
