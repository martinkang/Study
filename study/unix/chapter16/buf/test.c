#include <stdio.h>
#include <semaphore.h>


int main()
{
	int val = 0;

	sem_t sSem;

	sem_init( &sSem, 0, 0 );

	sem_getvalue( &sSem, &val );

	printf( "val : %d\n", val );

	sem_post( &sSem );

	sem_getvalue( &sSem, &val );

	printf( "val : %d\n", val );

	sem_wait( &sSem );

	sem_getvalue( &sSem, &val );

	printf( "val : %d\n", val );

	return 0;
}
