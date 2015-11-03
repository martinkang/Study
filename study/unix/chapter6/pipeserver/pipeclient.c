#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

#define FIFOARG 1
#define BUFSIZE 1024

int main( int argc, char *argv[] )
{
	time_t curtime;
	int len;
	char buf[BUFSIZE] = {0, };
	char msg[BUFSIZE] = {0, };
	char requestbuf[PIPE_BUF];
	int requestfd;

	if ( argc != 2)
	{
		fprintf( stderr, "Usage : %s fifoname", argv[0] );
		return 1;
	}

	if ( ( requestfd = open( argv[FIFOARG], O_WRONLY ) ) == -1 )
	{
		perror( "Client faile to open log fifo for write" );
		return 1;
	}

	curtime = time(NULL);
	snprintf( requestbuf, PIPE_BUF -1, "%d : %s", (int)getpid(), ctime(&curtime) );
	len = strlen(requestbuf);

	if ( write( requestfd, requestbuf, len ) != len )
	{
		perror( "client faile to write" );
		return 1;
	}

	while ( fgets( buf, BUFSIZE - 1, stdin  ) != NULL )
	{

		snprintf( msg, BUFSIZE - 1, "%d : %s", (int)getpid(), buf );
		len = strlen(msg);
		if ( write( requestfd, msg, len ) != len )
		{
			perror( "client faile to write" );
			return 1;
		}

		if ( strcmp( buf, "./exit\n" ) == 0 )
			break;
	}

	close(requestfd);
	return 0;
}
