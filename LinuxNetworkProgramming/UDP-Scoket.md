# UDP 소켓의 기초( SOCK_DGRAM 소켓 )
* UDP 는 TCP 와는 다르게 소켓의 연결 및 관리 과정이 없다.
	- 따라서 실시간성이나 일회성이 요구되는 서비스에서 많이 사용된다.
* UDP 는 서버/클라이언트 관계보다는 송신측, 수신측이라고 부른다.
	- 연결이 존재하지 않기 때문에 수동적/능동적에 따라서 서버/클라이언트로 나눌 수 없기 때문
![UDP socket fuction flow](https://github.com/martinkang/Study/blob/master/LinuxNetworkProgramming/img/udp.jpg)


## UDP 소켓의 장단점
* 장점
	- 소켓의 연결 및 관리 과정이 없기때문에 실시간성을 요구하는 데이터인 음성/화상 데이터에 많이 이용된다.
	- 브로드캐스트, 멀티캐스트와 같은 기능으로 한번의 전송으로 다수의 호스트에게 데이터를 보낼 수 있다.
* 단점
	- 전송이 제대로 되었는지 확인하지 않고 재전송 메커니즘도
	존재하지 않기 때문에 신뢰성이 떨어진다.


## UDP 소켓 제어 및 연결 제어 함수
* 함수
	* bind(2)
		- 소켓을 시스템에 부착, 소켓은 이때부터 외부로부터의 연결점을 가진다.
	- UDP 는 연결할 필요가 없으므로 서버측에서는 bind(2) 를 하게 되면
	클라이언트로부터 데이터를 읽어 들일 수 있다.
	* close(2)
		- 해당 프로세스 내에서 소켓 ID 를 닫는다.
	* shutdown(2)
		- 연결된 모든 소켓을 닫는다.


## 데이터 입출력 함수
* 함수
	* recvfrom(2)
		- UDP 소켓을 통해 데이터를 읽어 들인다.
		- 데이터를 받을 때 함수의 인수중 하나인 struct sockaddr 포인터에
		클라이언트측의 주소가 저장된다.
	* sendto(2)
		- UDP 소켓을 통해 데이터를 보낸다.
		- UDP 소켓은 연결이 없으므로 클라이언트에서 서버로 보낼때
		목적지의 주소와 포트를 적어서 보내야 한다.

## sendto(2), recvfrom(2) 함수의 사용
```c++
int sendto( int s, const void *msg, size_t len, int flags,
		const struct sockaddr *to, socklen_t tolen );

int recvfrom(int s, void *buf, size_t len, int flags,
		struct sockaddr *from, socklen_t *fromlen );
```
* sendto(2)
	- struct sockaddr *to
		- 메시지를 받을 목적지 주소
* recvfrom(2)
	- struct sockaddr *from
		- 메시지를 보낸 송신지의 주소

## 예제
* UDP 전송 예제
	- [udp_send.c]()
* UDP 수신 예제
	- [udp_`recv.c]()
