#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_STR_NAME 255
#define MAX_BUF 1024
#define READLINE 100

#define ERR_TEST( a ) \
	if ( a )		  \
	return -1

int getFile( char * aFileName, 
			 int    aFlag, 
			 int    aMode )
{
	int fd = -1;
		
	if ( aFileName[0] == '\n' || aFileName[0] == '\0' )
	{
		printf( "source name error\n" );
	}
	else
	{
		fd = open( aFileName, aFlag, aMode );
		if ( fd == -1 )
		{
			perror( "open() error" );
		}
	}

	return fd;
}

int sourceCopyToDest( int aSrc, int aDest )
{
	ssize_t sReadSize = 0;
	ssize_t sWriteSize = 0;
	char sBuf[MAX_BUF] = { 0, };

	sReadSize = read( aSrc, sBuf, READLINE );
	while ( sReadSize > 0 )
	{
		sWriteSize = write( aDest, sBuf, sReadSize );
		if ( sWriteSize != sReadSize )
		{
			perror( "write Error" );
			return -1;
		}
	
		sReadSize = read( aSrc, sBuf, READLINE );
	}

	return sReadSize;
}

int main( void )
{
	int sSrcFd = 0;
	int sDestFd = 0;

	char sSrcName[MAX_STR_NAME] = { 0, };
	char sDestName[MAX_STR_NAME] = { 0, };

	printf( "source file : " );
	gets( sSrcName );
	sSrcFd = getFile( sSrcName, 
					  O_RDONLY, 
					  00400 );
	ERR_TEST( sSrcFd == -1 );
	
	printf( "dest file : " );
	gets( sDestName );
	sDestFd = getFile( sDestName, 
					   O_CREAT | O_TRUNC | O_RDWR , 
					   00777 );
	ERR_TEST( sDestFd == -1 );

	ERR_TEST( sourceCopyToDest( sSrcFd, sDestFd ) == - 1 );
	
	return 0;
}
