from urllib.request import urlopen
from random import randint

def wordListSum( aWordList ):
	sSum = 0
	for word, value in aWordList.items():
		sSum += value

	return sSum


# wordList 의 딕셔너리 안의 단어들 중 일정한 값 ( 확률 )을 주고
# 이 확률보다 높은 값을 가진 단어를 뽑아 리턴한다.
# ex ) { "I", { "was" : 3, "have" : 5, "do" : 1 }
#  random 으로 확률을 선택 1 ~ ( 3 + 5 + 1 ) 사이 값중 하나
#  만약 random 으로 7 이 뽑혔다면 
#  ran = 7 - 3( was ) = 4 next step => ran = 4 - 5( have ) = -1  ==> ran <= 0 return word
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
		# 이 단어와 함께 나온 단어 딕셔너리를 추가.
		# { word1 : { word2 : 2, word3 : 5 }, word4 : { word5 : ... } } 
		if sWordList[i] not in sWordDict[sWordList[i - 1]]:
			sWordDict[sWordList[i-1]][sWordList[i]] = 0
		sWordDict[sWordList[i-1]][sWordList[i]] = sWordDict[sWordList[i-1]][sWordList[i]] + 1

	return sWordDict


sTxt = str( 
		urlopen( "http://pythonscraping.com/files/inaugurationSpeech.txt" ).read(), 
		'utf-8')
sWordDict = buildWordDict( sTxt )

# 길이가 sLen 인 마르코프 체인을 생성한다.
sLen = 100
sChain = ""
sCurWord = "I"
for i in range( 0, sLen ):
	sChain += sCurWord + " "
	sCurWord = retrieveRandomWord( sWordDict[sCurWord] )

print( sChain )
