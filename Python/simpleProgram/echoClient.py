import asyncore
import socket

class EchoClient( asyncore.dispatcher ):
	def __init__( self, aHost, aPort ):
		asyncore.dispatcher.__init__( self )
		self.create_socket( socket.AF_INET, socket.SOCK_STREAM )

		self.connect( ( aHost, aPort ) )
		self.messages = [ 'hi', 'hello', '안녕하세요\n안녕~' ]
		self.seperator = b'\r\n\r\n' 


	def __handle_connect( self ):
		pass


	def handle_expt( self ):
		self.close()


	def writable( self ):
		return self.messages


	def handle_write( self ):
		if self.messages:
			sMsg = self.messages.pop( 0 )
			self.send( sMsg.encode() )
			self.send( self.seperator )


	def handle_read( self ):
		sRecv = self.recv( 2048 )
		for msg in sRecv.split( self.seperator ):
			if msg:
				print( msg.decode() )


	def handle_close( self ):
		self.close()


	
request = EchoClient( 'localhost', 9090 )
asyncore.loop()
