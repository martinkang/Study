#include <iostream>

using namespace std;

class ThreadPool 
{
public:
	static ThreadPool * getInstance();
	static void createThread();

private:
	static int mMaxNumThread;
	static int mNumThread;
	static ThreadPool * _ThreadPoolManager;

/* protected 로 선언한건 서브클래싱을 위해서이며 서브클래싱을 하지 않는다면 private 로 해도 괜찮다. */
protected: 
	ThreadPool() {};
};

int ThreadPool::mMaxNumThread = 5;
int ThreadPool::mNumThread = 0;
ThreadPool * ThreadPool::_ThreadPoolManager = NULL;

ThreadPool * ThreadPool::getInstance()
{
	if( _ThreadPoolManager == NULL ) 
	{
		_ThreadPoolManager = new ThreadPool();
	}
	return _ThreadPoolManager;
}

void ThreadPool::createThread()
{
	/* 멀티쓰레딩에서는 Lock 이 필요하다. 동시에 접근하였을 경우에 문제가 된다. */
	if ( mMaxNumThread > mNumThread )
	{
		mNumThread += 1;
		cout << "Thread Create Success NumOfThread : " << mNumThread << endl;
	}
	else
	{
		cout << "Thread Num : " << mNumThread << " No More Thred" << endl;
	}
}

int main( void )
{
	int i = 0;
	ThreadPool * sThreadPool = ThreadPool::getInstance();  

	for ( i = 0 ; i < 6; i++ )
		sThreadPool->createThread();

	return 0;
}
