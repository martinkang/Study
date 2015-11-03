#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

void *pt( void )
{
	char buf[255] = {0, };
	int sRC = 0;
	printf( "pthread\n" );
	sRC = read( STDIN_FILENO, buf, 255 );
	printf( "%d\n", sRC );
	return;
}

int nonblockread( void )
{
	int flags = fcntl( STDIN_FILENO , F_GETFL, 0);
	int sRC = 0;
	char buf[255] = {0, };
	
	fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

	sRC = read( STDIN_FILENO, buf, 255 );

	fcntl(STDIN_FILENO, F_SETFL, flags );

	return sRC;

}

fun( void *arg, void *msg )
{

	printf( "void : %d\n", sizeof( arg ) );
	printf( "void : %d %s\n", sizeof( msg), msg );
}

int main()
{
	double a = 0.0;
	char msg[20];
	int len = 0;

	snprintf( msg, 20, "client %d\0", 1 );
	len = strlen( msg );

	printf( "%s\n%d\n", msg, len );
//	fun( &a, (void*)"asdfaa" );
//	printf( "int :%d void : %d\n", sizeof( a), sizeof( void ) );
	return 0;
}
