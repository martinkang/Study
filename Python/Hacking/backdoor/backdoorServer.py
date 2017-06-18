from socket import *
HOST = ''
PORT = 11443

sSock = socket( AF_INET, SOCK_STREAM )
sSock.setsockopt( SOL_SOCKET, SO_REUSEADDR, 1 )
sSock.bind( ( HOST, PORT ) )
sSock.listen( 10 )

sConn, sAddr = sSock.accept()
print( "Conneted by", sAddr )
sData = sConn.recv( 1024 )

while 1:
    sCmd = input( "Enter shell command or quit: " )
    sConn.send( sCmd )
    if sCmd == ".quit" :
        print( "send quit message to clinet")
        break
    
    sData = sConn.recv( 1024 )
    print( sData )

sConn.close()
