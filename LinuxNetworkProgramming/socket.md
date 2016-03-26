# Socket ( 소켓 )

## 소켓 도메인
* 소켓으로 통신할 수 있는 범위에 따라서 분류하는 것을 말함
* 분류 기준은 소켓 파이프에 접근 할 수 있는 위치가 어디까지인가.
- 분류
	* Unix domain socket
		- 로컬 유닉스 시스템 내에서만 파이프의 범위가 제한
		- 구현 방식은 시스템 내의 특정 파일에 파이프를 연결하여 데이터를 전송 할 수 있게 한다.
	* network domain socket
		- 네트워크 계층 내에 주소 규칙에 의해서 만들어진 주소로 파이프가 연결되는 방식
		- 원격지에 있는 호스트와 통신할 수 있게 한다.


## 소켓 타입
* 전송할 때 어떤 방법을 사용하여 주고 받을 것인지 결정
* 분류
	- datagram socket
		- 데이터 전송시에 특정 단위로 포장하여 그 만큼을 전송하는 방식
		- 연결을 만들 필요 없이 바로 보낼 수 있다.
			- 연결되있지 않기 때문에 데이터가 목적지까지 도착하는 것을 확인할 수 없다.
			- 연결 생성 및 연결 유지를 위한 오버헤드가 없어 일반적으로 stream socket 보다 빠르다.
	- stream socket
		- 연결되어 있는 흐름으로서 데이터가 전달되는 방식
		- 수신측과의 연결을 필요로 하며, 수신되었는지 확인이 가능하다.
			- 데이터가 목적지까지 도착하는 것을 확인할 수 있고, 
			연결의 상태를 알 수 있기 때문에 신뢰성 있는 통신에 주로 쓰인다.


## Byte Order ( 바이트 순서 )
* 네트워크를 통해서 데이터를 전송할 때 데이터를 나열하는 규칙
* 보통 데이터 전송의 최소 단위는 1 octec, 즉 1Byte 이다.
* 1Byte 를 넘는 데이터를 보낼때 바이트 순서가 사용된다.

* Big Endian
	- 왼쪽으로부터 오른쪽으로 저장되는 방식
	- 네트워크에서 사용되며, TCP/IP 에서 사용중
	- 가독성이 좋다.
* Little Endian
	- 오른쪽으로부터 왼쪽으로 저장되는 방식
	- 포인터 값 참조가 쉽다.

![Endian](https://github.com/martinkang/MyText/blob/master/LinuxNetworkProgramming/img/endian2.png )


## TCP 소켓 ( SOCK_STREAM 소켓 )

> int bind( int sockfd, struct sockaddr *my_addr, socklen_t addrlen );  

* bind(2) : 소켓의 부착
	- 소켓이 외부로부터 연결을 만들 수 있는 접점을 가지게 됨.
	- 네트워크 도메인 소켓이면 지정한 주소에 포트를 가지게 된다.
	- 유닉스 도메인 소켓이면 지정한 경로에 소켓 파일을 만든다.
	

> int listen( int s, int backlog );

* listen(2) : 서버측 연결 큐의 작성
	- 클라이언트로부터 연결을 받기 위한 서버측 포트에 소켓이 부착된다.
	- backlog 를 생성한다.
		- backlog 의 대기 큐가 다차면 클라이언트로부터의 연결을 거부하게 된다.


> int connect( int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen )

* connect(2) : 서버로 연결
	- 클라이언트측에서 connect(2) 가 성공적으로 이루어지면 
	  클라이언트는 connect(2) 함수에 사용된 소켓으로 연결이 만들어지고
	  서버측에서 accept(2) 함수의 리턴값이 해당 클라이언트와 연결된 소켓이 된다.
	- connect(2) 는 bind(2) 를 동반하기 때문에 굳이 bind(2) 를 할 필요가 없다.
		- 클라이언트의 포트는 이 과정에서 지정된다.


> int accept( int s, struct sockaddr *addr, socklen_t *addrlen )

* accept(2) : 클라이언트의 연결 수용


* close(2), shutdown(2) : 연결종료
	- close(2) 는 해당 프로세스 내에서 연결된 소켓의 ID 만 닫는다.
		- 다른 프로세스가 해당 소켓을 공유하고 있을 때, 현재 프로세스에서 소켓을 닫아도
		다른 프로세스에서 소켓을 열고 있다면 소켓의 연결은 살아있게 된다.
		- 열려있는 모든 소켓 ID 가 끊길때까지 연결이 유지된다.
	- shutdown(2) 는 해당 소켓 ID 를 가진 모든 연결이 끊어진다.


### TCO 통신의 연결 설정과 흐름에 대한 이해

![PacketFlow](https://github.com/martinkang/MyText/blob/master/LinuxNetworkProgramming/img/packetflow2.png )

* Active close
	- FIN 세그먼트를 서버로 보내 능동적으로 연결을 종료시키는 것

* Passive close

* TIME_WAIT 상태




## UDP 소켓 ( SOCK_DGREAM 소켓 )


