'##############################################################################
'#
'#  This is one of the include files for << RFO-BASIC! Quick APK >>
'#
'#  "utils.inc" contains some utility SUBs and FUNCTIONs
'#
'##############################################################################

%HTTP_TIMEOUT         = 5000
%Port                 = 80

'------------------------------------------------------------------------------
MACRO NUL = BYVAL 0&
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
FUNCTION DOS2WIN(t AS STRING) AS STRING
    LOCAL dos, win AS ASCIIZ * %MAX_PATH
    dos = t
    OemToChar(dos, win) 'Console to Windows
    FUNCTION = (win)
END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
FUNCTION EXIST(BYVAL fileOrFolder AS STRING) AS LONG
    LOCAL Dummy&
    ON ERROR GOTO Inexistant
    Dummy& = GETATTR(fileOrFolder)
    Inexistant:
    RESUME FLUSH
    FUNCTION = (ERRCLEAR = 0)
END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
FUNCTION Execute (Program AS STRING, Arguments AS STRING, nShow AS LONG) AS DWORD
'   Advanced Shell. Example of how to use :
'   Local PID As Dword
'   PID = Execute( Environ$("COMSPEC"), "/C dir " & $Dq & folderName & $Dq & " /ad /s /b > tmp.txt", 0 )
'   WaitForSingleObject PID, %INFINITE    ' activate this line if you want synchronous shell
'   CloseHandle PID
    LOCAL ShellInfo AS SHELLEXECUTEINFO
    ShellInfo.cbSize = SIZEOF(ShellInfo)
    ShellInfo.fMask = %SEE_MASK_FLAG_NO_UI OR %SEE_MASK_NOCLOSEPROCESS
    ShellInfo.lpFile = STRPTR(Program)
    ShellInfo.lpParameters = STRPTR(Arguments)
    ShellInfo.nShow = nShow
    IF ShellExecuteEx(ShellInfo) THEN FUNCTION = ShellInfo.hProcess
END FUNCTION
'------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
MACRO START(fn)
    IF hThreadMain THEN KillThread hThreadMain
    THREAD CREATE fn(BYVAL 0) TO hThreadMain
END MACRO
'--------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO EXIT_THREAD
    FUNCTION = 1
    PAUSE 500
    DIALOG POST hDlg, %WM_KILL_THREAD, 0, 0
    EXIT FUNCTION
END MACRO
'--------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB WaitForThread(BYREF TID AS DWORD) ' deadline in second
    LOCAL lRes AS LONG
    DO ' Wait for the thread to finish
        DIALOG DOEVENTS
        SLEEP 0
        lRes = WaitForSingleObject(TID, 100)
    LOOP UNTIL lRes <> %WAIT_TIMEOUT
    KillThread TID
END SUB
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB KillThread(BYREF TID AS DWORD)
    IF TID = 0 THEN EXIT SUB
    TID = 0
    LOCAL lRes AS LONG
    THREAD SUSPEND TID TO lRes
    CloseHandle TID
    THREAD CLOSE TID TO lRes
END SUB
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB PAUSE(timing AS LONG) ' timing in milli-second
    LOCAL startTime AS DOUBLE
    startTime = TIMER ' in SECOND
    DO ' Wait for the thread to finish
        DIALOG DOEVENTS ' Process dialog message queue
        SLEEP 0
    LOOP UNTIL 1000*(TIMER-startTime) > timing
END SUB
'------------------------------------------------------------------------------

'-----------------------------------------------------------------------------------------------------
MACRO GetSiteFromUrl(uri)
    site = PARSE$(uri, "/", 3) ' "http://monsite.com" or "https://monsite.com/page.html"
    IF INSTR(site, "?") <> 0 THEN site = PARSE$(site, "?", 1) ' "monsite.com?a=123"
END MACRO
'-----------------------------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
FUNCTION HttpGet(URL AS STRING, BYREF TCPBuffer AS STRING, OPTIONAL BYVAL buffersize AS LONG) AS DOUBLE
' Returns -1 if there was an error
' Returns the time needed to download the file (stored in TCPBuffer) otherwise
    LOCAL myURL, site, buffer AS STRING
    LOCAL httpTime, startTime, endTime AS DOUBLE
    LOCAL hThread AS DWORD
    LOCAL i, lRes, fNumber, downloaded, percent AS LONG
    LOCAL sentURL AS ASCIIZ * %MAX_PATH

    IF buffersize > 0 THEN INIT_PROGRESS_DL

    myURL = URL ' can start with "http://" or "https://" or directly "mysite.com"
    IF LEFT$(LCASE$(myURL), 4) <> "http" THEN myURL = "http://" + myURL

    TCPBuffer = ""
    sentURL = myURL
    THREAD CREATE TcpOpen(VARPTR(sentURL)) TO hThread
    startTime = TIMER ' ATTENTION! in SECONDs
    DO
        THREAD STATUS hThread TO lRes
        IF lRes = 0 OR lRes = &H103 THEN PAUSE 10 ' Thread still running
    LOOP UNTIL (lRes <> 0 AND lRes <> &H103) OR (1000 * (TIMER - startTime) > %HTTP_TIMEOUT) ' ATTENTION! *1000 car en ms
    endTime = TIMER ' ATTENTION! in SECONDs

    IF lRes = 0 OR lRes = &H103 THEN    ' Thread still running, means we hit time-out!
        KillThread hThread              ' -> we kill it
        FUNCTION = -1

    ELSEIF lRes < 0 THEN                ' Thread completed in time but sent an "Error cannot open TCP"
        FUNCTION = lRes
        THREAD CLOSE hThread TO lRes

    ELSE                                ' Thread completed and TCP opened correctly
        fNumber = lRes
        THREAD CLOSE hThread TO lRes
        GetSiteFromUrl(myURL)

        ' Now get the TCP packets (can take as much time as needed)
        TCP PRINT #fNumber, "GET " + myURL + " HTTP/1.0"
        TCP PRINT #fNumber, "Accept: */*"
        TCP PRINT #fNumber, "Accept-Language: en-us"
        TCP PRINT #fNumber, "Host: " + site
        TCP PRINT #fNumber, "Pragma: no-cache"
        TCP PRINT #fNumber, "Referer: http://" + site + "/"
        TCP PRINT #fNumber, "UserAgent: webget 1.2 (" + site + ")"
        TCP PRINT #fNumber, ""

        DO
            TCP RECV #fNumber, 1024, buffer
            TCPBuffer += buffer
            IF buffersize > 0 THEN
                downloaded += LEN(buffer)
                percent = INT(downloaded * 100 / buffersize)
                IF percent > 0 THEN REFRESH_PROGRESS_DL(MIN(100,percent))
            END IF
        LOOP WHILE LEN(buffer) AND ISFALSE ERR

        IF ERR THEN               ' error in packets received
            FUNCTION= -1
            TCP CLOSE #fNumber

        ELSE                      ' all packets received -> remove the header
            IF buffersize > 0 THEN REFRESH_PROGRESS_DL(100)
            TCP CLOSE #fNumber
            i = INSTR(TCPBuffer, "Server:")
            IF i <> 0 THEN
                i = INSTR(i, TCPBuffer, $CRLF + $CRLF) + 3
                TCPBuffer = RIGHT$(TCPBuffer, LEN(TCPBuffer) - i)
            END IF

            endTime = TIMER ' ATTENTION! in SECONDs
            httpTime = endTime - startTime
            FUNCTION = httpTime ' in seconds, DOUBLE floating point (resolution 1/100 of a second)
        END IF
    END IF

END FUNCTION
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
THREAD FUNCTION TcpOpen(BYVAL UrlPtr AS DWORD) AS LONG ' URL needs to start with "http://"
' Returns -1 if cannot open TCP
' Returns +1 otherwise
    LOCAL fNumber AS LONG
    LOCAL URL, site AS STRING
    LOCAL adr AS ASCIIZ POINTER

    adr = UrlPtr
    URL = TRIM$(@adr)
    GetSiteFromUrl(URL)

    fNumber = FREEFILE
    TCP OPEN PORT %Port AT site AS #fNumber TIMEOUT %HTTP_TIMEOUT

    IF ERR THEN
        FUNCTION= -1 ' could not open TCP
        TCP CLOSE #fNumber
    ELSE
        FUNCTION = fNumber
    END IF

END FUNCTION
'--------------------------------------------------------------------------------

'------------------------------------------------------------------------------
FUNCTION ASSIGNACCEL(tAccel AS ACCELAPI, BYVAL wKey AS WORD, BYVAL wCmd AS _
    WORD, BYVAL byFVirt AS BYTE) AS LONG
    tAccel.fVirt = byFVirt
    tAccel.key   = wKey
    tAccel.cmd   = wCmd
END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
FUNCTION AccelOkCancel(BYVAL hDlg AS DWORD) AS DWORD
    LOCAL hAccel   AS DWORD
    LOCAL tAccel() AS ACCELAPI
    DIM   tAccel(1 TO 2)

    ASSIGNACCEL tAccel(1), 13, %IDOK,  %FVIRTKEY OR %FNOINVERT     ' Return
    ASSIGNACCEL tAccel(2), 27, %IDCANCEL, %FVIRTKEY OR %FNOINVERT  ' Escape

    ACCEL ATTACH hDlg, tAccel() TO hAccel
    FUNCTION = hAccel
END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
'   ** Accelerator (aka hot keys) attachments **
'------------------------------------------------------------------------------
FUNCTION AccelAttach(BYVAL hWin AS DWORD, listofkeys AS STRING, listofmsg AS STRING) AS DWORD
    ' Example of usage: AccelAttach hDlg, "Ctl+N, Ctl+O, Ctl+S", _
    '                   TRIM$(%IDC_TB_NEW) + "," + TRIM$(%IDC_TB_OPEN) + "," + TRIM$(%IDC_TB_SAVE)
    ' will attach accelerator Ctrl+N to the message %IDC_TB_NEW, Ctrl+O to %IDC_TB_OPEN and Ctrl+S to %IDC_TB_SAVE
    ' Supported key codes are: "Ctl+X" or "Ctrl+X", "Alt+X", "Shift+X", or just "X"
    LOCAL hAccel   AS DWORD
    LOCAL tAccel() AS ACCELAPI
    LOCAL i, controlkey AS LONG
    LOCAL key AS STRING

    DIM tAccel(1 TO TALLY(listofkeys, ","))

    FOR i = 1 TO LEN(listofkeys)
        key = UCASE$(TRIM$(PARSE$(listofkeys, i)))
        IF INSTR(key, "CTL+") = 1 OR INSTR(key, "CTRL+") = 1 THEN
            controlkey = %FCONTROL
            key = RIGHT$(key, 1)
        ELSEIF INSTR(key, "ALT+") = 1 THEN
            controlkey = %FALT
            key = RIGHT$(key, 1)
        ELSEIF INSTR(key, "SHIFT+") = 1 THEN
            controlkey = %FSHIFT
            key = RIGHT$(key, 1)
        END IF
        ASSIGNACCEL tAccel(i), ASC(key), VAL(TRIM$(PARSE$(listofmsg, i))), %FVIRTKEY OR %FNOINVERT OR controlkey
    NEXT

    ACCEL ATTACH hWin, tAccel() TO hAccel
    FUNCTION = hAccel
END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO LinuxName(tx) = PARSE$(tx, "/", PARSECOUNT(tx, "/"))
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO LinuxPath(tx) = LEFT$(tx, INSTR(-1, tx, "/"))
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO WindowsName(tx) = PARSE$(tx, "\", PARSECOUNT(tx, "\"))
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO WindowsPath(tx) = LEFT$(tx, INSTR(-1, tx, "\"))
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
FUNCTION StrReplace(mainstring AS STRING, look4 AS STRING, replacement AS STRING) AS STRING
    LOCAL e AS STRING
    e = mainstring
    REPLACE look4 WITH replacement IN e
    FUNCTION = e
END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
%CSIDL_LOCAL_APPDATA  = &H001c ' <user name>\Local Settings\Application Data (non roaming)
DECLARE FUNCTION SHGetFolderPath LIB "Shell32.dll" _
    ALIAS "SHGetFolderPathA" (BYVAL hwnd AS DWORD, BYVAL csidl AS LONG, _
    BYVAL hToken AS DWORD, BYVAL dwFlags AS DWORD, pszPath AS ASCIIZ) AS LONG
'------------------------------------------------------------------------------
FUNCTION CreateLocalAppdataFolder AS STRING
    LOCAL szBaseFolder AS ASCIIZ * %MAX_PATH
    LOCAL folder AS STRING
    ShGetFolderPath (BYVAL %NULL, _             ' hWnd, "typically null"
                     %CSIDL_LOCAL_APPDATA, _    ' (supported) CSIDL value
                     BYVAL %NULL, _             ' hToken for security, null  = Default
                     BYVAL 0, _                 ' above
                     szBaseFolder)
    folder = TRIM$(szBaseFolder) + "\" + EXE.NAME$
    MKDIR folder
    FUNCTION = folder
END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
FUNCTION WinVer() AS STRING
        LOCAL osi AS OSVERSIONINFOEX
        LOCAL lResult AS LONG, sVer AS STRING
            osi.dwOsVersionInfoSize = SIZEOF(OSVERSIONINFOEX)
            lResult = GetVersionEx(BYVAL VARPTR(osi))
        IF ISFALSE lResult THEN
            osi.dwOsVersionInfoSize = SIZEOF(OsVersionInfo)
            lResult = GetVersionEX(BYVAL VARPTR(osi))
        END IF
        SELECT CASE osi.dwPlatformID
            CASE %VER_PLATFORM_WIN32_NT 'NT-2000-XP-2003 Platform
                LOCAL ver AS STRING
                ver = TRIM$(osi.dwMajorVersion) + "." + TRIM$(osi.dwMinorVersion)
                SELECT CASE ver
                    CASE "5.0" : sVer = "Windows 2000"
                    CASE "5.1" : sVer = "Windows XP"
                    CASE "5.2"
                        IF (osi.wProductType = %VER_NT_WORKSTATION) AND (is32or64bitOs=64) THEN _
                            sVer = "Windows XP Professional x64 Edition" ELSE
                            sVer = "Windows Server 2003"
                    CASE "6.0" : sVer = "Windows Vista"
                    CASE "6.1" : sVer = "Windows 7"
                    CASE "6.2" : sVer = "Windows 8"
                    CASE "6.3" : sVer = "Windows 8.1"
                END SELECT
                IF osi.dwMajorVersion <= 4 THEN _
                    sVer = "Microsoft Windows NT " & TRIM$(STR$(osi.dwMajorVersion)) _
                        & "." & TRIM$(STR$(osi.dwMinorVersion))
                sVer += $SPC
                SELECT CASE osi.wProductType
                    CASE %VER_NT_WORKSTATION 'workstation Product
                        IF osi.dwMajorVersion <= 4 THEN
                            sver = sver & "Workstation "
                        ELSEIF (osi.wSuiteMask AND %VER_SUITE_PERSONAL) THEN
                            sver = sver & "Home Edition "
                        ELSE
                            sver = sver & "Professional "
                        END IF
                    CASE %VER_NT_DOMAIN_CONTROLLER, %VER_NT_SERVER  'Server Product
                        IF osi.dwMajorVersion = 5 AND osi.dwMinorVersion = 2 THEN
                            IF (osi.wSuiteMask AND %VER_SUITE_DATACENTER) THEN
                                sver = sver & "Datacenter Edition "
                            ELSEIF (osi.wSuiteMask AND %VER_SUITE_ENTERPRISE) THEN
                                sver = sver & "Enterprise Edition "
                            ELSEIF (osi.wSuiteMask AND %VER_SUITE_BLADE) THEN
                                sver = sver & "Web Edition "
                            ELSE
                                sver = sver & "Standard Edition "
                            END IF
                        ELSEIF osi.dwMajorVersion = 5 AND osi.dwMinorVersion = 0 THEN
                            IF (osi.wSuiteMask AND %VER_SUITE_DATACENTER) THEN
                                sver = sver & "Datacenter Server "
                            ELSEIF (osi.wSuiteMask AND %VER_SUITE_ENTERPRISE) THEN
                                sver = sver & "Advanced Server "
                            ELSE
                                sver = sver & " Server "
                            END IF
                        ELSE
                            IF (osi.wSuiteMask AND %VER_SUITE_ENTERPRISE) THEN
                                sver = sver & "Server, Enterprise Edition "
                            ELSE
                                sver = sver & "Server "
                            END IF
                        END IF
                    END SELECT
                sver = sver & osi.szCSDVersion
            CASE %VER_PLATFORM_WIN32_WINDOWS '95-98-ME Platform
                        IF osi.dwMajorversion = 4 AND osi.dwMinorVersion = 0 THEN
                            sver = "Microsoft Windows 95 "
                                IF TRIM$(osi.szCSDVersion) = "C" OR TRIM$(osi.szCSDVersion) = "B" THEN _
                                sver = sver & "OSR2 "
                        END IF
                        IF osi.dwMajorVersion = 4 AND osi.dwMinorVersion = 10 THEN
                            sver = "Microsoft Windows 98 "
                                IF TRIM$(osi.szCSDVersion) = "A" THEN _
                                    sver = sver & "Second Edition"
                       END IF
                       IF osi.dwMajorversion = 4 AND osi.dwMinorVersion = 90 THEN _
                           sver = "Microsoft Windows Millennium Edition "
            CASE %VER_PLATFORM_WIN32s '3.1, 3.11 with win32s installed
                sver = "Microsoft Win32s"
        END SELECT
    FUNCTION = sver
END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
FUNCTION is32or64bitProc AS LONG
    LOCAL si AS SYSTEM_INFO
    GetNativeSystemInfo(si)
    SELECT CASE LOWRD(si.dwOemID)
        CASE %PROCESSOR_ARCHITECTURE_AMD64, %PROCESSOR_ARCHITECTURE_IA64
            FUNCTION = 64
        CASE ELSE
            FUNCTION = 32
    END SELECT
END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
DECLARE FUNCTION U_IsWow64Process  (BYVAL hProcess AS LONG, Wow64Process AS LONG) AS LONG
'------------------------------------------------------------------------------
FUNCTION is32or64bitOS AS LONG
    LOCAL szLib, szProc AS ASCIIZ * 64
    LOCAL Wow64, hLib AS LONG
    LOCAL dwProc AS DWORD
    szLib =  "KERNEL32.DLL"
    hLib   =  GetModuleHandle (szLib)
    Wow64  = 32   ' only set to 64 when it is.
    szProc = "IsWow64Process"
    dwProc =  GetProcAddress (hLib, szProc)
    IF ISTRUE dwProc THEN     '  must be on XP+
        CALL DWORD dwProc USING U_IsWow64Process (GetCurrentProcess(), Wow64)
    END IF
    FUNCTION = IIF(ISTRUE Wow64, 64, 32)
END FUNCTION
'------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
FUNCTION RUN_CMD(BYVAL cmdLine AS STRING, OPTIONAL BYVAL folder AS STRING) AS LONG
    LOCAL rep AS STRING
    LOCAL ff AS LONG
    LOCAL PID AS DWORD

    IF folder <> "" THEN rep = folder ELSE rep = ENVIRON$("TEMP") + "\"
    rep = RTRIM$(rep, "\") + "\"

    ff = FREEFILE
    OPEN rep + "temp.bat" FOR OUTPUT AS #ff
        PRINT #ff, "@echo off"
        PRINT #ff, LEFT$(rep, 2)
        PRINT #ff, "cd " + $DQ + rep + $DQ
        PRINT #ff, cmdLine
    CLOSE

    PID = Execute(rep + "temp.bat", "", 0)
    WaitForThread PID ' Wait for the script to finish

    KILL rep + "temp.bat"
END FUNCTION
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
FUNCTION DUMP_CMD(BYVAL cmdLine AS STRING, OPTIONAL BYVAL folder AS STRING) AS STRING
    LOCAL rep, e AS STRING
    LOCAL ff AS LONG
    LOCAL PID AS DWORD

    IF folder <> "" THEN rep = folder ELSE rep = ENVIRON$("TEMP") + "\"
    rep = RTRIM$(rep, "\") + "\"

    ff = FREEFILE
    OPEN rep + "temp.bat" FOR OUTPUT AS #ff
        PRINT #ff, "@echo off"
        PRINT #ff, LEFT$(rep, 2)
        PRINT #ff, "cd " + $DQ + rep + $DQ
        PRINT #ff, cmdLine + " > dump.txt 2>&1"
    CLOSE

    PID = Execute(rep + "temp.bat", "", 0)
    WaitForThread PID ' Wait for the script to finish

    KILL rep + "temp.bat"

    ff = FREEFILE
    OPEN rep + "dump.txt" FOR BINARY AS #ff
        GET$ #ff, LOF(#ff), e
    CLOSE

    KILL rep + "dump.txt"
    FUNCTION = RTRIM$(e, ANY $CRLF)
END FUNCTION
'--------------------------------------------------------------------------------
