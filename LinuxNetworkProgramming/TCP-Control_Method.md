# TCP 의 효율적인 통신을 위한 제어 기법들

## Nagle Algorithm ( 네이글 알고리즘 ), Delayed ACK ( 지연 ACK )
* 네이글 알고리즘이란
	* 송신측에서 짧은 시간 내에 전송되는 작은 패킷들을 모아두었다가 ACK 를 수신하면 전송하는 방법.
	- IP 네트워크에서 데이터는 몇 곂의 헤더로 캡슐화되어 목적지로 전송된다. 
	만약 데이터의 크기가 적을 경우 데이터보다 헤더의 크기가 더 커지는 경우가 발생하는데 
	이런 작은 데이터가 자주 전송될 경우 네트워크 전송 효율이 떨어질 것이다.
		- 따라서 조금씩 여러번 보내지 않고 한번에 많이 보내는 방법을 써서 전송 효율을 증가시킨다.
	- 보낼 수 있는 데이터를 바로 패킷으로 만들지 않고, ACK 가 올때까지 전송을 중지하고
	ACK 가 도착한 시점에, 지금까지 버퍼에 모인 데이터를 패킷으로 만들어서 전송한다.

![Nagle]( https://github.com/martinkang/MyText/blob/master/LinuxNetworkProgramming/img/nagle.png )


``` c++
#define MSS "maximum segment size"


if there is new data to send
  if the window size >= MSS and available data is >= MSS
    send complete MSS segment now
  else
    if there is unconfirmed data still in the pipe
      enqueue data in the buffer until an acknowledge is received
    else
      send data immediately
    end if
  end if
end if

/* 출처 http://en.wikipedia.org/wiki/Nagle's_algorithm */
```

* Delayed ACK ( 지연 ACK )
	- 수신측에서 짧은 시간 동안에 도착한 데이터 패킷들에게 일일히 즉시 ACK 를 보내면 
	헤더만 딸린 패킷을 보내기 때문에 위에서 지적한 네트워크 전송 효율이 떨어지게 된다.
	- 수신측에서 패킷을 받았을 때 바로 ACK 를 주는 것이 아니라 시간을 좀더 기다리다가
	만약 패킷 전송할 게 생긴다면 이 때 ACK 를 함께 전송하는 방식.
	- RFC ( Request For Commets ) 에 따르면 500mesc 이내의 시간을 지연시킬 것을 명시
		- 대부분 OS 는 200msec 이내의 시간을 사용.
		- 따라서 대부분 매우 작은 시간의 딜레이만 발생.
		- 대부분의 응용 레벨의 데이터 프로토콜이 수신측에서 응답 데이터를 보내주도록 되어있는 경우가 많기 때문에,
		즉시 응답을 하는 프로토콜에서는 거의 지연이 없음 ( e.g. HTTP 프로토콜 )

## TCP 흐름제어	
* TCP 흐름제어 기법이란
    - 송신 측과 수신 측의 데이터 처리 속도 차이를 해결하기 위한 기법.
    - 수신 측에서 수신된 데이터를 처리해서 상위 계층으로 서비스 하는 속도보다 
    송신 측에서 보내는 데이터의 속도가 더 빠르다면 수신 측에서는 제한된 저장용량을 초과하여 
    이후에 도착하는 데이터는 손실될 수 있다.
    - 이러한 위험을 줄이기 위해 송신 측의 데이터 전송 량을 수신 측에 따라 조절하는 방법.

    * 정지대기방식 ( Stop and Wait )
        - 송신측에서 1개의 프레임을 송신하고, 수신측에서 수신된 프레임의 에러 유무를 판단하여 송신층에 ACK 나  NAK 를 보내는 방식
        - 송신측은 수신측으로 ACK 를 수신했을 경우에만 다음 프레임을 전송.
        - 수신측으로부터 NAK 를 수신하거나, 일정시간까지 응답프레임을 수신하지 못하면 해당 프레임을 재전송
        - 장점
            - 구현이 단순
            - 송신측내에 최대 프레임 크기의 버퍼를 1개만 잡아도 됨
        - 단점
            - 송신측이 ACK 를 수신할 때까지 다음 프레임을 전송할 수 없으므로 전송효율이 떨어짐
        * 용어
        	- ACK ( Acknowledge )
        		- 한 컴퓨터가 일련의 데이터를 네트웍을 통해 다른 컴퓨터로 보낼 때, 만약 그 데이터 전송이 성공적이였을 때 
        		수신측 컴퓨터가 보내주는 응답 문자.
        	- ARQ ( Automatic Repeat Reques )
        		- 수신측이 송신측에게 손상된 데이터를 재 송신해줄 것을 요구하는 에러 통제 프로토콜.
        	- NAK ( Negative Acknowledgment, Not Acknowledged )
        		- 통신 제어용 신호로서, 데이터가 수신에 에러가 있었음을 확인하는 신호. 
        		- NAK 신호는 때로 '거절되었다'는 의미를 갖고 있는 REJ 신호로 불리기도 한다. 
        		- 이를 대체할 수 있는 신호로는 자동으로 재전송을 요구하는 신호인 ARQ가 있다.
            
    * 슬라이딩 윈도우 ( Sliding window )
        - 두 개의 네트워크 호스트간의 패킷의 흐름을 제어하기 위한 방법.
		- TCP 처럼 신뢰적인 데이터 통신을 하려면 유효 데이터에 대해서 응답을 받아야만, 다음 데이터를 보낼 수 있다.
		하지만 이렇게 프로토콜을 디자인하면 너무 느리다.
			- 이런 문제점을 해결하기 위해 제안된 방법.
        - 연속으로 프레임을 보낼 수 있는 공간의 크기인 윈도우 크기를 가지고 
		계속 이를 업데이트 ( 슬라이드 ) 하는 것을 의미한다.
		- 따라서 상대의 ACK 가 없어도 미리 상대방이 수신할 수 있는 공간의 크기를 알기 때문에
		연속해서 데이터 프레임을 전송할 수 있다.
		- 수신측이 ACK 를 하거나 응답데이터를 보낼 일이 있을 때 TCP 헤더에 윈도우 크기를 새로 업데이트 함으로서
			속도 조절이 가능하다.
	 	- ![Sliding window]( http://cfile214.uf.daum.net/image/162B454650EB87FA1285A9 )  
			- 출처 http://blog.daum.net/tlos6733/48

	* TCP autotuning
	* RFC 1323 - TCP Extensions for High Performance
	* RFC 2018 - TCP Selective Acknowledgment options

