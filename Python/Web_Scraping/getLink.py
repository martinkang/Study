from urllib.request import urlopen
from urllib.parse import urlparse
from bs4 import BeautifulSoup
import re
import datetime
import random
import sys

sPages = set()
random.seed( datetime.datetime.now() )

# / 로 시작하는 링크를 모두 찾습니다.
def getInternalLinks( aBsObj, aUrl ):
	aUrl = urlparse( aUrl ).scheme + "://" + urlparse( aUrl ).netloc
	sInternalLinks = []

	for link in aBsObj.findAll( "a", href = re.compile( "^(/|.*" + aUrl + ")" ) ):
		if link.attrs['href'] is not None:
			if link.attrs['href'] not in sInternalLinks:
				if link.attrs['href'].startwith( "/" ):
					sInternalLinks.append( aUrl + link.attrs['href'] )				
					print( "Add InternalLinks[ " + aUrl + link.attrs['href' ] + "]" )
				else:
					sInternalLinks.append( link.attrs['href'] )
					print( "Add InternalLinks[ " + link.attrs['href' ] + "]" )

	return sInternalLinks

# 현재 URL 을 포함하지 않으면서 http 나 www 로 시작하는 링크를 모두 찾는다.
def getExternalLinks( aBsObj, aUrl ):
	sExternalLinks = []
	for link in aBsObj.findAll( "a", href = re.compile( "^(http|www)((?!" + aUrl + ").)*$" ) ):
		if link.attrs['href'] is not None:
			if link.attrs['href'] not in sExternalLinks:
				sExternalLinks.append( link.attrs['href'] )
				print( "Add ExternalLinks[ " + link.attrs['href' ] + "]" )

	return sExternalLinks

def getRandomExternalLink( aStartingPage ):
	sHtml = urlopen( aStartingPage )
	sBsObj = BeautifulSoup( sHtml, "html.parser" )

	sExternalLinks = getExternalLinks( sBsObj, urlparse( aStartingPage ).netloc )
	if sExternalLinks != None:
		if len( sExternalLinks ) == 0:
			# 외부 링크가 하나도 없다면 내부 링크에서 찾기
			sDomain = urlparse( aStartingPage ).scheme + "://" + urlparse( aStartingPage ).netloc
			sInternalLinks = getInternalLinks( sBsObj, sDomain )
			return getRandomExternalLink( sInternalLinks[ random.randint( 0, len( sInternalLinks ) - 1 ) ] )
		else:
			if sExternalLinks != None:
				return sExternalLinks[ random.randint( 0, len( sExternalLinks ) - 1 ) ]
	else:
		print( "ExternalLink is None" )
	
def followExternalOnly( aStartingPage ):
	sExternalLink = getRandomExternalLink( aStartingPage )
	if sExternalLink != None:
		print( "Random external link [ " + sExternalLink + " ]" )
		followExternalOnly( sExternalLink )
	else:
		print( "ExternalLink is None" )

followExternalOnly( "http://oreilly.com" )
