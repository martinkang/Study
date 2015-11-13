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
			- __thread 지정자를 붙이는 것으로 해당 변수에 대해 스레드 별로 복제본을 생성해 스레드 간에 공유가 안되면서   
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
void ppp( void )                                                                                                                     
{
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
		1. Shared Memory
		2. mmap
	2. 프로세스 간의 세마포어 공유
		1. unnamed semaphore
		2. named semaphore
	3. 메시지 큐
		-
	4. 일반 파이프와 지정 파이프
		1. unnamed pipe
		2. named pipe
	5. 시그널을 이용한 프로세스 간 커뮤니케이션
		- 


## 6. 소켓의 이용
	1. TCP/IP
	2. UDP/IP


## 7. 재진입 가능한 코드와 컴파일러 플래그
	* 플랫폼과 컴파일러에 따라 멀티 스레드 애플리케이션을 컴파일 할 때 특별한 플래그를 설정할 필요가 있다.
		- gcc : -pthread
		- solais : -mt
