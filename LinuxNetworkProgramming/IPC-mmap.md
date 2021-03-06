# mmap ( 메모리 맵 )
* 메모리맵은 파일이나 장치를 메모리와 대응시켜서 메모리처럼 사용하는 방식.
	- 대응된 메모리맵은 포인터로 접근하므로 사용이 용이하다.
	- 파일로 attach 하여 사용하면 메모리와 파일 사이의 동기화가 편리하다.
	- 메모리의 내용이 변경되어도 곧바로 파일로 flush 하지 않고 임의이 시간에 하게 된다.
		* OS 마다 다르다
		* msync(2) 를 사용하면 메모리와 파일의 데이터를 동기화 한다. 
			* munmap 시 동기화에 대해 보장할 수 없기 때문에 명시적으로 msync() 를 수행해야 한다.
	- 크리티컬 섹션 보호에 신경써야한다. ( mmap 은 공유되므로 )
	- 메모리맵의 크기를 넘어서는 경우에 파일에는 영향을 주지 않는다.
* 예제
	- [mmap.c](https://github.com/martinkang/Study/tree/master/LinuxNetworkProgramming/ex/mmap_ex2.c)
		

## mmap 의 사용
> void *mmap( void *start, size_t length, int prot, int flags, int fd, off_t offset );
> 
> * mmap(2) 함수는 5번째 매개 변수인 fd 가 가리키는 위치로부터 offset 위치에서 length 길이만큼 할당

* 매개변수
	* void *start   
		- 할당받기 원하는 메모리의 시작 번지  
		- flags 가 MAP_FIXED 를 사용할 때만 가능    
		- 메모리 시작 번지는 페이지 크기의 배수여야 한다. ( 페이지 크기는 getpagesize() 로 알 수 있다. )  
		- 0 입력시 자동 할당  
	* prot  
	 	- 메모리 보호 메커니즘   
			- PROT_EXEC : 해당 페이지는 실행 가능  
			- PROC_READ : 해당 페이지는 읽기 가능  
			- PROC_WRITE : 헤당 페이지는 쓰기 가능  
			- PROC_NONE : 해당 페이지에 접근할 수 없음  
		- 플래그 값들은 OR 연산으로 비트 연산할 수 있다.  
	* flag  
		- 함수 작동 메커니즘  
			- MAP_FIEXD  
				- 원하는 메모리 시작 번지로부터 시작할 때 사용  
			- MAP_SHARED   
				- 메모리맵을 공
				- 파일과 내용을 동기화하여 메모리 변경시 파일도 변경   
			- MAP_PRIVATE  
				- 메모리맵을 공유하지도 않음.   
				- 원본 파일에 내용이 쓰여지지 않고 생성시 파일의 내용을 읽어만 온다.
				- 매핑 이후 변경되는 내용은 반영되지 않고 프로세스 내에서만 유효하다.
* SHARED 방식
	- 변경시 원본 파일에 변경된 내용이 적용된다.
	- 해당 메모리맵을 보고 있는 프로세스들은 변경된 내용을 볼 수 있다.
* PRIVATE 방식
	- copy on write 방식을 써서, 변경시 복사본을 생성한다.
	- 처음 시작할 때의 내용은 파일에서 가져오지만,
	그 다음부터는 메모리맵을 변경시켜도 파일에 저장하지 않고
	서로 다른 터미널에도 공유되지 않는다.
* 파일의 크기 확장 함수
	* 파일의 크기와 메모리 매핑
		- 존재하지 않거나 크기가 0인 파일은 메모리 매핑할 수 없음
		- 빈 파일 생성시 파일의 크기를 확장한 후 메모리 매핑을 해야함
		* 경로명을 사용한 파일 크기 확장
			- int truncate( const char *path, off_t length );
		* 파일 기술자를 사용한 파일 크기 확장 
			- int ftruncate( int fildes, off_t length );



