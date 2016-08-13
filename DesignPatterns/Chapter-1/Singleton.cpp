#include <iostream>

class ThreadPool 
{
Public:
	static ThreadPool getInstance();

Private:
	static ThreadPool * _ThreadPoolManager;

Protected: 
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

int main( void )
{
	ThreadPool sTheadPool = ThreadPool::getInstance();  

	return 0;
}
