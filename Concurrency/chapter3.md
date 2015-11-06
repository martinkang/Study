# 3. Sharing data between threads

## 3.2.6 Flexible locking with std::unique_lock

std::unique_lock provides a bit more flexibility than std::lock_guard by relaxing
the invariants; 
std::unique_lock 은 불변성을 완화시킴으로서 std::lock_guard 보다 조금더 유연한 기능을  제공한다.
a std::unique_lock instance doesn’t always own the mutex that it’s
associated with. 
std::unique_lock 객체는 항상 mutex 를 소유하지 않는다.

First off, just as you can pass std::adopt_lock as a second argument
to the constructor to have the lock object manage the lock on a mutex, you can also
pass std::defer_lock as the second argument to indicate that the mutex should
remain unlocked on construction. 


The lock can then be acquired later by calling
lock() on the std::unique_lock object (not the mutex) or by passing the std::
unique_lock object itself to std::lock(). 


Listing 3.6 could just as easily have been
written as shown in listing 3.9, using std::unique_lock and std::defer_lock B
rather than std::lock_guard and std::adopt_lock. 

The code has the same line
count and is essentially equivalent, apart from one small thing: std::unique_lock
takes more space and is a fraction slower to use than std::lock_guard. The flexibility
of allowing a std::unique_lock instance not to own the mutex comes at a price: this
information has to be stored, and it has to be updated.


### Listing 3.9 Using std::lock() and std::unique_lock in a swap operation
```c++
class some_big_object;
void swap(some_big_object& lhs,some_big_object& rhs);55
Protecting shared data with mutexes
class X
{
	private:
		some_big_object some_detail;
		std::mutex m;
	public:
		X(some_big_object const& sd):some_detail(sd){}
		std::defer_lock
			b
			friend void swap(X& lhs, X& rhs)
			leaves mutexes
			{
				unlocked
					if(&lhs==&rhs)
						return;
				std::unique_lock<std::mutex> lock_a(lhs.m,std::defer_lock);
				std::unique_lock<std::mutex> lock_b(rhs.m,std::defer_lock);
				std::lock(lock_a,lock_b);
				Mutexes are
					swap(lhs.some_detail,rhs.some_detail);
				locked here
			}
		c
};
```
