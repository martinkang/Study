from urllib.request import urlopen
# pip3 install beautifulsoup4
from bs4 import BeautifulSoup
import re
import string
import operator

gCommonWords = []

sFile = open( "Common.txt", "r" )
gCommonWords = sFile.read().lower()
sFile.close()

# 흔히 쓰이는 분석이 불필요한 단어를 걸러내기 위한 함수.
def isCommon( aNgram ):
	if aNgram in gCommonWords:
		return True

	return False


# 필요없는 단어, 문자를 제거한다.
def cleanInput( aInput ):
	sInput = aInput
	#\n+ : \n 이 1번 이상 나옴
	sInput = re.sub( '\n+', " ", sInput )
	# \[[0-9]*\] : [숫자]
	sInput = re.sub( '\[[0-9]*\]', "", sInput )
	sInput = re.sub( ' +', " ", sInput )
	sInput = re.sub( "u\.s\.", "us", sInput )
	
	sInput = bytes( sInput, "UTF-8" )
	# strip method 를 위해 ascii 로 변경
	sInput = sInput.decode( "ascii", "ignore" )
	
	sCleanInput = []
	sInput = sInput.split( ' ' )
	for item in sInput:
		# string.punctuation: 구두점 리스트 ( '!"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~' )
		# strip( string.puncuation ) : 단어 양 끝의 구두점을 모두 없앨 수 있다.
		sItem = item.strip( string.punctuation )
		if ( len( sItem ) > 1 ) and ( isCommon( sItem.lower() ) == False ):
			sCleanInput.append( sItem )

	return sCleanInput


def ngrams( aInput, aN ):
	sInput = cleanInput( aInput ) 
	sOutput = {} 

	for i in range( len( sInput ) - aN + 1 ):
		sTemp = " ".join( sInput[i:i + aN] )
		if sTemp not in sOutput:
			sOutput[sTemp] = 0
		# n-gram 의 단어 빈도수를 더한다
		sOutput[sTemp] += 1
	
	return sOutput


def isNGramsInContent( aInput, aGram, aN ):
	sInput = cleanInput( aInput )

	for i in range( len( sInput ) - aN + 1 ):
		sTemp = " ".join( sInput[i:i + aN] )
		if sTemp in aGram:
			return True

	return False

sContent = str( 
			urlopen( "http://pythonscraping.com/files/inaugurationSpeech.txt" ).read(),
			'utf-8' )
sNGrams = ngrams( sContent, 2 )
# dictionary 는 정렬이 불가능 하므로 key 순으로 정렬하여 튜플에 넣는다.
sSortedNGrams = sorted( sNGrams.items(), key = operator.itemgetter(1), reverse = True )

sMostGram = []
print( "가장 많이 쓰인 단어 리스트" )
for word in sSortedNGrams:
	sItem, sValue = word
	# 일정수 이하로 나온  n-gram 은 큰 의미가 없다고 판단하고 출력하지않는다.
	if sValue > 3:
		sMostGram.append( sItem.lower() )
		print( word )

print( "\n" )
print( "가장 많이 쓰인 단어가 포함된 문장 리스트" )
i = 0
for content in sContent.split( "." ):
	if isNGramsInContent( content, sMostGram, 2 ) == True:
		i = i + 1
		print( str(i) + " : " + content )

