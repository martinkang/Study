import string
import time
import csvFunc
from urllib.parse import quote
from urllib.request import urlopen, urlretrieve
from bs4 import BeautifulSoup


class searchFunc():
	gUrl = ""
	gBbsUrl = ""
	gSearchUrl = ""
	gComicTitle = []
	gResultFromCvs = []

	def __init__( self, aUrl, aBbsUrl, aSearchUrl, aComicTitle, aResultFromCvs ):
		gUrl = aUrl
		gBbsUrl = aBbsUrl
		gSearchUrl = aSearchUrl
		gComicTitle = aComicTitle
		gResultFromCvs = aResultFromCvs


# 구두점 리스트 !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~ 와 공백을 찾고 제거
	def getOnlyText( self, aStr ):
		sStr = aStr.replace( ' ', '' ).strip( '\n' )
		for pun in string.punctuation:
			sStr = sStr.replace( pun, '' )

		return sStr

	def searchComicsFromCsv( self, aTitle ):
		if aTitle is None or aTitle is '':
			return

		sSearchKeyword = aTitle.upper().split(' ')[0]
		sComicTitle = self.getOnlyText( aTitle.upper() )

		for titleList in gResultFromCsv:
			sTitle = self.getOnlyText( titleList[0].upper() )
			if sComicTitle in sTitle:
				return titleList

		return None


	def searchComicsFromWeb( self, aTitle ):
		if aTitle is None or aTitle is '':
			return

		# 복수의 단어일 경우 첫 단어만을 이용하여 검색한다.
		# 검색 결과를 크게 얻어온 후 그 중에서 일치하는 걸 찾는다.
		sSearchKeyword = aTitle.upper().split(' ')[0]
		sComicTitle = self.getOnlyText( aTitle.upper() )

		sSearchUrl = self.gSearchUrl + quote( sSearchKeyword.encode( 'utf8' ) )

		sHtml = urlopen( sSearchUrl )
		sBsObj = BeautifulSoup( sHtml, "html.parser" )

		sResult = sBsObj.findAll( "a", { "class":"sch_res_title" } )
		for res in sResult:
			sTitle = self.getOnlyText( res.get_text().upper() )

			if sComicTitle in sTitle:
				sVolUrl = self.gBbsUrl + res['href'].lstrip( '.' )
				return self.searchLastVol( sVolUrl )


	def searchLastVol( self, aUrl ):
		sHtml = urlopen( aUrl )
		sBsObj = BeautifulSoup( sHtml, "html.parser" )

		sResult = sBsObj.findAll( "a", { "class":"gal_subject" } )
		if sResult is None:
			return None

		for res in sResult:
			pass

		# 리스트의 마지막만 가져온다.
		sUrl = self.gUrl + res['href'].lstrip( '.' )
		sResultList = [ res.get_text(), aUrl, sUrl ]

		return sResultList


	def searchNew( self ):
		sResultList = []
		sIsModify = False

		for title in self.gComicTitle:
			sTitle = title.strip( '\n' )
			if sTitle == '':
				continue

			sResult = []
			sRes =  self.searchComicsFromCsv( sTitle )
			if sRes is not None:
				# 이미 이전에 찾아서 해당 만화에 대한 url 이 있음
				sResult = self.searchLastVol( sRes[1] )

				if sRes[0] == sResult[0]:
					pass # 마지막에 찾은 결과와 같다.
					#print( "[" + sTitle + "] 마지막 화 : " + sRes[0] + " url : " + sRes[2] )
				else:	
					sIsModify = True
					print( "신간 - [" + sTitle + "] 마지막 화 : " + sResult[0] + " url : " + sResult[2] )
			else:
				# 새로운 만화라 url 이 없음
				sResult = self.searchComicsFromWeb( sTitle )
				if sResult is None:
					print( "[" + sTitle + "] 은 존재하지 않습니다." )
				else:
					sIsModify = True
					print( "신간 - [" + sTitle + "] 마지막 화 : " + sResult[0] + " url : " + sResult[2] )

			if sResult is not None:
				sResultList.append( sResult )
		
		# 서버의 부담을 줄이기 위해
		time.sleep( 1 )

		# 추가되거나 변경된 사항이 있으면파일을 다시 쓴다.
		if sIsModify == True:
			csvFunc.writeCsv( sResultList )

