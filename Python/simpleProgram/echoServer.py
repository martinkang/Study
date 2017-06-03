import asyncore
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


class EchoHandler( asyncore.dispatcher ):
	def __init__( self, aSock = None ):
		asyncore.dispatcher.__init__( self, aSock )
		self.incom_data = b''
		# 각각의 메시지를 구분하는 구분 기호
		self.seperator = b'\r\n\r\n' 
		self.write_queue = []

	# 새로운 데이터가 소켓이 있을 때 호출됨
	def handle_read( self ):
		sData = self.recv( 2048 )
		self.incom_data += data

		if self.seperator in self.incom_data:
			# 가변 파라메터 sMsgs
			*sMsgs, self.incom_data = self.incom_data.split( self.seperator )
			for msg in sMsgs:
				print( 'got message ', msg.decode() )
				self.write_queue.append( msg )

	# 데이터를 쓸 수 있으면 True
	def writable( self ):
		return self.write_queue

	# 쓰는 소켓에 쓰는 것이 가능해지면 호출
	def handle_wirte( self ):
		if self.write_queue:
			sMsg = self.write_queue.pop( 0 )
			print( 'sending.. ', sMsg.decode() )

			sSent = self.send( sMsg )
			self.send( self.seperator )

		if not self.write_queue:
			print( 'closing..' )
			self.close()


print( 'Echo Server is Starting port 9090 ....' )

sServer = AsyncServer( 9090 )
asyncore.loop()
