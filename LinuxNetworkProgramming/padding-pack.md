# Padding/Pack

* cache line
	-지역성에 근거에 인접한 데이터를 미리 읽어옴으로써 속더 향상을 노린다.
	* Locality
		- 지금 읽힌 데이터는 이후에도 자주 사용될 가능성이 높다.
		- 지금 읽힌 데이터와 인접한 데이터는 이어서 사용될 가능성이 높다.

* Padding
	- 구조체를 사용할 때 구조체의 요소들은 오프셋에 따라서 접근되므로
	이 오프셋을 CPU 가 빠르게 접근할 수 있도록 임의의 Byte 를 채워 정렬한다.

* Pack
	- 위의 패딩을 수행하지 않고 강제로 주소 경계를 맞추지 않은 채로 끼어 넣는 것.
	- 시스템의 성능 저하를 가져올 수 있다.

## XDR : External Data Representation Standard
- 이기종 간에 데이터 교환시에 신경 써야 하는 부분을 명시하는 규약

- 일반적으로 프로그래밍 수준에서 크기 정렬은 크게 신경쓸 요소가 되지 않지만,
	통신 헤더나 이기종 간의 데이터 교환시에는 구조체 사용에서 
	문제가 발생할 수 있기 때문에 XDR 의 내용중 구조체를 사용하는 부분은 기억해둬야 한다.

- XDR 에서 권고하는 유닛 사이즈는 4Byte

- 보통 int 나 short, double 처럼 바이너리 멤버가 존재하는 구조체는 
주소 경계를 구조체 내의 가장 큰 멤버로 정렬하게 된다.
	- char, short, int 를 구조체에 선언한다면, 대체적으로 int 의 4Byte 주소 경계를 받게되어,
	데이터가 0x07 에서 끝나는 경우에 빈공간 1Byte 를 넣어 패딩하게 된다. 이는 기본적으로 일어나게 된다.


### 구조체 패딩 예제
```c++
typedef struct my_st_a 
{
	char str[9];
	char cnt[4];
}

typedef struct my_st_b
{
	char str[9];
	int cnt;
}

typedef struct my_st_c
{
	char str[9];
	short cnt;
}

```
다음과 같은 구조체가 있을 때
	*        sizeof( my_st_a ) : 13
	* offsetof( my_st_a, cnt ) : 0x0009
	*        sizeof( my_st_b ) : 16
	* offsetof( my_st_b, cnt ) : 0x000c
		- int 형 cnt 가 있으므로 4Byte 주소 경계로 정렬된다. 
		따라서 str[9] 이후 cnt 의 시작 주소는 12 인 c 가 되고
		나머지 공간은 padding 된다.

### 개별적으로 구조체에 pack 하기
```c++
typedef struct my_st_b
{
	char str[9];
	int cnt;
} __attribute__((packed)) MY_ST_B;
```

### 컴파일시 구조체 전체에 대해서 pack 하기
```c++
gcc --pack-struct ... ... -o pack
```


## 구조체를 지원하지 않는 언어로 작성된 경우에 패딩
- 구조체의 패딩 문제는 구조체를 지원하지 않는 언어와 
바이너리 통신을 할 때 문제가 발생한다.

- 구조체를 지원하지 않는 경우에는 1Byte 1Byte 읽어서 수치를 바꿔서 사용하게 되는데, 
	이 때 패딩이 발생하는 것을 제대로 이해하지 못하면,
	이기종의 프로그램에서 데이터를 변환하는데 어려움이 발생하거나 오류가 발생할 수 있다.

- 따라서 이기종 간의 데이터 교환시에는 애초부터 패딩을 발생시키지 않도록 구조체를 선언하는 것이 좋다.
