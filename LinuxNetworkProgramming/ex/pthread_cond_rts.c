
#define MAX_THREAD 5
#define MAX_Q_SIZE 20
#define PATH_FIFO "my_fifo"

void *threadFuncA( void * ); /* thread (A) */
void *threadFuncB( void * ); /* thread (B) */

struct threadArg
{
	{ 0, threadFuncA },
	{ 0, threadFuncB },
	{ 0, threadFuncB },
	{ 0, threadFuncB },
	{ 0, NULL }
}

int processJob( int * );
void cleanThread( struct threadArg * );
int instSigHandler( void );
void hSigevRt1( int signo, siginfo_t *si, void *data );
/*RTS 시그널 핸들러 함수 */

struct sharedData
{
	int queue[MAX_Q_SIZE];
	int idx; /* last index */
	int cnt; /* item counter */
	pthread_mutex_t mutex;
	pthread_cond_t cv;
} *pSharedData;

int main( void )
{

}

