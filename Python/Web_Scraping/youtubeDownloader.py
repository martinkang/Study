# youtube 의 동영상 주소형태는 다음과 같다.
# https://www.youtube.com/watch?v='Video_ID'
# 이 Video_ID 를 가져온 후
# http://www.youtube.com/get_video_info?video_id='Video ID' 를 이용하여 get_video_info 를 가져온다.
# get_video_info 에서 'title' 을 찾아 동영상 제목으로 하고
# url_encoded_fmt_stream_map 값 중 'url' 항목을 찾으면 해당 url 을 이용하여 동영상을 다운받을 수 있다.
# url_encoded_fmt_stream_map : video + audio combined
# adaptive_fmts : video or audio separate streams
import os
from urllib.request import urlopen, urlretrieve, urlparse
from urllib.parse import parse_qs
from bs4 import BeautifulSoup

KB_SIZE = 1024
MB_SIZE = KB_SIZE * 1024
GB_SIZE = MB_SIZE * 1024

# 테스트용 주소
#sUrl = "https://www.youtube.com/watch?v=bJqwSFFSHdI"
sUrl = "https://www.youtube.com/watch?v=t4pbzzKo2G0" #403 Error 나는 주소
# 해결책 : http://stackoverflow.com/questions/21496126/android-youtube-download-url-403-forbidden
# https://github.com/lure/YoutubeGrabber

def getVideoID( aUrl ):
	sUrl = aUrl.split( '/' )
	try:
		if "youtube" not in sUrl[2]:
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
		sTitle = "non_title"
	
	return sTitle


def getVideoURL( aInfo ):
	return aInfo['url_encoded_fmt_stream_map'][0]


def getFileName( aTitle ):
	sFileNo = 1
	sFileName = aTitle + '.mp4'

	while os.path.exists( sFileName ) == True:
		sFileName = aTitle + '_' + str( sFileNo ) + '.mp4'
		sFileNo = sFileNo + 1

	if os.path.isdir( 'download' ) == False:
		os.mkdir( 'download' )

	return 'download/' + sFileName


# 미완성
# youtube 동영상중 signature, s 도는 sig 항복을 포함하는 동영상이 있다.
# s 나 sig 는 이를 해독하여 url 에 합쳐 보내줘야 한다.
def getSignature( aInfo ):
	try:
		return  aInfo['s'][0]
	except:
		return  None

	if sSig is None:
		try:
			return aInfo['sig'][0]
		except:
			return None


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
			sSig = getSignature( sItem )
			sHtml = urlopen( sUrl + '&signature=' + sSig )
			sSize = int( sHtml.headers['Content-Length'] )

			sFileName = getFileName( sTitle )
			print( sFileName )
			sFile = open( sFileName , 'wb+' )

			sDownSize = 0
			sBuf = sHtml.read( KB_SIZE )
			while sBuf:
				sFile.write( sBuf )
				sDownSize += KB_SIZE
				sBuf = sHtml.read( KB_SIZE )

			sFile.close()
			sHtml.close()

			print( 'Download ' + sTitle + ' is Complete!! [ Size = ' + str( sDownSize / KB_SIZE ) + ' KB ] ' )
			break

	except Exception as e:
		print( 'Error During Download : ' + str(e) + "\nURL : " + sUrl )
		return None


#sUrl = input( "input youtube url : " )
getVideo( sUrl )
