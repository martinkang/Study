#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char *argv[] )
{
	pid_t child_id = 0;
	int i, n;
	

	if ( argc != 2 )
	{
		fprintf( stdout, "Usage ; %s processes\n", argv[0] );
		return -1;
	}

	n = atoi(argv[1]);

	for( i = 1; i < n; i++ )
	{
		child_id = fork();
		if ( child_id != 0 )
			break;
	}

	fprintf( stdout, " i:%d process ID :%ld parent ID:%ld child ID : %ld\n",
			i, (long)getpid(), (long)getppid(), (long)child_id );
	return 0;
}
