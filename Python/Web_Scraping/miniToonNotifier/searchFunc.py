import string
import time
from urllib.parse import quote
from urllib.request import urlopen, urlretrieve
from bs4 import BeautifulSoup

from csvFunc import csvFunc


class searchFunc():
	gUrl = ""
	gBbsUrl = ""
	gSearchUrl = ""
	gComicTitle = None
	gResultFromCvs = None

	def __init__( self, aUrl, aBbsUrl, aSearchUrl, aComicTitle, aResultFromCsv ):
		self.gUrl = aUrl
		self.gBbsUrl = aBbsUrl
		self.gSearchUrl = aSearchUrl
		self.gComicTitle = aComicTitle
		self.gResultFromCsv = aResultFromCsv


	# 구두점 리스트 !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~ 와 공백을 찾고 제거
	def getOnlyText( self, aStr ):
		sStr = aStr.replace( ' ', '' ).strip( '\n' )
		for pun in string.punctuation:
			sStr = sStr.replace( pun, '' )

		return sStr

	def searchComic( self, aTitle ):
		if aTitle is None or aTitle is '':
			return None

		sList = []
		for comicList in self.gResultFromCsv:
			sTitle = self.getOnlyText( comicList[0].upper() )
			if aTitle in sTitle:
				sList.append( comicList )

		if len( sList ) > 0:
			return sList
		else:
			return None


	def searchComicsFromCsv( self, aTitle ):
		if aTitle is None or aTitle is '':
			return None

		if self.gResultFromCsv is None:
			return None

		sComicTitle = self.getOnlyText( aTitle.upper() )
		sList = self.searchComic( sComicTitle )

		if len( sList ) > 0:
			return sList
		else:
			return None


	def searchComicsFromWeb( self, aTitle ):
		if aTitle is None or aTitle is '':
			return None

		sKeyword = ''
		sResultList = []

		for title in aTitle.split( ' ' ):
			sKeyword = sKeyword + ' ' + quote( self.getOnlyText( title ).encode( 'utf8' ) )

		# URL 에 조합할 키워드
		sSearchKeyword = sKeyword.strip( ' ' ).replace( ' ', '+' ).upper() 
		# 실제 검색 결과와 비교할 키워드
		sComicTitle = self.getOnlyText( aTitle.upper() )

		# 기본 searchUrl 에 URL 조합 키워드를 더한다.
		try:
			sSearchUrl = self.gSearchUrl + sSearchKeyword
			sHtml = urlopen( sSearchUrl )
			sBsObj = BeautifulSoup( sHtml, "html.parser" )

			sResult = sBsObj.findAll( "a", { "class":"sch_res_title" } )
		except:
			return None

		for res in sResult:
			sTitle = self.getOnlyText( res.get_text().upper() )

			if sComicTitle in sTitle:
				sVolUrl = self.gBbsUrl + res['href'].lstrip( '.' )
				sList = [ sComicTitle, sVolUrl, '' ]
				sResultList.append( sList )

		if len( sResultList ) > 0:
			return sResultList
		else:
			return None


	def searchLastVol( self, aUrl ):
		try:
			sHtml = urlopen( aUrl )
			sBsObj = BeautifulSoup( sHtml, "html.parser" )

			sResult = sBsObj.findAll( "a", { "class":"gal_subject" } )
		except:
			return None

		if sResult is None:
			return None

		for res in sResult:
			pass

		# 리스트의 마지막만 가져온다.
		sUrl = self.gUrl + res['href'].lstrip( '.' )
		sResultList = [ res.get_text(), aUrl, sUrl ]

		if len( sResultList ) > 0:
			return sResultList
		else:
			return None


	def searchNew( self ):
		sResultList = []
		sIsModify = False
		sCount = 0

		for title in self.gComicTitle:
			sTitle = title.strip( '\n' )
			if sTitle == '':
				continue
	
			sIsFromCsv = False
			sResult =  self.searchComicsFromCsv( sTitle )
			if sResult is None: 
				sResult = self.searchComicsFromWeb( sTitle )
			else:
				sIsFromCsv = True

			if sResult is not None:
				for res in sResult:
					sRes = self.searchLastVol( res[1] )
					if sRes is not None:
						sResultList.append( sRes )
						# 기존의 자료와 다르거나 웹에서 부터 얻어오면 신작이다.
						if sIsFromCsv == True and sRes[0] != res[0] \
									   or sIsFromCsv == False:
							sIsModify = True
							print( "[" + sTitle + "] 신작 : " + sRes[0] + "\n\tURL : " + sRes[1] +
									"\n\tLast Vol URL : " + sRes[2] )
						else:
							print( "[" + sTitle + "] 신작이 없습니다." )
							print( "가장 최근 신간 : " + sRes[0] + "\n\tURL : " + sRes[1] + \
									"\n\tLast Vol URL : " + sRes[2] )


			# 서버 부담을 줄이기 위해 3 번에 한번꼴로 1 초씩 쉰다.
			sCount += 1
			if sCount % 3 == 0:
				time.sleep( 1 )

		if sIsModify == True:
			csvFunc.writeCsv( sResultList )
