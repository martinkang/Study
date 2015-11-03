#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int x;
	int pid = 0;

	x  = 0;
	pid = fork();
	if ( pid == 0 )
		x = 1;
	else
		x = 2;
	printf( "\ni am process %ld and my x is %d\n", (long)getpid(), x );

	wait();

	return 0;
}
