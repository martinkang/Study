# 3. Sharing data between threads

### 3.2.6 Flexible locking with std::unique_lock

std::unique_lock 은 std::lock_guard 보다 조금더 유연한 기능을 제공합니다. std::unique_lock 인스턴스는 mutex 를 선택적으로 소유하여 invariants 를 완화시킵니다.  
첫째로, 생성자에 두번째 인자로 std::adopt_lock 을 전달하면 lock 객체가 뮤텍스의 lock 을 관리하고, 두번째 인자로 std::defer_lock 을 전달하면 생성시에 mutex 는 unlocked 상태로 남아있음을 나타냅니다.  
lock 은 이후 std::unique_lock 객체 ( mutex 가 아닌 ) 의 락 또는 std::unique_lock 객체를 std::lock 자신에 전달함으로서 얻을 수 있습니다.  
Listing 3.6 은 std::lock_guard 와 std::adopt_lock 을 std::unique_lock 와 std::defer_lock 로 대체하면 Listing 3.9 와 같이 쉽게 쓰일 수 있습니다.  
이 코드는 같은 라인수를 가지고, 크게 봤을 때  본질적으로 동일합니다. : 

std::unique_lock takes more space and is a fraction slower to use than std::lock_guard. The flexibility of allowing a std::unique_lock instance not to own the mutex comes at a price: 
std::unique_lock 은 std::lock_guard 보다 더 많은 공간을 가지고 부분적으로 보다 느립니다. std::unique_lock 인스턴스의 유연함은 ( 뮤텍스에 대한 소유권을 가지지 않음 ) 비용을 유발하는데, 이 뮤텍스의 소유권 정보는 저장되고, 업데이트 되어야 합니다.


### Listing 3.9 Using std::lock() and std::unique_lock in a swap operation
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
			std::unique_lock<std::mutex> lock_a(lhs.m,std::defer_lock);
			std::unique_lock<std::mutex> lock_b(rhs.m,std::defer_lock);
			std::lock(lock_a,lock_b);
			swap(lhs.some_detail,rhs.some_detail);
		}
		c
};
```

Listing 3.9 에서, std::unique_lock 개체는 std::lock() 에 전달될 수 있습니다. 왜냐하면 std::unique_lock 은 lock(), try_lock() 그리고 unlock 멤버 함수를 지원하기 때문입니다.  
이러한 같은 이름을 가진 뮤텍스 하위의 동일한 이름의  멤버 함수들은 실질적인 작업과 std::unique_lock 인스턴스의 플래그를 바로 업데이트 합니다. 이 플래그는 현재 인스턴스가 뮤텍스를 쇼유하는지를 나타냅니다.  
이 플래그는 소멸자에서 올바르게 unlock 이 호출되는 것을 보장하기 위해 필수적입니다.  
만약 인스턴스가 뮤텍스를 소유한다면, 소멸자는 unlcok 을 반드시 호출해야 하고, 만약 인스턴스가 뮤텍스를 소유하지 않으면, 이것은 unlock 을 호출해선 안됩니다.

This flag can be queried by calling the owns_lock()	member function.  
이 플래그는 owns_lock 멤버 함수를 호출함으로서 쿼리할 수 있습니다.

As you might expect, this flag has to be stored somewhere.   
당신이 예상하듯이, 이 플래그는 어딘가에 저장되어집니다.

Therefore, the size of a std::unique_lock object is typically larger than that of a std::lock_guard object, and there’s also a slight performance penalty when using std::unique_lock over std::lock_guard because the flag has to be updated or checked, as appropriate.   
그러므로, std::unique_lock 객체의 크기는 std::lock_guard 객체보다 크며, 또한 이것은 std::unique_lock 성능의 페널티가 있습니다.
std::lock_guard 위에 std::unique_lock 을 사용시 이런 플래그는 적절히 갱신되어야 하기 때문입니다.

If std::lock_guard is sufficient for your needs, I’d therefore recommend using it in preference.  
만약 std::lock_guard 가 당신의 needs 를 충분히 만족한다면, 저는 이것을 환경설정에서 사용 하기를 추천합니다. 


That said, there are cases where std::unique_lock is a better fit for the task at hand,	because you need to make use of the additional flexibility.   
이말은, std::unique_lock 이 수작업보다 유용한 케이스가 있다는 말입니다. 이는   당신의 needs 가 이러한 추가적인 유연함을 사용하도록 하기 때문입니다.

One example is deferred	locking, as you’ve already seen; another case is where the ownership of the lock needs to be transferred from one scope to another.  
이 예제는 앞에서 이미 보았던 deferred locking 입니다. 또다른 케이스는 락에 대한 소유권이 다른 범위로 이동하는 것 입니다.

### 3.2.7 Transferring mutex ownership between scopes


Because std::unique_lock instances don’t have to own their associated mutexes, the	ownership of a mutex can be transferred between instances by moving the instances	around.   
std::unique_lock 객체는 그들의 뮤텍스를 소유할 수 없기 때문에, 뮤텍스의 소유권은 객체 사이에 이동을 통해 전달 가능합니다.

In some cases such transfer is automatic, such as when returning an instance from a function, and in other cases you have to do it explicitly by calling std::move() .  
몇몇 케이스에서 이러한 전송은 함수로부터 객체의 리턴을 통해 자동적이고, 또한 또다른 케이스에서는  명시적으로 std::move() 함수 호출을 통해 이루어집니다.

Fundamentally this depends on whether the source is an lvalue—a real variable or reference to one—or an rvalue—a temporary of some kind.   
근본적으로 이것은 소스가  lvalue ( 실제 값 ) 또는 참조자 또는 rvalue ( 임시적인 값의 한종류 ) 인지 여부에 의존적입니다.

Ownership transfer is automatic if the source is an rvalue and must be done explicitly for an lvalue in order to avoid accidentally transferring ownership away from a variable.   
소유권 이전은 만약 소스가 rvalue 이면 자동적이며 lvalue 에서는 변수로부터 소유권이 의도치 않은 양도를 피하기 위해 명시적으로 행해집니다.

std::unique_lock is an example of a type that’s movable but not copyable. See appendix A, section A.1.1, for more about move semantics.  
std::unique_lock 은 이동가능하지만 복사 불가능한 타입의 예중 하나입니다. 부록 A 의 섹션 A.1.1 보면 에 더 많은 move semantic 에 대해 있습니다.

One possible use is to allow a function to lock a mutex and transfer ownership of that lock to the caller, so the caller can then perform additional actions under the protection of the same lock.   
하나의 사용 가능한 방법은 함수가 뮤텍스에 대한 락과 호출자에 대한 락의 소유권 이전을 허용하는 것 입니다. 그래서 호출자는 같은 락의 보호아래 추가적인 액션 수행이 가능합니다.


The following code snippet shows an example of this:   
아래의 코드는 이러한 예제의 한 단편입니다.

the function get_lock() locks the mutex and then prepares the data before returning	the lock to the caller:  
get_lock 함수는 뮤텍스 락을 잡고 락이 호출자로부터 반환되기 전 데이터를 준비합니다.


```c++
std::unique_lock<std::mutex> get_lock()
{
	extern std::mutex some_mutex;
	std::unique_lock<std::mutex> lk(some_mutex);
	prepare_data();
	return lk;
}
void process_data()
{
	std::unique_lock<std::mutex> lk(get_lock());
	do_something();
}
```

Because lk is an automatic variable declared within the function, it can be returned directly (1) without a call to std:move() ; the compiler takes care of calling the move constructor.   
lk 는 함수안에서 automatic 변수로 선언되었기 때문에, 이것은 std:move 없이 직접적으로 반환 가능합니다.  
컴파일러는 move constructor 호출을 담당합니다.

The process_data() function can then transfer ownership directly into its own std::unique_lock instance (2) , and the call to do_something() can rely on the data being correctly prepared without another thread altering the data in the meantime.  
process_data() 함수는 std::unique_lock 의 소유하고 있던 소유권을 직접적으로 이전시킬 수 있습니다. 그리고 do_somthing() 의 호출은 작업 시간 동안에 다른 스레드에 의해 데이터가 변질 없이 데이터가 준비되어야 하는 것에 의존합니다.  


Typically this sort of pattern would be used where the mutex to be locked is dependent on the current state of the program or on an argument passed in to the function that returns the std::unique_lock object. 
전형적으로 이런 종류의 패턴의 락은 현재 프로그램의 상태 또는 std::unique_lock 객체를 반환하는 함수의 전달 인자에 의존됩니다.


One such usage is where the lock isn’t returned directly but is a data member of a gateway class used to ensure correctly
locked access to some protected data. 
또 다른 용법은 직접적으로 락이 리턴하지는 않지만 gateway 의 클래스의 데이터 멤버는 보호된 데이터로의 접근의 락이 정확하다는 것을 보장합니다.


In this case, all access to the data is through this gateway class:   
이런 케이스에서, 데이터로의 모든 접근은 이런 gateway class 를 통합니다.

when you wish to access the data, you obtain an instance of the gateway class (by calling a function such as get_lock() in the preceding example), which	acquires the lock.   
당신이 데이터에 접근하기를 원할 때, 당신은 락을 걸 수 있는 gateway class ( 앞선 예제에서 get_lock() 과 같은 함수 ) 의 객체를 획득 합니다.


You can then access the data through member functions of the gateway object.   
당신은 gateway 객체의 멤버 함수를 통해 데이터에 접근할 수 있습니다.


When you’re finished, you destroy the gateway object, which releases the lock and allows other threads to access the protected data.   
당신의 작업이 끝났을 때, 당신은 gateway 객체를 파괴하여, 락을 해제하고, 다른 스레드가 보호 데이터에 접근할 수 있도록 허용합니다.

Such a gateway object may well be movable (so that it can be returned from a function), in which case the lock object data member also needs to be movable.  
이러한 gateway 객체가 만약 이동가능하다면 ( 그래서 함수에 의해 반환 가능하다면 ), 이런 상황에선 락 개체 데이터 멤버 역시 이동 가능해야 합니다.



The flexibility of std::unique_lock also allows instances to relinquish their locks before they’re destroyed.   
std::unique_lock 의 유연성은 객체가 파괴되기 전에 그들의 락을 포기하는 것을 허용 합니다.


You can do this with the unlock() member function, just like for a mutex:   
당신은 뮤텍스 처럼 unlock() 멤버 함수를 이용해 할 수 있습니다.


std::unique_lock supports the same basic set of member functions for locking and unlocking as a mutex does, in order that it can be used with generic functions such as std::lock .   
std::unique_lock 은 뮤텍스의 기본적인 locking 과 unlocking 멤버 함수 기능을 제공합니다. 이를 제공합으로서 이것은 std::lock 과 같은 generic functions 과 같이 사용 가능합니다.


The ability to release a lock before the std::unique_lock instance is destroyed means that you can optionally release it in a specific code branch if it’s apparent that the lock is no longer required.   
std::unique_lock 객체가 파괴되기전 락을 해제하는 방법은 당신이 선택적으로 특정한 코드 branch 에서 락이 더이상 필요 없다는 것이 명백할 때 락을 해제할 수 있음을 의미합니다.



This can be important for the performance of the application; holding a lock for longer than required can cause a drop in performance, because other threads waiting for the lock are prevented from proceeding for longer than necessary.
이것은 애플리케이션의 퍼포먼스에 매우 중요한 요소가 됩니다. 필요 이상으로 락을 잡고 있으면, 락을 대기하는 다른 스레드가 필요이상으로 오래 진행을 방해 받기 때문이다. 

### 3.2.8 Locking at an appropriate granularity


## 3.3 Alternative facilities for protecting shared data

### 3.3.1 Protecting shared data during initialization

### 3.3.2 Protecting rarely updated data structures

### 3.3.3 Recursive locking

## Summary
