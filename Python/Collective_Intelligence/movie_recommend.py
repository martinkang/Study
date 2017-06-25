from math import sqrt
import csv


# A dictionary of movie critics and their ratings of a small
# set of movies
sCritics={'Lisa Rose': {'Lady in the Water': 2.5, 'Snakes on a Plane': 3.5,
	'Just My Luck': 3.0, 'Superman Returns': 3.5, 'You, Me and Dupree': 2.5, 
	'The Night Listener': 3.0},
	'Gene Seymour': {'Lady in the Water': 3.0, 'Snakes on a Plane': 3.5, 
		'Just My Luck': 1.5, 'Superman Returns': 5.0, 'The Night Listener': 3.0, 
		'You, Me and Dupree': 3.5}, 
	'Michael Phillips': {'Lady in the Water': 2.5, 'Snakes on a Plane': 3.0,
		'Superman Returns': 3.5, 'The Night Listener': 4.0},
	'Claudia Puig': {'Snakes on a Plane': 3.5, 'Just My Luck': 3.0,
		'The Night Listener': 4.5, 'Superman Returns': 4.0, 
		'You, Me and Dupree': 2.5},
	'Mick LaSalle': {'Lady in the Water': 3.0, 'Snakes on a Plane': 4.0, 
		'Just My Luck': 2.0, 'Superman Returns': 3.0, 'The Night Listener': 3.0,
		'You, Me and Dupree': 2.0}, 
	'Jack Matthews': {'Lady in the Water': 3.0, 'Snakes on a Plane': 4.0,
		'The Night Listener': 3.0, 'Superman Returns': 5.0, 'You, Me and Dupree': 3.5},
	'Toby': {'Snakes on a Plane':4.5,'You, Me and Dupree':1.0,'Superman Returns':4.0}}


# 거리기반 유사도 점수 리턴
def sim_distance( aPrefs, aPerson1, aPerson2 ):
	sCommonItems = []

	# 공통항목 추출
	for item in aPrefs[aPerson1]:
		if item in aPrefs[aPerson2]:
			sCommonItems.append( item )

	if len( sCommonItems ) == 0:
		return 0

	sSumSqrt = sum( pow( aPrefs[aPerson1][item] - aPrefs[aPerson2][item], 2 )
					for item in sCommonItems )

	return 1 / ( 1 + sqrt( sSumSqrt ) )

# 피어슨 상관계수 리턴
def sim_pearson( aPrefs, aPerson1, aPerson2 ):
	sCommonItems = []

	# 공통항목 추출
	for item in aPrefs[aPerson1]:
		if item in aPrefs[aPerson2]:
			sCommonItems.append( item )

	sItemLen = len( sCommonItems )
	if sItemLen == 0:
		return 0

	sSumPrefs1 = sum( aPrefs[aPerson1][item] for item in sCommonItems )
	sSumPrefs2 = sum( aPrefs[aPerson2][item] for item in sCommonItems )

	sSumSqrt1 = sum( ( pow( aPrefs[aPerson1][item], 2 ) for item in sCommonItems ) )
	sSumSqrt2 = sum( ( pow( aPrefs[aPerson2][item], 2 ) for item in sCommonItems ) )

	sSumMulty = sum( ( aPrefs[aPerson1][item] * aPrefs[aPerson2][item] for item in sCommonItems ) )

	# 피어슨 점수 계산
	sPearson = sSumMulty - ( ( sSumPrefs1 * sSumPrefs2 ) / sItemLen )
	sDen = sqrt( ( sSumSqrt1 - pow( sSumPrefs1, 2 ) / sItemLen ) *  
			( sSumSqrt2 - pow( sSumPrefs2, 2 ) / sItemLen ) ) 

	if sDen == 0:
		return 0

	return sPearson / sDen

def getRecommendations( aPrefs, aPerson, aSimFunc ):
	sRecommendMovies = {}
	sSimSum = {}
	sRankings = []

	for other in aPrefs:
		if other == aPerson:
			continue

		sSim = aSimFunc( aPrefs, aPerson, other )
		if sSim < 0:
			continue

		for item in aPrefs[other]:
			# aPerson 이 못본 영화
			if item not in aPrefs[aPerson] or aPrefs[aPerson][item] == 0:
				sRecommendMovies.setdefault( item, 0 )
				# other 가 item 에 준 점수를 similarity 를 계산하여 점수를 준다.
				sRecommendMovies[item] += aPrefs[other][item] * sSim
				
				sSimSum.setdefault( item, 0 )
				sSimSum[item] += sSim

	for movie, score in sRecommendMovies.items():
		sRankings.append( ( score / sSimSum[movie], movie ) )
	
	sRankings.sort()
	sRankings.reverse()
	return sRankings

# sCritics 의 순서를 바꿈
def transformPrefs( aPrefs ):
	sResult = {}
	for person in aPrefs:
		for item in aPrefs[person]:
			sResult.setdefault( item, {} )

			sResult[item][person] = aPrefs[person][item]

	return sResult

'''
example
sMovies = transformPrefs( sCritics )

print( sim_distance( sCritics, 'Lisa Rose', 'Gene Seymour' ) )
print( sim_pearson( sCritics, 'Lisa Rose', 'Gene Seymour' ) )
print( getRecommendations( sCritics, 'Toby', sim_pearson ) )
print( sMovies )
'''

