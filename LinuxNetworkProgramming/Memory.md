# Memory

## 메모리 영역의 종류
* text
	- 일반적으로 프로그램의 실행 코드가 존재하는 영역.
	- 특수한 경우에 데이터 영역에 코드를 포함하여 컴파일 가능 
		- 보통 보안 관련 프로그래밍

* data
	- static 으로 사용되는 심벌을 가진 영역
	* 쓰임새에 따라서 3가지로 나뉜다.
		- 읽기 전용으로 초기화 되는 영역
			* .rodata 영역
			* const 로 선언되는 영역
			* 그 외에 시스템에서 사용된 각종 문자역 등이 이곳에 위치.
				- printf 문의 포맷 스트링이나 상수 문자열 등
		- 읽기/쓰기가 가능한 영역으로 초기화 되는 영역
			* .data 영역
			* 전역변수 중에 초기값을 가지는 경우
		- 초기화되지 않는 영역
			* BSS( Block Started by Symbol ) 역역
			* 전역변수로 초기화되지 않고 선언되거나 static 으로 선언된 영역
			* C 의 startup 함수가 main() 을 실행하기 전에 0(NULL) 로 초기화를 하게 되어 있습니다.

* stack
	- 메모리를 잡지 않아도 함수의 시작에서 자동으로 생성되고 파기되는 로컬 자동변수가 위치.
	- 해당 스코프를 벗어나면 스택의 프레임 릴레이에서 해제되므로 자연적으로 파기된다.

* heap
	- 메모리 할당 함수를 사용할 때 받는 메모리 영역
	- 자동으로 파기되지 않는다.

### 전역변수들의 형태에 따른 메모리 위치
	* int num;
		- bss 에 위치
	* int num = 1;
		- data 에 위치
	* char str[] = "hello world";
		- data 에 위치.
	* char *p_str= "hello world";
		- 문자열 자체는 rodata 에 위치하고 p_str 포인터 변수는 data 에 위치
	* const char str[] = "hello world";
		- 문자열 자체는 rodata 에 위치하고 str 변수 자체는 data 에 위치
	* static int i_val;
		- bss 영역에 위치


## 메모리 영역별 특성
* stack
	- 프레임릴레이(??)와 같은 기법을 통해 할당받아 오버헤드가 없으므로 빠르지만 그 사용량이 힙처럼 크진 않다.
		* 프레임릴레이 : 찾아보기
	- stack 에 할당된 변수는 스코프를 벗어나면 제거되므로 메모리 누수 문제도 없다.
	- 스택 메모리가 너무 커지면, 스택 사이즈제한 ( ulimit 로 확인 ) 에 걸려 SEGV ( segmentation violation ) 가 발생하고
	프로세스가 종료된다.
	- 들어오는 데이터의 크기가 좀처럼 크지 않고 해당 함수의 사용이 빈번하다면 VLA ( Variable Length Array ) 를 이용하여
	가변 길이의 배열을 사용하는 방법도 괜찮다.

### 스택 소모 예제
```c++
#define SZ_BUFFER 1024 * 1024

int exhaust_stack( int count )
{
	char buffer[SZ_BUFFER]; /* 1 Mib stack : define SZ_BUFFER = 1024 KiB */
	if ( count <= 0 )
	{
		printf( ">> reach break position, stop recursive function!!\n" );
		return 0;
	}
	sprintf( buffer, ">> exhaust 1MiB stack( addr : %p ), will more %d MiB...\n", buffer, count - 1 );
	printf( buffer );
	exhaust_stack( count - 1 );
	return 0;
}
```

### Variable Length Array
```c++
int exam_vla( int vlen)
{
	char buffer[vlen]; /* Using VLA */
	...
}
```
* heap


## 메모리 정렬 및 검색


## 메모리 락 ( Memory Lock )
