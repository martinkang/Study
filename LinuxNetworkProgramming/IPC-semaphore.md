# Semaphore ( 세마포어 )
* 정의
	- 세마포어의 사전적 의미인 신호를 보내는 것을 기본으로 어떤 자원의 상태를 신호하는 메커니즘
* 종류
	- 카운팅 세마포어 ( counting semaphore )
		* 특정 자원의 개수가 한정되어 있을 때, n 개의 자원을 
	가져가는 것을 표시해두는 스위치.
		* 기본 원리는 자원을 가져 갈 때마다 자원은 줄어들고 사용이 끝나면
		다시 가져다 놓아 자원이 늘어나는 
		- P Operation ( wait )
		- V Operation ( post )

	- 이진 세마포어 ( binary semaphore )

	- 뮤텍스 ( MUTEX : Mutual Exclusion )
