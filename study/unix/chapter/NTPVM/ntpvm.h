#include <pthread.h>
#include <sys/types.h>

#define MAX_PACK_SIZE 1024
#define MAX_TASKS 10
#define NUMTYPES 6

typedef enum ptype
{
	NEWTASK,
	DATA, 
	BROADCAST,
	DONE,
	TERMINATE,
	BARRIER
} packet_t;

typedef struct
{
	int compid;
	int taskid;
	packet_t type;
	int length;
} taskpacket_t;

typedef struct
{
	int compid;
	int taskid;
	int writefd;
	int readfd;
	int recvbytes;
	int recvpacksets;
	int sentbytes;
	int sentpackets;
	pid_t taskpid;
	pthread_t tasktid;
	int barrier;
	pthread_mutex_t mlock;
	int endinput;
} ntpvm_task_t;
