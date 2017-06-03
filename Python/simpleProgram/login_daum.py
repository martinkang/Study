import urllib.request
import urllib.parse
from http import cookiejar

sCj = cookiejar.CookieJar()
sCookie_handler = urllib.request.HTTPCookieProcessor( sCj )
sOpener = urllib.request.build_opener( sCookie_handler )
urllib.request.install_opener( sOpener )

def my_request( aUrl, aPostFields ):
	sHeaders = { 'User-agent' : 'Mozilla/4.0 (compatible: MSIE 5.5; Windows NT)' }

	sReq = urllib.request.Request( aUrl, aPostFields, sHeaders )
	sResponse = urllib.request.urlopen( sReq )
	return sResponse


def login( aUserName, aPw ):
	sLogin_Url = 'https://logins.daum.net/accounts/login.do'
	sForm_Values = {
		'id' : aUserName,
		'pw' : aPw,
	}
	sFormData = urllib.parse.urlencode( sForm_Values )

	try:
		sResponse = my_request( sLogin_Url, sFormData.encode() )
	except IOError as e:
		print( 'We failed to opne "%s", ' % sLogin_Url )
		if hasattr( e, 'code' ):
			print( 'We failed with error code - %s.' % e.code )
		raise SystemExit
	else:
		# response.info()
		# html = response.read()
		pass

def getPage( aUrl ):
	sResponse = my_request( aUrl, None )
	return sResponse.read()


def run():
	login( 'id', 'pw' )

	sUrl = 'http://www.daum.net/'
	sHtml = getPage( sUrl ).decode()
	# do something with html
	print( sHtml )


if __name__ == '__main__':
	run()
