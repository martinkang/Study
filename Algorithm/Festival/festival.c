#include <stdio.h>

#define MAX_TEST_CASE 100
#define MAX_RENTAL_DAY 1000
#define MAX_FESTIVAL_TEAM 1000
#define MAX_COST 100
#define NO_DATA -1

void sumRentalCost( double *aSumRentalCost, const int aRentalDay, const double *aRentalCost )
{
    int i = 0;
    double sTempCost = 0.0;

    for ( i = 0; i < aRentalDay; i++ )
    {
        aSumRentalCost[i] = sTempCost + aRentalCost[i];
        sTempCost = aSumRentalCost[i];
    }
}

double oldAverage( const double *aSumRentalCost, int aI, int aMinIdx )
{
    double sAverage = 0.0;

    if ( aMinIdx == -1 )
    {
        sAverage = aSumRentalCost[aI] / ( aI + 1 );
    }
    else
    {
        sAverage = ( aSumRentalCost[aI] - aSumRentalCost[aMinIdx] ) / ( aI - aMinIdx  );
    }

    return sAverage;
}

void compareAverage( double  aOldAverage,
                     double  aNewAverage,
                     double *aMinAverage,
                     int     aDay,
                     int    *aMinIdx )
{
    if ( aOldAverage < aNewAverage )
    {
        *aMinAverage = aOldAverage;
    }
    else
    {
        *aMinAverage = aNewAverage;
        *aMinIdx = aDay;
    }
}

double solveMinRentalCost( const int aRentalDay, const int aFestivalTeam, const double *aRentalCost )
{
    int i = 0;
    const int sRentalDay = aRentalDay;
    const int sFestivalTeam = aFestivalTeam;

    double sSumRentalCost[MAX_RENTAL_DAY] = {0.0, };

    int sMinIdx = -1;
    double sMin = NO_DATA;

    double sMinAverage = NO_DATA;
    double sOldAverage = NO_DATA;
    double sNewAverage = NO_DATA;

    sumRentalCost( sSumRentalCost, sRentalDay, aRentalCost );

    sMin = sSumRentalCost[sFestivalTeam - 1] / sFestivalTeam;

    for ( i = sFestivalTeam; i < sRentalDay; i++ )
    {
        sOldAverage = oldAverage( sSumRentalCost, i, sMinIdx );
        sNewAverage  = ( sSumRentalCost[i] - sSumRentalCost[i - sFestivalTeam] ) / sFestivalTeam ;

        compareAverage( sOldAverage,
                        sNewAverage,
                        &sMinAverage,
                        i - sFestivalTeam,
                        &sMinIdx );

        if ( sMinAverage < sMin )
        {
            sMin = sMinAverage;
        }
    }

    return sMin;
}

int main( void )
{
    int i = 0;
    int j = 0;
    int sNumOfTest = 0;
    int sRentalDay = 0;
    int sFestivalTeam = 0;
    double sFirstMin = 999.99;
    double sSecondMin = 999.99;

    double sRentalCost[MAX_RENTAL_DAY] = { 0.0, };
    double sSecondRentalCost[MAX_RENTAL_DAY] = { 0.0, };

    scanf( "%d", &sNumOfTest );

    for ( i = 0; i < sNumOfTest; i++ )
    {
        scanf( "%d %d", &sRentalDay, &sFestivalTeam );
        for ( j = 0; j < sRentalDay; j++ )
        {
            scanf( "%lf", &( sRentalCost[j] ) );
            sSecondRentalCost[sRentalDay - ( j + 1 )] = sRentalCost[j];
        }

        sFirstMin = solveMinRentalCost( sRentalDay, sFestivalTeam, (double*)sRentalCost );
        sSecondMin = solveMinRentalCost( sRentalDay, sFestivalTeam, (double*)sSecondRentalCost );

        if ( sFirstMin < sSecondMin )
            printf( "%.12f\n" , sFirstMin );
        else
            printf( "%.12f\n" , sSecondMin );
    }

    return 0;
}
