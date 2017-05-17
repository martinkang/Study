from urllib.request import urlretrieve
from urllib.request import urlopen
from bs4 import BeautifulSoup
import subprocess
import requests
from PIL import Image
from PIL import ImageOps


def cleanImage( aImgPath ):
	sImg = Image.open( aImgPath )
	sImg = sImg.point( lambda x : 0 if x < 143 else 255 )
	sBorderImg = ImageOps.expand( sImg, border = 20, fill = 'white' )
	sBorderImg.save( aImgPath )


sHtml = urlopen( "http://www.pythonscraping.com/humans-only" )
sBsObj = BeautifulSoup( sHtml, "html.parser" )

# 미리 만들어진 폼 값을 수집
sImgLocation = sBsObj.find( "img", { "title":"Image CAPTCHA" } )["src"]
sFormBuildId = sBsObj.find( "input", { "name":"form_build_id" } )["value"]
sCaptchaSId = sBsObj.find( "input", { "name":"captcha_sid" } )["value"]
sCaptchaToken = sBsObj.find( "input", { "name":"captcha_token" } )["value"]

sCaptchaUrl = "http://pythonscraping.com" + sImgLocation
# Captcha Url 을 받아와 이미지 다운로드
urlretrieve( sCaptchaUrl, "captcha.jpg" )
# 명암을 확실히 나눠 선명한 이미지를 얻는다.
cleanImage( "captcha.jpg" )

sProc = subprocess.Popen( ["tesseract", "captcha.jpg", "captcha" ], 
		stdout = subprocess.PIPE, stderr = subprocess.PIPE )
sProc.wait()

sFile = open( "captcha.txt", 'r' )

# 공백을 제거
sCaptchaRes = sFile.read().replace( " ", "" ).replace( "\n", "" )
print( "Captcha solution attempt : " + sCaptchaRes )

if len( sCaptchaRes ) > 0:
	sParams = { "captcha_token":sCaptchaToken, "captcha_sid":sCaptchaSId, 
		"form_id":"comment_node_page_form", "form_build_id":sFormBuildId, 
		"captcha_response":sCaptchaRes, "name":"Ryan Mitchell", 
		"subject":"I come to seek the Grail", 
		"comment_body[und][0][value]":"...and I am definitely not a bot" } 
	sRes = requests.post( "http://www.pythonscraping.com/comment/reply/10", data = sParams )

	sResObj = BeautifulSoup( sRes.text, "html.parser" )
	sResult = sResObj.find( "div", { "class":"messages" } )
	if sResult is not None:
		print( sResult.get_text() )

else:
	print( "There was a Problem reading the CAPTCHA correctly!" )


# subprocess.PIPE
#	Special value that can be used as the stdin, stdout or stderr argument to Popen and 
#	indicates that a pipe to the standard stream should be opened. 

# ImageOps : https://pillow.readthedocs.io/en/4.1.x/reference/ImageOps.html
# PIL.ImageOps.expand(image, border=0, fill=0)
#	Add border to the image
# Parameters:	
#	image – The image to expand.
#	border – Border width, in pixels.
#	fill – Pixel fill value (a color value). Default is 0 (black).
# Returns:	
#	An image.


