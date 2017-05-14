# pip3 install requests
import requests

# firstname 과 lastname 은 입력 필드의 name
sParams = { 'firstname' : 'Ryan', 'lastname' : 'Michell' }
#processing.php 는 폼이 실제로 동작하는 곳 <form method="post" action="processing.php" >
sRes = requests.post( "http://pythonscraping.com/files/processing.php", data = sParams )
print( sRes.text )
