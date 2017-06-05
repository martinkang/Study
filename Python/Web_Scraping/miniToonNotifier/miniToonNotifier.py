from urllib.request import urlopen, urlretrieve
from bs4 import BeautifulSoup

from readRobots import readRobots
from printFunc import printFunc
from csvFunc import csvFunc
from searchFunc import searchFunc

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
# 검색할 만화의 리스트가 있다.
sReadFile = open( './miniBook.txt' )
gComicTitle = sReadFile.readlines()
sReadFile.close()

# 이전에 찾은 만화 리스트가 있으면 불러온다.
gResultFromCsv = csvFunc.readCsv()
gSearch = searchFunc( gUrl, gBbsUrl, gSearchUrl, gComicTitle, gResultFromCsv )

while 1:
	print( " ==================================================" )
	print( " 1. 검색 리스트 출력" )
	print( " 2. 결과 리스트 출력" )
	print( " 3. 신간 찾아보기" )
	print( " 4. 만화 찾기" )
	print( " 5. 종료" )
	print( " ==================================================" )
	sInput = input( "원하는 메뉴 : " )
	sInput = sInput.strip().strip('\n')

	if sInput == '1':
		printFunc.comicTitle( gComicTitle )
	elif sInput == '2':
		printFunc.comicResult( gResultFromCsv )
	elif sInput == '3':
		gSearch.searchNew()
		# 신간 찾은 후 리스트가 바뀌었을 수 있으므로 새로 읽어옴
		gResultFromCsv = csvFunc.readCsv()
	elif sInput == '4':
		sTitle = input( "찾는 만화 : " )
		sComic = gSearch.searchComic( sTitle.strip().strip('\n' ) )
		printFunc.comicResult( sComic )
	elif sInput == '5':
		break;
	else:
		print( "1~4 번 메뉴중 하나를 선택해 주세요" )
