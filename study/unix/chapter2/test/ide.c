#include <stdio.h>

#define IDE_CONT( __label ) goto __label

#define IDE_TEST( __condition ) \
	if ( __condition == 0 )\
	  goto EXCEPTION_END  \
	
#define IDE_TEST_RAISE( __condition, __label ) \
	if ( __condition == 0 )\
	  goto __label  \

#define IDE_EXCEPTION( __label ) __label:


int main()
{
	int i = 0;

	IDE_CONT( ID );
	IDE_TEST_RAISE(0, Ii );

	return 0;

	IDE_EXCEPTION( ID )
	{
		printf( "ID\n" );
	}

	IDE_EXCEPTION( Ii )
	{
		printf( "IDi\n" );
	}


	return -1;
}
