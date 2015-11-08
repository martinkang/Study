Chapter 03, Sharing data between threads

# Chapter 03, Sharing data between threads
병행처리를 위해 Thread를 사용하는 것 중 가장 핵심이 되는 이점은 직접 쓰레드 사이에서 쉽게 데이터를 공유할 수 있다는 것입니다.
만약 쓰레드 사이에서 데이터를 공유하고 있다면, 어떤 쓰레드가 어떤 데이터의 비트를 접근할 수 있는지 그리고 어떻게 어느 업데이트들이   해당 데이터를 다루는 다른 쓰레드에게 알려줄지에 대한 규칙을 가져야 할 필요가 있습니다.
이번 챕터에서는 C++에서 쓰레드 사이의 안전한 데이터 공유와 일어날 수 있는 잠재적인 문제를 피하고, 이점을 최대화 할 수 있는 방안에    대해 이야기 하고자 합니다.

## 3.1 Problems with sharing data between threads
- 만약 모든 공유 데이터가 읽기 전용이라면 문제가 없지만, 데이터가 쓰레드 사이에서 공유되고 있고 하나이상의 쓰레드가 해당 데이터를    수정하려 한다면, 수 많은 잠재적인 위험이 존재하게 된다. 이 경우 모든것이 정상적으로 돌아갈 수 있도록 확인을 반드시 해야한다.
- 프로그래머들이 그들의 코드를 추론하는데 도움을 주는데 널리 사용되는 하나의 개념은 *invariants* 입니다.                             
    - 예를 들면 '이 변수는 리스트안에 있는 아이템의 갯수를 포함한다.'와 같이 특정 자료구조에 대한 항상 참 값을 나타내는 구문입니다.
    - 이러한 *invariants*들은 종종 업데이트 동안 특히 만약 해당 자료구조가 복잡하거나 업데이트가 하나 이상의 값을 수정을 요할 때     깨지게 됩니다.
- 다음 그림과 같이 double linked list를 생각해 봅시다. 
    - 만약 한 노드 (A)의 next pointer 는 (B)이고, (B)의 previous pointer는 (A) 입니다.
    - 한 노드를 리스트에서 지우려면 서로의 노드에서 각 포인터를 업데이트 해야하고, 이 때 *invariant*가 깨지게 됩니다.
    - 그림 3.1에서는 다음과 같은 방법으로 리스트에서 한 엔트리를 삭제하는 법을 보여줍니다.
    ![Imgur](http://i.imgur.com/HkZGShX.png)
    
        1. 삭제할 노드를 선택합니다.(N)
        2. N 이전의 노드의 링크를 N 이후의 노드와 연결합니다.
        3. N 이후의 노드의 링크를 N 이전의 노드와 연결합니다.
        4. 노드 N을 삭제합니다.

    - 위에서 볼 수 있다시피 단계 b와 c사이에서 *broken invariant*가 되는 것을 볼 수 있습니다.
- 쓰레드 사이에서 수정중인 데이터를 공유할 때 가장 간단한 문제는 *broken invariant* 입니다.
    - 만약 한 쓰레드가 노드를 삭제중일 때 다른 쓰레드가 double linked list를읽으려고 하면, 해당 쓰레드는 부분적으로 제거된 노드를 볼 수 있을지도 모릅니다. (위 그림에서 b처럼)
- 이 *broken invariant*의 결과는 다양합니다.
    - 다른 쓰레드가 단지 list의 아이템을 그림에서 왼쪽부터 오른쪽으로 읽는다면, 삭제되고 있는 노드를 지나칠 수 있을지도 모릅니다.
    - 반면에 두 번째 쓰레드가 그림에서 가장 오른쪽에 있는 노드를 삭제하려고 하면, 아마 자료 구조의 충돌이 발생하고 프로그램이        비정상적으로 종료될 것입니다.
    - 결과가 무엇이든간에 병렬 코드의 가장 흔한 버그는 바로 *race condition* 입니다.

### 3.1.1. Race conditions


# ================================================================
# 1 차 날번역중입니다. 다시 고치겠습니다.

# 3. Sharing data between threads

### 3.2.6 Flexible locking with std::unique_lock

- std::unique_lock 은 *invariants* 를 완화시켜 std::lock_guard 보다 조금 더 flexibility 한 기능을 제공합니다. 
	- std::unique_lock 인스턴스는 mutex 를 소유하지 않음으로서 invariant 를 완화 시킵니다.
- 우선, 생성자에 두번째 인자로 std::adopt_lock 을 전달하면 lock 객체가 mutex 의 lock 을 관리합니다,
그리고 두번째 인자로 std::defer_lock 을 전달할 수 있는데 이는 생성시에 mutex 는 unlocked 상태로 남아있음을 나타냅니다.
이후에 std::unique_lock 객체( mutex 가 아닌 ) 의 lock() 을 호출하거나 std::unique_lock 객체를 std::lock() 자신에 전달함으로서 lock 을 획득할 수 있습니다.  


- Listing 3.6 의 std::lock_guard 와 std::adopt_lock 을 std::unique_lock 와 std::defer_lock 로 대체하면 Listing 3.9 와 같이 쉽게 쓰일 수 있습니다. 이 두 코드는 같은 라인수를 가며, 본질적으로 동일합니다 : 
> std::unique_lock 은 std::lock_guard 보다 더 많은 공간을 필요로 하고 부분적으로 보다 느립니다. 

- std::unique_lock 인스턴스가 mutex 에 대한 소유권을 가지지 않음으로서 얻는 flexibility 는 그에 따른 비용이 발생합니다 : 
> mutex 의 소유권 정보는 저장되고, 업데이트 되어야 합니다.


##### Listing 3.9 Using std::lock() and std::unique_lock in a swap operation
```c++
class some_big_object;
void swap(some_big_object& lhs,some_big_object& rhs);

class X
{
	private:
		some_big_object some_detail;
		std::mutex m;
	public:
		X(some_big_object const& sd):some_detail(sd){}

		friend void swap(X& lhs, X& rhs)
		{
			if(&lhs==&rhs)
				return;
			std::unique_lock<std::mutex> lock_a(lhs.m,std::defer_lock); /* (1) */
			std::unique_lock<std::mutex> lock_b(rhs.m,std::defer_lock); /* (1) */
			std::lock(lock_a,lock_b); /* (2) */
			swap(lhs.some_detail,rhs.some_detail);
		}
};
```

Listing 3.9 에서, std::unique_lock 개체는 std::lock() 에 전달될 수 있습니다. 이는 std::unique_lock 가 lock(), try_lock() 그리고 unlock() 멤버 함수를 지원하기 때문입니다.
mutex 하위의 멤버 함수들과 이름이 같은 이러한 멤버 함수들은 실질적으로 작업을 수행 하고 std::unique_lock 인스턴스 내부의 flag 를 바로 갱신합니다. 이 flag 는 현재 인스턴스의 mutex 소유 여부를 나타냅니다.
이 flag 는 소멸자에서 올바르게 unlock() 이 호출되는 것을 보장하기 위해 필수적입니다.  
만약 인스턴스가 mutex 를 소유한다면, 소멸자는 unlcok() 을 반드시 호출해야 하고, 만약 인스턴스가 mutex 를 소유하지 않으면, 이것은 unlock() 을 호출해서는 안됩니다.
이 flag 는 owns_lock() 멤버 함수를 호출하여 조회할 수 있습니다.
그리고 이미 예상했듯이, 이 flag 는 어딘가에 저장되어집니다.
따라서, 일반적으로 std::unique_lock 객체의 크기는 std::lock_guard 객체보다 크며, 
또한 std::unique_lock 약간의 성능상 페널티가 생기는데, 
이는 std::lock_guard 가 아닌  std::unique_lock 을 사용시 flag 가 적절히 갱신되어야 하기 때문입니다.
그렇기 때문에 만약 std::lock_guard 가 당신의 needs 를 충분히 만족시킨다면, std::lock_guard 를 먼저 써 보시길 권장합니다.  

이말은 즉, std::unique_lock 이 이러한 추가적인 flexibility 를 필요로 하여 task at hand 보다 유용한 경우가 있다는 말입니다. 
이 예제는 앞에서 이미 보았던 deferred locking 입니다; 또 다른 케이스는 lock 에 대한 소유권이 다른 scope 로 이동하는 예입니다.


### 3.2.7 Transferring mutex ownership between scopes

std::unique_lock 인스턴스는 그들 자신의 mutex 를 소유할 수 없기 때문에, mutex 의 소유권은 인스턴스 사이에 이동을 통해 전달이 가능합니다. 
몇 케이스에서 이러한 이동은 함수로부터 인스턴스의 리턴을 통해 자동적으로 전달되고, 
또다른 케이스에서는 명시적으로 std::move() 함수 호출을 통해 이루어집니다.  
기본적으로 이런 소유권 전달은 source 가 lvalue( 실제 값 또는 참조자 ) 또는 rvalue ( 임시적인 값의 한종류 ) 인지 여부에 달려있습니다.
만약 소스가 rvalue 이면 자동적으로 소유권이 전달되며, lvalue 에서는 변수로부터 소유권이 의도치 않게 전달 되는 것을 피하기 위해 명시적으로 소유권을 전달해야 합니다.
std::unique_lock 은 이동가능하지만 복사 불가능한 타입 중 하나입니다. 
부록 A 의 섹션 A.1.1 보면 에 더 많은 move semantic 에 대해 알 수 있습니다.  
가능한 사용 방법 중 하나는 함수가 mutex 에 대한 lock 과 호출자에 대한 lock 의 소유권 이전을 허용하는 것 입니다, 
그래서 호출자는 동일한 lock 의 보호아래 추가적인 작업 수행이 가능해집니다.  
아래의 코드는 이러한 예제 중 하나입니다. get_lock() 함수는 mutex 의 lock 을 획득 하고 호출자에게 lock 을 반환하기 전에 prepare_date() 를 수행합니다.

```c++
std::unique_lock<std::mutex> get_lock()
{
	extern std::mutex some_mutex;
	std::unique_lock<std::mutex> lk(some_mutex);
	prepare_data();
	return lk; /* (1) */
}
void process_data()
{
	std::unique_lock<std::mutex> lk(get_lock()); /* (1) */
	do_something();
}
```

lk 는 함수안에서 automatic 변수로 선언되었기 때문에, 
이것은 std:move() 없이 직접적으로 반환 가능합니다; 
> 컴파일러는 move constructor 호출을 담당합니다.

The process_data() function can then transfer ownership directly into its own std::unique_lock instance (2) , 
and the call to do_something() can rely on the data being correctly prepared without another thread altering the data in the meantime.    
process_data() 함수는 (2) 의 std::unique_lock 인스턴스가 소유하고 있던 소유권을 직접적으로 이전시킬 수 있습니다,
그리고 do_somthing() 의 호출은 작업 시간 동안에 다른 스레드에 의해 데이터가 변질 없이 데이터가 준비되어야 합니다  ???


전형적으로 이런 mutex 가 lock 을 어디서 획득하지에 대한 문제는 현재 프로그램의 상태 또는 std::unique_lock 객체를 반환하는 함수의 전달 인자에 따릅니다.

One such usage is where the lock isn’t returned directly but is a data member of a gateway class used to ensure correctly locked access to some protected data.   
lock 획득에 대한 방법 중 하나는 직접적으로 lock 을 리턴하지는 않지만 클래스의 데이터 멤버는 보호된 데이터로의 접근에 대한 lock 이 정확하다는 것을 보장하는 gateway 클래스의 사용입니다. ??


이 케이스에서, 데이터로의 모든 접근은 이 gateway 클래스를 통합니다.
데이터에 접근하기를 원할 때, 락을 걸 수 있는 gateway class ( 앞선 예제에서 get_lock() 과 같은 함수 ) 의 객체를 획득 합니다.
그러면 gateway 객체의 멤버 함수를 통해 데이터에 접근할 수 있습니다.
작업이 끝나면, gateway 객체를 파괴하여, 락을 해제하고, 다른 스레드가 보호 데이터에 접근을 할 수 있도록 해야 합니다.
이 gateway 객체는 이동가능하고 ( 때문에 함수에 의해 반환 가능합니다 ), 이런 상황에선 lock 객체 데이터 멤버 역시 이동 가능해야 합니다.
std::unique_lock 에서는 인스턴스가 파괴되기 전에 그들의 lock 을 양도 하는 것을 허용합니다.
따라서 mutex 처럼 unlock() 멤버 함수를 이용해 할 수 있습니다.
std::unique_lock 은 mutex 와 같은 기본적인 locking 과 unlocking 멤버 함수 기능을 제공합니다. 
이를 제공합으로서 이것은 std::lock 과 같이 generic functions 과 같이 사용 가능합니다.
std::unique_lock 객체가 파괴되기전 lock 을 해제가 가능 하다는 건, 선택적으로 특정한 코드 branch 에서 lock 이 명백하게 필요 없어 졌을 때 lock 을 해제할 수 있음을 의미합니다.
이것은 애플리케이션의 성능에 매우 중요한 요소가 됩니다. 필요 이상으로 lock 을 잡고 있으면, lock 을 대기하는 다른 스레드가 필요이상으로 오래 진행을 방해 받기 때문입니다. 


### 3.2.8 Locking at an appropriate granularity

lock 의 granularity 는 이전 섹션 3.2.3 다뤘던 것 입니다 : 
> lock 의 granularity 는 single lock 에 의해 보호되는 데이터의 양을 설명하기 위한 hand-waving 용어입니다. 

세밀한 lock 은 작은 양의 데이터를 보호하고, 대단위 lock 은 많은 양의 데이터를 보호합니다.
대단위 lock 을 선택하여 데이터가 보호 된다는 것을 보장 하는 것 뿐 아니라, lock 이 실질적으로 필요한 때만 동작함을 보장하는 것 또한 매우 중요합니다.  

우리는 일상 생활에서 다음과 같은 매우 짜증 스러운 상황을 본 적이 있을 것 입니다.
당신은 슈퍼마켓에서 야채로 꽉찬 카트를 가지고 계산을 위해 줄서서 기다리고 있습니다. 그런데 이 때 계산을 받고 있던 사람이 갑자기 크랜배리 소스를 빠뜨린 것을 깨닫고는 기다리는 모두를 뒤로한채 그것을 찾으러 가버렸습니다.
또는 계산원은 계산할 준비를 하고 있고 손님은 돈을 지불하기 위해 지갑을 찾기 시작했습니다.
만약 모든 사람이 사려고 한 물건을 이미 다 준비하였고, 적절한 지불 수단을 미리 준비한 상태에서 체크아웃을 한다면 모든 작업 진행이 더 쉬워질 것 입니다.

이와 같은 일은 스레드에도 적용됩니다.  
만약 다중 스레드가 같은 자원을 기다리고 있다면 ( 계산하는 직원 ), 
그리고 어떤 스레드가 필요 이상으로 lock 을 획득 하고 있다면, 
이는 전체 대기 시간을 증가시킬 것 입니다. ( 당신이 계산대에 도착해서 크랜베리 소스를 찾기 전까진 기다리지 않습니다. )
한가지 가능한 방법은, 공유 데이터를 실제로 접근할때만 mutex 를 lock 하는 것입니다; 
특히, 파일 I/O 와 같은 시간을 오래 지체하는 작업은 lock 을 획득한 상태에서 하지 말아야 합니다.
파일 I/O 는 전형적으로 메모리에서 같은 양의 데이터를 읽거나 쓰는 것보다 수백배 ( 또는 수천 ) 느립니다.
그렇기 때문에 만약 파일로의 접근을 막는게 정말로 의도한게 아니라면,
lock 을 획득한 채 I/O 를 수행하는 것은 다른 스레드들을 불필요한 지연을 겪게 할 것 입니다 ( 왜냐하면 다른 스레드들은 그동안 lock 을 얻기위해 대기할 것 입니다. ), 
또한 다중 스레드의 이점 역시 사라질 것 입니다.
std::unique_lock 는 이런 상황에 유용합니다. 왜냐하면 코드가 더이상 공유 데이터에 접근할 필요가 없을 때 unlock() 을 호출할 수 있고, 다시 접근이 필요할 때 코드에서 다시 lock() 을 호출할 수 있기 때문입니다.


```c++
void get_and_process_data()
{
	std::unique_lock<std::mutex> my_lock(the_mutex);
	some_class data_to_process=get_next_data_chunk();
	my_lock.unlock(); /* (1) */
	result_type result=process(data_to_process);
	my_lock.lock(); /* (2) */
	write_result(data_to_process,result);
}
```

You don’t need the mutex locked across the call to process() , so you manually unlock it before the call (1) and then lock it again afterward (2) .  
당신은 process() 를 호출하기 위해 mutex 를 locked across 할 필요하 없기 때문에, 당신은 (1) 을 호출하기 전에 lock 을 수동적으로 해제해야 하고 lock 은 (2) 이후 다시 호출 될 것입니다. ??

Hopefully it’s obvious that if you have one mutex protecting an entire data structure, not only is there likely to be more contention for the lock, but also the potential or reducing the time that the lock is held is less.   
희망적으로 이것은 명백할 것입니다. 만약 당신이 하나의 mutex 를 가지고 전체 데이터 구조를 보호한다면, 더 많은 lock 경합 뿐만 아니라, lock 이 유지되는 시간을 줄일 가능성이 있습니다.

More of the operation steps will require a lock on the same mutex, so the lock must be held longer.   
같은 mutex 에 대한 lock 을 필요로 할 것이고, lock 은 더 오랜 시간 유지될 것입니다.

This double whammy of a cost is thus also a double incentive to move toward finer-grained locking wherever possible.  
두배의 비용은, 두배의 

As this example shows, locking at an appropriate granularity isn’t only about the amount of data locked; it’s also about how long the lock is held and what operations are performed while the lock is held.   
예로 보였듯이, 적절한 범위의 locking 이란 데이터 양에 대한 lock 뿐 아니라, lock 의 유지 시간과, lock 유지 동안에 어떤 작업을 할 것인가 입니다.

In general, a lock should be held for only the minimum possible time needed to perform the required operations.   
일반적으로, lock 은 가능한 필요한 작업을 하는데 걸리는 최소 시간만을 유지합니다.


This also means that time consuming operations such as acquiring another lock (even if you know it won’t deadlock) or waiting for I/O to complete shouldn’t be done while holding a lock unless absolutely necessary.  
이것을 또한 lock 이후 또 다른 lock ( 심지어 이것이 데드락에 빠지지 않는 다는 사실을 알고 있어도 ) 을 하거나, I/O 가 끝나는 것을 기다리는 것과 같은 시간을 많이 먹는 작업은 절대적으로 필요하지 않는한 해선 안된다는 뜻입니다. 


In listings 3.6 and 3.9, the operation that required locking the two mutexes was a swap operation, which obviously requires concurrent access to both objects.      
Listing 3.6 과 3.9 에서 교환 연산은 두개의 mutex 들에 대한 locking 을 필요로 합니다.   
명백하게 각각의 객체에 대한 접근이 동시에 필요할 한  ??

Suppose instead you were trying to compare a simple data member that was just a plain int .    
당신이 그저 int 형의 데이터 멤버를 비교하는 작업을 시도한다고 가정해 봅시다.


Would this make a difference? int s are cheap to copy, so you could easily copy the data for each object being compared while only holding the lock for that object and then compare the copied values.   
이것은 차이를 만들 까요? int s 는 복사 비용이 매우 적습니다, 그렇기 때문에 당신은 lock 이 유지되는 동안 
쉽게 각각 객체에 있는 data 를 복사할 수 있

This would mean that you were holding the lock on each mutex for the minimum amount of time and also that you weren’t holding one lock while locking another.   
이것은 당신이 각각의 mutex 를 최소한의 양만큼 lock 을 유지했다는 것을 뜻하고, 또한 locking 중 다른 lock 을 하지 않았다는 것을 뜻합니다.

The following listing shows a class Y for which this is the case and a sample implementation of the equality comparison operator.  
다음의 listing 에서 보여주는 것에 따르면 클래스 Y 는 이런 케이스를 보여주며, 평등 비교 연산을 하는 예를 보여줍니다.


####Listing 3.10 Locking one mutex at a time in a comparison operator
```c++
class Y
{
	private:
		int some_detail;
		mutable std::mutex m;
		int get_detail() const
		{
			std::lock_guard<std::mutex> lock_a(m); /* (1) */
			return some_detail;
		}
	public:
		Y(int sd):some_detail(sd){}
			friend bool operator==(Y const& lhs, Y const& rhs)
			{
				if(&lhs==&rhs)
					return true;
				int const lhs_value=lhs.get_detail(); /* (2) */
				int const rhs_value=rhs.get_detail(); /* (3) */
				return lhs_value==rhs_value; /* (4) */
			}
};
```
In this case, the comparison operator first retrieves the values to be compared by calling the get_detail() member function (2), (3) .  
이 예제에서, 비교 연산자는 (2)와 (3) 의 멤버 함수 get_detail() 에서 비교될 값을 첫번째로 검색합니다.


This function retrieves the value while protecting it with a lock (1) .  
이 함수는 (1) 이 lock 으로 보호되는 동안에 값을 검색합니다. 

The comparison operator then compares the retrieved values (4).  
비교 연산자는 검색한 값들을 비교합니다. (4)


Note, however, that as well as reducing the locking periods so that only one lock is held at a time (and thus eliminating the possibility of deadlock), this has subtly changed the semantics of the operation compared to holding both locks together.   
하지만 locking 기간이 줄어들어 한 순간에 오직 하나의 lock 이 유지하는 만큼 ( 이것은 데드락의 가능성을 줄입니다. ), 이것은 미묘하게 ???

In listing 3.10, if the operator returns true , it means that the value of lhs.some_detail at one point in time is equal to the value of rhs.some_detail at another point in time.  
listing 3.10 에서, 만약 연산자가 참을 반환하면, 이 시점에 하나의 포인트 값 lhs.some_detail 는 또다른 포인트 값rhs.some_detail 이 같다는 것을 의미합니다.

The two values could have been changed in any way in between the two reads; the values could have been swapped in between (2) and (3) , for example, thus rendering the comparison meaningless.   
이 값은 두 읽기과정 사이에서 변경되었을 수 있습니다; 
예를 들어, 이 값은 (2)와 (3) 사이에 무의미한 비교 렌더링 사이에서 교환되었을 수 있습니다.


The equality comparison might thus return true to indicate that the values were equal, even though there was never an instant in time when the values were actually equal.  
동이 값들이 실제로 같은 순간이 없었음에도 불구하고 동등 연산자는 값들이 같다는 뜻인 참을 리턴할 것입니다.

It’s therefore important to be careful when making such changes that the semantics of the operation are not changed in a problematic fashion:   
이런 문제적 상황에서는 조심하는게 중요하다? 

if you don’t hold the required locks for the entire duration of an operation, you’re exposing yourself to race conditions.  
만약에 전체 과정중에 필요한 순간에 lock 을 가지고 있지 않는다면, 당신은 당신 자신을 교착 상태에 빠뜨릴 것입니다.


Sometimes, there just isn’t an appropriate level of granularity because not all accesses to the data structure require the same level of protection.   
때때로 데이터 구조로의 모든 접근이 같은 수준의 보호를 필요하기 때문에 적절한 수준의 범위가 존재하지 않을 수 있습니다.

In this case, it might be appropriate to use an alternative mechanism, instead of a plain std::mutex .  
이런 상황에서는, 일반적인 std::mutex 대신에 다른 방식을 쓰는 것이 적절할 수 있습니다.



## 3.3 Alternative facilities for protecting shared data

Although they’re the most general mechanism, mutexes aren’t the only game in town when it comes to protecting shared data;  
이것들이 가장 일반적인 방식임에도 불구하고, mutex 들은 공유 데이터를 보호하는데 관한한 유일한 방법이 아닙니다.

there are alternatives that provide more appropriate protection in specific scenarios.  
이런 특정한 시나리오를 적절히 보호해 줄 대안들이 존재합니다.


One particularly extreme (but remarkably common) case is where the shared data needs protection only from concurrent access while it’s being initialized, but after that no explicit synchronization is required.   
극단적인 하나의 케이스는 ( 하지만 현저하게 일반적인  ) 공유 데이터가 오로지 초기화 과정에서 동시 접근할때만 보호가 필요합니다.
하지만 이후 명시적인 동기화는 필요하지 않습니다.

This might be because the data is read-only once created, and so there are no possible synchronization issues, or it might be because the necessary protection is performed implicitly as part of the operations on the data.   
이것은 데이터가 생성시 읽기 전용이기 때문입니다, 그리고 동기화 이슈에 관한 가능성이 전혀 없고, 


In either case, locking a mutex after the data has been initialized, purely in order to protect the initialization, is unnecessary and a needless hit to performance.  
어느 경우에나, 초기화를 온전히 보호하기 위해 데이터를 초기화 한 후에 mutex locking 하면, 이것은 불필요 하고, 필요없는 성능 히트? 이다.


It’s for this reason that the C++ Standard provides a mechanism purely for protecting shared data during initialization.  
C++ 표준이 초기화 중 공유 데이터를 보호하기 위한 메커니즘을 제공하는 이유이다.

### 3.3.1 Protecting shared data during initialization

Suppose you have a shared resource that’s so expensive to construct that you want to do so only if it’s actually required;   
당신에게 생성 비용이 비싸지만 정말 실제로 필요한 공유 자원이 있다고 가정해 봅니다. ??

maybe it opens a database connection or allocates a lot of memory.   
아마 이것은 데이터 베이스에 연결하거나 메모리의 많은 부분을 할당합니다.


Lazy initialization such as this is common in single-threaded code each operation that requires the resource first checks to see if it has been initialized and then initializes it before use if not:   
이러한 Lazy 초기화는 싱글 스레드 코드와 공통적입니다. 이는 각 연산에서 필요로 하는 자원이 있을때 이 자원이 초기화 됬는지 검사를 하고, 이것이 사용되기 전에 초기화 합니다.


```c++
std::shared_ptr<some_resource> resource_ptr;
void foo()
{
	if(!resource_ptr)
	{
		resource_ptr.reset(new some_resource); /* (1) */
	}
	resource_ptr->do_something();
}
```

If the shared resource itself is safe for concurrent access, the only part that needs protecting when converting this to multithreaded code is the initialization (1) ,  
만약 공유 자원이 동시 접근에서 안전하다면, 이 코드를 멀티스레드 코드로 변경할 때 보호가 필요한 부분은 오직 초기화 부분 (1) 입니다. 
   
but a naïve translation such as that in the following listing can cause unnecessary serialization of threads using the resource.   
하지만 다음에 나오는 listing 과 같은 순수번역? 은 자원을 이용하는?? 불필요한 스레드 직렬화를 야기 시킬 수 있습니다.


This is because each thread must wait on the mutex in order to check whether the resource has already been initialized.  
이것이 스레드가 mutex 를 기다리는 이유입니다. 이는 리소스가 이미 초기화 됬는지 여부를 체크합니다.


####Listing 3.11 Thread-safe lazy initialization using a mutex
```c++
std::shared_ptr<some_resource> resource_ptr;
std::mutex resource_mutex;
void foo()
{
	std::unique_lock<std::mutex> lk(resource_mutex);
	if(!resource_ptr)
	{
		resource_ptr.reset(new some_resource);
	}
	lk.unlock();
	resource_ptr->do_something();
}
```

This code is common enough, and the unnecessary serialization problematic enough, that many people have tried to come up with a better way of doing this, including the infamous Double-Checked Locking pattern:   
이코드는 충분히 일반적인 불필요한 직렬화 문제입니다. 이는 많은 사람들이 더 나은 방법을 위해 노력하는, locking 패턴을 두번씩 검사하게 하는 악명 높은 문제입니다.


the pointer is first read without acquiring the lock (1) (in the code below), and the lock is acquired only if the pointer is NULL .  
포인터는 (1) 에서 우선 lock 획득 없이 읽고 ( 아래 코드 ), 그리고 포인터가 NULL 일 경우만 lock 을 획득합니다.


The pointer is then checked again once the lock has been acquired (2) (hence the double checked part) in case another thread has done the initialization between the first check and this thread acquiring the lock:   
포인터는 lock 을 획득하기 전에 (2) 에서 다시 체크 합니다. ( 두번 체크 하는 부분입니다. ) 이 케이스에서 다른 스레드는 첫번째 체크와 이 스레드가 lock 을 획득하는 사이에 초기화를 마칩니다.


```c++
void undefined_behaviour_with_double_checked_locking()
{
	if(!resource_ptr) /* (1) */
	{
		std::lock_guard<std::mutex> lk(resource_mutex);
		if(!resource_ptr) /* (2) */
		{
			resource_ptr.reset(new some_resource); /* (3) */
		}
	}
	resource_ptr->do_something(); /* (4) */
}
```

Unfortunately, this pattern is infamous for a reason: it has the potential for nasty race conditions, because the read outside the lock (1) isn’t synchronized with the write done by another thread inside the lock (3) .   
불행히도, 이런 패턴은 다음과 같은 이유로 악명이 높습니다. 이것은 잠재적인 불쾌한? 교착 상태입니다, 왜냐하면 lock 없이 읽는 (1) 은 다른 스레드 안의 lock (3) 의 쓰기 작업으로 인해 동기화가 되지 않습니다.


This therefore creates a race condition that covers not just the pointer itself but also the object pointed to;   
even if a thread sees the pointer written by another thread, it might not see the newly created instance of some_resource , resulting in the call to do_something() e operating on incorrect values.   
그러므로 이것은 교착 상태를 만들고 



This is an example of the type of race condition defined as a data race by the C++ Standard and thus specified as undefined behavior.  
이는 C++ 에서 데이터 레이스로 정의되는 교착 상태의 한 타입의 예중 하나입니다. 따라서 정의되지 않은 행동으로 지정되었습니다.


It’s is therefore quite definitely something to avoid.   


See chapter 5 for a detailed discussion of the memory model, including what constitutes a data race.  
챕터 5에서는 메모리 모델에 대한 상세와, 무엇이 데이터 레이스를 구성하는 가에 대해 논의합니다.

The C++ Standards Committee also saw that this was an important scenario, and so the C++ Standard Library provides std::once_flag and std::call_once to handle this situation.   
C++ 표준 위원회 또한 이를 중요한 시나리오로 보고, 따라서 C++ 표준 라이브러리는 std::once_flag 와 std::call_once 를 제공하여 이런 상황을 조정할 수 있도록 제공합니다.


Rather than locking a mutex and explicitly checking the pointer, every thread can just use std::call_once , safe in the knowledge that the pointer will have been initialized by some thread (in a properly synchronized fashion) by the time std::call_once returns.  
mutex 를 lokcing 하는 것 보다, 그리고 명시적으로 포인터를 검사하는 것 보다, 모든 스레드가 std::call_once 를 사용ㅎ는 것만으로도, 포인터가 몇몇 스레드에 의해 초기화가 될 것임을 ( 적절한 동기화 아래 )  ????


Use of std::call_once will typically have a lower overhead than using a mutex explicitly, especially when the initialization has already been done, so should be used in preference where it matches the required functionality.  
std::call_once 의 사용은 전형적으로 명시적인 mutex 의 사용보다 오버헤드가 적고, 특히 초기화가 이미 끝나 우선 함수적으로 매치가 필요할 때 ???

The following example shows the same operation as listing 3.11, rewritten to use std::call_once .   
다음의 예는 listing 3.11 과 같은 작업을 하지만, std::call_cone 를 이용하여 다시 쓰였습니다.

In this case, the initialization is done by calling a function, but it could just as easily have been done with an instance of a class with a function call operator.   
이런 케이스에서, 초기화는 함수의 호출로 인해 끝났지만, 호출자 함수의 클래스 인스턴스 ??
 
Like most of the functions in the standard library that take functions or predicates as arguments, std::call_once works with any function or callable object.  
인수로 함수나 조건을 받는 표준 라이브러리의 대부분 함수들과 마찬가지로, std::call_once 는 어떠한 함수나 호출 가능한 객체?? 없이 수행합니다.

```c++
std::shared_ptr<some_resource> resource_ptr;
std::once_flag resource_flag; /* (1) */
void init_resource()
{
	resource_ptr.reset(new some_resource);
}
void foo()
{
	std::call_once(resource_flag,init_resource);
	resource_ptr->do_something();
}
```

In this example, both the std::once_flag (1) and data being initialized are namespace-scope objects, but std::call_once() can just as easily be used for lazy initialization of class members, as in the following listing.  
이 예제에서, 초기화 되는 std::once_flag (1) 과 데이터는 네임스페이스 범위 영역 객체입니다, 하지만 std::call_once 는 다음의 listing 에서와 같이 lazy 초기화 클래스 멤버로서 쉽게 쓸 수 있습니다.


####Listing 3.12 Thread-safe lazy initialization of a class member using std::call_once
```c++
class X
{
	private:
		connection_info connection_details;
		connection_handle connection;
		std::once_flag connection_init_flag;
		void open_connection()
		{
			connection=connection_manager.open(connection_details);
		}
	public:
		X(connection_info const& connection_details_):
			connection_details(connection_details_)
	{}
		void send_data(data_packet const& data) /* (1) */
		{
			std::call_once(connection_init_flag,&X::open_connection,this); /* (2) */
			connection.send_data(data);
		}
		data_packet receive_data() /* (3) */
		{
			std::call_once(connection_init_flag,&X::open_connection,this); /* (4) */
			return connection.receive_data();
		}
};
```

In that example, the initialization is done either by the first call to send_data() (1) or by the first call to receive_data() (3) .   
이 예제에서, 초기화는 (1) send_data() 의 첫 호출 또는 (3) receive_data() 의 첫 호출 때 이루어 집니다.


The use of the member function open_connection() to initialize the data also requires that the this pointer be passed in.  
데이터 초기화를 위한 멤버 함수 open_connection() 의 사용은 패스드 인 된 포인터를 필요로 합니다 ??

Just as for other functions in the Standard Library that accept callable objects, such as the constructor for std::thread and std::bind() ,   
표준 라이브러리에서 std::thread 나 std::bind() 의 생성자와 같은 호출가능한 객체의 접근 하는 다른 함수와 같이 ??

	 
this is done by passing an additional argument to std::call_once() (2) .  
이것은 (2) std::call_once() 에 추가적인 인자를 전달함으로서 이루어집니다.


It’s worth noting that, like std::mutex , std::once_flag instances can’t be copied or moved,   
이것은 말할필요도 없이 std::mutex 처럼 std::once_flage 인스턴스는 복사할수도 이동할수도 없습니다.

so if you use them as a class member like this, you’ll have to explicitly define these special member functions should you require them.  
그렇기 때문에 당신이 이처럼 클래스 멤버로 사용한다면, 당신은 명시적으로 이러한 특별한 멤버 함수가 필요하다는 것을 정의해야 합니다?


One scenario where there’s a potential race condition over initialization is that of a local variable declared with static .   
초기화 상황에서의 잠재적 교착 상태에 대한 시나리오는 정적으로 지역 변수를 선언하는 것 입니다.


The initialization of such a variable is defined to occur the first time control passes through its declaration;   
이러한 변수의 초기화는 처음 선언때 제어가 발생하도록 정의되어 있습니다. ??

for multiple threads calling the function, this means there’s the potential for a race condition to define first.   
함수에서 멀티 스레드를 부르기 위해서, ??

On many pre-C++11 compilers this race condition is problematic in practice, because multiple threads may believe they’re first and try to initialize the variable, or threads may try to use it after initialization has started on another thread but before it’s finished.    
멀티 스레드는 자신들이 처음으로 변수를 초기화 하고 있다고 믿고 있거나, 다른 스레드가 초기화가 미쳐 끝마치기 이전에 이 것을 사용하려고 합니다. 따라서 이런 교착 상태는 많은 C++11 이전의 컴파일러들에게 실질적 문제입니다.

   
In C++11 this problem is solved: the initialization is defined to happen on exactly one thread, and no other threads will proceed until that initialization is complete,   
C++11 에서 이런 문제가 해결되었습니다. : 초기화는 정확히 한 스레드에서 일어나도록 정의하고, 다른 어떤 스레드도 초기화가 마치기 전까지 진행하지 안습니다.
   

so the race condition is just over which thread gets to do the initialization rather than anything more problematic.   
그렇기 때문에 이런 초기화에서의 교착 상태는 어떤 문제도 되지 않습니다. ??


This can be used as an alternative to std::call_once for those cases where a single global instance is required:  
이것은 단일 전역 인스턴스가 필요로 하는 std::call_once 의 대안으로 사용 가능합니다.  ???? 


```c++
class my_class;
my_class& get_my_class_instance()
{
	static my_class instance; (1)
	return instance;
}
```

Multiple threads can then call get_my_class_instance() safely (1) , without having to worry about race conditions on the initialization.  
멀티 스레드는 초기화 과정에서의 교착 상태에 대한 어떠한 걱정없이 (1) get_my_class_instance() 를 안전하게 호출할 수 있습니다.


Protecting data only for initialization is a special case of a more general scenario:  
초기화 과정에서만의 데이터 보호는 좀더 일반적인 시나리오에서의 특별한 케이스입니다. ??


that of a rarely updated data structure. For most of the time, such a data structure is read-only and can therefore be merrily read by multiple threads concurrently, but on occasion the data structure may need updating. 
가끔씩 갱신되는 데이터 구조가 있습니다. 대부분의 경우, 이러한 데이터 구조는 읽기 전용이고, 멀티 스레드에 의해 동시적으로 쉽게 읽혀 집니다, 하지만 때때로 데이터 구조의 갱신이 필요할 때가 있습니다.


What’s needed here is a protection mechanism that acknowledges this fact.
이러한 사실을 인정하고 보호 메커니즘을 필요로 합니다.



### 3.3.2 Protecting rarely updated data structures

Consider a table used to store a cache of DNS entries for resolving domain names to their corresponding IP addresses. 
도메인 네임을 올바른 IP 주소로 풀어주는 DNS 엔트리의 캐시를 저장하는 테이블을 생각해 봅시다.


Typically, a given DNS entry will remain unchanged for a long period of time—in many cases DNS entries remain unchanged for years.
전형적으로 주어진 DNS 엔트리는 대부분의 경우 오랜시간 동안 변함 없이 몇년동안 남겨져 있을 것 입니다.


Although new entries may be added to the table from time to time as users access different websites, this data will therefore remain largely unchanged throughout its life.
그럼에도 불구하고, 유저이 다른 웹 사이트에 방문함에 따라 새로운 엔트리는 시간이 갈수록 테이블에 쌓여가고, 그러므로 데이터는 큰 변함없이 남아있을 것 입니다.


It’s important that the validity of the cached entries be checked periodically, but this still requires an update only if the details have actually changed.
이것이 바로 주기적인 캐시 엔트리의 유효성 검사의 중요성 입니다, 하지만 이것은 여전히 실질적 변화가 있을 때 갱신을 필요로 합니다.


Although updates are rare, they can still happen, and if this cache is to be accessed from multiple threads, it will need to be appropriately protected during updates to ensure that none of the threads reading the cache see a broken data structure.
정보의 갱신이 매우 적음에도 불구하고, 그 일은 여전히 발생하며, 만약 이 캐시에 멀티스레드가 접근한다면, 어떤 스레드도 broken 데이터 구조를 읽지 않음을 보장하도록 데이터 갱신동안의 적절한 보호를 필요로 할 것입니다.


In the absence of a special-purpose data structure that exactly fits the desired usage and that’s specially designed for concurrent updates and reads (such as those in chapters 6 and 7),
읽기와 갱신의 동시성을 고려하여 이에 맞게 디자인 ( 6 장과 7 장에 나온 ) 되거나 목적에 맞는 데이터 구조의 부재에서, 


such an update requires that the thread doing the update have exclusive access to the data structure until it has completed the operation. 
이러한 종류의 업데이트는 스레드가 작업이 완료될 때 까지 데이터 구조에 상호 배제적으로 접근하는 방식이 필요로 합니다.



Once the change is complete, the data structure is again safe for multiple threads to access concurrently. 
변경이 끝나면, 데이터 구조는 다시금 멀티 스레드의 동시적인 접근에서 안전해 집니다.


Using a std::mutex to protect the data structure is therefore overly pessimistic, because it will eliminate the possible concurrency in reading the data structure when it isn’t undergoing modification; what’s needed is a different kind of mutex.
std::mutex 를 데이터 구조 보호에 사용하는 것은 지나치게 비관적인 방법입니다, 데이터 구조가 수정 없이 읽기만 한다면 이는 데이터의 동시적 읽기의 가능성을 제거하게 됩니다. 따라서 우리는 다른 종류의 mutex 가 필요합니다.


This new kind of mutex is typically called a reader-writer mutex, because it allows for two different kinds of usage: exclusive access by a single “writer” thread or shared, concurrent access by multiple “reader” threads.
이러한 종류의 mutex 는 전형적으로 리더스-라이트 mutex 라고 부릅니다. 이는 두가지 종류의 사용법이 있는데 배타적으로 단 하나의 "wirter" 또는 공유 스레드만 접근 가능한 방법과, 동시적으로 읽기 가능한 "reader"  멀티 스레드 들 입니다.



The new C++ Standard Library doesn’t provide such a mutex out of the box, although one was proposed to the Standards Committee. 
새로운 C++ 표준 라이브러리는 표준 위원회에 제시되었음에도 불구하고, 이런 특별한 mutex 를 제공하지 않습니다.


Because the proposal wasn’t accepted, the examples in this section use the implementation provided by the Boost library, which is based on the proposal. 
이런 제안이 받아들여지지 않았기 때문에, 이번 섹션의 예제에서 Boost 라이브러리를 사용합니다. 이 Boost 라이브러리는 이런 제안에 기반됩니다.


As you’ll see in chapter 8, the use of such a mutex isn’t a panacea, and the performance is dependent on the number of processors involved and the relative workloads of the reader and updater threads. 
챕터 8 에서 다루게 되듯이, 이런 뮤텍스의 사용은 만변 통치약이 아닙니다, 그리고 성능은 프로세서의 갯수와 읽기와 갱신을 가진 스레드의 업무량에 좌우 됩니다.


It’s therefore important to profile the performance of the code on the target system to ensure that there’s actually a benefit to the additional complexity.
타겟 시스템에서 추가적인 복잡성이 실제로 이득이 있는지 확신하기 위해선, 코드의 성능을 프로파일링 하는 것은 매우 중요합니다.


Rather than using an instance of std::mutex for the synchronization, you use an	instance of boost::shared_mutex .   
동기화를 위하여 std::mutex 인스턴스를 사용하기 보단, boost::shard_mutex 인스턴스를 사용하세요.


For the update operations, std::lock_guard <boost::shared_mutex> and std::unique_lock<boost::shared_mutex> can be used for the locking, in place of the corresponding std::mutex specializations.   
갱신 작업을 위해서는, std::lock__guard <bosst::shared_mutex> 와 std::unique_lock<boost::shared_mutex> 는 std::mutex 를 대신하여 locking 할 수 있습니다.


These ensure exclusive access, just as with std::mutex . 
이는 std::mutex 와 같인 배타적인 접근을 보장합니다.


Those threads that don’t need to update the data structure can instead use boost::shared_lock<boost::shared_mutex>	to obtain shared access. 
이러한 스레드들은 boost::shared_lock<bost::shared_mutex> 를 사용하여 공유 접근을 얻음으로서 갱신이 필요 없습니다.


This is used just the same as std::unique_lock , except that multiple threads may have a shared lock on the same boost::shared_mutex at the same time. 
이는 std::unique_lock 처럼 쓰이는데, 차이점은 boost::shared_mutex 에서는 멀티 스레드가 동시에 공유 lock 을 가질 수 있습니다.


The only constraint is that if any thread has a shared lock, a thread that tries to acquire an exclusive lock will block until all other threads have relinquished their locks, and likewise if any thread has an exclusive lock, no other thread may acquire a shared or exclusive lock until the first thread has relinquished its lock.
공유 lock 의 유일한 제약조건은, 스레드가 배타적인 lock 을 획득하려고 하면 다른 모든 스레드가 lock 을 해제 하기 전까지 블록 당할 것 입니다. 그리고 스레드가 배타 lock 을 가지고 있다면, 다른 어느 스레드도 이 배타 lock 이 해제되기 전까진 공유 lock 이나 배타 lock 을 가질 수 없습니다.


The following listing shows a simple DNS cache like the one just described, using a	std::map to hold the cached data, protected using a boost::shared_mutex .
다음의 listing 은 위에서 묘사했던 간단한 DNS 캐시를 보여주며, std::map 을 이용하여 캐시 데이터를 보유하고, boost::shared_mutex 를 이용하여 보호합니다.


####Listing 3.13 Protecting a data structure with a boost::shared_mutex
```c++
#include <map>
#include <string>
#include <mutex>
#include <boost/thread/shared_mutex.hpp>

class dns_entry;
class dns_cache
{
	std::map<std::string,dns_entry> entries;
	mutable boost::shared_mutex entry_mutex;
public:
	dns_entry find_entry(std::string const& domain) const
	{
		boost::shared_lock<boost::shared_mutex> lk(entry_mutex); /* (1) */
		std::map<std::string,dns_entry>::const_iterator const it=
			entries.find(domain);
		return (it==entries.end())?dns_entry():it->second;
	}
	void update_or_add_entry(std::string const& domain,
			dns_entry const& dns_details)
	{
		std::lock_guard<boost::shared_mutex> lk(entry_mutex); /* (1) */
		entries[domain]=dns_details;
	}
};
```

In listing 3.13, find_entry() uses an instance of boost::shared_lock<> to protect it for shared, read-only access (1) ; 
listing 3.13 에서 find_entry() 는 boost::shraed_lock<> 인스턴스를 이용하여, 공유, (1) read-only 접근 를 합니다.


multiple threads can therefore call find_entry() simultaneously without problems. 
그렇기 때문에 다른 멀티 스레드들도 문제 없이 find_entry() 를 동시적으로 호출 가능합니다.


On the other hand, update_or_add_entry() uses an instance of std::lock_guard<> to provide exclusive access while the table is updated (2) ; 
반면에, update_or_add_entry() 는 std::lock_guard<> 인스턴스를 사용하는데, 이는 테이블이 (2) 갱신되는 동안 배타적인 접근을 제공합니다;


not only are other threads prevented from doing updates in a call update_or_add_entry() , but threads that call find_entry() are blocked too.
이는 다른 스레드가 update_or_add_entry() 를 호출하여 데이터를 갱신하는 것을 방지할뿐만 아니라, find_entry() 를 호출하는 다른 스레드 모두를 블록합니다.



### 3.3.3 Recursive locking

With std::mutex , it’s an error for a thread to try to lock a mutex it already owns, and attempting to do so will result in undefined behavior. However, in some circumstances it would be desirable for a thread to reacquire the same mutex several times without having first released it. 
std::mutex 에서는, mutex 가 lock 을 이미 가진 상태에서 다시금 lock 을 시도하면 에러가 발생합니다, 그리고 이는 정의되지 않은 결과를 발생시킬 것 입니다. 하지만, 스레드가 같은 mutex 를 기존 lock 의 해제 없이 얻어야 하는 상황이 있습니다. 


For this purpose, the C++ Standard Library provides std::recursive_mutex . It works just like std::mutex , except that you can acquire multiple locks on a single instance from the same thread. 
이런 경우를 위해, C++ 표준 라이브러리는 std::recursive_mutex 를 제공합니다. 이거는 std::mutex 와 동일한 기능을 하지만, 같은 스레드의 단일 인스턴스에 대해서 반복적으로  lock 을 획득 할 수 있습니다.


You must release all your locks before the mutex can be locked by another thread, so if you call lock() three times, you must also call unlock() three times.  
다른 스레드의 mutex 를 lock 하기 위해선 당신이 획득한 모든 lock 을 해제해야 합니다. 만약 lock() 을 세번 호출했다면, unlock() 역시 3번 호출해야 합니다.



Correct use of std::lock_guard <std::recursive_mutex> and std::unique_lock<std::recursive_mutex> will handle this for you.  
올바른 std::lock_gurad<std::recursivd_mutex> 와 std::unique_lock<std::recursive_mutex> 의 사용은 이러한 문제를 해결합니다.


Most of the time, if you think you want a recursive mutex, you probably need to change your design instead.  
만약 당신이 recursive mutex 를 사용하기 원한다면, 당신의 디자인을 바꿔야 할 것입니다.


A common use of recursive mutexes is where a class is designed to be accessible from multiple threads concurrently, so it has a mutex protecting the member data.   
일반적으로  recursive mutex 는 클래스에서 멤버 데이터에 대한 멀티 스레드의 동시적 접근으로부터 보호하기 위해 사용합니다.


Each public member function locks the mutex, does the work, and then unlocks the mutex. However, sometimes it’s desirable for one public member function to call another as part of its operation. In this case, the second member function will also try to lock the mutex, thus leading to undefined behavior. 
각각의 퍼블릭 멤버 함수는 mutex 를 lock 합니다. 그리고 mutex 를 unlock 합니다. 하지만 때때로, 하나의 퍼블릭 멤버 함수가 오퍼레이션의 한 일부분으로 서 불려지기를 원합니다. 이런 케이스에서, 두번째 멤버 함수는 mutex 에 lock 을 시도하고, 이로인해 정의 되지 않은 결과로 이끕니다.



The quick-and-dirty solution is to change the mutex to a recursive mutex.  
quick-and-dirty 한 해결 방법은 mutex 를 recursive mutex 로 바꾸는 것 입니다.


This will allow the mutex lock in the second member function to succeed and the function to proceed.
이것은 mutex lock 이 두번째 멤버 함수에 성공하고, 이 함수가 진행을 계속 하는 것을 허용합니다.


However, such usage is not recommended, because it can lead to sloppy thinking and bad design.  
하지만, 이러한 방법은 추천할 만한 방법은 아닌데, 안좋은 설계와 엉성한 생각으로 이끌 수 있기 때문입니다.


In particular, the class invariants are typically broken while the lock is held, which means that the second member function needs to work even when called with the invariants broken. 
클래스의 invariants 는 전형적으로  lock 이 유지되는 동안에 broken 되는데, 이것은 두번째 멤버 함수는 invariants 가 broken 된 상황에서 work 하기를 원한 다는 것을 뜻합니다. ????


It’s usually better to extract a new private member function that’s called from both member functions, which does not lock the mutex (it expects it to already be locked). 
이것은 대체로 새로운 private 멤버 함수를 extrat 하는 것이 mutex 가 lock 되지 않은 ( 이미 lock 되어 있을 거라고 예상하는 ) 함수를 호출하는 것보다 낫습니다.  ??


You can then think carefully about the circumstances under which that new function can be called and the state of the data under those circumstances.
이러한 상황에서의 새로운 함수나 데이터의 호출은 다시금 생각해 봐야 합니다.



## Summary

In this chapter I discussed how problematic race conditions can be disastrous when sharing data between threads and how to use std::mutex and careful interface design to avoid them.   
이번 챕터에서 저는 스레드 간 데이터 공유할 때 발생할 수 있는 교착 상태 문제와, std::mutex 사용법 그리고 이런 문제를 피할 인터페이스 구축을 하는 방법에 대하여 논의하였습니다.


You saw that mutexes aren’t a panacea and do have their own problems in the form of deadlock, though the C++ Standard Library provides a tool to help avoid that in the form of std::lock() .   
당신은 mutex 들이 만병 통치약이 아닌 것을 보았습니다. 또한 이것들이 가진 문제를 데드락 이란 형태로 보았습니다.
그래도 C++ 표준 라이브러리가 제공하는 std::lock() 란 형태의 도구를 제공하여 이를 피하는 방법을 보여주었습니다.


You then looked at some further techniques for avoiding deadlock, followed by a brief look at transferring lock ownership and issues surrounding choosing the appropriate granularity for locking.   
당신은 데드락을 피하는 몇가지 테크닉인 소유권 이전과 적절한 locking 범위를 선택하는 이슈를 간략히 보았습니다.


Finally, I covered the alternative data-protection facilities provided for specific scenarios, such as std::call_once() , and boost::shared_mutex .  
마지막으로, 저는 대안으로서 특정 상황에서 제공하는 데이터 보호 기능인 std::call_once() 와 boost::shared_mutex 와 같은 기능을을 다뤘습니다.


One thing that I haven’t covered yet, however, is waiting for input from other threads.   
하지만 한가지 제가 아직 다루지 않은 것이 있는데, 다른 스레드 부터의 입력을 기다리는 것 입니다.


Our thread-safe stack just throws an exception if the stack is empty, so if one thread wanted to wait for another thread to push a value on the stack (which is, after all, one of the primary uses for a thread-safe stack), it would have to repeatedly try to pop a value, retrying if an exception gets thrown.    
우리의 스레드 안정적인 스택은 만약 스택이 비어있다면 예외상황을 일으킬 것이고, 만약 한 스레드중 하나가 다른 스레드가 스택에 데이터를 넣기를 기다리고 있다면 ( 한 주요 유저가 스레드 세이프한 스택을 사용 ), 데이터를 반복적으로 pop 하는 것을 시도하고, 만약 에러가 발생하면 재시도를 합니다



This consumes valuable processing time in performing the check, without actually making any progress;   
이런 소비는 어떤 실제적인 작업을 진행하지 않고, 검사를 수행하는데 시간을 소비합니다.

indeed, the constant checking might hamper progress by preventing the other threads in the system from running.   
실제로, 실행중인 시스템에서 일정한 검사는 다른 스레드의 작업을 방해할 수 있습니다.
                                                                                                                                      

What’s needed is some way for a thread to wait for another thread to complete a task without consuming CPU time in the process.   
필요한 것은 다른 스레드가 작업을 완료시킬때 CPU 의 자원 소비없이 기다리는 방법 입니다.


Chapter 4 builds on the facilities I’ve discussed for protecting shared data and introduces the various mechanisms for synchronizing operations between threads in C++;   
챕터 4는 공유 데이터를 보호하기 위한 기능의 생성에 대해 논의하고, C++ 에서 스레드 간의 동기화 작업에 대한 다양한 메커니즘을 소개합니다.

chapter 6 shows how these can be used to build larger reusable data structures.  
챕터 6 에서는 이러한 재사용 가능한 데이터 구조를 어떻게 만드는지 보여줍니다.
