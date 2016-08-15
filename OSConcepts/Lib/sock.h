#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifndef __SOCK_H__
#define __SOCK_H__ 1

// return fd
int createAndBindSock( );

#endif
