import http.client # httplib in python2
import os
from html.parser import HTMLParser # from HtmlParser in python2
from urllib.parse import urljoin, urlunparse # from urlparse import urljoin in python 2
from urllib.request import urlretrieve # from urllib import urlretrieve in python 2

class ImageParser( HTMLParser ):
	def handle_starttag( self, aTag, aAttrs ):
		if aTag != 'img':
			return

		if not hasattr( self, 'result' ):
			self.result = []

		for name, value in aAttrs:
			if name == 'src':
				self.result.append( value )

def downloadImage( aSrcUrl, aData ):
	if not os.path.exists( 'DOWNLOAD' ):
		os.makedirs( 'DOWNLOAD' )

	sParser = ImageParser()
	sParser.feed( aData )

	sResultSet = set( x for x in sParser.result )

	for x in sorted( sResultSet ):
		sSrc = urljoin( aSrcUrl, x )
		sBaseName = os.path.basename( sSrc )
		sTargetFile = os.path.join( 'DOWNLOAD', sBaseName )

		print( "Downloading...", sSrc )
		urlretrieve( sSrc, sTargetFile )


def main():
	sHost = "www.google.co.kr"

	sConn = http.client.HTTPConnection( sHost )
	sConn.request( "GET", '' )
	sResponse = sConn.getresponse()

	sCharset = sResponse.msg.get_content_charset()
	sData = sResponse.read().decode( sCharset )
	sConn.close()

	print( "\n>>>>>>>>>> Download Image from ", sHost )
	# URL 요소 6 개를 튜플로 받아서 완성된 URL 을 리턴
	sUrl = urlunparse( ('http', sHost, '', '', '', '' ) )
	downloadImage( sUrl, sData )


if __name__ == '__main__':
	main()
