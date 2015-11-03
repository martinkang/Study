#include <stdio.h>
#include <string.h>

#define CONCAT( __a, __b ) (#__a)##(#__b)

int main()
{
	char *a;
	char *b = "string";
	char *c = "bbbb";

	a = CONCAT( b, c );

	printf( "CONCAT : %s\n", a );

	return 0;
}
