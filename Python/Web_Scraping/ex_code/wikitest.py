from urllib.request import urlopen
from bs4 import BeautifulSoup
import unittest

class TestWiki( unittest.TestCase ):
	sBsObj = None

	def setUpClass():
		global sBsObj
		sUrl = "http://en.wikipedia.org/wiki/Monty_Python"
		sBsObj = BeautifulSoup( urlopen( sUrl ) )

	def test_titleText( self ):
		global sBsObj
		sPageTitle = sBsObj.find("h1").get_text()
		self.assertEqual( "Monty Python", pageTitle )

	def test_contentExists( self ):
		global sBsObj
		sContent = sBsObj.find( "div", { "id":"mw-content-text" } )
		self.assertIsNotNone( content )


if __name__ == '__main__':
	unittest.main()
