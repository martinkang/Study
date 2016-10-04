# 연습문제
* 7.1 외나무 다리에서 두 농부가 만났을 때, 동시에 다리의 중간에 도착하면 교착상태가 발생한다.
이런 교착 상태를 예방하는 알고리즘을 세마포를 사용하여 설계하시오. ( 기아 상태 무시 )
```c++
```

* 7.2 7.1 의 해결책을 기아 현상이 발생하지 않도록 수정하시오.
```c++
Sem gBridgeSem; // Semaphore 는 1 로 초기화.

City *gCityA
City *gCityB

void moveFamer( Farmer *aFam )
{
	aFam->mLocate = B;

	gCityA->mCount--;
	gCityB->mCount++;
}

void CrossBridge( Farmer * aFam )
{
	wait( gBridgeSem );

	wait( gCityA->mSem );
	wait( gCityB->mSem );

	if ( aFam->mLocate == gCityA ) // 농부가 A 마을방향에서 왔다.
		moveFarmer( aFam, gCityA, gCityB ); // A 마을에서 B 마을 방향으로 다리를 건넌다.
	else
		moveFarmer( aFam, gCityB, gCityA );

	post( gCityB->mSem );
	post( gCityA->mSem );

	post( gBridgeSem );
}
```
- 이 방법은 결국 gBridgeSem 을 먼저 wait() 한 순서대로 동작을 하고, 앞에 먼저 요청한 작업이 끝나면 자신의 순서가 돌아오기 대문에
기아 상태가 발생하지 않음을 알 수 있다.
* 7.3 세 개의 프로세스에 의해 공유되는 동일한 타입의 네 개의 자원으로 구성된 시스템을 고려해보자.
이들 각 프로세스는 최대 두 개의 자원을 필요로 한다. 시스템에 교착상태가 발생하지 않음을 보이시오.
	- 최악의 경우 세 개의 프로세스 모두 두개의 자원을 필요로 한다고 가정하자.
	- 이 때 가능한 최악의 시나리오는 각 프로세스가 하나씩만 자원을 가지고 있을 때이다.
		- 하지만 각 프로세스가 하나씩 자원을 점유하고 있을 때 하나의 자원이 남게되므로 어떤 프로세스든 
		남은 자원을 요청하여 프로세스를 끝낼 수 있다.
		- 프로세스가 끝나면 자원 2 개가 해제되므로 결국 남은 프로세스 역시 종료될 수 있다.
	- 따라서 점유하며 대기 조건을 만족하지 못하므로 교착상태가 발생할 수 없다.
* 7.4 그림에 보인 교통의 교착상태를 생각해 보자
### 교착상태
![img](https://github.com/martinkang/Study/blob/master/OSConcepts/ProcessManagement/img/chap7-traffic.png)
	- a. 이 예에서 교착상태를 위한 네 가지 필요조건이 정말로 성립하는지 보이시오
		- 상호배제
			- 차도는 상-하, 좌-우 둘 중 한 방향으로만 움직일 수밖에 없다.
		- 점유하며 대기
			- 각 사거리에서 대기하고 있는 차는 자신이 점유하고 있는 자리를 계속 점유한체 대기하고 있다.
		- 비선점
			- 어느 차도 각 차를 사거리에서 대기하고 있는 차를 강제로 치워버릴 수가 없다.
		- 순환 대기
			- 좌측 상단에서 우측으로 사거리마다 번호를 붙였을 때, 1 번 사거리의 차는 3번 사거리의 차가 빠지기를 기다리고 있고
			3번 사거리의 차는 4번, 즉 1번 -> 3 번 -> 4번 -> 2번 -> 1번 이런 순으로 각 사거리의 차를 순환적 하며 대기하고 있다.
	- b. 이 시스템에서 교착상태를 회피하는 간단한 법칙을 설명하시오.
		- 뮤텍스를 이용하여 신호등과 같은 역할을 하도록 한다.
			- 뮤텍스를 이용한 잠금을 이용하여 상-하 움직임을 하기 전에 좌-우 움직임을 모두 통제하여 사거리에 진입하지 못한 상태에서
			상-하 움직임을 하도록 하고 반대로 좌-우 움직임을 수행할대는 상-하 움직임을 모두 통제한 후 움직인다.
* 7.5
* 7.6
* 7.9
* 7.11
* 7.12
* 7.14
* 7.16
* 7.17
