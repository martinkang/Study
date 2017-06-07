import csv

class csvFunc:
	gCharSet = "utf-8"

	def __init__ ( self, aCharSet ):
		self.gCharSet = aCharSet


	def readCsv( self ):
		sResultSet = []

		try:
			sFile = open( './miniBook.csv', 'r', encoding = self.gCharSet )
			sCsv = csv.reader( sFile )

			for line in sCsv:
				sResultSet.append( tuple( line ) )

		except:
			print( "Create new miniBook.csv file" )
			sResultSet = None
		else:
			sFile.close()

		return sResultSet


	def writeCsv( self, aComicList ):
		sFile = open( './miniBook.csv', 'w', encoding = self.gCharSet, newline = '' )
		sCsv = csv.writer( sFile )

		for comic in aComicList:
			sCsv.writerow( [ comic[0], comic[1], comic[2] ] )

		sFile.close()

