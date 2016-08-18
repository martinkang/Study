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

protected: 
	ThreadPool() {}
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
