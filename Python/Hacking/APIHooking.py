import utils, sys
from pydbg import *
from pydbg.defines import *

# Python 2.7

'''
BOOL WINAPI WriteFile(
    _In_        HANDLE hFile,
    _In_        LPCVOID lpBuffer,
    _In_        DWORD nNumberOfBytesToWrite,
    _Out_opt_   LPDWORD lpNumberOfBytesWritten,
    _Inout_opt_ LPOVERLPAPPED lpOverlapped
    );
'''

sDbg = pydbg()
sIsProcess = False
sProcessName = "notepad.exe"

gOriginPattern = "love"
gReplacePattern = "hate"


def replaceString( aDbg, aArgs ):
    sBuffer = aDbg.read_process_memory( aArgs[1], aArgs[2] )

    if gOriginPattern in sBuffer:
        print "[APIHooking] Before : %s" % sBuffer
        sBuffer = sBuffer.replace( gOriginPattern, gReplacePattern )
        sReplace = aDbg.write_process_memory( aArgs[1], sBuffer )

        print "[APIHooking] After : %s" \
        % aDbg.read_process_memory( aArgs[1], aArgs[2] )

    return DBG_CONTINUE


# 프로세스 id 리스트 얻기
for( pid, name ) in sDbg.enumerate_processes():   
    if name.lower() == sProcessName:
        sIsProcess = True
        sHooks = utils.hook_container()

        # 프로세스 핸들 구하기.
        '''
        여기서 DebugActiveProcess(pid): 지원되지 않는 요청입니다. 에러가 나는 경우가 있다.
        이는 후킹 타겟과 후커가 실행 비트가 달라서 뜨는 에러이다.
        여기서 notepad 의 경우 C:\Windows\SysWOW64\notepad.exe 로 실행시키면 해결된다.
        SysWOW64 는 32비트 프로세스를 위한 폴
        '''
        sDbg.attach( pid )
        print" Saves a process handle in self.h_process of pid[%d]" % pid

        # 중단점을 설치할 함수의 주소 구하기
        sHookAddress = sDbg.func_resolve_debuggee( "kernel32.dll", "WriteFile" )

        if sHookAddress:
            # 중단점 설정
            sHooks.add( sDbg, sHookAddress, 5, replaceString, None )
            print "sets a breakpoint at the designated address : 0x%08x" \
            % sHookAddress

            break
        else:
            print "[Error] : couldn`t resolv hook address"
            sys.exit( -1 )

if sIsProcess:
    print "waiting for occurring debugger event"
    sDbg.run()
else:
    print "[Error] : There in no process [%s]" % sProcessName
    sys.exit( -1 )
    

        
