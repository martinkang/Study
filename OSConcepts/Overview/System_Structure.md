# 시스템 구조

## 2.1 운영체제 서비스
* 사용자 편의를 위한 서비스
	- 사용자 인터페이스
	- 프로그램 실행
	- 입출력 연산
	- 파일 시스템 조작
	- 통신
	- 오류 탐지
* 시스템 자체의 효율적인 동작을 위한 서비스
	- 자원 할당
	- 회계
	- 보호 와 보안


## 2.2 사용자 운영체제 인터페이스
* 사용자 운영체제 인터페이스는 사용자가 운영체제와 접촉하는 방식으로 다음과 같은 방법에 있다.
	- 명령어 라인 인터페이스 또는 명령어 해석기 ( Character User Interface : CUI )
	- 그래피컬 사용자 인터페이스가 있다. ( Graphical User Interface : GUI )


### 2.2.1 명령 해석기 ( Command-Interpreter )
- 커널에 명령 해석기를 포함하고 있거나, 사용자가 처음 로그온 할 때 실행되는 특수한 프로그램
* 쉘 ( Shell ) 이라고도 한다.
	- 사용자가 지정한 명령을 가져와서 그것을 실행시키는 역할.
* 명령어 해석기의 두가지 구현 방법
	- 명령 해석기 자체가 명령을 실행할 코드를 가지고 있는 경우
		- 명령 해석기가 자신의 코드의 한 부분으로 분기하고, 시스템호출을 하기 때문에
		제공될 수 있는 명령어 수에 따라 명령 해석기의 크기가 결정된다.
			- 각 명령이 자신의 구현 코드를 요구하기 때문.
	- 시스템 프로그램에 의해 대부분의 명령어를 구현하고, 명령 해석기는 그 명령을 실행.
		- 명령 해석기는 전혀 그 명령을 모른다.
		- 명령 해석기는 구현된 파일을 가져와 실행시키므로, 
		새로운 명령을 추가하기 위해 명령 해석기가 변경될 필요가 없다.


## 2.3 시스템 호출 ( System Calls )
- 운영체제가 제공하는 서비스에 대한 인터페이스 제공

* System Call
	-  사용자 프로그램에서 운영 체제의 기능을 불러 내기 위한 프로그램 절차 호출. 
	- OS 에서는 사용자 프로그램에서 메모리나 입출력 장치를 직접 조작하는 것이 허락되지 않으므로 
	파일이나 입출력 장치나 메모리에 액세스할 때 사용한다.
* API ( Application Programming Interface )
	- 함수에 전달되어야 할 매개변수들과 프로그래머가 기대할 수 있는 값을 포함하여
	응용 프로그래머가 사용 가능한 함수의 집합을 명시.
* 왜 응용프로그래머는 실제 System Call 을 부르는 것보다 API 에 따라 프로그래밍 하는 것을 선호할까?
	- 프로그램 호환성
		- 자신의 프로그램이 같은 API 를 지원하는 시스템에서 컴파일 되고 실행되는 것을 기대할 수 있다.
	- 보잡성 
		- 실제 System Call 은 종종 더 자세한 명세가 필요하여 사용이 어렵다.
* 시스템 호출 인터페이스
	- API 함수의 호출을 가로채어 필요한 운영체제 시스템 호출을 부르고
	시스템 호출의 상태와 반환값을 돌려준다.
	- 호출자는 시스템 호출이 어떻게 구현되고 실행 중 무슨 작업을 하는지 알 필요가 없다.


## 2.4 시스템 호출의 유형
- 프로세스 제어
- 파일관리
- 장치 관리
- 정보의 유지
- 통신
- 보호


## 2.7 운영체제 구조

### 2.7.1 간단한 구조 ( Simple Structure )
### 2.7.2 계층적 접근 ( Layered Approach )
### 2.7.3 마이크로 커널 ( Microkernels )
### 2.7.4 모듈

## 2.8 가상기계 ( Virtual Machines )
* 한 컴퓨터의 하드웨어를 추상화 하여 다른 다수의 다른 실행 환경을 제공하도록 한다.
	- 각각의 개별적인 실행 환경이 자신만의 개별적인 컴퓨터를 사용하는 것처럼 보인다.

### JVM ( Java Virtual Machines )



## 2.11 시스템 부트
* 대부분 컴퓨터는 부트스트램 또는 부트스트랩 로더가 커널을 주 메모리에 적재하고, 커널을 실행한다.
* 컴퓨터가 전원을 키거나 리셋 사건이 생기면 명령 레지스터는 미리 지정된 메모리 위치를 가리키고 ( 부트스트랩이 존재 ) 
	그곳에서 실행을 시작.
	- RAM 은 시스템 시작 시에 알 수 없는 상태가 되기 때문에 이 프로그램은 ROM 에 존재한다.
* 펌웨어 ( Firmware )
	- 하드웨어와 ( ROM 과 같이 읽기만 가능 ) 소프트 웨어의 중간적 특성을 
( 쓰기 가능하도록 만드는 명령어가 주어지기 전까진 읽기 전용 ) 가지는 ROM 의 모든 형태를 뜻함
	- 펌웨어는 RAM 보다 느리기 때문에 몇몇 시스템은 펌웨어에 운영체제를 저장하고 실행할 때 RAM 으로 복사하여 실행
	- 일반적인 운영체제 및 대용량 운영체제는 부트스트램 로더는 펌웨어에 있고 운영체제는 디스크에 존재한다.