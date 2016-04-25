# 록 페스티벌

## naive 알고리즘
* 기본 아이디어
	- 가능한 모든 조합중에 최소 평균값을 가지는 집합을 찾는다.

* 방법
	* 최소 3팀이 공연할 때
	* ![naive]( https://github.com/martinkang/MyText/blob/master/Algorithm/Festival/img/festival-naive.jpg )
	
	- A 를 기점으로 하루씩 늘려가며 공연장 대관료의 평균값을 구한다.
		- (좌) ( A + B + C ) / 3
		- (우) ( A + B + C + D ) / 4
		- .... ( A + B + .... F ) / 6
		
	* ![naive2]( https://github.com/martinkang/MyText/blob/master/Algorithm/Festival/img/festival-naive2.jpg )
	
	- A 부터 F 까지 빌렸을 때 공연장 대관료 평균값을 구했으면 B 부터 구했을 때의 평균값을 구한다.
	- 대여 가능한 일수 - 최소 공연팀 ( 이 예제에서는 D ) 부터 시작하는 공연장 대관료를 다 구했으면 { ( D + E + F ) / 3 } 구한 값들 중 최소값을 출력한다.
	

* 시간 복잡도
	- O(n^2)



## 2번째 알고리즘
* 기본 아이디어 
	- N 개의 대관 가능한 날이 있을 때 N + 1 의 대관료의 평균값이 최소로 나오게 빌리려면
		* ( N 번째 날을 포함한 최소 평균값 ) + ( N + 1 ) 빌림
		* N + 1 번째를 포함한 최소 공연팀의 수만큼 빌림
		* 이전 최소 평균값 만큼 빌림


* 방법
	* 최소 3팀이 공연할 때
	* ![img1]( https://github.com/martinkang/MyText/blob/master/Algorithm/Festival/img/festival1.png )
	- 대관 가능한 날이 3일이라면( C )  ( A + B + C ) / 3 이 공연 대관료의 최소 평균값이다. ( 선택권이 없으므로 )
	- 공연 가능한 날이 i + 1( D ) 이라고 한다면, ( D ) 를 포함한 공연 대관료의 최소 평균값은 다음중 하나이다.
		* ( A + B + C + D ) / 4
		* ( B + C + D ) / 3	
		* ( A + B + C ) / 3
	- 이 중 A + B + C 의 대관료 평균값은 이미 구한 상태이므로 위의 두가지 값을 비교하여 더 작은 값을 저장한다.
		- 이 예에선 ( A + B + C + D ) / 4 보다 ( B + C + D ) / 3 이 더 작다고 가정한다. 
		- ( A + B + C ) / 3 은 ( B + C + D ) /3  보다 더 작다
	* ![img2]( https://github.com/martinkang/MyText/blob/master/Algorithm/Festival/img/festival2.png )
	- 공연 가능한 날이 E 까지라고 할 때 E 를 포함한 공연 대관료 최소 평균값은 다음 중 하나이다.
		* D 를 포함한 최소 평균값 ( ( B + C + D ) / 3 )  과 새로운 E 를 대관한 평균 대관료
		* E 를 마지막날짜로 하는 최소 팀 수만큼 대여하는 대관료 ( C + D + E ) / 3
		* 이전 최소 평균값 ( 위 예제에서는 ( A + B + C ) / 3 )
	- D 를 포함한 최소 평균값은 이미 D 날까지의 대관료중 가장 적은 값이기 때문에, 
	* ![img3]( https://github.com/martinkang/MyText/blob/master/Algorithm/Festival/img/festival3.png )


* 시간 복잡도
	- O(n)
