import csv

class csvFunc:
	def readCsv():
		sResultSet = []

		try:
			sFile = open( './miniBook.csv', 'r', encoding='utf-8' )
			sCsv = csv.reader( sFile )

			for line in sCsv:
				sResultSet.append( tuple( line ) )

			sFile.close()
		except:
			print( "Create new miniBook.csv file" )

		return sResultSet


	def writeCsv( aComicList ):
		sFile = open( './miniBook.csv', 'w', encoding='utf-8', newline='' )
		sCsv = csv.writer( sFile )

		for comic in aComicList:
			sCsv.writerow( [ comic[0], comic[1], comic[2] ] )

		sFile.close()

