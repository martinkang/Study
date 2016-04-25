#include <stdio.h>

#define MAX_TEST_CASE 100
#define MAX_RENTAL_DAY 1000
#define MAX_FESTIVAL_TEAM 1000
#define MAX_COST 100

void solveMinRentalCost( const int aRentalDay, const int aFestivalTeam, const double *aRentalCost )
{
    int i = 0;
    int sRentalDay = aRentalDay;
    int sFestivalTeam = aFestivalTeam;
    const double *sRentalCost = aRentalCost;
    double sSumRentalCost[MAX_RENTAL_DAY] = {0.0, };
    double sTempCost = 0;

    int sIdx = -1;
    double sMin = 999.0;
    double sMinAverage = 999.0;

    double sOldAverage = 0.0;
    double sNewAverage = 0.0;

    for ( i = 0; i < sRentalDay; i++ )
    {
        sSumRentalCost[i] = sTempCost + sRentalCost[i];
        sTempCost = sSumRentalCost[i];
    }
    sMin = sSumRentalCost[sFestivalTeam - 1] / sFestivalTeam;
    sMinAverage = sMin;

    for ( i = sFestivalTeam; i < sRentalDay; i++ )
    {
        if ( sIdx == -1 )
        {
            sOldAverage = sSumRentalCost[i] / ( i + 1 );
        }
        else
        {
            sOldAverage = ( sSumRentalCost[i] - sSumRentalCost[sIdx] ) / ( ( i + 1 ) - ( sIdx + 1 ) );
        }
        sNewAverage  = ( sSumRentalCost[i] - sSumRentalCost[i - sFestivalTeam] ) / sFestivalTeam ;

        if ( sOldAverage <= sNewAverage )
        {
            sMinAverage = sOldAverage;
        }
        else
        {
            sMinAverage = sNewAverage;
            sIdx = i - sFestivalTeam;
        }

        if ( sMinAverage < sMin )
            sMin = sMinAverage;
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
