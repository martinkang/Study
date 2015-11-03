#include <time.h>

typedef struct data_struct
{
	time_t time;
	char *str;
} data_t;

int accData(void);
int addData(data_t data);
int freeKey(int key);
int getData(int key, data_t *dataPtr );
