
#ifndef __PACKET_H__
#define __PACKET_H__

typedef struct packet
{
	int type;
	int flag;
	int size;
	char *msg
} packet_t;
#endif

/* type */
#define REQUEST 0
#define RESPONS 1
#define MESSAGE 2

/* flag */
#define NEWCHAT 10
#define ENDCHAT 11
