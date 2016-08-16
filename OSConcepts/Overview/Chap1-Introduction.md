# 1. 서론

## 운영체제의 정의
* 컴퓨터의 기동과 함께 핵심부(kernel)가 주기억 장치에 올려져 컴퓨터를 작동시키고 
운영을 관리하여 응용 프로그램이 효율적으로 실행될 수 있는 환경을 제공하는 기본 소프트웨어

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
* 커널 ( Kernel )
		- 운영체제는 컴퓨터에서 항상 실행되는 프로그램으로 커널이라고도 불린다.
* 인터럽트 ( Interrupt )
		- 하드웨어나 소프트웨어에서 어떤 사건이 발생했음을 알리는 신호
			- 하드웨어는 어느 순간이던 시스템 버스를 통해 CPU 에 신호를 보내 인터럽트 발생 가능
			- 소프트웨어는 System Call 을 통해 인터럽트 발생
		- CPU 가 인터럽트 되면 하던 일을 중지하고, 즉시 고정된 위치로 실행을 옮긴다.
			- 이 고정된 위치에는 인터럽트 서비스 루틴이 위치한 시작 주소를 가지고 있다.
			- 인터럽트가 완료되면 전에 하던 연산을 재개

##### 출력을 실행하고 있는 단일 프로세스에 대한 인터럽트 시간 일정
![1.3](https://github.com/martinkang/Study/blob/master/OSConcepts/Overview/img/chap1-interrupt.png)


## 저장장치 구조
* CPU 는 오직 메모리로부터 명령을 적재할 수 있으며, 
	실행하고자 하는 프로그램은 모두 메모리에 저장되어야 한다.
* 범용 컴퓨터는 대부분의 프로그램을 주 메모리에서 실행시킨다.
* 모든 형태이ㅡ 메모리는 워드의 배열을 제공한다.
	- 적재 : 주 메모리로부터 CPU 내부의 레지스터로 한 워드를 옮긴다.
	- 저장 : 레지스터의 내용을 주 메모리로 옮긴다.
* 이상적으로 프로그램과 데이터가 주 메모리에 영구히 존재하기를 원하지만 다음과 같은 이유로 불가능하다.
	1. 주 메모리는 모든 필요한 프로그램과 데이터를 영구히 저장하기에 너무 작다.
	2. 주 메모리는 전원이 공급되지 않으면 그 내용을 읽어버리는 휘발성 저장장치이다.
* 따라서 대부분의 컴퓨터 시스템은 주 메모리의 확장으로 보조 저장장치를 제공한다.

#### 용어
* RAM ( Random Access Memory )
	- 읽기 쓰기 가능한 메모리
	- 주메모리 라고 불린다.
* ROM ( Read Only Memory )
	- 읽기 전용 메모리

##### 저장장치 계층
![1.4](http://www.cs.odu.edu/~cs471w/spring10/lectures/introduction_files/image007.jpg)
- 출처 http://www.cs.odu.edu/~cs471w/spring10/lectures/introduction.htm
* 상위 수준은 가격이 비싸지만 빠르다. 휘발성이다
* 하위 수준은 느리지만 용량이 크고 비휘발성이다..


## 입출력 구조
* 장치 제어기 ( Device Controller )
	- 자신이 제어하는 주변장치와 자신의 로컬 버퍼 저장장치 사이의 데이터 전송을 담당
	- 통상적으로 운영체제는 각 장치 제어기마다 디바이스 드라이버를 가지고 있다.
	- 특정 타입의 장치를 담당
	- 장치 제어기에 따라 하나 이상의 장치가 부착될 수 있다.
	- 약간의 로컬 버퍼 저장장치와 특수 목적용 레지스터 집합을 유지한다.
* 입출력 연상 과정
	- 입출력 연산을 시작하기 위해 디바이스 드라이버는 장치 제어기의 적절한 레지스터에 필요한 값을 적재한다.
	- 장치 제어기는 이어 취할 동작을 결정하기 위해 이들 레지스터의 내용을 조사한다.
		- ex ) 키보드에서 한 문자를 읽어라
	- 제어기는 장치로부터 자신의 로컬 버퍼로 데이터 전송을 시작한다.
	- 데이터 전송이 완료되면, 장치 제어기는 자신이 연산을 완료했음을 
	인터럽트를 이용하여 디바이스 드라이버에 통보한다.
	- 디바이스 드라비어는 제어를 운영체제에게 반환한다.
		- 입력 완료일 경우 데이터를 반환하거나 다른 동작에 대해서는 디바이스 드라이버 상태 정보를 반환한다.
* DMA( Direct Memory Access )
	- 인터럽트 구동 방식의 입출력은 적은 양의 데이터를 전송하는 데에는 문제가 없으나
	디스크 입출력과 같은 대량의 데이터를 전송하는 데에는 높은 오버헤드를 초래한다.
		- 데이터를 일정량 보낼때마다 인터럽트를 발생시키면 오버헤드가 발생한다.
	- 장치 제어기는 CPU 의 개입 없이 메모리로부터 자신의 버퍼 장치로 또는 버퍼로부터
	메모리로 데이터 블록 전체를 전송하고, 블록 전송이 완료될 때마다 인터럽트를 발생한다.
		- 블록 전송을 하는 동안 CPU 는 다른 작업을 할 수 있다.

##### 현대 컴퓨터 시스템의 동작
![1.5](http://www.cs.odu.edu/~cs471w/spring10/lectures/introduction_files/image009.jpg)
- 출처 http://www.cs.odu.edu/~cs471w/spring10/lectures/introduction.htm


## 컴퓨터 시스템 구조

##### 대칭적 다중처리 구조
![1.6](https://github.com/martinkang/Study/blob/master/OSConcepts/Overview/img/chap1-smp.png)

### 하나의 칩에 두 개의 코어가 장착된 이중 코어 설계
![1.7](https://github.com/martinkang/Study/blob/master/OSConcepts/Overview/img/chap1-dualcore.png)

## 운영체제의 구조

##### 다중 프로그래밍 시스템을 위한 메모리 배치
![1.8](https://github.com/martinkang/Study/blob/master/OSConcepts/Overview/img/chap1-memory-layout.png)


## 운영체제 동작

### 이중 동작 모드 ( Dual Mode Operation )
### 타이머

### 캐싱 ( Caching )
![1.9](https://github.com/martinkang/Study/blob/master/OSConcepts/Overview/img/cahp1-cache.png)

#### 용어
* 인터럽트 구동식 ( Interrupt Driven )
* 트랩 ( trap )
##### 사용자 모드에서 커널 모드로의 전환
![1.10](http://www.cs.odu.edu/~cs471w/spring10/lectures/introduction_files/image011.jpg)
- 출처 http://www.cs.odu.edu/~cs471w/spring10/lectures/introduction.htm



