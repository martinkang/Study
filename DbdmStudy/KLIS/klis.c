#include <stdio.h>                                                 
#include <stdlib.h>

#define MAX_DATA 500
#define MAX_NUM 100000

#include <stdio.h>                                                 
#include <stdlib.h>

#define MAX_DATA 500
#define MAX_NUM 100000

int sort( int max, const int *data, int *array )
{
	int n = max - 1;
	int i = 0;
	int j = 0;
	int temp = 0;
	int num = data[array[n]];

	for ( i = 0; i < n; i++ )
	{
		if ( num == data[array[i]] )
		{
			return -1;
		}
		else if ( num < data[array[i]] )
		{
			temp = array[n];

			for ( j = i + 1; j < n; j++ )
				array[i] = array[i + 1];

			array[i] = temp;
			break;
		}
	}

	return 0;
}

int *traceRoute( const int kTh,
				 const int max,
				 const int maxIdxNum,
				 const int *maxIdx,
				 const int *routeNum, 
				 const int *routeSum, 
				 const int *data, 
				 int (*route)[MAX_DATA] )
{
	int sExit = 0;
	int n = max;

	int i = 0;
	int j = 0;
	int temp = kTh;
	int idx = 0;

	int *result = NULL;

	while( sExit == 0 )
	{
		for ( i = 0; i < maxIdxNum; i++ )
		{
			if ( temp > routeSum[maxIdx[i]])
			{
				temp = temp - routeSum[maxIdx[i]];
			}
			else if ( temp <= routeSum[maxIdx[i]] )
			{
				idx = maxIdx[i];
				sExit = 1;
				break;
			}
		}
	}

	result = (int*)malloc( sizeof(int) * max );

	for ( i = 0; i < n; i++ )
	{
		result[i] = data[idx];

		if ( ( routeSum[idx] == 1 ) || ( temp <= 1 ) )
		{
			idx = route[idx][0];
		}
		else 
		{
			for( j = 0; j < routeNum[idx]; j++ )
			{
				if ( temp > routeSum[route[idx][j]])
				{
					temp = temp - routeSum[route[idx][j]];
				}
				else if ( temp <= routeSum[route[idx][j]] )
				{
					idx = route[idx][j];
					break;
				}

			}
		}
	}

	return result;
}

int *LIS( const int kTh,
		  const int n, 
	  	  const int *data,
		  int *numResult ) 
{
	int i = 0;
	int j = 0;


