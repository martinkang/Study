#include <stdio.h>

int main()
{
	char test1[10] ={0, };
	char test2[20] = {0, };
	
	fputs( "input test1 : ", stdout );
	fgets( test1, 5, stdin );

	fputs( "input test2 : ", stdout );
	fgets( test2, sizeof( test2 ), stdin );

	printf( "test1 : %s\n", test1 );
	printf( "test2 : %s\n", test2 );
	
	return 0;
}
