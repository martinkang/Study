import asynchat # https://docs.python.org/3/library/asynchat.html
import asyncore # https://docs.python.org/3/library/asyncore.html
import socket

class AsyncServer( asyncore.dispatcher ):
    def __init__( self, aPort ):
        asyncore.dispatcher.__init__( self )
        self.port = aPort

        self.create_socket( socket.AF_INET, socket.SOCK_STREAM )
        # port 를 즉시 재사용 하기 위해서
        self.set_reuse_addr()

        self.bind( ( 'localhost', aPort ) )
        self.listen( 5 )
        print( "listening on port", self.port )

    # 연결이 이루어지면 호출된다.
    def handle_accepted( self, sClient_Sock, aAddr ):
        print( "accepted", aAddr )
        # 핸들러 객체를 생성
        return EchoHandler( aSock = sClient_Sock )

class HttpHandler( asynchat.async_chat ):
	def __init__( self, aSock = None ):
		asynchat.async_chat.__init__( self, aSock )

		self.set_terminator( b'\r\n\r\n' )
		self.incom_data = b''

	def collect_incoming_data( self, aData )
		self.incom_data += aData

	def found_terminator( self ):
		

	def push_text( self, aText ):
		if type( aText ) == bytes:
			self.push( aText ) # 클라이언트 측으로 메시지를 정송
		else:
			self.push( aText.encode() ) # 문자열이면 바이트열로 변환해서 전송


sServer = AsyncServer( 9090 )
asyncore.loop()
