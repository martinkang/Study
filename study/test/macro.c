#include <stdio.h>
#include <string.h>

#define isup(c) ((c) >= 'A' && (c) <= 'Z' )

int main(void)
{
	int sRC = 0;
	char c;

	while( (isup( c = getchar() ) ) );
	return 0;
}
