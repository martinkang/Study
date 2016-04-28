# select, pselect 의 사용
* select(2), pselect(2) 는 레벨 트리거를 사용하는 방식으로 버퍼의 수준 ( level = amount ) 을 검사하는 방식.
	* select(2) 는 실무에 사용하는데 사실상 조금 무리가 따른다.
		- 자신이 관리하는 파일 기술자 번호를 외부에 저장해두고 검사할 때마다 루프를돌면서 검사해야 한다.
		- 내부적으로는 검사할 가장 큰 수를 가지는 파일 기술자 + 1 을 입력받아서 검사하므로 내부적으로도 쓸모없는 루프문을 돌도록 구성되어 있다.
		- 이런 제약점이 존재하기 때문에 실제로는 간단한 코드나 혹은 디바이스 장치 몇개를 감시하는 경우에만 주로 사용된다.
	- 레벨 트리거 ( Level Trigger )
		- 인터럽트가 발생하여 그 레벨이 인가 될 때 발생.  ( 사건이 발생하고 있는 동안 발생 )
		- 하이 레벨, 로우 레벨 두 경우가 있음
	- 엣지 트리거 ( Edge Trigger )
		- 인터럽트가 발생하여 레벨이 상승하거나 하강 할 때 발생. ( 사건이 발생하는 찰나에 발생  )
		- 상승 엣지와 하강 엣지 두 경우가 있음.

## 함수 원형
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

## select(2) 와 pselect(2) 의 차이
- 타임 아웃
	* select 는 타임아웃을 마이크로초 ( 10E-6 ) 초 단위까지 지정 가능
	* pselect 는 타임아웃이 struct timespec 구조체로 바뀌어 나노 ( 10E-9 ) 초 단위까지 지정 가능.
- 시그널 핸들러의 사용
	* select 는 블록킹 중에 시그널이 발생하면 에러로 리턴하고 빠져나가기 때문에 
	시그널 핸들러를 사용할 때 전역적인 시그널 블록킹을 같이 해줘야 한다.
	* pseelct 는 함수 호출시에 블록킹할 시그널 마스크를 지정하여 시그널을 지연시킬 수 있다.

