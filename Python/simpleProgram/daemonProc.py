import os
import time
import sys

class Dummy:
    def write( self, s ):
        pass
    
pid = os.fork()

if pid:
    print( "parent[%d] exit" % os.getpid() )
    os._exit(0)

print( "child[%d] start" % os.getpid() )
# 독립적인 그룹을 생성
os.setpgrp()
# 프로그램이 지정하는 파일 모드를 그대로 생성하기 위해 마스크 비트를 0 으로 설정
os.umask(0)
# 콘솔 입출력으로 인한 문제를 예방하기 위해 전환.
sys.stdin.close()
sys.stdout = Dummy()
sys.stderr = Dummy()

# 여기서부터 데몬 프로그램 작성을 시작하면 된다.
# 데몬은 5초뒤 자동을 ㅗ종료
time.sleep(5)
sys._exit(0)