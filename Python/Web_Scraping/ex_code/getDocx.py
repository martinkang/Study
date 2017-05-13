from zipfile import ZipFile
from urllib.request import urlopen
from io import BytesIO

sWordFile = urlopen( "http://pythonscraping.com/pages/AWordDocument.docx" ).read()
# 바이너리 파일 객체로 읽음
sWordFile = BytesIO( sWordFile )

# zipfile 라이브러리로 압축을푼다. ( .docx 는 모두 압축되어 있기 때문 )
sDoc = ZipFile( sWordFile )
#					   	 positional argument: 'name'
sXmlContent = sDoc.read( 'word/document.xml' )

sWordObj = BeautifulSoup( sXmlContent.decode( 'utf-8' ), "html.parser" )
# 텍스트는 모두 <w:t> 태그 안에 들어있다.
sTxt = sWordObj.findAll( "w:t" )

for sStr in sTxt:
	print( sStr )
