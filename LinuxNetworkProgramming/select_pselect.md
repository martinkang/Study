# select(2), pselect(2) 의 사용
* 특징
	* select(2), pselect(2) 는 레벨 트리거를 사용하는 방식으로 버퍼의 수준 ( level = amount ) 을 검사하는 방식.
	* select(2) 는 실무에 사용하는데 사실상 조금 무리가 따른다.
		- 자신이 관리하는 파일 기술자 번호를 외부에 저장해두고 검사할 때마다 루프를돌면서 검사해야 한다.
		- 내부적으로는 검사할 가장 큰 수를 가지는 파일 기술자 + 1 을 입력받아서 검사하므로 내부적으로도 쓸모없는 루프문을 돌도록 구성되어 있다.
		- 이런 제약점이 존재하기 때문에 실제로는 간단한 코드나 혹은 디바이스 장치 몇개를 감시하는 경우에만 주로 사용된다.
* 용어
	- 레벨 트리거 ( Level Trigger )
		- 인터럽트가 발생하여 그 레벨이 인가 될 때 발생.  ( 사건이 발생하고 있는 동안 발생 )
		- 하이 레벨, 로우 레벨 두 경우가 있음
	- 엣지 트리거 ( Edge Trigger )
		- 인터럽트가 발생하여 레벨이 상승하거나 하강 할 때 발생. ( 사건이 발생하는 찰나에 발생  )
		- 상승 엣지와 하강 엣지 두 경우가 있음.

## select(2), pselect(2) 의 사용
> int select( int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout );    
> int pselect( int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, const struct timespec *timespec, const sigset_t *sigmask );     

* fd_set    
	* readfds ( 읽기 가능 이벤트 감시 )   
		- 소켓 수신 버퍼에 데이터가 도착한 경우에 리턴      
		- 소켓 수신 버퍼에 접속 연결 해제 요청 ( FIN ) 이 발생한 경우에 리턴   
		- 리스터 소켓의 경우 새로운 접속 ( SYN 요청 ) 이 있는 경우에 리턴   
		- 스트림으로부터 데이터가 도착하였을 경우에 리턴 ( e.g. stdin 키보드 입력 발생 )    
	* writefds ( 쓰기 가능 이벤트 감시 )    
		- 송신 소켓 버퍼에 빈 공간이 생긴 경우에 리턴    
		- 반태편 peer 가 끊어진 경우에 리턴     
		- 스트림으로부터 데이터가 전송 가능한 경우에 리턴 ( e.g 넌블록킹 connect(2) 호출 )  
	* exceptfds ( 예외 상황 이벤트 감시 )     
		- TCP 의 OOB 데이터 ( URG 플래그 지정됨 ) 가 수신된 경우에 리턴    

> FD_ZERO( fd_set * set )  
> 	* set 을 초기화 한다.
> FD_SET( int fd, fd_set * set )  
> 	* 파일 기술자 세트인 set 에 파일 기술자 fd 를 더한다.
> FD_CLR( int fd, fd_set * set )  
> 	* 파일 기술자 세트인 set 에 파일 기술자 fd 를 뺀다.
> FD_ISSET( int fd, fd_set * set )	    
>	* 파일 기술자 세트인 set 에서 파일 기술자 fd 에 이벤트가 발생하는 지를 체크한다.   
>	* fd 에 이벤트가 발생하면 1이 리턴, 이벤트가 없으면 0 이 리턴된다.

* 파일 기술자 세트들이 select(2) 를 호출하여 리턴받은 다음에는 변경된다.
	- select(2) 는 함수가 이벤트를 반환하기 위해서 해당 파일 기술자 세트들에 변경을 가하게 된다.
		- 이벤트가 발생한 파일 기술자 비트가 1 로 변경된다.
	- 그러므로 파일 기술자들을 따로 관리해야 한다.


##### fd_set 구조체 ( sys/select.h>
```c++
#define __NFDBITS	( 8 * sizeof( __fd_mask ) ) /* 즉 32 */
typedef long int __fd_mask;
typedef struct
{
	/* XPG4.2 requires this memeber name. otherwise avoid the name from the global namespace. */

#ifdef __USE_XOPEN
	__fd_mask fds_bits[__FD_SETSIZE / __ NFDBITS]; /* 1024 / 32 즉 32 개의 배열로 이루어져 있다. */
#define __FDS_BITS(set) ((set)->fds_bits)
#else
	__fd_mask __fds_bits[__FD_SETSIZE / __NFDBITS];
#define __FDS_BITS(set) ((set)->__fds_bits)
#endif
} fd_set;

#define FD_SETSIZE __FD_SETSIZE /* 다른 헤더에 1024 로 정의되어 있다. *.
```
* fds_bits 는 32 개의 long int 배열로 이루어져 있다.
	- 각각의 비트가 한 개의 파일 기술자를 의미하므로 최대 1024 번호까지의 파일 기술자를 감시할 수 있다.

## select(2) 와 pselect(2) 의 차이
- 타임 아웃
	* select 는 타임아웃을 마이크로초 ( 10E-6 ) 초 단위까지 지정 가능
	* pselect 는 타임아웃이 struct timespec 구조체로 바뀌어 나노 ( 10E-9 ) 초 단위까지 지정 가능.
- 시그널 핸들러의 사용
	* select 는 블록킹 중에 시그널이 발생하면 에러로 리턴하고 빠져나가기 때문에 
	시그널 핸들러를 사용할 때 전역적인 시그널 블록킹을 같이 해줘야 한다.
	* pseelct 는 함수 호출시에 블록킹할 시그널 마스크를 지정하여 시그널을 지연시킬 수 있다.


## 왜 select(2) 함수가 비효율 적인가?
- 감시해야할 파일 기술자가 4번과, 800 번이라고 할 때
	- select(2) 는 0 부터 800 번까지 항상 루프를 돌면서 fd_Set 구조체의 비트마스크 상태를 검사하게 된다.
	- 단 2개의 파일 기술자를 감시하기 위해 800 번까지 루프를 돌기 때문에 의미없이 CPU 시간을 잡아먹는다.
- blocking accept(2) 사용 시 한 번에 하나의 accept 만 하기 때문에 비효율적이다.
	- 이는 select 뿐만 아니라 poll/epoll 모두 해당된다.


## 왜 blocking accept 는 비효율 적인가?
* 기존 blocking accept 의 처리 순서
    1. I/O 멀티플렉서 ( select/poll/epoll ) 의 읽기 요청 감지
    2. 리스너 소켓 ( listener socket ) 에 읽기 요청 ( 연결 요청 ) 이 있었는가?
    3. accept(2) 한번 한 뒤에 다시 (1) 번으로.

* 위의 과정에서 한번에 한 개씩만 accept(2) 하는 이유는 몇 개의 연결 요청이 있었는지 알 수 없기 때문이다.
    - 만약 루프문을 넣어 여러 개의 accept(2) 요청을 처리하도록 할 경우
        * 3 개의 연결 요청시 3개의 요청 accept 후 4 번째 accept 를 하게 되어 블록킹 된다.
        * 요청이 얼마나 올지 모르기 때문에 잠정적으로 무한 대기에 빠질 수 있다.
    - 따라서 poller( select/poll/epoll ) 가 한번 감지한 뒤에 접속을 한번 accept 하고
    다시 poller 를 호출하기 때문에 잦은 시스템 호출로 인해서 성능상 불이익이 생긴다.

* 예제
	- [io_select-blocking-accept.c](https://github.com/martinkang/Study/tree/master/LinuxNetworkProgramming/ex/io_select-blocking-accept.c )
	- ex/io_select-non-blocking-accept.c
	- ex/client.c
