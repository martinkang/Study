import string
import time
import csv
from readRobots import readRobots
from urllib.request import urlopen, urlretrieve
from bs4 import BeautifulSoup
from urllib.parse import parse_qs

gUrl = "http://minitoon.net"
gBbsUrl = "http://minitoon.net/bbs"
gSearchUrl = "http://minitoon.net/bbs/search.php?sfl=wr_subject&sop=and&stx=" 


# robots.txt 를 읽고 AllowDict 을 만든다.
# 하지만 여기선 쓸일이 없음....
gReadBot = readRobots()
gReadBot.checkNReadRobots( gUrl )

# charset 받아오기
sHtml = urlopen( gUrl )
sBsObj = BeautifulSoup( sHtml, "html.parser" )
sCharSet = sBsObj.find( "meta" )['charset']

# miniToonNotifier.py 와 같은 폴더에 miniBook.txt 를 읽어온다.
sReadFile = open( './miniBook.txt' )
gComicTitle = sReadFile.readlines()
sReadFile.close()


def readCsv():
	sResultSet = []

	try:
		sFile = open( './miniBook.csv', 'r', encoding='utf-8' )
		sCsv = csv.reader( sFile )

		for line in sCsv:
			sResultSet.append( tuple( line ) )
		
		sFile.close()
	except:
		print( "Create new miniBook.csv file" )

	return sResultSet


def writeCsv( aComicSet ):
	sFile = open( './miniBook.csv', 'a', encoding='utf-8', newline='' )
	sCsv = csv.writer( sFile )
	
	sCsv.writerow( [ aComicSet[0], aComicSet[1], aComicSet[2] ] )
	sFile.close()


# 구두점 리스트 !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~ 와 공백을 찾고 제거
def getOnlyText( aStr ):
	sStr = aStr.replace( ' ', '' ).strip( '\n' )
	for pun in string.punctuation:
		sStr = sStr.replace( pun, '' )

	return sStr


def searchComicsFromCsv( aTitle ):
	if aTitle is None or aTitle is '':
		return

	sSearchKeyword = aTitle.upper().split(' ')[0]
	sComicTitle = getOnlyText( aTitle.upper() )

	for titleList in gResultFromCsv:
		sTitle = getOnlyText( titleList[0].upper() )

		if sComicTitle in sTitle:
			return titleList

	return None


def searchComicsFromWeb( aTitle ):
	if aTitle is None or aTitle is '':
		return

	# 복수의 단어일 경우 첫 단어만을 이용하여 검색한다.
	# 검색 결과를 크게 얻어온 후 그 중에서 일치하는 걸 찾는다.
	sSearchKeyword = aTitle.upper().split(' ')[0]
	sComicTitle = getOnlyText( aTitle.upper() )

	sSearchUrl = gSearchUrl + sSearchKeyword 

	sHtml = urlopen( sSearchUrl )
	sBsObj = BeautifulSoup( sHtml, "html.parser" )

	sResult = sBsObj.findAll( "a", { "class":"sch_res_title" } )
	sResultList = []
	for res in sResult:
		sTitle = getOnlyText( res.get_text().upper() )

		if sComicTitle in sTitle:
			sVolUrl = gBbsUrl + res['href'].lstrip( '.' )
			return searchLastVol( sVolUrl )


def searchLastVol( aUrl ):
	sHtml = urlopen( aUrl )
	sBsObj = BeautifulSoup( sHtml, "html.parser" )

	sResult = sBsObj.findAll( "a", { "class":"gal_subject" } )
	if sResult is None:
		return None

	for res in sResult:
		pass

	# 리스트의 마지막만 가져온다.
	sUrl = gUrl + res['href'].lstrip( '.' )
	sResultList = [ res.get_text(), aUrl, sUrl ]

	return sResultList



# 이전에 찾은 만화 리스트가 있으면 불러온다.
gResultFromCsv = readCsv()

for title in gComicTitle:
	sTitle = title.strip( '\n' )

	sRes =  searchComicsFromCsv( sTitle )
	if sRes is not None:
		# 이미 이전에 찾아서 해당 만화에 대한 url 이 있음
		sResult = searchLastVol( sRes[1] )

		if sRes[0] == sResult[0]:
			print( sTitle + " 마지막 화 : " + sRes[0] + " url : " + sRes[2] )
		else:
			print( "신간 - " + sTitle + " 마지막 화 : " + sRes[0] + " url : " + sRes[2] )
	else:
		# 새로운 만화라 url 이 없음
		sResult = searchComicsFromWeb( sTitle )
		if sResult is None:
			print( sTitle + "은 존재하지 않습니다." )
		else:

			print( "신간 - " + sTitle + " 마지막 화 : " + sResult[0] + " url : " + sResult[2] )

			writeCsv( sResult )

	# 서버의 부담을 줄이기 위해
	time.sleep( 1 )

