# RealTiem Sigtal ( RTS )

## RTS 란?
* POSIX 리얼타임 확장에서 추가된 시그널
* 전통적인 UNIX 시그널 메커니즘과 크게 2가지 다른 점이 존재
	- POSIX 의 새로운 몇몇 I/O 관련 API 를 사용할 때 I/O 발생의 이벤트를
	RTS 로 보고받을 수 있다.
		- SIGEV 이벤트
	- 기존 시그널은 큐를 가지지 못하여 중복된 시그널 발생시에 지연되면
	읽어버리는 것에 비해, RTS 는 시그널 큐를 가진다.
		- RTS 는 중복된 시그널이 발생해도 큐에 의해 관리되어
		읽어버리지 않는다.
			- 큐의 길이는 시스템에 따라 다르며 대개 32개 정도 
* 시그널의 장점인 실시간성을 유지하면서 단점인 대기열부재의 문제를 해결한 향상된 시그널도구라고 이해할 수 있다.


### 기존 I/O 멀티플렉싱에서 poller 의 문제점
* I/O 멀티플렉싱을 구성하는 poller 인 select(2), poll(2) 혹은 epoll 등은 함수를 호출하는 시점보다 더 이전에
I/O 이벤트가 발생하면 함수 호출이 되기 전가지는 이벤트를 알 수 없다.
	- 따라서 poller 를 사용하는 동기적 I/O 구조의 프로그램에서는 어느 정도의 지연을 감수해야 한다.
	- 쓰레드와 같은 비동기 적인 요소의 도입으로 지연을 감소시킬 수 있지만, 
	이벤트 감지를 위해 poller API 의 잦은 호출은 오버헤드를 동반한다.
	- 시그널로 전다한 경우에는 이벤트가 즉시 통보되는 구조이므로 좀 더 명확한 동시성을 확보할 수 있다.


### RTS 의 장단점
* 장점
	* poller 를 사용하는 메커니즘과 달리 이벤트 보고를 위한 리스트를 관리하지 않는다.
		- 커널 공간과 유저 공간 사이의 많은 메모리 복사에 따른 오버헤드 감소
* 단점
	* 시그널 처리기를 사용하는 방식으로 기존 poller 를 사용하는 I/O 멀티플렉싱 구조와는 다른 설계가 된다.
	* 시그널 큐의 길이가 제한되어 있으므로 무작정 큰 스케일ㅇ르 가지는 프로그램을 만드는 것은 불가능.



## RTS 의 사용
* RTS 는 전통적인 UNIX 시그널 뒤에 존재.
	- 일반적으로 SIGRTMIN ~ SIGRTMAX 사이의 값이며, 보통 SIGRTMIN 쪽의 우선순위가 높다.
* RTS 는 siginfo_t 구조체를 통해서 그 정보를 주고받기 때문에 siginfo_t 구조체에 대해서 알아야 한다.

##### siginfo_t 구조체
```c++
siginfo_t
{
	int si_signo; /* 발생된 시그널 번호 */
	int si_errno; /* 에러 코드값 */
	int si_code; /* 시그널의 발생 이유 */
	pid_t si_pid; /* 시그널을 보낸 프로세스의 PID */
	uid_t si_uid; /* 시그널을 보낸 실제 유저 ID ( 시그널을 보낸 프로세스의 effective user id ) */
	int si_status; /* Exit 값이나 시그널 */
	clock_t si_utime; /* 소요된 user time */
	clock_t si_stime; /* 소요된 System time */
	sigval_t si_value; /* Signal value : 시그널 발생시 전달할 값이 있을 경우 사용 ( 공용체 ) */
	int si_int; /* Posix.1b signal */
	void *si_ptr; /* Posix.1b signal */
	void *si_addr; /* Memory Location which caused fault */
	int si_band; /* Band event */
	int si_fd; /* File descriptor */
} sigval_t;

typedef union sigval
{
	int sival_int; /* int 값을 전달하는 경우의 공용체 멤버 */
	void *sival_ptr; /* 포인터를 전달하는 경우의 공용체 멤버 */
} sigval_t;
```

* siginfo_t 구조체는 어떤 I/O 접점에서 언제 어떤 이벤트가 발생하였는지 추적할 수 있다.
* 주요 구조체 멤버
	- int si_code
		* 시그널 발생의 원인이나 특성을 분별
			- 음수 혹은 0 이면 특정 프로세스에 의해서 발생한 이벤트를 통지하기 위해 시그널이 발생했음을 의미
				- 실제로 사용 되는 경우는 I/O 작업의 완료를 알리거나 상태의 변화를 추적하거나
				타이머 만료, kill(2) 로 전달되어지는 시그널 발생을 의미
			* 프로세스로부터 전달된 경우의 si_code 값 : 음수나 0의 값을 가짐
				- SI_SIGIO
					- 대기된 SIGIO 에 의해서 발생한 시그널
				- SI_ASYNCIO
					- AIO ( Asynchronous I/O ) 의 완료에 의해서 발생한 시그널
				- SI_MESGQ
					- 실시간 메시지 큐의 상태가 변화되어 발생한 시그널
				- SI_TIMER
					- 타이머가 만료되어 발생한 시그널
				- SI_QUEUE
					- sigqueue(2) 함수에 의해서 전달되어진 시그널
				- SI_USER
					- kill(2), raise(2) 와 같은 함수에 의해서 전달되어진 시그널 ( 보통 0 의 값을 지님 )
			- 양수로 나타난다면 커널에 의해서 생성된 시그널을 의미
				- 발생된 시그널에 따라서 si_code 값은 각자 다른 값을 가진다.
			* 커널로부터 전달된 시그널 : 양수의 값을 가짐
				- SIGSEGV 시그널 발생 시
					- si_code 발생 값
						- SEGV_MAPERR 
						- SEGV_ACCERR
				- SIGCHLD 시그널 발생시
					- si_code 발생 값
						- CLD_EXITED
						- CLD_KILLED
						- CLD_DUMPED
						- CLD_TRAPPED
						- CLD_STOPPED
						- CLD_CONTINUED
	- si_fd
		- 발생한 파일 기술자 번호
	- si_band
		- 발생한 이벤트
		- poll(2) 에서 사용하는 이벤트 매크로를 그대로 사용
			- ex ) POLLIN


### RTS 에서 사용하는 함수
```c++
int sigwaitinfo( const sigset_t *set, siginfo_t *info );
int sigtimewait( const sigset_t *set, siginfo_t *info, const struct timespec timeout );
int sigqueue( pid_t pid, int sig, const union sigval value );

union sigval
{
	int sival_int;
	void *sival_ptr;
}

```



## RTS 를 이용한 I/O multiplexing


## RTS 를 이용한 디렉토리의 변화 감시



