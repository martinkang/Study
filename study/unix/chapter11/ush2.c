#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PROMPT_STRING "ush2>>"
#define QUIT_STRING "q"

#define BUF_SIZE 1024
#define JMP 10

static sigjmp_buf jumptoprompt;
static volatile sig_atomic_t okaytojump = 0;

void sigint( int signo )
{
	if ( okaytojump == 1 )
	{
		okaytojump = 0;
		siglongjmp( jumptoprompt, JMP );
	}
	else
	{
		return;
	}
}

void executecmd(char *incmd);

int main ( void )
{
	int pid = 0;
	int sRC = 0;
	int len = 0;
	char inbuf[MAX_CANON] = {0, };
	char *sRCC = NULL;

	struct sigaction act;

	sigset_t newSet;
	sigset_t oldSet;

	sRC = sigfillset( &newSet );
	if ( sRC == -1 )
		return -1;

	sRC = sigdelset( &newSet, SIGINT );
	if ( sRC == -1 )
		return -1;

	sRC = sigprocmask( SIG_SETMASK, &newSet, &oldSet );
	if ( sRC == - 1 )
		return -1;

	act.sa_flags = 0;
	act.sa_handler = sigint;
	sigemptyset( &act.sa_mask );
	sRC = sigaction( SIGINT, &act, NULL );
	if ( sRC == -1 )
		return -1;

	if ( sRC == -1 )
	{
		perror( "signal setup is error" );
		return -1;
	}

	while ( 1 )
	{
		sRC = sigsetjmp( jumptoprompt, 1 );
		if ( sRC == JMP ) 
		{
			fputs( "\nJummped\n", stdout );
			wait(NULL);
			continue;
		}

		wait( NULL );

		okaytojump = 1;

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
			sRC = sigprocmask( SIG_SETMASK, &oldSet, NULL );
			if ( sRC == -1 )
				return 1;
			
			executecmd(inbuf);
			return 1;
		}
		else
		{
			wait( NULL );
		}
	}

	sigprocmask( SIG_SETMASK, &oldSet, NULL );

	return 0;
}
