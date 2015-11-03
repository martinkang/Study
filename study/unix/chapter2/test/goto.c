#include <stdio.h>

int main()
{
	goto mu;
	printf ( "before\n");
mu:
	printf( "after\n" );
	return 0;
}
