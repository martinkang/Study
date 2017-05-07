import os
from urllib.request import urlretrieve
from urllib.request import urlopen
from bs4 import BeautifulSoup

sDownDir = "downloaded"
sBaseUrl = "http://pyhonscraping.com"


def getAbsURL( aUrl, aSrc ):
	if aSrc.startswith( "http://www." ):
		sUrl = "http://" + aSrc[11:]
	elif aSrc.startswith( "http://" ):
		sUrl = aSrc
	elif aSrc.startswith( "www." ):
		sUrl = "http://" + aUrl[4:]
	else:
	 	sUrl = aUrl + "/" + aSrc

	return sUrl

def getDownPath( aUrl, aDownDir ):
	sFileName = aUrl[aUrl.rfind('/') + 1:]
	sPath = aDownDir + "/" + sFileName

	sDir = os.path.dirname( sPath )
	if not os.path.exists( sDir ):
		os.makedirs( sDir )

	print( "Download Path : " + sPath )
	return sPath

sHtml = urlopen( "http://www.pythonscraping.com" )
sBsObj = BeautifulSoup( sHtml, "html.parser" )
sDownList = sBsObj.findAll( src = True )

for sDownload in sDownList:
	sFileUrl = getAbsURL( sBaseUrl, sDownload["src"] )
	if sFileUrl is not None:
		if '.jpg' in sFileUrl:
			print( sFileUrl )
			urlretrieve( sFileUrl, getDownPath( sFileUrl, sDownDir ) )


