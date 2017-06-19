from winreg import *
import sys

def getValue( aKey, aKeyName, aHint ):
    try:
        for i in range( 1024 ):
            sName, sValue, sType = EnumValue( aKey, i )
            if ( aKeyName in sName and aHint in sValue ):
                print( sName, sValue )
    except:
        sErrMsg = "Exception Inner:", sys.exc_info()[0]

def searchNPrintRegistry( aRootKey, aSubKey, aKeyName, aHint ):
    sVarSubKey = aSubKey

    # ConnectRegistry(computer_name, key)
    # computer_name is the name of the remote computer, of the form r"\\computername". If None, the local computer is used.    
    sVarReg = ConnectRegistry( None, aRootKey )
    sVarKey = OpenKey( sVarReg, sVarSubKey )

    for i in range( 1024 ):
        try:
            # EnumKey(key, index) https://docs.python.org/2/library/_winreg.html
            sKeyName = EnumKey( sVarKey, i )
            sVarSubKey2 = "%s\\%s"%( sVarSubKey, sKeyName )
            sVarKey2 = OpenKey( sVarReg, sVarSubKey2 )

            getValue( sVarKey2, aKeyName, aHint )
            CloseKey( sVarKey2 )
        except: 
            # subKey 가 더 존재하지 않을경우
            getValue( sVarKey, aKeyName, aHint )
            break

    CloseKey( sVarKey )
    CloseKey( sVarReg )
    

sRootKeyUser = HKEY_LOCAL_MACHINE
# 레지스트리의 아래 주소를 보면 서브디렉터리에 사용자 계정 SID 항목이 존재한다.
sSubKeyUser = "SOFTWARE\Microsoft\Windows NT\CurrentVersion\ProfileList"
sUserKeyName = "ProfileImagePath"
print( "registry search for user list")
searchNPrintRegistry( sRootKeyUser, sSubKeyUser, sUserKeyName, "Users" )
                 
sRootKeyUrlLog = HKEY_CURRENT_USER
# 인터넷 접속로그는 아래 레지스트리 항목에 저장됨
sUrlLogSubKey = "Software\Microsoft\Internet Explorer\TypedURLs"
sUrlLogKeyName = "url"
print( "registry search for user url log")
searchNPrintRegistry( sRootKeyUrlLog, sUrlLogSubKey, sUrlLogKeyName, "" )


