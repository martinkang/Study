
from winreg import *
import sys

# 방화벽 관련 설정 정보
sVarSubKey = "SYSTEM\CurrentControlSet\services\SharedAccess\Parameters\FirewallPolicy"
# 홈 또는 회사 네트워크 레지스트리 키
sVarStd = "\StandardProfile"
# 공용 레지스트리 키
sVarPub = "\PublicProfile"
# 방화벽 사용 여부
sVarEnbKey = "EnableFirewall"
sVarOff = 0

try:
    sVarReg = ConnectRegistry( None, HKEY_LOCAL_MACHINE )

    sVarKey = CreateKey( sVarReg, sVarSubKey + sVarStd )
    SetValueEx( sVarKey, sVarEnbKey, sVarOff, REG_DWORD, sVarOff )
    CloseKey( sVarKey )

    sVarKey = CreateKey( sVArReg, sVarSubKey + sVarPub )
    SetValueEx( sVarKey, sVarEnbKey, sVarOff, REG_DWORD, sVarOff )
    CloseKey( sVarKey )
except:
    sErrMsg = "Exception Outter: ", sys.exc_info()[0]
    print( sErrMsg )

CloseKey( sVarReg )