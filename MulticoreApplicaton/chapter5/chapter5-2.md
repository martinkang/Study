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
		- pointer alias 가 없음을 한정해줌 -> 컴파일러 최적화
	4. Thread Private Data
		- 스택에 저장된 데이터.
		- Thread Local Storage( TLS )
			- __thread 지정자를 붙이는 것으로 해당 변수에 대해 스레드 별로 복제본을 생성해 주어스레드 간에 공유가 안되면서  
			같은 스레드 컨텍스트 하에서 실행되는 코드에는 전역 변수로 보이게 해준다.
		- pthread_key_create() & pthread_key_delete()
			* posix 에서 지원하는 스레드 로컬 변수
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
	*mem = 300;

	pthread_setspecific( pkey, mem );

	*mem = 400;
	ppp();
}
```
## 5. 멀티 프로세스 프로그래밍
	1. 프로세스 간 메모리 공유
	2. 프로세스 간의 세마포어 공유
	3. 메시지 큐
	4. 일반 파이프와 지정 파이프
	5. 시그널을 이용한 프로세스 간 커뮤니케이션

## 6. 소켓의 이용

## 7. 재진입 가능한 코드와 컴파일러 플래그
	 
