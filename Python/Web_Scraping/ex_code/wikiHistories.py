from urllib.request import urlopen
from bs4 import BeautifulSoup
import datetime
import random
import re
import getCountry

random.seed( datetime.datetime.now() )

def getLinks( aUrl ):
	sHtml = urlopen( "http://en.wikipedia.org" + aUrl )
	sBsObj = BeautifulSoup( sHtml, "html.parser" )

	return sBsObj.find( "div", {"id":"bodyContent"} ). \
			      findAll( "a", href = re.compile( "^(/wiki/)((?!:).)*$" ))


def getHistoryIps( aUrl ):
	# 계정 내의 페이지 URL 은 다음과 같은 형식
	# http://en.wikipedia.org/w/index.php?title=Title_in_URL&action=history
	sPageUrl = aUrl.replace( "/wiki/", "" )
	sHistoryUrl = "http://en.wikipedia.org/w/index.php?title="
	sHistoryUrl += sPageUrl + "&action=history"
	print( "history url is : " + sHistoryUrl )

	sHtml = urlopen( sHistoryUrl )
	sBsObj = BeautifulSoup( sHtml, "html.parser" )
	# 사용자명 대신 IP 주소가 담긴, 클래스가 mw-anonuserlink 인 링크만 찾는다
	sIpAddrs = sBsObj.findAll( "a", {"class":"mw-anonuserlink"} )

	sAddrList = set()

	for sIpAddr in sIpAddrs:
		sAddrList.add( sIpAddr.get_text() )

	return sAddrList


sLinks = getLinks( "/wiki/Python_(programming_language)" )

sCountryDic = dict()

while( len( sLinks ) > 0 ):
	for sLink in sLinks:
		print( "----------------------" )

		sHistoryIps = getHistoryIps( sLink.attrs["href"] )
		for sHistoryIp in sHistoryIps:
			sCountry = getCountry.getCountryByIP( sHistoryIp )
			print( sHistoryIp + " is From " + sCountry )

			if sCountry not in sCountryDic:
				sCountryDic[sCountry] = 1 
			else:
				sCountryDic[sCountry] += 1
					
		print( "--- Total ---" )
		print( sCountryDic )
		sCountryDic.clear()

#sNewLink = sLinks[ random.randint( 0, len( links ) - 1 ) ].attrs["href"]
#sLinks = getLinks( sNewLink )

