# UDP 브로드캐스팅
* UDP 브로드 캐스팅
	- 브로드캐스팅을 사용하려면 소켓을 생성한 뒤에 소켓에 브로드 캐스팅 옵션을 주어야 한다.

##### UDP 브로드캐스팅을 위한 SO_BROADCAST 옵션 사용

>  int sockopt = 1;
> if ( setsockopt( fd, SOL_SOCKET, SO_BROADCAST, &sockopt, sizeof( sockopt ) ) == -1 ) { /* error */ }

	- 브로드캐스트의 주소는 대개 자신이 속한 네트워크 주소의 맨 마지막 주소거나 
	255.255.255.255 로 지정된다.
		- 둘 중 어느걸 사용해도 되지만 일반적으로 255.255.255.255 를 지정하면 알아서 
		시스템이 사용하는 네트워크 설정에 정의된 모든 브로드캐스팅 주소를 사용한다.
			- INADDR_BROADCAST 라고 정의되어 있다.
	
