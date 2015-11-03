#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "jobs.h"

#define BUF_SIZE 1024
#define NUM_CMD 5 

char *gCmd[NUM_CMD] = { "stop", "show", "bg", "fg", "mykill" };

int makeArg( const char *aOrigText, const char *aDelimiter, char ***aArgv );

void executecmd( char *incmd );

int command( char *cmd, const int cmdNum, const int idx );

int main(void)
{
	int sRC = 0;
	int loop = 0;
	int i = 0;
	int len = 0;

	int sArgc = 0;
	char **sArgv = NULL;

	char buf[BUF_SIZE] = {0, };

	int cmdNum = 0;
	char *cmd;

	while ( 1 )
	{
		fputs( "prompt : " , stdout );
		if ( gets( buf ) == NULL )
			return -1;

		len = strlen( buf );
		if ( buf[len - 1] == '\n' )
			buf[len - 1] == 0;

		if ( strcmp( buf, "Q" ) == 0 || strcmp( buf, "q" ) == 0 )
		{
			break;
		}

		sArgc = makeArg( buf, " ", &sArgv );
	
		if ( sArgc == 1 )
		{
			cmd = sArgv[0];
			cmdNum = 0;
		}
		else if ( sArgc == 2 )
		{
			cmdNum = atoi( sArgv[1] );
		}
		else
		{
			fprintf( stderr, "Argument error\n" );
			continue;
		}

		for ( i = 0; i < NUM_CMD; i++ )
		{	
			if ( strcmp( cmd, gCmd[i] ) == 0  ) 
			{
				break;
			}		
		}

		sRC = command( cmd, cmdNum, i );

		cmdNum = 0;
	}

	finalize();

	return 0;
}

int command( char *cmd, const int cmdNum, const int idx )
{
	//"stop", "show", "bg", "fg", "mykill" 
	int sRC = 0;
	int status = 0;
	int pid = 0;

	switch ( idx )
	{
		case 0:
			sRC = kill( cmdNum, SIGSTOP );
			if ( sRC == -1 )
				perror( "process stop is error\n" );
			break;
		case 1:
			showjobs();
			break;
		case 2:
			if ( cmdNum == 0 )
			{
				fprintf( stderr, "Argument error\n" );
				sRC = -1;
				break;
			}
		
			pid = fork();
			if ( pid == 0 )
			{
				executecmd( cmd );

				pid = setsid();
			}
			else if ( pid > 0 )
			{
				addjob( pid );
			}
			else
			{
				fprintf( stderr, "fork error\n" );
			}
			break;
		case 3:
			break;
		case 4:
			sRC = kill( cmdNum, SIGKILL );
			if ( sRC == -1 )
				perror( "process kill is error" );
			deljob( cmdNum );
			waitpid( cmdNum, &status, 0 );
			break;
		default:
			fprintf( stderr, "no such cmd\n" );
			break;
	}

	return sRC;
}
