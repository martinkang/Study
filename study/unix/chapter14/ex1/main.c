#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "license.h"

int main( int argc, char *argv[] )
{
	int sRC = 0;
	int i = 0;

	int pid = 0;
	int pr_limit = 0;
	int pr_count = 0;

	int licenseNum = 0;

	if ( argc != 2 )
	{
		fprintf( stderr, "argument error\n" );
		return -1;
	}

	sem_unlink( "license" );

	pr_limit = atoi( argv[1] );

	sRC = initlicense();
	if ( sRC != 0 )
	{
		perror( "init license error" );
		return -1;
	}

	licenseNum = getLicenseNum();
	fprintf( stderr, "Num of License : %d\n", licenseNum );

	for ( i = 0; i < pr_limit; i++ )
	{
		pid = fork();
		if ( pid == 0 )
		{
			execv( "child", NULL );
			break;
		}
		else if ( pid < 0 )
		{
			fprintf( stderr, "fork error" );
			return -1;
		}
		fprintf( stderr, "process %d is created\n", pid );
	}

	fprintf( stderr, "waiting\n" );
	for ( i = 0; i < pr_limit; i++ )
	{
		wait( NULL );
	}
	fprintf( stderr, "wait is completed\n" );

	sRC = finallicense();

	return sRC;
}
