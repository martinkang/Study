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
    - ```notify_one```
  2. ```std::condition_variable_any```
    - ㅁㅁ

### Building a thread-safe queue with condition variables

## 4.2 Waiting for One-off events with futures
### Returning values from background tasks
### Associating a task with a future
### Passing tasks between threads
### Making (std::)promises
### Saving an exception for the future
### Waiting from multiple threads
## 4.3 Waiting with a time limit
## 4.4 Using synchronization of operations to simplify code
## 4.5 Summary

