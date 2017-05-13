from urllib.request import urlopen
from io import StringIO
import csv

sData = urlopen(
		"http://pythonscraping.com/files/MontyPythonAlbums.csv" ).read(). \
		decode( 'ascii', 'ignore' )
sDataFile = StringIO( sData )
#sCsvReader = csv.reader( sDataFile )
sDictReader = csv.DictReader( sDataFile )

print( sDictReader.fieldnames )

for row in sDictReader:
	print( "The album : " + row['Name'] + " was released in " + row['Year'] )
