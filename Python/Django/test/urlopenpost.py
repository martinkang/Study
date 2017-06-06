from urllib.request import urlopen

data = b"query=python"
f = urlopen( "http://www.example.com", data )
print ( f.read(300) )
