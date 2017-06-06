from http.server import HTTPServer, BaseHTTPRequestHandler # from BaseHTTPServer import `` in python 2

class MyHandler( BaseHTTPRequestHandler ):
	def do_GET( self ):
		self.wfile.write( ("Hello World").encode() )


if __name__ == '__main__':
	sServer = HTTPServer( ( '', 8888 ), MyHandler )
	print( "Started WebServer on port 8888..." )
	print ( "Press ^C to quit WebServer " )

	sServer.serve_forever()
