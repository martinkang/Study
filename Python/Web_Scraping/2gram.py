from urllib.request import urlopen
from bs4 import BeautifulSoup
import re
import string

# 필요없는 단어, 문자를 제거한다.
def cleanInput( aInput ):
	sInput = aInput
	#\n+ : \n 이 1번 이상 나옴
	sInput = re.sub( '\n+', " ", sInput )
	# \[[0-9]*\] : [숫자]
	sInput = re.sub( '\[[0-9]*\]', "", sInput )
	sInput = re.sub( ' +', " ", sInput )
	
	sInput = bytes( sInput, "UTF-8" )
	# strip method 를 위해 ascii 로 변경
	sInput = sInput.decode( "ascii", "ignore" )
	
	sCleanInput = []
	sInput = sInput.split( ' ' )
	for item in sInput:
		# string.punctuation: 구두점 리스트 ( '!"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~' )
		#  strip( string.puncuation ) : 단어 양 끝의 구두점을 모두 없앨 수 있다.
		sItem = item.strip( string.punctuation )
		if len( sItem ) > 1 or ( sItem.lower() == 'a' or sItem.lower() == 'i' ):
			sCleanInput.append( sItem )

	return sCleanInput


def ngrams( aInput, aN ):
	sInput = cleanInput( aInput ) 
	sOutput = []

	for i in range( len( sInput ) - aN + 1 ):
		sOutput.append( sInput[i:i+aN] )
	
	return sOutput

sHtml = urlopen( "http://en.wikipedia.org/wiki/Python_(programming_language) " )

sBsObj = BeautifulSoup( sHtml, "html.parser" )
sContent = sBsObj.find( "div", {"id":"mw-content-text"} ).get_text()

sNGrams = ngrams( sContent, 2 )
print( sNGrams )
print( "2-grams count is : " + str( len( sNGrams ) ) )
