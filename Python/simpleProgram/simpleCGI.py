import sys
import codecs

sWriter = codecs.getwriter( 'utf-8' )( sys.stdout.buffer )
print( "Content-Type: text/html;charset=utf-8\n\n", file = sWriter )
print( '''
		<HTML>
		<HEAD></HEAD>
		<BODY>
		<H2>한글</H2>
		Hello world
		</BODY>
		</HTML>''', 
		file = sWriter )
