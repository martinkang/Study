import requests
from bs4 import BeautifulSoup

sSession = requests.Session()
sUrl = "https://www.whatismybrowser.com/"

print( "before Header Modify" )
sReq = sSession.get( sUrl )
sBsObj = BeautifulSoup( sReq.text, "html.parser" )
print( sBsObj.find( "div", {"class":"user-agent" } ).get_text() )

print( "\n" )
print( "after Header Modify" )

sHeaders = { "User-Agent":"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_5)" \
	"AppleWebKit 537.36(KHTML, like Gecko) Chrome",
		 "Accept":"text/html,application/xhtml+xml,application/xml;q=0.9," \
			 "image/webp,*/*;q=0.8" }

sReq = sSession.get( sUrl, headers = sHeaders )
sBsObj = BeautifulSoup( sReq.text, "html.parser" )
print( sBsObj.find( "div", {"class":"user-agent" } ).get_text() )

# python 스크레이퍼에서 보내는 헤더의 경우
# Accept-Encording : identify
# User-Agent : Python-urllib/3.4
# 와 같다. 이럴경우 봇으로 의심받기 때문에 헤더를 위와같이 수정하여 차단을 피한다.
# 보통 User-Agent 와 Accept-Language 같은 필드를 수정하는 것이 좋다.
# 헤더를 모바일 형식으로 바꿀 경우 사이트가 더 간단해질 경우가 많아 사이트 스크랩이 더 쉬워진다.
# User-Agent:Mozilla/5.0 (IPhone; IPhone OS 7_1_2 like Mac OS X)
#			 AppleWebKit/537.51.2 (KHTML, like Gecko) Version/7.0 Mobile/11D257 Safari/9537.53

