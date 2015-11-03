#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../../ide.h"

#define REQ_PATH "request"
#define REL_PATH "release"

#define OFFSET '0'

#define BUF_SIZE 1024

#define REL_SIGN 'R'
#define END_SIGN 'E'

char *itoa( int aID )
{
	int sRC = 0;
	int dev = 1;
	int n = 0;
	int i = 0;
	int sID = aID;
	int moc = 0;

	char *pid = NULL;

	while ( sRC == 0 )
	{
		if ( sID / dev > 1 )
		{
			dev = dev * 10;
			n++;
		}
		else
		{
			break;
		}
	}

	if ( n == 0 )
		return NULL;

	dev = dev / 10;

	pid = (char*)malloc( sizeof(char) * ( n + 1 ) );

	for ( i = 0; i < n; i++ )
	{
		moc = sID / dev;
		pid[i] = moc + OFFSET ;

		sID = sID - ( moc * dev );
		dev = dev / 10;
	}

	pid[i] = '\0';

	return pid;
}

int main ( int argc, char *argv[] )
{
	int sRC = 0;
	int step = 0;

	int req_fd = 0;
	int rel_fd = 0;
	int pro_fd = 0;

	int write_size = 0;
	int read_size = 0;

	int sign = 0;

	char offset = '0';
	char of1 = '1';

	char *pid = NULL;
	char buf[BUF_SIZE] = {0, };

	pid = itoa( getpid() );

	if ( argc == 2 )
	{
		sign = 1;
	}
	
	if ( pid == NULL )
	{
		fprintf( stderr, "getpid error\n" );
	}

	printf( "getpid : %d\nitoa : %s\n", getpid(), pid );

	printf( "open request fifo\n" );
	req_fd = open( REQ_PATH, O_WRONLY );
	if ( req_fd == -1 )
	{
		perror( "open request fifo error" );
		IDE_TEST( 1 );
	}
	step = 1;
	
	if ( sign == 1 )
	{
		printf( "send end sign to server\n" );
		write_size = write( req_fd, "E", strlen( "E" ) );
		IDE_TEST( 1 );
	}
	else
	{
		printf( "write request fifo\n" );
		write_size = write( req_fd, pid, strlen(pid) );
	}

	step = 2;
	close( req_fd );

	printf( "open release fifo\n" );
	rel_fd = open( REL_PATH, O_RDONLY );
	if ( rel_fd == -1 )
	{
		perror( "open release fifo error" );
		IDE_TEST( 1 );
	}
	step = 3;

	printf( "read release fifo\n" );
	read_size = read( rel_fd, buf, BUF_SIZE ); 
	if ( read_size == 0 )
	{
		perror( "read error" );
		IDE_TEST( 1 );
	}

	if ( buf[0] != REL_SIGN )
	{
		fprintf( stderr, "wrong datd received\n" );
		IDE_TEST( 1 );
	}

	printf( "open client fifo\n" );
	pro_fd = open( pid, O_RDWR );
	if ( pro_fd == -1 )
	{
		perror( "open process fifo error" );
		IDE_TEST( 1 );
	}
	step = 4;	
	
	printf( "write success msg to server\n" );
	
	write_size = write( pro_fd, "SUCCESS", strlen( "SUCCESS" ) );

	step = 4;
	close ( pro_fd );
	
	step = 3;
	close ( rel_fd );

	step = 1;
	close ( req_fd );

	return 0;

	EXCEPTION_END

	switch ( step )
	{
		case 4:
			close( pro_fd );
		case 3:
			close( rel_fd );
		case 1:
			close( req_fd );
			break;
		default :
			break;
	}

	return -1;
}
