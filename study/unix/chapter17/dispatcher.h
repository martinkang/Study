#include "ntpvm.h"

#define TERMINATE_STRING ".exit"
#define NUMTYPES 6

#ifndef __DISPATCHER_H__
#define __DISPATCHER_H__

#endif

taskpacket_t *getpacket( int fdin, unsigned char * buf );

int putpacket( int fdout, taskpacket_t *pack, unsigned char *buf );



