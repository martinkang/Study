class printFunc():
	def comicTitle( aTitleList ):
		for title in aTitleList:
			print( title.strip('\n') )			

	def comicResult( aResultList ):
		for result in aResultList:
			print( "[" + result[0] + "] URL : " + result[2] )

