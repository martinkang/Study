from urllib.request import urlopen
from random import randint

def wordListSum( aWordList ):
	sSum = 0
	for word, value in aWordList.items():
		sSum += value

	return sSum


def retrieveRandomWord( aWordList ):
	sRandIdx = randint( 1, wordListSum( aWordList ) )
	for word, value in aWordList.items():
		sRandIdx -= value
		if sRandIdx <= 0:
			return word


def buildWordDict( aTxt ):
	sTxt = aTxt.replace( "\n", " " )
	sTxt = sTxt.replace( "\"", "" )

	# 구두점도 단어로 취급함
	sPunctuation = [ ',', '.', ';', ':' ]
	for symbol in sPunctuation:
		sTxt = sTxt.replace( symbol, " " + symbol + " " )

	sWords = sTxt.split( " " )
	sWordList = []
	for word in sWords:
		if word != "":
			sWordList.append( word )

	sWordDict = {}
	for i in range( 1, len( sWordList ) ):
		if sWordList[i-1] not in sWordDict:
			# 이 단어에 필요한 새딕셔너리를 만듬
			sWordDict[ sWordList[i-1] ] = {}
		if sWordList[i] not in sWordDict[sWordList[i - 1]]:
			sWordDict[sWordList[i-1]][sWordList[i]] = 0
		sWordDict[sWordList[i-1]][sWordList[i]] = sWordDict[sWordList[i-1]][sWordList[i]] + 1

	return sWordDict


sTxt = str( 
		urlopen( "http://pythonscraping.com/files/inaugurationSpeech.txt" ).read(), 
		'utf-8')
sWordDict = buildWordDict( sTxt )

sLen = 100
sChain = ""
sCurWord = "I"
for i in range( 0, sLen ):
	sChain += sCurWord + " "
	sCurWord = retrieveRandomWord( sWordDict[sCurWord] )

print( sChain )
