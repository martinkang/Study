from math import sqrt

# 피어슨 상관계수 리턴
def sim_pearson( aVal1, aVal2 ):
	sSum1 = sum( aVal1 )
	sSum2 = sum( aVal2 )

	sSumSqrt1 = sum( ( pow( val, 2 ) for val in aVal1 ) )
	sSumSqrt2 = sum( ( pow( val, 2 ) for val in aVal2 ) )

	sSumMulty = sum( aVal1[i] * aVal2[i] for i in range( len( aVal1 ) ) )

	# 피어슨 점수 계산
	sPearson = sSumMulty - ( ( sSum1 * sSum2 ) / len( aVal1 ) )
	sDen = sqrt( ( sSumSqrt1 - pow( sSum1, 2 ) / len( aVal2 ) ) *  
			( sSumSqrt2 - pow( sSum2, 2 ) / len( aVal1 ) ) ) 

	if sDen == 0:
		return 0

	return 1.0 - sPearson / sDen

