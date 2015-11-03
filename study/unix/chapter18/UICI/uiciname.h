#include <netdb.h>

int name2addr( const char *name, in_addr_t *addrp );

void addr2name( struct in_addr addr, char *name, int namelen );
