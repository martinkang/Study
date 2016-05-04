# 프로세스 복사 함수 fork(2)

## fork(2) 의 사용
> pid_t fork(void);

* 반환값
	- 0 : 자식 프로세스에게 리턴되는 값
	- >0 : 부모프로세스에게 리턴되는, 생성된 자식 프로세스의 PID
	- > -1 에러
	 
	
## fork(2) 의 특징
* 특징
	* 프로세스를 분기한다. ( 자식 프로세스를 생성 )
	* 자식은 부모 프로세스의 복사본이며, 코드(텍스트) 세그먼트는 복사하지 않고 공유한다.
	* 메모리 영역을 공유하지 않고 복사해서 사용한다.
	* 부모와 자식중 누가 먼저 실행될 지 모른다.
	* Copy on write ( COW )
		- 부모와 자식이 모든 영역을 공유하고 있다가 서로 메모리 이미지가 달라지는 시점에서 부분만 복사하는 것 
		( 복사의 단위는 page )
	* 장점
		- 멀티태스킹 : 복제된 자식 프로세스는 부모 프로세스와 독립적으로 작동
	* 단점
		- 복제된 프로레스 사이에 데이터를 주고받는 구조이고, 데이터 통신 처리에 드는 비용이 크다면 성능 하락이 일어날 수 있다.


## vfork(2)
* 특징
	* fork 후에 곧바로 exec 계열 함수를 호출하는 경우, 모든 메모리가 복사되었다가 해제되므로 무겁다는 단점을 해결하기 위해 사용한다.
	* 부모 프로세스와 자식 프로세스가 모든 것을 공유한다.
	* 자식부터 실행이 되는 것을 보장한다.
* 예제
	- [vfork.c](https://github.com/martinkang/Study/tree/master/LinuxNetworkProgramming/ex/vfork.c)


##### vfork 사용 예제
```c++
#include <stdio.h>                                                
#include <unistd.h>
#include <stdlib.h>

int glob = 0;

int main(void)
{
	int var = 1;
	pid_t pid;

	printf( "before fork\n" );

	pid = vfork();
	if ( pid < 0 )
	{
		fprintf ( stderr, "fork error\n" );
	}
	else if ( pid == 0 ) /* child */
	{
		glob++;
		var++;
	}
	else /* parent */
	{
		sleep(1);
	}

	printf ( "pid = %d, glob = %d, var = %d\n", getpid(), glob,   var );
	printf ( "after printf getpid() : %d\n", getpid() );
	exit(0);
}

```  
> ./a.out  
> before fork  
> pid = .., glob = 1, var = 2  
> pid = .., glob = 1, var = 2  
  

* system(3)
	- 쉘(sh) 를 실행시켜서 프로그램을 실행
	- 명령어가 실행되는 동안 함수는 블록되어지며, 이외에 SIGCHLD 시그널은 블록, SIGINT, SIGQUIT 시그널은 무시되어진다.
	- system(3) 은 이런 시그널이 블록킹 되어 외부에서 종료할 수 없게 되므로 종종 프로세스를 종료시킬 수 없다는 단점이 있다.

## fork(2) 와 리다이렉션

##### fork와 리다이렉션 예제
```c++
#include <stdio.h>                                                
#include <unistd.h>
#include <stdlib.h>

int glob = 0;

int main(void)
{
	int var = 1;
	pid_t pid;

	printf( "before fork\n" );

	pid = fork();
	if ( pid < 0 )
	{
		fprintf ( stderr, "fork error\n" );
	}
	else if ( pid == 0 ) /* child */
	{
		glob++;
		var++;
	}
	else /* parent */
	{
		sleep(1);
	}

	printf ( "pid = %d, glob = %d, var = %d\n", getpid(), glob,   var );
	printf ( "after printf getpid() : %d\n", getpid() );
	exit(0);
}

```
> (1)  
> ./a.out  
> before fork  
> pid = .., glob = 1, var = 2  
> pid = .., glob = 0, var = 1  
>  
> (2)  
> ./a.out > a.txt  
> cat a.txt  
> before fork  
> pid = .., glob = 1, var = 2  
> before fork  
> pid = .., glob = 0, var = 1  


* (2) 의 결과는 리다이렉션과 printf 때문이다.
	* write 함수는 버퍼링 없이 곧바로 출력되지만, printf 의 경우 리다이렉션인 경우, 전체 버퍼링이 적용된다.
	* 터미널에 실행할 경우 줄단위 버퍼링이 적용되므로 개행문자를 만나 바로 출력하지만,
	리다이렉션한 경우 전체 버퍼링이 적용되어 곧바로 flush 가 되지 않고 버퍼에 남아있는 상태가 된다.
	* fork 로 인해 파일 디스크립터가 복제되고, 버퍼도 복제되면서
	두번 출력하게 된다.
