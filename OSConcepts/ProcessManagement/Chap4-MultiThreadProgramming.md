# 다중 스레드 프로그래밍

## 4.1 개요 ( Overview )
* 스레드 ( Thread )
	- CPU 이용의 기본 단위
	- 구성
		- 스레드 ID
		- 프로그램 카운터
		- 레지스터 집합
		- 스택
	* 같은 프로세스에 속한 다른 스레드와 코드, 데이터 섹션, 그리고 열린 파일이나 Signal 같은
	운영체제 자원들을 공유한다.
* 중량 프로세스 ( Heavyweight Process )
	- 전통적인 프로세스 개념으로 단일 제어 스레드를 가지고 있다.
		- 완전한 독립적 개체

##### 단일 및 다중 스레드 프로세스
![4.1](http://www.cs.odu.edu/~cs471w/spring11/lectures/threads_files/image012.jpg)
- 출처 http://www.cs.odu.edu/~cs471w/spring11/lectures/threads.htm


### 4.1.2 장점
* 응답성
	- 대화영 어플리케이션을 다중 스레드화 하면 응용 프로그램의 일부분이 봉쇄되거나,
	다른 어플리케이션이 긴 작업을 실행하더라도 사용자에 대한 응답성이 좋다.
* 자원 공유
	- 스레드는 자동적으로 그들이 속한 프로세스의 자원들과 메모리를 공유.
		- 프로세스는 공유 메모리 또는 메시지 전달 기법을 통해야만 자원 공유가 가능
* 경제성
	- 스레드는 자신이 속한 프로세스의 자원들을 공유하기 때문에 프로세스를 생성하고 Context Switching 하는 것보다
	경제적이다.
* 규모 가변성
	- 다중 처리기 구조에서 각각의 스레드가 다른 처리기에서 병렬로 실행될 수 있다.
		- 단일 스레드 프로세스는 처리기가 아무리 많더라도 오직 한 처리기에서만 실행 가능

##### 다중 스레드화 된 서버 구조
![4.2](http://www.cs.odu.edu/~cs471w/spring11/lectures/threads_files/image013.jpg)
- 출처 http://www.cs.odu.edu/~cs471w/spring11/lectures/threads.htm


### 4.1.3 다중 코어 프로그래밍
* 시스템 설계에 있어서 최근의 경향은 하나의 칩 안에 여러 계산 코어를 넣는 것.
	- 각 코어는 운영체제에게는 독립된 처리기로 보인다.
	- 하나의 코어는 한 번에 오직 하나의 스레드만 실행할 수 있다.
* 다중코어 시스템 상에서 프로그래밍 하기 위해서는 다음과 같은 도전 과제가 있다.
	- 작업 나누기
		- 각 태스크가 개별 코어 상에서 병렬적으로 실행될 수 있도록 어플리케이션을
		독립된 병행가능 태스크로 나누는 져야 한다.
	- 균형
		- 전체 작업에 균등한 기여도를 가지도록 태스크를 나누져야 한다.
	- 데이터 분리
		- 태스크가 접근하고 조작하는 데이터 또한 개별 코어에서 사용할 수 있도록 나눠져야 한다.
	- 데이터 종속성
		- 태스크가 접근하는 데이터는 둘 이상의 태스크 사이에 종속성이 없는지 검토되어야 한다.
	- 시험 및 디버깅
		- 프로그램이 다중코어세서 병렬로 실행될 대 다양한 실행 경로가 존재한다.
		이런 병행 프로그램을 시험하고 디버깅 하는 것은 매우 어렵다.

##### 단일 코어 시스템에서의 병행 실행
![4.3](https://github.com/martinkang/Study/blob/master/OSConcepts/ProcessManagement/img/chap4-singlecore.png)

##### 다중코어 시스템에서의 병렬 실행
![4.4](https://github.com/martinkang/Study/blob/master/OSConcepts/ProcessManagement/img/chap4-multicore.png)


## 4.2 다중 스레드 모델
* 사용자 스레드 ( user thread )
	- 커널은 스레드의 존재를 모르기 때문에, 유저 모드에서 커널 모드로의 전환이 필요없다.
		- 성능이 좋다.
	* Context switching 및 thread scheduling 에서 낮은 오버헤드를 가지지만 실행 중 blocking 당하면
	전체 프로세스가 다 같이 blocking 되며, 멀티프로세스의 지원을 받을 수 없다.
* 커널 스레드 ( kernel thread )
	- 스레드 지원을 커널 수준에서 제공, 스레드 각각의 상태와 Context 저장 가능
	- 하나의 스레드가 blocking 당해도 다른 스레드는 blocking 당하지 않으며 스레드 단위로 프로세서에 할당 가능하다.
	- 스레드 전환이 발생하면 커널이 참여하고, 커널 메모리를 사용하여 오버헤드가 발생한다.

### 4.2.1 다대일 모델 ( Many to One Model )
* 많은 사용자 수준 스레드를 하나의 커널 스레드로 사상.
	- 스레드 관리는 사용자 공간의 스레드 라이브러리에 의해 행해진다.
	- 한 스레드가 Blocking 당하면 전체 스레드가 Blocking 당한다.
	- 한 번에 하나의 스레드만이 커널에 접근할 수 있기 때문에 다중 스레드가 다중 처리기에서 돌아도
	병렬도 작동할 수 없다.

##### 다대일 모델
![4.5](http://www.cs.odu.edu/~cs471w/spring11/lectures/threads_files/image014.jpg)
- 출처 http://www.cs.odu.edu/~cs471w/spring11/lectures/threads.htm


### 4.2.2 일대일 모델 ( One to One Model )
* 각 사용자 스레드를 각각 하나의 커널 스레드로 사상.
	- 하나의 스레드가 봉쇄적 시스템을 호출하더라도 다른 스레드가 실행될 수 있다.
	- 다중 처리기에서 다중 스레드가 병렬로 실행되는 것을 허용. 더 많은 병렬성을 제공
	- 단점
		- 사용자 수준 스레드를 생성할 대 그에 따른 컨러 스레드를 생성해야 하는데, 
		커널 스레드를 생성하는 오버헤드가 성능 저하를 야기시킬 수 있다.
		- 따라서 이 모델의 대부분 구현은 시스템에 의해서 지원되는 스레드 수를 제한

##### 일대일 모델
![4.6](http://www.cs.odu.edu/~cs471w/spring11/lectures/threads_files/image015.jpg)
- 출처 http://www.cs.odu.edu/~cs471w/spring11/lectures/threads.htm


### 4.2.3 다대다 모델 ( Many to Many Model )
* 다수의 사용자 수준 스레드를 그보다 작은 수 혹은, 같은 수의 커널 스레드로 멀티 플렉스 한다.
	- 커널 스레드의 수는 응용 프로그램이나 특정 기계에 따라 결정된다.
* 개발자가 원하는 만큼의 사용자 수준 스레드를 만들도록 허용되지만, 한 번에 하나의 스레드만이 커널에
의해서 스케줄되기 때문에 진정한 동시성을 획들할 수는 없다.
* 필요한 만큼 사용자 스레드를 만들 수 있고, 그에 상응하는 커널 스레드가 다중 처리기에서 병렬 처리가 가능하다.

##### 다대다 모델
![4.7](http://www.cs.odu.edu/~cs471w/spring11/lectures/threads_files/image016.jpg)
- 출처 http://www.cs.odu.edu/~cs471w/spring11/lectures/threads.htm

##### 두 수준 모델
![4.8](https://github.com/martinkang/Study/blob/master/OSConcepts/ProcessManagement/img/chap4-two-level-model.png)


## 4.4 스레드와 관련된 문제들

### 4.4.1 Fork() 및 Exec() 시스템 호출

### 4.4.2 스레드 취소

### 4.4.3 Signal Handling

### 4.4.4 Thread pools

### 4.4.5 Thread Specific Data

### Scheduler Activations
