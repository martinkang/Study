import requests
sFiles = { 'uploadFile' : open( 'img.jpg', 'rb' ) }
sRes = requests.post( "http://pythonscraping.com/pages/processing2.php", files = sFiles )
print( sRes.text )
