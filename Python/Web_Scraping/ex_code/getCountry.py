import json
from urllib.request import urlopen

def getCountryByIP( aIpAddr ):
	try:
		sRes = urlopen( "http://freegeoip.net/json/" + aIpAddr ). \
				    	read().decode( 'utf-8' )
	except HTTPError:
		return None

	sResJson = json.loads( sRes )
	return sResJson.get( "country_code" ).strip()
