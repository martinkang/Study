import requests
from requests.auth import AuthBase
from requests.auth import HTTPBasicAuth

sAuth = HTTPBasicAuth( 'ryan', 'password' )
sRes = requests.post( url = "http://pythonscraping.com/pages/auth/login.php", auth = sAuth )
print( sRes.text )
