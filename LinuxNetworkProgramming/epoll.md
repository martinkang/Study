# epoll ( event poll )

## select(2) 와 poll(2)
* 앞서 poll(2) 에서 언급했듯이 select(2) 보다는 poll(2) 이 더 좋다.
	- 하지만 검사할 파일 기술자가 적은 경우는 그 차이가 크지 않다.
* poll(2) 은 적은 개수의 ( 일반적으로 500개 미만 ) 파일 기술자에서는 그 성능이 괜찮음
	- 관리하는 파일 기술자의 개수가 커지면 커질수록 비선형적으로 응답 속도가 느려진다.
	- 앞에서 언급한 C10K 문제가 거론되면서 이를 개량할 필요성이 대두됨
	- 실제로는 UNIX 시스템에서 poll(2) 을 많이 쓰기 때문에 호환성을 위해서 poll(2) 배워둬야함


## epoll(2)
* level trigger 와 edge trigger 모두 지원함
	- default : level trigger
	- 레벨 트리거는 소켓 버퍼에 데이터가 있는가( 버퍼의 레벨 ) 을 정확하게 분별하므로
	데이터의 일부를 읽어 들이는 경우에도 아직 버퍼에 데이터가 남아 있으면 다음 번 select/poll
	을 호출할 때 여전히 버퍼에 데이터가 있으므로 다시 보고함
	- epoll 의 엣지 트리거는 이전 상태에서 변화가 있는가를 검사하므로, 데이터의 일부만 읽어들여도
	다음 번 epoll 호출에서 보고하지 않음.
		- epoll 이 감지한 마지막 상태 전위에서 변화가 없으므로 다시 새로운 데이터가 도착하여
		상태가 변할 때까지 엣지 트리거는 감지하지 않는다.
	- 따라서 TCP 통신에서 스트림 통신에 데이터가 불충분하게 도착했다면 다시 epoll 을 호출하여
	데이터가 더 도착하기를 기다리게 할 수 있다.
		- 엣지 트리거는 상태 전위만 감시하므로 미리 버퍼를 비워주지 않아도 다시 데이터가 더 도착하기 전에는
		트리거 이벤트를 발생하지 않으므로 프로그래밍이 더 간단해진다.


### 엣지 트리거의 이점
* 엣지 트리거를 사용하면 전체적으로 프로세스에 비동기적인 부분을 많이 적용할 수 있다.
	- 예를들어 기존의 select(2) 나 poll(2) 에서는 poller 에서 리스너 소켓에 접속 요청이 있는 경우
	이를 다 처리하기 전에는 다시 poller 를 호출하지 않았다.
		- 레벨 트리거에서는 상태 변화가 아닌 접속 요청이 있었나 판단하기 때문에 리스너 소켓에
		접속 요청을 모두 받아들이기 전에 다시 select(2) 나 poll(2) 을 호출하면
		미처리된 접속 요청에 의해서 다시 함수가 리턴된다.
		- 엣지 트리거의 경우 미처리된 접속 요청이 있어도 바로 다시 epoll 을 호출할 수 있으므로
		비동기적으로 처리가 가능하다.
			- 접속과 처리 등을 비동기적으로 멀티 쓰레드를 이용하여 작업할 수 있다.


## epoll API
* int epoll_create( int size )
	- epoll 파일 기술자를 생성한다
		- 소켓의 파일 기술자와 다르다.
		- epoll 파일 기술자는 epoll 을 구분할 수 있는 하나의 ID 면서 epoll 이 관리하는
		소켓 파일 기술자 공간을 가리킨다.
			- IPC 에서 IPC id 와 유사하다.
			- epoll 은 상태가 커널에 저장되므로, 이 상태 리스트를 관리하기 위하여 ID 가 필요하다.
	- epoll 은 관리할 파일 기술자들을 위해서 메모리 공간을 미리 만들어 둔다.
	- size : 관리할 파일 기술자의 최대값
* int epoll_ctl( int dpfd, int op, int fd, struct epoll_evnet *event )
	- epoll 에(소켓) 파일 기술자와 이벤트를 등록하거나 변경, 제거한다.
	* epoll_ctl(2) 함수의 op 인수
		- EPOLL_CTL_ADD
			- 해당 파일 기술자와 이벤트를 epoll 에 등록
		- EPOLL_CTL_DEL
			- 해당 파일 기술자를 epoll 에서 제거
		- EPOLL_CTL_MOD
			- 해당 파일 기술자에 해당하는 이벤트를 epoll 에서 수정
* int epoll_wait( int epfd, struct epoll_event *events, int maxevents, int timeout )
	- epoll 의 실제 poller 이벤트 함수로서 이벤트를 기다리며 블록한다.
	- timeout : 음수읠 경우 무한 대기이며 0 일 경우 넌블럭킹
	- epoll이 관리하는 파일 기술자들 중 이벤트가 발생한 파일 기술자 수를 리턴한다.
	- stateful 함수이므로 events 인수에는 단지 이벤트가 발생한 경우에만 반환한다.
		- 만일 리턴값이 3 이라면 events[0], events[1], events[2] 까지만 이벤트를 검사하면 된다.
* close( int fd )
	- 기존의 소켓이나 파일처럼 프로그램 종료시 닫히지 않기 때문에 epoll 기술자를 꼭 닫아줘야 한다.


### epoll_ctl
```c++
int epoll_ctl( int dpfd, int op, int fd, struct epoll_evnet *event )

typedef union epoll_data
{
	void *ptr;
	int fd;
	_uint32_t u32;
	_uint64_t u64;
} epoll_data_t;

struct epoll_event
{
	_uint32_t events; /* Epoll evnets */
	epoll_data_t data; /* User data variable */
}
```


### epoll 의 이벤트 종류
* epoll 의 이벤트는 기존 poll 에 E 접두어가 붙었을 뿐이다.
* POLLNVAL 이라는 이벤트는 epoll 에서 사라졌다.
	- 잘못된 파일 기술자는 epoll 파일 기술자에 등록조차 안되기 때문에 해당 이벤트는 발생할 수 없다.
* 종류
	* EPOLLIN
		- 데이터가 읽기 버퍼에 존재하는 경우
		- TCP 의 연결 요청도 읽기 데이터에 포함
	* EPOLLPRI
		- OOB 데이터로 사용되는 URG 플래그가 감지됨
	* EPOLLOUT
		- 쓰기 버퍼가 사용 가능한 상태
		- 버퍼가 비어졌거나 넌블록킹 연결이 완료되어 쓰기 가능한 상태가 됨
	* EPOLLERR
		- 연결 에러가 발생
	* EPOLLHUP
		- 닫힌 연결에 쓰기 시도 감지
	* EPOLLONESHOT
		- 이벤트가 한번 설정( 감지 ) 된 후에는 해당 파일 기술자의 이벤트 마스크를 비활성화 시킨다.
		- 따라서 해당 파일 기술자는 다시 epoll_ctl(2)로 이벤트 마스크를 재설정할 대까지( EPOLL_CTL_MOD 행동 )
	이벤트를 받지 못한다.
	* EPOLLET
		- 이벤트를 엣지 트리거로 작동

