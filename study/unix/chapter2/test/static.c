#include <stdio.h>


static void test()
{
	static int i = 0;

	i++;
	printf( "static %d \n", i );
}

int main()
{
	test();

	test();

	return 0;
}
