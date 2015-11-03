#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

static void myhandler( int s )
{
	char aStar = '*';
	int errsave;
	errsave = errno;
	write( STDERR_FILENO, &aStar, 1 );
	errno = errsave;
}

static int setupinterrupt(void)
{
	int sRC = 0;
	struct sigaction act;
	
	act.sa_handler = myhandler;
	act.sa_flags = 0;

	sRC = sigemptyset( &act.sa_mask );
	if ( sRC == -1 )
		return sRC;

	sRC = sigaction( SIGPROF, &act, NULL );
	return sRC;
}

static int setupitimer(void)
{
	struct itimerval value;
	value.it_interval.tv_sec = 2;
	value.it_interval.tv_usec = 0;
	value.it_value = value.it_interval;
	return (setitimer( ITIMER_PROF, &value, NULL ) );
}

int main( void )
{
	int sRC = 0;

	if ( setupinterrupt() == -1 )
	{
		perror( "fail to setup interrupt" );
		return -1;
	}
	if ( setupitimer() == -1 )
	{
		perror( "failed to setup timer" );
		return -1;
	}

	for( ; ; );
}
