from urllib.request import urlopen
from html.parser import HTMLParser # from HtmlParser in python2


class ImageParser( HTMLParser ):
	def handle_starttag( self, aTag, aAttrs ):
		if aTag != 'img':
			return

		if not hasattr( self, 'result' ):
			self.result = []

		for name, value in aAttrs:
			if name == 'src':
				self.result.append( value )


def parseImage( aData ):
	sParser = ImageParser()
	sParser.feed( aData )
	sDataSet = set( x for x in sParser.result )
	print( '\n'.join( sorted( sDataSet ) ) )


def main():
	sUrl = "http://www.google.co.kr"

	sHtml = urlopen( sUrl )
	sCharset = sHtml.info().get_content_charset()
	if sCharset is not None:
		sData = sHtml.read().decode( sCharset )
	else:
		sData = sHtml.read()

	sHtml.close()

	print ( "\n>>>>>>>>> Fetch Images from", sUrl )
	parseImage( sData )


if __name__ == '__main__':
	main()
