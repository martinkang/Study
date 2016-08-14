# 프로세스 관리
* 잡 ( Job )
	- 일괄 처리 시스템에서 작업 단위
* 태스크 ( Task )
	- 시분할 시스템에서의 작업 단위


## 3.1 프로세스 개념

### 3.1.1 프로세스 ( Process )
* 프로세스 ( Process )
	- 정의
		- 실행 중인 프로그램
		- 프로그램은 명령어 리스트를 내용으로 가진 디스크에 저장된 파일로 실행을 기다리는 수동적인 존재
		- 프로세스는 다음에 실행할 명령어를 지정하는 프로그램 카운터 및 관련된 자원의 집합을 가진 능동적인 존재.
		- 실행 파일이 메모리에 적재될 때 프로그램은 프로세스가 된다.
	- 구성
		- 스택
			- 복귀 주소와 지역 변수 및 함수의 매개변수 등 임시적인 자료
		- 힙
			- 동적 메모리 할당 영역
		- 데이터 섹션
			- 전역 변수
		- 텍스트 섹션
			- 프로그램 코드

##### 메모리 상의 프로세스
![3.1](http://www.cs.odu.edu/~cs471w/spring11/lectures/Processes_files/image002.jpg)
- 출처 http://www.cs.odu.edu/~cs471w/spring11/lectures/Processes.htm


### 3.1.2 프로세스 상태
* 프로세스의 현재의 활동에 따라 정의된다.
	- new : 프로세스가 생성 중이다.
	- running : 명령어들이 실행되고 있는 중이다.
	- waiting : 프로세스가 어떤 사건( 입출력 완료 또는 신호의 수신 가틍ㄴ )이 일어나기를 기다린다.
	- ready : 프로세스가 처리기에 할당되기를 기다린다.
	- terminated : 프로세스가 실행을 종료하였다.

##### state diagram
![3.2](http://www.cs.odu.edu/~cs471w/spring11/lectures/Processes_files/image004.jpg)
- 출처 http://www.cs.odu.edu/~cs471w/spring11/lectures/Processes.htm


### 3.1.3 프로세스 제어 블록 ( Process Control Block : PCB )
* 특정 프로세스와 연관된 여러 정보를 수록.
	- 프로세스 상태
		- 현재 프로세스의 활동 상태 
	- 프로그램 카운터
		- 다음에 실행할 명령어의 주소
	- CPU 레지스터들
		- 컴퓨터 구조에 따라 다양한 개수와 타입을 가진다.
	- CPU-스케줄링 정보
		- 프로세스 우선순위, 스케줄 큐에 대한 포인터와 다른 스케줄 매개변수를 포함
	- 메모리 관리 정보
		- 운영체제가 지원하는 메모리 시스템에 따라 base 레지스터와 limit 레지스터의 값, 
	페이지 테이블 또는 세그먼트 테이블 등과 같은 정보를 포함
	- 회계 정보 ( accounting )
		- CPU 가 사용된 양과 사용된 실제 시간, 시간제한, 계정번호, 잡 또는 프로세스 번호 등을 포함
	- 입출력 상태 정보
		- 프로세스에 할당된 입출력장치들과 열린 파일의 목록 등을 포함

##### PCB
![3.3](http://www.cs.odu.edu/~cs471w/spring11/lectures/Processes_files/image006.jpg)
- 출처 http://www.cs.odu.edu/~cs471w/spring11/lectures/Processes.htm

##### CPU 가 한 프로세스에서 다른 프로세스로 교환되는 과정
![3.4](http://www.cs.odu.edu/~cs471w/spring11/lectures/Processes_files/image008.jpg)
- 출처 http://www.cs.odu.edu/~cs471w/spring11/lectures/Processes.htm


## 3.2 프로세스 스케줄링 ( Process Scheduling )
* 다중 프로그래밍의 목적은 CPU 이용을 최대화 하기 위하여 항상 
어떤 프로세스가 실행 중이도록 하는데 있다.
* 시분할의 목적은 각 프로그램이 실행되는 동안 사용자가 상호할동 할 수 있도록
프로세스 사이에서 CPU 를 빈번하게 교체하는 것이다.
	- 이 목적을 달성하기 위해 프로세스 스케줄러는 CPU 에서
	실행 가능한 여러 프로세스들 중에서 하나의 프로세스를 선택한다.

### 3.2.1 스케줄링 큐 ( Scheduling Queues )
* Job Queue
	- 시스템에 들어온 모든 프로세스 리스트
* Ready Queue
	- 준비완료 상태에서 실행을 대기하는 프로세스 리스트
	- 큐의 헤더는 리스트의 첫번째와 마지막 PCB 를 가리키는 포인터를 가진다.
	- 각 PCB 는 준비완료 큐에 있는 다음 프로세스를 가리키는 포인터 필드를 가진다.
* Device Queue
	- 특정 입출력 장치를 대기하는 프로세스들의 리스트
	- 각 장치는 장치를 대기하는 Device Queue 를 가진다.

##### 준비완료 큐와 다양한 입출력 장치 큐
![3.6](http://www.cs.odu.edu/~cs471w/spring11/lectures/Processes_files/image010.jpg)
- 출처 http://www.cs.odu.edu/~cs471w/spring11/lectures/Processes.htm

##### 프로세스 스케줄링을 표현하는 큐잉 도표
![3.7](http://www.cs.odu.edu/~cs471w/spring11/lectures/Processes_files/image012.jpg)
- 출처 http://www.cs.odu.edu/~cs471w/spring11/lectures/Processes.htm
* 새로운 프로세스는 처음에 준비완료 큐에 놓인다. 프로세스가 실행되도록 선택될 때 ( CPU 를 할당받을 때 ) 다음과 같은 사건들중 하나가 발생할 수 있다.
	- 프로세스가 입출력을 요구하여 I/O Device Queue 에 넣어질 수 있다.
	- 프로세스가 새로운 서브프로세스를 생성하고 그 프로세스의 종료를 기다릴 수 있다.
	- 프로세스가 인터럽트의 결과에 따라 강제로 CPU 로부터 제거되어 준비완료 큐에 다시 놓일 수 있다.
* 프로세스는 종료될 때까지 이 주기를 계속하며, 종료되면 모든 큐에서 삭제되고 그 자신의 PCB 와 자원을 반납한다.


### 3.2.2 스케줄러 ( Scheduling )
* 프로세스는 일생 동안에 다양한 스케줄링 큐들 사이를 이주한다.
프로세스를 스케줄링 하기 위해서 운영체제는 이들 큐에서 프로세스들을 반드시 선택하며, 선택절차를 적절한 스케줄러에 의해 실행된다.
* 스케줄러
	* 종류 
		- 장기 스케줄러 ( 또는 잡 스케줄러 )
			- 실행 빈도가 적다.
			- 프로세스들을 선택하여 실행하기 위해 메모리로 적재한다.
			- 다중 프로그램에서 메모리에 있는 프로세스의 수를 제어한다. ( 다중 프로그래밍의 정도 )
			- 다중 프로그래밍의 정도가 안정적이면, 평균프로세스 생성률이 시스템을 떠나는 평균 프로세스 이탈률 ( average departure rate ) 가 반드시 같아야 한다.
				- 장기 스케줄러는 프로세스가 시스템에 떠날 때만 호출 될 필요가 있다.
		- 단기 스케줄러 ( 또는 CPU 스케줄러 )
			- 실행 준비가 완료되어 있는 프로세스들 중 하나에게 CPU 를 할당
			- 실행 간격이 짧기 때문에 매우 빨라야 한다.
		- 중기 스케줄러
			- 일부 운영체제들을 중간 수준의 스케줄링들 도입한다.
			- 메모리에서 프로세스들을 제거하여, 즉 CPU 를 위한 경쟁에서 제거하여 다중 프로그래밍의 정도를 완화하는 게 적합할 때도 있다.
			- 차후 다시 프로세스를 메모리로 불러와서 중단되었던 시즘에서부터 실행을 재개한다.
				- 스와핑 ( Swapping ) 이라고 한다.
	* 장기와 단기 스케줄러의 차이는 실행 빈도에 있다.
	* 장기 스케줄러는 입출력 중심과 CPU 중심의 프로세스가 적절히 혼합되도록 한다.
		- 만약 모든 스케줄이 입출력 중심이라면?	
			- 준비완료 큐는 항상 비게 되고 단기 스케줄러는 할 일이 없다.
		- 만약 모든 스케줄이 CPU 중심이라면?
			- 입출력 대시 큐는 항상 비고, 장치들이 사용되지 않는다.

##### 큐잉 도표에 중기 스케줄러 추가
![3.8](http://www.tutorialspoint.com/operating_system/images/queuing_diagram.jpg)
- 출처 http://www.tutorialspoint.com/operating_system/os_process_scheduling.htm


### 3.2.3 문맥 교환 ( Context Switch )
* Context
	- 프로세스의 PCB 에 표현된다.
	- CPU 값, 프로세스 상태, 메모리 관리 정보 등을 포함
* Context Switching
	- CPU 를 다른 프로세스로 교환하기 위해 현재 프로세스의 상태를 저장하고 다른 프로세스의 저장된 상태를 복구하는 작업
	- Context Switching 이 발생하면, 커널은 과거 프로세스의 Context 를 PCB 에 저장하고, 스케줄된 새로운 프로세스의 저장된 Context 를 복구한다.
	- Context Switching 동안은 시스템이 아무런 유용한 일을 하지 못하기 때문에 순수한 오버헤드이다.


## 3.3 프로세스에 대한 연산

### 3.3.1 프로세스 생성
* 프로세스는 실행 도중에 프로세스 생성 시스템 호출을 통해서 여러 개의 새로운 프로세스들을 생성할 수 있다.
	- 부모 프로세스 : 생성하는 프로세스
	- 자식 프로세스 : 부모 프로세스가 생성한 새로운 프로세스
	- pid : 유일한 프로세스 식별자
* UNIX 운영체제에서의 프로세스 생성
	- fork()
		- 새로운 프로세스를 생성 ( 원래 프로세스의 주소 공간 복제본으로 구성 )
		- 자식 프로세스는 열린 파일과 같은 자원 분 아니라 특권과 스케줄링 속성을 부모에세 상속 받음
	- exec()
		- 자신의 메모리 공간ㅇ르 새로운 프로그램으로 바꾸기 위해 사용
		- 이진 파일을 메모리에 적재하고 원 프로그램 이미지는 파괴, 적재된 프로그램을 실행한다.
	- wait()
		- 자식 프로세스가 끝날때까지 기다린다.

##### 새 프로세스를 fork 하는 c 프로그램
```c++
#include <stdio.h>

int main( int argc, char *argv[] )
{
	int pid;
	
	pid = fork();
	
	if ( pid < 0 ) /* error */
	{
		fprintf( stderr, "Fork Failed" );
		return 1;
	}
	else if ( pid == 0 ) /* child */
	{
		execlp( "/bin/ls", NULL );
	}
	else /* parent */
	{
		wait( NULL );
		printf( "Child Complete" );
	}
	
	reutnr 0;
}
```

##### 프로세스 생성
![3.11](http://www.cs.odu.edu/~cs471w/spring11/lectures/Processes.htm)
- 출처 http://www.cs.odu.edu/~cs471w/spring11/lectures/Processes.htm
- 
##### 전형적인 Solaris 시스템의 프로세스 트리
![3.9](http://www.cs.odu.edu/~cs471w/spring11/lectures/Processes_files/image014.jpg)
-출처 http://www.cs.odu.edu/~cs471w/spring11/lectures/Processes.htm


### 3.3.2 프로세스 종료
* 프로세스가 마지막 문장의 실행을 끝내고, exit 시스템 호출을 사용하여 운영체제에게 자신의 제거를 요청하면 종료한다.
	- 이 시점에서 프로세스는 자신의 부모 프로세스에게 ( 부모 프로세스가 wait() 호출 ) 상태값을 반환할 수 있다.
	- 물리 메모리와 가상 메모리, 열린 파일, 입출력 버퍼를 포함한 프로세스의 모든 자원이 운영체제로 반납된다.
* 좀비 프로세스
	- 실행이 종료되었지만 아직 삭제되지 않은 프로세스를 뜻한다.
	- 보통 프로세스는 exit 시스템 함수를 호출하여 프로세스를 종료시키고 자신의 모든 자원을 해제시킨다
	- 부모 프로세스는 자식 프로세스의 종료 상태를 가져올 수 있어서 자신의 자식 프로세스가 어떤 상태로
	작업을 마쳤는지에 대한 정보를 확인하고 task_struct 를 삭제한다. ( 부모 프로세스의 wait() 계열 함수를 이용하여 )
	- 만약 부모가 이 상태값을 확인하지 않는다면, 이 종료 상태는 커널에서 유지한다.
		- 커널 입장에서 봤을때 task 자료구조를 유지하기 위한 얼마간의 자원을 소모하게 된다.
		- 커널이 유지할 수 있는 task 구조체의 리스트 크기가 제한이 있으므로,
		많은 좀비프로세스가 발생할 경우 시스템 성능에 악영향ㅇ르 미칠 수 있다.

## 3.4 프로세스간 통신 ( Interprocess Communication : IPC )

### 3.4.1 공유 메모리 시스템
### 3.4.2 메시지 전달 시스템

## 3.6 클라이언트 서버 환경에서 통신

### 3.6.1 소켓 ( Socket )

### 3.6.2 원격 프로시저 호출 ( Remote Procedure Calls, RPC )

### 3.6.3 파이프 ( Pipes )
