#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

#define M 3
#define K 2
#define N 3

int A[M][K] = { {1,4}, {2,5}, {3,6} };
int B[K][N] = { {8,7,6}, {5,4,3} };
int C[M][N];

void work( void *arg )
{
	int m = ( (int*)arg )[0];
	int n = ( (int*)arg )[1];
	int k = 0;
	int sum = 0;

	for ( k = 0; k < K; k++ )
	{
		sum += A[m][k] * B[k][n];
	}
	C[m][n] = sum;
}

int main( void )
{
	int idx[2] = {0, 0 };
	int i = 0;
	int j = 0;
	int sRC = 0;
	pthread_t worker[M * N];

	for ( i = 0; i < M; i++ )
	{
		for ( j = 0; j < N; j++ )
		{
			idx[0] = i;
			idx[1] = j; 
			sRC = pthread_create( &( worker[ ( i * M ) + j] ), NULL, (void*)work, (void*)idx );
			if ( sRC < 0 )
			{
				perror( "pthread create error" );
				return -1;
			}
		}
	}

	for ( i = 0; i < M * N; i++ )
	{
		sRC = pthread_join( worker[i], NULL );
		if ( sRC < 0 )
			perror( "pthread join error" );
	}

	for ( i = 0; i < M; i ++ )
	{
		for ( j = 0; j < N; j ++ )
		{
			printf( "%d ", C[i][j] );
		}
		printf( "\n" );
	}
	
	return 0;
}
