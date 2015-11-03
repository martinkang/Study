#include <stdio.h>
#include <unistd.h>

int main( int argc, char *argv[] )
{
	if ( argc != 2 )
		return 1;

	fprintf( stderr, "%s pid : %d ppid : %d gid : %d sid : %d\n", argv[1], getpid(), getppid(), getgid(), getsid(0) );

	return 0;
}
