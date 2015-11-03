#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PROMPT_STRING "ush2>>"
#define QUIT_STRING "q"

#define BUF_SIZE 1024
void sigint( int signo )
{
	char buf[1024] = " SIGINT blocked!!\n";
	if ( signo == SIGINT )
	{
		write( STDOUT_FILENO, buf, strlen( buf ) );
	}
}

int signalsetup( int signo )
{
	struct sigaction act;
	int sRC = 0;

	act.sa_flags = 0;
	act.sa_handler = sigint; 
	sigemptyset( &act.sa_mask );

	sRC = sigaction( signo, &act, NULL );
	return sRC;
}

void executecmd(char *incmd);

int main ( void )
{
	int pid = 0;
	int sRC = 0;
	int len = 0;
	char inbuf[MAX_CANON] = {0, };
	char *sRCC = NULL;

	sRC = signalsetup( SIGINT );
	if ( sRC == -1 )
	{
		perror( "signal setup is error" );
		return -1;
	}

	while ( 1 )
	{
		sRC = fputs( PROMPT_STRING, stdout );
		if ( sRC == EOF )
			continue;

		sRCC = fgets( inbuf, MAX_CANON, stdin );
		if ( sRCC == NULL )
			continue;

		len = strlen( inbuf );
		if ( inbuf[len - 1] == '\n' )
			inbuf[len - 1] = 0;

		if ( strcmp( inbuf, QUIT_STRING ) == 0 )
			break;
	
		pid = fork();
		if ( pid < 0 )
		{
			perror( " failed to fork child" );
		}
		else if ( pid == 0 )
		{
			executecmd(inbuf);
			return 1;
		}
		else
		{
			wait( NULL );
		}
	}

	return 0;
}
