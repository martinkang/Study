# 서론

## 운영체제의 정의

##### 컴퓨터 시스템 구성요소에 대한 개략적 구성도
![1.1](http://www.cs.odu.edu/~cs471w/spring10/lectures/introduction_files/image002.jpg)
- 출처 http://www.cs.odu.edu/~cs471w/spring10/lectures/introduction.htm

## 컴퓨터 시스템의 구성

##### 현대의 컴퓨터 시스템
![1.2](http://www.cs.odu.edu/~cs471w/spring10/lectures/introduction_files/image004.jpg)
- 출처 http://www.cs.odu.edu/~cs471w/spring10/lectures/introduction.htm
* 현대의 범용 컴퓨터는 공용 메모리에 대한 접근을 제공하는 공통 버스를 통해 연결된 여러개의
장치 제어기와 하나 이상의 CPU 로 구성되어 있다.
	- CPU 와 장치 제어기는 메모리 사이클을 얻기 위해 경쟁하면서 병행 실행된다.

#### 용어
* 인터럽트 ( Interrupt )
		- 하드웨어나 소프트웨어에서 어떤 사건이 발생했음을 알리는 신호
			- 하드웨어는 어느 순간이던 시스템 버스를 통해 CPU 에 신호를 보내 인터럽트 발생 가능
			- 소프트웨어는 System Call 을 통해 인터럽트 발생
		- CPU 가 인터럽트 되면 하던 일을 중지하고, 즉시 고정된 위치로 실행을 옮긴다.
			- 인터럽트가 완료되면 전에 하던 연산을 재개

##### 저장장치 계층
![1.4](http://www.cs.odu.edu/~cs471w/spring10/lectures/introduction_files/image007.jpg)
- 출처 http://www.cs.odu.edu/~cs471w/spring10/lectures/introduction.htm

##### 현대 컴퓨터 시스템의 동작
![1.5](http://www.cs.odu.edu/~cs471w/spring10/lectures/introduction_files/image009.jpg)
- 출처 http://www.cs.odu.edu/~cs471w/spring10/lectures/introduction.htm

##### 사용자 모드에서 커널 모드로의 전환
![1.10](http://www.cs.odu.edu/~cs471w/spring10/lectures/introduction_files/image011.jpg)
- 출처 http://www.cs.odu.edu/~cs471w/spring10/lectures/introduction.htm



