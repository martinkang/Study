#include <stdio.h>
#include <string.h>

int main(void)
{
	char a[] = "asdf > aaa < sdfsf\n";

	char *ptr = NULL;

	ptr = strchr( a, '>' );
	*ptr = 0;

	ptr = strtok( ptr + 1, "\t" );

	printf( "ptr :%s\n", ptr );
	return 0;
}
