# 고전적인 동기화 문제들

## 교착상태와 기아 ( Deadlock and Starvation )
* 교착상태 ( Deadlock )
	- 둘 이상의 프로세스가 서로 다른 프로세스가 가진 자원을 요구하면서
	양쪽 모두 작업을 수행 못하고 대기하는 상태
##### 교착상태
![deadlock](http://2.bp.blogspot.com/-63RZ-BTlAFs/VfGeHMnGdFI/AAAAAAAADuw/gwqtrVliMsM/s1600/Deadlock%2Bof%2BThreads.jpg)
-출처 : http://javarevisited.blogspot.com/2010/10/what-is-deadlock-in-java-how-to-fix-it.html

* 기아상태
	- 프로세스가 끊임없이 필요한 자원을 못 가져오는 상태
	- 세마포와 연관된 큐에서 프로세스들을 후입선출 순서로 제거할 경우 발생할 수 있다.


## 우선순위 역전 ( Priority Inversion )
* 낮은 우선순위 프로세스 또는 연속된 낮은 우선순위가 커널 데이터에 접근하고 있을 때
높은 우선순위 프로세스가 이 커널 데이터를 읽거나 수정하려 하면 문제가 생길 수 있다.
	- 통상 커널 데이터는 lock 에 의해 보호되기 때문에 낮은 우선 순위의 프로세스가
	자원의 사용을 마칠때까지 높은 우선순위 프로세스가 기다려야 한다.
* 예제
	- 우선순위 A < B < C
		1. A 가 커널 데이터를 접근하여 lock.
		2. C 는 A 가 커널 데이터를 다 쓰기를 기다림.
		3. B 가 준비 완료가 되어 프로세스 A 를 .점함
		4. C 는 B 에 의해 대기 시간이 더 길어짐.
* 해결방법
	- 셋 이상의 우선순위를 가질 때 발생하므로 우선 순위를 두개만 갖도록 한다.
		- 두 개의 우선순위만 사용하는 프로세스는 범용 운영체제에서 사용하기는 불충분하다.
	- 상속 프로토콜 구현 ( Priority Inheritance Protocol )
		- 더 높은 우선순위 프로세스가 필요로 하는 자원을 접근하면 모든 프로세스들은,
		문제가 된 자원의 사용이 끝날때까지 더 높은 우선순위를 상속받는다.


## 유한 버퍼 문제 ( The Bounded Buffer Problem )
* 유한한 개수의 물건(데이터)을 임시로 보관하는 보관함(버퍼)에 여러 명의 생산자들과 소비자들이 접근한다. 
생산자는 물건이 하나 만들어지면 그 공간에 저장하고 물건의 Count 를 증가시키고
, 소비자는 물건이 필요할 때 보관함에서 물건을 하나 가져오고 Count 를 감소시킨다.
	- 만약 아래 source 에서 Buffer 상에 아이템이 1개만 있을때 두 개의 소비자 프로세스가 동시에 접근하게 된다면
	동시에 같은 아이템을 가져가게 된다.


```c++
int headIdx = 0; // 소비자가 가져갈 아이템 Index */
int tailIdx = 1; // 생산자가 생성한 아이템이 저장될 Index */
int curItemCount = 1;

/* 생산자 프로세스 */
do 
{
	buffer[curItemIdx] = item;
	curItemIdx++;
	curItemCount++;
} while (1);


/* 소비자 프로세스 */
do 
{
	// curItemCount 가 1 인 상황에서 두 개의 서로 다른 프로세스가 동시에 접근할 수 있다.
	if ( curItemCount > 0 )
	{
		// 결국 두 개의 프로세스는 서로 같은 아이템을 가져가게 된다.
		outItem = buffer[headIdx];
		headIdx++;
		curItemCount--;
	}
} while (1);
```

* 해결방법
	- 

## Readers-Writers 문제
*
* 해결방법


## 식사하는 철학자들 문제 ( The Dining Philosophers Problem )


