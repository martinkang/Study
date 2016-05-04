# poll(2) 의 사용

## poll(2) 의 사용
* poll(2) 는 자신이 관리하는 파일 기술자 번호를 struct pollfd 구조체에 넣어서 관리하므로,
	select(2) 처럼 쓸데없이 감시하지 않는 파일 기술자까지 루프 문에서 검사할 필요가 없다.
	- 하지만 기본적으로 단번에 입출력 이벤트가 발생한 파일 기술자를 알수 없으므로
	루프 문을 돌면서 각 파일 기술자에 대해서 일일히 이벤트가 발생했는지 검사해야 한다.
	- select(2) 보다 루프의 횟수가 줄어든다.
* select(2) 를 개량하여 나온 기능, 성능상의 약간의 이점이 있지만 절대적으로 말할 정도는 아니다.
	- 하지만 될 수 있으면 select(2) 보다는 poll(2) 을 쓰는 것이 좋다.

##### poll(2) 의 함수 원형과 파일 기술자 구조체
```c++
int poll( struct pollfd *ufds, unsigned int nfds, int timeout );

struct pollfd
{
	int fd;         /* 파일 기술자   */
	short events;   /* 요구된 이벤트 */
	short revents;  /* 반환된 이벤트 */
}
```
* 구조체
	* int fd ( 파일 기술자 )
		- 감시할 파일 기술자 번호.
	* short evnets ( 요구된 이벤트 )
		- 이벤트의 종류에 따라서 일기간 쓰기, 예외 상황등을 감시하도록 비트 마스크를 지정.
		- 비트 마스크에 지정되지 않은 이벤트는 poll(2) 함수가 감지하지 않음.
	* short revents( 반환된 이벤트 )
		- 지정한 이벤트 중에 발생한 것이 저장됨

##### poll(2)의 이벤트 종류
* 종류
	* POLLIN
		- 데이터가 일기 버퍼에 존재하는 경우
			* TCP 의 연결 요청도 읽기 데이터에 포함
	* POLLPRI
		- 우선순위 데이터를 사용
			- TCP 의 OOB 데이터 감지
	* POLLOUT
		- 쓰기 버퍼가 사용 가능한 상태
			- 버퍼가 비워졌거나 넌블록킹 연결이 완료되어 쓰기 가능한 상태가 됨
	* POLLERR
		- 연결 에러가 발생
	* POLLHUP
		- 닫힌 연결에 쓰기 시도 감지
	* POLLNVAL
		- 무효한 파일 기술자를 지정한 에러
			- 연결되지 않은 파일 기술자를 지정함
* 에러 처리
	* POLLERR, POLLHUP, POLLNVAL
		- struct pollfd 구조체의 events 멤버에 지정해주지 않더라도, 수신 이벤트가 저장이 되어지는
		revents 전용의 멤버들
		- 따라서 events 에 쓸 수 없다.


## poller 를 이용한 TCP 의 OOB( Out Of Band ) 데이터 감지
* TCP 에서 OOB 데이터가 도착하는 것을 감지하기 위해서는 두가지 방법이 있다.
	- poller 를 이용하여 예외 상황을 감지하는 방법
		- poll(2) 의 이벤트중 POLLPRI 는 OOB 데이터를 감지한다.
		- URG 플래그가 세팅된 데이터를 받았는지 감지한다.
	- SIGURG 을 감시하는 방법.

* 예제
	= ex/io_poll.c

