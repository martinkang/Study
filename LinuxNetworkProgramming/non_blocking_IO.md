# IO Non-Blocking 모드의 사용
* Blocking 모드와의 차이점
	- 넌블록킹 모드에서는 자원이 사용 불가능한 경우에는 블록킹 되지 않고 -1 로 발환되어 실패한다.
		- EAGAIN 이나 EWOULDBLOCK 에러가 발생. ( 리눅스에서는 두 값을 같은 값으로 명시 )
		- 기존 블록킹 모드는 시스템 에러나 연결 이상을 제외하고는 블록킹되어 대기상태가 된다.

##### 넌블록킹의 read(2) 의 반환값
> * 성공시 : 양수 반환  
>	- 성공적으로 읽어 들인 바이트 수  
>
> * 읽을 데이터가 없음 : 음수반환  
> 	- errno 가 EAGAIN, EWOULDBLOCK 일 경우  
>
> * 에러 발생 : 음수 반환  


##### 넌블록킹의 write(2) 의 반환값
> * 성공시 : 양수 반환  
> 	- 성공적으로 보낸 바이트 수   
>
> * 소켓 버퍼 공간 부족 : 음수 반환    
>	- errno 가 EAGAIN, EWOULDBLOCK 일 경우 사용 가능한 윈도우 크기를 모두 소모하고,  
>	송신측의 송신 버퍼에도 가용할 수 있는 빈 버퍼 공간이 없을 때    
>
> * 에러 발생 : 음수 반환  

- 넌블록킹 모드에서는 전송 요청한 바이트 수와 실제 전송 성공항 바이트 수가 차이가 날 수 있다.
- 블록킹 모드에서는 전송 요청한 크기만큼 다 전송할 때까지 블록킹되지만,
	넌블록킹 모드에서는 전송 가능한 만큼만 전송하고 빠져나온다.
	- 전송 성공했다는 의미는 실제로 전송한 것이 아닌 송신측 소켓의 송신 버퍼에 실제로 복사된 바이트 수 


## 블록킹과 넌블록킹 모드에서의 읽기의 문제

##### TCP 스트림 소켓으로부터 정해진 크기만큼 읽어 들이는 랩핑 합수
```c++
int recv_nbyte( int sd, void *rbuf, int len)
{
	int ret, len_recv = 0; /* len_recv : 현재까지 읽어 들인 바이트 수 */
	while ( len_recv < len )
	{
		if ( ( ret = recv( sd, (char*)&rbuf[len_recv], 0 ) ) <= 0 )
		{
			if ( errno == EINTR || errno == EAGAIN )
			{
				continue; /* restart for EINTR */
			}
			else
			{
				return ret; /* if 0, disconnect, otherwise error */
			}
		}
		len_recv += ret; /* cumulative received data */
	}
	return len_recv;
}
```
* 문제점
	* 블록킹 모드
		- 정해진 크기만큼 수신받지 못한 경우에 블록킹에 빠져서 무한 대기를 한다.
	* 넌블록킹 모드
		- 무한루프에 빠지고 성공할때까지 계속 실패하여 -1 을 리턴한다.
			- CPU 자원을 계속 소비한다.
	- 일반적으로 넌블록킹 수신 메커니즘은 데이터 수신 버퍼큐를 만들고 수신될 때마다 버퍼큐에 넣고, 
	처리 가능한 데이터 크기 ( chunk size ) 가 되었을 대 핸들러를 발생시키는 방식을 많이 쓴다.

* 성능 향상을 위해 버퍼큐와 핸들러 부분은 독립적으로 작동하기 위해 쓰레드로 설계하는 경우가 많다.
	- I/O 멀티플렉서 나 쓰레드를 사용한다.


## 블록킹과 넌블록킹 모드에서의 쓰기의 문제

##### 넌블록킹 모드에서의 send(2) 의 재전송의 예시 코드
```c++
while ( len_sbuf > 0 )
{
	/* sbuf : 보낼 데이터 버퍼, len_sbuf : sbuf 중에 보낼 데이터의 크기 */
	ret_snd = send( fd, sbuf, len_sbuf, 0 );
	if ( ret_snd == -1 )
	{
		if ( errno & ( EAGAIN | EWOULDBLOCK ) )
		{
			/* 소켓 버퍼 공간 부족, 나중에 다시 시도 혹은 적절한 처리 */
		}
		else if ( /* 그 외의 에러 처리 */ )
		{
			/* ... */
		}
		else
		{
			/* 전송 성공, 하지만 전송되어진 데이터 크기를 검사하여 덜 전송되어졌다면 다시 전송 */
		}

		len_sbuf =- ret_snd;
}
```

* 문제점
	* 블록킹 모드
		- 정해진 크기만큼 소켓 버퍼로 모두 복사할 때까지 블록킹에 빠진다.
	* 넌블록킹 모드
		- 넌블록킹에서는 요청한 데이터 전부가 전송되지 않을 수 있기 때문에 전부 전송될 대까지 에러가 발생한다.

		
* 넌블록킹 모드의 문제점 해결방안
	* 루프를 돌면서 재전송한다
		- 구현이 쉽지만 신뢰성이 떨어진다.
		- 일부분만 전송되는 경우는 소켓 버퍼의 공간이 부족한 경우므로 다음과 같은 경우이다.
			- 상대방 수신측의 프로그램이 빠르게 데이터를 처리하지 못하는 경우
			- 전송량에 비해서 수신측 네트워크 라인 속도가 받쳐주지 못하는 경우.
		- 이와 같은 경우 수신측의 수신 버퍼가 가득 차면 더 이상 데이터 전송이 안 되므로
		보낼 예정의 데이터가 송신측의 송신 버퍼에 쌓이게 되고, 
		언젠가는 이 데이터가 가득 차서 더 이상 전송이 힘들다.
		* 이 방법은 네트워크 구간의 전달 속도보다 CPU 루프 문을 처리하는 것이 더 빠를 것이 자명하므로
		루프 문을 통한 재전송은 아무런 소득을 얻지 못한다.
	* 큐와 타이머의 조합으로 재전송한다.
		- 큐에 전송을 대기하는 작업을 등록하고, 타이머는 등록된 작업들의 타임아웃을 검사하여
		타임아웃이 되기 전에 전송이 가능해지면 전송을 하는 방법
		- 소켓 송신 버퍼의 워터마크( watermark ) 를 검사할 수 있는 메커니즘이 필요하기 때문에
		I/O 멀티플렉싱 기법이 필요하다.