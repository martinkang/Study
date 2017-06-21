import signal
import time
import sys

def handler( aSignum, f ):
	print( f )
	print( "signal handler catch : ", aSignum )
	sys.exit()

signal.signal( signal.SIGINT, handler )

while True:
	print( time.ctime() )
	time.sleep(1)
