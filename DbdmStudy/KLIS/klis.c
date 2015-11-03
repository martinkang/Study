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

	int max = 0;
	int maxIdx[MAX_DATA];
	int maxIdxNum = 0;

	int route[MAX_DATA][MAX_DATA];
	int routeNum[MAX_DATA];
	int routeSum[MAX_DATA];
	int value[MAX_DATA];

	int *result = NULL;

	for ( i = 0; i < n; i ++ )
	{
		routeNum[i] = 0;
		routeSum[i] = 1;
		value[i] = 1;

		for ( j = 0; j < n; j++ )
		{
			route[i][j] = -1;
		}
	}

	for ( i = n - 1; i > 0; i-- )
	{
		for ( j = i - 1; j >= 0; j-- )
		{
			if ( data[i] > data[j] )
			{
				if ( value[i] >= value[j] )
				{
					route[j][0] = i;
					value[j] = value[i] + 1;

					routeNum[j] = 1;
					routeSum[j] = routeSum[i];

					if ( value[j] > max )
					{
						max = value[j];
						maxIdx[0] = j;
						maxIdxNum = 1;
					}
					else if ( value[j] == max )
					{
						maxIdx[maxIdxNum] = j;
						if ( sort( maxIdxNum + 1, data, maxIdx ) == 0 )
						{
							maxIdxNum++;
						}
					}
				}
				else if ( value[i] + 1 == value[j] )
				{
					route[j][routeNum[j]] = i;
										
					if ( sort( routeNum[j] + 1, data, route[j] ) == 0 )
					{
						routeSum[j] += routeSum[i];
						routeNum[j]++;
					}

					if ( value[j] == max )
					{
						maxIdx[maxIdxNum] = j;
						if ( sort( maxIdxNum + 1, data, maxIdx ) == 0 )
						{
							maxIdxNum++;
						}
					}
				}
			} // if data[i] > data[j]
		} // for j
	} // for i

	result = traceRoute( kTh,
						 max, 
						 maxIdxNum, 
						 maxIdx,
						 routeNum, 
						 routeSum, 
						 data, 
						 route );

	*numResult = max;

	return result;
}

void inputData( const int n , int *data )
{
	int i = 0;

	for ( i = 0; i < n; i++ )
	{
		scanf( "%d", &data[i] );
	}
}

int main( void )
{
	int i = 0;
	int j = 0;

	int kThLIS = 0;
	int numOfTest = 0;
	int numOfData = 0;

	int data[MAX_DATA];

	int numResult = 0;
	int *result = NULL;

	scanf( "%d", &numOfTest );

	for ( i = 0; i < numOfTest; i++ )
	{
		scanf( "%d %d", &numOfData, &kThLIS );

		inputData( numOfData, data );

		result = LIS( kThLIS, numOfData, data, &numResult );
	
		printf( "%d\n", numResult );
		for ( j = 0; j < numResult; j++ )
		{
			printf( "%d ", result[j] );
		}
		printf( "\n" );

		free( result );
		result = NULL;
	}
	return 0;
}
