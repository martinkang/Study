# POSIX 스레드의 이용 - 2

## 4. 변수와 메모리
	1. heap
		- 전역 변수와 힙 영역에 할당된 메모리는 자동으로 스레드 간에 공유된다.
		- 한 애플리케이션 내의 모든 스레드는 전역 변수와 힙 영역에 할당된 데이터에 접근할 수 있다.
	2. volatile
		- 컴파일러가 최적화를 하는 것을 방지한다.
		- volatile 로 선언된 변수는 변수에 접근할 때마다 매번 메모리에서 새로 로딩한다.
			- 컴파일러에 따라 지원하지 않을 수 있다.
		- 함수 호출을 통해 강제적으로 변수를 다시 로딩하게 하는 방법도 있다.
			- 어떤 컴파일러는 함수 호출 부분을 인라이닝 해 버려서 안될 수 있다.
```c++
//무한 루프로 빠질 수 있는 코드
int done = 0;

void wait()
{
	while( !done ){}
}
```
```c++
int done = 0;

void wait()
{
	while( !(volatile int )done ){}
}
```
```c++
//함수 호출을 통해 강제적으로 변수를 다시 로딩하게 한다.
int done = 0;

void pause()
{
}

void wait()
{
	while( !done ){ pause() }
}
```
	3. restrict
		- *pointer alias* 가 없음을 한정해줌 -> 컴파일러 최적화
		- 아래 예제 코드에서, 컴파일 시점에는 변수 배열이 저장되는 변수 total  이 가리키는 메모리 위치가   
		어디인지 알 수가 없다.
			- 따라서 컴파일러 입장에서는 해당 변수가 배열의 한 부분일 수 있다고 가정하는 것이 안전하다.
			- 이러한 가정에 따라 total 의 주소에 저장된 값은 루프마다 메모리로 업데이트 되어야 한다.
			- 컴파일러로 하여금 불필요한 메모리 읽기/저장 명령을 포함하여 성능 저하 유발
			- 메모리 작업 순서를 바꿀 수 없게 하여 컴파일러 최적화를 못하게 한다.

```c++
//pointer alias 가 가능한 코드

void sum( double* total, double* array, int len )
{
	int i = 0;

	for ( i = 0; i < len; i++ )
	{
		*total += array[i];
	}
}
```

	4. Thread Private Data
		- 스택에 저장된 데이터.
		- Thread Local Storage( TLS )
		- __thread   
			- 해당 지정자를 붙이는 것으로 해당 변수에 대해 스레드 별로 복제본을 생성해 스레드 간에 공유가 안되면서   
			같은 스레드 컨텍스트 하에서 실행되는 코드에는 전역 변수로 보이게 해준다.
			- 컴파일러가 스레드 별로 별도의 메모리(.tls)영역을 할당해서 thread 전용 변수들을 저장한다.   
			이후 링크 과정에서 PT_TLS 타입의 데이터 세그먼트로 합쳐지게 된다
			- 단점
				- 컴파일할때 TLS의 크기가 결정되기 때문에 정적으로만 사용가능하다
				- 런타임에 쓰레드에게 더 큰 메모리할당이 필요해도 확장하는 것이 불가능하다.
				- 운영체제가 일반적으로 사용하는 메모리와는 다른 별도의 메모리 영역을 사용하기 때문에,   
				컴파일러의 TLS변수를 참조하는 코드는 번역단계에서 추가적인 명령들이 필요하다.
					* 따라서 속도가 좀 느려진다

		- pthread_key_create() & pthread_key_delete()
			* posix 에서 지원하는 스레드 로컬 변수
			* 필요할 때마다 명령어를 통해서 원하는 공간을 요청할 수 있다는 점에서 동적이다.
			* 기존 메모리 구조를 그대로 사용해서 별도의 추가 명령어가 필요하지 않다.
			- int pthread_key_create( pthread_key_t *key, void (*destr_function )( void *) )
			  * pthread_key_t : unsigned int
				- 쓰레드 개별 공간(thread specific data) 영역 만들기.
				- 스레드 로컬 변수에 접근할 때마다 해당 변수를 식별할 수 있는 키를 생성.
			- int pthread_setspecific( pthread_key_t key, const void *pointer )  
				- TSD(thread specific data) 영역에 값쓰기
			- void *pthread_getspecific( pthread_key_t key )  
				- TSD 영역에 값 읽기
			- int pthread_delete( pthread_key_t )  
				- TSD 영역 없애기 (키 없애기)

```c++
// 스레드 종속 데이터 - 스택에 저장된 데이터
double func( double a )
{
	double b;
	...
```
```c++
//__thread 지정자를 이용한 스레드 로컬 데이터의 선언
__thread void* mydata;

void* threadFunc( void* param )
{
	mydata = param;
	...
```
```c++
//posix 에서 지원하는 스레드 로컬 변수
void ppp( void )                                                                                                               {
	void *param2 = pthread_getspecific( pkey );
	printf( "param %d\n", *((int*)(param2)) );
}

void *threadFunc( void *param )
{
	int *mem = (int*)malloc( 100 );

	pthread_setspecific( pkey, mem );

	*mem = 400;
	ppp();
}

int main()
{
    pthread_t thread;
    pthread_t th;
    pthread_key_create( &pkey, destructor );
    pthread_create( &thread, NULL, (void*)threadFunc, NULL );
    pthread_create( &thread, NULL, (void*)ppp2, NULL );
    pthread_join( thread, NULL );
    pthread_key_delete( pkey );
}  
```


## 5. 멀티 프로세스 프로그래밍
	* 멀티 프로세스는 스레드와는 달리 프로세스 하나가 비정상적으로 종료되더라도 다른 프로세스가 영향을 받지 않는다.
	* UNIX 는 프로세스 생성시 Fork-Exec 모델을 이용한다.
		- fork()
			* 어떤 프로세스에서 fork 를 실행하게 되면, 자신의 프로세스와 똑같은 프로세스를 copy-on-write 형식으로 실행하게 되며,  
			이때 생성된 프로세는 자신만의 PID를 가지고 독자적인 길을 가게 된다
		- exec()
			* 새로운 프로새스를 생성시키지만, fork 와 같이 copy-on-write 를 이용한 전혀 새로운 프로세스를 실행시키지 않고,   
			현재의 프로세스이미지를 새로운 프로세스 이미지가 덮어써 버린다.  

	1. 프로세스 간 메모리 공유
		- int shm_open(const char *name, int oflag, mode_t mode)
			* 공유 메모리 생성
			- oflag : 읽기, 쓰기 등의 속성 플래그.
			- mode : 접근 권한 - 일반적인 UNIX 파일 권한 설정 비트와 같다.
		- int ftruncate( int fildes, off_t length )
			* 파일을 지정한 크기로 변경
		- void* mmap( void* addr, size_t len, int prot, int flags, int fd, off_t offset )
		 	* fd 로 지정된 파일 ( 혹은 객체 ) 에서 offset 을 시작으로 length 바이트 만큼을 addr 주소로 대응시키도록 한다.
			* return : 맵핑이 시작하는 실제 메모리 주소
			- void* addr : NULL 일 경우 공유 메모리 세그먼트의 시작 주솟값, addr 값이 존재할 경우 addr 이 시작 주솟값이 된다.
				- 해당 주소 공간을 이용할 수 없을 경우에는 적용되지 않을 수 있다.
			- int prot : 공유 메모리 보호 설정 플래그, 보통 shm_open() 시 설정한 접근 권한과 맞추어 설정
				- PROT_EXEC : 코드 실행용
				- PROT_READ : 읽기 전용
				- PROT_WRITE : 쓰기 가능 메모리
			- int flags : 공유설정
				-	MAP_SHARED : 다른 프로세스와 공유
				- MAP_PRIVATE : 해당 프로세스만 사용 가능
			- off_t offset : 공유 메모리 영역에 접근할 때 기본으로 적용될 주소 오픈.
```c++
//공유 메모리를 이용한 mutex 공유
void main()
{
	pthread_mutex_t * mutex;
	pthread_mutexattr_t attributes;
	pthread_mutexattr_init( &attributes );
	pthread_mutexattr_setpshared( &attributes, PTHREAD_PROCESS_SHARED );

	int handle = shm_open( "/shm", O_CREAT|O_RDWR, 0777 );
	ftruncate( handle, 1024*sizeof(int) );
	char * mem = mmap( 0, 1024*sizeof(int), PROT_READ|PROT_WRITE,
					MAP_SHARED, handle,0 );

	mutex = (pthread_mutex_t*)mem;
	pthread_mutex_init( mutex, &attributes );
	pthread_mutexattr_destroy( &attributes );

	int ret = 0;
	int * pcount = (int*)( mem + sizeof(pthread_mutex_t) );
	*pcount = 0;

	pid_t pid = fork();
	if (pid == 0)
	{  /* Child process */
			pthread_mutex_lock( mutex );
			(*pcount)++;
			pthread_mutex_unlock( mutex );
			ret = 57;
	}
	else
	{
			int status;
			waitpid( pid, &status, 0 );
			printf( "Child returned %i\n", WEXITSTATUS(status) );
			pthread_mutex_lock( mutex );
			(*pcount)++;
			pthread_mutex_unlock( mutex );
			printf( "Count = %i\n", *pcount );
			pthread_mutex_destroy( mutex );
	}
	munmap( mem, 1024*sizeof(int) );
	shm_unlink( "/shm" );
}
```

	2. 프로세스 간의 세마포어 공유
		* sem_t* sem_open( const char *name, int oflag,
                       mode_t mode, unsigned int value)
			- int oflag :
			- mode_t mode :
			- unsigned int value : 세마포 초기값
```c++
	typedef union
	{
  	char __size[__SIZEOF_SEM_T];
  	long int __align;                                                                                                                  
	} sem_t;
```

```c++
	 //공유 메모리를 이용한 세마포어 공유
	 int status;
	 pid_t f = fork();
	 sem_t * semaphore;

	 semaphore = sem_open( "/my_semaphore", O_CREAT, 0777, 1 );

	 if ( f == 0 )
	 { /* Child process */
			 sem_post( semaphore );
			 sem_close( semaphore );
			 printf( "Child process completed\n" );
	 }
	 else
	 {
			 sem_wait( semaphore );
			 sem_close( semaphore );
			 sem_unlink( "my_semaphore" );
			 printf( "Parent process completed\n" );
	 }
```

	3. 메시지 큐
		* 스레드 또는 프로세스 간에 메시지를 주고받을 수 있게 해준다.
		*  mqd_t mq_open(const char *name, int oflag, mode_t mode,
                     struct mq_attr *attr);
		* int mq_send(mqd_t mqdes, const char *msg_ptr,
                     size_t msg_len, unsigned int msg_prio);
		* int mq_timedsend(mqd_t mqdes, const char *msg_ptr,
                     size_t msg_len, unsigned int msg_prio,
                     const struct timespec *abs_timeout);

```c++
		typedef int mqd_t;                                                                                                                   
		struct mq_attr
		{
  		__syscall_slong_t mq_flags;   /* Message queue flags.  */
  		__syscall_slong_t mq_maxmsg;  /* Maximum number of messages.  */
  		__syscall_slong_t mq_msgsize; /* Maximum message size.  */
  		__syscall_slong_t mq_curmsgs; /* Number of messages currently queued.  */
  		__syscall_slong_t __pad[4];
		};
```

```c++
		//메시지 큐를 이용하여 부모와 자식 프로세스의 메시지 전달
		int status;
		mqd_t queue;
		char message[200];
		pid_t f = fork();

		if ( f == 0 )
		{ /* Child process */
				queue = mq_open( "/messages", O_WRONLY | O_CREAT, 0777, 0 );
				strncpy( message, "Hello", 6 );
				printf( "Send message %s\n", message );
				mq_send( queue, message, strlen(message)+1, 0 );
				mq_close( queue );
				printf( "Child process completed\n" );
		}
		else
		{
				queue = mq_open( "/messages", O_RDONLY | O_CREAT, 0777, 0 );
				mq_receive( queue, message, 200, 0 );
				printf( "Receive message %s\n", message );
				mq_close( queue );
				mq_unlink( "/messages" );
				printf( "Parent process completed\n" );
		}
```

	4. 일반 파이프와 지정 파이프
		* 파이프는 두 프로세스 간의 데이터 스트림을 연결해준다.
			- 선입선출로 동작한다.
		1. unnamed pipe
```c++
	int main()
	{
		int status;
		int pipes[2];
		pipe(pipes);
		pid_t f = fork();

		if (f==0)
		{ /* Child process */
			close(pipes[0]);
			write(pipes[1],"a",1);
			printf("Child sent 'a'\n");
			close(pipes[1]);
		}
		else
		{
			char buffer[11];
			close(pipes[1]);
			int len=read(pipes[0],buffer,10);
			buffer[len]=0;
			printf("Parent received %s\n",buffer);
			close(pipes[0]);
		}                                                                           
		return 0;
	}
```
		2. named pipe
```c++
int main()
{
	int status;
	mknod( "/tmp/pipefile", S_IFIFO|S_IRUSR|S_IWUSR, 0 );
	pid_t f = fork();
	if ( f == 0 )
	{ /* Child process */
		int mypipe = open( "/tmp/pipefile", O_WRONLY );
		write( mypipe, "a", 1 );
		printf( "Child sent 'a'\n" );
		close( mypipe );
	}
	else
	{
		int mypipe = open( "/tmp/pipefile", O_RDONLY );
		char buffer[11];
		int len = read( mypipe, buffer, 10 );
		buffer[len] = 0;
		printf( "Parent received %s\n", buffer );
		close( mypipe );
	}
	unlink( "/tmp/pipefile" );
	return 0;
}
```
	5. 시그널을 이용한 프로세스 간 커뮤니케이션
		* 시그널을 이용할 때 한가지 문제점은 시그널 핸들러가 구동되는 동안에는 다른 스레드에서  
		호출한 시스템 콜을 방해할 수 있다는 점이다.
		* 시그널 핸들러 안에서 함수를 호출할 때 signal-safe  한지 확인해야 한다.
		* 시그널 핸들러는 프로세스 단위로 지정되는 반면, 각 쓰레드는 자기만의 시그널 마스크를 가진다.
			- int pthread_sigmask( int how, const sigset_t *restrict set, sigset_t *restrict oset )
			- int sigprocmask( int how, const sigset_t *set, sigset_t *oldset );
			* int how
				- SIG_BLOCK
					- 기존에 블록된 시그널 집합에 두번째 인수 set 시그널 집합을 추가
				- SIG_UNBLOCK
					- 기존에 블록화된 시그널 집합에 두번째 인수 set 시그널 집합을 제거
				- SIG_SETMASK
					- 이전 블록된 시그널 집합을 모두 지우고 두번째 인수인 set 시그널 집합으로 설정
		* 프로세스가 여러 쓰레드를 가진다면, 메인 쓰레드가 다른 쓰레드를 생성한 이후에  
		sigprocmask 함수를 더 이상 사용해서는 안된다.
		* 멀티쓰레드 프로세스에서 시그널을 다루기 위한 추천할 만한 방법중 하나는 특정 쓰레드가 시그널 핸들링을 전담
		* SIGRTMIN & SIGRTMAX
			- 사용자 정의 시그널로 이용할 수 있는 시그널 번호에 대한 최솟값과 최댓값
		- sighandler_t signal(int signum, sighandler_t handler);
			- return : 이전에 등록 되어 있던 시그널 핸들러
		- int sigaction(int signum, const struct sigaction *act,
                     struct sigaction *oldact);
			- struct sigaction *oldact : 이전 등록 되어 있던 sigaction 구조체 포인터
		- int sigqueue(pid_t pid, int sig, const union sigval value);
			- pid 에 지정된 프로세스로 sig 를 보낸다. value 를 이용해서 시그널과 함께 필요한 데이터를 전송할 수 있다.
		- int kill(pid_t pid, int sig);
			- pid 로 signal( sig ) 를 보낸다.
		- int pthread_kill( pthread_t thread, int sig );
			- thread 로 signal( sig ) 를 보낸다.
		- int sigwait( const sigset_t *set, int *sig )
			- sigset 에 지정된 시그널이 전달될 때까지 해당 영역에서 대기.   
			- 시그널을 받게 되면 받은 시그널 번호를 sig 에 복사하고 리턴
			- set 에 명시된 시그널들은 반드시 봉쇄로 지정되어야 한다.
```c++
union sigval {
               int   sival_int;
               void *sival_ptr;
           };
```

```c++
struct sigaction {
               void     (*sa_handler)(int); // 시그널을 처리하기 위한 핸들러
               void     (*sa_sigaction)(int, siginfo_t *, void *); // 밑의 sa_flags 가 SA_SIGINFO 일 때
			   													   // sa_handler 대신에 동작하는 핸들러
               sigset_t   sa_mask; // 시그널을 처리하는 동안 블록화할 시그널 집합의 마스크
               int        sa_flags; // 아래 참조              
			   void     (*sa_restorer)(void); // 사용해서는 안됩니다.
           };

/*
- SA_NOCLDSTOP : signum이 SIGCHILD일 경우, 자식 프로세스가 멈추었을 때, 부모 프로세스에 SIGCHILD가 전달되지 않는다.  
-SA_ONESHOT or SA_RESETHAND : 시그널을 받으면 설정된 행도을 취하고 시스템 기본 설정인 SIG_DFL 로 재설정된다.  
- SA_RESTART : 시그널 처리에 의해 방해 받은 시스템 호출은 시그널 처리가 끝나면 재시작한다.  
SA_NOMASK or SA_NODEFER : 시그널을 처리하는 동안에 전달되는 시그널은 블록되지 않는다.  
- SA_SIGINFO : 이 옵션이 사용되면 sa_handler대신에 sa_sigaction이 동작되며, sa_handler 보다 더 다양한 인수를 받을 수 있다.  
			   sa_sigaction이 받는 인수에는 시그널 번호, 시그널이 만들어진 이유, 시그널을 받는 프로세스의 정보이다.
*/

```
```c++
void hsignal(int signal)
{
	/* do somthing */
}

int main()
{
	signal(SIGRTMIN+4,hsignal);
	kill(getpid(),SIGRTMIN+4);
	return 0;
}
```

```c++
//시그널 핸들러의 체이닝
struct sigaction oldhandler;

void hsignal( int signal, siginfo_t* info, void* other )
{
	write( 1, "Got signal\n", 11 );
	if(oldhandler.sa_sigaction)
	{
		oldhandler.sa_sigaction( signal, info, other );
	}
}

void main()
{
	struct sigaction newhandler;
	newhandler.sa_sigaction = hsignal;
	newhandler.sa_flags = 0;
	sigemptyset( &newhandler.sa_mask );
	sigaction( SIGPROF,&newhandler, &oldhandler );
	kill( getpid(), SIGPROF );
}
```

```c++
//데이터가 첨부된 시그널 전송
volatile int go = 0;
struct sigaction oldhandler;

void handler( int sig, siginfo_t *info, void *context )
{
	go = (int)info->si_value.sival_int;
	write( 1, "Signal arrived\n", 16 );
}

void main()
{
	struct sigaction newhandler;
	newhandler.sa_sigaction = handler;
	newhandler.sa_flags = SA_SIGINFO;
	sigemptyset( &newhandler.sa_mask );
	sigaction( SIGRTMIN+4, &newhandler, &oldhandler );

	pid_t f = fork();
	if ( f == 0 )
	{ /* Child process */
		while ( !go ){}
		printf( "Child completed go=%i\n", go );
	}
	else
	{
		union sigval value;
		value.sival_int = 7;
		sigqueue( f, SIGRTMIN+4, value );
		waitpid( f, 0, 0 );
		printf( "Parent completed\n" );
	}
}
```

```c++
//시그널 전담 쓰레드의 이용

static void *signalthread(void *arg) 
{                                                          
	sigset_t intmask;
	int sig;

	sigemptyset(&intmask);

	sigaddset(&intmask, signalnum);

	sigwait(&intmask, &sig);
}

int signalthreadinit(int signo)
{
	sigset_t set;
	pthread_t sighandid;

	signalnum = signo;                   

	sigemptyset(&set); 

	sigaddset(&set, signalnum);

	sigprocmask(SIG_BLOCK, &set, NULL);

	pthread_create(&sighandid, NULL, signalthread, NULL))

		return 0;
}  

int main(int argc, char *argv[]) 
{
	int i;
	int numthreads;
	pthread_t *tids;

	signalthreadinit(SIGUSR1));

	numthreads = atoi(argv[1]);
	tids = (pthread_t *)calloc(numthreads, sizeof(pthread_t));

	for (i = 0; i < numthreads; i++)      /* create numthreads computethreads */
		pthread_create(tids+ i, NULL, computethread, NULL);

	fprintf(stderr, "Send SIGUSR1(%d) signal to proc %ld to stop calculation\n",
			SIGUSR1, (long)getpid());

	for (i = 0; i < numthreads; i++)    /* wait for computethreads to be done */
		pthread_join(tids[i], NULL);

	showresults();

	return 0;
}        
```
