# POSIX 스레드의 이용

## POSIX 란?
	* POSIX( Portable Operating System Interface ) 
		1. 유닉스 운영체제에 기반을 두고 있는 일련의 표준 운영체제 인터페이스이다.
		2. UNIX 애플리케이션의 호환성을 위해 제정되었다.
		3. 표준화에 관한 필요성은, 컴퓨터를 사용하고 있는 기업들이 다시 코딩하지 않고서도 다른 컴퓨터   
		  회사가 만든 컴퓨터 시스템에도 운영할 수 있도록, 호환성이 있는 프로그램의 개발을 원하는 데에서   
		  기인했다
		4. 마이크로소프트 윈도는 POSIX 표준을 직접적으로 지원하고 있지는 않지만, 위도 환경에서   
		  POSIX 표준을 이용할 수 있도록 중간 레이어를 제공하는 솔루션을 활용할 수 있다.

		* 이 장에서는 POSIX 표준 인터페이스를 이용하여 멀티스레드 프로그램을 개발하는 방법을 다룬다.


## 1. 스레드
	* THREAD 란?
		- CPU가 독립적으로 처리하는 하나의 작업 단위
		- 컴퓨터 프로그램 수행 시 프로세스 내부에 존재하는 수행 경로, 즉 일련의 실행 코드


	1. 스레드드의 생성
		* int pthread_create ( pthread_t  *thread, pthread_attr_t *attr,   
							  void *(*start_routine)(void *), void *arg )
			* pthread_t 구조체 포인터
				- 스레드에 접근할 수 있는 핸들이 이 구조체에 저장된다.
			* pthread_attr_t 구조체 포인터
				- 스레드 속성 포인터. NULL 을 넘기면 디폴트
			* 스레드로 실행될 함수 포인터
				- 새 스레드가 구동될 시작함수 포인터
			* 반환 값
				- 성공 : 0
				- 실패 : 에러 종류에 따른 0 이 아닌 값


	2. 스레드의 종료
		* void pthread_exit( void *var )
			* 스레드를 생성할 때 등록했던 함수가 리턴을 하면 스레드 종료.
				- 묵시적으로 pthread_exit( ) 호출
			* void* var : 부모 스레드가 받을 리턴 데이터를 var 에 저장 가능
			* 명시적으로 스레드가 분리 ( detach ) 되지 않았다면 스레드가 종료되더라도   
			  스레드에서 이용한 리소스는 해제되지 않고 남아 있게 된다.
				- 이 리소스는 종료된 스레드의 핸들을 파라미터로 pthread_join()을 호출할 때까지 유지


	3. 자식 스레드와의 데이터 교환
		1) pthread_create 의 인자로 교환 ( void *arg )
			* 포인터로 자식 스레드에 데이터를 전달하는 것은 가능하면 피하는 것이 좋다.
				- 어느 시점에 어떤 값을 읽어갈지 명확하지 않으므로 메모리 영역이   
	             스레드가 구동되는 동안 유효하다는 보장이 없다.
		2) int pthread_join( pthread_t thread, void **thread_return )
			* 자식 스레드를 기다림. 자식 스레드가 종료되어야만 리턴
			* 만약 thread_return값이 NULL 이 아니라면,스레드 thread 의 리턴값이 저장된 영역이 전달되게 된다.
			* pthread_detach() 함수를 통해서 detached 상태가 되었거나 혹은 pthread_create()로   
			  실행될때 PTHREAD_CREATE_DETACHED 특성으로 실행되었다면 join 으로 기다릴수 없게 된다.
		* 반환값
			* 성공할경우 쓰레드 식별자인 thread 에 쓰레드 식별번호를 저장하고, 0을 리턴한다
			* 실패했을경우 0 이 아닌 에러코드 값을 리턴한다
	

	4. 분리된 스레드
		* int pthread_detach( pthread_t thread )
			* 실행중인 쓰레드를 detached(분리)상태로 만든다
			* 식별자 thread 를 가지는 쓰레드를 메인쓰레드에서 분리 시킨다.
			* 식별자 thread 를 가지는 쓰레드가 종료되는 즉시 쓰레드의 모든 자원을 되돌려줄 것을 보장한다.
			* detach상태가 아닐경우 쓰레드가 종료한다고 하더라도 pthread_join()을 호출하지 않는 한   
			 자원을 되돌려주지 않는다. 
		* 반환값
			- 성공 : 0
			- 실패 : 0 이 아닌 값

	5. 스레드 속성 설정
		* pthread_attr_t
			* detach 여부를 제외한 스레드 속성은 한번 생성되고 나면 바꿀 수가 없다.
			* 각  속성별로 존재하는 속성 설정 전용 API 를 이용해야 한다.
		* 자주 쓰는 스택 속성
			1) 스레드 분리
				- int  pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate)
					(1) pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE)
						- DETACHED 상태로 변경하고자 할때
					(2) pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED)
						- JOINABLE 상태로 변경하고자 할때 
					* 반환 값
						- 성공 : 0
						- 실패 : 0 이 아닌 값
			2) 스레드 스택
				(1) 스택 크기 가져오기
					* int pthread_attr_getstacksize( pthread_attr_t *attributes, int *size )
				* 반환값
					- 성공 : 0
					- 실패 : 0 이 아닌 값
				(2) 스택 크기 조절
					* int pthread_attr_setstacksize( const pthread_attr_t *attr, size_t *stacksize )
					* 멀티 스레드에서 스레드 스택은 싱글 스레드에서 다이나믹한 크기인 것과 달리 크기가 고정된다.	
					* 각 스레드는 다른 스레드으 스택에 바로 인접하여 할당되기 때문에 스레드의 수가 증가하거나   
					 스레드 스택의 크기가 증가하면 힙으로 이용할 수 있는 영역이 줄어든다.
						- 여러 개의 스레드가 각기 스택을 가지려면 메인 스레드의 초기 스택 크기를   
						 제한해야만 하고 자식 스레드의 스택 크기도 제한받는다.
				* 반환값
					- 성공 : 0
					- 실패 : 0 이 아닌 값


## 2. 멀티 스레드 코드 컴파일 하기
	* 멀티 스레드 코드를 컴파일할 때는 헤더 파일과 라이브러리가 문제가 될 수 있다.
		- 어떤 라이브러리 들은 thread-safe 하지 않다. 
		* 솔라리스 : -mt
			- REENTRANT 매크로 정의가 활성화 된다.
		* gcc : -phread
			- D_REENTRANT 매크로 정의가 활성화 된다.


## 3.스레드 간의 데이터 공유
    * 멀티 스레드를 이용하면 모든 스레드가 같은 주소 공간을 가지므로 데이터 공유가 자동적으로 된다.
    * 적절한 관리 메커니즘 없이 스레드가 공유 데이터를 이용하면 데이터 레이스를 발생시킨다.
    * 이를 방지하기 위해 POSIX 표준에서는 다양한 동기화 장치와 데이터 공유 방법을 지원한다.


    1. 뮤텍스 락 ( Mutex )
		* pthread_mutex_t
			- mutex 구조체
		* pthread_mutex_attr
			- mutex 속성을 정의할 수 있는 구조체
		1) 뮤텍스 초기화
			(1) int pthread_mutex_init( pthread_mutex_t *mutex, const pthread_mutex_attr *attr )
				* 반환값
					- 성공 : 0
					- 실패 : errno
			(2) PTHREAD_MUTEX_INITIALIZER 매크로
				* mutex 를 static 하게 선언할 때 사용.
			
		2) int pthread_mutex_destroy( pthread_mutex_t *mutex)
			* mutex 구조체가 가르키는 mutex 리소스 해제.
				- 성공 : 0
				- 실패 : -1
		3) int pthread_mutex_lock( pthread_mutex_t *mutex )
			- lock 대기시 블로킹
		4) int pthread_mutex_trylock( pthread_mutex_t *mutex )
			- lock 획득 실패시 에러코드 리턴 
		5) int pthread_mutex_unlock( pthread_mutex_t *mutex )
    

	2. 뮤텍스 속성 설정 
		*pthread_mutexattr_init( pthread_mutexattr_t *attr )
			- pthread_mutexattr_t 구조체의 값을 디폴트 값으로 채운다.
		1) pthread_mutexattr_setpshared
			(1) PTHREAD_PROCESS_SHARED
				- 프로세스 간의 뮤텍스 공유 가능
			(2) PTHREAD_PROCESS_PRIVATE
				- 프로세스 간의 뮤텍스 공유 불가
		2) int pthread_mutexattr_gettype(const pthread_mutexattr_t *restrict attr,	int *restrict type)
			- 현재 뮤텍스 타입을 가져온다.
		3) int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type)
			- 뮤텍스의 타입을 아래 Type 으로 변경한다.
			- Type
    			(1) PTHREAD_MUTEX_NORMAL
					- deadlock detect 를 하지 못한다.
					- 다른 뮤텍스가 락을 획득 한 상황에서 락을 걸면 deadlock
					- 다른 뮤텍스가 락을 획득한 상태에서 unlock 또는 뮤텍스 락이 획득되지 않은 
					  상태에서 unlock 하면 undefined behavior.
				(2) PTHREAD_MUTEX_ERRORCHECK
					- error checking 을 지원
					- deadlock 인 경우 에러 리턴
					- 다른 뮤텍스가 락을 획득한 상태에서 unlock 또는 뮤텍스 락이 획득되지 않은
					  상태에서 unlock 하면 error 리턴
				(3) PTHREAD_MUTEX_RECURSIVE
					- 처음 뮤텍스 락을 획득한 상태에서 unlocking 없이 다시 뮤텍스 락 획득 가능
					- 뮤텍스 락을 중첩 획득한 만큼 unlock 해야 다른 스레드가 뮤텍스 락 획득 가능
				(4) PTHREAD_MUTEX_DEFAULT
					- default
		4) 그 외 속성
			* int pthread_mutexattr_setprotocol( pthread_mutexattr_t *attr, int protocol ) 
				(1) PTHREAD_PRIO_NONE
					- 우선순위나 스케쥴링이 뮤텍스 소유권에  영향을 주지 않는다.
				(2) PTHREAD_PRIO_PROTECT
					- 
				(3) PTHREAD_PRIO_INHERIT
					-
			* scheduling priority 
				- 효율성을 위해 뮤텍스를 획득한 스레드에 뮤텍스 획득을 시도하는 스레드 중   
				 가장 높은 우선순위를 부여할 수 있다.
			* priority ceiling 
				- 뮤텍스를 획득한 동안만 해당 스레드에 사전에 정해진 최고 우선순위가   
	 			 자동으로 부여되게 할 수 있다.
	

	3. 스핀 락 ( spin lock )
		* 스핀 락은 락 대기 상태에서 계속 락 획득을 시도한다. ( spinning 이라고 한다. )
			- CPU 점유
		* pthread_spin_init( pthread_spinlock_t *lock, int pshared )
			* pshared
				1) PTHREAD_PROCESS_PRIVATE
				2) PTHREAD_PROCESS_SHARED
		* pthread_spin_trylock( pthread_spinlock_t *lock )
			- 락을 획득하지 못할 경우 바로 리턴.
    

	4. 리더스-라이터 락 ( Readers Writer Locks )
		* 읽기 접근은 동시에 여러 스레드를 허용하되, 쓰기 접근은 한 시점에 한 스레드만 허용.
		* 락 획득을 시도하면 뮤텍스 락과 마찬가지로 락을 얻을 때까지 스레드가 블로킹 된다.
		* pthread_rwlock_timewrlock( pthread_rwlock_t *lock, struct timespec time ) &   
		  pthread_rwlock_timerdlock( pthread_rwlock_t *struct timespec time )
			- 지정된 절대 시점까지 락을 얻지 못할 경우 에러 코드 값을 리턴.
    		
	
	5. 배리어
		* 모든 자식 스레드의 작업 완료를 기다린다.
		* pthread_barrier_init( pthread_barrier_t *restrict barrier,   
								const pthread_barrierattr_t *restrict attr, unsigned count )
			1) pthread_barrier_t
				- 초기화할 배리어 객체의 포인터
			2) pthread_barrierattr_t
				- 선택적으로 이용할 수 있는 속성 구조체
			3) unsigned count
				- 완료되기를 기다릴 스레드 개수
		* pthread_barrier_wait( pthread_barrier_t *barrier )
    		- 배리어 초기화 때 설정한 스레드 개수만큼 작업 완료된 스레드 개수가  
			 충족될 때까지 블록킹 된다.
	

	6. 세마포어 ( Semaphore )
		* 수기신호 : 항해 중이거나 항내에 정박 중인 선박끼리 또는 선박과 육지 사이에서   
		 쓰이는 가장 간단한 근거리 통신법
		* 리소스 개수를 카운팅하고 그 값에 기반해 리소스가 가용함을 시그널을 통해 알려준다.
		
		1) unnamed semaphore
			* int sem_init( sem_t *sem, int pshared, unsigned int value )
				- sem_t : 세마포 객체의 포인터
				- pshared : 공유 여부
				- value : 카운터 초기값
		2) named semaphore
			* sem_t *sem_open(const char *name, int oflag, mode_t mode, unsigned int value)
				- name : 세마포 경로
				- oflag : 오픈 속성
					- O_CREAT : 경로에 해단 세마포가 없을 경우 새로 생성 
					- O_EXCL  : 무조건 새로 생성
		* sem_wait( sem_t *sem )
			- 세마포어 카운터 감소
		* sem_post( sem_t *sem )
			- 세마포어 카운터 증가
		* sem_trywait( sem_t *sem )
			- 세마포어 카운터가 ㅇ미 0 이면 에러 값 리턴
		* sem_getvalue( sem_t *Sem )
			- 세마포어 카운터 값 리턴

    7. 조건 변수
		* 스레드 간에 상태 값을 전달할 수 있게 해준다.
		
		1) int pthread_cond_siganl( pthread_cond_t *cond )
			- 대기 중인 소비자 스레드 중 하나만 깨우게 된다.
		2) int pthread_cond_broadcast( pthread_cond_t *cond )
			- 모든 스레드에 웨이크 업 시그널을 보낸다.
		3) int pthread_cond_wait( pthread_cond_t *cond, pthread_mutex_t *mutex )
			- 시그널을 기다리며 대기한다. 대기 모드에 있는 동안 mutex 는 해제된다.
			- 시그널을 받으면 자동으로 mutex 획득한다.
		4) int pthread_cond_timewait( pthread_cond_t *cond, pthread_mutex_t *mutex, 
									  struct timespec time )
			- mutex 를 획득하거나 타임 아웃이 되었을 때 리턴한다.
		
		* lost wake-up
			- pthread_cond_wait 함수를 호출하기 전에 조건 변수가 웨이크 업 시그널을 보내버리면  
			 다음 번에 다시 시그널을 받을 때까지 대기 모드에 있게 된다.

