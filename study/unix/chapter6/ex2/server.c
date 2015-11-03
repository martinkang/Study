#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "../../ide.h"

#define PATH_MAX 255
#define CLIENT_MAX 4

#define BUF_MAX 1024

#define REQ_PATH "request"
#define REL_PATH "release"

#define END_SIGN 'E'
#define REQ_PERM ( S_IRUSR | S_IWUSR | S_IWGRP | S_IWOTH ) 
#define REL_PERM ( S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH )
#define CLIENT_PERM ( S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH )

int main ( int argc, char *argv[] )
{
	int sRC = 0;
	int fd;
	int req_fd;
	int rel_fd;

	int read_size = 0;
	int write_size = 0;

	int step = 0;

	char buf[BUF_MAX] = {0, };
	char CL_BUF[BUF_MAX] = { 0, };

	if ( argc != 1 )
	{
		fprintf( stderr, "Argument Error\n" );
		return -1;
	}

	sRC = mkfifo( REQ_PATH, REQ_PERM ); // make reqest fifo
	if ( sRC == -1 && errno == EEXIST )
	{
		unlink( REQ_PATH );
		sRC = mkfifo( REQ_PATH, REQ_PERM );
	}
	else if ( sRC == -1 && errno == EEXIST )
	{
		perror( "make request fifo error" );
		return -1;
	}
	step = 1;

	sRC = mkfifo( REL_PATH, REL_PERM ); // make release fifo
	if ( sRC == -1 && errno == EEXIST )
	{
		unlink( REL_PATH );
		sRC = mkfifo( REL_PATH, REL_PERM ); // make release fifo
	}
	else if ( sRC == -1 && errno != EEXIST )
	{
		perror( "make release fifo error" );
		return -1;
	}
	step = 2;

	while ( 1 )
	{
		memset( (void*)buf, 0, sizeof( buf ) );
		memset( (void*)CL_BUF, 0, sizeof( CL_BUF ) );

		printf( "waiting for open req fifo\n" );
		req_fd = open( REQ_PATH, O_RDONLY );
		if ( req_fd == -1 )
		{
			perror( "open request fifo error" );
			IDE_TEST_RAISE( 1, FINALIZE );
		}
		step = 3;

	
		printf( "waiting for read request fifo\n" );
		read_size = read( req_fd, buf, BUF_MAX );
		if ( read_size == 0 )
		{
			fprintf( stderr,  "read msg error\n" );
			IDE_TEST_RAISE( 1, FINALIZE );
		}

		if ( buf[0] == END_SIGN )
		{
			printf( "receive end sign\n" );
			break;
		}
		strcpy( CL_BUF, buf );

		printf( "make CLIENT[%s]\n", CL_BUF );
		sRC = mkfifo( CL_BUF, CLIENT_PERM );
		if ( sRC == -1 && errno != EEXIST )
		{
			perror( "make client fifo error" );
			IDE_TEST_RAISE( 1, FINALIZE );
		}
		step = 4;

		printf ( "waiting for release fifo\n" );
		rel_fd = open( REL_PATH , O_WRONLY );
		if ( rel_fd == -1 )
		{
			perror( "open release fifo error" );
			IDE_TEST_RAISE( 1, FINALIZE );
		}
		step = 5;

		write_size = write( rel_fd, "R", 1 ); 

		printf( "waiting for client fifo\n" );
		fd = open( CL_BUF , O_RDONLY );
		if ( fd == -1 )
		{
			perror( "open release fifo error" );
			IDE_TEST_RAISE( 1, FINALIZE );
		}
		step = 6;

		printf( "waiting for read client fifo\n" );
		read_size = read( fd, buf, BUF_MAX );
		if ( read_size == 0 )
		{
			fprintf( stderr,  "read msg error" );
			IDE_TEST_RAISE( 1, FINALIZE );
		}

		printf( "MSG FROM CLIENT[%s]\nMSG : %s\n", CL_BUF, buf );

		step = 6;
		close( fd );

		step = 5;
		close( rel_fd );

		step = 4;
		unlink( CL_BUF );

		step = 3;
		close( req_fd );
		printf( "\n\n" );
	}

	step = 2;
	unlink( REL_PATH );

	step = 1;
	unlink( REQ_PATH );

	return 0;

	IDE_EXCEPTION( FINALIZE )
	{
		switch ( step )
		{
			case 6 :
				close(fd);
			case 5 :
				close( rel_fd );
			case 4 :
				unlink( CL_BUF );
			case 3 :
				close( req_fd );
			case 2 :
				unlink( REL_PATH );
			case 1 :
				unlink( REQ_PATH );
				break;
			default :
				break;
		}
	}

	EXCEPTION_END

	return -1;
}

