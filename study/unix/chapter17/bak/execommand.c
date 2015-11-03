#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ntpvm.h"

int execommand( taskpacket_t *header, char *cmd )
{
    int sRC;
    int i = 0;
    int argc = 0;
    char **argv;

    if ( cmd == NULL )
        return -1;

    if ( header == NULL )
        return -1;

    argc = makearg( cmd, &argv, " " );
    if ( argc == -1 )
        return -1;

    sRC = execvp( argv[0], argv );
	
    free( argv );

    return sRC;
}

