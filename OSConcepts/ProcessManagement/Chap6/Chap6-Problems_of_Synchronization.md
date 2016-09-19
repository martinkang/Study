# 고전적인 동기화 문제들

## 교착상태와 기아 ( Deadlock and Starvation )
* 교착상태 ( Deadlock )
	- 둘 이상의 프로세스가 서로 다른 프로세스가 가진 자원을 요구하면서
	양쪽 모두 작업을 수행 못하고 대기하는 상태


##### 교착상태
![deadlock](https://github.com/martinkang/Study/blob/master/OSConcepts/ProcessManagement/img/Chap6-deadlock.png)
* 교착상태
	- Process 1 은 mtxA 의 Lock 을 잡은 후 mtxB 를 잡으려 하고 Process 2 는 mtxB 를 잡고 mtxA 를 잡으려 한다.
	- Process 1 이 mtxA 를 잡은 후 mtxB 를 잡으려 하는데, Process 2 가 mtxB 를 이미 잡은 상태에서
	mtxA 를 잡으려 하면, 어느 한쪽이 Lock 을 풀기 전까진 양쪽 모두 작업을 더 이상 수행하지 못한다.

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
	- Semaphore, Mutex 등을 이용한 동기화가 필요.


## Readers-Writers 문제
* Readers 만 존재할 경우에는 문제가 발생하지 않는다.
* Readers 와 Writers 가 공존시 문제가 발생한다.
	- Readers 가 읽고있는 데이터를 Writers 가 변경한다면 문제가 발생한다.
* 해결방법
	- Reader Lock 과 Writer Lock 
		- Reader Lock
			- 동시에 여러 프로세스가 읽기 가능
		- Writer Lock
			- 베타적으로 오직 하나의 프로세스만 획득 가능
	- 발생 가능한 문제
		- 기아 현상
			- Reader 가 Lock 을 잡고있는 동안 Writer 는 Lock 을 획득하지 못한다.
			- 이 때 새로운 Reader 가 계속 권한을 요청하면 Writer 는 계속 Lock 을 획득하지 못한다.
			- Writer 가 Lock 을 잡는 동안에도 마찬가지.

#### Readers-Writers 문제 해결방안
```c++
semaphore mutex, wrt; // 1 로 초기화
int readCount = 0;


/* Writer Process */
void Writer()
{
	do
	{
		wait( wrt );
		// ... Writing is Performed //

		signal( wrt );
	} while ( TRUE );

	}
}


/* Reader Process */
void Reader()
{
	do 
	{
		wait( mutex );

		readCount++;
		if ( readCount == 1 )
			wait( wrt ); // 이미 Writer 가 Lock 을 획득했다면 여기서 대기하게 된다.
		
		signal( mutex ); // 다른 Reader 도 병렬 접근이 가능하도록 한다.

		// ... reading is Performed

		wait( mutex );
		
		readCount--;
		if ( readCount == 0 )
			signal( wrt );
		
		signal( mutex );
	} while( TRUE );
}
```


## 식사하는 철학자들 문제 ( The Dining Philosophers Problem )
* 문제
	- 다섯 명의 철학자가 원탁에 앉아 있고, 각자의 앞에는 스파게티가 있고 양옆에 젓가락이 한 짝씩 있다. 
	그리고 각각의 철학자는 다른 철학자에게 말을 할 수 없다. 
	이때 철학자가 스파게티를 먹기 위해서는 양 옆의 젓가락 짝을 동시에 들고 있어야 한다. 
	이때 각각의 철학자가 왼쪽의 젓가락 짝을 들고 그 다음 오른쪽의 젓가락 짝을 들어서 
	스파게티를 먹는 알고리즘을 가지고 있으면, 
	다섯 철학자가 동시에 왼쪽의 젓가락 짝을 든 다음 오른쪽의 젓가락 짝을 들 때까지 
	무한정 기다리는 교착 상태에 빠지게 될 수 있다.
	-  또한 어떤 경우에는 동시에 젓가락 양짝을 집을 수 없어 식사를 하지 못하는 
	기아 상태가 발생할 수도 있고, 몇몇 철학자가 다른 철학자보다 식사를 적게 하는 경우가 발생하기도 한다.
* 해결 방법
	- 최대 4 명의 철학자들만이 테이블에 동시에 앉을 수 있도록 한다.
	- 한 철학자가 젓가락 두 개를 모두 집을 수 있을 때만 젓가락을 집도록 허용한다.
		- 철학자는 임계 영역에서만 젓가락을 집어야 한다.
	- 비대칭 해결안을 사용한다. 즉, 홀수 번호의 철학자는 먼저 왼쪽 젓가락을 집고
	다음에 오른쪽 젓가락을 집는다. 반면에 짝수 번호의 철학자는 
	오른쪽 젓가락을 집고 다음에 왼쪽 젓가락을 집는다.
