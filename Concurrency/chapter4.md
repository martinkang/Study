# Chapter 04, Synchronizing concurrent operations


## 4.1 Waiting for Event
이벤트가 일어났을 때 이를 쓰레드에서 확인하는 방법
1. 플래그를 반복적을 확인
2. ```std::this_thread::sleep_for()``` 를 사용하여 짧은 시간동안 대기하고 플래그 확인
  - 하지만 이 방법은 잠든 동안에는 권한을 얻기 힘들고, 오버슬립으로 인해 성능에도 영향을 준다.
3. C++ 표준 라이브러리의 이벤트 대기 기능을 이용.


### Waiting for a condition with condition variables : 조건 변수를 사용해 특정 조건 기다리기
C++ 표준 라이브러리에서 제공하는 2개의 조건변수.
  * ```<condition_variable>``` 라이브러리에 선언
    - 모두 적절한 동기화를 위해 뮤텍스와 함께 작동해야 한다.
  1. ```std::condition_variable```
    - unique_lock 만 사용
  2. ```std::condition_variable_any```
    - lockable object 를 사용 가능
    - 좀더 유연하다. 어느 점에서 유연하단 것인가?
  - ```notify_one()``` : 잠든 상태에서 깨어나 뮤텍스 잠금을 다시 획득, 상태를 다시 체크하고 조건이 만족한다면 잠긴 뮤텍스와 wait() 에서 반환한다. 조건이 충족하지 않으면 쓰레드는 뮤텍스 잠금을 해제하고, 대기를 다시 시작.
  - ```notify_all()``` : 
  - ```wait()``` : 뮤텍스를 해제하고 대기상태가 된다.
  *Spurious wakeup :  다른 스레드가 깨우지 않았는데 깨는 상황.

### Building a thread-safe queue with condition variables

## 4.2 Waiting for One-off events with futures
- *future* : 1회성 이벤트를 모델화 한 것.
  - 한번 이벤트가 발생해서 future 가 ready 가 되면, 그 future 는 다시 초기화 될 수 없습니다.
  - ```<future>``` 라이브러리 헤더 사용
  - *future* 객체는 동기화된 접근을 제공하지 안기 때문에, 병렬적 접근을 한다면 동기화 메커니즘을 활용해야 한다.
  1. ```std::future<>``` : unique futures
    - 관련된 이벤트를 표현하는 유일한 인스턴스. get 시에 move 를 하기 때문에 get 은 한번만 가능
  2. ```std::shared_future<>``` : shared future
    - 하나의 같은 이벤트에 대해서, 여러 인스턴스로 표현.
    - 저장된 이벤트가 발생하면 shared_future 의 모든 인스턴스는 동시에 ready 상태가 되고, 이벤트와 관련된 데이터에 접근
     
### Returning values from background tasks

### Associating a task with a future
### Passing tasks between threads
### Making (std::)promises
### Saving an exception for the future
### Waiting from multiple threads
## 4.3 Waiting with a time limit
## 4.4 Using synchronization of operations to simplify code
## 4.5 Summary

