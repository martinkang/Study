# 프로세스 동기화
* 협력적 프로세스의 질서있는 실행을 보장하고, 데이터의 일관성을 유지하기 위해서는 동기화를 필요로 한다.

## 6.2 임계영역 문제 ( The Critical Section Problem )

#### 용어
* 임계 영역 ( The Critical Section )
	- 병렬 컴퓨팅에서 둘 이상의 스레드가 접근하면 안되는 공유자원 ( 자료 또는 장치 ) 를
	접근하는 코드의 일부
* 진입 영역 ( Entry Section )
* 퇴출 영역 ( Exit Section )
* 나머지 영역 ( Remainder Section )

