#COMPILE EXE "updater.exe"
#DIM ALL

#RESOURCE BITMAP LOADING00, "res\updater\loader00.bmp"
#RESOURCE BITMAP LOADING01, "res\updater\loader01.bmp"
#RESOURCE BITMAP LOADING02, "res\updater\loader02.bmp"
#RESOURCE BITMAP LOADING03, "res\updater\loader03.bmp"
#RESOURCE BITMAP LOADING04, "res\updater\loader04.bmp"
#RESOURCE BITMAP LOADING05, "res\updater\loader05.bmp"
#RESOURCE BITMAP LOADING06, "res\updater\loader06.bmp"
#RESOURCE BITMAP LOADING07, "res\updater\loader07.bmp"

%UPDATER = 1
$EXE     = "RFO-BASIC! Quick APK Updater"
$TGT     = "rfo-basic-quick-apk"
$WEB     = "http://mougino.free.fr/RFO/quick-apk/"

'--------------------------------------------------------------------------------
'   ** Includes **
'--------------------------------------------------------------------------------
#INCLUDE ONCE "inc\Windows.inc"
#INCLUDE ONCE "inc\GDIPlus.inc"
#INCLUDE ONCE "inc\utils.inc"
#INCLUDE ONCE "inc\MyMsgBox.inc"
#INCLUDE ONCE "inc\rfo-ui.inc"
#INCLUDE ONCE "inc\MD5.inc"
'------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
'   ** Constants **
'--------------------------------------------------------------------------------
%Port                   = 80
%ID_TIMER_MAIN          = %WM_USER + 400
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
'   ** Global **
'--------------------------------------------------------------------------------
GLOBAL exepath, DL_RemoteLabel AS STRING
GLOBAL DownloadInProgress AS LONG
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
MACRO MYSERVER = $WEB + IIF$(LCASE$(COMMAND$) = "-testupdate", "test/", "")
'--------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB INIT_PROGRESS_DL
    LOCAL w, h AS LONG
    GRAPHIC ATTACH hDlg, 1001, REDRAW
    GRAPHIC BOX (343, 183)-(510, 276), 10, %WHITE, %WHITE
    GRAPHIC SET FONT hFontLbl
    GRAPHIC COLOR RGB(7,99,164), %WHITE
    GRAPHIC TEXT SIZE DL_RemoteLabel TO w, h
    GRAPHIC SET POS (426 - w\2, 187)
    GRAPHIC PRINT DL_RemoteLabel
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
MACRO AddToDownload (localFile, fileSize, fileMD5)
    ff = UBOUND(DL_File)
    INCR ff
    REDIM PRESERVE DL_File(ff)
    REDIM PRESERVE DL_Size(ff)
    REDIM PRESERVE DL_MD5(ff)
    DL_File(ff) = localFile
    DL_Size(ff) = fileSize
    DL_MD5(ff)  = fileMD5
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO DecideIfDownload (localFile, fileSize, fileMD5)
    IF EXIST(exepath & localFile) THEN
        ff = FREEFILE
        OPEN exepath & localFile FOR BINARY AS #ff
        GET$ #ff, LOF(#ff), TCPBuffer
        CLOSE #ff
        IF MD5_Checksum(TCPBuffer) <> fileMD5 THEN ' Update component only if MD5 differs
            AddToDownload (localFile, fileSize, fileMD5)
        END IF
    ELSE
        AddToDownload (localFile, fileSize, fileMD5)
    END IF
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
FUNCTION MD5_Checksum(BYVAL buffer AS STRING) AS STRING
    LOCAL md5c AS MD5_CTX
    md5_Init VARPTR(md5c)
    md5_Update VARPTR(md5c), STRPTR(buffer), LEN(buffer)
    FUNCTION = md5_Digest(VARPTR(md5c))
END FUNCTION
'------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
'   ** Thread Functions **
'--------------------------------------------------------------------------------
THREAD FUNCTION DownloadNewVersion(BYVAL hDlg AS DWORD) AS LONG

    LOCAL httpTime AS SINGLE
    LOCAL i, DL_Size() AS LONG
    LOCAL t, TCPBuffer, DL_MD5(), DL_File() AS STRING
    LOCAL ff AS INTEGER

    ' Get path of the exe we are trying to update
    IF NOT EXIST(EXE.PATH$ + $TGT + "-updater.nfo") THEN
        FUNCTION = -1
        EXIT FUNCTION
    END IF
    ff = FREEFILE
    OPEN EXE.PATH$ + $TGT + "-updater.nfo" FOR INPUT AS #ff
    LINE INPUT #ff, exepath
    CLOSE #ff
    KILL EXE.PATH$ + $TGT + "-updater.nfo"
    IF NOT EXIST(exepath + $TGT + ".exe") THEN
        FUNCTION = -1
        EXIT FUNCTION
    END IF

    ' Download data from server 'lastversion.dat'
    DownloadInProgress = 0
    httpTime = HttpGet(MYSERVER + "lastversion.dat", TCPBuffer) ' no size specified, so no INIT_PROGRESS_DL
    ' Content of lastversion.dat = "(1)v0.0;(2)mandatory;(3)updtr_size;(4)updtr_MD5"
    SLEEP 3000
    DownloadInProgress = -1
    IF httpTime = -1 THEN
        FUNCTION = -1
        EXIT FUNCTION
    END IF

    ' Check components that need to be downloaded
    DIM DL_File(0), DL_Size(0), DL_MD5(0)
    ' New signapk.bat & Co to handle custom signing
    DecideIfDownload ("tools\signapk.bat", 89, "09CC52D3D6E7E0C7851694A9E3045A52")
    DecideIfDownload ("tools\keytool.exe", 15752, "55CCE9529C1FAF06DA72C74D7282B244")
    DecideIfDownload ("tools\jli.dll", 142728, "F65E59F92F2E071E9B81D22D4F9F93E4")
    DecideIfDownload ("tools\openssl.exe", 393728, "D7952814F94B5282D6E26D415B803007")
    DecideIfDownload ("tools\ssleay32.dll", 270336, "A72887AB04FF5BB2FEC3E4405D2B351B")
    DecideIfDownload ("tools\libeay32.dll", 1176576, "6CDD11FD37ACB4A9A3E42C26A928E889")
    DecideIfDownload ("tools\splitpem.exe", 19968, "FA60DDF4B4D78C8C104393C4097AE6D5")
    ' New Android Debug Bridge from 2014-03-12 release
    DecideIfDownload ("tools\AdbWinApi.dll", 96256, "47A6EE3F186B2C2F5057028906BAC0C6")
    DecideIfDownload ("tools\AdbWinUsbApi.dll", 60928, "5F23F2F936BDFAC90BB0A4970AD365CF")
    DecideIfDownload ("tools\adb.exe", 819200, "81D188A849C8768E8F3694EB1C0E6086")
    ' Android Asset Packaging Tool taken from Android SDK Build-tools 19.0.3
    DecideIfDownload ("tools\aapt.exe", 852992, "E9EA7FAC5D5E1B9DF6441F0C8D7D05FE")
    ' New easyapk tool
    DecideIfDownload ("tools\easyapk.exe", 150016, "C6D18AE2F7DCCE0B1B262A50F199B9FB")
    ' New 'convert' tool (easyapk dependency)
    DecideIfDownload ("tools\convert.exe", 50688, "ED4B84C49FFCDF53F6C055FA235B4B79")
    ' New 'pbe_md5_des.jar' tool (easyapk dependency)
    DecideIfDownload ("tools\pbe_md5_des.jar", 2071, "C3A9E216F65EDE2912289D2BC51033A4")
    DecideIfDownload ("tools\pbe_md5_des.bat", 93, "8FFD5FD5C6827ABB07FB2D51D52FB78A")
    ' New GW-lib default theme
    DecideIfDownload ("tools\basic.js", 215, "3D62DBC1A6343DFAE18D44E452BF6411")
    DecideIfDownload ("tools\jquery-2.1.1.min.js", 84245, "E40EC2161FE7993196F23C8A07346306")
    DecideIfDownload ("tools\jquery.mobile-1.4.5.min.css", 207465, "B835B04BBFF5A8020C31CE21714E389B")
    DecideIfDownload ("tools\jquery.mobile-1.4.5.min.js", 200143, "39EE6F20751F4FB0653862AE56F9CBBA")
    DecideIfDownload ("tools\styles.css", 85, "C6AC027F2AC820F1189F307AD9A4518E")
    ' Last BASIC! customized and built with API 19+
    DecideIfDownload ("tools\Basic.apk", 417852, "EA0E83866B58C1D5CF08A1C123CF915C")
    ' Quick APK last version
    DecideIfDownload ("rfo-basic-quick-apk.exe", 1456128, "57C0776E8ACF2F89B5AAB8CCFCCB6D82")

    ' Download components, one by one
    FOR i = 1 TO UBOUND(DL_File)
        t = PARSE$(DL_File(i), "\", PARSECOUNT(DL_File(i), "\"))
        REPLACE $SPC WITH "%20" IN t
        DL_RemoteLabel = "[" + TRIM$(i) + "/" + TRIM$(UBOUND(DL_File)) + "] " + LEFT$(t, 14)
        httpTime = HttpGet(MYSERVER & t, TCPBuffer, DL_Size(i))
        IF httpTime = -1 THEN
            FUNCTION = -1
            EXIT FUNCTION
        END IF
        ' Calculate checksum
        IF MD5_Checksum(TCPBuffer) <> DL_MD5(i) THEN
            FUNCTION = -2
            EXIT FUNCTION
        ELSE
            t = exepath & DL_File(i)
            IF EXIST(t) THEN KILL t
            ff = FREEFILE
            OPEN t FOR BINARY AS #ff
            PUT$ #ff, TCPBuffer
            CLOSE #ff
        END IF
    NEXT

    FUNCTION = 1 ' Everything went ok, send signal to close thread function

END FUNCTION
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
'   ** CallBacks **
'--------------------------------------------------------------------------------
CALLBACK FUNCTION ProcMainDialog()

    LOCAL pt AS POINTAPI
    LOCAL lRes, lRslt AS LONG
    STATIC idEvent AS LONG
    STATIC hThread AS DWORD
    STATIC Cnt AS INTEGER

    SELECT CASE CB.MSG

        CASE %WM_INITDIALOG
            CONTROL ADD GRAPHIC, CB.HNDL, 1001, "", 0, 0, 640, 400
            GRAPHIC ATTACH CB.HNDL, 1001, REDRAW
            GRAPHIC RENDER BITMAP "SPLASH", (0, 0) - (640, 400)
            GRAPHIC BOX (343, 183)-(510, 276), 10, %WHITE, %WHITE
            GRAPHIC REDRAW
            SetWindowPos CB.HNDL, %HWND_TOPMOST, 0, 0, 0, 0, %SWP_NOMOVE OR %SWP_NOSIZE OR %SWP_SHOWWINDOW
            SetForegroundWindow CB.HNDL
            SetFocus CB.HNDL
            idEvent = SetTimer(CB.HNDL, %ID_TIMER_MAIN, 150, BYVAL %NULL)
            DIALOG POST CB.HNDL, %WM_TIMER, %ID_TIMER_MAIN, 0
            THREAD CREATE DownloadNewVersion(CB.HNDL) TO hThread

        CASE %WM_LBUTTONDOWN ' handle dialog drag-and-drop, or a clic on "close" button
            GetCursorPos pt
            ScreenToClient CB.HNDL, pt
            IF CB.WPARAM = %MK_LBUTTON THEN SendMessage CB.HNDL, %WM_NCLBUTTONDOWN, %HTCaption, BYVAL %Null  ' force drag

        CASE %WM_TIMER
          IF CB.WPARAM = %ID_TIMER_MAIN THEN
              IF NOT DownloadInProgress THEN ' Attempting to initially connect to server
                GRAPHIC RENDER BITMAP "LOADING" & FORMAT$(Cnt, "00"), (418, 225) - (418+16, 225+11)
                GRAPHIC REDRAW
                Cnt = (Cnt + 1) MOD 8
              END IF ' Whether we are in initial connect or the download of new version is in progress
              THREAD STATUS hThread TO lRes
              IF lRes <> 0 AND lRes <> &H103 THEN ' Thread function ended
                  THREAD CLOSE hThread TO lRslt
                  KillTimer CB.HNDL, idEvent
                  GRAPHIC BOX (343, 183)-(510, 276), 10, %WHITE, %WHITE
                  IF lRes < 0 THEN ' connection error or checksum error
                      GRAPHIC RENDER BITMAP "WRONG", (389, 185) - (389+90, 185+90)
                  ELSE
                      GRAPHIC RENDER BITMAP "GOOD", (364, 185) - (364+140, 185+90)
                  END IF
                  GRAPHIC REDRAW
                  PAUSE 1500
                  DIALOG END CB.HNDL
              END IF
            END IF

      CASE %WM_DESTROY
        CLOSE
        THREAD SUSPEND hThread TO lRes
        THREAD CLOSE hThread TO lRes
        IF idEvent THEN KillTimer CB.HNDL, idEvent
        ShellExecute BYVAL 0&, "open", exepath & $TGT & ".exe" & CHR$(0), BYVAL 0&, BYVAL 0&, %SW_SHOW

    END SELECT

END FUNCTION
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
FUNCTION PBMAIN () AS LONG

    ' Initialize font of message boxes
    SetMyMsgBoxFont "Lucida Console", 10, %BLACK, %WHITE

    ' Create main dialog
    INIT_DIALOG (640, 400, $EXE)

END FUNCTION
'--------------------------------------------------------------------------------
