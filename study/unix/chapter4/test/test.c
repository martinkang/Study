#include <stdio.h>
#include <string.h>

int main()
{
	char a[] = "asdf\0";
	int b = 0;

	b = strspn( a, "d" );

	printf("b : %d\n", b);
}
