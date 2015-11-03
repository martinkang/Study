#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main( int argc, char *argv[])
{
	int pid = 0;
	
	printf( "child pid : %d\nargv[0] : %s\nargv[1] : %s\n", getpid(), argv[0], argv[1]);

	
	return 0;

}
