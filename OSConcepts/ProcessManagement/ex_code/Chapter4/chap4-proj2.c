#include <stdio.h>
#include <pthread.h>
#include <errno.h>

#define M 3
#define K 2
#define N 3

#define ERR_TEST( a, b ) \
	if ( a ) \
	{		\
		perror( b ); \
		return -1;	\
	}

int A[M][K] = { { 1, 4 }, { 2, 5 }, { 3, 6 } };
int B[K][N] = { { 8, 7, 6 }, { 5, 4, 3 } };
int C[M][N] = { { 0, } };

/* 한 스레드당 한 로우만 계산한다. */
typedef struct v
{
	int i; /* row */	
} thr_data;

void worker( void *arg )
{
	int i = 0;
	int j = 0;
	int k = 0;
	int sIdx = 0;
	
	thr_data *v = (thr_data*)arg;

	sIdx = v->i;

	for ( i = 0; i < N; i++ )  
	{
		for ( j = 0; j < K; j++ ) 
		{
			C[sIdx][i] += A[sIdx][j] * B[j][i];
		}
	}

}

int main( void )
{
	int i = 0;
	int j = 0;
	int sRC = 0;

	int sJoinNum = 0;

	thr_data v[M];
	pthread_t pid[M];

	/* Create Thread */
	for ( i = 0; i < M; i ++ )
	{
		v[i].i = i;
		sRC = pthread_create( &pid[i], NULL, worker, (void*)( &v[i] ) );
		ERR_TEST( sRC== -1, "pthread_create() error" );
	}

	/* Join Thread */
	while( sJoinNum < M )
	{
		for ( i = 0; i < M; i++ )
		{
			sRC = pthread_join( pid[i], NULL );
			if ( sRC == 0 )
				sJoinNum++;
		}
	}

	/* printf Matrix C */
	for ( i = 0; i < M; i++ )
	{
		for ( j = 0; j < N; j++ )
		{
			printf( "%d ", C[i][j] );
		}
		printf("\n" );
	}
	printf( "\n" );

	return 0;
}
