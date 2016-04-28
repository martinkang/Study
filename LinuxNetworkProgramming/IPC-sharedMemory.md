# Shared Memory ( 공유메모리 )

* 공유 메모리는 직접적으로 메모리에 접근하므로 IPC 자원 중에 데이터 교환에서 가장 빠른 성능을 보여줌
* 일반적으로 공유 메모리 연결도 파일 기술자처럼 fork(2) 를 통해서 자식 프로세스를 만들 때 상속 된다.
* 크리티컬 섹션에서의 data corruption 문제가 발생할 수 있다.
* 읽는 시점에서 쓰는 작업이 진행중일 때는 데이터를 미처 다 읽지 못하거나 잘못된 데이터를 읽을 수 있다.
* 공유 메모리 구현방식에는 SysV 와 POSIX 방식이 있다.
	* SysV
		- 가장 보편화 되있고 많은 시스템이 이 방식을 기본으로 제공함
		- 시스템 내에 존재하는 특정 공유 메모리 영역을 프로세스가 사용하는 가상 주소에 attach 하여 접근함
	* POSIX
		- 향후 호환성이 좋다.
		- 파일처럼 사용하되 그 파일의 위치가 공유되는 가상 메모리 공간에 만들어지는 개념
			- 그냥 메모리 처럼 사용하기 위해서는 mmap 을 사용해야 한다.


## SysV 공유메모리
> shmget(2)    

- 공유 메모리의 IPC id 를 얻는다.  

> shmat(2)   

- IPC id 에 대응하는 메모리 주소 번지를 가져온다.  

> shmctl(2)   

- 공유 메모리를 조작한다. 공유 메모리 정보를 가져오거나 제거할 수 있다.   

> shmdt(2)   

- 공유 메모리와의 대응을 푼다.   
- 대응을 풀어도 공유 메모리를 제거하기 전까지는 시스템에 존재한다.   

* 공유 메모리를 사용 중일 때 제거하게 된다면?
	- attach 된 프로세스가 1개 이상일 때 공유 메모리를 삭제하면, 삭제 예약으로 잡힌다. 
		- ipcs 상에서 status 가 dest 로 표시된다.
	- 제거 예약 이후 다른 프로세스가 공유 메모리를 획득하지 못하게 되고, 
	마지막 attach 프로레스가 detach 한 이후 공유 메모리가 파괴된다.
		- 다른 유닉스 벤더의 경우 즉시 시스템 오류를 일으키거나, 로컬 메모리인 heap 영역처럼 사용되기도 한다.

* 공유 메모리 사용중 프로세스 종료 된다면?
	- 공유 메모리는 프로세스 종료시 자동으로 detach 가 된다.
	- 생성된 공유 메모리는 파괴되지 않는다.
		- 다른 프로세스가 사용하거나 할 수 있다.
		- n attach 가 0 이 되어도 공유 메모리는 그대로 내용을 보존하고 시스템에 존재한다.

> int shmget( key_t key, int size, int shmflg )  
> IPC ID 값을 획득한다.  

 	* ket_t ket
		- IPC_PRIVATE  
			- 이 키는 0 으로 실제 키는 없고 ID 는 랜덤하게 생성된다.
		- ftok( const char *pathname, int proj_id )  
			- 경로와 id 를 이용하여 key 값 생성, 같은 경로와 같은 아이디는 항상 같은 key 값을 리턴한다.  

> int *shmat( int shmid, const void *shmaddr, int shmflg )
> 공유 메모리에 attach 한다.  

	* int shmid
		- IPC ID 값
	* const void *shmaddr
		- 공유 메모리와 연결할 가상 메모리 주소
		- NULL 을 넣을 경우 특정 주소를 받지 않고 아무 주소나 할당 된다.
		- 특적 주소를 넣을 때는 페이지 경계, 즉 페이지의 배수에 해당하는 주소값을 넣어야 한다.
			- SHM_RND 플래그를 shmflg 에 넣을 경우, 페이지 경계에 반내림으로 계산하여 할당한다.
	* int shmflg
		- 지정할 것이 없으면 0
		- SHM_RDONLY 를 사용할 경우 읽기 전용


> int shmctl( int shmid, int cmd, struct shmid_ds *buf )    
> 공유 메모리를 조작한다.  

 	* int cmd  
		- 동작을 지시하는 명령  
		* IPC_STAT  
			- IPC 자원의 정보 ( 생성자, 생성 시간, 접근 권한 등등 ) 을 읽어온다.  
			- shmid_ds 구조체를 사용하여 읽어온다.
		* IPC_SET  
			- IPC 자원의 정보( 권한 ) 을 변경  
		* IPC_INFO  
			- IPC 자원의 시스템 설정값을 읽어온다.
			- shminfo 구조체를 사용하여 읽어온다.
		* IPC_RMID
			- 시스템에서 IPC 자원 ( 공유메모리 ) 를 제거
			- 제거할 때 shmid_ds 정보가 필요 없다면 NULL 을 넣는다.

##### struct shmid_ds
```c++
strcut shmid_ds
{
	struct ipc_perm shm_perm;     /* 퍼미션 */
	size_t	        shm_segsz;    /* 세그먼트의 크기 ( byte ) */
	time_t			shm_atime;    /* 마지막으로 shmat() 을 호출한 시간 */
	time_t			shm_dtime;    /* 마지막으로 shmdt() 를 호출한 시간 */
	time_t			shm_ctime;    /* 공유 메모리 생성 시간 */
	pid_t			shm_cpid;     /* 생성자의 pid */
	pid_t			shm_lpid;     /* 마지막으로 작동한 프로세스의 pid */
	shmatt_t		shm_nattach;  /* 현재 접근한 프로세스의 수 */
}
```

* 상기 cmd 명령은 다른 SysV IPC 에서도 똑같이 사용되는 명령어이다.
	- semctl(2), msqctl(2) 에서 사용 가능하다.





## POSIX 공유 메모리
* POSIX 공유 메모리는 저수준의 파일 핸들링과 유사한 인터페이스를 제공
	- shm_open() 이 성공하면 파일 기술자를 반환, 이때부터 파일처럼 사용 가능
* POSIX 공유 메모리는 realtime 라이브러리에 존재하므로 "-lrt" 로 링크해줘야 한다.
* 공유 메모리는 프로세스가 종료하게 되면 자동으로 연결해제 되지만, 시스템에 남아 있게 된다.
	- unlink 를 해줘야 한다.


* 수행 절차
	* shm_open(3)
		- 공유 메모리의 파일 기술자를 얻음( 생성 )
	* mmap(2)
		- 공유 메모리의 기술자를 메모리맵으로 맵핑
	* shm_unlink(3)
		- 공유 메모리를 제거

