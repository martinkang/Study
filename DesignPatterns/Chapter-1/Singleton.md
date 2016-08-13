# Singleton Pattern

## 동기
* 만약 스레드 풀을 관리하는 객체가 여러개라면 어떻게 될 까?
	- 스레드 풀의 task 할당 및 해제를 여러 객체에서 관리한다면 
	중복 할당 및 할당을 못받는 스레드 등이 생겨 문제점이 발생할 것이다.
* 따라서 이런 스레드 풀, 파일 시스템, 디바이스 드라이버 등의 어떤 클래스 들은 유일한 인스턴스를 생성하여 접근을 관리하는 것이 좋다. 
	- 오직 한개의 클래스 인스턴스만을 갖도록 보장하고,
		이에 대한 전역적인 접근점을 제공한다.

* 간단한 예제

``` c++
public class ThreadPool 
{
Public:
	public static ThreadPool getInstance();

	Private
		static ThreadPool * _ThreadPoolManager;

	Protected 
		ThreadPool() {}
};

ThreadPool * ThreadPool::_ThreadPoolManager = NULL;

ThreadPool * ThreadPool::getInstance()
{
	if( ThreadPool == null) 
	{
		ThreadPoolManager = new ThreadPool();
	}
	return ThreadPoolManager;
}
```

* 예제
	- 사용자는 반드시 getInstance() 를 통해 접근해야 한다.
		- 초기값은 NULL 로 설정되고 실제 첫번째로 접근할 때 유일한 인스턴스를 생성한다.
	- 생성자가 Protected 로 선언된 이유?
		- 사용자가 임의로 Singleton 클래스에서 인스턴스를 생성하려고 new Singleton() 이라고 할 경우 컴파일 오류가 발생할 것이다.
		- 유일한 인스턴스의 관리가 가능해진다.


## 장점
	- 유일하게 존재하는 인스턴스로의 접근을 통제
	- namespace 를 좁힌다.
		- 전역 변수를 정의하여 발생하는 디버깅의 어려움을 막는다.
	- 연산 및 표현의 정제를 허용
	- 인스턴스의 개수를 변경하기가 자유롭다.


## 생길 수 있는 문제
* Singleton 을 전역 변수나 정적 객체로 정의하고 자동 초기화 하는 것은 충분하지 않다.
	- 정적 객체의 유일한 인스턴스만 선언되리라는 보장을 할 수 없다.
	- 정적 객체는 생성 시점이 명확하게 정의되지 않아, 
	다른 객체에서 이를 참조하고자 할 때 생성 순서가 문제가 될 수 있다.

