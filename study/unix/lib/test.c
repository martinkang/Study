#include <stdio.h>
#include "ide.h"

int main(void)
{
	int sRC = 0;

	IDE_TEST_RAISE( 1, a );
	IDE_TEST_RAISE( 1, b );

	IDE_TEST( 1 );

	return 0;

	IDE_EXCEPTION( a )
	{
		printf( "a\n" );
	}

	IDE_EXCEPTION( b )
	{
		printf( "b\n" );
	}
	
	EXCEPTION_END

	printf( "c\n" );

	return -1;
}
