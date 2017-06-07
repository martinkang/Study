class printFunc():
	def comicTitle( aTitleList ):
		if aTitleList is None:
			print( "검색 리스트가 없습니다." )
		else:
			for title in aTitleList:
				print( title.strip('\n') )			

	def comicResult( aResultList ):
		if aResultList is None:
			print( "결과 리스트가 없습니다." )
		else:
			for result in aResultList:
				print( "[" + result[0] + "]" ) 
				print( "\tURL : " + result[2] )

