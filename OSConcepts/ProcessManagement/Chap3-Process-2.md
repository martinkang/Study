## 3.4 프로세스간 통신 ( Interprocess Communication : IPC )
* 용어
	* IPC
		- 프로세스들 사이에 데이터와 정보를 교환하는 방법
	* 독립적 프로세스
		- 시스템에서 실행 중인 다른 프로세스들에게 영향을 주거나 받지 않는 프로세스
	* 협력적 프로세스
		- 시스템에서 실행 중인 다른 프로세스들에게 영향을 주거나 받는 프로세스.
* IPC 의 사용 이유
	- 정보 공유
		- 정보를 병행적 접근 가능
	- 계산 가속화
		- 하나의 계산을 병렬화
	- 모듈성
		- 시스템 기능을 별도의 프로세스, 스레드로 나누어 모듈식 형태 구성
	- 편의성
		- 동시에 서로 다른 작업 병렬화

##### IPC
![3.13](https://github.com/martinkang/Study/blob/master/OSConcepts/ProcessManagement/img/chap3-ipc.png)


### 3.4.1 공유 메모리 시스템
* 협력 프로세스들에 의해 공유되는 메모리 영역이 구축되어 이 영역에 데이터를 읽고 씀으로서 정보를 교환
* 공유 메모리 영역은 공유 메모리 세그먼트를 생성하는 프로세스의 주소 공간에 위치.
	- 이 공유 메모리 세그먼트를 이용하여 통신하고자 하는 프로세스들은 이 세그먼트를 자신의 주소 공간에 추가해야 한다.
	- 커널에 의해 관리된다.
* 장/단점
	- 장점
		- 최대 속도와 편의를 제공
			- 일반적으로 메시지 전달 시스템은 시스템 콜을 호출하지만 공유 메모리는 공유 메모리 생성 시에만 
			시스템 콜을 호출
			- 자신의 로컬 변수에 접근하듯 접근 가능
	- 단점
		- 동기화 문제

### 3.4.2 메시지 전달 시스템
* 메시지를 통하여 정보를 교환
* 장/단점
	- 장점
		- 구현이 쉽다.
		- 동기화 문제가 발생하지 않고 충동을 회피할 필요가 없어 적은 양의 데이터 교환시 유리
	- 단점
		- 연결을 유지 관리 하기 위한 비용
* 메시지 전달 시스템의 논리적 구현 방법
	- 직접 / 간접 통신
	- 동기식 / 비동기식 통신
	- 자동 / 명시적 버퍼링

#### 3.4.2.1 Naming
* 직접 통신
	- 각 프로세스는 통신의 수신자 또는 송신자의 이름을 명시해야 한다.
	- 연결은 정확히 두 프로세스 사이에만 연관된다.
	- 단점은 프로세스를 정확히 지정함으로써 모듈화를 제한한다.
		- 프로세스의 이름을 바꾸면 모든 다른 프로세스 지정부분의 검사가 필요하다.
* 간접 통신
	- 메시지들이 메일박스 또는 포트로 송신되고, 그것으로 부터 수신한다.
	- 연결은 두 개 이상의 프로세스들과 연관될 수 있다.

#### 3.4.2.2 Synchronization
* 봉쇄형 송신
	- 송신 프로세스의 메시지를 수신 프로세스가 수신하거나, 메일박스에 수신될 때까지 봉쇄
* 비봉쇄형 송신
	- 송신 프로세스가 메시지를 보내고 작업을 재시작
* 봉쇄형 수신
	- 메시지가 이용 가능할 때까지 수신 프로세스가 봉쇄
* 비봉쇄형 수신
	- 수신하는 프로세스가 유효한 메시지 또는 null 을 받는다.

#### 3.4.2.3 Buffering
* Zero Capacity
	- 무용량, 큐의 최대 길이가 0 이다.
	- 링크는 자체 안에 대기하는 메시지들을 가질 수 없다.
	- 송신자는 수신자가 메시지를 수신할 때까지 기다려야 한다.
	- 버퍼링하지 않는 메시지 시스템이라 한다.
* Bounded Capacity
	- 큐가 유한한 길이 n 을가진다.
	- 최대 n 개의 메시지가 들어있을 수 있다.
	- 큐가 만원이면 송신자는 큐 안에 가용한 공간이 생길 때까지 반드시 봉쇄되어야 한다.
* Unbounded Capacity
	- 큐는 잠재적으로 무한한 길이를 가진다.
	- 송신자는 결코 봉쇄되지 않는다.
## 3.6 클라이언트 서버 환경에서 통신

### 3.6.1 소켓 ( Socket )
* 소켓 ( Socket )
	- 통신의 극점 ( endpoint ) 을 뜻한다.
	- 두 프로세스가 네트워크상에서 통신을 하기 위한 프로세스 간 통신의 극점.
	- 1024 미만의 포트들은 well-known 포트로 간주되며, 표준 서비스를 구현하는데 사용할 수 있다.
* 소켓을 이용한 통신은 저수준이다.
	- 스레드들 간에 구조화 되지 않은 바이트 스트림만을 통신하도록 하기 때문
	- 이러한 원시적 바이트 스트림 데이터를 구조화하여 해석하는 것은 클라이언트와 서버의 책임
* Loopback
	- 전기 신호의 라우팅, 디지털 데이터의 스트림 등이 의도적 가공이나 수정없이 원래의 장치나 장비로 돌아가는 것

##### 소켓을 사용한 통신
![3.18]( http://www.cs.odu.edu/~cs471w/spring11/lectures/Processes_files/image019.jpg )
- 출처 http://www.cs.odu.edu/~cs471w/spring11/lectures/Processes.htm


### 3.6.2 원격 프로시저 호출 ( Remote Procedure Calls : RPC )
* 네트워크에 연결되어 있는 두 시스템 사시의 통신에 사용하기 위하여 프로시저 호출을 추상화 하기 위한 방편
	- 프로세스들이 서로 다른 시스템에서 실행되기 때문에 원격 서비스를 제고아기 위하여
	메시지 기반 통신을 해야 한다.
* IPC 와 달리 RPC 통신에서 전달되는 메시지는 구조화 되어 있고 따라서 데이터의 패킷 수준을 넘어서게 된다.
	- 각 메시지에는 원격지 퐅에서 listen 중인 rpc deamon 의 주소가 지정되어 있고
	실행되어야 할 함수의 식별자, 그 함수에게 전달되어야 할 매개변수가 포함
		- 요청된 함수가 실행되고 어떤 출력이든지 별도의 메시지를 통해 요청자에게 반환
* Stub
	- 클라이언트가 자기의 프로시저를 호출하는 것처럼 원격 홒스트의 프로시저를 호출할 수 있도록 해주는 기능
	- RPC 시스템은 클라이언트 쪽에 Stub 을 제공하여 통신을 하는 데 필요한 자세한 사항을 숨겨준다.
	- 원격 프로시저 마다 다른 스텁이 존재한다.
* 과정
	- 클라이언트가 원격 프로시저를 호출하면 RPC 는 그에 대응하는 Stub 을 호출하고 프로시저가 필요로 하는 매개변수를 건네준다.
		- Stub 이 원격 서버의 포트를 찾고 매개변수 정돈 ( parameter marshalling ) 을 수행
		- parameter marshalling
			- 프로시저에게 갈 매개변수를 네트워크로 전송하기 위해 적절한 형태로 재구성 하는 작업
	- Stub 은 메시지 전달 기법을 사용하여 서버에게 메시지를 전송
	- 이에 대응하는 Stub 이 서버에도 존재하여 서버 측 Stub 이 메시지를 수신한 후 적절한 서버의 프로시저를 호출

##### RPC 호출의 실행
![3.21](https://github.com/martinkang/Study/blob/master/OSConcepts/ProcessManagement/img/chap3-rpc.jpeg)
- 출처 OS System Concepts 3장

#### 최대 한 번 기능성
* 각 메시지에 타임스탬프를 매기는 것으로 보장.
	- 서버는 이미 처리한 모든 메시지의 타임스탬프 기록을 갖거나 중복된 메시지를 검사해 낼 수 있을 만큼의 기록을 가져야 한다.
	- 기록에 보관된 타임스탬프를 가진 메시지가 도착하면 그 메시지는 무시.
	- 이렇게 하면 클라이언트는 한 번 이상 메시지를 보낼 수 있고 메시지에 대한 실행이 단 한번 실행된다는 것을 보장 받을 수 있다.


#### 정확히 한 번 기능성
* 서버가 요청을 받지 못하는 위험을 제거할 필요가 있다.
	- 최대 한번 프로토콜을 구현하고 추가로 응낙 ( acknowledgement : ACK ) 메시지를 보내야 한다.
	- 클라이언트는 해당 호출에 대한 ACK 를 받을 때까지 주기적으로 RPC 호출을 재전송 한다.

#### XDR ( external data representation )
* 기종 중립적인 데이터 표현 방식 ( Big/Little Endian 중립적 )

#### RPC 에서 클라이언트가 서버의 포트번호를 어떻게 알 수 있을까?
* 고정된 포트 주소 형태 주소 형태로 미리 정해 놓은 방법 
	- 임의로 포트를 바꿀 수 없다.
* 미리 정해져 있는 고정 RPC 포트를 통해 랑데부용 디먼 ( matchmaker ) 을 제공
	- 자신이 실행하기를 원하는 RPC 이름을 matchmaker 로 전송하여, RPC 에 대응하는 포트 번호를 요청

### 3.6.3 파이프 ( Pipes )
* 두 프로세스가 통신할 수 있게 하는 전달자로서 동작
* 용어
	- 반이중 ( half duplex )
		- 한 순간에 한 방향만 전송 가능
	- 전이중 ( full duplex )
		- 동시에 양방향 전송 가능

#### 3.6.3.1 anoaymous pipe
* 반이중 통신
* 파이프를 생성한 프로세스 의외에는 접근할 수 없다.
	- 자식 프로세스와 통신하기 위해 사용
	- 파이프는 파일의 특수한 유형이기 대문에 자식 프로세스는 부모로부터 상속받는다.

##### anoaymous pipe 를 위한 File Descriptor
![3.22](http://www.cs.odu.edu/~cs471w/spring11/lectures/Processes_files/image021.jpg)
- 출처 http://www.cs.odu.edu/~cs471w/spring11/lectures/Processes.htm

#### 3.6.3.2 named pipe
* 반이중 전송만이 가능하다.
* 다수의 프로세스와 통신할 때 사용
* UNIX 에서는 FIFO 라고 칭한다.
* 파일의 형태로 존재하며 프로세스와 전혀 관련이 없기 때문에, 프로세스와 관련없이 독자적인 파일의 형태로 존재하게 할 수 있다
