#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void test( int signo )
{
	static int a = 0;
	printf( "asdf\n" );
	write( STDERR_FILENO, &a, sizeof(a) );
	a++;
	sleep(2);
}
int main()
{
	struct sigaction act;
	union sigval sval;


	act.sa_flags = SA_SIGINFO;
	act.sa_handler = test;
	sigemptyset( &act.sa_mask );
	sigaction( SIGUSR1, &act, NULL );
	sigaction( SIGUSR2, &act, NULL );

	sigqueue( getpid(), SIGUSR1, sval  );

	fprintf( stderr, "pid : %d\n", getpid() );
	for( ; ; ){}
	return 0;
}
