#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

pthread_mutex_t gMtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *fun1( void *a )
{
	while( write( 1, "in fun1 aaaaaaabbbbbbb\n", 30 ) == -1 && errno == EINTR );

	pthread_mutex_lock( &gMtx );

	pthread_cond_wait ( &cond, &gMtx );
	while( write( 1, "in fun1 catch signal\n", 30 ) == -1 && errno == EINTR );

	pthread_mutex_unlock( &gMtx );

}

void *fun3( void *a )
{
	while( write( 1, "in fun3 aaaaaaabbbbbbb\n", 30 ) == -1 && errno == EINTR );

	pthread_mutex_lock( &gMtx );

	pthread_cond_wait ( &cond, &gMtx );
	while( write( 1, "in fun3 catch signal\n", 30 ) == -1 && errno == EINTR );

	pthread_mutex_unlock( &gMtx );

}


void *fun2( void *a )
{
	while( write( 1, "in fun2 aaaaaaabbbbbbb\n", 30 ) == -1 && errno == EINTR );

	pthread_mutex_lock( &gMtx );

	pthread_cond_signal ( &cond );
	while( write( 1, "in fun2 send signal\n", 30 ) == -1 && errno == EINTR );
	sleep( 1 );

	pthread_mutex_unlock( &gMtx );

}

int main( void )
{
	pthread_t t1;
	pthread_t t2;
	pthread_t t3;

	pthread_create( &t1, NULL, fun1, NULL );

	pthread_create( &t3, NULL, fun3, NULL );
	sleep (1);
	pthread_create( &t2, NULL, fun2, NULL );

	pthread_join( t1, NULL );	
	while( write( 1, "t1 is end\n", 20 ) == -1 && errno == EINTR );

	pthread_join( t2, NULL );
	
	while( write( 1, "t2 is end\n", 20 ) == -1 && errno == EINTR );

	pthread_join( t3, NULL );

	while( write( 1, "t3 is end\n", 20 ) == -1 && errno == EINTR );

	return 0;
}
