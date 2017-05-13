import pymysql

sConn = pymysql.connect( host='127.0.0.1',
						 user = 'root', passwd ='3177', db = 'mysql' )

sCur = sConn.cursor()
sCur.execute( "USE scraping" )
sCur.execute( "desc pages" )

cur = sCur.fetchone()
while cur is not None:
	print( cur )
	cur = sCur.fetchone() 

sCur.close()
sConn.close()
