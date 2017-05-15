import requests
sParams = { 'username' : 'Ryan', 'password' : 'password' }
sRes = requests.post( "http://pythonscraping.com/pages/cookies/welcome.php", sParams )
print( "Cookie is set to : " )
print( sRes.cookies.get_dict() )
print( "--------------" )
print( "Going to profile page..." )
sRes = requests.get( "http://pythonscraping.com/pages/cookies/profile.php", 
		cookies = sRes.cookies )
print( sRes.text )
