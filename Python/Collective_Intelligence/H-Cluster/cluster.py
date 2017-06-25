from PIL import Image,ImageDraw
from sim import sim_pearson

def readfile( aFName ):
	sLines = [line for line in open( aFName )]

	sColNames = sLines[0].strip().split( '\t' )[1:]
	sRowNames = []
	sData = []

	for line in sLines[1:]:
		sTemp = line.strip().split( '\t' )
		sRowNames.append( sTemp[0] )
		sData.append( [float(val) for val in sTemp[1:]] )

	return sRowNames, sColNames, sData

class bicluster:
	def __init__( self, aVec, aLeft = None, aRight = None, aDistance = 0.0, aId = None ):
		self.mLeft = aLeft
		self.mRight = aRight
		self.mDistance = aDistance
		self.mId = aId
		self.mVec = aVec

def hcluster( aRows, aDistanceFunc = sim_pearson ):
	sDistances = {}
	sCurrentClusterId = -1

	# 각 Row 를 하나의 군집으로 생성
	sClust = [bicluster( aRows[i], aId = i ) for i in range( len( aRows ))]

	while len( sClust ) > 1:
		sLowstpair = ( 0, 1 )
		sClosest = aDistanceFunc( sClust[0].mVec, sClust[1].mVec )

		# 가장 작은 거리 값을 가지는 쌍을 찾는 루프
		for i in range( len( sClust ) ):
			for j in range( i + 1, len( sClust ) ):
				# sDistances 는 거리 계산 캐시
				if ( sClust[i].mId, sClust[j].mId ) not in sDistances:
					sDistances[( sClust[i].mId, sClust[j].mId )] = 	aDistanceFunc( sClust[i].mVec, sClust[j].mVec )

				sTemp = sDistances[( sClust[i].mId, sClust[j].mId )]
				if sTemp < sClosest:
					sClosest = sTemp
					sLowstpair = ( i, j )

		# 두 군집 간 평균을 계산함	
		sMergeVec = [
			( sClust[sLowstpair[0]].mVec[i] + sClust[sLowstpair[1]].mVec[i] ) / 2.0
			for i in range( len( sClust[0].mVec ) )]

		# 새로운 군집을 생성함
		sNewCluster = bicluster( sMergeVec, aLeft = sClust[sLowstpair[0]],
				aRight = sClust[sLowstpair[1]],
				aDistance = sClosest, aId = sCurrentClusterId ) 

		# 원래의 집합 안에 포함되지 않은 군집 id 들은 음수로 나타낸다.
		sCurrentClusterId -= 1
		del sClust[sLowstpair[1]]
		del sClust[sLowstpair[0]]
		sClust.append( sNewCluster )

	return sClust[0]

def printclust( aClust, aLabels = None, aNum = 0 ):
	sIndent = ''
	# 계층구조를 만들기 위해 들여 씀
	for i in range( aNum ):
		sIndent = sIndent +  ' '

	if aClust.mId < 0:
		# 음수 id 값은 트리의 브랜치를 뜻함
		print( sIndent, '-' )
	else:
		# 양수 id 값은 트리의 종점을 뜻함
		if aLabels == None:
			print( sIndent, aClust.mId )
		else:
			print( sIndent, aLabels[aClust.mId] )

	# 우측과 좌측 브랜치를 출력함
	if aClust.mLeft != None:
		printclust( aClust.mLeft, aLabels = aLabels, aNum = aNum + 1 )
	if aClust.mRight != None:
		printclust( aClust.mRight, aLabels = aLabels, aNum = aNum + 1 )

def getheight( aClust ):
	# 종점의 경우 높이는 1 임
	if aClust.mLeft == None and aClust.mRight == None:
		return 1

	# 그렇지 않으면 높이는 각 브랜치 높이들의 합임
	return getheight( aClust.mLeft ) + getheight( aClust.mRight )

def getdepth( aClust ):
	# 종점 거리는 0.0 임
	if aClust.mLeft == None and aClust.mRight == None:
		return 0

	# 브랜치의 거리는 양쪽 중 큰 것에 자신의 거리를 더한 값임
	return max( getdepth( aClust.mLeft ), getdepth( aClust.mRight ) ) + aClust.mDistance

# 최종 cluster 마다 높이 20 픽셀과 고정된 폭을 가진 이미지를 생성
def drawdendrogram( aClust, aLabels, aName ):
	# 높이와 폭
	sHeight = getheight( aClust ) * 20
	sWith = 1200
	sDepth = getdepth( aClust )

	# 고정 폭에 맞게 비율 고정
	sScale = float( sWith - 150 ) / sDepth

	# 흰색 배경의 새로운 이미지 생성
	sImg = Image.new( 'RGB', ( sWith, sHeight ), ( 255, 255, 255 ) )
	sDraw = ImageDraw.Draw( sImg )

	sDraw.line( ( 0, sHeight / 2, 10, sHeight / 2 ), fill = ( 255, 0, 0 ) )

	# 첫 번째 노드 그림
	drawnode( sDraw, aClust, 10, ( sHeight / 2 ), sScale, aLabels )
	sImg.save( aName, 'JPEG' )

def drawnode( aDraw, aClust, x, y, aScale, aLabels ):
	if aClust.mId < 0:
		sH1 = getheight( aClust.mLeft ) * 20
		sH2 = getheight( aClust.mRight ) * 20

		sTop = y - ( sH1 + sH2 ) / 2
		sBottom = y + ( sH1 + sH2 ) / 2

		# 선 길이
		sLine = aClust.mDistance * aScale
		# 이 군집에서 자식들까지 수직선
		aDraw.line( ( x, sTop + sH1 / 2, x, sBottom - sH2 / 2 ), fill = ( 255, 0, 0 ) )
		# 왼쪽 항목까지 수평선		
		aDraw.line( ( x, sTop + sH1 / 2, x + 11, sTop + sH1 / 2 ), fill = ( 255, 0, 0 ) )
		# 오른쪽 항목까지 수평선		
		aDraw.line( ( x, sBottom - sH2 / 2, x + 11, sBottom - sH2 / 2 ), fill = ( 255, 0, 0 ) )

		# 이 함수로 왼쪽 오른쪽 노드를 그림
		drawnode( aDraw, aClust.mLeft, x + 11, sTop + sH1 / 2, aScale, aLabels )
		drawnode( aDraw, aClust.mRight, x + 11, sBottom - sH2 / 2, aScale, aLabels )
	else:
		# 종점이면 항목 라벨을 그림
		aDraw.text( ( x + 5, y - 7 ), aLabels[aClust.mId], ( 0, 0, 0 ) )
