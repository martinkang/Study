from urllib.request import urlopen
from bs4 import BeautifulSoup
import re
import datetime
import random
import pymysql

gConn = pymysql.connect( host='127.0.0.1',
		    			 user = 'root', passwd = 'pwd', db = 'mysql', charset = 'utf8' )

gCur = gConn.cursor()
gCur.execute( "USE scraping" )

random.seed( datetime.datetime.now() )


def store( aTitle, aContent ):
	gCur.execute( "insert into pages ( title, content ) values (\"%s\", \"%s\" )",
				  ( aTitle, aContent ) )
	gCur.connection.commit()


def getLinks( aUrl ):
	sHtml = urlopen( "http://en.wikipedia.org" + aUrl )
	sBsObj = BeautifulSoup( sHtml, "html.parser" )

	sTitle = sBsObj.find( "h1" ).get_text()
	if sTitle is not None:
		sContent = sBsObj.find( "div", {"id":"mw-content-text"} ).find( "p" ).get_text()
		store( sTitle, sContent )

		return sBsObj.find( "div", {"id":"bodyContent"} ). \
					  findAll( "a", href = re.compile( "^(/wiki/)((?!:).)*$" ))
	else:
		return None


sLinks = getLinks( "/wiki/Kevin_Bacon" )
try:
	while len( sLinks ) > 0:
		sNewArticle = sLinks[ random.randint( 0, len( sLinks ) - 1 ) ].attrs["href"]
		print( sNewArticle )
		sLink = getLinks( sNewArticle )
finally:
	gCur.close()
	gConn.close()
