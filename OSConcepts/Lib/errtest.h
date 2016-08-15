#include <errno.h>

#define ERR_TEST( a, b ) \
    if ( a ) 			 \
    {       			 \
        perror( b ); 	 \
        return -1; 		 \
    }

