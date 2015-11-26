#COMPILER PBWIN 10
#COMPILE EXE = "rfo-basic-quick-apk.exe"
#DIM ALL

$EXE = "RFO-BASIC! Quick APK"
$VER = "v01.88.00"
$WEB = "http://mougino.free.fr/RFO/quick-apk/"

%PERM_NB     = 20
%RFO_REV     = 3  ' Revision number of the latest QuickAPK project format (.rfo)

$LNGLIST = "ENFRDENLPTRO"
#RESOURCE RCDATA LNG_EN,    "lng\EN.xml"
#RESOURCE RCDATA LNG_FR,    "lng\FR.xml"
#RESOURCE RCDATA LNG_DE,    "lng\DE.xml"
#RESOURCE RCDATA LNG_NL,    "lng\NL.xml"
#RESOURCE RCDATA LNG_PT,    "lng\PT.xml"
#RESOURCE RCDATA LNG_RO,    "lng\RO.xml"

'------------------------------------------------------------------------------
'   ** Types **
'------------------------------------------------------------------------------
' 1) CURRENT (%RFO_REV = 3)
'------------------------------------------------------------------------------
TYPE BASICProject_V3
    name                 AS ASCIIZ * %MAX_PATH
    path                 AS ASCIIZ * %MAX_PATH
    package              AS ASCIIZ * %MAX_PATH
    bas                  AS ASCIIZ * %MAX_PATH
    icon                 AS ASCIIZ * %MAX_PATH
    keystore             AS ASCIIZ * %MAX_PATH
    KSusr                AS ASCIIZ * %MAX_PATH
    KSpwd                AS ASCIIZ * %MAX_PATH
    startupmsg           AS ASCIIZ * %MAX_PATH
    splashimg            AS ASCIIZ * %MAX_PATH
    version              AS ASCIIZ * 11
    vcode                AS ASCIIZ * 11
    loadchr              AS ASCIIZ * 11
    splashbgndcolor      AS ASCIIZ * 7
    advanced             AS LONG
    createdatadir        AS LONG
    createdatabasedir    AS LONG
    startatboot          AS LONG
    hardwareaccel        AS LONG
    splashdisplay        AS LONG
    splashprogress       AS LONG
    permission(%PERM_NB) AS LONG
    encryptbas           AS LONG
    splashtimer          AS LONG
END TYPE

TYPE ConsoleDescriptor_V1
    title                AS ASCIIZ * %MAX_PATH
    input                AS ASCIIZ * %MAX_PATH
    fontcolor            AS ASCIIZ * 11
    backcolor            AS ASCIIZ * 11
    clearcolor           AS ASCIIZ * 11
    fontsize             AS LONG
    fonttype             AS LONG
    screenorientation    AS LONG
    uselines             AS LONG
END TYPE
'------------------------------------------------------------------------------
' 2) LEGACY
'------------------------------------------------------------------------------
TYPE BASICProject_V2
    name                 AS ASCIIZ * %MAX_PATH
    path                 AS ASCIIZ * %MAX_PATH
    package              AS ASCIIZ * %MAX_PATH
    bas                  AS ASCIIZ * %MAX_PATH
    icon                 AS ASCIIZ * %MAX_PATH
    keystore             AS ASCIIZ * %MAX_PATH
    KSusr                AS ASCIIZ * %MAX_PATH
    KSpwd                AS ASCIIZ * %MAX_PATH
    startupmsg           AS ASCIIZ * %MAX_PATH
    splashimg            AS ASCIIZ * %MAX_PATH
    version              AS ASCIIZ * 11
    vcode                AS ASCIIZ * 11
    loadchr              AS ASCIIZ * 11
    splashbgndcolor      AS ASCIIZ * 7
    advanced             AS LONG
    createdatadir        AS LONG
    createdatabasedir    AS LONG
    startatboot          AS LONG
    hardwareaccel        AS LONG
    splashdisplay        AS LONG
    splashprogress       AS LONG
    permission(%PERM_NB) AS LONG
    encryptbas           AS LONG
END TYPE

TYPE BASICProject_V1
    name                 AS ASCIIZ * %MAX_PATH
    path                 AS ASCIIZ * %MAX_PATH
    package              AS ASCIIZ * %MAX_PATH
    bas                  AS ASCIIZ * %MAX_PATH
    icon                 AS ASCIIZ * %MAX_PATH
    keystore             AS ASCIIZ * %MAX_PATH
    KSusr                AS ASCIIZ * %MAX_PATH
    KSpwd                AS ASCIIZ * %MAX_PATH
    startupmsg           AS ASCIIZ * %MAX_PATH
    splashimg            AS ASCIIZ * %MAX_PATH
    version              AS ASCIIZ * 11
    vcode                AS ASCIIZ * 11
    loadchr              AS ASCIIZ * 11
    splashbgndcolor      AS ASCIIZ * 7
    advanced             AS LONG
    createdatadir        AS LONG
    createdatabasedir    AS LONG
    startatboot          AS LONG
    hardwareaccel        AS LONG
    splashdisplay        AS LONG
    splashprogress       AS LONG
    permission(%PERM_NB) AS LONG
END TYPE
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
'   ** Globals **
'------------------------------------------------------------------------------
GLOBAL APPDATA      AS STRING   ' PROGRAM GLOBALS
GLOBAL APKfolder    AS STRING
GLOBAL Permission() AS STRING
GLOBAL SCREEN, ID   AS LONG
GLOBAL linkable     AS STRING
GLOBAL LNG          AS STRING*2
GLOBAL DEVLNG       AS STRING*2
GLOBAL hLanguage    AS LONG
GLOBAL hProject     AS LONG
GLOBAL all_res()    AS STRING
GLOBAL device_res() AS STRING
GLOBAL local_res()  AS STRING
GLOBAL res4app()    AS LONG
GLOBAL app2SD()     AS LONG
GLOBAL hTab()       AS DWORD
GLOBAL AfterAbout   AS LONG
GLOBAL aax, aay     AS LONG
GLOBAL easyapk_err  AS STRING
GLOBAL easyapk_dir  AS STRING
'------------------------------------------------------------------------------
GLOBAL project()    AS STRING
GLOBAL projNam()    AS STRING
GLOBAL projLoc()    AS LONG
GLOBAL ProjShortcut AS BYTE
'------------------------------------------------------------------------------
GLOBAL device       AS STRING   ' PROJECT-RELATED GLOBALS
GLOBAL sdpath       AS STRING
GLOBAL FMWK         AS STRING
GLOBAL app          AS BASICProject_V3
GLOBAL console      AS ConsoleDescriptor_V1
GLOBAL app_res()    AS STRING
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO INIFILE  = APPDATA + EXE.NAME$ + ".ini"
MACRO UPDATER  = APPDATA + EXE.NAME$ + "-updater.exe"
MACRO LOCALBAS = APKfolder + "rfo-basic\source\" + LinuxName(app.bas)
MACRO MYSERVER = $WEB + IIF$(LCASE$(COMMAND$) = "-testupdate", "test/", "")
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
'   ** Includes **
'------------------------------------------------------------------------------
#INCLUDE ONCE "inc\Windows.inc"
#INCLUDE ONCE "inc\XML.inc"
#INCLUDE ONCE "inc\RTF.inc"
#INCLUDE ONCE "inc\MyMsgBox.inc"
#INCLUDE ONCE "inc\GDIPlus.inc"
#INCLUDE ONCE "inc\RecursiveDir.inc"
#INCLUDE ONCE "inc\UW.inc"
#INCLUDE ONCE "inc\utils.inc"
#INCLUDE ONCE "inc\rfo-ui.inc"
#INCLUDE ONCE "inc\rfo-advanced.inc"
#INCLUDE ONCE "inc\android-comm.inc"
#INCLUDE ONCE "inc\dragndrop.inc"
#INCLUDE ONCE "inc\MD5.inc"
#INCLUDE ONCE "inc\savepos.inc"
#INCLUDE ONCE "inc\ColorTabs.inc"
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
'   ** Main Application Entry Point **
'------------------------------------------------------------------------------
FUNCTION PBMAIN () AS LONG

    ' Initialize GDI+ graphical library and RichEdit library
    GdipInitialize()
    LoadLibrary("RICHED32.DLL")

    ' Initialize font of message boxes
    SetMyMsgBoxFont "Lucida Console", 10, %BLACK, %WHITE

    ' Set Local App Data and Temp folders
    APPDATA = CreateLocalAppdataFolder() + "\"
    MakeSureDirectoryPathExists APPDATA + "system\framework\"
    ENVIRON "PATH=" + EXE.PATH$ + "tools" + ";" + ENVIRON$("PATH")

    ' Create language popup menu
    MENU NEW POPUP TO hLanguage
    LOCAL lg AS LONG
    INCR  lg : MENU ADD STRING, hLanguage, "English",      lg, %MF_ENABLED
    INCR  lg : MENU ADD STRING, hLanguage, "Français",     lg, %MF_ENABLED
    INCR  lg : MENU ADD STRING, hLanguage, "Deutsch",      lg, %MF_ENABLED
    INCR  lg : MENU ADD STRING, hLanguage, "Nederlands",   lg, %MF_ENABLED
    INCR  lg : MENU ADD STRING, hLanguage, "Português",    lg, %MF_ENABLED
    INCR  lg : MENU ADD STRING, hLanguage, "Romanian",     lg, %MF_ENABLED
    IF EXIST(EXE.PATH$ + "lng_test.xml") THEN
        LOCAL ff AS LONG
        ff = FREEFILE
        OPEN EXE.PATH$ + "lng_test.xml" FOR BINARY AS #ff
        GET$ #ff, LOF(#ff), LngXml
        LngXml = UTF8TOCHR$(LngXml)
        CLOSE #ff
        DEVLNG = InlineContent(LngXml, "code")
        INCR lg : MENU ADD STRING,  hLanguage, InlineContent(LngXml, "name") + " (dev)", lg, %MF_ENABLED
    END IF

    ' Create existing project popup menu
    LOCAL hProjsub AS DWORD
    MENU NEW POPUP TO hProjsub
    MENU ADD STRING,  hProjsub, "WiFi",    4, %MF_ENABLED
    MENU ADD STRING,  hProjsub, "USB",     5, %MF_ENABLED
    MENU NEW POPUP TO hProject
    MENU ADD STRING,  hProject, "build",   1, %MF_ENABLED
    MENU ADD STRING,  hProject, "modify",  2, %MF_ENABLED
    MENU ADD POPUP,   hProject, "install", hProjsub AS 3, %MF_ENABLED

    ' Get the preferences
    LoadConfig()
    ConnectMode = %LOCAL
    IF INSTR($LNGLIST+TRIM$(DEVLNG), LNG) = 0 THEN LNG = "EN"
    MENU SET STATE hLanguage, (INSTR($LNGLIST+TRIM$(DEVLNG), LNG)+1)\2, %MF_CHECKED
    IF INSTR($LNGLIST, LNG) <> 0 THEN LngXml = UTF8TOCHR$(RESOURCE$(RCDATA, "LNG_" + LNG)) ' A standard (not dev) language
    LBL_CANCEL   = XmlContent(LngXml, "button id=""cancel""")
    LBL_RETRY    = XmlContent(LngXml, "button id=""retry""")
    LBL_CONTINUE = XmlContent(LngXml, "button id=""continue""")
    LBL_YES      = XmlContent(LngXml, "button id=""yes""")
    LBL_NO       = XmlContent(LngXml, "button id=""no""")
    LBL_ABORT    = XmlContent(LngXml, "button id=""abort""")
    LBL_IGNORE   = XmlContent(LngXml, "button id=""ignore""")
    LBL_OK       = XmlContent(LngXml, "button id=""ok""")
    WritePrivateProfileString "Config", "rfoqa_folder", EXE.PATH$, INIFILE

    ' Create main dialog
    INIT_DIALOG (640, 400, $EXE + $SPC + $VER + "   by mougino 2015")

END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB INIT_PROGRESS_DL
    LOCAL w, h AS LONG
    GRAPHIC ATTACH hDlg, 1001, REDRAW
    GRAPHIC BOX (343, 183)-(510, 276), 10, %WHITE, %WHITE
    GRAPHIC SET FONT hFontLbl
    GRAPHIC COLOR RGB(7,99,164), %WHITE
    GRAPHIC TEXT SIZE GET_LABEL(4) TO w, h ' "Updating..."
    GRAPHIC SET POS (426 - w\2, 187)
    GRAPHIC PRINT GET_LABEL(4)
    GRAPHIC ELLIPSE (390, 203) - (461, 273), RGB(195,175,150), RGB(195,175,150)
    GRAPHIC ELLIPSE (402, 215) - (449, 262), RGB(195,175,150), %WHITE
    GRAPHIC REDRAW
END SUB
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB REFRESH_PROGRESS_DL(pct AS LONG)
    LOCAL w, h AS LONG
    LOCAL PI AS DOUBLE
    PI = 4 * ATN(1)
    GRAPHIC ATTACH hDlg, 1001, REDRAW
    GRAPHIC PIE (390, 203) - (461, 273), 2*PI * (1 - pct/100) + PI/2, PI/2, -1, -1
    GRAPHIC ELLIPSE (402, 215) - (449, 262), RGB(195,175,150), %WHITE
    GRAPHIC TEXT SIZE TRIM$(pct) + "%" TO w, h
    GRAPHIC SET POS (426 - w\2, 234)
    GRAPHIC PRINT TRIM$(pct) + "%"
    GRAPHIC REDRAW
END SUB
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB RESET_APP
    ' Reset App resources
    ERASE app_res()
    ' Reset full App UDT
    RESET app
    ' Default App settings
    app.splashbgndcolor = "FFFFFF" + $NUL
    app.splashdisplay   = 1
    app.splashtimer     = 1500
    app.startupmsg      = "Standby for initial file loading." + $NUL
    app.loadchr         = "." + $NUL
    AutosetPermissions("")
    ' Reset full Console UDT
    RESET console
    ' Default Console settings
    console.fontcolor   = "0xff000000" + $NUL
    console.backcolor   = "0xffffffff" + $NUL
    console.clearcolor  = "0xff006478" + $NUL
    console.screenorientation = 1 ' Variable By Sensors
    console.fontsize    = 2 ' Medium
    console.fonttype    = 1 ' Monospace
    console.uselines    = 1
END SUB
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB INIT_PERMS
    DATA "00.WRITE_EXTERNAL_STORAGE"
    DATA "01.INTERNET"
    DATA "02.ACCESS_COARSE_LOCATION"
    DATA "03.ACCESS_MOCK_LOCATION"
    DATA "04.ACCESS_FINE_LOCATION"
    DATA "05.ACCESS_LOCATION_EXTRA_COMMANDS"
    DATA "06.VIBRATE"
    DATA "07.WAKE_LOCK"
    DATA "08.CAMERA"
    DATA "09.BLUETOOTH"
    DATA "10.BLUETOOTH_ADMIN"
    DATA "11.RECORD_AUDIO"
    DATA "12.READ_PHONE_STATE"
    DATA "13.READ_SMS"
    DATA "14.SEND_SMS"
    DATA "15.RECEIVE_SMS"
    DATA "16.CALL_PHONE"
    DATA "17.ACCESS_NETWORK_STATE"
    DATA "18.DUMP"
    DATA "19.ACCESS_WIFI_STATE"
    DATA "20.CHANGE_WIFI_STATE"
    IF DATACOUNT <> %PERM_NB+1 THEN
        MyMsgBox hDlg, "Fatal error in number of permissions!", $EXE + $SPC + $VER, %MB_ICONERROR
        DIALOG POST hDlg, %WM_END_DIALOG, 0, 0
    END IF
    DIM Permission(0 TO %PERM_NB) AS GLOBAL STRING
    DIM i AS LONG
    FOR i = 1 TO DATACOUNT
        Permission(i-1) = MID$(READ$(i), 4)
    NEXT
END SUB
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB ShowScreen0splash
    SCREEN = 0
    CLEAN_DIALOG

    ID = 1
    CONTROL ADD GRAPHIC, hDlg, 1001, "", 0, 0, 640, 400
    GRAPHIC RENDER BITMAP "SPLASH", (0, 0) - (640, 400)

    START (ThreadScreen0splash)
END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
THREAD FUNCTION ThreadScreen0splash(BYVAL arg AS DWORD) AS LONG
    LOCAL t0, t1, httpTime AS DOUBLE
    LOCAL e, LatestVer AS STRING
    LOCAL i, j, lRes AS LONG

    t0 = INT(1000 * TIMER) ' Start time

    ' Did we just upgrade?
    IF EXIST(UPDATER) THEN ' Current version IS the new version (just installed)
        KILL UPDATER       ' Remove updater from older installation
        KILL StrReplace(UPDATER, ".exe", ".nfo")
    END IF

    ' Check online if there is a newer version
    CHECKONLINE:
    ' Content of lastversion.dat = "(1)v0.0;(2)mandatory;(3)updtr_size;(4)updtr_MD5"
    httpTime = HttpGet(MYSERVER + "lastversion.dat", LatestVer)
    IF httpTime >= 0 THEN
        IF LEFT$(LatestVer, 1) <> "<" THEN ' Starting with "<" not expected -> probably a 404 Html file
            IF PARSE$(LatestVer, ";", 1) > $VER OR LCASE$(COMMAND$) = "-testupdate" THEN
                e = GET_LABEL(1) + $CR ' A program update is available
                IF PARSE$(LatestVer, ";", 2) = "1" THEN
                    e += GET_LABEL(2) ' It's a critical update: mandatory install
                ELSE
                    e += GET_LABEL(3) ' It's a minor update: do you wish to install it?
                END IF
                lRes = MyMsgBox(hDlg, e, $EXE + $SPC + $VER, %MB_ICONQUESTION OR %MB_YESNO)
                t0 = INT(1000 * TIMER) - 1000 ' Start time
                IF lRes = 2 AND PARSE$(LatestVer, ";", 2) <> "1" THEN EXIT IF ' User declined minor update
                IF lRes = 2 AND PARSE$(LatestVer, ";", 2)  = "1" THEN ' User refused mandatory update
                    DIALOG POST hDlg, %WM_END_DIALOG, 0, 0
                    EXIT_THREAD
                END IF
                DOWNLOADUPDATER:
                httpTime = HttpGet(MYSERVER + "updater.x", e, VAL(PARSE$(LatestVer, ";", 3)))
                IF httpTime < 0 THEN
                    lRes = MyMsgBox(hDlg, GET_LABEL(5), $EXE + $SPC + $VER, %MB_ICONERROR OR %MB_RETRYCANCEL) ' Failure!
                    IF lRes = 1 THEN GOTO DOWNLOADUPDATER ' Retry
                    IF lRes = 2 AND PARSE$(LatestVer, ";", 2) = "1" THEN ' Cancel on mandatory update -> quit program
                        DIALOG POST hDlg, %WM_END_DIALOG, 0, 0
                        EXIT_THREAD
                    END IF
                ELSE
                    LOCAL md5c AS MD5_CTX
                    md5_Init VARPTR(md5c)
                    md5_Update VARPTR(md5c), STRPTR(e), LEN(e)
                    IF md5_Digest(VARPTR(md5c)) = PARSE$(LatestVer, ";", 4) THEN
                        IF EXIST(UPDATER) THEN KILL UPDATER
                        i = FREEFILE ' create updater.nfo
                        OPEN StrReplace(UPDATER, ".exe", ".nfo") FOR OUTPUT AS #i
                        PRINT #i, EXE.PATH$
                        CLOSE #i
                        i = FREEFILE ' dump updater.exe
                        OPEN UPDATER FOR BINARY AS #i
                        PUT$ #i, e
                        CLOSE #i
                        ShellExecute NUL, "open", UPDATER + $NUL, COMMAND$ + $NUL, NUL, %SW_SHOWNORMAL
                        DIALOG POST hDlg, %WM_END_DIALOG, 0, 0
                        EXIT_THREAD
                    ELSE ' Bad download (wrong updater MD5)
                        lRes = MyMsgBox(hDlg, GET_LABEL(5), $EXE + $SPC + $VER, %MB_ICONERROR OR %MB_RETRYCANCEL) ' Failure!
                        IF lRes = 1 THEN GOTO DOWNLOADUPDATER ' Retry
                        IF lRes = 2 AND PARSE$(LatestVer, ";", 2) = "1" THEN ' Cancel on mandatory update -> quit program
                            DIALOG POST hDlg, %WM_END_DIALOG, 0, 0
                            EXIT_THREAD
                        END IF
                    END IF ' End of updater checksum (MD5 hash)
                END IF ' End of updater download
            END IF ' End of new version available
        END IF ' End of lastversion.dat is not a 404 Html page
    END IF ' End of successful download of lastversion.dat

    ' Check presence of Java Runtime Environment
    RUN_CMD "adb kill-server" : PAUSE 500 ' To release Java
    e = DUMP_CMD("java -version")
    IF INSTR(e, "Java(TM)") = 0 OR INSTR(e, "Runtime Environment") = 0 THEN
        e = "You need to have Java installed on your computer " _
          + "for " + $EXE + " to work." + $CR + $CR _
          + "Click OK to be redirected to the Java download web page."
        MyMsgBox hDlg, e, $EXE + $SPC + $VER, %MB_ICONWARNING
        ShellExecute NUL, "open", "https://www.java.com/download/" + $NUL, NUL, NUL, %SW_SHOWNORMAL
        DIALOG POST hDlg, %WM_END_DIALOG, 0, 0
        EXIT_THREAD
    ELSE
        i = INSTR(e, $DQ)
        IF i THEN
            INCR i
            j = INSTR(i, e, $DQ)
            IF j THEN
                e = MID$(e, i, j-i)
                IF e < "1.7" THEN
                    e = "We found a Java Runtime Environment " + e + " on your computer." + $CR + $CR _
                      + "You need Java 1.7 or above for " + $EXE + " to work." + $CR + $CR _
                      + "Click OK to upgrade your Java from the official download web page." + SPACE$(30)
                    MyMsgBox hDlg, e, $EXE + $SPC + $VER, %MB_ICONWARNING
                    ShellExecute NUL, "open", "https://www.java.com/download/" + $NUL, NUL, NUL, %SW_SHOWNORMAL
                    DIALOG POST hDlg, %WM_END_DIALOG, 0, 0
                    EXIT_THREAD
                END IF
            END IF
        END IF
    END IF

    ' Check needed tools
    DATA "tools\adb.exe"
    DATA "tools\AdbWinApi.dll"
    DATA "tools\AdbWinUsbApi.dll"
    DATA "tools\apktool.bat"
    DATA "tools\apktool.jar"
    DATA "tools\aapt.exe"
    DATA "tools\Basic.apk"
    DATA "tools\signapk.bat"
    DATA "tools\signapk.jar"
    DATA "tools\cert.x509.pem"
    DATA "tools\key.pk8"
    DATA "tools\zipalign.exe"
    DATA "tools\keytool.exe"
    DATA "tools\jli.dll"
    DATA "tools\openssl.exe"
    DATA "tools\ssleay32.dll"
    DATA "tools\libeay32.dll"
    DATA "tools\splitpem.exe"
    DATA "tools\easyapk.exe"
    DATA "tools\convert.exe"
    DATA "tools\basic.js"
    DATA "tools\jquery-2.1.1.min.js"
    DATA "tools\jquery.mobile-1.4.5.min.css"
    DATA "tools\jquery.mobile-1.4.5.min.js"
    DATA "tools\styles.css"
    e = ""
    FOR i = 1 TO DATACOUNT
        IF NOT EXIST(EXE.PATH$ + READ$(i)) THEN e += "- " + READ$(i) + $CR
    NEXT
    IF LEN(e)>0 THEN
        e = "Some of the tools needed by " + $EXE + " are missing!" + $CR + $CR _
          + "Please re-install " + $EXE + " from http://mougino.free.fr " _
          + "and make sure of the existence of the following tools in " _
          + "the installation folder:" + $CR + e + SPACE$(30)
        MyMsgBox hDlg, e, $EXE + $SPC + $VER, %MB_ICONERROR
        DIALOG POST hDlg, %WM_END_DIALOG, 0, 0
        EXIT_THREAD
    END IF

    ' Initilize permissions
    INIT_PERMS()

    t1 = INT(1000 * TIMER) ' End time
    IF t1 - t0 > 0 AND t1 - t0 < 2000 THEN PAUSE 2000 - (t1 - t0)

    DIALOG POST hDlg, %WM_INIT_DIALOG, 0, 0
    EXIT_THREAD

END FUNCTION
'--------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB ShowScreen1welcome
    SCREEN = 1
    CLEAN_DIALOG

    ADD_TITLE (8, 28, 580, 2*%LINEHEIGHT, "Welcome to " + $EXE) 'CTL ID #1001
    SET_LABEL (CUR_LABEL + $SPC + $EXE)

    ADD_LABEL (8, 80, 624, 2*%LINEHEIGHT, "This is your first time here, please chose " _
        + "a target folder where your APKs will be created:") 'CTL ID #1002
    SET_LABEL (CUR_LABEL)

    ADD_BUTTON (8, 136, 120, 24, "Select") 'CTL ID #1003
    SET_LABEL (CUR_LABEL)

    ADD_LABEL (148, 141, 624, %LINEHEIGHT, "<" + "no folder selected" + ">") 'CTL ID #1004
    SET_LABEL ("<" + CUR_LABEL + ">")

    ADD_LINK (8, 370, 320, %LINEHEIGHT, "Donate via PayPal at www.rfo-basic.com") 'CTL ID #1005

    ADD_BUTTON (532, 360, 100, 32, "Next >") 'CTL ID #1006
    SET_LABEL (CUR_LABEL)
    CONTROL DISABLE hDlg, 1000+ID

END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB ProcScreen1welcome(CTL AS LONG, MSG AS LONG)
    LOCAL e AS STRING
    SELECT CASE CTL

        CASE 1003 ' "Select" button
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                DISPLAY SAVEFILE hDlg, 40, 0, GET_LABEL(11), _
                    "", "", GET_LABEL(12), "", %OFN_PATHMUSTEXIST TO e
                e = TRIM$(e)
                IF e <> "" THEN
                    APKfolder = LEFT$(e, INSTR(-1, e, "\"))
                    CONTROL KILL hDlg, 1004
                    CONTROL ADD LABEL, hDlg, 1004, LEFT$(APKfolder, -1), _
                        148, 141, 484, %LINEHEIGHT, %SS_NOTIFY OR %SS_PATHELLIPSIS
                    CONTROL SET COLOR  hDlg, 1004, %BLUE, %WHITE
                    CONTROL SET FONT   hDlg, 1004, hFontLnk
                    IF INSTR(linkable, "<1004>") = 0 THEN linkable += "<1004>"
                    CONTROL ENABLE hDlg, 1006 ' "Next" button
                END IF
            END IF

        CASE 1004 ' APKfolder
            IF MSG = %STN_CLICKED THEN ShellExecute NUL, "open", "explorer.exe" + $NUL, _
                APKfolder + $NUL, NUL, %SW_SHOWNORMAL

        CASE 1005 ' http://www.rfo-basic.com
            IF MSG = %STN_CLICKED THEN ShellExecute NUL, "open", _
                "http://www.rfo-basic.com" + $NUL, NUL, NUL, %SW_SHOWNORMAL

        CASE 1006 ' "Next" button
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                WritePrivateProfileString "Config", "apk_folder", TRIM$(APKfolder), INIFILE
                ShowScreen2projects
            END IF

        END SELECT
END SUB
'--------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB ShowScreen2projects
    LOCAL hDib, ti() AS DWORD
    LOCAL j, n, w, h AS LONG
    LOCAL e, e2 AS STRING
    LOCAL dt AS DIRDATA

    SCREEN = 2
    CLEAN_DIALOG

    ' Define labels of project popup menu
    ProjShortcut = 0
    MENU SET TEXT hProject, 1, GET_LABEL(11)
    MENU SET TEXT hProject, 2, GET_LABEL(12)
    MENU SET TEXT hProject, 3, GET_LABEL(13)

    ' Populate list of existing projects
    ERASE project()
    ERASE projNam()
    ERASE projLoc()
    ERASE ti()
    n = 1
    REDIM project(1 TO n) : DIM ti(1 TO n)
    project(n) = DIR$(APKfolder + "\*.rfo" TO dt)
    DO WHILE project(n) <> ""
         ti(n) = dt.LastWriteTime
         INCR n
         REDIM PRESERVE project(1 TO n) : REDIM PRESERVE ti(1 TO n)
         project(n) = DIR$(NEXT TO dt)
    LOOP
    DECR n
    IF n = 0 THEN
        ERASE project()
    ELSE
        REDIM PRESERVE project(1 TO n) : REDIM PRESERVE ti(1 TO n)
        ARRAY SORT ti(), TAGARRAY project(), ASCEND
        IF n > 17 THEN n = 17 : REDIM PRESERVE project(1 TO n)
        ERASE ti() ' Last Write Times not needed anymore
        REDIM PRESERVE projNam(1 TO n)
        REDIM PRESERVE projLoc(1 TO n)
    END IF

    ' Display "new project" then list of existing projects, fill the rest with empty projects
    FOR j = 1 TO 3
        FOR i = 1 TO 6
            n = 6 * (j - 1) + i - 1
            FillCurrentBox:
            IF n = 0 THEN ' New project
                ADD_IMG (10 + (i-1)*(72+35), 44 + (j-1)*(72+%LINEHEIGHT+25), 84, 84, "NEWP") 'CTL ID #1001
                e = CUR_LABEL : n = INSTR(e, $SPC) : e2 = MID$(e, n+1) : e = LEFT$(e, n-1)
                GRAPHIC ATTACH hDlg, 1000+ID, REDRAW
                GRAPHIC SET FONT hFontTtl : GRAPHIC COLOR %BLACK, -2
                GRAPHIC TEXT SIZE e  TO w, h : GRAPHIC SET POS (42-w\2, 21)   : GRAPHIC PRINT e  ' New
                GRAPHIC TEXT SIZE e2 TO w, h : GRAPHIC SET POS (42-w\2, 63-h) : GRAPHIC PRINT e2 ' project
                GRAPHIC REDRAW
                linkable += "<" + TRIM$(1000+ID) + ">"
            ELSEIF n >= 1 AND n <= UBOUND(project) THEN ' Existing project
                IF ISFALSE LoadRFOproject (APKfolder + project(n)) THEN ' Incorrect project
                    ARRAY DELETE project(n)
                    IF UBOUND(project) > 1 THEN REDIM PRESERVE project(1 TO UBOUND(project) - 1) ELSE ERASE project()
                    GOTO FillCurrentBox
                ELSE                      ' Correct project
                    projNam(n) = app.name
                    ADD_IMG (16 + (i-1)*(72+35), 50 + (j-1)*(72+%LINEHEIGHT+25), 72, 73+%LINEHEIGHT, "") 'CTL ID #1002 TO #1002+UBOUND(project)-1
                    GRAPHIC ATTACH hDlg, 1000+ID, REDRAW : GRAPHIC CLEAR %WHITE
                    GRAPHIC BOX (0, 0) - (72, 72), 30, RGB(200,200,200), %WHITE
                    IF app.icon = "" THEN
                        GRAPHIC RENDER BITMAP "ICO72", (0, 0) - (71, 71) ' Default icon
                    ELSE
                        hDib = 0
                        IF EXIST(app.icon) THEN GdipLoadImageFromFile UCODE$(app.icon), hDib ' Custom icon
                        IF hDib = 0 THEN
                            GRAPHIC RENDER BITMAP "ICO72", (0, 0) - (71, 71) ' Bad custom icon > back to default icon
                        ELSE
                            GdipDrawImageRect hGdip(), hDib, 0, 0, 72, 72
                            GdipDisposeImage hDib
                        END IF
                    END IF
                    GRAPHIC SET FONT hFontLbl : GRAPHIC COLOR %BLACK, %WHITE
                    w = GRAPHIC(TEXT.SIZE.X, projNam(n))
                    IF w <= 72 THEN
                        GRAPHIC SET POS ((72-w)\2, 73)
                        projLoc(n) = 32767
                    ELSE
                        GRAPHIC SET POS (0, 73)
                        projLoc(n) = 0
                    END IF
                    GRAPHIC PRINT projNam(n) ' Project name
                    GRAPHIC REDRAW
                    linkable += "<" + TRIM$(1000+ID) + ">"
                END IF
            ELSE ' Empty
                ADD_IMG (16 + (i-1)*(72+35), 50 + (j-1)*(72+%LINEHEIGHT+25), 72, 72+%LINEHEIGHT, "") 'CTL ID #1002+UBOUND(project) TO #1018
                e = GET_LABEL(2) ' "Empty"
                GRAPHIC ATTACH hDlg, 1000+ID, REDRAW : GRAPHIC CLEAR %WHITE
                GRAPHIC BOX (0, 0) - (72, 72), 30, RGB(200,200,200), %WHITE
                GRAPHIC BOX (1, 1) - (71, 71), 30, %WHITE, RGB(220,220,220), 4
                GRAPHIC SET FONT hFontLbl : GRAPHIC COLOR RGB(220,220,220), %WHITE
                GRAPHIC TEXT SIZE e  TO w, h : GRAPHIC SET POS (36-w\2, 73) : GRAPHIC PRINT e
                GRAPHIC REDRAW
            END IF
        NEXT
    NEXT
    RESET_APP

    ' Existing or empty project (= all but new project) : add a delete red cross
    FOR j = 1 TO 3
        FOR i = 1 TO 6
            n = 6 * (j - 1) + i - 1
            IF n >= 1 THEN
                ADD_IMG (16 + (i-1)*(72+35) + 72, 50 + (j-1)*(72+%LINEHEIGHT+25) - 4, 13, 13, "DEL") 'CTL ID #1019 TO #1035
                IF n <= UBOUND(project) THEN linkable += "<" + TRIM$(1000+ID) + ">" ELSE CONTROL SHOW STATE hDlg, 1000+ID, %SW_HIDE ' Hide it for empty projects
            END IF
        NEXT
    NEXT

    ADD_LABEL (8, 10, 590, %LINEHEIGHT, "Home directory:") 'CTL ID #1036
    SET_LABEL (GET_LABEL(6))

    ADD_LINK (8, 10, 528, %LINEHEIGHT, RTRIM$(APKfolder, "\")) 'CTL ID #1037

    ADD_BUTTON (552, 7, 80, %LINEHEIGHT+4, "Change") 'CTL ID #1038
    SET_LABEL (GET_LABEL(7))

    SubScreen2refreshApkfolder()

    ADD_IMG (407, 378, 14, 14, "FONTDECR") ' CTL ID #1039
    linkable += "<1039>"

    ADD_IMG (425, 372, 20, 20, "FONTINCR") ' CTL ID #1040
    linkable += "<1040>"

    ADD_LINK (8, 375, 320, %LINEHEIGHT, "Donate via PayPal at www.rfo-basic.com") 'CTL ID #1000+ID-1

    ADD_BUTTON (532, 360, 100, 32, "About") 'CTL ID #1000+ID
    SET_LABEL (GET_LABEL(8))

END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB SubScreen2refreshApkfolder
    LOCAL w, h AS LONG
    GRAPHIC ATTACH hDlg, 1019, REDRAW ' or any other graphic will do
    GRAPHIC SET FONT hFontLbl
    w = GRAPHIC(TEXT.SIZE.X, GET_LABEL(6)) + 6
    CONTROL SET SIZE hDlg, 1036, w, %LINEHEIGHT ' "Home directory:"
    CONTROL REDRAW   hDlg, 1036
    w += 15
    CONTROL SET LOC  hDlg, 1037, w, 10 ' APKfolder link
    h = GRAPHIC(TEXT.SIZE.X, APKfolder)
    IF h > 500-w THEN h = 500-w
    CONTROL SET SIZE hDlg, 1037, h, %LINEHEIGHT ' APKfolder link
    CONTROL SET LOC  hDlg, 1038, w+h+13, 7 ' Change button
END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
FUNCTION FnScreen2CheckHomeDirWriting AS LONG
' Check rights to write in homedir: returns %TRUE if app has the rights, %FALSE otherwhise
    LOCAL ff AS LONG
    LOCAL e AS STRING
    KILL APKfolder + "dummy.test"
    ff = FREEFILE : OPEN APKfolder + "dummy.test" FOR OUTPUT AS #ff : PRINT #ff, "null" : CLOSE #ff
    IF EXIST(APKfolder + "dummy.test") THEN
        KILL APKfolder + "dummy.test"
        FUNCTION = %TRUE
    ELSE
        e = "You don't have the rights to write in" + $CR _
          + "your home directory:" + $CR _
          + APKfolder + $CR + $CR _
          + "Either change your home directory, or" + $CR _
          + "right-click Quick APK > Run as Admin."
        MyMsgBox hDlg, e, $EXE + $SPC + $VER, %MB_ICONWARNING
        FUNCTION = %FALSE
    END IF
END FUNCTION
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB ProcScreen2projects(CTL AS LONG, MSG AS LONG)
    LOCAL e AS STRING
    LOCAL w, cm AS LONG
    STATIC dir AS INTEGER
    STATIC curproj AS STRING

    SELECT CASE CTL

        CASE 1001 ' New project
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                IF FnScreen2CheckHomeDirWriting() THEN
                    RESET_APP
                    IF (ConnectMode  = %LOCAL AND UBOUND(local_res)  < 0) OR _
                       (ConnectMode <> %LOCAL AND UBOUND(device_res) < 0) THEN
                       ShowScreen3listres    ' First time getting the resource list for this connect mode
                    ELSE
                       ShowScreen4setbasres ' We already have the ressource list from a previous session
                    END IF
                END IF
            END IF

        CASE 1002 TO 1002 + UBOUND(project) - 1 ' Hover or click on existing project
            IF MSG = %ID_TIMER_SCROLL - 1 THEN ' Stop scrolling
                STOP_ANIM_SCROLLING
            ELSEIF MSG = %ID_TIMER_SCROLL THEN ' Scroll !
                ANIMATE_SCROLLING
            ELSEIF MSG = %BN_CLICKED OR MSG = 1 OR MSG = 2 THEN ' Click proj -> popup menu "build|modify|install"
                cm = ConnectMode ' backup old ConnectMode
                curproj = APKfolder + project(CTL-1001)
                LoadRFOproject curproj
                MENU SET STATE hProject, 3, IIF(EXIST(APKfolder + app.name + ".apk"), %MF_ENABLED, %MF_GRAYED) ' "Install" option available only if APK exists
                ConnectMode = cm ' restore old ConnectMode
                LOCAL pt AS POINTAPI
                LOCAL hWnd AS LONG
                LOCAL lRes AS LONG
                GetCursorPos pt
                hWnd = WindowFromPoint(pt)
                lRes = TrackPopupMenuEx(hProject, %TPM_LEFTALIGN OR %TPM_RIGHTBUTTON OR %TPM_RETURNCMD, pt.X, pt.Y, hWnd, NUL)
                IF lRes = 1 AND FnScreen2CheckHomeDirWriting() THEN
                    ProjShortcut = 1 ' "build" shortcut
                    ShowScreen8makeapk
                ELSEIF lRes = 2 AND FnScreen2CheckHomeDirWriting() THEN
                    ProjShortcut = 0 ' "modify" (not a shortcut)
                    IF (ConnectMode  = %LOCAL AND UBOUND(local_res)  < 0) OR _
                       (ConnectMode <> %LOCAL AND UBOUND(device_res) < 0) THEN
                       ShowScreen3listres    ' First time getting the resource list for this connect mode
                    ELSE
                       ShowScreen4setbasres ' We already have the ressource list from a previous session
                    END IF
                ELSEIF lRes = 4 THEN
                    ProjShortcut = 2 ' "WiFi install" shortcut
                    ConnectMode = %WIFI
                    ShowScreen8makeapk
                ELSEIF lRes = 5 THEN
                    ProjShortcut = 3 ' "USB install" shortcut
                    ConnectMode = %USB
                    ShowScreen8makeapk
                END IF
            END IF

        CASE 1019 TO 1035 ' Delete existing project
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                IF NOT FnScreen2CheckHomeDirWriting() THEN EXIT SUB
                IF MyMsgBox(hDlg, GET_LABEL(14), projNam(CTL-1018), %MB_ICONWARNING OR %MB_YESNO) = 1 THEN
                    KILL APKfolder + project(CTL-1018)
                    ShowScreen2projects
                END IF
            END IF

        CASE 1037 ' APKfolder
            IF MSG = %STN_CLICKED THEN ShellExecute NUL, "open", "explorer.exe" + $NUL, _
                APKfolder + $NUL, NUL, %SW_SHOWNORMAL

        CASE 1038 ' "Change" button
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                DISPLAY SAVEFILE hDlg, 40, 0, GET_LABEL(9), _           ' "Quick APK Home directory"
                    "", "", GET_LABEL(10), "", %OFN_PATHMUSTEXIST TO e  ' "choose this folder"
                e = TRIM$(e)
                IF e <> "" THEN
                    APKfolder = LEFT$(e, INSTR(-1, e, "\"))
                    WritePrivateProfileString "Config", "apk_folder", TRIM$(APKfolder), INIFILE
                    RESET_APP
                    ERASE local_res()   ' Reset list of local resources
                    ShowScreen2projects ' Force a new scan of projects because APKfolder changed
                END IF
            END IF

        CASE 1039 ' Decrease font size
            DECR font_size
            IF font_size < -5 THEN
                font_size = -5
            ELSE
                CHANGE_FONT
            END IF

        CASE 1040 ' Increase font size
            INCR font_size
            IF font_size > 5 THEN
                font_size = 5
            ELSE
                CHANGE_FONT
            END IF

        CASE 1000+ID-1 ' http://www.rfo-basic.com
            IF MSG = %STN_CLICKED THEN ShellExecute NUL, "open", _
                "http://www.rfo-basic.com" + $NUL, NUL, NUL, %SW_SHOWNORMAL

        CASE 1000+ID ' "About" button
            IF MSG = %BN_CLICKED OR MSG = 1 THEN ShowScreen9about

        END SELECT
END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB ShowScreen3listres
    LOCAL e AS STRING

    SCREEN = 3
    CLEAN_DIALOG

    ADD_ANIM 'CTL ID #1001

    ADD_LABEL (8, 14*%LINEHEIGHT, 624, 2*%LINEHEIGHT, "Analyzing local FS /OR/ Detecting plugged device /OR/ Found a XX, analyzing...") 'CTL ID #1002
    IF ConnectMode = %LOCAL THEN
        SET_LABEL (GET_LABEL(1))
    ELSE
        SET_LABEL (CUR_LABEL)
    END IF

    ADD_LABEL (8, 14*%LINEHEIGHT, 624, 2*%LINEHEIGHT, "No device found") 'CTL ID #1003
    SET_LABEL (CUR_LABEL)
    CONTROL SHOW STATE hDlg, 1000+ID, %SW_HIDE

    ADD_LABEL (8, 14*%LINEHEIGHT, 624, %LINEHEIGHT, "BASIC! not installed") 'CTL ID #1004
    SET_LABEL (CUR_LABEL)
    CONTROL SHOW STATE hDlg, 1000+ID, %SW_HIDE

    ADD_BUTTON (8, 360, 100, 32, "Previous") 'CTL ID #1005
    SET_LABEL (CUR_LABEL)

    ADD_BUTTON (270, 360, 100, 32, "Retry") 'CTL ID #1006
    SET_LABEL (CUR_LABEL)
    CONTROL DISABLE hDlg, 1000+ID

    ADD_BUTTON (532, 360, 100, 32, "Next") 'CTL ID #1007
    SET_LABEL (CUR_LABEL)
    CONTROL DISABLE hDlg, 1000+ID

    LOCAL dropdown() AS STRING
    DIM dropdown(0 TO 2)
    ARRAY ASSIGN dropdown() = $SPC + GET_LABEL(10), $SPC + GET_LABEL(11), $SPC + GET_LABEL(12)
    ADD_DROPDOWN (18, 1, 434, 20*4, dropdown, 1) 'CTL ID #1008
    COMBOBOX SELECT hDlg, 1000+ID, ConnectMode ' %LOCAL = 1 ; %WIFI = 2 ; %USB = 3
    CONTROL DISABLE hDlg, 1000+ID

    ADD_LINK (460, 5, 100, %LINEHEIGHT, "read me") 'CTL ID #1009
    SET_LABEL (GET_LABEL(13))
    CONTROL DISABLE hDlg, 1000+ID

    START (ThreadScreen3listres)

END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
FUNCTION FnScreen3SolveMixedCaseFolder(e AS STRING) AS STRING
    ' Solve mixed case local folders e.g. "RFO-Basic" or "Source" instead of "rfo-basic" and "source"
    LOCAL r AS STRING
    r = e
    IF INSTR(r, "rfo-basic/source/") = 0 AND INSTR(LCASE$(r), "rfo-basic/source/") = 1 THEN
        r = "rfo-basic/source/" + MID$(r, LEN("rfo-basic/source/") + 1)
    END IF
    IF INSTR(r, "rfo-basic/databases/") = 0 AND INSTR(LCASE$(r), "rfo-basic/databases/") = 1 THEN
        r = "rfo-basic/databases/" + MID$(r, LEN("rfo-basic/databases/") + 1)
    END IF
    IF INSTR(r, "rfo-basic/data/") = 0 AND INSTR(LCASE$(r), "rfo-basic/data/") = 1 THEN
        r = "rfo-basic/data/" + MID$(r, LEN("rfo-basic/data/") + 1)
    END IF
    IF INSTR(r, "rfo-basic/") = 0 AND INSTR(LCASE$(r), "rfo-basic/") = 1 THEN
        r = "rfo-basic/" + MID$(r, LEN("rfo-basic/") + 1)
    END IF
    FUNCTION = r
END FUNCTION
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
THREAD FUNCTION ThreadScreen3listres(BYVAL arg AS DWORD) AS LONG
    RDIR_DeclareLocals  ' Local variables for recursive dir macros
    LOCAL i, k AS LONG
    LOCAL e AS STRING

    IF ConnectMode = %LOCAL AND UBOUND(local_res) < 0 THEN
        DIALOG POST hDlg, %WM_START_ANIM_THINK, 0, 0

        RDIR_ListSubfoldersIn(APKfolder) ' List all local subfolders
        REDIM local_res(1 TO RDIR_NbOfSubfolders + 2)
        FOR k = 1 TO RDIR_NbOfSubfolders
            e = REMOVE$(RDIR_Subfolder(k), APKfolder) + "\"
            REPLACE "\" WITH "/" IN e
            local_res(k+1) = FnScreen3SolveMixedCaseFolder(e)
        NEXT

        RDIR_ListFilesIn(APKfolder, "*.*") ' List all local files
        FOR k = 0 TO RDIR_NbOfFiles
            IF RIGHT$(LCASE$(RDIR_File(k)), 4) <> ".rfo" _
            AND RIGHT$(LCASE$(RDIR_File(k)), 4) <> ".xml" _
            AND RIGHT$(LCASE$(RDIR_File(k)), 4) <> ".apk" THEN
                e = REMOVE$(RDIR_File(k), APKfolder)
                REPLACE "\" WITH "/" IN e
                local_res(UBOUND(local_res)) = FnScreen3SolveMixedCaseFolder(e)
                REDIM PRESERVE local_res(1 TO UBOUND(local_res) + 1)
            END IF
        NEXT
        REDIM PRESERVE local_res(1 TO UBOUND(local_res) - 1)
        DIALOG POST hDlg, %WM_GOT_RES, 0, 0 ' Correctly listed the resources -> proceed to screen 4 setbasres

    ELSEIF ConnectMode <> %LOCAL AND UBOUND(device_res) < 0 THEN
        DIALOG POST hDlg, %WM_START_ANIM_LOG, 0, 0

        device = GetConnectedAndroidModelName() ' Try to connect to device
        IF device = "" THEN ' No device connected
            DIALOG POST hDlg, %WM_NO_DEVICE_CONNECTED, 0, 0
            EXIT_THREAD
        END IF

        SENDMESSAGE (hDlg, %WM_WAIT_END_ANIM_LOG, 0, 0) ' Synchronous = wait for a response
        DIALOG POST hDlg, %WM_START_ANIM_TRANSFER, 0, 0

        RetrieveFileList()
        IF sdpath = "" THEN ' RFO-BASIC! not found on device
            DIALOG POST hDlg, %WM_RFO_NOT_FOUND, 0, 0
            EXIT_THREAD
        END IF
        DIALOG POST hDlg, %WM_GOT_RES, 0, 0 ' Correctly retrieved the resource list -> proceed to screen 4 setbasres
    END IF

    EXIT_THREAD

END FUNCTION
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB ProcScreen3listres (CTL AS LONG, MSG AS LONG)
    LOCAL i, lRes, checked AS LONG

    SELECT CASE CTL

        CASE 1005 ' "Previous" button
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                KillThread hThreadMain
                IF ConnectMode = %LOCAL THEN
                    ERASE local_res() ' Reset list of local resources
                ELSE
                    ERASE device_res() ' Reset list of device resources
                END IF
                ShowScreen2projects
            END IF

        CASE 1006 ' "Retry" button
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                IF ConnectMode = %LOCAL THEN
                    ERASE local_res() ' Reset list of local resources
                ELSE
                    ERASE device_res() ' Reset list of device resources
                END IF
                ShowScreen3listres
            END IF

        CASE 1008 ' Connect mode dropdown list
            IF MSG = %CBN_SELENDOK THEN
                COMBOBOX GET SELECT hDlg, CTL TO i
                ConnectMode = i ' %LOCAL = 1 ; %WIFI = 2 ; %USB = 3
                IF (ConnectMode  = %LOCAL AND UBOUND(local_res)  < 0) OR _
                   (ConnectMode <> %LOCAL AND UBOUND(device_res) < 0) THEN
                   ShowScreen3listres    ' First time getting the resource list for this connect mode
                ELSE
                   ShowScreen4setbasres
                END IF
            END IF

        CASE 1009 ' Connect mode readme
            IF MSG = %STN_CLICKED THEN
                SHOW_POPUP
            END IF

        END SELECT

END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB ShowScreen4setbasres
    LOCAL e AS STRING

    SCREEN = 4
    CLEAN_DIALOG

    ADD_ANIM 'CTL ID #1001

    ADD_LABEL (8, 13*%LINEHEIGHT, 624, 2*%LINEHEIGHT, "Select program & resource") 'CTL ID #1002
    SET_LABEL (CUR_LABEL)

    ADD_LABEL (8, 13*%LINEHEIGHT, 624, 2*%LINEHEIGHT, "No .bas found in XXX\rfo-basic\source\") 'CTL ID #1003
    IF ConnectMode = %LOCAL THEN
        SET_LABEL (StrReplace(CUR_LABEL, "XXX", RTRIM$(APKfolder, "\")))
    ELSE
        SET_LABEL (StrReplace(StrReplace(CUR_LABEL, "XXX", RTRIM$(sdpath, "/")), "\", "/"))
    END IF
    CONTROL SHOW STATE hDlg, 1000+ID, %SW_HIDE

    ADD_LISTBOX (8, 15*%LINEHEIGHT, 250, 11.5*%LINEHEIGHT)  'CTL ID #1004
    CONTROL SHOW STATE hDlg, 1000+ID, %SW_HIDE

    ADD_TREEVIEW (274, 15*%LINEHEIGHT, 350, 10.25*%LINEHEIGHT)  'CTL ID #1005
    CONTROL SHOW STATE hDlg, 1000+ID, %SW_HIDE

    ADD_LABEL (274, 25.5*%LINEHEIGHT, 350, %LINEHEIGHT, "N folder(s) N file(s) selected") 'CTL ID #1006
    CONTROL SHOW STATE hDlg, 1000+ID, %SW_HIDE

    ADD_BUTTON (8, 360, 100, 32, "Previous") 'CTL ID #1007
    SET_LABEL (CUR_LABEL)

    ADD_BUTTON (270, 360, 100, 32, "Refresh") 'CTL ID #1008
    SET_LABEL (CUR_LABEL)

    ADD_BUTTON (532, 360, 100, 32, "Next") 'CTL ID #1009
    SET_LABEL (CUR_LABEL)
    CONTROL DISABLE hDlg, 1000+ID

    LOCAL dropdown() AS STRING
    DIM dropdown(0 TO 2)
    ARRAY ASSIGN dropdown() = $SPC + GET_OLABEL(3, 10), $SPC + GET_OLABEL(3, 11), $SPC + GET_OLABEL(3, 12)
    ADD_DROPDOWN (18, 1, 434, 20*4, dropdown, 1) 'CTL ID #1010
    COMBOBOX SELECT hDlg, 1000+ID, ConnectMode ' %LOCAL = 1 ; %WIFI = 2 ; %USB = 3

    ADD_LINK (460, 5, 100, %LINEHEIGHT, "read me") 'CTL ID #1011
    SET_LABEL (GET_OLABEL(3, 13))

    START (ThreadScreen4setbasres)

END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB SubScreen4AutoSetPermsEtc
    LOCAL i AS LONG

    IF setperms = 0 THEN ' New project, we come from RESET_APP not from LoadRFOproject, and autoset of permissions, createdatadir etc. was never done

        ' Auto-detect the use of GW lib (will embed + copy to SD-Card the GW default theme)
        AutodetectGwLib(LOCALBAS)

        ' Auto-create data and/or databases folders at startup according to content of program
        CheckIfCreateDataDir

        ' Auto-create data and/or databases folders at startup according to resources
        FOR i = 1 TO UBOUND(app_res)
            IF  LEFT$(app_res(i), 1) = "1" THEN ' Copy to SD at startup
                IF RIGHT$(LCASE$(app_res(i)), 3) = ".db" THEN app.createdatabasedir = %TRUE ELSE app.createdatadir = %TRUE
            END IF
        NEXT

        ' Auto-set permissions (will set 'setperms' to '1')
        AutosetPermissions(LOCALBAS)

    END IF

END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
THREAD FUNCTION ThreadScreen4setbasres(BYVAL arg AS DWORD) AS LONG
    LOCAL i, j AS LONG

    IF ConnectMode = %LOCAL THEN
        REDIM all_res(1 TO UBOUND(local_res))
        FOR i = 1 TO UBOUND(local_res)
            all_res(i) = local_res(i)
        NEXT
        REDIM res4app(1 TO UBOUND(local_res))
        REDIM app2SD (1 TO UBOUND(local_res))

    ELSEIF ConnectMode = %USB OR ConnectMode = %WIFI THEN
        REDIM all_res(1 TO UBOUND(device_res))
        FOR i = 1 TO UBOUND(device_res)
            all_res(i) = device_res(i)
        NEXT
        REDIM res4app(1 TO UBOUND(device_res))
        REDIM app2SD (1 TO UBOUND(device_res))
    END IF

    IF UBOUND(app_res) > 0 THEN ' Fill res4app() and app2SD() from a previous project
        FOR i = 1 TO UBOUND(app_res)
            ARRAY SCAN all_res(), =MID$(app_res(i), 2), TO j
            IF j > 0 THEN
                res4app(j) = -1
                app2SD(j)  = VAL(LEFT$(app_res(i), 1))
            END IF
        NEXT
    END IF

    j = 0
    FOR i = 1 TO UBOUND(all_res) ' Check if there is at least 1 .bas to choose as main program
        IF INSTR(all_res(i), "rfo-basic/source/") = 1 _
            AND TALLY(all_res(i), "/") = 2 _
            AND RIGHT$(all_res(i), 1) <> "/" THEN
            INCR j
            EXIT FOR
        END IF
    NEXT
    IF j = 0 THEN
        DIALOG POST hDlg, %WM_NO_LOCAL_BAS, 0, 0
        EXIT_THREAD
    END IF

    DIALOG POST hDlg, %WM_DISPLAY_BAS_RES, 0, 0 ' All lists filled -> populate .bas listbox and resources treeview
    EXIT_THREAD

END FUNCTION
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB ProcScreen4setbasres(CTL AS LONG, MSG AS LONG)
    LOCAL i, lRes, checked AS LONG

    SELECT CASE CTL

        CASE 1004 ' main program listbox
            IF MSG = %LBN_SELCHANGE THEN
                LISTBOX GET SELECT hDlg, 1004 TO lRes
                LISTBOX GET USER hDlg, 1004, lRes TO i
                IF app.bas <> (all_res(i)) THEN
                    RESET_APP ' reset main program chosen by user
                END IF
                CONTROL ENABLE hDlg, 1009 ' Activate "Next" button
            ELSEIF MSG = %LBN_DBLCLK THEN
                LISTBOX GET SELECT hDlg, 1004 TO lRes
                LISTBOX GET USER hDlg, 1004, lRes TO i
                ShellExecute(%NULL, "open", BYCOPY APKfolder + all_res(i), "", "", %SW_SHOW)
            END IF

        CASE 1005 ' res TREEVIEW
            SELECT CASE MSG
                ' Clicking on an icon or a label checks/unchecks the checkbox
                CASE %TVN_SELCHANGEDMOUSE, %TVN_CHECKBOX
                    TREEVIEW GET SELECT hDlg, CTL TO lRes
                    IF lRes = 0 THEN EXIT SUB
                    TREEVIEW GET CHECK hDlg, CTL, lRes TO checked
                    IF MSG = %TVN_SELCHANGEDMOUSE THEN TREEVIEW SET CHECK hDlg, CTL, lRes, -1 - checked
                    TREEVIEW GET USER hDlg, CTL, lRes TO i
                    res4app(i) = -1 - checked
                    TREEVIEW_RECURSIVE_CHECK(hDlg, CTL, lRes, -1 - checked)
                    DIALOG POST hDlg, %WM_REFRESH_TREEVIEW_SEL, 0, 0
                ' Checking/unchecking a checkbox also checks/unchecks the checkboxes of the children elements
                CASE %TVN_CHECKBOX
                    TREEVIEW GET SELECT hDlg, CTL TO lRes
                    IF lRes = 0 THEN EXIT SUB
                    TREEVIEW GET CHECK hDlg, CTL, lRes TO checked
                    TREEVIEW SET CHECK hDlg, CTL, lRes, -1 - checked
                    TREEVIEW GET USER hDlg, CTL, lRes TO i
                    res4app(i) = checked
                    TREEVIEW_RECURSIVE_CHECK(hDlg, CTL, lRes, checked)
                    DIALOG POST hDlg, %WM_REFRESH_TREEVIEW_SEL, 0, 0
                END SELECT

        CASE 1007 ' "Previous" button
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                KillThread hThreadMain
                ShowScreen2projects
            END IF

        CASE 1008 ' "Refresh" button
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                IF ConnectMode = %LOCAL THEN
                    ERASE local_res() ' Reset list of local resources
                ELSE
                    ERASE device_res() ' Reset list of device resources
                END IF
                ShowScreen3listres
            END IF

        CASE 1009 ' "Next" button
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                ' Set main program .bas + default name and version if not defined
                LISTBOX GET SELECT hDlg, 1004 TO lRes
                LISTBOX GET USER hDlg, 1004, lRes TO i
                app.bas = all_res(i) + $NUL
                IF app.name = "" THEN app.name = REMOVE$(LinuxName(app.bas), ".bas") + $NUL
                IF app.version = "" THEN app.version = "0.1" + $NUL
                ' Create list of app resources
                ERASE app_res()
                FOR i = 1 TO UBOUND(all_res)
                    IF res4app(i) THEN
                        REDIM PRESERVE app_res(1 TO UBOUND(app_res) - LBOUND(app_res) + 2)
                        app_res(UBOUND(app_res)) = TRIM$(app2SD(i)) + all_res(i)
                    END IF
                NEXT
                IF ConnectMode = %LOCAL THEN
                    SubScreen4AutoSetPermsEtc
                    ShowScreen6appnamever
                ELSE
                    ShowScreen5getdeviceres
                END IF
            END IF

        CASE 1010 ' Connect mode dropdown list
            IF MSG = %CBN_SELENDOK THEN
                COMBOBOX GET SELECT hDlg, CTL TO i
                ConnectMode = i ' %LOCAL = 1 ; %WIFI = 2 ; %USB = 3
                IF (ConnectMode  = %LOCAL AND UBOUND(local_res)  < 0) OR _
                   (ConnectMode <> %LOCAL AND UBOUND(device_res) < 0) THEN
                   ShowScreen3listres    ' First time getting the resource list for this connect mode
                ELSE
                   ShowScreen4setbasres
                END IF
            END IF

        CASE 1011 ' Connect mode readme
            IF MSG = %STN_CLICKED THEN
                SHOW_POPUP
            END IF

        END SELECT

END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB ShowScreen5getdeviceres ' Screen 5 is only used in WiFi and USB modes
    LOCAL e AS STRING

    SCREEN = 5
    CLEAN_DIALOG

    ADD_ANIM 'CTL ID #1001

    ADD_LABEL (8, 13*%LINEHEIGHT, 624, 2*%LINEHEIGHT, "Detecting device /OR/ Copying files from device to...") 'CTL ID #1002
    IF device = "" THEN
        SET_LABEL (GET_LABEL(1)) ' "Detecting plugged device..."
    ELSE
        SET_LABEL (CUR_LABEL + $SPC + device + $SPC + GET_LABEL(12)) ' "Copying files from the " + device + " to the computer..."
    END IF

    ADD_LABEL (8, 13*%LINEHEIGHT, 624, 2*%LINEHEIGHT, "Error: the device has been unplugged!") 'CTL ID #1003
    SET_LABEL (CUR_LABEL)
    CONTROL SHOW STATE hDlg, 1000+ID, %SW_HIDE

    ADD_BUTTON (8, 360, 100, 32, "Previous") 'CTL ID #1004
    SET_LABEL (CUR_LABEL)

    ADD_BUTTON (270, 360, 100, 32, "Retry") 'CTL ID #1005
    SET_LABEL (CUR_LABEL)
    CONTROL DISABLE hDlg, 1000+ID

    LOCAL dropdown() AS STRING
    DIM dropdown(0 TO 2)
    ARRAY ASSIGN dropdown() = $SPC + GET_OLABEL(3, 10), $SPC + GET_OLABEL(3, 11), $SPC + GET_OLABEL(3, 12)
    ADD_DROPDOWN (18, 1, 434, 20*4, dropdown, 1) 'CTL ID #1006
    COMBOBOX SELECT hDlg, 1000+ID, ConnectMode ' %LOCAL = 1 ; %WIFI = 2 ; %USB = 3

    ADD_LINK (460, 5, 100, %LINEHEIGHT, "read me") 'CTL ID #1007
    SET_LABEL (GET_OLABEL(3, 13))

    START (ThreadScreen5getdeviceres)

END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
MACRO SubScreen5checkUnplugged ' Screen 5 is only used in WiFi and USB modes
    IF lRes = %FALSE THEN ' Error copying resource from device to computer
        DIALOG POST hDlg, %WM_TRANSFER_END, -1, 0 ' Status -1 = abort because device unplugged
        EXIT_THREAD
    END IF
END MACRO
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
THREAD FUNCTION ThreadScreen5getdeviceres(BYVAL arg AS DWORD) AS LONG ' Screen 5 is only used in WiFi and USB modes
    LOCAL i, lRes AS LONG
    LOCAL e AS STRING

    DIALOG POST hDlg, %WM_START_ANIM_TRANSFER, 0, 0

    IF device = "" THEN device = GetConnectedAndroidModelName() ' Try to connect to device
    IF device = "" THEN ' No device connected
        DIALOG POST hDlg, %WM_TRANSFER_END, -1, 0 ' Status -1 = abort because device unplugged
        EXIT_THREAD
    END IF

    IF sdpath = "" THEN RetrieveSDpath
    IF sdpath = "" THEN ' RFO-BASIC! not found on device
        DIALOG POST hDlg, %WM_TRANSFER_END, -1, 0 ' Status -1 = abort because device unplugged
        EXIT_THREAD
    END IF

    DIALOG POST hDlg, %WM_DEVICE_FOUND, 0, 0 ' Switch from "Detecting plugged device..." to "Copying files from the " + device + " to the computer..."

    ' Copy main program from device to computer's APKfolder\rfo-basic\source
    lRes = CopyAndroidFileTo ((app.bas), APKfolder)
    SubScreen5checkUnplugged

    ' Copy all selected resources from device to computer's APKfolder\rfo-basic\data (because <app.path> is not defined yet!)
    FOR i = 1 TO UBOUND(app_res)
        IF RIGHT$(app_res(i), 1) = "/" THEN ' Create asset subfolder
            e = MID$(app_res(i), 2)
            REPLACE "/" WITH "\" IN e
            e = APKfolder + e ' before: TEMP + "assets\" + e
            MakeSureDirectoryPathExists TRIM$(e)
        ELSE ' Copy file to assets
            lRes = CopyAndroidFileTo (MID$(app_res(i), 2), APKfolder)
            SubScreen5checkUnplugged
        END IF
    NEXT

    ' Finished copying files
    SubScreen4AutoSetPermsEtc
    DIALOG POST hDlg, %WM_TRANSFER_END, +1, 0 ' Status +1 = finished copying files, continue to Screen6appnamever
    EXIT_THREAD

END FUNCTION
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB ProcScreen5getdeviceres(CTL AS LONG, MSG AS LONG) ' Screen 5 is only used in WiFi and USB modes
    LOCAL i AS LONG

    SELECT CASE CTL

        CASE 1004 ' "Previous" button
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                KillThread hThreadMain
                ShowScreen4setbasres
            END IF

        CASE 1005 ' "Retry" button
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                ShowScreen5getdeviceres
            END IF

        CASE 1006 ' Connect mode dropdown list
            IF MSG = %CBN_SELENDOK THEN
                COMBOBOX GET SELECT hDlg, CTL TO i
                ConnectMode = i ' %LOCAL = 1 ; %WIFI = 2 ; %USB = 3
                IF ConnectMode  = %LOCAL THEN ShowScreen4setbasres ELSE ShowScreen5getdeviceres
            END IF

        CASE 1007 ' Connect mode readme
            IF MSG = %STN_CLICKED THEN
                SHOW_POPUP
            END IF

        END SELECT
END SUB
'--------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB ShowScreen6appnamever

    SCREEN = 6
    CLEAN_DIALOG

    ADD_TITLE (8, 18, 580, 2*%LINEHEIGHT, "Your app details") 'CTL ID #1001
    SET_LABEL (CUR_LABEL)

    ADD_LABEL (8, 6*%LINEHEIGHT, 100, %LINEHEIGHT, "Name") 'CTL ID #1002
    SET_LABEL (CUR_LABEL)

    ADD_INPUT (108, 6*%LINEHEIGHT-3, 400, 20, app.name) 'CTL ID #1003

    ADD_LABEL (8, 8*%LINEHEIGHT, 100, %LINEHEIGHT, "Version") 'CTL ID #1004
    SET_LABEL (CUR_LABEL)

    ADD_INPUT (108, 8*%LINEHEIGHT-3, 400, 20, app.version) 'CTL ID #1005

    ADD_LABEL (8, 10*%LINEHEIGHT, 624, 2*%LINEHEIGHT, "Icon (click button or drag and drop)") 'CTL ID #1006
    SET_LABEL (CUR_LABEL)

    ADD_BUTTON (280+72\2-150\2, 13*%LINEHEIGHT + 4, 150, 24, "Change icon") 'CTL ID #1007
    SET_LABEL (CUR_LABEL)

    ADD_BUTTON (280+72\2-150\2, 13*%LINEHEIGHT + 104, 150, 24, "Default") 'CTL ID #1008
    SET_LABEL (CUR_LABEL)

    ADD_CHECK (8, 320, 624, %LINEHEIGHT, "Display advanced settings in the next screen") 'CTL ID #1009
    SET_LABEL (CUR_LABEL)
    CONTROL SET CHECK hDlg, 1000+ID, app.advanced

    ADD_BUTTON (8, 360, 100, 32, "Previous") 'CTL ID #1010
    SET_LABEL (CUR_LABEL)

    ADD_BUTTON (532, 360, 100, 32, "Next") 'CTL ID #1011
    SET_LABEL (CUR_LABEL)

    ADD_LABEL (370, 13.5*%LINEHEIGHT+39, 262, 3*%LINEHEIGHT, "Not a valid image") 'CTL ID #1012
    CONTROL SET COLOR hDlg, 1000+ID, %RED, %WHITE
    CONTROL SHOW STATE hDlg, 1000+ID, %SW_HIDE

    ADD_IMG (280, 13*%LINEHEIGHT + 30, 72, 72, "ICO72") 'CTL ID #1013
    IF app.icon <> "" THEN SubScreen6displayicon ((app.icon))

    DragAcceptFiles hDlg, %TRUE

END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB ProcScreen6appnamever(CTL AS LONG, MSG AS LONG)
    STATIC version AS STRING
    LOCAL e AS STRING

    SELECT CASE CTL

        CASE 1005 ' version input
            IF MSG = %EN_CHANGE THEN
                CONTROL GET TEXT hDlg, CTL TO e
                IF REMOVE$(e, ANY "1234567890.") <> "" OR INSTR(e, "..") <> 0 THEN
                    BEEP
                    CONTROL SET TEXT hDlg, CTL, version
                END IF
                CONTROL GET TEXT hDlg, CTL TO version
            END IF

        CASE 1007, 1013 ' "Change icon" button + icon image
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                CONTROL SHOW STATE hDlg, 1012, %SW_HIDE ' Hide error message
                SubScreen6pickicon
            END IF

        CASE 1008 ' "default icon" button
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                CONTROL SHOW STATE hDlg, 1012, %SW_HIDE ' Hide error message
                GRAPHIC ATTACH hDlg, 1013, REDRAW
                GRAPHIC RENDER BITMAP "ICO72", (0, 0) - (72, 72)
                GRAPHIC REDRAW
                app.icon = $NUL
            END IF

        CASE 1010 ' "Previous" button
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                DragAcceptFiles hDlg, %FALSE ' deactivate drag'n drop for next screen
                ' get app name
                CONTROL GET TEXT hDlg, 1003 TO e
                app.name = e + $NUL
                ' and app version
                CONTROL GET TEXT hDlg, 1005 TO e
                e = RTRIM$(e, ".")
                IF LEFT$(e, 1) = "." THEN e = "0" + e
                app.version = e + $NUL
                ShowScreen4setbasres
            END IF

        CASE 1011 ' "Next" button
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                DragAcceptFiles hDlg, %FALSE ' deactivate drag'n drop for next screen
                ' get app name
                CONTROL GET TEXT hDlg, 1003 TO e
                app.name = e + $NUL
                ' and app version
                CONTROL GET TEXT hDlg, 1005 TO e
                e = RTRIM$(e, ".")
                IF LEFT$(e, 1) = "." THEN e = "0" + e
                app.version = e + $NUL
                ' fill all other default app values
                IF app.vcode = "" THEN app.vcode = VerCod((app.version)) + $NUL
                IF app.path = "" THEN app.path = app.name + $NUL
                IF app.package = "" THEN DefineDefaultAppPackage
                IF console.title = "" THEN console.title = app.name + " Program Output" + $NUL
                IF console.input = "" THEN console.input = app.name + " Text Input" + $NUL
                ' go on with the building (or advanced options)
                CONTROL GET CHECK hDlg, 1009 TO app.advanced
                IF app.advanced THEN
                    ShowScreen7advanced
                ELSE
                    ShowScreen8makeapk
                END IF
            END IF

        END SELECT
END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB SubScreen6displayicon (img AS STRING)
    LOCAL i, j AS LONG

    IF hAPKicon THEN GdipDisposeImage hAPKicon
    GdipLoadImageFromFile UCODE$(img), hAPKicon

    IF hAPKicon = 0 THEN
        CONTROL SET TEXT hDlg, 1012, GET_LABEL(14) ' Invalid image format
        CONTROL SHOW STATE hDlg, 1012, %SW_SHOW
    ELSE
        GdipGetImageWidth hAPKicon, i
        GdipGetImageHeight hAPKicon, j
        IF i <> j THEN
            CONTROL SET TEXT hDlg, 1012, GET_LABEL(12) ' Pic is not square
            CONTROL SHOW STATE hDlg, 1012, %SW_SHOW
            GdipDisposeImage hAPKicon
        ELSE
            GRAPHIC ATTACH hDlg, 1013, REDRAW
            GRAPHIC CLEAR %WHITE
            GdipDrawImageRect hGdip(), hAPKicon, 0, 0, 72, 72
            GRAPHIC REDRAW
            app.icon = img + $NUL
        END IF
    END IF
END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB SubScreen6pickicon
    LOCAL extList AS STRING
    LOCAL img AS STRING

    extList = CHR$("All Images", 0, "*.BMP;*.ICO;*.PNG;*.JPG;*.JIF;*.GIF;" _
      + "*.TIF;*.TIFF;*.PCD;*.PCX;*.CUT;*.IFF;*.PBM;*.PGM;*.PPM;*.RAS;" _
      + "*.TGA;*.JNG;*.LBM;*.MNG;*.PSD;*.HDR;*.WBMP;*.XPM", 0)

    extList += CHR$("Windows Bitmap (*.bmp)", 0, "*.BMP", 0)
    extList += CHR$("Windows Icon (*.ico)", 0, "*.ICO", 0)
    extList += CHR$("Portable Network Graphic (*.png)", 0, "*.PNG", 0)
    extList += CHR$("JPeg (*.jpg)", 0, "*.JPG;*.JIF", 0)
    extList += CHR$("Graphics Interchange Format (*.gif)", 0, "*.GIF", 0)
    extList += CHR$("Tagged Image File Format (*.tif)", 0, "*.TIF;*.TIFF", 0)
    extList += CHR$("Kodak Photo CD (*.pcd)", 0, "*.PCD", 0)
    extList += CHR$("PC Paintbrush (*.pcx)", 0, "*.PCX", 0)
    extList += CHR$("Dr. Halo (*.cut)", 0, "*.CUT", 0)
    extList += CHR$("Interchange File (*.iff)", 0, "*.IFF", 0)
    extList += CHR$("Portable Bitmap (*.pbm)", 0, "*.PBM", 0)
    extList += CHR$("Portable Graymap (*.pgm)", 0, "*.PGM", 0)
    extList += CHR$("Portable Pixelmap (*.ppm)", 0, "*.PPM", 0)
    extList += CHR$("Sun Raster (*.ras)", 0, "*.RAS", 0)
    extList += CHR$("Targa (*.tga)", 0, "*.TGA", 0)
    extList += CHR$("JPEG Network Graphics (*.jng)", 0, "*.JNG", 0)
    extList += CHR$("IFF Interleaved Bitmap (*.lbm)", 0, "*.LBM", 0)
    extList += CHR$("Multiple-image Network Graphics (*.mng)", 0, "*.MNG", 0)
    extList += CHR$("Photoshop Document (*.psd)", 0, "*.PSD", 0)
    extList += CHR$("High Dynamic Range image (*.hdr)" , 0, "*.HDR", 0)
    extList += CHR$("Wireless Bitmap (*.wbmp)", 0, "*.WBMP", 0)
    extList += CHR$("X Pixmap (*.xpm)", 0, "*.XPM", 0)

    DISPLAY OPENFILE hDlg, , , $EXE, APKfolder, extList, "", "", %OFN_FILEMUSTEXIST TO img
    IF img = "" THEN EXIT SUB ELSE SubScreen6displayicon img

END SUB
'--------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB ShowScreen7advanced

    SCREEN = 7
    CLEAN_DIALOG

    ADD_TITLE (8, 14, 580, 2*%LINEHEIGHT, "MyApp 0.1 advanced settings") 'CTL ID #1001
    SET_LABEL (app.name + $SPC + app.version + $SPC + CUR_LABEL)

    ADD_LABEL (8, 64, 175, %LINEHEIGHT, "App folder") 'CTL ID #1002
    SET_LABEL (CUR_LABEL)

    ADD_INPUT (186, 60, 192, 20, "app.path") 'CTL ID #1003
    SET_LABEL (app.path)

    ADD_LABEL (8, 64+28, 175, %LINEHEIGHT, "Package name") 'CTL ID #1004
    SET_LABEL (CUR_LABEL)

    ADD_INPUT (186, 60+28, 192, 20, "app.package") 'CTL ID #1005
    SET_LABEL (app.package)

    ADD_LABEL (8, 64+28*2, 175, %LINEHEIGHT, "Version code") 'CTL ID #1006
    SET_LABEL (CUR_LABEL)

    ADD_NUMINPUT (186, 60+28*2, 192, 20, "app.vcode") 'CTL ID #1007
    SET_LABEL (app.vcode)

    ADD_LABEL (8, 64+28*3, 175, %LINEHEIGHT, "Console title") 'CTL ID #1008
    SET_LABEL (CUR_LABEL)

    ADD_INPUT (186, 60+28*3, 192, 20, "console.title") 'CTL ID #1009
    SET_LABEL (console.title)

    ADD_LABEL (8, 64+28*4, 175, %LINEHEIGHT, "Input prompt") 'CTL ID #1010
    SET_LABEL (CUR_LABEL)

    ADD_INPUT (186, 60+28*4, 192, 20, "console.input") 'CTL ID #1011
    SET_LABEL (console.input)

    ADD_LABEL (8, 64+28*5, 175, %LINEHEIGHT, "Certificate:") 'CTL ID #1012
    SET_LABEL (CUR_LABEL)

    LOCAL dropdown() AS STRING
    DIM dropdown(0 TO 2)
    ARRAY ASSIGN dropdown() = GET_LABEL(25), GET_LABEL(26), GET_LABEL(27)
    ADD_DROPDOWN (186, 60+28*5, 192, 20*4, dropdown, 1) 'CTL ID #1013
    IF app.keystore <> "" THEN COMBOBOX SELECT hDlg, 1000+ID, 2

    ADD_CHECK (10, 234, 368, %LINEHEIGHT, "Activate Hardware Acceleration") 'CTL ID #1014
    SET_LABEL (CUR_LABEL)
    CONTROL SET CHECK hDlg, 1000+ID, app.hardwareaccel

    ADD_LABEL (-100,-100,0,0, "") 'CTL ID #1015

    ADD_CHECK (10, 266, 520, %LINEHEIGHT, "Create sdcard/<my-app>/data at installation") 'CTL ID #1016
    SET_LABEL (CUR_LABEL + $SPC + $DQ + "sdcard/" + app.path + "/data" + $DQ + $SPC + GET_LABEL(29))
    CONTROL SET CHECK hDlg, 1000+ID, app.createdatadir

    ADD_CHECK (10, 298, 520, %LINEHEIGHT, "Create sdcard/<my-app>/databases at installation") 'CTL ID #1017
    SET_LABEL (GET_LABEL(16) + $SPC + $DQ + "sdcard/" + app.path + "/databases" + $DQ + $SPC + GET_LABEL(29))
    CONTROL SET CHECK hDlg, 1000+ID, app.createdatabasedir

    ADD_CHECK (10, 330, 520, %LINEHEIGHT, "Auto-start your app at device startup") 'CTL ID #1018
    SET_LABEL (CUR_LABEL)
    CONTROL SET CHECK hDlg, 1000+ID, app.startatboot

    ADD_BUTTON (396, 58, 236, 32, "Splash screen / Loading") 'CTL ID #1019
    SET_LABEL (CUR_LABEL)

    ADD_BUTTON (396, 101, 236, 32, "Permissions") 'CTL ID #1020
    SET_LABEL (CUR_LABEL)

    ADD_BUTTON (396, 144, 236, 32, "Console look n feel") 'CTL ID #1021
    SET_LABEL (CUR_LABEL)

    ADD_BUTTON (396, 187, 236, 32, "Copy files to SD at startup") 'CTL ID #1022
    SET_LABEL (CUR_LABEL)
    ' Deactivate if app doesn't need resource
    CONTROL DISABLE hDlg, 1000+ID
    FOR i = 1 TO UBOUND(all_res)
        IF  res4app(i) AND RIGHT$(all_res(i), 1) <> "/" THEN
            CONTROL ENABLE hDlg, 1000+ID
            EXIT FOR
        END IF
    NEXT

    ADD_BUTTON (8, 360, 100, 32, "Previous") 'CTL ID #1023
    SET_LABEL (CUR_LABEL)

    ADD_BUTTON (532, 360, 100, 32, "Next") 'CTL ID #1024
    SET_LABEL (CUR_LABEL)

    ADD_CHECK (396, 234, 236, %LINEHEIGHT, "Activate Hardware Acceleration") 'CTL ID #1025
    SET_LABEL (GET_LABEL(13))
    CONTROL SET CHECK hDlg, 1000+ID, app.encryptbas


END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB ProcScreen7advanced(CTL AS LONG, MSG AS LONG)
    LOCAL e AS STRING
    LOCAL i AS LONG

    SELECT CASE CTL

        CASE 1003 ' app path input
            IF MSG = %EN_CHANGE THEN
                CONTROL GET TEXT hDlg, CTL TO e
                CONTROL SET TEXT hDlg, 1016, GET_LABEL(16) + $SPC + $DQ _
                    + "sdcard/" + e + "/data" + $DQ + $SPC + GET_LABEL(29)
                CONTROL SET TEXT hDlg, 1017, GET_LABEL(16) + $SPC + $DQ _
                    + "sdcard/" + e + "/databases" + $DQ + $SPC + GET_LABEL(29)
            END IF

        CASE 1013 ' Keystore dropdown list
            IF MSG = %CBN_SELENDOK THEN
                COMBOBOX GET SELECT hDlg, CTL TO i
                SELECT CASE i
                    CASE 1 : app.keystore = $NUL ' Default keystore
                    CASE 2 : SelectKeystore      ' Select another (existing) keystore
                    CASE 3 : CreateNewKeystore   ' Create a new keystore
                END SELECT
            END IF

        CASE 1014 ' "Hardware Acceleration" checkbox
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                CONTROL GET CHECK hDlg, 1014 TO i
                IF i THEN MyMsgBox hDlg, GET_LABEL(70), GET_LABEL(14), %MB_ICONWARNING
            END IF

        CASE 1019 ' "Splash screen" button
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                ShowSplashScreenDialog hDlg
            END IF

        CASE 1020 ' "Permissions" button
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                ShowPermissionsDialog hDlg
            END IF

        CASE 1021 ' "Console look n feel" button
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                ShowConsoleLnfDialog hDlg
            END IF

        CASE 1022 ' "Copy resources to SD" button
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                ShowCopyToSDdialog hDlg
            END IF

        CASE 1023 ' "Previous" button
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                ShowScreen6appnamever
            END IF

        CASE 1024 ' "Next" button
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                CONTROL GET CHECK hDlg, 1014 TO app.hardwareaccel
                CONTROL GET CHECK hDlg, 1016 TO app.createdatadir
                CONTROL GET CHECK hDlg, 1017 TO app.createdatabasedir
                CONTROL GET CHECK hDlg, 1018 TO app.startatboot
                CONTROL GET CHECK hDlg, 1025 TO app.encryptbas
                CONTROL GET TEXT  hDlg, 1003 TO e : app.path       = e + $NUL
                CONTROL GET TEXT  hDlg, 1005 TO e : app.package    = e + $NUL
                CONTROL GET TEXT  hDlg, 1007 TO e : app.vcode      = e + $NUL
                CONTROL GET TEXT  hDlg, 1009 TO e : console.title  = e + $NUL
                CONTROL GET TEXT  hDlg, 1011 TO e : console.input  = e + $NUL
                ShowScreen8makeapk
            END IF

        END SELECT
END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB ShowScreen8makeapk
    LOCAL e AS STRING

    SCREEN = 8
    CLEAN_DIALOG
    ConnectMode = %LOCAL ' New architecture: force %LOCAL mode

    ADD_ANIM 'CTL ID #1001

    ADD_LABEL (8, 13*%LINEHEIGHT, 624, 2*%LINEHEIGHT, "Creating XXX.APK /OR/ Your APK was created!") 'CTL ID #1002
    SET_LABEL (STRREPLACE(CUR_LABEL, "APK", app.name + ".apk")) ' "Creating <app-name>.apk"

    ADD_BUTTON (8, 360, 100, 32, "Previous") 'CTL ID #1003
    SET_LABEL (CUR_LABEL)

    ADD_BUTTON (8, 15.5*%LINEHEIGHT, 500, 24, "install it on your device via WiFi")  'CTL ID #1004
    SET_LABEL (CUR_LABEL)
    CONTROL SHOW STATE hDlg, 1000+ID, %SW_HIDE

    ADD_BUTTON (8, 18.5*%LINEHEIGHT, 500, 24, "install it on your device via USB")  'CTL ID #1005
    SET_LABEL (CUR_LABEL)
    CONTROL SHOW STATE hDlg, 1000+ID, %SW_HIDE

    ADD_BUTTON (8, 21.5*%LINEHEIGHT, 500, 24, "open its folder on your computer")  'CTL ID #1006
    SET_LABEL (CUR_LABEL)
    CONTROL SHOW STATE hDlg, 1000+ID, %SW_HIDE

    ADD_BUTTON (8, 24.5*%LINEHEIGHT, 500, 24, "convert another program to APK")  'CTL ID #1007
    SET_LABEL (CUR_LABEL)
    CONTROL SHOW STATE hDlg, 1000+ID, %SW_HIDE

    ADD_BUTTON (532, 360, 100, 32, "Leave") 'CTL ID #1008
    SET_LABEL (CUR_LABEL)

    IF ProjShortcut <= 1 THEN ' normal mode, or "build" shortcut -> make APK
        START (ThreadScreen8makeapk)
    ELSEIF ProjShortcut >= 2 THEN ' "WiFi install" or "USB install" shortcuts -> install APK
        DIALOG POST hDlg, %WM_END_OF_BUILD, 0, 0
        START (ThreadScreen8installapk)
    END IF

END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
THREAD FUNCTION ThreadScreen8makeapk(BYVAL arg AS DWORD) AS LONG
    LOCAL i, lRes AS LONG
    LOCAL e, s AS STRING

    ' Move resources from generic rfo-basic\data to dedicated <app.path>\data
    FOR i = 1 TO UBOUND(app_res)
        IF INSTR(app_res(i), "/source/") = 0 AND INSTR(app_res(i), "rfo-basic") <> 0 THEN lRes = -1 : EXIT FOR
    NEXT
    IF lRes THEN
        IF MyMsgBox(hDlg, STRREPLACE(GET_LABEL(10), "XXX", (app.path)+"\data"), $EXE, %MB_ICONQUESTION OR %MB_YESNO) = 1 THEN
            FOR i = 1 TO UBOUND(app_res)
                e = MID$(app_res(i), 2)
                IF INSTR(e, "/source/") = 0 AND INSTR(e, "rfo-basic") = 1 THEN
                    s = STRREPLACE(e, "rfo-basic", (app.path))
                    MakeSureDirectoryPathExists APKfolder + STRREPLACE(s, "/", "\")
                    NAME APKfolder + STRREPLACE(e, "/", "\") AS APKfolder + STRREPLACE(s, "/", "\")
                    app_res(i) = LEFT$(app_res(i), 1) + s
                END IF
            NEXT
        END IF
    END IF

    DIALOG POST hDlg, %WM_START_ANIM_THINK, 0, 0

    ' Save the project both in .RFO format and easyapk .XML
    SaveProjectAsRFO APKfolder + app.name + ".rfo"
    SaveProjectAsXML APKfolder + app.name + ".xml"

    ' If no custom splash screen defined, dump default one from Quick APK resource
    IF app.splashdisplay  = 0 OR app.splashimg = "" THEN
        LOCAL uwBuff AS STRING
        uwBuff = RESOURCE$(RCDATA, "POWERED")
        MakeSureDirectoryPathExists APKfolder + "rfo-basic\data\"
        UW_SaveRawFile (APKfolder + "rfo-basic\data\splash.jpg")
    END IF

    ' Recompile with easyapk
    easyapk_err = "" : easyapk_dir = ""
    s  = DATE$ ' Try to guess easyapk temp folder
    s  = RIGHT$(s, 4) + LEFT$(s, 2) + MID$(s, 4, 2)
    s += STRREPLACE(TIME$, ":", "")
    s  = ENVIRON$("TEMP") + "\" + LEFT$(s,-2) + "??.*"

    e = DUMP_CMD ("easyapk " + $DQ + APKfolder + app.name + ".xml" + $DQ)

    IF INSTR(e, " correctly produced") = 0 THEN
        i = INSTR(e, $LF)   ' Build failed -> extract the error
        i = INSTR(i+1, e, $LF)
        easyapk_err = DOS2WIN(MID$(e, i+1))
        s = DIR$(s, %SUBDIR) : DIR$ CLOSE ' Get exact easyapk temp folder
        IF LEN(s) THEN easyapk_dir = ENVIRON$("TEMP") + "\" + s
    END IF

    DIALOG POST hDlg, %WM_END_OF_BUILD, 0, 0
    EXIT_THREAD

END FUNCTION
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
THREAD FUNCTION ThreadScreen8installapk(BYVAL arg AS DWORD) AS LONG
    STATIC e AS STRING
    LOCAL i, j, lRes AS LONG

    FMWK = "installation in progress"
    DIALOG POST hDlg, %WM_INSTALL_START, 0, 0

    IF ConnectMode = %WIFI THEN
        lRes = InstallApkOverWiFi (APKfolder + app.name + ".apk")
        IF lRes = %FALSE THEN e = "Error during WiFi transfer"
    ELSE ' %USB
        device = GetConnectedAndroidModelName() ' Try to connect to device
        IF device = "" THEN
            lRes = %FALSE
            e = "No Android device found over USB"
        ELSE
            RUN_CMD "adb kill-server" : PAUSE 500 ' To release ADB
            e = DUMP_CMD ("adb uninstall " + app.package, EXE.PATH$ + "tools")
            e = DUMP_CMD ("adb install " + $DQ + APKfolder + app.name + ".apk" + $DQ, EXE.PATH$ + "tools")
        END IF
        IF INSTR(LCASE$(e), "success") > 0 THEN ' look at install result, and propose to launch
            lRes = %TRUE
        ELSEIF e <> "" THEN ' app could not be installed
            i = INSTR(e, "[")
            IF i > 0 THEN
                j = INSTR(i, e, "]")
                IF j > 0 THEN
                    e = MID$(e, i, j-i+1)
                END IF
            END IF
            lRes = %FALSE
        END IF
    END IF

    FMWK = e
    DIALOG POST hDlg, %WM_INSTALL_STOP, lRes, 0
    EXIT_THREAD

END FUNCTION
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
THREAD FUNCTION ThreadScreen8launchinstalledapk(BYVAL arg AS DWORD) AS LONG
    LOCAL e AS STRING

    RUN_CMD "adb kill-server" : PAUSE 500 ' To release ADB

    e = DUMP_CMD ("adb shell am start -n " + app.package + "/" _
      + app.package + ".Basic", EXE.PATH$ + "tools")

    EXIT_THREAD

END FUNCTION
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB ProcScreen8makeapk(CTL AS LONG, MSG AS LONG)
    SELECT CASE CTL

        CASE 1003 ' "Previous" button
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                KillThread hThreadMain
                IF ProjShortcut = 0 THEN ' normal mode, or "modify" -> back to screen 6 appdetail
                    ShowScreen6appnamever
                ELSE                     ' any shortcut (build, wifi install, usb install) -> back to screen 2 projects
                    ShowScreen2projects
                END IF
            END IF

        CASE 1004 ' Install APK via WiFi / View error message
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                IF LEN(easyapk_err) THEN
                    MYMSGBOX hDlg, easyapk_err, "easyapk error", %MB_ICONERROR
                ELSE
                    ConnectMode = %WIFI
                    START (ThreadScreen8installapk)
                END IF
            END IF

        CASE 1005 ' Install APK via USB / Verbose build in command line
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                IF LEN(easyapk_err) THEN
                    Execute(ENVIRON$("COMSPEC"), "/K easyapk -v " + $DQ + APKfolder + app.name + ".xml" + $DQ, 1)
                ELSE
                    ConnectMode = %USB
                    START (ThreadScreen8installapk)
                END IF
            END IF

        CASE 1006 ' Open APK folder / Open easyapk temp folder for debug
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                IF LEN(easyapk_err) THEN
                    IF LEN(easyapk_dir) THEN
                        ShellExecute NUL, "open", "explorer.exe" + $NUL, _
                            easyapk_dir + $NUL, NUL, %SW_SHOWNORMAL
                    ELSE
                        ShellExecute NUL, "open", "explorer.exe" + $NUL, "/select," _
                            + $DQ + APKfolder + app.name + ".xml" + $DQ + $NUL, NUL, %SW_SHOWNORMAL
                    END IF
                ELSE
                    IF EXIST(APKfolder + app.name + ".apk") THEN
                        ShellExecute NUL, "open", "explorer.exe" + $NUL, "/select," _
                            + $DQ + APKfolder + app.name + ".apk" + $DQ + $NUL, NUL, %SW_SHOWNORMAL
                    ELSE
                        ShellExecute NUL, "open", "explorer.exe" + $NUL, _
                            APKfolder + $NUL, NUL, %SW_SHOWNORMAL
                    END IF
                END IF
            END IF

        CASE 1007 ' Convert another program
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                ShowScreen2projects
            END IF

        CASE 1008 ' Exit
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                DIALOG POST hDlg, %WM_END_DIALOG, 0, 0
            END IF

        END SELECT
END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB ShowScreen9about
    SCREEN = 9
    CLEAN_DIALOG

    ADD_TABS (10, 20, 625, 350) 'CTL ID #1001

    ADD_BUTTON (532, 360, 100, 32, "Back") 'CTL ID #1002
    SET_LABEL (CUR_LABEL)

    IF UBOUND(hTab) < 0 THEN DIM hTab(1 TO 5)
    LOCAL richtext AS STRING

    '=============================================================================
    hTab(1) = ot.AddPage ("Buy me a beer", RGB(120,120,120), %WHITE)
    '=============================================================================
        CONTROL ADD GRAPHIC, hTab(1), 1010, "", 5, 118, 147, 47, _
            %SS_NOTIFY OR %WS_TABSTOP CALL ProcMainDialog()
        GRAPHIC ATTACH       hTab(1), 1010
        GRAPHIC RENDER BITMAP "DONATE", (0, 0) - (147, 47)
        linkable += "<1010>"

        CONTROL ADD "RichEdit", hTab(1), 1011, "", 155, 52, 453, 252, _
            %WS_CHILD OR %WS_VISIBLE OR %ES_MULTILINE OR %ES_READONLY _
            OR %WS_VSCROLL CALL ProcMainDialog()
        richtext += "[font:l,14][c][titleBlue][b][i]RFO-BASIC! Quick APK[/b][/i][eop]"
        richtext += "[font:l,10][black]"
        richtext += "is developed by Nicolas Mougin aka mougino[eol][eol]"
        richtext += "Along with the BASIC! Launcher and App Builder,[eol]"
        richtext += "it is completely free and in constant evolution[eol][eol]"
        richtext += "Making this program is a hobby of mine but it also[eol]"
        richtext += "takes significant time and effort[eol][eol]"
        richtext += "If you appreciate my work, consider giving[eol]"
        richtext += "5 or 10€ by clicking the image on the left[eol]"
        richtext += "or by visiting http://mougino.free.fr[eol][eol]"
        richtext += "Thank you!"
        RTF_SET hTab(1), 1011, richtext

    '=============================================================================
    hTab(2) = ot.AddPage ("Changelog", RGB(120,120,120), %WHITE)
    '=============================================================================
        CONTROL ADD "RichEdit", hTab(2), 1020, "", 5, 2, 603, 302, _
          %WS_CHILD OR %WS_VISIBLE OR %ES_MULTILINE OR %ES_READONLY _
          OR %WS_VSCROLL CALL ProcMainDialog()

        richtext  = "[font:l,10][black][l][eol]"
        richtext += "[h:yellow]You are currently running " + $EXE + $SPC + $VER
        richtext += " under " + TRIM$(is32or64bitOS) + "-bit " + WinVer
        richtext += " on a " + TRIM$(is32or64bitProc) + "-bit architecture.[/h][eop]"

        richtext += "[font:l,14][eol][titleBlue][b][i]History:[/b][/i][eop]"
        richtext += "[font:l,10][black][l][eol]"

        richtext += "[h:aqua]v01.88.00[/h] released on [i]September 14th, 2015[/i][eol]"
        richtext += "[*][b]Updated [/b]BASIC! version: QuickAPK now relies on v01.88.01[eol]"
        richtext += "  https://bintray.com/rfo-basic/android/RFO-BASIC/v01.88/view/release[eol]"
        richtext += "[*][b]Added [/b]support for new splash screen timer[eol]"
        richtext += "[*][b]Added [/b]check of writing permission in home directory[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.87.02[/h] released on [i]June 3rd, 2015[/i][eol]"
        richtext += "[*][b]Fixed [/b]corrupted APK when app name contains an underscore[eol]"
        richtext += "[*][b]Updated [/b]BASIC! version: QuickAPK now relies on v01.87.04[eol]"
        richtext += "  (adds Gr.Bitmap.Fill, multiple Gr.Get.Value, OnLowMemory: etc.)[eol]"
        richtext += "[*][b]Added [/b]support for new 1.87.04 graphic acceleration option[eol]"
        richtext += "[*][b]Added [/b]detection + automatic support for GW lib (Gika's idea)[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.87.01[/h] released on [i]May 30th, 2015[/i][eol]"
        richtext += "[*][b]Fixed [/b]crash when trying to open illegal .rfo project file[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.87.00[/h] released on [i]May 1st, 2015[/i][eol]"
        richtext += "[*][b]Updated [/b]BASIC! version: QuickAPK now relies on v01.87[eol]"
        richtext += "  https://bintray.com/rfo-basic/android/RFO-BASIC/v01.87/view/release[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.86.01[/h] released on [i]April 22nd, 2015[/i][eol]"
        richtext += "[*][b]Added [/b]buttons to change font size (in main projects screen)[eol]"
        richtext += "[*][b]Removed [/b]by default the Stop/Editor console-menu in user apk[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.86.00[/h] released on [i]April 13th, 2015[/i][eol]"
        richtext += "[*][b]Updated [/b]BASIC! version: QuickAPK now relies on v01.86[eol]"
        richtext += "  https://bintray.com/rfo-basic/android/RFO-BASIC/v01.86/view/release[eol]"
        richtext += "[*][b]Fixed [/b]the RUN command to run another .bas in standalone APK[eol]"
        richtext += "[*][b]Removed [/b]the VIBRATE permission that was always needed[eol]"
        richtext += "[*][b]Added [/b]support for multiple commands per lines[eol]"
        richtext += "[*][b]Added [/b]support for '?' command, improved LEFT$ RIGHT$ and MID$, etc.[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.85.06[/h] released on [i]March 31st, 2015[/i][eol]"
        richtext += "[*][b]Improved [/b]easyapk to v0.7 testing if admin rights are needed[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.85.05[/h] released on [i]March 20th, 2015[/i][eol]"
        richtext += "[*][b]Fixed [/b]new .bas files encryption option that was always activated[eol]"
        richtext += "[*][b]Improved [/b]speed of Quick APK WiFi: added support for v0.2 Android app[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.85.04[/h] released on [i]March 18th, 2015[/i][eol]"
        richtext += "[*][b]Fixed [/b]permission autodetect for command ""WIFI.INFO""[eol]"
        richtext += "[*][b]Fixed [/b]a regression that sometimes added an empty permission[eol]"
        richtext += "[*][b]Removed [/b]the days offline counter, for users behind a proxy[eol]"
        richtext += "[*][b]Added [/b]advanced setting to encrypt .bas files (needs easyapk v0.6)[eol]"
        richtext += "[*][b]Added [/b]double-click to edit .bas file in second screen program list[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.85.03[/h] released on [i]March 10th, 2015[/i][eol]"
        richtext += "[*][b]Fixed [/b]autodetection of permissions for commands ""TTS.*""[eol]"
        richtext += "  and ""DEVICE bundle_ptr""[eol]"
        richtext += "[*][b]Fixed [/b]BASIC! apk on which Quick APK relies on (no more[eol]"
        richtext += "  performance decrease in graphics)[eol]"
        richtext += "[*][b]Updated [/b]Dutch translation thanks to Aat[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.85.02[/h] released on [i]March 5th, 2015[/i][eol]"
        richtext += "[*][b]Fixed [/b]non-working APKs when local folders don't respect lower case[eol]"
        richtext += "  see http://rfobasic.freeforums.org/post18397.html#p18397[eol]"
        richtext += "[*][b]Improved [/b]easyapk to v0.5 (minor changes in standalone mode)[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.85.01[/h] released on [i]March 1st, 2015[/i][eol]"
        richtext += "[*][b]Fixed [/b]random crash in the initialization (splash screen)[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.85.00[/h] released on [i]February 23rd, 2015[/i][eol]"
        richtext += "[*][b]Updated [/b]BASIC! version: QuickAPK now relies on v01.85[eol]"
        richtext += "  https://bintray.com/rfo-basic/android/RFO-BASIC/v01.85/view/release[eol]"
        richtext += "[*][b]Fixed [/b]auto-set of permissions, databases folder, etc. in local mode[eol]"
        richtext += "[*][b]Fixed [/b]easyapk v0.4 ""access denied"" when creating custom icons[eol]"
        richtext += "[*][b]Improved [/b]permissions autoset in advanced settings screen[eol]"
        richtext += "[*][b]Added [/b]fallback verbose command-line build when normal build fails[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.84.07[/h] released on [i]January 13th, 2015[/i][eol]"
        richtext += "[*][b]Fixed [/b]easyapk v0.3, handle resources with extension > 3 characters[eol]"
        richtext += "[*][b]Fixed [/b]a crash when modifying a project whose .bas was deleted[eol]"
        richtext += "[*][b]Added [/b]a popup to move your app resources to a dedicated folder[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.84.06[/h] released on [i]January 6th, 2015[/i][eol]"
        richtext += "[*][b]Fixed [/b]Quick APK nasty crashes after successful/failed build[eol]"
        richtext += "[*][b]Fixed [/b]the crash when the program contained an illegal INCLUDE[eol]"
        richtext += "  file, see http://rfobasic.freeforums.org/post16755.html#p16755[eol]"
        richtext += "[*][b]Fixed [/b]the auto-detection of permissions needed by ""DEVICE"",[eol]"
        richtext += "  see http://rfobasic.freeforums.org/post16749.html[eol]"
        richtext += "[*][b]Fixed [/b]the auto-start option of your app at device boot, see[eol]"
        richtext += "  http://rfobasic.freeforums.org/post16433.html#p16433[eol]"
        richtext += "[*][b]Added [/b]automatic creation of an easyapk '.xml' in the home directory[eol]"
        richtext += "[*][b]Extended [/b]the project history in the main screen to 18 last projects[eol]"
        richtext += "[*][b]Rewrote [/b]the prog architecture esp. regrouped the device-connect code[eol]"
        richtext += "[*][b]Grayed [/b]WRITE_EXTERNAL_STORAGE and VIBRATE permissions (always set)[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.84.05[/h] released on [i]November 24th, 2014[/i][eol]"
        richtext += "[*][b]Fixed [/b]auto-detection of RECEIVE_SMS permission[eol]"
        richtext += "[*][b]Fixed [/b]language popup menu in first welcome screen[eol]"
        richtext += "[*][b]Fixed [/b]connection warning for new offline installations[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.84.04[/h] released on [i]November 23rd, 2014[/i][eol]"
        richtext += "[*][b]Added [/b]Romanian translation thanks to gikam[eol]"
        richtext += "[*][b]Fixed [/b]a small path issue in easyapk v0.2[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.84.03[/h] released on [i]November 22nd, 2014[/i][eol]"
        richtext += "[*][b]Fixed [/b]temporary files persistence after a build fails[eol]"
        richtext += "[*][b]Changed [/b]""convert.exe"" from ImageMagick to own solution (reduces[eol]"
        richtext += "  size from 8 MB to 50 KB, and more input formats recognized...)[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.84.02[/h] released on [i]November 9th, 2014[/i][eol]"
        richtext += "[*][b]Fixed [/b]copy resources to SD > Select all/none[eol]"
        richtext += "[*][b]Added [/b]control on allowed characters in certificate passwords[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.84.01[/h] released on [i]November 3rd, 2014[/i][eol]"
        richtext += "[*][b]Fixed [/b]crash in project screen when a project icon was deleted[eol]"
        richtext += "[*][b]Fixed [/b]empty black screen bug introduced in v01.84.00[eol]"
        richtext += "[*][b]Updated [/b]Dutch translation thanks to Aat[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.84.00[/h] released on [i]November 2nd, 2014[/i][eol]"
        richtext += "[*][b]Updated [/b]BASIC! version: QuickAPK now relies on v01.84[eol]"
        richtext += "  https://bintray.com/rfo-basic/android/RFO-BASIC/v01.84/view/release[eol]"
        richtext += "[*][b]Changed [/b]architecture: QuickAPK now relies on 3rd party[eol]"
        richtext += "  tool 'easyapk' v0.1 (Open Source GNU/GPL)[eol]"
        richtext += "[*][b]Force [/b]readme popup to display before selecting main bas & res[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.83.01[/h] released on [i]September 26th, 2014[/i][eol]"
        richtext += "[*][b]Fixed [/b]copy of resources to SD-Card (Advanced settings)[eol]"
        richtext += "[*][b]Fixed [/b]choice of no progress box (Advanced settings)[eol]"
        richtext += "[*][b]Fixed [/b]changing the progress message (Advanced settings)[eol]"
        richtext += "[*][b]Fixed [/b]permission autodetect memory (Advanced settings)[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.83.00[/h] released on [i]September 10th, 2014[/i][eol]"
        richtext += "[*][b]Updated [/b]BASIC! version: QuickAPK now relies on v01.83[eol]"
        richtext += "  https://bintray.com/rfo-basic/android/RFO-BASIC/v01.83/view/release[eol]"
        richtext += "[*][b]Fixed [/b]last console personalization issue at https://github.com/RFO-BASIC/Basic/issues/162[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.82.01[/h] released on [i]September 5th, 2014[/i][eol]"
        richtext += "[*][b]Fixed [/b]sub-windows wrong titles in Advanced settings[eol]"
        richtext += "[*][b]Fixed [/b]checkboxes refresh problem in Advanced settings[eol]"
        richtext += "[*][b]Fixed [/b]console personalization, except a regression in [eol]"
        richtext += "  ""empty"" color, see https://github.com/RFO-BASIC/Basic/issues/162[eol]"
        richtext += "[*][b]Added [/b]an updated Dutch translation thanks to Aat[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.82.00[/h] released on [i]August 22nd, 2014[/i][eol]"
        richtext += "[*][b]Updated [/b]BASIC! version: QuickAPK now relies on v01.82[eol]"
        richtext += "  https://bintray.com/rfo-basic/android/RFO-BASIC/v01.82/view/release[eol]"
        richtext += "[*][b]Updated [/b]""Splash screen"" configuration in advanced settings[eol]"
        richtext += "[*][b]Implemented [/b]new screen: projects! Added possibility to save a[eol]"
        richtext += "  previous compilation as a project, then load, re-build, modify, or[eol]"
        richtext += "  install it on device. Redesigned connect modes options in this screen[eol]"
        richtext += "[*][b]Implemented [/b]auto save & restore of QuickAPK window last position[eol]"
        richtext += "[*][b]Implemented [/b]test of /rfo-basic/source/*.bas existence in both local[eol]"
        richtext += "  and remote modes http://rfobasic.freeforums.org/post13358.html#p13358[eol]"
        richtext += "[*][b]Dramatically improved [/b]analysis speed of device filesystem in USB[eol]"
        richtext += "  (by a factor 16x: 15s instead of 4mn on an old WinXP machine)[eol]"
        richtext += "[*][b]Plugged [/b]button ""convert another program"" after successful APK[eol]"
        richtext += "[*][b]Fixed [/b]copy to SD of resources not in /data or /databases[eol]"
        richtext += "[*][b]Fixed [/b]loss of control after entering then exiting ""About""[eol]"
        richtext += "[*][b]Fixed [/b]the nasty freezes during animations (at last!)[eol]"
        richtext += "[*][b]Fixed [/b]a bug when using INCLUDE files inside main .bas[eol]"
        richtext += "[*][b]Fixed [/b]""Java not installed"" false alarm when an adb[eol]"
        richtext += "  session is residing in the background monopolizing Java[eol]"
        richtext += "[*][b]Added [/b]Connect Mode tips in select prog/build/install screens[eol]"
        richtext += "[*][b]Added [/b]new German translation thanks to Konrad (dip GmbH)[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.80.03[/h] released on [i]May 31st, 2014[/i][eol]"
        richtext += "[*][b]Fixed [/b]auto-detection of ""<my-app>/data"" creation, see[eol]"
        richtext += "  http://rfobasic.freeforums.org/post12399.html[eol]"
        richtext += "[*][b]Added [/b]test to make sure Java is on the system, see[eol]"
        richtext += "  http://rfobasic.freeforums.org/post12384.html[eol]"
        richtext += "[*][b]Added [/b]System info (for diagnostic) in About > Changelog[eol]"
        richtext += "[*][b]Added [/b]new Dutch translation thanks to Aat[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.80.02[/h] released on [i]May 11th, 2014[/i][eol]"
        richtext += "[*][b]Fixed [/b]a bug when copying resource to SD selected in QuickAPK from[eol]"
        richtext += "  <home-directory>\<my-app>\data and not <home-directory>\rfo-basic\data[eol]"
        richtext += "[*][b]Fixed [/b]volume keys in graphic screen, they now work! [eol]"
        richtext += "  see GitHub issue #9: https://github.com/RFO-BASIC/Basic/issues/9[eol]"
        richtext += "[*][b]Added [/b]a button to change your home directory, see[eol]"
        richtext += "  http://rfobasic.freeforums.org/how-to-change-home-directory-t2450.html[eol]"
        richtext += "[*][b]Added [/b]a ""select all/none"" button in the ""copy to sd"" window[eol]"
        richtext += "  + allowed Shift and Ctrl keys in the listbox[eol]"
        richtext += "  see http://rfobasic.freeforums.org/copy-resources-to-sd-t2452.html[eol]"
        richtext += "[*][b]Added [/b]splash screen functionality in the Advanced settings, see[eol]"
        richtext += "  http://rfobasic.freeforums.org/graphical-loading-screen-t2451.html[eol]"
        richtext += "[*][b]Added [/b]Hardware Acceleration option in the Advanced settings[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.80.01[/h] released on [i]May the 4th, 2014[/i][eol]"
        richtext += "[*][b]Added [/b]an About menu containing info about libraries, tools, QuickAPK[eol]"
        richtext += "  license, changelog etc.[eol]"
        richtext += "[*][b]Added [/b]support for xhdpi, xxhdpi and xxxhdpi iconography[eol]"
        richtext += "  See http://rfobasic.freeforums.org/icon-size-t2427.html[eol]"
        richtext += "[*][b]Upgraded [/b]Android Asset Packaging Tool (aapt.exe) to v19.0.3[eol]"
        richtext += "[*][b]Fixed [/b]label refresh issue when changing language in screen 4, in Local[eol]"
        richtext += "  mode[eol]"
        richtext += "[*][b]Improved [/b]Local mode resource subfolder creation[eol]"
        richtext += "  see http://rfobasic.freeforums.org/post12066.html[eol]"
        richtext += "[*][b]Improved [/b](yet again) stability, especially after APK creation[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.80.00[/h] released on [i]April 28th, 2014[/i][eol]"
        richtext += "[*][b]Implemented [/b]WiFi connection mode, working with the Android app[eol]"
        richtext += "  downloadable at https://play.google.com/store/apps/details?id=com.rfo.quickapk[eol]"
        richtext += "[*][b]Improved [/b]stability in case of diconnected device (USB and WiFi), and[eol]"
        richtext += "  animations[eol]"
        richtext += "[*][b]Updated [/b]BASIC! version: QuickAPK now relies on v01.80[eol]"
        richtext += "  https://bintray.com/rfo-basic/android/RFO-BASIC/v01.80/view/release[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.79.00[/h] released on [i]April 8th, 2014[/i][eol]"
        richtext += "[*][b]Removed [/b]incorrect popup ""offline for N days""[eol]"
        richtext += "  see http://rfobasic.freeforums.org/post11653.html#p11653[eol]"
        richtext += "[*][b]Fixed [/b]USB communication for Android 4.2.2 and greater devices[eol]"
        richtext += "  by using latest secure adb with new commands[eol]"
        richtext += "  see http://rfobasic.freeforums.org/post11651.html#p11651[eol]"
        richtext += "[*][b]Updated [/b]Dutch translation thanks to Aat[eol]"
        richtext += "[*][b]Updated [/b]BASIC! version: QuickAPK now relies on v01.79[eol]"
        richtext += "  https://bintray.com/rfo-basic/android/RFO-BASIC/v01.79/view/release[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.78.03[/h] released on [i]April 2nd, 2014[/i][eol]"
        richtext += "[*][b]Removed [/b]limitation of 30 days for beta versions[eol]"
        richtext += "[*][b]Fixed [/b]usage of web resources[eol]"
        richtext += "  see http://rfobasic.freeforums.org/file-path-reference-in-basic-t2314.html[eol]"
        richtext += "[*][b]Fixed [/b]databases folder auto-creation[eol]"
        richtext += "  see http://rfobasic.freeforums.org/sqlite-db-open-t2334.html[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.78.02[/h] released on [i]March 14th, 2014[/i][eol]"
        richtext += "[*][b]Fixed [/b]custom permissions, they now work[eol]"
        richtext += "[*][b]Deactivated [/b]BASIC! shortcut creation in built APKs[eol]"
        richtext += "[*][b]Improved [/b]readability of some screens (e.g. button ""change icon"")[eol]"
        richtext += "[*][b]Finished [/b]screen ""advanced settings"", you can now:[eol]"
        richtext += "   [*]Copy resources to SD (button is activated only if you chose[eol]"
        richtext += "     resources)[eol]"
        richtext += "   [*]Use custom certificate or even create a new one[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.78.01[/h] released on [i]March 1st, 2014[/i][eol]"
        richtext += "[*][b]Fixed [/b]the mandatory download issue[eol]"
        richtext += "[*][b]Fixed [/b]some graphical glitches[eol]"
        richtext += "[*][b]Updated [/b]BASIC! version: QuickAPK now relies on v01.78[eol]"
        richtext += "  https://bintray.com/rfo-basic/android/RFO-BASIC/v01.78/view/release[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.77.04[/h] released on [i]February 28th, 2014[/i][eol]"
        richtext += "[*][b]Removed [/b]the 30 days limit of beta version[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.77.03[/h] released on [i]February 22nd, 2014[/i][eol]"
        richtext += "[*][b]Fixed [/b]the behaviour of the resource treeview, it works now well[eol]"
        richtext += "  both with mouse and keyboard navigation[eol]"
        richtext += "[*][b]Made [/b]Quick APK blind-friendly, changed some screens for better[eol]"
        richtext += "  accessibility and now compatible with Freedom Scientific's JAWS[eol]"
        richtext += "[*][b]Added [/b]latest Dutsh translation from Aat[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.77.01[/h] released on [i]February 13th, 2014[/i][eol]"
        richtext += "[*][b]Dropped [/b]the GDI+ library to make program more stable[eol]"
        richtext += "[*][b]Fixed [/b]the possible crash at the end after successful APK[eol]"
        richtext += "[*][b]Updated [/b]BASIC! version: QuickAPK now relies on v01.77[eol]"
        richtext += "  https://bintray.com/rfo-basic/android/RFO-BASIC/v01.77/view/release[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.76.03[/h] released on [i]January 29th, 2014[/i][eol]"
        richtext += "[*][b]Added [/b]possibility for translators of QuickAPK to test[eol]"
        richtext += "  their labels in a new language with a local ""lng_test.xml""[eol]"
        richtext += "[eol][eop]"

        richtext += "[h:aqua]v01.76.01[/h] released on [i]January 24th, 2014[/i][eol]"
        richtext += "[*][b]Initial release[/b][eol]"
        richtext += "  see http://rfobasic.freeforums.org/anouncing-rfo-basic-quick-apk-standalone-builder-t1988.html"
        richtext += "[eol][eop]"

        RTF_SET hTab(2), 1020, richtext

    '=============================================================================
    hTab(3) = ot.AddPage ("Thanks", RGB(120,120,120), %WHITE)
    '=============================================================================
        CONTROL ADD "RichEdit", hTab(3), 1030, "", 5, 2, 603, 302, _
          %WS_CHILD OR %WS_VISIBLE OR %ES_MULTILINE OR %ES_READONLY _
          OR %WS_VSCROLL CALL ProcMainDialog()
        richtext  = "[l][font:t,13][j][maroon]I wish to express my sincere gratitude to the existing donators: Tom Hatcher, Gary Green, "
        richtext += "Andreas Wilhelm, Andrew Hood, Jeffrey Morgan, Laurence Taylor, Mike Hersee, Barry C D Johnson, John Spillett, "
        richtext += "Richard Brown, Peter Smith, Theodore Cornelissen, Uday Kothari, spacemax, Peter Michael Ripley, Arlen Moens, "
        richtext += "Peter G. Smith, Alessio Gatti, Paul Rodgers, Joel Ostiguy, Maria Martincekova, Dale Rupert, Christopher Odgen, "
        richtext += "Chris Terpin, Andre Triches, and John Wong"
        richtext += ", many thanks to them![eol][eol]"
        richtext += "I also want to thank Paul Laughton for his wonderful work and the confidence "
        richtext += "he put in me and my fellow companions of the BASIC! community, "
        richtext += "Antonis for his dedication on the forum, "
        richtext += "Marc Sammartano without whom BASIC! wouldn't be what it is, "
        richtext += "Aat Don for his great ideas and help on the Dutch translation, "
        richtext += "humpty for his investment and his great apps, "
        richtext += "Stefano P for his x-apk-builder and x-quick-apk for Linux "
        richtext += "and also Alberto, Gilles (Cassiope34), apeine, AcidRain, and brochi.[eol][eol]"
        richtext += "Long live BASIC![eop]"
        RTF_SET hTab(3), 1030, richtext

    '=============================================================================
    hTab(4) = ot.AddPage ("Components", RGB(120,120,120), %WHITE)
    '=============================================================================
        CONTROL ADD "RichEdit", hTab(4), 1040, "", 5, 2, 603, 302, _
          %WS_CHILD OR %WS_VISIBLE OR %ES_MULTILINE OR %ES_READONLY _
          OR %WS_VSCROLL CALL ProcMainDialog()

        richtext  = "[font:l,14][eol][l][titleBlue][b][i]Quick APK uses the following tools:[/b][/i][eol][eop]"
        richtext += "[font:l,10][black]"

        richtext += "[*][h:yellow]The Android Asset Packaging Tool (aapt)[/h] "
        richtext += "part of The Android Software Development Kit, "
        richtext += "© Google Open Source software licensed here: http://developer.android.com/sdk/terms.html[eol][eop]"

        richtext += "[*][h:yellow]The Android Debug Bridge tool (ADB)[/h] "
        richtext += "part of The Android Software Development Kit, "
        richtext += "© Google Open Source software licenced here: http://developer.android.com/sdk/terms.html[eol][eop]"

        richtext += "[*][h:yellow]apktool[/h] an Open Source tool for reverse engineering Android apk files, "
        richtext += "under Apache License 2.0 website: https://code.google.com/p/android-apktool/[eol][eop]"

        richtext += "[*][h:yellow]keytool[/h] a key and certificate management utility part of Java SE, "
        richtext += "© Oracle license here: http://www.oracle.com/technetwork/java/javase/terms/license/[eol][eop]"

        richtext += "[*][h:yellow]openssl[/h] an SSL/TLS Open Source toolkit © The OpenSSL Project, "
        richtext += "under a BSD-style license available here: http://www.openssl.org/source/license.html[eol][eop]"

        richtext += "[*][h:yellow]signapk[/h] an Open Source application to sign Android Packages (.apk) "
        richtext += "under a GNU GPL v2 license website: https://code.google.com/p/signapk/[eol][eop]"

        richtext += "[*][h:yellow]zipalign[/h] an archive alignment tool that provides "
        richtext += "important optimization to Android application (.apk) files, "
        richtext += "part of The Android Software Development Kit, "
        richtext += "© Google Open Source software licensed here: http://developer.android.com/sdk/terms.html[eol][eop]"

        richtext += "[*][h:yellow]splitpem[/h] an utility to split the certificate and private key from a "
        richtext += "Privacy Enhanced Email file (PEM), © Nicolas Mougin under a CC BY-NC-ND 3.0 license "
        richtext += "http://creativecommons.org/licenses/by-nc-nd/3.0/[eol][eop]"

        RTF_SET hTab(4), 1040, richtext

    '=============================================================================
    hTab(5) = ot.AddPage ("License", RGB(120,120,120), %WHITE)
    '=============================================================================
        CONTROL ADD "RichEdit", hTab(5), 1050, "", 5, 2, 603, 302, _
          %WS_CHILD OR %WS_VISIBLE OR %ES_MULTILINE OR %ES_READONLY _
          OR %WS_VSCROLL CALL ProcMainDialog()
        richtext  = "[font:l,10][black][eol][eol]RFO-BASIC! Quick APK is released under a Creative Commons license of type [b]CC BY-NC-ND 3.0[/b] "
        richtext += "[i]Attribution-NonCommercial-NoDerivs 3.0 Unported[/i]:[eol][eol]"
        richtext += "By using this software, you agree to bond to the terms of this license, meaning you are free "
        richtext += "to download and share it [u](for free)[/u] with others as long as you credit me, but you cannot "
        richtext += "change it or sell it in any way.[eol][eol]"
        richtext += "You can find the full license text here:[eol]"
        richtext += "http://creativecommons.org/licenses/by-nc-nd/3.0/legalcode"
        RTF_SET hTab(5), 1050, richtext

END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB ProcScreen9about(CTL AS LONG, MSG AS LONG)
    SELECT CASE CTL

        CASE 1010 ' "Donate" image
            IF MSG = %STN_CLICKED THEN
                LOCAL linkText AS STRING
                linkText  = "https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business="
                linkText += "Y7N7QDWN5ZC94&lc=US&item_name=mougino%20software&currency_code="
                linkText += "EUR&bn=PP%2dDonationsBF%3abtn_donateCC_LG%2egif%3aNonHosted"
                ShellExecute(%NULL, "open", BYCOPY linkText, "", "", %SW_SHOW)
            END IF

        CASE 1002 ' "Back" button
            IF MSG = %BN_CLICKED OR MSG = 1 THEN
                ot = NOTHING ' Destroy class instantiation
                DIALOG GET LOC hDlg TO aax, aay
                DIALOG POST hDlg, %WM_END_DIALOG, 0, 0
                AfterAbout = %TRUE
                INIT_DIALOG (640, 400, $EXE + $SPC + $VER + "   by mougino 2014")
            END IF
        END SELECT
END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
CALLBACK FUNCTION ProcMainDialog()
    LOCAL e, rep AS STRING
    LOCAL i, j, hNul, hNode() AS LONG
    STATIC idEvent, projOver, lpo, anim, sens, alpha, wavimg, eots AS LONG
    STATIC binstr AS STRING

    CB_SAVEPOS ' Save and restore position of program window upon creation / closing

    IF SCREEN = 5 THEN
        CB_DRAGNDROP ' Handle drag and drop of icon (in Screen 5 - App detail)
    END IF

    IF SCREEN = 9 THEN
        CB_COLORTABS ' Handle colored tabs control  (in Screen 9 - About)
    END IF

    SELECT CASE AS LONG CB.MSG

        CASE %WM_INITDIALOG ' Standard dialog creation message
            IF AfterAbout THEN
                'dialog set color cb.hndl, -1, %rgb_lightblue ' DEBUG
                CONTROL ADD IMAGE, CB.HNDL, 1000, "LNG", 608, 8, 24, 24, %SS_NOTIFY
                ShowScreen2projects
                DIALOG SET LOC CB.HNDL, aax, aay
            ELSE
                ShowScreen0splash
            END IF

        CASE %WM_INIT_DIALOG ' Custom initialization message
            'DIALOG SET COLOR CB.HNDL, -1, %RGB_LIGHTBLUE ' DEBUG
            CONTROL ADD IMAGE, CB.HNDL, 1000, "LNG", 608, 8, 24, 24, %SS_NOTIFY
            IF APKfolder = "" THEN ShowScreen1welcome ELSE ShowScreen2projects

        CASE %WM_REFRESH_DIALOG ' Custom refresh message, used in CLEAN_DIALOG()
            DIALOG REDRAW CB.HNDL

        CASE %WM_END_DIALOG ' Custom end message, used when exiting About screen
            DIALOG END CB.HNDL

        CASE %WM_NOTIFY ' Standard notify message
            SELECT CASE SCREEN
                CASE 4 ' screen 4 setbasres
                    LOCAL ctl, msg AS LONG
                    CB_TREEVIEW_HANDLEMSG(ctl, msg) ' Specific messages from TREEVIEW control
                    ProcScreen4setbasres  ctl, msg
                CASE 9 ' screen 9 About
                    IF CB.NMCODE = %EN_LINK THEN RTF_hyperlink (CB.HNDL, CB.CTL, CB.LPARAM) ' Process RTF hyperlinks
            END SELECT

        CASE %WM_SETCURSOR ' Standard hovering-over-controls message
            IF GetDlgCtrlId(CB.WPARAM) = 1000 THEN ' Change cursor to question mark when hovering over language button
                SetCursor LoadCursor(%NULL, BYVAL %IDC_HELP)
                SetWindowLong CB.HNDL, %dwl_msgresult, 1
                FUNCTION = 1
            ELSEIF INSTR(linkable, "<" + TRIM$(GetDlgCtrlId(CB.WPARAM)) + ">") <> 0 THEN ' Change cursor to link-hand when hovering over links
                SetCursor LoadCursor(%NULL, BYVAL %IDC_HAND)
                SetWindowLong CB.HNDL, %dwl_msgresult, 1
                FUNCTION = 1
            END IF
            IF SCREEN = 2 THEN ' Scroll long project names when hovering over them in screen 2
                projOver = GetDlgCtrlId(CB.WPARAM) - 1001
                IF projOver >= 1 AND projOver <= UBOUND(project) THEN
                    IF projLoc(projOver) <> 32767 AND idEvent = 0 THEN
                        lpo = projOver ' lpo = last project over
                        TRIGGER_TIMER (%ID_TIMER_SCROLL, 150)
                    END IF
                ELSEIF idEvent <> 0 THEN
                    STOP_ANY_TIMER ' Stop scrolling
                    ProcScreen2projects 1001 + lpo, %ID_TIMER_SCROLL - 1
                END IF
            END IF

        CASE %WM_CONTEXTMENU ' Standard right-click message
            IF SCREEN = 2 THEN
                IF projOver <> 0 THEN ProcScreen2projects 1001+projOver, 2 ' "2" = right-click message
            END IF

        CASE %WM_START_ANIM_LOG
            CONTROL SET FOCUS CB.HNDL, 1002
            START_ANIM_LOG

        CASE %WM_WAIT_END_ANIM_LOG
            IF ConnectMode = %USB THEN
                DO WHILE sens = 1 ' wait for USB Android guy to arrive
                    SLEEP 100
                    DIALOG DOEVENTS ' let callback breathe
                LOOP
            END IF
            FUNCTION = 1

        CASE %WM_NO_DEVICE_CONNECTED
            STOP_ANY_TIMER ' Stop "logging" animation where it is (leave it as is)
            SHOW_DISCONNECTED
            CONTROL SHOW STATE CB.HNDL, 1002, %SW_HIDE
            CONTROL SHOW STATE CB.HNDL, 1003, %SW_SHOW ' "No device found"
            CONTROL SET FOCUS  CB.HNDL, 1003
            CONTROL ENABLE     CB.HNDL, 1008 ' Connect mode dropdown list
            CONTROL ENABLE     CB.HNDL, 1009 ' Connect mode readme
            CONTROL ENABLE     CB.HNDL, 1006 ' "Retry" button

        CASE %WM_RFO_NOT_FOUND
            STOP_ANIM_TRANSFER ' Stop "transfer" animation properly
            SHOW_DISCONNECTED
            CONTROL SHOW STATE CB.HNDL, 1002, %SW_HIDE
            CONTROL SHOW STATE CB.HNDL, 1004, %SW_SHOW ' "RFO-BASIC! not found"
            CONTROL SET FOCUS  CB.HNDL, 1004
            CONTROL ENABLE     CB.HNDL, 1008 ' Connect mode dropdown list
            CONTROL ENABLE     CB.HNDL, 1009 ' Connect mode readme
            CONTROL ENABLE     CB.HNDL, 1006 ' "Retry" button

        CASE %WM_START_ANIM_TRANSFER
            IF SCREEN = 3 THEN CONTROL SET TEXT CB.HNDL, 1002, GET_LABEL(8) + $SPC + device + ". " + GET_LABEL(9) ' "Found a XX. Analyzing"
            CONTROL SET FOCUS CB.HNDL, 1002
            SHOW_LOGGED
            START_ANIM_TRANSFER

        CASE %WM_DEVICE_FOUND
            CONTROL SET TEXT CB.HNDL, 1002, GET_LABEL(2) + $SPC + device + $SPC + GET_LABEL(12) ' "Copying files from the " + device + " to the computer..."

        CASE %WM_GOT_RES
            IF ConnectMode = %USB THEN
                STOP_ANIM_TRANSFER ' Stop "transfer" animation properly
                'After it ends, it will transfer automatically to ShowScreen4setbasres
            ELSE
                STOP_ANY_TIMER ' Stop "thinking" (LOCAL) or "transfer" (WIFI) animations as is
                ShowScreen4setbasres
            END IF

        CASE %WM_NO_LOCAL_BAS ' In Screen 4
            CONTROL SHOW STATE CB.HNDL, 1002, %SW_HIDE
            CONTROL SHOW STATE CB.HNDL, 1003, %SW_SHOW ' "No .bas in rfo-basic/source/"
            CONTROL SET FOCUS  CB.HNDL, 1003
            CONTROL ENABLE     CB.HNDL, 1008 ' "Refresh" button

        CASE %WM_START_ANIM_THINK
            anim = 0
            TRIGGER_TIMER (%ID_TIMER_THINK, 50)

        CASE %WM_TRANSFER_END ' In Screen 5
            eots = CB.WPARAM ' eots = "End Of Transfer" Status
            STOP_ANIM_TRANSFER ' Stop "transfer" animation properly
            IF eots = -1 THEN ' End Of Transfer Status -1 = abort because device unplugged
                SHOW_DISCONNECTED
                CONTROL SHOW STATE CB.HNDL, 1002, %SW_HIDE ' Hide "Copying files"
                CONTROL SHOW STATE CB.HNDL, 1003, %SW_SHOW ' Show "Error: the device has been unplugged!"
                CONTROL ENABLE     CB.HNDL, 1005 ' "Retry" button
                CONTROL SET FOCUS  CB.HNDL, 1003
            ELSEIF eots = +1 THEN ' End Of Transfer Status +1 = go on with creating APK
                ShowScreen6appnamever
            END IF

        CASE %WM_TIMER ' Standard timer events
            IF idEvent = 0 THEN EXIT FUNCTION
            IF CB.WPARAM = %ID_TIMER_SCROLL THEN ' Scroll long project names in screen 2
                IF SCREEN <> 2 THEN
                    STOP_ANY_TIMER
                ELSE
                    ProcScreen2projects 1001 + projOver, %ID_TIMER_SCROLL
                END IF
            ELSEIF CB.WPARAM = %ID_TIMER_LOG THEN ' Animate "logging" animation in screen 3
                IF SCREEN <> 3 THEN
                    STOP_ANY_TIMER
                ELSE
                    ANIMATE_LOGGING
                END IF
            ELSEIF CB.WPARAM = %ID_TIMER_TRANSFER THEN ' Animate "transfer" animation in screens 3 and 5
                IF SCREEN <> 3 AND SCREEN <> 5 THEN
                    STOP_ANY_TIMER
                ELSE
                    ANIMATE_TRANSFER
                END IF
            ELSEIF CB.WPARAM = %ID_TIMER_THINK THEN ' Animate "thinking" animation in screens 3 and 8
                IF SCREEN <> 3 AND SCREEN <> 8 THEN
                    STOP_ANY_TIMER
                ELSE
                    ANIMATE_THINKING
                END IF
            ELSEIF CB.WPARAM = %ID_TIMER_INSTALL THEN ' Animate "install" animation in screen 8
                IF SCREEN <> 8 THEN
                    STOP_ANY_TIMER
                ELSE
                    ANIMATE_INSTALL
                END IF
            END IF

        CASE %WM_DISPLAY_BAS_RES
            CONTROL DISABLE CB.HNDL, 1010 ' Connect mode dropdown list
            CONTROL SET FOCUS  CB.HNDL, 1002 ' "Select your bas and res"
            DIM hNode(1 TO UBOUND(all_res))
            FOR i = 1 TO UBOUND(all_res)
                IF i MOD 1000 = 0 THEN DIALOG DOEVENTS ' let the callback breathe
                ' populate listbox with .bas files from rfo-basic/source exactly
                IF INSTR(all_res(i), "rfo-basic/source/") = 1 _
                    AND TALLY(all_res(i), "/") = 2 _
                    AND RIGHT$(all_res(i), 1) <> "/" THEN
                    LISTBOX ADD CB.HNDL, 1004, LinuxName(all_res(i)) TO hNul
                    LISTBOX SET USER CB.HNDL, 1004, hNul, i
                    IF app.bas = all_res(i) THEN
                        LISTBOX SELECT CB.HNDL, 1004, hNul
                        CONTROL ENABLE CB.HNDL, 1009 ' Activate "Next" button
                    END IF
                END IF
                ' populate treeview with entire <sdpath> file system
                IF RIGHT$(all_res(i), 1) = "/" THEN ' folder
                    hNul = 0
                    IF TALLY(all_res(i), "/") > 1 THEN ' subfolder (not at the root)
                        rep = LinuxPath(LEFT$(all_res(i), -1))
                        FOR j = 1 TO i - 1
                            IF all_res(j) = rep THEN hNul = hNode(j) : EXIT FOR
                        NEXT
                    END IF
                    IF LinuxName(LEFT$(all_res(i), -1)) <> "" THEN
                        TREEVIEW INSERT ITEM CB.HNDL, 1005, hNul, %TVI_SORT, 1, 1, LinuxName(LEFT$(all_res(i), -1)) TO hNode(i)
                        TREEVIEW SET USER CB.HNDL, 1005, hNode(i), i
                        IF UBOUND(res4app) > 0 THEN TREEVIEW SET CHECK CB.HNDL, 1005, hNode(i), res4app(i)
                    END IF
                ELSE ' file
                    hNul = 0
                    rep = REMOVE$(all_res(i), LinuxName(all_res(i)))
                    FOR j = 1 TO i - 1
                        IF all_res(j) = rep THEN hNul = hNode(j) : EXIT FOR
                    NEXT
                    IF LinuxName(all_res(i)) <> "" THEN
                        TREEVIEW INSERT ITEM CB.HNDL, 1005, hNul, %TVI_LAST, 2, 2, LinuxName(all_res(i)) TO hNul
                        TREEVIEW SET USER    CB.HNDL, 1005, hNul, i
                        IF UBOUND(res4app) > 0 THEN TREEVIEW SET CHECK CB.HNDL, 1005, hNul, res4app(i)
                    END IF
                END IF
            NEXT
            CONTROL SHOW STATE CB.HNDL, 1004, %SW_SHOW ' make bas listbox visible
            CONTROL REDRAW     CB.HNDL, 1004
            TREEVIEW SELECT    CB.HNDL, 1005, 0 ' Unselect
            CONTROL SHOW STATE CB.HNDL, 1005, %SW_SHOW ' make res treeview visible
            CONTROL REDRAW     CB.HNDL, 1005
            CONTROL SHOW STATE CB.HNDL, 1006, %SW_SHOW ' make treeview selection visible
            DIALOG POST        CB.HNDL, %WM_REFRESH_TREEVIEW_SEL, 0, 0
            CONTROL ENABLE CB.HNDL, 1010 ' Connect mode dropdown list

        CASE %WM_REFRESH_TREEVIEW_SEL
            LOCAL ndir, nfil AS LONG
            FOR i = 1 TO UBOUND(all_res)
                IF res4app(i) THEN
                    IF RIGHT$(all_res(i), 1) = "/" THEN INCR ndir ELSE INCR nfil
                END IF
            NEXT
            IF ndir > 0 THEN
                e = TRIM$(ndir) + $SPC + GET_LABEL(4)
                IF ndir > 1 THEN REPLACE "(s)" WITH "s" IN e ELSE REPLACE "(s)" WITH "" IN e
                IF ndir > 1 THEN REPLACE "(en)" WITH "en" IN e ELSE REPLACE "(en)" WITH "" IN e
            END IF
            IF nfil > 0 THEN
                IF ndir > 0 THEN e += $SPC
                e += TRIM$(nfil) + $SPC + GET_LABEL(5)
                IF nfil > 1 THEN REPLACE "(s)" WITH "s" IN e ELSE REPLACE "(s)" WITH "" IN e
                IF nfil > 1 THEN REPLACE "(en)" WITH "en" IN e ELSE REPLACE "(en)" WITH "" IN e
            END IF
            IF ndir > 0 OR nfil > 0 THEN
                e += $SPC + GET_LABEL(6)
                IF ndir + nfil > 1 THEN REPLACE "(s)" WITH "s" IN e ELSE REPLACE "(s)" WITH "" IN e
                IF ndir + nfil > 1 THEN REPLACE "(en)" WITH "en" IN e ELSE REPLACE "(en)" WITH "" IN e
            END IF
            CONTROL SET TEXT CB.HNDL, 1006, e ' "N folder(s) N file(s) selected"

        CASE %WM_END_OF_BUILD
            IF LEN(easyapk_err) THEN ' Build Status -1 = there was a problem
                SHOW_APK_KO
                CONTROL SHOW STATE CB.HNDL, 1003, %SW_SHOW ' "Previous" button
                CONTROL SET TEXT  CB.HNDL, 1002, GET_LABEL(13) ' "Problem creating APK!"
                CONTROL SET FOCUS CB.HNDL, 1001
                CONTROL SET FOCUS CB.HNDL, 1002
                CONTROL SET TEXT  CB.HNDL, 1004, GET_LABEL(20) ' "See the error message"
                CONTROL SET TEXT  CB.HNDL, 1005, GET_LABEL(22) ' "Launch verbose build in command line"
                CONTROL SET TEXT  CB.HNDL, 1006, GET_LABEL(21) ' "Open the easyapk folder for debug"
                CONTROL SHOW STATE CB.HNDL, 1004, %SW_SHOW
                CONTROL SHOW STATE CB.HNDL, 1005, %SW_SHOW
                CONTROL SHOW STATE CB.HNDL, 1006, %SW_SHOW
            ELSE                     ' Build Status +1 = APK correctly produced
                SHOW_APK_OK
                CONTROL SHOW STATE CB.HNDL, 1003, %SW_HIDE ' "Previous" button
                CONTROL SET TEXT  CB.HNDL, 1002, STRREPLACE(GET_LABEL(14), "APK", app.name + ".apk") ' "APK created!"
                CONTROL SET FOCUS CB.HNDL, 1001
                CONTROL SET FOCUS CB.HNDL, 1002
                FOR i = 1004 TO 1007
                    CONTROL SHOW STATE CB.HNDL, i, %SW_SHOW
                NEXT
            END IF

        CASE %WM_INSTALL_START
            CONTROL SET TEXT CB.HNDL, 1002, GET_LABEL(16) ' "Installation in progress"
            FOR i = 1004 TO 1007
                CONTROL DISABLE CB.HNDL, i
            NEXT
            CONTROL SET FOCUS CB.HNDL, 1002
            anim = 0
            TRIGGER_TIMER (%ID_TIMER_INSTALL, 250)

        CASE %WM_INSTALL_STOP
            CONTROL SET TEXT CB.HNDL, 1002, GET_LABEL(14) ' "APK created!"
            SHOW_APK_OK
            FOR i = 1004 TO 1007
                CONTROL ENABLE CB.HNDL, i
            NEXT
            CONTROL SET FOCUS CB.HNDL, 1002
            IF CB.WPARAM = %TRUE THEN ' Installation worked!
                e = GET_LABEL(17)
                IF ConnectMode = %USB THEN ' in USB, propose to launch it!
                    i = MyMsgBox (CB.HNDL, e, $EXE + $SPC + $VER, %MB_ICONQUESTION OR %MB_YESNO)
                    IF i = 1 THEN
                        START (ThreadScreen8launchinstalledapk)
                    END IF
                ELSEIF ConnectMode = %WIFI THEN ' in WiFi, just say everything went ok
                    e = LEFT$(e, INSTR(-1, e, "."))
                    MyMsgBox CB.HNDL, e, $EXE + $SPC + $VER, %MB_ICONINFORMATION
                END IF
            ELSEIF CB.WPARAM = %FALSE THEN ' Installation didn't work
                e = GET_LABEL(18) + $CR + FMWK
                MyMsgBox CB.HNDL, e, $EXE + $SPC + $VER, %MB_ICONWARNING
            END IF

        CASE %WM_SET_CTL_VISIBILITY
            CONTROL SHOW STATE CB.HNDL, CB.WPARAM, CB.LPARAM

        CASE %WM_SET_CTL_FOCUS
            CONTROL SET FOCUS CB.HNDL, CB.WPARAM

        CASE %WM_COMMAND
            IF CB.CTL = 1000 THEN ' Language popup menu
                LOCAL pt AS POINTAPI
                LOCAL hWnd AS LONG
                LOCAL lRes AS LONG
                GetCursorPos pt
                hWnd = WindowFromPoint(pt)
                lRes = TrackPopupMenuEx(hLanguage, %TPM_LEFTALIGN OR %TPM_RIGHTBUTTON OR %TPM_RETURNCMD, pt.X, pt.Y, hWnd, NUL)
                IF lRes <> 0 AND lRes <> (INSTR($LNGLIST+TRIM$(DEVLNG), LNG)+1)\2 THEN
                    MENU SET STATE hLanguage, (INSTR($LNGLIST+TRIM$(DEVLNG), LNG)+1)\2, %MF_UNCHECKED
                    LNG = MID$($LNGLIST+TRIM$(DEVLNG), 2*lRes-1, 2)
                    MENU SET STATE hLanguage, (INSTR($LNGLIST+TRIM$(DEVLNG), LNG)+1)\2, %MF_CHECKED
                    ChangeLanguage()
                END IF
            END IF

            SELECT CASE SCREEN
                CASE 1   : ProcScreen1welcome      CB.CTL, CB.CTLMSG
                CASE 2   : ProcScreen2projects     CB.CTL, CB.CTLMSG
                CASE 3   : ProcScreen3listres      CB.CTL, CB.CTLMSG
                CASE 4   : ProcScreen4setbasres    CB.CTL, CB.CTLMSG
                CASE 5   : ProcScreen5getdeviceres CB.CTL, CB.CTLMSG
                CASE 6   : ProcScreen6appnamever   CB.CTL, CB.CTLMSG
                CASE 7   : ProcScreen7advanced     CB.CTL, CB.CTLMSG
                CASE 8   : ProcScreen8makeapk      CB.CTL, CB.CTLMSG
                CASE 9   : ProcScreen9about        CB.CTL, CB.CTLMSG
            END SELECT

'        CASE %WM_SYSCOMMAND
'            IF ((CB.WPARAM AND &HFFF0) AND %SC_CLOSE) = %SC_CLOSE THEN
'                IF SCREEN = 8 THEN
'                    FUNCTION = 0 'App will terminate
'                   'FUNCTION = 1 'App won't terminate, even with ALT-F4
'                END IF
'            END IF

        CASE %WM_DESTROY, %WM_KILL_THREAD
            KillThread hThreadMain

    END SELECT
END FUNCTION
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB ChangeLanguage()
    LOCAL t, t2 AS STRING
    LOCAL ctl, lRes, w, h AS LONG

    WritePrivateProfileString "Config", "language_code", TRIM$(LNG), INIFILE
    IF INSTR($LNGLIST, LNG) = 0 THEN ' the dev language under test
        LOCAL ff AS LONG
        ff = FREEFILE
        OPEN EXE.PATH$ + "lng_test.xml" FOR BINARY AS #ff
        GET$ #ff, LOF(#ff), LngXml
        LngXml = UTF8TOCHR$(LngXml)
        CLOSE #ff
    ELSE ' a standard language, not dev
        LngXml = UTF8TOCHR$(RESOURCE$(RCDATA, "LNG_" + LNG))
    END IF

    LBL_CANCEL   = XmlContent(LngXml, "button id=""cancel""")
    LBL_RETRY    = XmlContent(LngXml, "button id=""retry""")
    LBL_CONTINUE = XmlContent(LngXml, "button id=""continue""")
    LBL_YES      = XmlContent(LngXml, "button id=""yes""")
    LBL_NO       = XmlContent(LngXml, "button id=""no""")
    LBL_ABORT    = XmlContent(LngXml, "button id=""abort""")
    LBL_IGNORE   = XmlContent(LngXml, "button id=""ignore""")
    LBL_OK       = XmlContent(LngXml, "button id=""ok""")

    ScrXml = XmlContent(LngXml, "screen id=" + $DQ + TRIM$(SCREEN) + $DQ)

    FOR ctl = 1 TO ID
        t = GET_LABEL(ctl)

        IF SCREEN = 1 THEN
            IF ctl = 1 THEN
                t += $SPC + $EXE
            ELSEIF ctl = 4 THEN
                t = "<" + t + ">"
                IF APKfolder <> "" THEN t = LEFT$(APKfolder, -1)
            END IF

        ELSEIF SCREEN = 2 THEN
            IF ctl = 1 THEN
                lRes = INSTR(t, $SPC) : t2 = MID$(t, lRes+1) : t = LEFT$(t, lRes-1)
                GRAPHIC ATTACH hDlg, 1000+ctl, REDRAW : GRAPHIC CLEAR %WHITE : GRAPHIC RENDER BITMAP "NEWP", (0, 0) - (84, 84)
                GRAPHIC SET FONT hFontTtl : GRAPHIC COLOR %BLACK, -2
                GRAPHIC TEXT SIZE t  TO w, h : GRAPHIC SET POS (42-w\2, 21)   : GRAPHIC PRINT t  ' New
                GRAPHIC TEXT SIZE t2 TO w, h : GRAPHIC SET POS (42-w\2, 63-h) : GRAPHIC PRINT t2 ' project
                GRAPHIC REDRAW
                t = ""
            ELSEIF ctl > UBOUND(project) + 1 AND ctl <= 18 THEN
                t = GET_LABEL(2)
                GRAPHIC ATTACH hDlg, 1000+ctl, REDRAW
                GRAPHIC BOX (0, 73) - (72, 73+%LINEHEIGHT), 0, %WHITE, %WHITE
                GRAPHIC SET FONT hFontLbl : GRAPHIC COLOR RGB(220,220,220), %WHITE
                GRAPHIC TEXT SIZE t TO w, h : GRAPHIC SET POS (36-w\2, 73) : GRAPHIC PRINT t
                GRAPHIC REDRAW
                t = ""
            ELSEIF ctl = 35 THEN
                MENU SET TEXT hProject, 1, GET_LABEL(11)
                MENU SET TEXT hProject, 2, GET_LABEL(12)
                MENU SET TEXT hProject, 3, GET_LABEL(13)
                t = ""
            ELSEIF ctl = 36 THEN
                t = GET_LABEL(6)
            ELSEIF ctl = 38 THEN
                SubScreen2refreshApkfolder()
                t = GET_LABEL(7)
            ELSEIF ctl = ID THEN
                t = GET_LABEL(8)
            ELSE
                t = ""
            END IF

        ELSEIF SCREEN = 3 THEN
            IF ctl = 2 THEN
                IF ConnectMode = %LOCAL THEN
                    t = GET_LABEL(1)
                ELSEIF device <> "" THEN
                    t = GET_LABEL(8) + $SPC + device + ". " + GET_LABEL(9)
                END IF
            ELSEIF ctl = 8 THEN
                COMBOBOX SET TEXT   hDlg, 1000+ctl, 1, GET_LABEL(10)
                COMBOBOX SET TEXT   hDlg, 1000+ctl, 2, GET_LABEL(11)
                COMBOBOX SET TEXT   hDlg, 1000+ctl, 3, GET_LABEL(12)
                COMBOBOX SELECT     hDlg, 1000+ctl, ConnectMode
                t = ""
            ELSEIF ctl = 9 THEN
                t = GET_LABEL(13)
            END IF

        ELSEIF SCREEN = 4 THEN
            IF ctl = 3 THEN
                IF ConnectMode = %LOCAL THEN
                    t = StrReplace(t, "XXX", RTRIM$(APKfolder, "\"))
                ELSE
                    t = StrReplace(StrReplace(t, "XXX", RTRIM$(sdpath, "/")), "\", "/")
                END IF
            ELSEIF ctl = 6 THEN
                DIALOG POST hDlg, %WM_REFRESH_TREEVIEW_SEL, 0, 0
            ELSEIF ctl = 10 THEN
                COMBOBOX SET TEXT   hDlg, 1000+ctl, 1, GET_OLABEL(3, 10)
                COMBOBOX SET TEXT   hDlg, 1000+ctl, 2, GET_OLABEL(3, 11)
                COMBOBOX SET TEXT   hDlg, 1000+ctl, 3, GET_OLABEL(3, 12)
                COMBOBOX SELECT     hDlg, 1000+ctl, ConnectMode
                t = ""
            ELSEIF ctl = 11 THEN
                t = GET_OLABEL(3, 13)
            END IF

        ELSEIF SCREEN = 5 THEN
            IF ctl = 2 THEN
                IF device = "" THEN
                    t = GET_LABEL(1)
                ELSE
                    t += $SPC + device + $SPC + GET_LABEL(12)
                END IF
            ELSEIF ctl = 6 THEN
                COMBOBOX SET TEXT   hDlg, 1000+ctl, 1, GET_OLABEL(3, 10)
                COMBOBOX SET TEXT   hDlg, 1000+ctl, 2, GET_OLABEL(3, 11)
                COMBOBOX SET TEXT   hDlg, 1000+ctl, 3, GET_OLABEL(3, 12)
                COMBOBOX SELECT     hDlg, 1000+ctl, ConnectMode
                t = ""
            ELSEIF ctl = 7 THEN
                t = GET_OLABEL(3, 13)
            END IF

        ELSEIF SCREEN = 6 THEN
            IF ctl = 12 THEN
                t = " "
            END IF

        ELSEIF SCREEN = 7 THEN
            IF ctl = 1 THEN
                t = app.name + $SPC + app.version + $SPC + t
            ELSEIF ctl = 16 THEN
                t = t + $SPC + $DQ + "sdcard/" + app.path + "/data" + $DQ + $SPC + GET_LABEL(29)
            ELSEIF ctl = 17 THEN
                t = GET_LABEL(16) + $SPC + $DQ + "sdcard/" + app.path + "/databases" + $DQ + $SPC + GET_LABEL(29)
            ELSEIF ctl = 13 THEN
                COMBOBOX GET SELECT hDlg, 1000+ctl TO lRes
                COMBOBOX SET TEXT   hDlg, 1000+ctl, 1, GET_LABEL(25)
                COMBOBOX SET TEXT   hDlg, 1000+ctl, 2, GET_LABEL(26)
                COMBOBOX SET TEXT   hDlg, 1000+ctl, 3, GET_LABEL(27)
                COMBOBOX SELECT     hDlg, 1000+ctl, lRes
                t = ""
            ELSEIF ctl = 25 THEN
                t = GET_LABEL(13)
            END IF

        ELSEIF SCREEN = 8 THEN
            IF ctl = 2 THEN
                IF FMWK = "installation in progress" THEN
                    t = GET_LABEL(16)
                ELSEIF EXIST(APKfolder + app.name + ".apk") THEN
                    t = STRREPLACE(GET_LABEL(14), "APK", app.name + ".apk")
                ELSE
                    t = STRREPLACE(t, "APK", app.name + ".apk")
                END IF
            ELSEIF (ctl = 4 OR ctl = 5) AND LEN(easyapk_err) THEN
                t = GET_LABEL(16+ctl)
            END IF

        END IF

        IF t <> "" THEN CONTROL SET TEXT hDlg, 1000+ctl, t
    NEXT

END SUB
'--------------------------------------------------------------------------------

'-----------------------------------------------------------------------------------------------------
SUB LoadConfig()
    LOCAL a AS ASCIIZ * %MAX_PATH
    GetPrivateProfileString "Config", "language_code", "EN", a, %MAX_PATH, INIFILE
    LNG = TRIM$(a)
    GetPrivateProfileString "Config", "font_size", "0", a, %MAX_PATH, INIFILE
    font_size = VAL(a)
    GetPrivateProfileString "Config", "apk_folder", "", a, %MAX_PATH, INIFILE
    APKfolder = TRIM$(a)
END SUB
'-----------------------------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB SaveProjectAsXML (proj AS STRING)
    LOCAL i, lRes AS LONG
    LOCAL e, toSD AS STRING

    IF UBOUND(all_res) <= 0 THEN ' Coming from screen 2 "Project > Build" -> array all_res() does not exist!
        IF UBOUND(app_res) > 0 THEN ' Create all_res(), res4app() and app2SD() from a previous session - if any
            REDIM all_res(1 TO UBOUND(app_res))
            REDIM res4app(1 TO UBOUND(app_res))
            REDIM app2SD (1 TO UBOUND(app_res))
            FOR i = 1 TO UBOUND(app_res)
                all_res(i) = MID$(app_res(i), 2)
                res4app(i) = -1
                app2SD(i)  = VAL(LEFT$(app_res(i), 1))
            NEXT
        END IF
    END IF

    ' Make list of resources to be copied to SD-Card
    FOR i = 1 TO UBOUND(app_res)
        IF  LEFT$(app_res(i), 1) = "1" THEN
            e = REMOVE$(MID$(app_res(i), 2), "rfo-basic/")
            e = REMOVE$(e, app.path + "/")
            IF RIGHT$(LCASE$(app_res(i)), 3) = ".db" THEN
                IF INSTR(e, "databases/") = 1 THEN e = REMOVE$(e, "databases/") ELSE e = "../" + e
            ELSE
                IF INSTR(e, "data/") = 1 THEN e = REMOVE$ (e, "data/") ELSE e = "../" + e
            END IF
            toSD += STRING$(3, $TAB) + "<item>" + e + "</item>" + $CRLF
        END IF
    NEXT
    toSD = RTRIM$(toSD, $CRLF)

    ' Create the easyapk XML script
    i = FREEFILE
    OPEN proj FOR OUTPUT AS #i
        PRINT #i, "<?xml version=""1.0"" encoding=""utf-8""?>"
        PRINT #i, ""
        PRINT #i, "<set_local_folder path=" + $DQ + APKfolder + $DQ + " />"
        PRINT #i, ""
        PRINT #i, "<use_base_apk source=" + $DQ + EXE.PATH$ + "tools\Basic.apk"" target=" + $DQ + app.name + ".apk"">"
        PRINT #i, ""

        ' Copy program source (main .bas + include files) and resources
        PRINT #i, $TAB + "<!-- Copy program source (main .bas + include files) and resources -->"
        PRINT #i, $TAB + "<copy_file source=" + $DQ + app.bas + $DQ + " target=""assets/" + STRREPLACE((app.bas), "rfo-basic", (app.path)) + $DQ + " />"
        IF app.encryptbas THEN PRINT #i, $TAB + "<encrypt_file source=""assets/" + STRREPLACE((app.bas), "rfo-basic", (app.path)) + $DQ + " password=" + $DQ + (app.package) + $DQ + " />"
        FOR lRes = 1 TO UBOUND(app_res)
            IF RIGHT$(app_res(lRes), 1) <> "/" THEN ' Not a subfolder
                e = MID$(app_res(lRes), 2)
                PRINT #i, $TAB + "<copy_file source=" + $DQ + e + $DQ + " target=""assets/" + STRREPLACE(e, "rfo-basic", (app.path)) + $DQ + " />"
                IF app.encryptbas AND INSTR(e, "/source/")>0 THEN PRINT #i, $TAB + "<encrypt_file source=""assets/" + STRREPLACE(e, "rfo-basic", (app.path)) + $DQ + " password=" + $DQ + (app.package) + $DQ + " />"
            END IF
        NEXT
        PRINT #i, ""

        ' Change app splash screen
        PRINT #i, $TAB + "<!-- Change app splash screen -->"
        IF app.splashdisplay AND app.splashimg <> "" THEN
            e = RIGHT$((app.splashimg), 3)
            PRINT #i, $TAB + "<copy_file source=" + $DQ + (app.splashimg) + $DQ + " target=""res/drawable/splash." + e + $DQ + " />"
        ELSE
            PRINT #i, $TAB + "<copy_file source=""rfo-basic/data/splash.jpg"" target=""res/drawable/splash.jpg"" />"
        END IF
        PRINT #i, ""

        ' Change app icon
        IF app.icon <> "" THEN
            PRINT #i, $TAB + "<!-- Change app icon -->"
            PRINT #i, $TAB + "<set_app_icon source=" + $DQ + (app.icon) + $DQ + " />"
            PRINT #i, ""
        END IF

        ' Change app name, version, and different settings
        PRINT #i, $TAB + "<!-- Change app name, version, and different settings -->"
        PRINT #i, $TAB + "<modify_xml_values type=""string"">"
        PRINT #i, STRING$(2, $TAB) + "<set_xml_value name=""my_program"" value=" + $DQ + LinuxName(app.bas) + $DQ + " />"
        PRINT #i, STRING$(2, $TAB) + "<set_xml_value name=""app_name"" value=" + $DQ + (app.name) + $DQ + " />
        PRINT #i, STRING$(2, $TAB) + "<set_xml_value name=""version"" value=" + $DQ + (app.version) + $DQ + " />
        PRINT #i, STRING$(2, $TAB) + "<set_xml_value name=""app_path"" value=" + $DQ + (app.path) + $DQ + " />
        PRINT #i, STRING$(2, $TAB) + "<set_xml_value name=""run_name"" value=" + $DQ + (console.title) + $DQ + " />
        PRINT #i, STRING$(2, $TAB) + "<set_xml_value name=""textinput_name"" value=" + $DQ + (console.input) + $DQ + " />
        IF app.advanced THEN PRINT #i, STRING$(2, $TAB) + "<set_xml_value name=""progress_marker"" value=" + $DQ + (app.loadchr) + $DQ + " />
        PRINT #i, $TAB + "</modify_xml_values>"

        PRINT #i, $TAB + "<modify_xml_values type=""bool"">"
        PRINT #i, STRING$(2, $TAB) + "<set_xml_value name=""is_apk"" value=""true"" />"
        PRINT #i, STRING$(2, $TAB) + "<set_xml_value name=""apk_create_data_dir"" value=" + $DQ + JavaTrueFalse(app.createdatadir) + $DQ + " />"
        PRINT #i, STRING$(2, $TAB) + "<set_xml_value name=""apk_create_database_dir"" value=" + $DQ + JavaTrueFalse(app.createdatabasedir) + $DQ + " />"
        PRINT #i, STRING$(2, $TAB) + "<set_xml_value name=""apk_programs_encrypted"" value=" + $DQ + JavaTrueFalse(app.encryptbas) + $DQ + " />"
        IF app.advanced THEN PRINT #i, STRING$(2, $TAB) + "<set_xml_value name=""splash_display"" value=" + $DQ + JavaTrueFalse(app.splashdisplay) + $DQ + " />"
        PRINT #i, $TAB + "</modify_xml_values>"
        PRINT #i, ""

        IF toSD <> "" THEN
            PRINT #i, $TAB + "<!-- List resources to be copied to SD-Card at startup -->"
            PRINT #i, $TAB + "<modify_xml_values type=""array"">"
            PRINT #i, STRING$(2, $TAB) + "<set_xml_value name=""load_file_names"">"
            PRINT #i, toSD
            PRINT #i, STRING$(2, $TAB) + "</set_xml_value>"
            PRINT #i, $TAB + "</modify_xml_values>"
            PRINT #i, ""
        END IF

        ' Change advanced options
        IF app.advanced THEN
            PRINT #i, $TAB + "<modify_xml_values type=""color"">"
            PRINT #i, STRING$(2, $TAB) + "<set_xml_value name=""splash_color"" value=" + $DQ + "#ff" + LCASE$(app.splashbgndcolor) + $DQ + " />"
            PRINT #i, $TAB + "</modify_xml_values>"

            PRINT #i, $TAB + "<modify_xml_values type=""integer"">"
            PRINT #i, STRING$(2, $TAB) + "<set_xml_value name=""color1"" value=" + $DQ + (console.clearcolor) + $DQ + " />"
            PRINT #i, STRING$(2, $TAB) + "<set_xml_value name=""color2"" value=" + $DQ + (console.fontcolor) + $DQ + " />"
            PRINT #i, STRING$(2, $TAB) + "<set_xml_value name=""color3"" value=" + $DQ + (console.backcolor) + $DQ + " />"
            PRINT #i, STRING$(2, $TAB) + "<set_xml_value name=""splash_time"" value=" + $DQ + TRIM$(app.splashtimer) + $DQ + " />"
            PRINT #i, $TAB + "</modify_xml_values>"

            PRINT #i, $TAB + "<modify_xml_values type=""array"">"
            PRINT #i, STRING$(2, $TAB) + "<set_xml_value name=""loading_msg"">  <!-- leave empty if you want no progress popup -->"
            IF app.splashprogress THEN PRINT #i, STRING$(3, $TAB) + "<item>" + app.startupmsg + "</item>"
            PRINT #i, STRING$(2, $TAB) + "</set_xml_value>"
            PRINT #i, $TAB + "</modify_xml_values>"
            PRINT #i, ""

            PRINT #i, $TAB + "<!-- Change advanced options -->"
            PRINT #i, $TAB + "<modify_xml source=""res/xml/settings.xml"">"
            PRINT #i, STRING$(2, $TAB) + "<set_attribute_value tag='android:key=""empty_color_pref""' attribute=""android:defaultValue"" value=""line"" />" ' console background color
            e = JavaTrueFalse(console.uselines)
            PRINT #i, STRING$(2, $TAB) + "<set_attribute_value tag='android:key=""lined_console""' attribute=""android:defaultValue"" value=" + $DQ + e + $DQ + " />" ' console underlines
            IF console.fontsize = 1 THEN e = "Small" ELSE IF console.fontsize = 2 THEN e = "Medium" ELSE e = "Large"
            PRINT #i, STRING$(2, $TAB) + "<set_attribute_value tag='android:key=""font_pref""' attribute=""android:defaultValue"" value=" + $DQ + e + $DQ + " />" ' console font size
            IF console.fonttype = 1 THEN e = "MS" ELSE IF console.fonttype = 2 THEN e = "SS" ELSE e = "S" ' Monospace/Sans-Serif/Serif
            PRINT #i, STRING$(2, $TAB) + "<set_attribute_value tag='android:key=""csf_pref""' attribute=""android:defaultValue"" value="  + $DQ + e + $DQ + " />" ' console font type
            e = TRIM$(console.screenorientation - 1)
            PRINT #i, STRING$(2, $TAB) + "<set_attribute_value tag='android:key=""so_pref""' attribute=""android:defaultValue"" value="  + $DQ + e + $DQ + " />" ' screen orientation
            PRINT #i, STRING$(2, $TAB) + "<set_attribute_value tag='android:key=""es_pref""' attribute=""android:defaultValue"" value=""WBL"" />" ' scr colors = font + text bgnd + empty zone
            e = JavaTrueFalse(app.hardwareaccel)
            PRINT #i, STRING$(2, $TAB) + "<set_attribute_value tag='android:key=""gr_accel""' attribute=""android:defaultValue"" value=" + $DQ + e + $DQ + " />"
            PRINT #i, $TAB + "</modify_xml>"
            PRINT #i, ""
        END IF

        ' Deactivate console menu option by default
        PRINT #i, $TAB + "<!-- Deactivate console menu option by default -->"
        PRINT #i, $TAB + "<modify_xml source=""res/xml/settings.xml"">"
        PRINT #i, STRING$(2, $TAB) + "<set_attribute_value tag='android:key=""console_menu""' attribute=""android:defaultValue"" value=""false"" />"
        PRINT #i, $TAB + "</modify_xml>"
        PRINT #i, ""

        ' Modify AndroidManifest.xml
        PRINT #i, $TAB + "<!-- Modify AndroidManifest.xml -->"
        PRINT #i, $TAB + "<modify_manifest>"
        PRINT #i, STRING$(2, $TAB) + "<set_attribute_value tag=""manifest"" attribute=""android:versionName"" value=" + $DQ + (app.version) + $DQ + " />"
        PRINT #i, STRING$(2, $TAB) + "<set_attribute_value tag=""manifest"" attribute=""android:versionCode"" value=" + $DQ + (app.vcode) + $DQ + " />"
        PRINT #i, STRING$(2, $TAB) + "<!-- Permissions -->"
        PRINT #i, STRING$(2, $TAB) + "<reset_permissions />"
        FOR lRes = 0 TO %PERM_NB
            IF app.permission(lRes) = 1 THEN PRINT #i, STRING$(2, $TAB) + "<add_permission name=" + $DQ + UCASE$(Permission(lRes)) + $DQ + " />"
        NEXT
        IF app.advanced AND app.startatboot = 1 THEN PRINT #i, STRING$(2, $TAB) + "<add_permission name=""RECEIVE_BOOT_COMPLETED"" />"
        PRINT #i, STRING$(2, $TAB) + "<!-- Unregister .bas file extension -->"
        PRINT #i, STRING$(2, $TAB) + "<remove_tag tag=""intent-filter"" contains='android:pathPattern="".*\\.bas""' />"
        PRINT #i, STRING$(2, $TAB) + "<!-- Unregister launcher shortcut -->"
        PRINT #i, STRING$(2, $TAB) + "<remove_tag tag=""activity"" contains='android:name=""LauncherShortcuts' />"
        PRINT #i, STRING$(2, $TAB) + "<remove_tag tag=""activity-alias"" contains='android:name=""CreateShortcuts' />"
        IF app.advanced THEN
            PRINT #i, STRING$(2, $TAB) + "<!-- Manifest advanced settings -->"
            PRINT #i, STRING$(2, $TAB) + "<set_attribute_value tag='receiver android:name="".BootUpReceiver""' attribute=""android:enabled"" value=" + $DQ + JavaTrueFalse(app.startatboot) + $DQ + " />"
            'PRINT #i, STRING$(2, $TAB) + "<set_attribute_value tag='application' attribute=""android:hardwareAccelerated"" value=" + $DQ + JavaTrueFalse(app.hardwareaccel) + $DQ + " />"
        END IF
        PRINT #i, $TAB + "</modify_manifest>"
        PRINT #i, ""

        PRINT #i, $TAB + "<!-- Manually change package name in *.smali files (=decompressed Java classes) and manifest -->"
        PRINT #i, $TAB + "<change_package old=""com.rfo.basic"" new=" + $DQ + (app.package) + $DQ + " />"
        PRINT #i, ""

        IF app.keystore <> "" THEN ' Sign with custom keystore
            PRINT #i, $TAB + "<!-- Sign the resulting APK with custom certificate -->"
            PRINT #i, $TAB + "<sign_with certificate=" + $DQ + (app.keystore) + $DQ + " password=" + $DQ + (app.KSpwd) + $DQ + " />"
            PRINT #i, ""
        END IF

        PRINT #i, "</use_base_apk>"
    CLOSE #i

END SUB
'-----------------------------------------------------------------------------------------------------

'-----------------------------------------------------------------------------------------------------
SUB SaveProjectAsRFO (proj AS STRING)
    LOCAL ff, resDim AS LONG
    LOCAL pjname AS STRING
    LOCAL projrev AS BYTE

    projrev = %RFO_REV
    pjname = proj : IF RIGHT$(LCASE$(pjname), 4) <> ".rfo" THEN pjname += ".rfo"
    resDim = UBOUND(app_res)

    KILL pjname
    ff = FREEFILE
    OPEN pjname FOR BINARY AS #ff
        PUT #ff,, ConnectMode ' Long = 4 bytes (32 bits)
        PUT #ff,, resDim      ' Long = 4 bytes (32 bits)
        IF resDim > 0 THEN
            PUT #ff,, app_res()   ' Array of dynamic strings
        END IF
        PUT #ff,, projrev     ' Byte
        PUT #ff,, app         ' whole app UDT
        PUT #ff,, console     ' whole console UDT
    CLOSE #ff
END SUB
'-----------------------------------------------------------------------------------------------------

'-----------------------------------------------------------------------------------------------------
FUNCTION LoadRFOproject (proj AS STRING) AS LONG
    LOCAL ff, resDim, cursize AS LONG
    LOCAL pjname AS STRING
    LOCAL projrev AS BYTE

    RESET_APP
    pjname = proj : IF RIGHT$(LCASE$(pjname), 4) <> ".rfo" THEN pjname += ".rfo"
    setperms = 1

    ff = FREEFILE
    OPEN pjname FOR BINARY AS #ff
        cursize = SIZEOF(ConnectMode) + SIZEOF(resDim)
        IF LOF(#ff) < cursize THEN CLOSE #ff : EXIT FUNCTION ' Illegal project file: exit with code 0
        GET #ff,, ConnectMode ' Long = 4 bytes (32 bits)
        GET #ff,, resDim      ' Long = 4 bytes (32 bits)

        IF resDim > 0 THEN
            cursize += resDim
            IF LOF(#ff) < cursize THEN CLOSE #ff : EXIT FUNCTION ' Illegal project file: exit with code 0
            REDIM app_res(1 TO resDim)
            GET #ff,, app_res()   ' Array of dynamic strings
        END IF

        cursize += SIZEOF(projrev)
        IF LOF(#ff) < cursize THEN CLOSE #ff : EXIT FUNCTION ' Illegal project file: exit with code 0
        GET #ff,, projrev     ' Byte = 1 byte (8 bits)

        IF projrev = 3 THEN         ' Latest project format (up-to-date)
            cursize += SIZEOF(BASICProject_V3) + SIZEOF(ConsoleDescriptor_V1)
            IF LOF(#ff) < cursize THEN CLOSE #ff : EXIT FUNCTION ' Illegal project file: exit with code 0
            GET #ff,, app           ' whole app UDT
            GET #ff,, console       ' whole console UDT
        ELSEIF projrev = 2 THEN     ' Handle project format legacy
            cursize += SIZEOF(BASICProject_V2) + SIZEOF(ConsoleDescriptor_V1)
            IF LOF(#ff) < cursize THEN CLOSE #ff : EXIT FUNCTION ' Illegal project file: exit with code 0
            GET #ff,, app           ' whole app UDT
            app.splashtimer = 1500  ' Rectify last 'Long' value (4 bytes)
            resDim = SEEK(#ff) - 4  ' Set back last 'Long' offset (4 bytes)
            GET #ff, resDim, console ' whole console UDT
        ELSEIF projrev = 1 THEN     ' Handle project format legacy
            cursize += SIZEOF(BASICProject_V1) + SIZEOF(ConsoleDescriptor_V1)
            IF LOF(#ff) < cursize THEN CLOSE #ff : EXIT FUNCTION ' Illegal project file: exit with code 0
            GET #ff,, app           ' whole app UDT
            app.splashtimer = 1500  ' Rectify 2 last 'Long' values (2x4 bytes)
            app.encryptbas = 0
            resDim = SEEK(#ff) - 8  ' Set back 2 last 'Long' offsets (2x4 bytes)
            GET #ff, resDim, console ' whole console UDT
        END IF
    CLOSE #ff

    FUNCTION = 1 ' Loading ok: exit with code 1
END FUNCTION
'-----------------------------------------------------------------------------------------------------
