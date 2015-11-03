#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "../../ide.h"

#define REl_CHAR 'l'
#define ERR_CHAR 'e'

#define PATH_MAX 255

int main( int argc, char *argv[] )
{
	int sRC = 0;
	int req_fd = 0;
	int rel_fd = 0;
	int step = 0;
	int w_size = 0;
	int r_size = 0;

	char req_char[1] = "r";
	char err_char[1] = "e";
	char rel_char[1] = {0, };

	char sPath_req[PATH_MAX] = {0, };
	char sPath_rel[PATH_MAX] = {0, };

	sRC = snprintf( sPath_req, PATH_MAX, "%s.%s", 
			argv[1], "request" );
	req_fd = open( sPath_req, O_WRONLY );
	if ( req_fd < 0 )
	{
		perror( "request fifo open error " );
		IDE_TEST( 1 );
	}
	step = 1;

	sRC = snprintf( sPath_rel, PATH_MAX, "%s.%s", 
			argv[1], "release" );
	rel_fd = open( sPath_rel, O_RDONLY );
	if ( req_fd < 0 )
	{
		perror( "release fifo open error " );
		IDE_TEST_RAISE( 1, FINALIZE );
	}
	step = 2;

	w_size = write( req_fd, req_char, 1 );
	if ( w_size != 1 )
	{
		write( req_fd, err_char, 1 );
	}

	read( rel_fd, rel_char, 1 );
	if ( rel_char[0] == ERR_CHAR )
	{
		IDE_TEST_RAISE( 1, FINALIZE );
	}

	printf( "[%ld] receive Release CHAR\n", (long)getpid() );

	close ( req_fd );
	step = 1;

	close ( rel_fd );
	step = 0;

	return 0;

	IDE_EXCEPTION( FINALIZE )
	{
		switch( step )
		{
			case 2:
				close ( rel_fd );
			case 1:
				close ( req_fd );
				break;
			default :
				break;
		}
	}

	EXCEPTION_END
	
	return -1;
}
