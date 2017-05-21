# youtube 의 동영상 주소형태는 다음과 같다.
# https://www.youtube.com/watch?v='Video_ID'
# 이 Video_ID 를 가져온 후
# http://www.youtube.com/get_video_info?video_id='Video ID' 를 이용하여 get_video_info 를 가져온다.
# get_video_info 에서 'title' 을 찾아 동영상 제목으로 하고
# url_encoded_fmt_stream_map 값 중 'url' 항목을 찾으면 해당 url 을 이용하여 동영상을 다운받을 수 있다.

from urllib.request import urlopen, urlretrieve, urlparse
from urllib.parse import parse_qs
from bs4 import BeautifulSoup

#sUrl = input( "input youtube url : " )

sUrl = "https://www.youtube.com/watch?v=t4pbzzKo2G0"

def getVideoID( aUrl ):
	sUrl = aUrl.split( '/' )
	try:
		if "youtube.com" not in sUrl[2]:
			print( "youtube url is needed" )
			return None
		else:
			sVideoIDStartIdx = sUrl[3].find( "watch?v=" ) + 8
	except:
	  return None
	
	return sUrl[3][sVideoIDStartIdx:]


def getTitle( aInfo ):
	try:
		sTitle = aInfo['title'][0]
		sTitle = sTitle.replace( string.punctuation, '' )
	except:
		sTitle = "non title"
	
	return sTitle


def getVideoURL( aInfo ):
	return aInfo['url_encoded_fmt_stream_map'][0]


def getVideo( aUrl ):
	sVideoID = getVideoID( aUrl )
	if sVideoID is None:
		return None

	sGetVideoInfoUrl = "http://www.youtube.com/get_video_info?video_id=" + sVideoID
	sGetInfo = parse_qs( urlopen( sGetVideoInfoUrl ).read().decode( 'utf-8' ) )
	sTitle = getTitle( sGetInfo )
	
	sVideoURL = getVideoURL( sGetInfo ).split( ',' )

	try:
		for video in sVideoURL:
			sItem = parse_qs(video)
			sUrl = sItem['url'][0]
			print( sUrl )

#			sHtml = urlopen( sUrl )
#			print( sHtml )
#sSize = int( sHtml.headers['Content-Length'] )

	except Exception as e:
		print( 'Error During Download : ' + str(e) )
		return None
	


getVideo( sUrl )