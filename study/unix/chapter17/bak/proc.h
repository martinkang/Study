#include "ntpvm.h"

int makearg( const char *origText, char ***argv, const char *delim );

int execommand( taskpacket_t *header, char *cmd );
