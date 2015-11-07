# 3. Sharing data between threads

### 3.2.6 Flexible locking with std::unique_lock

std::unique_lock 은 std::lock_guard 보다 조금더 유연한 기능을 제공합니다. std::unique_lock 인스턴스는 mutex 를 소유하지 않음으로서  invariants 를 완화시킵니다.  
생성자에 두번째 인자로 std::adopt_lock 을 전달하면 lock 객체가 mutex 의 lock 을 관리하고, 두번째 인자로 std::defer_lock 을 전달하면 생성시에 mutex 는 unlocked 상태로 남아있음을 나타냅니다.  
lock 은 이후 std::unique_lock 객체 ( mutex 가 아닌 ) 의 lock 을 호출하거나 std::unique_lock 객체를 std::lock 자신에 전달함으로서 얻을 수 있습니다.  
Listing 3.6 은 std::lock_guard 와 std::adopt_lock 을 std::unique_lock 와 std::defer_lock 로 대체하면 Listing 3.9 와 같이 쉽게 쓰일 수 있습니다.  
이 코드는 같은 라인수를 가지고, 크게 봤을 때  본질적으로 동일합니다. :    
std::unique_lock 은 std::lock_guard 보다 더 많은 공간을 가지고 부분적으로 보다 느립니다. std::unique_lock 인스턴스가 mutex 에 대한 소유권을 가지지 않음으로서 허용된 유연함은 비용을 유발합니다 : 이 mutex 의 소유권 정보는 저장되고, 업데이트 되어야 합니다.


#### Listing 3.9 Using std::lock() and std::unique_lock in a swap operation
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
};
```

Listing 3.9 에서, std::unique_lock 개체는 std::lock() 에 전달될 수 있습니다. 왜냐하면 std::unique_lock 은 lock(), try_lock() 그리고 unlock 멤버 함수를 지원하기 때문입니다.  
이런 동일한 이름을 가진 mutex 하위의 멤버 함수들은 실질적인 작업을 하고 std::unique_lock 인스턴스 내부의 플래그를 ( 현재 인스턴스가 mutex 를 소유하는지 나타내는 ) 바로 갱신 합니다.   
이 플래그는 소멸자에서 올바르게 unlock 이 호출되는 것을 보장하기 위해 필수적입니다.  
만약 인스턴스가 mutex 를 소유한다면, 소멸자는 unlcok 을 반드시 호출해야 하고, 만약 인스턴스가 mutex 를 소유하지 않으면, 이것은 unlock 을 호출해선 안됩니다. 
이 플래그는 owns_lock 멤버 함수를 호출하여 조회할 수 있습니다. 
당신이 예상하듯이, 이 플래그는 어딘가에 저장되어집니다.
그러므로, std::unique_lock 객체의 크기는 std::lock_guard 객체보다 크며, 또한 std::unique_lock 성능상 페널티로 작용합니다.
이는 std::lock_guard 위에 std::unique_lock 을 사용시 플래그가 적절히 갱신되어야 하기 때문입니다.
만약 std::lock_guard 가 당신의 필요성을 충분히 만족시킨다면, 이것을 환경설정에서 사용 하기를 추천합니다. 
이말은, std::unique_lock 이 수작업보다 유용한 케이스가 있다는 말입니다. 
이는 당신의 필요에 의해 추가적인 유연함을 사용하도록 하기 때문입니다.  
이 예제는 앞에서 이미 보았던 deferred locking 입니다. 
또다른 케이스는 락에 대한 소유권이 다른 범위로 이동하는 것 입니다.

### 3.2.7 Transferring mutex ownership between scopes


std::unique_lock 인스턴스는 그들 자신의 mutex 를 소유할 수 없기 때문에, mutex 의 소유권은 인스턴스 사이에 이동을 통해 전달 가능합니다.
몇몇 케이스에서 이러한 전송은 함수로부터 인스턴스의 리턴을 통해 자동적으로 전송되고, 또다른 케이스에서는 명시적으로 std::move() 함수 호출을 통해 이루어집니다.
근본적으로 이것은 소스가 lvalue ( 실제 값 ) 또는 참조자 또는 rvalue ( 임시적인 값의 한종류 ) 인지 여부에 의존적입니다.
만약 소스가 rvalue 이면 자동적으로 소유권이 전달되며, lvalue 에서는 변수로부터 소유권이 의도치 않은 양도를 피하기 위해 명시적으로 소유권을 전달해야 합니다.
std::unique_lock 은 이동가능하지만 복사 불가능한 타입의 예중 하나입니다. 부록 A 의 섹션 A.1.1 보면 에 더 많은 move semantic 에 대해 있습니다.
하나의 사용 가능한 방법은 함수가 mutex 에 대한 lock 과 호출자에 대한 lock 의 소유권 이전을 허용하는 것 입니다. 그래서 호출자는 같은 ;lock 의 보호아래 추가적인 액션 수행이 가능합니다.
아래의 코드는 이러한 예제의 한 단편입니다.

the function get_lock() locks the mutex and then prepares the data before returning	the lock to the caller:  
get_lock() 함수는 mutex 를 lock 하고 호출자에게 lock 을 반환하기 전에 prepare_date() 를 수행합니다.


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

lk 는 함수안에서 automatic 변수로 선언되었기 때문에, 이것은 std:move 없이 직접적으로 반환 가능합니다.  
컴파일러는 move constructor 호출을 담당합니다.

The process_data() function can then transfer ownership directly into its own std::unique_lock instance (2) , and the call to do_something() can rely on the data being correctly prepared without another thread altering the data in the meantime.  
process_data() 함수는 std::unique_lock 인스턴스가 소유하고 있던 소유권을 직접적으로 이전시킬 수 있습니다. 
그리고 do_somthing() 의 호출은 작업 시간 동안에 다른 스레드에 의해 데이터가 변질 없이 데이터가 준비되어야 하는 것에 의존적입니다.  

전형적으로 이런 mutex 가 lock 을 어디에 사용할지에 대한 패턴은 현재 프로그램의 상태 또는 std::unique_lock 객체를 반환하는 함수의 전달 인자에 의존됩니다.
또 다른 용법은 직접적으로 lock 을 리턴하지는 않지만 gateway 의 클래스의 데이터 멤버는 보호된 데이터로의 접근에 대한 lock 이 정확하다는 것을 보장합니다.
이런 케이스에서, 데이터로의 모든 접근은 이런 gateway class 를 통합니다.

when you wish to access the data, you obtain an instance of the gateway class (by calling a function such as get_lock() in the preceding example), which	acquires the lock.   
당신이 데이터에 접근하기를 원할 때, 당신은 락을 걸 수 있는 gateway class ( 앞선 예제에서 get_lock() 과 같은 함수 ) 의 객체를 획득 합니다.


You can then access the data through member functions of the gateway object.   
당신은 gateway 객체의 멤버 함수를 통해 데이터에 접근할 수 있습니다.


When you’re finished, you destroy the gateway object, which releases the lock and allows other threads to access the protected data.   
당신의 작업이 끝났을 때, 당신은 gateway 객체를 파괴하여, 락을 해제하고, 다른 스레드가 보호 데이터에 접근할 수 있도록 허용합니다.

Such a gateway object may well be movable (so that it can be returned from a function), in which case the lock object data member also needs to be movable.  
이러한 gateway 객체가 만약 이동가능하다면 ( 그래서 함수에 의해 반환 가능하다면 ), 이런 상황에선 lock 개체 데이터 멤버 역시 이동 가능해야 합니다.



The flexibility of std::unique_lock also allows instances to relinquish their locks before they’re destroyed.   
std::unique_lock 의 유연성은 인스턴스가 파괴되기 전에 그들의 lock 을 포기하는 것을 허용 합니다.


You can do this with the unlock() member function, just like for a mutex:   
당신은 mutex 처럼 unlock() 멤버 함수를 이용해 할 수 있습니다.


std::unique_lock supports the same basic set of member functions for locking and unlocking as a mutex does, in order that it can be used with generic functions such as std::lock .   
std::unique_lock 은 mutex 의 기본적인 locking 과 unlocking 멤버 함수 기능을 제공합니다. 이를 제공합으로서 이것은 std::lock 과 같은 generic functions 과 같이 사용 가능합니다.


The ability to release a lock before the std::unique_lock instance is destroyed means that you can optionally release it in a specific code branch if it’s apparent that the lock is no longer required.   
std::unique_lock 객체가 파괴되기전 lock 을 해제하는 기능은 당신이 선택적으로 특정한 코드 branch 에서 lock 이 더이상 필요 없다는 것이 명백할 때 lock 을 해제할 수 있음을 의미합니다.



This can be important for the performance of the application; holding a lock for longer than required can cause a drop in performance, because other threads waiting for the lock are prevented from proceeding for longer than necessary.
이것은 애플리케이션의 퍼포먼스에 매우 중요한 요소가 됩니다. 필요 이상으로 lock 을 잡고 있으면, lock 을 대기하는 다른 스레드가 필요이상으로 오래 진행을 방해 받기 때문입니다. 

### 3.2.8 Locking at an appropriate granularity

The granularity of a lock is something I touched on earlier, in section 3.2.3: the lock granularity is a hand-waving term to describe the amount of data protected by a single lock. 
lock 의 단위

A fine-grained lock protects a small amount of data, and a coarse-grained lock protects a large amount of data. 


Not only is it important to choose a sufficiently coarse lock granularity to ensure the required data is protected, but it’s also important to ensure that a lock is held only for the operations that actually require it. 


We all know the frustration of waiting in the checkout line in a supermarket with a cart full of groceries only for the person currently being served to suddenly realize that they forgot some cranberry sauce and then leave everybody waiting while they go and find some, or for the cashier to be ready for payment and the customer to only then start rummaging in their purse for their wallet.

Everything proceeds much more easily if everybody gets to the checkout with everything they want and with an appropriate means of payment ready.

The same applies to threads: 

if multiple threads are waiting for the same resource (the cashier at the checkout), then if any thread holds the lock for longer than necessary, it will increase the total time spent waiting (don’t wait until you’ve reached the checkout to start looking for the cranberry sauce). 


Where possible, lock a mutex only while actually accessing the shared data; try to do any processing of the data outside the lock. 

In particular, don’t do any really time-consuming activities like file I/O while holding a lock. 

File I/O is typically hundreds (if not thousands) of times slower than reading or writing the same volume of data from memory. 

So unless the lock is really intended to protect access to the file, performing I/O while holding the lock will delay other threads unnecessarily (because they’ll block while waiting to acquire the lock), potentially eliminating any performance gain from the use of multiple threads.

 std::unique_lock works well in this situation, because you can call unlock() when the code no longer needs access to the shared data and then call lock() again if	access is required later in the code:

```c++
void get_and_process_data()
{
	std::unique_lock<std::mutex> my_lock(the_mutex);
	some_class data_to_process=get_next_data_chunk();
	my_lock.unlock();
	result_type result=process(data_to_process);
	my_lock.lock();
	write_result(data_to_process,result);
}
```

You don’t need the mutex locked across the call to process() , so you manually unlock it before the call B and then lock it again afterward c .

Hopefully it’s obvious that if you have one mutex protecting an entire data structure, not only is there likely to be more contention for the lock, but also the potential or reducing the time that the lock is held is less. 

More of the operation steps will require a lock on the same mutex, so the lock must be held longer. 

This double whammy of a cost is thus also a double incentive to move toward finer-grained locking wherever possible.


As this example shows, locking at an appropriate granularity isn’t only about the amount of data locked; it’s also about how long the lock is held and what operations are performed while the lock is held. 

In general, a lock should be held for only the minimum possible time needed to perform the required operations. 

This also means that time consuming operations such as acquiring another lock (even if you know it won’t deadlock) or waiting for I/O to complete shouldn’t be done while holding a lock unless absolutely necessary.

In listings 3.6 and 3.9, the operation that required locking the two mutexes was a swap operation, which obviously requires concurrent access to both objects. 

Suppose instead you were trying to compare a simple data member that was just a plain int .

Would this make a difference? int s are cheap to copy, so you could easily copy the data for each object being compared while only holding the lock for that object and then compare the copied values. 

This would mean that you were holding the lock on each mutex for the minimum amount of time and also that you weren’t holding one lock while locking another. 

The following listing shows a class Y for which this is the case and a sample implementation of the equality comparison operator.

####Listing 3.10 Locking one mutex at a time in a comparison operator
```c++
class Y
{
	private:
		int some_detail;
		mutable std::mutex m;
		int get_detail() const
		{
			std::lock_guard<std::mutex> lock_a(m);
			return some_detail;
		}
	public:
		Y(int sd):some_detail(sd){}
			friend bool operator==(Y const& lhs, Y const& rhs)
			{
				if(&lhs==&rhs)
					return true;
				int const lhs_value=lhs.get_detail();
				int const rhs_value=rhs.get_detail();
				return lhs_value==rhs_value;
			}
};
```
In this case, the comparison operator first retrieves the values to be compared by calling the get_detail() member function c , d . 

This function retrieves the value while protecting it with a lock B . 

The comparison operator then compares the retrieved values e . 

Note, however, that as well as reducing the locking periods so that only one lock is held at a time (and thus eliminating the possibility of deadlock), this has subtly changed the semantics of the operation compared to holding both locks together. 

In listing 3.10, if the operator returns true , it means that the value of lhs.some_detail at one point in time is equal to the value of rhs.some_detail at another point in time.

The two values could have been changed in any way in between the two reads; the values could have been swapped in between c and d , for example, thus rendering the comparison meaningless. 

The equality comparison might thus return true to indicate that the values were equal, even though there was never an instant in time when the values were actually equal. 

It’s therefore important to be careful when making such changes that the semantics of the operation are not changed in a problematic fashion: 

if you don’t hold the required locks for the entire duration of an operation, you’re exposing yourself to race conditions.

Sometimes, there just isn’t an appropriate level of granularity because not all accesses to the data structure require the same level of protection. 

In this case, it might be appropriate to use an alternative mechanism, instead of a plain std::mutex .


## 3.3 Alternative facilities for protecting shared data

Although they’re the most general mechanism, mutexes aren’t the only game in town
when it comes to protecting shared data; there are alternatives that provide more
appropriate protection in specific scenarios.
One particularly extreme (but remarkably common) case is where the shared data
needs protection only from concurrent access while it’s being initialized, but after that
no explicit synchronization is required. This might be because the data is read-only
once created, and so there are no possible synchronization issues, or it might be
because the necessary protection is performed implicitly as part of the operations on
the data. In either case, locking a mutex after the data has been initialized, purely in
order to protect the initialization, is unnecessary and a needless hit to performance.
It’s for this reason that the C++ Standard provides a mechanism purely for protecting
shared data during initialization

### 3.3.1 Protecting shared data during initialization
Suppose you have a shared resource that’s so expensive to construct that you want to
do so only if it’s actually required; maybe it opens a database connection or allocates a
lot of memory. Lazy initialization such as this is common in single-threaded code—
each operation that requires the resource first checks to see if it has been initialized
and then initializes it before use if not:

```c++
std::shared_ptr<some_resource> resource_ptr;
void foo()
{
	if(!resource_ptr)
	{
		resource_ptr.reset(new some_resource);
	}
	resource_ptr->do_something();
}
```

If the shared resource itself is safe for concurrent access, the only part that needs pro-
tecting when converting this to multithreaded code is the initialization B , but a naïve
translation such as that in the following listing can cause unnecessary serialization of
threads using the resource. This is because each thread must wait on the mutex in
order to check whether the resource has already been initialized.

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

the pointer is first read without acquiring the lock B (in the code below), and the lock is acquired only if the pointer is NULL .

The pointer is then checked again once the lock has been acquired c (hence the double checked part) in case another thread has done the initialization between the first check and this thread acquiring the lock:

```c++
void undefined_behaviour_with_double_checked_locking()
{
	if(!resource_ptr)
	{
		std::lock_guard<std::mutex> lk(resource_mutex);
		if(!resource_ptr)
		{
			resource_ptr.reset(new some_resource);
		}
	}
	resource_ptr->do_something();
}
```

Unfortunately, this pattern is infamous for a reason: it has the potential for nasty race conditions, because the read outside the lock B isn’t synchronized with the write done by another thread inside the lock d . 

This therefore creates a race condition that covers not just the pointer itself but also the object pointed to; 
even if a thread sees the pointer written by another thread, it might not see the newly created instance of some_resource , resulting in the call to do_something() e operating on incorrect values. 

This is an example of the type of race condition defined as a data race by the C++ Standard and thus specified as undefined behavior. It’s is therefore quite definitely something to avoid. 

See chapter 5 for a detailed discussion of the memory model, including what constitutes a data race.
The C++ Standards Committee also saw that this was an important scenario, and so the C++ Standard Library provides std::once_flag and std::call_once to handle this situation. 

Rather than locking a mutex and explicitly checking the pointer, every thread can just use std::call_once , safe in the knowledge that the pointer will have been initialized by some thread (in a properly synchronized fashion) by the time std::call_once returns.

Use of std::call_once will typically have a lower overhead than using a mutex explicitly, especially when the initialization has already been done, so should be used in preference where it matches the required functionality.

 The following example shows the same operation as listing 3.11, rewritten to use std::call_once . 
 
 In this case, the initialization is done by calling a function, but it could just as easily have been done with an instance of a class with a function call operator. 
 
 Like most of the functions in the standard library that take functions or predicates as arguments, std::call_once works with any function or callable object.

```c++
std::shared_ptr<some_resource> resource_ptr;
std::once_flag resource_flag;
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

In this example, both the std::once_flag B and data being initialized are
namespace-scope objects, but std::call_once() can just as easily be used for lazy ini-
tialization of class members, as in the following listing.

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
		void send_data(data_packet const& data)
		{
			std::call_once(connection_init_flag,&X::open_connection,this);
			connection.send_data(data);
		}
		data_packet receive_data()
		{
			std::call_once(connection_init_flag,&X::open_connection,this);
			return connection.receive_data();
		}
};
```

In that example, the initialization is done either by the first call to send_data() B
or by the first call to receive_data() d . The use of the member function open_
connection() to initialize the data also requires that the this pointer be passed in.
Just as for other functions in the Standard Library that accept callable objects, such as
the constructor for std::thread and std::bind() , this is done by passing an addi-
tional argument to std::call_once() c .

It’s worth noting that, like std::mutex , std::once_flag instances can’t be copied
or moved, so if you use them as a class member like this, you’ll have to explicitly
define these special member functions should you require them.

One scenario where there’s a potential race condition over initialization is that of a
local variable declared with static . 

The initialization of such a variable is defined to
occur the first time control passes through its declaration; for multiple threads calling
the function, this means there’s the potential for a race condition to define first. 

On many pre-C++11 compilers this race condition is problematic in practice, because
multiple threads may believe they’re first and try to initialize the variable, or threads
may try to use it after initialization has started on another thread but before it’s fin-
ished. In C++11 this problem is solved: the initialization is defined to happen on
exactly one thread, and no other threads will proceed until that initialization is com-
plete, so the race condition is just over which thread gets to do the initialization rather
than anything more problematic. 

This can be used as an alternative to std::call_once for those cases where a single global instance is required:

```c++
class my_class;
my_class& get_my_class_instance()
{
	static my_class instance;
	return instance;
}
```

Multiple threads can then call get_my_class_instance() safely B , without having to
worry about race conditions on the initialization.
Protecting data only for initialization is a special case of a more general scenario:
that of a rarely updated data structure. For most of the time, such a data structure is

read-only and can therefore be merrily read by multiple threads concurrently, but on
occasion the data structure may need updating. What’s needed here is a protection
mechanism that acknowledges this fact.

### 3.3.2 Protecting rarely updated data structures

Consider a table used to store a cache of DNS entries for resolving domain names to
their corresponding IP addresses. Typically, a given DNS entry will remain unchanged
for a long period of time—in many cases DNS entries remain unchanged for years.
Although new entries may be added to the table from time to time as users access dif-
ferent websites, this data will therefore remain largely unchanged throughout its life.
It’s important that the validity of the cached entries be checked periodically, but this
still requires an update only if the details have actually changed.
Although updates are rare, they can still happen, and if this cache is to be accessed
from multiple threads, it will need to be appropriately protected during updates to
ensure that none of the threads reading the cache see a broken data structure.
In the absence of a special-purpose data structure that exactly fits the desired
usage and that’s specially designed for concurrent updates and reads (such as those in
		chapters 6 and 7), such an update requires that the thread doing the update have
exclusive access to the data structure until it has completed the operation. Once the
change is complete, the data structure is again safe for multiple threads to access con-
currently. Using a std::mutex to protect the data structure is therefore overly pessi-
mistic, because it will eliminate the possible concurrency in reading the data structure
when it isn’t undergoing modification; what’s needed is a different kind of mutex.
This new kind of mutex is typically called a reader-writer mutex, because it allows for
two different kinds of usage: exclusive access by a single “writer” thread or shared,
	concurrent access by multiple “reader” threads.
	The new C++ Standard Library doesn’t provide such a mutex out of the box,
	although one was proposed to the Standards Committee. 3 Because the proposal
	wasn’t accepted, the examples in this section use the implementation provided by the
	Boost library, which is based on the proposal. As you’ll see in chapter 8, the use of
	such a mutex isn’t a panacea, and the performance is dependent on the number of
	processors involved and the relative workloads of the reader and updater threads. It’s
	therefore important to profile the performance of the code on the target system to
	ensure that there’s actually a benefit to the additional complexity.
	Rather than using an instance of std::mutex for the synchronization, you use an
	instance of boost::shared_mutex . For the update operations, std::lock_guard
	<boost::shared_mutex> and std::unique_lock<boost::shared_mutex> can be used
	for the locking, in place of the corresponding std::mutex specializations. These
	ensure exclusive access, just as with std::mutex . Those threads that don’t need to
	update the data structure can instead use boost::shared_lock<boost::shared_mutex>

	to obtain shared access. This is used just the same as std::unique_lock , except that
	multiple threads may have a shared lock on the same boost::shared_mutex at the
	same time. The only constraint is that if any thread has a shared lock, a thread that
	tries to acquire an exclusive lock will block until all other threads have relinquished
	their locks, and likewise if any thread has an exclusive lock, no other thread may
	acquire a shared or exclusive lock until the first thread has relinquished its lock.
	The following listing shows a simple DNS cache like the one just described, using a
	std::map to hold the cached data, protected using a boost::shared_mutex .

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
		boost::shared_lock<boost::shared_mutex> lk(entry_mutex);
		std::map<std::string,dns_entry>::const_iterator const it=
			entries.find(domain);
		return (it==entries.end())?dns_entry():it->second;
	}
	void update_or_add_entry(std::string const& domain,
			dns_entry const& dns_details)
	{
		std::lock_guard<boost::shared_mutex> lk(entry_mutex);
		entries[domain]=dns_details;
	}
};
```
In listing 3.13, find_entry() uses an instance of boost::shared_lock<> to protect it
for shared, read-only access B ; multiple threads can therefore call find_entry()
simultaneously without problems. On the other hand, update_or_add_entry() uses
an instance of std::lock_guard<> to provide exclusive access while the table is
updated c ; not only are other threads prevented from doing updates in a call update_
or_add_entry() , but threads that call find_entry() are blocked too.

### 3.3.3 Recursive locking

With std::mutex , it’s an error for a thread to try to lock a mutex it already owns, and
attempting to do so will result in undefined behavior. However, in some circumstances it
would be desirable for a thread to reacquire the same mutex several times without
having first released it. For this purpose, the C++ Standard Library provides
std::recursive_mutex . It works just like std::mutex , except that you can acquire
multiple locks on a single instance from the same thread. You must release all your
locks before the mutex can be locked by another thread, so if you call lock() three
times, you must also call unlock() three times. Correct use of std::lock_guard
<std::recursive_mutex> and std::unique_lock<std::recursive_mutex> will han-
dle this for you.
Most of the time, if you think you want a recursive mutex, you probably need to
change your design instead. A common use of recursive mutexes is where a class is
designed to be accessible from multiple threads concurrently, so it has a mutex pro-
tecting the member data. Each public member function locks the mutex, does the
work, and then unlocks the mutex. However, sometimes it’s desirable for one public
member function to call another as part of its operation. In this case, the second
member function will also try to lock the mutex, thus leading to undefined behav-
ior. The quick-and-dirty solution is to change the mutex to a recursive mutex. This
will allow the mutex lock in the second member function to succeed and the func-
tion to proceed.
However, such usage is not recommended, because it can lead to sloppy thinking
and bad design. In particular, the class invariants are typically broken while the lock
is held, which means that the second member function needs to work even when
called with the invariants broken. It’s usually better to extract a new private member
function that’s called from both member functions, which does not lock the mutex
(it expects it to already be locked). You can then think carefully about the circum-
stances under which that new function can be called and the state of the data under
those circumstances.

## Summary

In this chapter I discussed how problematic race conditions can be disastrous when
sharing data between threads and how to use std::mutex and careful interface design
to avoid them. You saw that mutexes aren’t a panacea and do have their own problems
in the form of deadlock, though the C++ Standard Library provides a tool to help
avoid that in the form of std::lock() . You then looked at some further techniques
for avoiding deadlock, followed by a brief look at transferring lock ownership and
issues surrounding choosing the appropriate granularity for locking. Finally, I covered
the alternative data-protection facilities provided for specific scenarios, such as std::
call_once() , and boost::shared_mutex .
One thing that I haven’t covered yet, however, is waiting for input from other
threads. Our thread-safe stack just throws an exception if the stack is empty, so if one
thread wanted to wait for another thread to push a value on the stack (which is, after
		all, one of the primary uses for a thread-safe stack), it would have to repeatedly try to
pop a value, retrying if an exception gets thrown. This consumes valuable processing
time in performing the check, without actually making any progress; indeed, the con-
stant checking might hamper progress by preventing the other threads in the system
from running. What’s needed is some way for a thread to wait for another thread to
complete a task without consuming CPU time in the process. Chapter 4 builds on the
facilities I’ve discussed for protecting shared data and introduces the various mecha-
nisms for synchronizing operations between threads in C++; chapter 6 shows how
these can be used to build larger reusable data structures.

