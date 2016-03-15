# IPC( Inter Process Communication )
* 정의 
	- 프로세스 간의 정보를 주고받는 메커니즘의 통칭

## SysV IPC
* SysV 방식의 API 를 사용하는 세마포어, 공유 메모리, 메시지 큐를 의미한다.
* 이들을 다루기 위해서 IPC 자원 관리를 위해서 유틸리티인 ipcs, ipcrm 을 제공한다.

> * SysV IPC 유틸리티  
>	- ipcs  : IPC status, 시스템 내의 SysV IPC 자원의 정보를 출력  
>	- ipcrm : IPC remove, 시스템 내의 SysV IPC 자원을 제거합니다.  

* SysV IPC 의 구성요소
	- IPC Key 
		- 자원을 얻기 위해 사용하는 일종의 해시 키
		- 정수인 key_t 형으로 표현
		- 고정된 키값을 사용해도 무방하나, 
		중복된 키를 사용하게 될 수 있으므로 중복키 검사를 해야 한다.  
		- 경로명을 이용하여 키값을 생성 할 수 있다.  
			
			> key_t ftok( const char *pathname, int proj_id )
			> 	* 같은 경로와 같은 proj_id 를 입력시 항상 같은 key_t 값이 리턴된다.
	- IPC ID
	- 소유권
	- 소유 권한 ( mode )

## POSIX IPC
