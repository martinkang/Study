#include <stdio.h>

#define MAX_TEST_CASE 100
#define MAX_RENTAL_DAY 1000
#define MAX_FESTIVAL_TEAM 1000
#define MAX_COST 100

void solveMinRentalCost( const int aRentalDay, const int aFestivalTeam, double *aRentalCost )
{
	int i = 0;
	int j = 0;
	
	int sRentalDay = aRentalDay;
	
	int sFestivalTeam = aFestivalTeam;
	
	double *sRentalCost = aRentalCost;
	double sSumRentalCost[MAX_RENTAL_DAY] = {0.0, };
	double sTempCost = 0;
	double sMin = 0;

	for ( i = 0; i < sRentalDay; i++ )
	{
		sSumRentalCost[i] = sTempCost + sRentalCost[i];
		sTempCost = sSumRentalCost[i];
	}
	sMin = sSumRentalCost[sFestivalTeam - 1] / sFestivalTeam;

	for ( i = 0; i < sRentalDay - sFestivalTeam + 1; i++ )
	{
		for ( j = i + sFestivalTeam - 1; j < sRentalDay; j++ )
		{
			if ( i > 0 )
				sTempCost = ( sSumRentalCost[j] - sSumRentalCost[i - 1] )/ ( j - i + 1 );
			else
				sTempCost = sSumRentalCost[j] /  ( j + 1 );
			if ( sTempCost < sMin )
			{
				sMin = sTempCost;
			}
		}
	}

	printf("%.12lf\n", sMin );
}

int main( void )
{
	int i = 0;
	int j = 0;
	int sNumOfTest = 0;
	int sRentalDay = 0;
	int sFestivalTeam = 0;

	double sRentalCost[MAX_RENTAL_DAY] = { 0.0, };

	scanf( "%d", &sNumOfTest );

	for ( i = 0; i < sNumOfTest; i++ )
	{
		scanf( "%d %d", &sRentalDay, &sFestivalTeam );
		for ( j = 0; j < sRentalDay; j++ )
		{
			scanf( "%lf", &( sRentalCost[j] ) );
		}

		solveMinRentalCost( sRentalDay, sFestivalTeam, (double*)sRentalCost );
	}

	return 0;
}
