# def checkNReadRobots( aUrl )
#   각 사이트의 주소를 받아와 robots.txt 파일을 찾고 해당 파일을 읽어 아래의 리스트를 작성
#   User-Agent, Allows, DisAllows
#   return True of False
#       robot.txt 가 있으면 True 이고 없으면 False
#
# def isAllowed( aUrl )
#   해당 주소가 진입이 허락되었는지 확인
#   return : True or False
#       Allow 리스트에 있으면 True 없으면 False
#
# gAllowDicts = 딕셔너리 형태로 "주소" : "disallow" or "allow"  형태로 저장한다.

from urllib.request import HTTPError
from urllib.request import urlopen

class readRobots:
	gAllowDicts = dict()

	def classifyAllowDict( self, aRobotsTxt ):
		sTxt = aRobotsTxt.decode( 'utf-8' ).replace( ' ', '' ).replace( ':', '' ).lower()
		for item in sTxt.split( '\n' ):
			if item[0:8] == 'disallow':
				self.gAllowDicts[ gUrl + item[8:] ] = 'disallow'
			elif item[0:5] == 'allow':
				self.gAllowDicts[ gUrl + item[5:] ] = 'allow'
			else:
				# 나머지 정보는 버림
				pass

	def checkNReadRobots( self, aUrl ):
		global gUrl
		# 주소 끝이 공백 또는 / 로 끝난다면 제거해준다.
		gUrl = aUrl.strip().strip('/')

		try:
			sRobotsTxt = urlopen( gUrl + "/robots.txt" ).read()
		except HTTPError as e:
			# 해당 주소에 접근을 못하거나 robots.txt 파일이 존재하지 않는다.
			return False
		else:
			# robots.txt 를 읽고 user agent, allow, disallow  딕셔너리를 생성한다.
			self.classifyAllowDict( sRobotsTxt )
			return True

	def isAllowed( self, aUrl ):
		# 검사할 주소
		sUrl = aUrl.lower()
		try:
			if self.gAllowDicts[sUrl] == 'disallow':
				return False
			else:
				# False 가 아니라면 허용 가능으로 판단하고 True 를 리턴
				return True
		except KeyError:
			# 리스트에 없다면 허용 가능으로 판단하고 True 를 리턴
			return True
