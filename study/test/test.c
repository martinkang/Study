#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	int pid = 0;
	char *sargv[2];

	sargv[0] = "asdf\0";
	sargv[1] = "hi\0";

	printf( "pid : %d\n", getpid() );
	pid = fork();
	if ( pid == 0 )
	{
		execv( "child", sargv );
		return 0;
	}
	else if ( pid > 0 )
		wait(NULL );

	
	return 0;

}
