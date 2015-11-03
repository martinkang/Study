#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "../../ide.h"

#define PATH_MAX 255

#define REQ_CHAR 'r'
#define ERR_CHAR 'e'

#define REQUEST_PERM ( S_IRUSR | S_IWUSR | S_IWGRP | S_IWOTH ) 
#define RELEASE_PERM ( S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH )

int makeFIFO( char *aB_name, char *aType, char **aPath )
{
	int sRC = 0;
	int slen = 0;
	char *sPath = NULL;

	slen = strlen( aB_name ) + strlen( "request" ) + 1;

	sPath = (char*)malloc( sizeof(char) * slen );
	sRC = snprintf( sPath, PATH_MAX, "%s.%s", aB_name, aType );
	if ( sRC < 0 )
	{
		fprintf( stderr, "%s barrier name generate error", aType );
		return -1;
	}

	if ( strcmp( aType, "request" ) == 0 )
	{
		sRC = mkfifo( sPath, REQUEST_PERM );
	}
	else if ( strcmp( aType, "release" ) == 0 )
	{
		sRC = mkfifo( sPath, RELEASE_PERM );
	}
	else
	{
		free ( sPath );
		return -1;
	}

	if ( sRC == -1 && errno != EEXIST )
	{
		free ( sPath );
		return -1;
	}

	*aPath = sPath;
}


int main( int argc, char *argv[] )
{
	int sRC = 0;
	int i = 0 ;
	int b_size = 0;
	int read_size = 0;
	int write_size = 0;
	int req_fd = 0;
	int rel_fd = 0;
	int step = 0;
	int sE_check = 0;

	char sRel_ch[1] = "l";
	char sErr_ch[1] = "e";
	char sBuf[1];
	char *b_name;
	char *b_request;
	char *b_release;

	if ( argc != 3 )
	{
		fprintf( stderr, "Argument Error\n" );
		return -1;
	}

	b_size = atoi( argv[2] );
	b_name = argv[1];

	sRC = makeFIFO( b_name, "request", &b_request );
	if ( sRC == -1 )
	{
		perror( "Make REQUEST FIFO ERROR" );
		IDE_TEST_RAISE( 1, FINALIZE );
	}
	step = 1;

	sRC = makeFIFO( b_name, "release", &b_release );
	if ( sRC == -1 )
	{
		perror( "Make RELEASE FIFO ERROR" );
		IDE_TEST_RAISE( 1, FINALIZE );
	}
	step = 2;

	printf( "request fd is opening.....\n" );
	req_fd = open( b_request, O_RDONLY );
	if ( req_fd == -1 )
	{
		perror( "request fifo open error" );
		IDE_TEST_RAISE( 1, FINALIZE );
	}
	step = 3;
	
	printf( "release fd is opening.....\n" );
	rel_fd = open( b_release, O_WRONLY );
	if ( rel_fd == -1 )
	{	
		perror( "release fifo open error" );
		IDE_TEST_RAISE( 1, FINALIZE );
	}
	step = 4;

	printf( "wait for request char\n" );
	for ( i = 0; i < b_size; i++ )
	{
		read_size = read( req_fd, sBuf, 1 );

		printf( "receive %d th char %s\n", i + 1, sBuf );

		if ( sBuf[0] == ERR_CHAR )
		{
			sE_check = 1;
			fprintf( stderr, "Error char received\n" );
		}
	}

	if ( sE_check == 1 )
		{
			for ( i = 0; i < b_size; i++ );
			{
				write( rel_fd, sErr_ch, 1 );
			}
			fprintf( stderr, "Error char send\n" );
	
			IDE_TEST_RAISE( 1, FINALIZE );
		}

	printf( "send release char\n" );
	for ( i = 0; i < b_size; i++ )
	{
		while ( ( write_size = write( rel_fd, sRel_ch, 1 ) ) != 1 );
	}

	close ( req_fd );
	step = 3;

	close ( rel_fd );
	step = 2;

	unlink ( b_release );
	step = 1;

	unlink ( b_request );

	return 0;

	IDE_EXCEPTION ( FINALIZE )
	{
		switch ( step )
		{
			case 4:
				close( rel_fd );
			case 3:
				close ( req_fd );
			case 2:
				unlink( b_release );
			case 1:
				unlink( b_request );
				break;
			default:
				break;
		}
	}

	EXCEPTION_END

	return -1;
}
