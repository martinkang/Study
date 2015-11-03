#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include "manager.h"
#include "consumer.h"
#include "producer.h"
#include "../../ide.h"

#define PRODUCER_NUM 1
#define CONSUMER_NUM 4

static sig_atomic_t gDoneFlag = 0;

static void setdone( void )
{
	gDoneFlag = 1;
}

int main( void )
{
	int sRC = 0;
	
	int signo = SIGUSR1;
	int signum = 0;
	sigset_t s_set;

	pthread_t mgr;
	
	sRC = sigemptyset( &s_set );
	IDE_TEST( sRC );
	sRC = sigaddset( &s_set, signo );
	IDE_TEST( sRC );

	sRC = sigprocmask( SIG_BLOCK, &s_set, NULL );
	IDE_TEST( sRC );
	
	sRC = pthread_create( &mgr, NULL, (void*)T_manager, NULL );
	IDE_TEST( sRC );
	fprintf( stdout, "Manager Thread is created\n" );

	sRC = pthread_join( mgr, NULL );
	fprintf( stdout, "Manager Thread is destoryed\n" );
	return 0;

	EXCEPTION_END

	fprintf( stderr, "asdf\n" );

	return -1;
}

void T_manager(void )
{
	int sRC = 0;
	int i = 0;
	int step = 0;
	
	int signo = SIGUSR1;
	int signum = 0;
	sigset_t s_set;
	struct sigaction act;

	pthread_t p_thread[PRODUCER_NUM];
	pthread_t c_thread[CONSUMER_NUM];
	
	for( i = 0; i < PRODUCER_NUM; i++ )
	{
		sRC = pthread_create( &p_thread[i], NULL, (void*)P_manager, NULL );
		if ( sRC == -1 )
			fprintf( stdout, "producer thread[%d] create error\n", i );
	}
	step = 2;

	for( i = 0; i < CONSUMER_NUM; i++ )
	{
		sRC = pthread_create ( &c_thread[i], NULL, (void*)C_manager, NULL );
		if ( sRC == -1 )
			fprintf( stdout, "comsumer thread[%d] create error\n", i );
	}
	step = 3;

	sRC = sigemptyset( &s_set );
	IDE_TEST( sRC );
	sRC = sigaddset( &s_set, signo );
	IDE_TEST( sRC );

	fprintf( stdout, "wait until sigusr1 is received\n" );
	sRC = sigwait( &s_set, &signum ); 
	fprintf( stdout, "sigusr1 is received\n" );
	setdone();

	step = 2;
	for( i = 0; i < CONSUMER_NUM; i++ )
	{
		sRC = pthread_join( c_thread[i], NULL );
		if ( sRC == -1 )
			fprintf( stdout, "comsumer thread[%d] join error\n", i );
	}
	fprintf( stdout, "consumer join end\n" );

	step = 1;
	for( i = 0; i < PRODUCER_NUM; i++ )
	{
		sRC = pthread_join( p_thread[i], NULL );
		if ( sRC == -1 )
			fprintf( stdout, "producer thread[%d] join error\n", i );
	}
	fprintf( stdout, "producer join end\n" )
			;
	step = 0;

	fprintf( stdout, "manager thread is ended\n" );
	
	return;

	EXCEPTION_END

	switch ( step )
	{
		case 3:
			for( i = 0; i < CONSUMER_NUM; i++ )
			{
				sRC = pthread_join( c_thread[i], NULL );
				if ( sRC == -1 )
					fprintf( stdout, "comsumer thread[%d] join error\n", i );
			}
		case 2:
			for( i = 0; i < PRODUCER_NUM; i++ )
			{
				sRC = pthread_join( p_thread[i], NULL );
				if ( sRC == -1 )
					fprintf( stdout, "producer thread[%d] join error\n", i );
			}
		case 1:
			break;
		default :
			break;
	}
}


void getdone( int *flag )
{
	*flag = gDoneFlag;
}
