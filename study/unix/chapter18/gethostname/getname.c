#include <netdb.h>
#include <stdio.h>

int main()
{
	char *hostn = "usp.cs.utsa.edu";
	char *hostn2 = "www.naver.com";

	struct hostent *hp;
	char **q;

	hp = gethostbyname(hostn2);
	if ( hp == NULL )
		return -1;

	for ( q = hp->h_addr_list; *q != NULL; q++ )
		fprintf( stderr, "host %s IP addr : %s\n", hp->h_name, *q );
	return 0;
}
