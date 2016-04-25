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
	- 
* 시간 복잡도
	- O(n^2)


## 2번째 알고리즘
* 기본 아이디어 
	- 1 일부터 N 일까지 대여 가능한 일자가 있을 때 ( 1 부터 N 까지는 연속된 수 ), 
	

* 방법
	* 최소 3팀이 공연할 때
	* ![img1]( https://github.com/martinkang/MyText/blob/master/Algorithm/Festival/img/festival1.jpg )
	- 공연 가능한 날이 i( C ) 까지일 때 ( A + B + C ) / 3 이 공연 대관료의 최소 평균값이다.
	- 공연 가능한 날이 i + 1( D ) 이라고 한다면, ( D ) 를 포함한 공연 대관료의 최소 평균값은 다음중 하나이다.
		* ( A + B + C + D ) / 4
		* ( B + C + D ) / 3	
	* ![img2]( https://github.com/martinkang/MyText/blob/master/Algorithm/Festival/img/festival2.jpg )
	- 공연 가능한 날이 E 까지라고 할 때 공연 대관료 최소 평균값은 다음 3 가지 중 하나이다.
		* D 를 포함한 최소 평균값 + E 를 포함
		* C + D + E 의 평균값 ( E 를 마지막날짜로 하는 최소 대여일 )
	* ![img3]( https://github.com/martinkang/MyText/blob/master/Algorithm/Festival/img/festival3.jpg )


* 시간 복잡도
	- O(n)
