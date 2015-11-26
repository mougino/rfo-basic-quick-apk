'##############################################################################
'#
'#  This is one of the include files for << RFO-BASIC! Quick APK >>
'#
'#  "rfo-advanced.inc" contains Functions and Subs for apps advanced options
'#
'##############################################################################

TYPE CustomColors
    c(15) AS LONG
END TYPE

GLOBAL setperms AS LONG

'------------------------------------------------------------------------------
FUNCTION JavaTrueFalse(value AS LONG) AS STRING
    IF value THEN FUNCTION = "true" ELSE FUNCTION = "false"
END FUNCTION
'------------------------------------------------------------------------------

'-----------------------------------------------------------------------------------------------------
FUNCTION JavaColor(PBcol AS LONG) AS STRING
    LOCAL e AS STRING
    e = LCASE$(HEX$(PBcol, 6))
    e = MID$(e, 5, 2) + MID$(e, 3, 2) + LEFT$(e, 2)
    FUNCTION = "0xff" + e
END FUNCTION
'-----------------------------------------------------------------------------------------------------

'-----------------------------------------------------------------------------------------------------
FUNCTION HtmlColor(PBcol AS LONG) AS STRING
    LOCAL e AS STRING
    e = LCASE$(HEX$(PBcol, 6))
    e = MID$(e, 5, 2) + MID$(e, 3, 2) + LEFT$(e, 2)
    FUNCTION = e
END FUNCTION
'-----------------------------------------------------------------------------------------------------

'-----------------------------------------------------------------------------------------------------
FUNCTION PBColor(JavaCol AS STRING) AS LONG
    LOCAL e AS STRING
    IF JavaCol = "" THEN FUNCTION = 0 : EXIT FUNCTION
    e = REMOVE$(JavaCol, "0xff")
    e = MID$(e, 5, 2) + MID$(e, 3, 2) + LEFT$(e, 2)
    FUNCTION = VAL("&H" + UCASE$(e))
END FUNCTION
'-----------------------------------------------------------------------------------------------------

'------------------------------------------------------------------------------
FUNCTION VerCod(verName AS STRING) AS STRING
    LOCAL e AS STRING
    e = REMOVE$(verName, ".")
    IF LEN(e) < 4 THEN e = STRING$(4 - LEN(e), "0") + e
    FUNCTION = e
END FUNCTION
'------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB DefineDefaultAppPackage ' Define default app package
    LOCAL apppackage AS STRING

    apppackage = REMOVE$(app.name, ANY " :!?./\-_'&~""#'{([|`^@)]°=+}$£¤%*µ")
    REPLACE ANY "àâä"  WITH "aaa"  IN apppackage
    REPLACE ANY "éèêë" WITH "eeee" IN apppackage
    REPLACE ANY "îï"   WITH "ii"   IN apppackage
    REPLACE ANY "ôö"   WITH "oo"   IN apppackage
    REPLACE ANY "ùûü"  WITH "uuu"  IN apppackage
    REPLACE ANY "ÿ"    WITH "y"    IN apppackage
    REPLACE ANY "ñ"    WITH "n"    IN apppackage

    app.package = "com.rfo." + apppackage + $NUL
END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB CheckIfCreateDataDir ' Set app.createdatadir and app.createdatabasedir according to content of program
    UW_DeclareLocals     ' Local variables for Unix/Windows file-access macros

    UW_GetRawFile(APKfolder + "rfo-basic\source\" + LinuxName(app.bas))
    uwBuff = LCASE$(uwBuff)

    IF INSTR(uwBuff, "text.write") <> 0 _
    OR INSTR(uwBuff, "byte.write") <> 0 _
    OR INSTR(uwBuff, "byte.copy") <> 0 _
    OR INSTR(uwBuff, "console.save") <> 0 _
    OR INSTR(uwBuff, "gr.camera.autoshoot") <> 0 _
       THEN app.createdatadir = %TRUE

    IF INSTR(uwBuff, "sql.") <> 0 _
       THEN app.createdatabasedir = %TRUE

END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
FUNCTION PermissionStr(idx AS LONG) AS STRING
    LOCAL s AS STRING

    IF idx > %PERM_NB THEN EXIT FUNCTION  ' security

    s = LCASE$(Permission(idx))
    s = UCASE$(LEFT$(s,1)) + MID$(s, 2)
    REPLACE "_" WITH $SPC IN s
    IF TALLY(s, $SPC) >= 3 THEN s = LEFT$(s, INSTR(-1, s, $SPC))

    FUNCTION = s

END FUNCTION
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
'FUNCTION PermissionXml(idx AS LONG) AS STRING
'
'    LOCAL s1, s2f, s2 AS STRING
'    s1  = "<uses-permission android:name=""android.permission."
'    s2f = $DQ + " android:required=""false""></uses-permission>"
'    s2  = $DQ + " />"
'
'    IF idx > %PERM_NB THEN EXIT FUNCTION  ' security
'
'    IF idx = 0  _         ' WRITE_EXTERNAL_STORAGE
'        OR idx = 17 _     ' ACCESS_NETWORK_STATE
'        OR idx = 18 _     ' DUMP
'        THEN
'        FUNCTION = s1 + Permission(idx) + s2
'    ELSE
'        FUNCTION = s1 + Permission(idx) + s2f
'    END IF
'
'END FUNCTION
'--------------------------------------------------------------------------------

SUB Copy2DataAdd2Res(file AS STRING)
    LOCAL i AS LONG

    ARRAY SCAN app_res(), COLLATE UCASE, = "1rfo-basic/data/" + file, TO i
    IF i > 0 THEN EXIT SUB ' Resource already in app and copied to SD -> we're done!

    ARRAY SCAN app_res(), COLLATE UCASE, = "1" + (app.path) + "/data/" + file, TO i
    IF i > 0 THEN EXIT SUB ' [Alternative] Resource already in app and copied to SD -> we're done!

    ARRAY SCAN app_res(), COLLATE UCASE, = "0rfo-basic/data/" + file, TO i
    IF i > 0 THEN app_res(i) = "1rfo-basic/data/" + file ' Resource already in app, but not copied to SD -> activate copy to SD

    ARRAY SCAN app_res(), COLLATE UCASE, = "0" + (app.path) + "/data/" + file, TO i
    IF i > 0 THEN app_res(i) = "1" + (app.path) + "/data/" + file ' [Alternative] Resource already in app, but not copied to SD -> activate copy to SD

    IF EXIST(APKfolder + "rfo-basic\data\" + file) THEN
        REDIM PRESERVE app_res(1 TO UBOUND(app_res) - LBOUND(app_res) + 2) ' Resource already in APKfolder but not selected as part of app -> add it
        app_res(UBOUND(app_res)) = "1rfo-basic/data/" + file

    ELSEIF EXIST(APKfolder + (app.path) + "\data\" + file) THEN
        REDIM PRESERVE app_res(1 TO UBOUND(app_res) - LBOUND(app_res) + 2) ' [Alternative] Resource already in APKfolder but not selected as part of app -> add it
        app_res(UBOUND(app_res)) = "1" + (app.path) + "/data/" + file

    ELSE
        FILECOPY EXE.PATH$ + "tools\" + file, APKfolder + "rfo-basic\data\" + file ' Resource not in APKfolder -> copy it and add it to app
        REDIM PRESERVE app_res(1 TO UBOUND(app_res) - LBOUND(app_res) + 2)
        app_res(UBOUND(app_res)) = "1rfo-basic/data/" + file
    END IF

END SUB

'------------------------------------------------------------------------------
SUB AutodetectGwLib(basfile AS STRING)
    LOCAL e AS STRING
    LOCAL i, j AS LONG

    i = FREEFILE
    OPEN basfile FOR BINARY AS #i
        GET$ #i, LOF(#i), e
    CLOSE #i

    i = INSTR(LCASE$(e), " gw.bas")
    IF i = 0 THEN i = INSTR(LCASE$(e), $DQ + "gw.bas")
    IF i = 0 THEN EXIT SUB ' Doesn't make use of GW lib

    i = INSTR(i - LEN(e), LCASE$(e), "include")
    j = INSTR(i, e, ANY $CRLF)
    IF i = 0 OR j = 0 THEN EXIT SUB ' Not an "INCLUDE GW.bas"

    ' Make sure to protect GW.bas in double quotes in the INCLUDE statement
    REPLACE MID$(e, i, j-i) WITH "INCLUDE " + $DQ + "GW.bas" + $DQ IN e
    KILL basfile
    i = FREEFILE
    OPEN basfile FOR BINARY AS #i
        PUT$ #i, e
    CLOSE #i

    ' Copy GW default theme (5 files) to APKfolder/rfo-basic/data with "copy to SD" activated
    Copy2DataAdd2Res "basic.js"
    Copy2DataAdd2Res "jquery-2.1.1.min.js"
    Copy2DataAdd2Res "jquery.mobile-1.4.5.min.css"
    Copy2DataAdd2Res "jquery.mobile-1.4.5.min.js"
    Copy2DataAdd2Res "styles.css"
END SUB
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB AutosetPermissions(basfile AS STRING)
    LOCAL e AS STRING
    LOCAL i AS LONG

    IF LEN(basfile) > 0 THEN
        IF EXIST(basfile) THEN
            i = FREEFILE
            OPEN basfile FOR BINARY AS #i
                GET$ #i, LOF(#i), e
            CLOSE #i
            e = UCASE$(e)
        END IF
        setperms = 1
    ELSE
        setperms = 0
    END IF

    ' Supress in the program all tabulations and spaces that start a line
    REPLACE $CRLF WITH $LF IN e
    REPLACE ":" WITH $LF IN e
    WHILE INSTR(e, $LF + $TAB)<>0 : REPLACE $LF + $TAB WITH $LF IN e : WEND
    WHILE INSTR(e, $LF + $SPC)<>0 : REPLACE $LF + $SPC WITH $LF IN e : WEND

    ' Initialize (reset) permissions
    FOR i = 0 TO %PERM_NB
        app.permission(i) = 0
    NEXT

    '[0.WRITE_EXTERNAL_STORAGE] always set by default
    app.permission(0) = 1
    '[1.INTERNET]
    IF INSTR(e, $LF + "STT.")<>0 OR INSTR(e, $LF + "TTS.")<>0 OR INSTR(e, $LF + "EMAIL.SEND")<>0 _
      OR INSTR(e, $LF + "FTP.")<>0 OR INSTR(e, $LF + "GRABFILE")<>0 OR INSTR(e, $LF + "GRABURL")<>0 _
      OR INSTR(e, $LF + "HTTP.POST")<>0 OR INSTR(e, $LF + "SOCKET.")<>0 OR INSTR(e, $LF + "HTML.")<>0 THEN
        app.permission(1) = 1
    END IF
    '[2.3.4.5.ACCESS COARSE/MOCK/FINE/EXTRA LOCATION]
    IF INSTR(e, $LF + "GPS.")<>0 THEN
        app.permission(2) = 1
        app.permission(3) = 1
        app.permission(4) = 1
        app.permission(5) = 1
    END IF
    '[6.VIBRATE]
    IF INSTR(e, $LF + "VIBRATE")<>0 THEN
        app.permission(6) = 1
    END IF
    '[7.WAKE_LOCK]
    IF INSTR(e, $LF + "WAKELOCK")<>0 THEN
        app.permission(7) = 1
    END IF
    '[8.CAMERA]
    IF INSTR(e, $LF + "GR.CAMERA.")<>0 THEN
        app.permission(8) = 1
    END IF
    '[9.10.BLUETOOTH (+ADMIN)]
    IF INSTR(e, $LF + "BT.")<>0 OR INSTR(e, $LF + "ONBTREADREADY")<>0 THEN
        app.permission(9) = 1
        app.permission(10) = 1
    END IF
    '[11.RECORD_AUDIO]
    IF INSTR(e, $LF + "STT.")<>0 THEN
        app.permission(11) = 1
    END IF
    '[12.READ_PHONE_STATE]
    IF INSTR(e, $LF + "PHONE.")<>0 OR INSTR(e, $LF + "MYPHONENUMBER")<>0 _
      OR INSTR(e, "VERSION$")<>0 OR INSTR(e, $LF + "DEVICE")<>0 THEN
        app.permission(12) = 1
    END IF
    IF INSTR(e, $LF + "DEVICE")<>0 THEN
        app.permission(4)  = 1 '[4.ACCESS_FINE_LOCATION]
        app.permission(5)  = 1 '[5.ACCESS_EXTRA_LOCATION]
    END IF
    '[13.READ_SMS]
    IF INSTR(e, $LF + "SMS.READ")<>0 THEN
        app.permission(13) = 1
    END IF
    '[14.SEND_SMS]
    IF INSTR(e, $LF + "SMS.SEND")<>0 THEN
        app.permission(14) = 1
    END IF
    '[15.RECEIVE_SMS]
    IF INSTR(e, $LF + "SMS.RCV.")<>0 THEN
        app.permission(15) = 1
    END IF
    '[16.CALL_PHONE]
    IF INSTR(e, $LF + "PHONE.CALL")<>0 THEN
        app.permission(16) = 1
    END IF
    '[17.ACCESS_NETWORK_STATE] for AdMob
    '[18.DUMP] for system-command 'dumpsys'
    IF INSTR(e, $LF + "DUMPSYS")<>0 THEN
        app.permission(18) = 1
    END IF
    '[19.ACCESS_WIFI_STATE] for WiFi commands
    IF INSTR(e, $LF + "WIFI.INFO")<>0 THEN
        app.permission(19) = 1
    END IF

END SUB
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
FUNCTION UsrAndPwdCorrectForKeystore() AS LONG
    LOCAL e AS STRING
    KILL APPDATA + "tmpkeystore.jks"
    FILECOPY app.keystore, APPDATA + "tmpkeystore.jks"
    e  = "keytool -changealias -keystore tmpkeystore.jks" _
       + " -alias "     + app.KSusr + " -destalias appbuilderadmin" _
       + " -storepass " + app.KSpwd + " -keypass " + app.KSpwd
    e  = DUMP_CMD(e, APPDATA)
    '?"XpUsrAndPwdCorrectForKeystore returned"+$cr+$dq+e+$dq+"   length:"+trim$(len(e)),,"degub"
    KILL APPDATA + "tmpkeystore.jks"
    IF e = "" THEN FUNCTION = %TRUE ELSE FUNCTION = %FALSE
END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB SelectKeystore
    LOCAL lRes, goodconf AS LONG
    LOCAL e AS STRING

    DISPLAY OPENFILE hDlg, 0, 0, GET_LABEL(26), _ ' Select another keystore
      "", CHR$("All files", 0, "*.*", 0), "", "", %OFN_FILEMUSTEXIST TO e

    IF e = "" THEN ' cancelled by user
        app.keystore = $NUL
        COMBOBOX SELECT hDlg, 1013, 1 ' default keystore
        EXIT SUB
    END IF

    app.keystore = e + $NUL

    DO
        IF ShowKSConfDialog(hDlg, lRes) = 0 THEN ' cancelled by user
            app.keystore = $NUL
            COMBOBOX SELECT hDlg, 1013, 1 ' default keystore
            EXIT SUB
        END IF
        goodconf = UsrAndPwdCorrectForKeystore()
        lRes     = NOT goodconf
    LOOP UNTIL goodconf ' good user and password

END SUB
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
GLOBAL KSCommonName AS STRING
GLOBAL KSOrgName    AS STRING
GLOBAL KSOrgUnit    AS STRING
GLOBAL KSLocality   AS STRING
GLOBAL KSState      AS STRING
GLOBAL KSCountry    AS STRING
MACRO KSdefine (variable, value) = IF variable = "" THEN variable = value
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB CreateNewKeystore
    LOCAL e, r, ti AS STRING

    ti = GET_LABEL(12) ' Certificate:
    ti = RTRIM$(ti, ANY " :")

    DISPLAY SAVEFILE hDlg, 0, 0, "", "", CHR$("Java Keystore", 0, "*.jks", 0), app.name + ".jks", "", %OFN_CREATEPROMPT TO e

    IF e = "" THEN  ' Canceled by user
        app.keystore = $NUL
        COMBOBOX SELECT hDlg, 1013, 1 ' default keystore
        EXIT SUB
    END IF

    IF EXIST(e) THEN
        IF MyMsgBox(hDlg, e + $CR + GET_LABEL(59), _ ' A file with this name already exists. Replace ?
            ti, %MB_ICONQUESTION OR %MB_YESNOCANCEL) = 1 THEN ' Yes
            KILL e
        ELSE
            app.keystore = $NUL
            COMBOBOX SELECT hDlg, 1013, 1 ' default keystore
            EXIT SUB
        END IF
    END IF

    IF ShowKSConfDialog(hDlg) = 0 THEN  ' Canceled by user
        app.keystore = $NUL
        COMBOBOX SELECT hDlg, 1013, 1 ' default keystore
        EXIT SUB
    END IF

    KSdefine (KSCommonName, "Susan Jones")
    KSdefine (KSOrgName,    "ABCSystems Inc.")
    KSdefine (KSOrgUnit,    "Purchasing")
    KSdefine (KSLocality,   "Palo Alto")
    KSdefine (KSState,      "California")
    KSdefine (KSCountry,    "CH")

    IF ShowNewKSdetails(hDlg) = 0 THEN ' Canceled by user
        app.keystore = $NUL
        COMBOBOX SELECT hDlg, 1013, 1 ' default keystore
        EXIT SUB
    END IF

    r = DUMP_CMD("keytool -genkey -keyalg RSA -keystore " + WindowsName(e) _
                  + " -alias " + app.KSusr + " -storepass " + app.KSpwd _
                  + " -keypass " + app.KSpwd + " -validity 10000" _
                  + " -dname " + $DQ + "CN=" + KSCommonName _
                  + ", OU=" + KSOrgUnit + ", O=" + KSOrgName _
                  + ", L=" + KSLocality + ", S=" + KSState _
                  + ", C=" + KSCountry + $DQ, WindowsPath(e))

    IF EXIST(e) THEN
        app.keystore = e + $NUL
        COMBOBOX SELECT hDlg, 1013, 2 ' user keystore
        MyMsgBox hDlg, GET_LABEL(60), ti, %MB_ICONINFORMATION ' Your certificate has been successfuly created
    ELSE
        app.keystore = $NUL
        COMBOBOX SELECT hDlg, 1013, 1 ' default keystore
        MyMsgBox hDlg, GET_LABEL(61) + $CR + r, ti, %MB_ICONERROR ' There was a problem while attempting to create your certificate:
    END IF

END SUB
'------------------------------------------------------------------------------


%IDC_LABEL1     = 1001
%IDC_LABEL2     = 1002
%IDC_LABEL3     = 1003
%IDC_LABEL4     = 1004
%IDC_LABEL5     = 1005
%IDC_LABEL6     = 1006
%IDC_LABEL7     = 1007
%IDC_TEXTBOX1   = 1008
%IDC_TEXTBOX2   = 1009
%IDC_TEXTBOX3   = 1010
%IDC_TEXTBOX4   = 1011
%IDC_BUTTON1    = 1012
%IDC_BUTTON2    = 1013
%IDC_BUTTON3    = 1014
%IDC_BUTTON4    = 1015
%IDC_IMAGE1     = 1016
%IDC_IMAGE2     = 1017
%IDC_IMAGE3     = 1018
%IDC_IMAGE4     = 1019
%IDC_TRACKBAR   = 1020

'------------------------------------------------------------------------------
MACRO SET_COL_FNT_LBL(ctl, lbl)
    MACROTEMP e
    DIM e AS STRING
    e = GET_LABEL(lbl)
    IF e <> "" THEN CONTROL SET TEXT  hD, ctl, e
    CONTROL SET COLOR hD, ctl, %BLACK, %WHITE
    CONTROL SET FONT  hD, ctl, hFontLbl
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
FUNCTION ShowConsoleLnfDialog(BYVAL hParent AS DWORD) AS LONG
    LOCAL lRslt AS LONG
    LOCAL hD  AS DWORD
    LOCAL combo() AS STRING

    DIALOG NEW PIXELS, hParent, GET_LABEL(21), , , 500, 365, %WS_POPUP OR %WS_BORDER _
        OR %WS_DLGFRAME  OR %WS_SYSMENU OR %WS_CLIPSIBLINGS OR %WS_VISIBLE OR %DS_MODALFRAME _
        OR %DS_3DLOOK OR %DS_NOFAILCREATE OR %DS_SETFONT OR %WS_CAPTION, %WS_EX_WINDOWEDGE _
        OR %WS_EX_CONTROLPARENT OR %WS_EX_LEFT OR %WS_EX_LTRREADING OR %WS_EX_RIGHTSCROLLBAR, _
        TO hD
    DIALOG SET COLOR hD, %BLACK, %WHITE

    CONTROL ADD BUTTON,   hD, %IDC_BUTTON1, "Reset console to default look and feel", 8, 8, 484, 30
    SET_COL_FNT_LBL (%IDC_BUTTON1, 30)

    CONTROL ADD LABEL,    hD, %IDC_LABEL1, "Font Size", 15, 49, 234, 24
    SET_COL_FNT_LBL (%IDC_LABEL1, 31)
    DIM combo(3) : ARRAY ASSIGN combo() = GET_LABEL(32), GET_LABEL(33), GET_LABEL(34) ' "Small", "Medium", "Large"
    CONTROL ADD COMBOBOX, hD, %IDC_TEXTBOX1, combo(), 242, 47, 250, 100, %CBS_DROPDOWNLIST
    SET_COL_FNT_LBL (%IDC_TEXTBOX1, 0)
    COMBOBOX SELECT       hD, %IDC_TEXTBOX1, console.fontsize

    CONTROL ADD LABEL,    hD, %IDC_LABEL2, "Font Typeface", 15, 79, 234, 24
    SET_COL_FNT_LBL (%IDC_LABEL2, 35)
    RESET combo() : ARRAY ASSIGN combo() = "Monospace", "Sans Serif", "Serif"
    CONTROL ADD COMBOBOX, hD, %IDC_TEXTBOX2, combo(), 242, 77, 250, 100, %CBS_DROPDOWNLIST
    SET_COL_FNT_LBL (%IDC_TEXTBOX2, 0)
    COMBOBOX SELECT       hD, %IDC_TEXTBOX2, console.fonttype

    CONTROL ADD LABEL,    hD, %IDC_LABEL7, "Screen Orientation", 15, 112, 234, 24
    SET_COL_FNT_LBL (%IDC_LABEL7, 36)
    REDIM combo(5) : ARRAY ASSIGN combo() = GET_LABEL(37), GET_LABEL(38), GET_LABEL(39), GET_LABEL(40), GET_LABEL(41)
    ' "Variable By Sensors", "Fixed Landscape", "Fixed Reverse Landscape", "Fixed Portrait", "Fixed Reverse Portrait"
    CONTROL ADD COMBOBOX, hD, %IDC_TEXTBOX3, combo(), 242, 108, 250, 100, %CBS_DROPDOWNLIST
    SET_COL_FNT_LBL (%IDC_TEXTBOX3, 0)
    COMBOBOX SELECT       hD, %IDC_TEXTBOX3, console.screenorientation

    CONTROL ADD CHECKBOX, hD, %IDC_TEXTBOX4, "Use Lined Console", 16, 144, 225, 13
    SET_COL_FNT_LBL (%IDC_TEXTBOX4, 42)
    CONTROL SET CHECK     hD, %IDC_TEXTBOX4, console.uselines

    IF console.screenorientation = 2 OR console.screenorientation = 3 THEN
        CONTROL ADD GRAPHIC,  hD, %IDC_IMAGE3, "", 242, 180, 182, 112, _
            %WS_CHILD OR %WS_VISIBLE, %WS_EX_CLIENTEDGE
    ELSE
        CONTROL ADD GRAPHIC,  hD, %IDC_IMAGE3, "", 242, 160, 112, 152, _
            %WS_CHILD OR %WS_VISIBLE, %WS_EX_CLIENTEDGE
    END IF

    CONTROL ADD LABEL,    hD, %IDC_LABEL3, "Colors:", 15, 178, 95, 24
    SET_COL_FNT_LBL (%IDC_LABEL3, 43)

    CONTROL ADD LABEL,    hD, %IDC_LABEL4, "Font", 65, 210, 100, 24
    SET_COL_FNT_LBL (%IDC_LABEL4, 44)
    CONTROL ADD GRAPHIC,  hD, %IDC_IMAGE1, "", 35, 204, 26, 26, _
        %SS_NOTIFY OR %WS_CHILD OR %WS_VISIBLE OR %SS_SUNKEN, %WS_EX_CLIENTEDGE
    GRAPHIC ATTACH        hD, %IDC_IMAGE1
    GRAPHIC COLOR -1,     PBColor((console.fontcolor))
    GRAPHIC CLEAR

    CONTROL ADD LABEL,    hD, %IDC_LABEL5, "Background", 65, 250, 100, 24
    SET_COL_FNT_LBL (%IDC_LABEL5, 45)
    CONTROL ADD GRAPHIC,  hD, %IDC_IMAGE2, "", 35, 244, 26, 26, _
        %SS_NOTIFY OR %WS_CHILD OR %WS_VISIBLE OR %SS_SUNKEN, %WS_EX_CLIENTEDGE
    GRAPHIC ATTACH        hD, %IDC_IMAGE2
    GRAPHIC COLOR -1,     PBColor((console.backcolor))
    GRAPHIC CLEAR

    CONTROL ADD LABEL,    hD, %IDC_LABEL6, "Empty", 65, 290, 100, 24
    SET_COL_FNT_LBL (%IDC_LABEL6, 46)
    CONTROL ADD GRAPHIC,  hD, %IDC_IMAGE4, "", 35, 284, 26, 26, _
        %SS_NOTIFY OR %WS_CHILD OR %WS_VISIBLE OR %SS_SUNKEN, %WS_EX_CLIENTEDGE
    GRAPHIC ATTACH        hD, %IDC_IMAGE4
    GRAPHIC COLOR -1,     PBColor((console.clearcolor))
    GRAPHIC CLEAR

    CONTROL ADD BUTTON,   hD, %IDC_BUTTON3, "Cancel", 316, 327, 80, 30
    SET_COL_FNT_LBL (%IDC_BUTTON3, 47)
    CONTROL ADD BUTTON,   hD, %IDC_BUTTON2, "OK", 412, 327, 80, 30
    SET_COL_FNT_LBL (%IDC_BUTTON2, 48)

    DIALOG SET ICON hD, "AICO"
    DIALOG SHOW MODAL hD, CALL ProcConsoleLnf TO lRslt
    FUNCTION = lRslt
END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO RefreshSampleConsole
    GRAPHIC ATTACH CB.HNDL, %IDC_IMAGE3, REDRAW
    GRAPHIC GET SCALE TO x1, y1, x2, y2
    GRAPHIC COLOR cfc, cbc
    GRAPHIC CLEAR
    IF hFont <> 0 THEN FONT END hFont
    FONT NEW fonts(cft), 3 * (1 + cfs), 0, %ANSI_CHARSET TO hFont
    GRAPHIC SET FONT hFont
    GRAPHIC CELL SIZE TO w, h
    GRAPHIC BOX (x1, h * 3 + 1)-(x2, y2), 0, ccc, ccc
    FOR lRes = 1 TO 3
        GRAPHIC PRINT "Hello, world!"
    NEXT
    IF cul = 1 THEN
        FOR lRes = 1 TO 3
            GRAPHIC LINE (x1, h * lRes)-(x2, h * lRes)
        NEXT
    END IF
    IF cso = 3 OR cso = 5 THEN ' turn screen upside down
        GRAPHIC SAVE APPDATA + "temp.bmp"
        GdipLoadImageFromFile UCODE$(APPDATA + "temp.bmp"), pImage
        GdipImageRotateFlip pImage, %Rotate180FlipNone
        GdipDrawImageRect hGdip(), pImage, x1, y1, x2, y2
        GdipDisposeImage pImage
        KILL APPDATA + "temp.bmp"
    END IF
    GRAPHIC REDRAW
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
CALLBACK FUNCTION ProcConsoleLnf()
    LOCAL lRes, x1, y1, x2, y2, w, h AS LONG
    LOCAL pImage AS DWORD
    STATIC hFont, cfs, cft, cso, cfc, cbc, ccc, cul AS LONG
    STATIC fonts() AS STRING
    STATIC Palette AS CustomColors

    lRes = GetDlgCtrlId(CB.WPARAM)

    IF CB.MSG = %WM_INITDIALOG THEN
        CenterWindow CB.HNDL
        DIM fonts(1 TO 3)
        ARRAY ASSIGN fonts() = "Courier New", "Arial", "Times New Roman"
        cfs = console.fontsize
        cft = console.fonttype
        cso = console.screenorientation
        cfc = PBColor((console.fontcolor))
        cbc = PBColor((console.backcolor))
        ccc = PBColor((console.clearcolor))
        cul = console.uselines
        RefreshSampleConsole

    ELSEIF CB.MSG = %WM_SETCURSOR AND (lRes = %IDC_IMAGE1 OR lRes = %IDC_IMAGE2 OR lRes = %IDC_IMAGE4) THEN
        SetCursor LoadCursor(%NULL, BYVAL %IDC_HAND) ' change cursor to link-hand when hovering over graphics
        SetWindowLong CB.HNDL, %dwl_msgresult, 1
        FUNCTION = 1


    ELSEIF CB.MSG = %WM_COMMAND AND CB.CTL = %IDC_BUTTON1 AND _
      (CB.CTLMSG = %BN_CLICKED OR CB.CTLMSG = 1) THEN                 ' Default look and feel
        cfs = 2 : COMBOBOX SELECT CB.HNDL,   %IDC_TEXTBOX1, cfs
        cft = 1 : COMBOBOX SELECT CB.HNDL,   %IDC_TEXTBOX2, cft
        cso = 1 : COMBOBOX SELECT CB.HNDL,   %IDC_TEXTBOX3, cso
        cul = 1 : CONTROL SET CHECK CB.HNDL, %IDC_TEXTBOX4, cul
        cfc = PBColor("0xff000000")
        GRAPHIC ATTACH CB.HNDL, %IDC_IMAGE1
        GRAPHIC COLOR -1, cfc
        GRAPHIC CLEAR
        cbc = PBColor("0xffffffff")
        GRAPHIC ATTACH CB.HNDL, %IDC_IMAGE2
        GRAPHIC COLOR -1, cbc
        GRAPHIC CLEAR
        ccc = PBColor("0xff006478")
        GRAPHIC ATTACH CB.HNDL, %IDC_IMAGE4
        GRAPHIC COLOR -1, ccc
        GRAPHIC CLEAR
        CONTROL KILL CB.HNDL, %IDC_IMAGE3
        CONTROL ADD GRAPHIC, CB.HNDL, %IDC_IMAGE3, "", 242, 160, 112, 152, %WS_CHILD OR %WS_VISIBLE, %WS_EX_CLIENTEDGE
        RefreshSampleConsole

    ELSEIF CB.MSG = %WM_COMMAND AND CB.CTLMSG = %CBN_SELENDOK AND CB.CTL = %IDC_TEXTBOX1 THEN ' Console Font Size
        COMBOBOX GET SELECT CB.HNDL, %IDC_TEXTBOX1 TO cfs
        RefreshSampleConsole

    ELSEIF CB.MSG = %WM_COMMAND AND CB.CTLMSG = %CBN_SELENDOK AND CB.CTL = %IDC_TEXTBOX2 THEN ' Console Font Type
        COMBOBOX GET SELECT CB.HNDL, %IDC_TEXTBOX2 TO cft
        RefreshSampleConsole

    ELSEIF CB.MSG = %WM_COMMAND AND CB.CTLMSG = %CBN_SELENDOK AND CB.CTL = %IDC_TEXTBOX3 THEN ' Console Screen Orientation
        COMBOBOX GET SELECT CB.HNDL, %IDC_TEXTBOX3 TO cso
        IF cso = 2 OR cso = 3 THEN  ' 2."Fixed Landscape", 3."Fixed Reverse Landscape"
            CONTROL KILL CB.HNDL, %IDC_IMAGE3
            CONTROL ADD GRAPHIC, CB.HNDL, %IDC_IMAGE3, "", 242, 180, 182, 112, %WS_CHILD OR %WS_VISIBLE, %WS_EX_CLIENTEDGE
        ELSE              ' 1."Variable By Sensors", 4."Fixed Portrait", 5."Fixed Reverse Portrait"
            CONTROL KILL CB.HNDL, %IDC_IMAGE3
            CONTROL ADD GRAPHIC, CB.HNDL, %IDC_IMAGE3, "", 242, 160, 112, 152, %WS_CHILD OR %WS_VISIBLE, %WS_EX_CLIENTEDGE
        END IF
        RefreshSampleConsole

    ELSEIF CB.MSG = %WM_COMMAND AND CB.CTL = %IDC_IMAGE1 AND _
      (CB.CTLMSG = %BN_CLICKED OR CB.CTLMSG = 1) THEN                 ' Console Font color
        DISPLAY COLOR CB.HNDL, 0, 0, cfc, Palette, 0 TO lRes
        IF lRes <> -1 THEN
            cfc = lRes
            GRAPHIC ATTACH CB.HNDL, %IDC_IMAGE1
            GRAPHIC COLOR -1, cfc
            GRAPHIC CLEAR
        END IF
        RefreshSampleConsole

    ELSEIF CB.MSG = %WM_COMMAND AND CB.CTL = %IDC_IMAGE2 AND _
      (CB.CTLMSG = %BN_CLICKED OR CB.CTLMSG = 1) THEN                 ' Console Background color
        DISPLAY COLOR CB.HNDL, 60, 0, cbc, Palette, 0 TO lRes
        IF lRes <> -1 THEN
            cbc = lRes
            GRAPHIC ATTACH CB.HNDL, %IDC_IMAGE2
            GRAPHIC COLOR -1, cbc
            GRAPHIC CLEAR
        END IF
        RefreshSampleConsole

    ELSEIF CB.MSG = %WM_COMMAND AND CB.CTL = %IDC_IMAGE4 AND _
      (CB.CTLMSG = %BN_CLICKED OR CB.CTLMSG = 1) THEN                 ' Console Empty color
        DISPLAY COLOR CB.HNDL, 60, 0, ccc, Palette, 0 TO lRes
        IF lRes <> -1 THEN
            ccc = lRes
            GRAPHIC ATTACH CB.HNDL, %IDC_IMAGE4
            GRAPHIC COLOR -1, ccc
            GRAPHIC CLEAR
        END IF
        RefreshSampleConsole

    ELSEIF CB.MSG = %WM_COMMAND AND CB.CTL = %IDC_TEXTBOX4 AND _
      CB.CTLMSG = %BN_CLICKED THEN                                   ' Console Use Lines checkbox
        CONTROL GET CHECK CB.HNDL, %IDC_TEXTBOX4 TO cul
        RefreshSampleConsole

    ELSEIF CB.MSG = %WM_COMMAND AND CB.CTL = %IDC_BUTTON2 AND _
      (CB.CTLMSG = %BN_CLICKED OR CB.CTLMSG = 1) THEN                ' OK button
        console.fontsize          = cfs
        console.fonttype          = cft
        console.screenorientation = cso
        console.fontcolor         = JavaColor(cfc) + $NUL
        console.backcolor         = JavaColor(cbc) + $NUL
        console.clearcolor        = JavaColor(ccc) + $NUL
        console.uselines          = cul
        DIALOG END CB.HNDL

    ELSEIF CB.MSG = %WM_COMMAND AND CB.CTL = %IDC_BUTTON3 AND _
      (CB.CTLMSG = %BN_CLICKED OR CB.CTLMSG = 1) THEN                ' Cancel button
        DIALOG END CB.HNDL

    END IF
END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
FUNCTION ShowPermissionsDialog(BYVAL hParent AS DWORD) AS LONG
    LOCAL lRslt, i   AS LONG
    LOCAL hD, hFont1 AS DWORD

    DIALOG NEW PIXELS, hParent, GET_LABEL(20), , , 450, CEIL(%PERM_NB/2)*(%LINEHEIGHT+4)+75, _
      %WS_POPUP OR %WS_BORDER OR %WS_DLGFRAME  OR %WS_SYSMENU OR %WS_CLIPSIBLINGS OR %WS_VISIBLE _
      OR %DS_MODALFRAME OR %DS_3DLOOK OR %DS_NOFAILCREATE OR %DS_SETFONT OR %WS_CAPTION, _
      %WS_EX_WINDOWEDGE OR %WS_EX_CONTROLPARENT OR %WS_EX_LEFT OR %WS_EX_LTRREADING _
      OR %WS_EX_RIGHTSCROLLBAR, TO hD
    DIALOG SET COLOR hD, %BLACK, %WHITE

    CONTROL ADD CHECKBOX, hD, %IDC_TEXTBOX1, "Autodetect", 25, 5, 130, 16
    SET_COL_FNT_LBL (%IDC_TEXTBOX1, 49)
    FONT NEW "Tahoma", 7, 3 TO hFont1
    CONTROL SET FONT hD, %IDC_TEXTBOX1, hFont1
    CONTROL SET CHECK hD, %IDC_TEXTBOX1, 1 - setperms

    CONTROL ADD BUTTON, hD, %IDC_BUTTON3, "Select all / none ...", 170, 5, 200, 16
    SET_COL_FNT_LBL (%IDC_BUTTON3, 50)
    FONT NEW "Tahoma", 7, 3 TO hFont1
    CONTROL SET FONT hD, %IDC_BUTTON3, hFont1

    FOR i = 0 TO CEIL(%PERM_NB/2)
        CONTROL ADD CHECKBOX, hD, 1100+i, PermissionStr(i), 5, i*(%LINEHEIGHT+4)+26, 218, %LINEHEIGHT
        SET_COL_FNT_LBL (1100+i, 0)
        IF app.permission(i) THEN CONTROL SET CHECK hD, 1100+i, 1
        IF i = 0 THEN CONTROL DISABLE hD, 1100+i ' 0.WRITE_EXTERNAL_STORAGE always set by default
    NEXT
    FOR i = CEIL(%PERM_NB/2)+1 TO %PERM_NB
        CONTROL ADD CHECKBOX, hD, 1100+i, PermissionStr(i), 225, (i-CEIL(%PERM_NB/2)-1)*(%LINEHEIGHT+4)+26, 218, %LINEHEIGHT
        SET_COL_FNT_LBL (1100+i, 0)
        IF app.permission(i) THEN CONTROL SET CHECK hD, 1100+i, 1
    NEXT

    CONTROL ADD BUTTON, hD, %IDC_BUTTON1, "OK",     365, CEIL(%PERM_NB/2)*(%LINEHEIGHT+4)+40, 80, 30
    SET_COL_FNT_LBL (%IDC_BUTTON1, 48)
    CONTROL ADD BUTTON, hD, %IDC_BUTTON2, "Cancel", 270, CEIL(%PERM_NB/2)*(%LINEHEIGHT+4)+40, 80, 30
    SET_COL_FNT_LBL (%IDC_BUTTON2, 47)

    DIALOG SET ICON hD, "AICO"
    DIALOG SHOW MODAL hD, CALL ProcPermissions TO lRslt
    FUNCTION = lRslt

    FONT END hFont1
END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
CALLBACK FUNCTION ProcPermissions() ' PERMISSIONS DIALOG
    LOCAL i, j, k AS LONG
    STATIC sp AS LONG ' local setperms

    SELECT CASE AS LONG CB.MSG

        CASE %WM_INITDIALOG
            sp = setperms
            CenterWindow CB.HNDL
            FOR i = 0 TO %PERM_NB
                CONTROL SET CHECK CB.HNDL, 1100+i, app.permission(i)
                IF sp = 0 THEN CONTROL DISABLE CB.HNDL, 1100+i
            NEXT
            IF sp = 0 THEN CONTROL DISABLE CB.HNDL, %IDC_BUTTON3

        CASE %WM_COMMAND
            SELECT CASE CB.CTL

            CASE %IDC_BUTTON1 ' OK
                IF CB.CTLMSG = %BN_CLICKED OR CB.CTLMSG = 1 THEN
                    FOR i = 0 TO %PERM_NB
                        CONTROL GET CHECK CB.HNDL, 1100+i TO app.permission(i)
                    NEXT
                    setperms = sp
                    DIALOG END CB.HNDL, %IDOK
                END IF

            CASE %IDC_BUTTON2 ' CANCEL
                IF CB.CTLMSG = %BN_CLICKED OR CB.CTLMSG = 1 THEN
                    DIALOG END CB.HNDL, %IDCANCEL
                END IF

            CASE %IDC_BUTTON3 ' ALL / NONE
                IF CB.CTLMSG = %BN_CLICKED OR CB.CTLMSG = 1 THEN
                    k = 0
                    FOR i = 0 TO %PERM_NB
                        CONTROL GET CHECK CB.HNDL, 1100+i TO j
                        k = k + j
                    NEXT
                    FOR i = 0 TO %PERM_NB
                        IF k > %PERM_NB / 2 THEN
                            IF i <> 0 THEN CONTROL SET CHECK CB.HNDL, 1100+i, 0 ' 0.WRITE_EXTERNAL_STORAGE always set by default
                        ELSE
                            CONTROL SET CHECK CB.HNDL, 1100+i, 1
                        END IF
                    NEXT
                END IF

            CASE %idc_textbox1 ' AUTOSET PERMISSIONS
                IF CB.CTLMSG = %BN_CLICKED THEN
                    CONTROL GET CHECK CB.HNDL, %IDC_TEXTBOX1 TO k
                    sp = 1 - k
                    IF sp = 0 THEN ' User chose "autoset"
                        DIM tp(%PERM_NB) AS LOCAL LONG ' tp() = temporary permissions
                        FOR i = 0 TO %PERM_NB : tp(i) = app.permission(i) : NEXT ' Backup current permissions
                        AutosetPermissions (LOCALBAS) ' Redo a full autoset
                        FOR i = 0 TO %PERM_NB ' Show autodetected perms + Restore current permissions
                            CONTROL SET CHECK CB.HNDL, 1100+i, app.permission(i)
                            CONTROL DISABLE CB.HNDL, 1100+i
                            app.permission(i) = tp(i)
                        NEXT
                        ERASE tp() ' Release memory
                        CONTROL DISABLE CB.HNDL, %IDC_BUTTON3
                    ELSE    ' User chose "manual"
                        FOR i = 0 TO %PERM_NB ' Enable permission checkboxes to be changed
                            IF i <> 0 THEN CONTROL ENABLE CB.HNDL, 1100+i
                        NEXT
                        CONTROL ENABLE CB.HNDL, %IDC_BUTTON3
                    END IF
                END IF

            END SELECT
        END SELECT

END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
FUNCTION ShowCopyToSDdialog(BYVAL hParent AS DWORD) AS LONG
    LOCAL i, lRslt AS LONG
    LOCAL hD AS DWORD

    DIALOG NEW PIXELS, hParent, GET_LABEL(22), , , 436, 260, %WS_POPUP OR %WS_BORDER _
        OR %WS_DLGFRAME  OR %WS_SYSMENU OR %WS_CLIPSIBLINGS OR %WS_VISIBLE OR %DS_MODALFRAME _
        OR %DS_3DLOOK OR %DS_NOFAILCREATE OR %DS_SETFONT OR %WS_CAPTION, %WS_EX_WINDOWEDGE _
        OR %WS_EX_CONTROLPARENT OR %WS_EX_LEFT OR %WS_EX_LTRREADING OR %WS_EX_RIGHTSCROLLBAR, _
        TO hD
    DIALOG SET COLOR hD, %BLACK, %WHITE

    CONTROL ADD LISTBOX, hD, %IDC_TEXTBOX1, , 8, 8, 420, 200, %WS_TABSTOP _
        OR %WS_VSCROLL OR %LBS_EXTENDEDSEL, %WS_EX_CLIENTEDGE
    SET_COL_FNT_LBL (%IDC_TEXTBOX1, 0)

    ' Make list of resources selected by the user
    FOR i = 1 TO UBOUND(all_res)
        IF  res4app(i) AND RIGHT$(all_res(i), 1) <> "/" THEN
            LISTBOX ADD hD, %IDC_TEXTBOX1, all_res(i) TO lRslt
            LISTBOX SET USER hD, %IDC_TEXTBOX1, lRslt, i
            IF app2SD(i) = 1 THEN LISTBOX SELECT hD, %IDC_TEXTBOX1, lRslt
        END IF
    NEXT

    CONTROL ADD BUTTON, hD, %IDC_BUTTON2, "Cancel", 10, 216, 80, 30
    SET_COL_FNT_LBL (%IDC_BUTTON2, 47)
    CONTROL ADD BUTTON, hD, %IDC_BUTTON3, "Select all/none", 102, 216, 236, 30
    SET_COL_FNT_LBL (%IDC_BUTTON3, 50)
    CONTROL ADD BUTTON, hD, %IDC_BUTTON1, "OK",     348, 216, 80, 30
    SET_COL_FNT_LBL (%IDC_BUTTON1, 48)

    DIALOG SET ICON   hD, "AICO"
    CenterWindow      hD
    DIALOG SHOW MODAL hD, CALL ProcCopyToSD TO lRslt
    FUNCTION = lRslt
END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
CALLBACK FUNCTION ProcCopyToSD()
    LOCAL i, j, k, n AS LONG

    SELECT CASE AS LONG CB.MSG

        CASE %WM_COMMAND
            SELECT CASE CB.CTL

            CASE %IDC_BUTTON1 ' OK
                IF CB.CTLMSG = %BN_CLICKED OR CB.CTLMSG = 1 THEN
                    RESET app2SD() ' by default, do not copy to SD
                    LISTBOX GET SELCOUNT CB.HNDL, %IDC_TEXTBOX1 TO n
                    FOR i = 1 TO n
                        LISTBOX GET SELECT CB.HNDL, %IDC_TEXTBOX1, j+1 TO j
                        LISTBOX GET USER   CB.HNDL, %IDC_TEXTBOX1, j TO k
                        app2SD(k) = 1
                    NEXT
                    ERASE app_res()
                    FOR i = 1 TO UBOUND(all_res)
                        IF res4app(i) THEN
                            REDIM PRESERVE app_res(1 TO UBOUND(app_res) - LBOUND(app_res) + 2)
                            app_res(UBOUND(app_res)) = TRIM$(app2SD(i)) + all_res(i)
                        END IF
                    NEXT
                    DIALOG END CB.HNDL, %IDOK
                END IF

            CASE %IDC_BUTTON2 ' CANCEL
                IF CB.CTLMSG = %BN_CLICKED OR CB.CTLMSG = 1 THEN
                    DIALOG END CB.HNDL, %IDCANCEL
                END IF

            CASE %IDC_BUTTON3 ' ALL / NONE
                IF CB.CTLMSG = %BN_CLICKED OR CB.CTLMSG = 1 THEN
                    LISTBOX GET COUNT    CB.HNDL, %IDC_TEXTBOX1 TO n
                    LISTBOX GET SELCOUNT CB.HNDL, %IDC_TEXTBOX1 TO k
                    FOR i = 1 TO n
                        IF k > n / 2 THEN
                            LISTBOX UNSELECT CB.HNDL, %IDC_TEXTBOX1, i
                        ELSE
                            LISTBOX SELECT   CB.HNDL, %IDC_TEXTBOX1, i
                        END IF
                    NEXT
                END IF

            END SELECT
        END SELECT

END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
FUNCTION ShowKSConfDialog(BYVAL hParent AS DWORD, OPTIONAL BYVAL incorrectUsrAndPwd AS LONG) AS LONG
    LOCAL lRslt AS LONG
    LOCAL hDlg  AS DWORD
    LOCAL hFont AS DWORD
    LOCAL ti AS STRING

    ti = GET_LABEL(12) ' Certificate:
    ti = RTRIM$(ti, ANY " :")

    DIALOG NEW PIXELS, hParent, ti, , , 237, 130, %WS_POPUP OR %WS_BORDER _
        OR %WS_DLGFRAME  OR %WS_SYSMENU OR %WS_CLIPSIBLINGS OR %WS_VISIBLE OR %DS_MODALFRAME _
        OR %DS_3DLOOK OR %DS_NOFAILCREATE OR %DS_SETFONT OR %WS_CAPTION, %WS_EX_WINDOWEDGE _
        OR %WS_EX_CONTROLPARENT OR %WS_EX_LEFT OR %WS_EX_LTRREADING OR %WS_EX_RIGHTSCROLLBAR, _
        TO hDlg

    AccelOkCancel hDlg

    IF incorrectUsrAndPwd THEN
        CONTROL ADD LABEL, hDlg, %IDC_LABEL4, GET_LABEL(51), 8, 4, 229, 32
        CONTROL SET COLOR  hDlg, %IDC_LABEL4, %RED, -1
    ELSE
        CONTROL ADD LABEL, hDlg, %IDC_LABEL4, GET_LABEL(52), 8, 4, 229, 32
        CONTROL SET COLOR  hDlg, %IDC_LABEL4, %RGB_DARKBLUE, -1
    END IF

    CONTROL ADD LABEL,   hDlg, %IDC_LABEL1, GET_LABEL(53), 8, 39, 60, 16 ' Alias
    CONTROL ADD TEXTBOX, hDlg, %IDC_TEXTBOX1, app.KSusr, 72, 37, 160, 20

    CONTROL ADD LABEL,   hDlg, %IDC_LABEL2, GET_LABEL(54), 8, 61, 60, 16 ' Password
    CONTROL ADD TEXTBOX, hDlg, %IDC_TEXTBOX2, app.KSpwd, 72, 59, 160, 20, %WS_CHILD _
        OR %WS_VISIBLE OR %WS_TABSTOP OR %ES_LEFT OR %ES_PASSWORD OR _
        %ES_AUTOHSCROLL, %WS_EX_CLIENTEDGE OR %WS_EX_LEFT OR _
        %WS_EX_LTRREADING OR %WS_EX_RIGHTSCROLLBAR

    CONTROL ADD LABEL,   hDlg, %IDC_LABEL3, GET_LABEL(55), 8, 83, 60, 16 ' Confirm
    CONTROL ADD TEXTBOX, hDlg, %IDC_TEXTBOX3, app.KSpwd, 72, 81, 160, 20, %WS_CHILD _
        OR %WS_VISIBLE OR %WS_TABSTOP OR %ES_LEFT OR %ES_PASSWORD OR _
        %ES_AUTOHSCROLL, %WS_EX_CLIENTEDGE OR %WS_EX_LEFT OR _
        %WS_EX_LTRREADING OR %WS_EX_RIGHTSCROLLBAR

    CONTROL ADD BUTTON,  hDlg, %IDOK,     GET_LABEL(48), 8, 105, 168, 20  ' OK
    CONTROL ADD BUTTON,  hDlg, %IDCANCEL, GET_LABEL(47), 182, 105, 50, 20 ' Cancel

    FONT NEW "Arial", 8, 1, %ANSI_CHARSET TO hFont
    CONTROL SET FONT     hDlg, %IDC_LABEL4, hFont

    DIALOG SET ICON      hDlg, "AICO"
    CenterWindow         hDlg
    DIALOG SHOW MODAL    hDlg, CALL ProcKSConf TO lRslt
    FUNCTION = lRslt

    FONT END hFont
END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
CALLBACK FUNCTION ProcKSConf()
    LOCAL p1, p2, ti AS STRING

    IF CB.MSG = %WM_COMMAND AND CB.CTL = %IDOK AND _ ' OK BUTTON
        (CB.CTLMSG = %BN_CLICKED OR CB.CTLMSG = 1) THEN
        CONTROL GET TEXT CB.HNDL, %IDC_TEXTBOX2 TO p1
        CONTROL GET TEXT CB.HNDL, %IDC_TEXTBOX3 TO p2
        ti = GET_LABEL(12) ' Certificate:
        ti = RTRIM$(ti, ANY " :")
        IF p1 = "" OR p2 = "" THEN
            CONTROL SET FOCUS CB.HNDL, %IDC_TEXTBOX2
            MyMsgBox CB.HNDL, GET_LABEL(56), ti, %MB_ICONWARNING ' You need to specify non-empty Alias and Password
            EXIT FUNCTION
        END IF
        IF p1 <> p2 THEN
            MyMsgBox CB.HNDL, GET_LABEL(57), ti, %MB_ICONERROR ' Passwords don't match
            EXIT FUNCTION
        END IF
        IF REMOVE$(LCASE$(p1), ANY "0123456789abcdefghijklmnopqrstuvwxyz_$#") <> "" THEN
            CONTROL SET FOCUS CB.HNDL, %IDC_TEXTBOX2
            MyMsgBox CB.HNDL, GET_LABEL(58), ti, %MB_ICONWARNING ' Passwords can only contain characters A-Z a-z 0-9 and _ $ #
            EXIT FUNCTION
        END IF
        app.KSpwd = p2 + $NUL
        CONTROL GET TEXT CB.HNDL, %IDC_TEXTBOX1 TO p1
        app.KSusr = p1 + $NUL
        DIALOG END CB.HNDL, 1 ' Alias and Password validated by user

    ELSEIF CB.MSG = %WM_COMMAND AND CB.CTL = %IDCANCEL AND _ ' CANCEL BUTTON
        (CB.CTLMSG = %BN_CLICKED OR CB.CTLMSG = 1) THEN
        DIALOG END CB.HNDL, 0 ' Cancelled by user

    END IF

END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
FUNCTION ShowNewKSdetails(BYVAL hParent AS DWORD) AS LONG
    LOCAL lRslt AS LONG
    LOCAL hDlg  AS DWORD
    LOCAL ti AS STRING

    ti = GET_LABEL(12) ' Certificate:
    ti = RTRIM$(ti, ANY " :")

    DIALOG NEW PIXELS, hParent, ti, , , 264, 176, %WS_POPUP OR %WS_BORDER _
        OR %WS_DLGFRAME  OR %WS_SYSMENU OR %WS_CLIPSIBLINGS OR %WS_VISIBLE OR %DS_MODALFRAME _
        OR %DS_3DLOOK OR %DS_NOFAILCREATE OR %DS_SETFONT OR %WS_CAPTION, %WS_EX_WINDOWEDGE _
        OR %WS_EX_CONTROLPARENT OR %WS_EX_LEFT OR %WS_EX_LTRREADING OR %WS_EX_RIGHTSCROLLBAR, _
        TO hDlg

    AccelOkCancel hDlg

    CONTROL ADD LABEL,   hDlg, %IDC_LABEL1, GET_LABEL(62), 8, 11, 120, 16 ' Your full name:
    CONTROL ADD TEXTBOX, hDlg, %IDC_TEXTBOX1, KSCommonName, 128, 7, 127, 18

    CONTROL ADD LABEL,   hDlg, %IDC_LABEL1, GET_LABEL(63), 8, 32, 120, 16 ' Your organization name:
    CONTROL ADD TEXTBOX, hDlg, %IDC_TEXTBOX2, KSOrgName, 128, 28, 127, 18

    CONTROL ADD LABEL,   hDlg, %IDC_LABEL1, GET_LABEL(64), 8, 53, 118, 16 ' Your organization unit:
    CONTROL ADD TEXTBOX, hDlg, %IDC_TEXTBOX3, KSOrgUnit, 128, 49, 125, 18

    CONTROL ADD LABEL,   hDlg, %IDC_LABEL1, GET_LABEL(65), 8, 74, 118, 16 ' Your city / locality:
    CONTROL ADD TEXTBOX, hDlg, %IDC_BUTTON1, KSLocality, 128, 70, 125, 18

    CONTROL ADD LABEL,   hDlg, %IDC_LABEL1, GET_LABEL(66), 8, 96, 118, 16 ' Your state / province:
    CONTROL ADD TEXTBOX, hDlg, %IDC_BUTTON2, KSState, 128, 92, 125, 18

    CONTROL ADD LABEL,   hDlg, %IDC_LABEL1, GET_LABEL(67), 8, 117, 158, 16 ' Your 2-letter country code:
    CONTROL ADD TEXTBOX, hDlg, %IDC_BUTTON3, KSCountry, 198, 113, 55, 18

    CONTROL ADD BUTTON,  hDlg, %IDCANCEL, GET_LABEL(47), 72, 144, 72, 24  ' Cancel
    CONTROL ADD BUTTON,  hDlg, %IDOK,     GET_LABEL(48), 176, 144, 72, 24 ' OK

    DIALOG SET ICON      hDlg, "AICO"
    CenterWindow         hDlg
    DIALOG SHOW MODAL    hDlg, CALL ProcNewKSdetails TO lRslt

    FUNCTION = lRslt
END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
CALLBACK FUNCTION ProcNewKSdetails()
    LOCAL e AS STRING

    IF CB.MSG = %WM_COMMAND AND CB.CTL = %IDOK AND _ ' OK BUTTON
        (CB.CTLMSG = %BN_CLICKED OR CB.CTLMSG = 1) THEN
        CONTROL GET TEXT CB.HNDL, %IDC_TEXTBOX1 TO e
        e = TRIM$(e) : IF e <> "" THEN KSCommonName = e
        CONTROL GET TEXT CB.HNDL, %IDC_TEXTBOX2 TO e
        e = TRIM$(e) : IF e <> "" THEN KSOrgName = e
        CONTROL GET TEXT CB.HNDL, %IDC_TEXTBOX3 TO e
        e = TRIM$(e) : IF e <> "" THEN KSOrgUnit = e
        CONTROL GET TEXT CB.HNDL, %IDC_BUTTON1 TO e
        e = TRIM$(e) : IF e <> "" THEN KSLocality = e
        CONTROL GET TEXT CB.HNDL, %IDC_BUTTON2 TO e
        e = TRIM$(e) : IF e <> "" THEN KSState = e
        CONTROL GET TEXT CB.HNDL, %IDC_BUTTON3 TO e
        e = TRIM$(e) : IF e <> "" THEN KSCountry = e
        DIALOG END CB.HNDL, 1 ' Details validated by user

    ELSEIF CB.MSG = %WM_COMMAND AND CB.CTL = %IDCANCEL AND _ ' CANCEL BUTTON
        (CB.CTLMSG = %BN_CLICKED OR CB.CTLMSG = 1) THEN
        DIALOG END CB.HNDL, 0 ' Cancelled by user

    END IF

END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
FUNCTION ShowSplashScreenDialog(BYVAL hParent AS DWORD) AS LONG
    LOCAL hD    AS DWORD
    LOCAL w     AS LONG
    LOCAL lRslt AS LONG

    DIALOG NEW PIXELS, hParent, GET_LABEL(19), , , 531, 247, %WS_POPUP OR %WS_BORDER _
        OR %WS_DLGFRAME  OR %WS_SYSMENU OR %WS_CLIPSIBLINGS OR %WS_VISIBLE OR %DS_MODALFRAME _
        OR %DS_3DLOOK OR %DS_NOFAILCREATE OR %DS_SETFONT OR %WS_CAPTION, %WS_EX_WINDOWEDGE _
        OR %WS_EX_CONTROLPARENT OR %WS_EX_LEFT OR %WS_EX_LTRREADING OR %WS_EX_RIGHTSCROLLBAR, _
        TO hD
    DIALOG SET COLOR hD, %BLACK, %WHITE

    CONTROL ADD GRAPHIC,  hD, %IDC_IMAGE1, "", 8, 8, 112, 176, _
        %WS_CHILD OR %WS_VISIBLE, %WS_EX_CLIENTEDGE

    CONTROL ADD "msctls_trackbar32", hD, %IDC_TRACKBAR, "", 8, 186, 112, 20, %WS_CHILD OR %WS_VISIBLE OR &H0008 OR &H0010 OR %WS_TABSTOP
    CONTROL SET COLOR     hD, %IDC_TRACKBAR, %BLACK, %WHITE
    CONTROL SEND          hD, %IDC_TRACKBAR, %WM_USER+6, 1, MAKDWD(0, 20)         ' %TBM_SETRANGE
    CONTROL SEND          hD, %IDC_TRACKBAR, %WM_USER+5, 1, (app.splashtimer\500) ' %TBM_SETPOS

    CONTROL ADD FRAME,    hD, 999, "", 128, 16, 392, 90
    CONTROL ADD FRAME,    hD, 999, "", 128, 116, 392, 78

    CONTROL ADD CHECKBOX, hD, %IDC_TEXTBOX2, "Graphical splash screen", 138, 100-88, 374, 24
    SET_COL_FNT_LBL (%IDC_TEXTBOX2, 72)
    GRAPHIC ATTACH        hD, %IDC_IMAGE1
    GRAPHIC SET FONT hFontLbl
    w = GRAPHIC(TEXT.SIZE.X, GET_LABEL(72)) + 20
    CONTROL SET SIZE      hD, %IDC_TEXTBOX2, w, 24

    CONTROL ADD CHECKBOX, hD, %IDC_TEXTBOX1, "Progress text", 138, 12+100, 374, 24, _
        %BS_LEFT OR %BS_VCENTER OR %WS_TABSTOP OR %WS_GROUP
    SET_COL_FNT_LBL (%IDC_TEXTBOX1, 71)
    GRAPHIC ATTACH        hD, %IDC_IMAGE1
    GRAPHIC SET FONT hFontLbl
    w = GRAPHIC(TEXT.SIZE.X, GET_LABEL(71)) + 20
    CONTROL SET SIZE      hD, %IDC_TEXTBOX1, w, 24

    CONTROL ADD BUTTON,   hD, %IDC_BUTTON4, "Change image", 136, 130-88, 180, 24, _
        %BS_CENTER OR %BS_VCENTER OR %WS_TABSTOP OR %WS_GROUP
    SET_COL_FNT_LBL (%IDC_BUTTON4, 73)

    CONTROL ADD GRAPHIC,  hD, %IDC_IMAGE2, "", 136, 163-88, 26, 26, %WS_CHILD _
        OR %WS_VISIBLE OR %SS_SUNKEN OR %SS_NOTIFY, %WS_EX_CLIENTEDGE
    CONTROL ADD LABEL,   hD, %IDC_LABEL3, "Background color", 168, 169-88, 256, 17
    SET_COL_FNT_LBL (%IDC_LABEL3, 74)

    CONTROL ADD LABEL,    hD, %IDC_LABEL1, "Message", 138, 44+100, 88, 13
    SET_COL_FNT_LBL (%IDC_LABEL1, 68)
    CONTROL ADD TEXTBOX,  hD, %IDC_TEXTBOX3, app.startupmsg, 226, 40+100, 282, 20, _
        %ES_AUTOHSCROLL OR %ES_LEFT OR %WS_BORDER OR %WS_TABSTOP OR %WS_GROUP, _
        %WS_EX_CLIENTEDGE OR %WS_EX_LEFT
    SET_COL_FNT_LBL (%IDC_TEXTBOX3, 0)

    CONTROL ADD LABEL,    hD, %IDC_LABEL2, "Loading indicator", 138, 68+100, 178, 13
    SET_COL_FNT_LBL (%IDC_LABEL2, 69)
    CONTROL ADD TEXTBOX,  hD, %IDC_TEXTBOX4, app.loadchr, 316, 64+100, 30, 20
    SET_COL_FNT_LBL (%IDC_TEXTBOX4, 0)

    CONTROL ADD BUTTON,   hD, %IDC_BUTTON3, "Cancel", 8, 208, 80, 30, _
        %BS_CENTER OR %BS_VCENTER OR %WS_TABSTOP OR %WS_GROUP
    SET_COL_FNT_LBL (%IDC_BUTTON3, 47)
    CONTROL ADD BUTTON,   hD, %IDC_BUTTON1, "Reset to default", 128, 211, 270, 24
    SET_COL_FNT_LBL (%IDC_BUTTON1, 75)
    CONTROL ADD BUTTON,   hD, %IDC_BUTTON2, "OK", 440, 208, 80, 30
    SET_COL_FNT_LBL (%IDC_BUTTON2, 48)

    DIALOG SET ICON hD, "AICO"
    DIALOG SHOW MODAL hD, CALL ProcSplashScreen TO lRslt
    FUNCTION = lRslt
END FUNCTION
'------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
MACRO PickSplashImage
    MACROTEMP extList
    DIM extList AS STRING

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

    DISPLAY OPENFILE CB.HNDL, , , GET_LABEL(72), GET_LABEL(73), extList, "", "", %OFN_FILEMUSTEXIST TO simg
    IF simg <> "" THEN
        GdipDisposeImage pImage
        GdipLoadImageFromFile UCODE$(simg), pImage
    END IF

END MACRO
'--------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO RefreshProgressBox
    GRAPHIC ATTACH CB.HNDL, %IDC_IMAGE1
    GRAPHIC BOX  (7, 69) - (7+98, 69+38), 0, RGB(41,41,41), RGB(41,41,41)
    GRAPHIC RENDER BITMAP "ICO72", (11, 71) - (11+16, 71+16)
    GRAPHIC COLOR RGB(100,201,255), RGB(41,41,41) ' light blue on dark gray
    GRAPHIC SET FONT hFont
    CONTROL GET TEXT   CB.HNDL, %IDC_TEXTBOX3 TO e ' Progress message
    IF LEN(e) THEN
        e2 = ""
        DO
            GRAPHIC TEXT SIZE e TO w, h
            IF w >= 74 THEN
                lRes = INSTR(-1, e, $SPC)
                IF lRes = 0 THEN ' no space but too long string!
                    DO
                        e2 = RIGHT$(e, 1) + e2
                        e = LEFT$(e, LEN(e) - 1)
                        GRAPHIC TEXT SIZE e TO w, h
                    LOOP UNTIL w < 74
                ELSE
                    e2 = MID$ (e, lRes + 1) + $SPC + e2
                    e  = LEFT$(e, lRes - 1)
                    GRAPHIC TEXT SIZE e TO w, h
                END IF
            END IF
        LOOP UNTIL w < 74
        GRAPHIC TEXT SIZE e2 TO w, h
        IF w >= 74 THEN
            DO
                e2 = LEFT$(e2, LEN(e2) - 1)
                GRAPHIC TEXT SIZE e2 TO w, h
            LOOP UNTIL w < 74
        END IF
        GRAPHIC SET POS (30, 69)
        GRAPHIC PRINT e
        GRAPHIC SET POS (30, 79)
        GRAPHIC PRINT e2
    END IF
    CONTROL GET TEXT CB.HNDL, %IDC_TEXTBOX4 TO e2 ' Progress marker
    IF LEN(e2) THEN
        e = e2
        GRAPHIC TEXT SIZE e TO w, h
        DO WHILE w < 60
            e += e2
            GRAPHIC TEXT SIZE e TO w, h
        LOOP
        GRAPHIC COLOR %WHITE, RGB(41,41,41) ' white on dark gray
        GRAPHIC SET POS (8, 90)
        GRAPHIC PRINT e
    END IF
    GRAPHIC LINE (7, 91) - (7+98, 91), RGB(100,201,255) ' light blue separator
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO RefreshSplashScreen
    IF sdisp THEN                 ' GRAPHICAL SPLASH SCREEN 'ON'
        CONTROL SET CHECK  CB.HNDL, %IDC_TEXTBOX2, 1
        CONTROL SET COLOR  CB.HNDL, %IDC_TEXTBOX2, %BLACK, %WHITE
        CONTROL REDRAW     CB.HNDL, %IDC_TEXTBOX2
        CONTROL ENABLE     CB.HNDL, %IDC_BUTTON4 ' Change image
        CONTROL ENABLE     CB.HNDL, %IDC_IMAGE2  ' Background color
        CONTROL ENABLE     CB.HNDL, %IDC_LABEL3
        GRAPHIC ATTACH     CB.HNDL, %IDC_IMAGE1
        GRAPHIC COLOR %BLACK, sbgn
        GRAPHIC CLEAR
        GdipGetImageWidth  pImage, w
        GdipGetImageHeight pImage, h
        IF w >= h THEN
            lRes = MIN(108, w)
            GdipDrawImageRect hGdip(), pImage, (112-lRes)\2, (176-lRes*h\w)\2, lRes, lRes*h\w
        ELSE
            lRes = MIN(172, h)
            GdipDrawImageRect hGdip(), pImage, (112-lRes*w\h)\2, (176-lRes)\2, lRes*w\h, lRes
        END IF
        GRAPHIC REDRAW
    ELSE                          ' GRAPHICAL SPLASH SCREEN 'OFF'
        CONTROL SET CHECK  CB.HNDL, %IDC_TEXTBOX2, 0
        CONTROL SET COLOR  CB.HNDL, %IDC_TEXTBOX2, RGB(100,100,100), %WHITE
        CONTROL REDRAW     CB.HNDL, %IDC_TEXTBOX2
        CONTROL DISABLE    CB.HNDL, %IDC_BUTTON4 ' Change image
        CONTROL DISABLE    CB.HNDL, %IDC_IMAGE2  ' Background color
        CONTROL DISABLE    CB.HNDL, %IDC_LABEL3
        GRAPHIC ATTACH     CB.HNDL, %IDC_IMAGE1
        GRAPHIC COLOR %BLACK, sbgn
        GRAPHIC CLEAR
    END IF

    IF sprog THEN                 ' PROGRESS TEXT 'ON'
        CONTROL SET CHECK  CB.HNDL, %IDC_TEXTBOX1, 1
        CONTROL SET COLOR  CB.HNDL, %IDC_TEXTBOX1, %BLACK, %WHITE
        CONTROL REDRAW     CB.HNDL, %IDC_TEXTBOX1
        CONTROL ENABLE     CB.HNDL, %IDC_TEXTBOX3 ' Message
        CONTROL ENABLE     CB.HNDL, %IDC_LABEL1
        CONTROL ENABLE     CB.HNDL, %IDC_TEXTBOX4 ' Loading indicator
        CONTROL ENABLE     CB.HNDL, %IDC_LABEL2
        RefreshProgressBox
    ELSE                          ' PROGRESS TEXT 'OFF'
        CONTROL SET CHECK  CB.HNDL, %IDC_TEXTBOX1, 0
        CONTROL SET COLOR  CB.HNDL, %IDC_TEXTBOX1, RGB(100,100,100), %WHITE
        CONTROL REDRAW     CB.HNDL, %IDC_TEXTBOX1
        CONTROL DISABLE    CB.HNDL, %IDC_TEXTBOX3 ' Message
        CONTROL DISABLE    CB.HNDL, %IDC_LABEL1
        CONTROL DISABLE    CB.HNDL, %IDC_TEXTBOX4 ' Loading indicator
        CONTROL DISABLE    CB.HNDL, %IDC_LABEL2
    END IF

    GRAPHIC SET FONT hFont2
    e = TRIM$(stmr/1000) + "s"
    IF e = ".5s" THEN e = "0.5s"
    GRAPHIC TEXT SIZE e TO w, h
    GRAPHIC RENDER BITMAP "TMR", ((112-w)\2-6, 176-h-7) - ((112-w)\2-5+9, 176-h-7+14)
    GRAPHIC SET POS ((112-w)\2+6, 176-h-7)
    GRAPHIC COLOR %BLACK, sbgn
    GRAPHIC PRINT e
    GRAPHIC REDRAW

END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
CALLBACK FUNCTION ProcSplashScreen()
    LOCAL lRes, w, h AS LONG
    LOCAL e, e2 AS STRING
    STATIC Palette AS CustomColors
    STATIC hFont, hFont2, pImage AS DWORD
    STATIC sbgn, sdisp, sprog, stmr AS LONG
    STATIC simg AS STRING

    lRes = GetDlgCtrlId(CB.WPARAM)

    IF CB.MSG = %WM_INITDIALOG THEN
        CenterWindow CB.HNDL
        FONT NEW "Courier New", 7, 0, %ANSI_CHARSET TO hFont
        FONT NEW "Courier New", 9, 0, %ANSI_CHARSET TO hFont2
        sdisp = app.splashdisplay
        sprog = app.splashprogress
        sbgn = PBColor((app.splashbgndcolor))
        stmr = app.splashtimer
        GRAPHIC ATTACH CB.HNDL, %IDC_IMAGE2 ' Background color
        GRAPHIC COLOR -1, sbgn
        GRAPHIC CLEAR
        simg = app.splashimg
        IF LEN(simg) THEN GdipLoadImageFromFile UCODE$(simg), pImage
        IF pImage = 0 THEN simg = ""
        IF simg = "" THEN pImage = GdipLoadJpgFromResource("POWERED")
        RefreshSplashScreen

    ELSEIF CB.MSG = %WM_SETCURSOR AND lRes = %IDC_IMAGE2 THEN
        SetCursor LoadCursor(%NULL, BYVAL %IDC_HAND) ' change cursor to link-hand when hovering over graphics
        SetWindowLong CB.HNDL, %dwl_msgresult, 1
        FUNCTION = 1

    ELSEIF CB.MSG = %WM_HSCROLL THEN ' Timer trackbar
        IF GetDlgCtrlId(CB.LPARAM) = %IDC_TRACKBAR THEN
            CONTROL SEND CB.HNDL, %IDC_TRACKBAR, %WM_USER, 0, 0 TO w ' %TBM_GETPOS
            stmr = w*500
            RefreshSplashScreen
        END IF

    ELSEIF CB.MSG = %WM_COMMAND AND CB.CTL = %IDC_TEXTBOX2 AND CB.CTLMSG = %BN_CLICKED THEN  ' Checkbox 'Graphical splash screen'
        CONTROL GET CHECK CB.HNDL, %IDC_TEXTBOX2 TO sdisp
        RefreshSplashScreen

    ELSEIF CB.MSG = %WM_COMMAND AND CB.CTL = %IDC_TEXTBOX1 AND CB.CTLMSG = %BN_CLICKED THEN  ' Checkbox 'Progress text'
        CONTROL GET CHECK CB.HNDL, %IDC_TEXTBOX1 TO sprog
        RefreshSplashScreen

    ELSEIF CB.MSG = %WM_COMMAND AND CB.CTL = %IDC_TEXTBOX3 _
      AND CB.CTLMSG = %EN_CHANGE THEN                           ' Message
        RefreshProgressBox

    ELSEIF CB.MSG = %WM_COMMAND AND CB.CTL = %IDC_TEXTBOX4 _
      AND CB.CTLMSG = %EN_CHANGE THEN                           ' Loading indicator
        RefreshProgressBox

    ELSEIF CB.MSG = %WM_COMMAND AND CB.CTL = %IDC_BUTTON4 AND _
      (CB.CTLMSG = %BN_CLICKED OR CB.CTLMSG = 1) THEN                ' "Change image" button
        PickSplashImage
        IF pImage = 0 THEN pImage = GdipLoadJpgFromResource("POWERED")
        RefreshSplashScreen

    ELSEIF CB.MSG = %WM_COMMAND AND CB.CTL = %IDC_IMAGE2 AND _
      (CB.CTLMSG = %BN_CLICKED OR CB.CTLMSG = 1) THEN                 ' Background color
        DISPLAY COLOR CB.HNDL, 60, 0, sbgn, Palette, 0 TO lRes
        IF lRes <> -1 THEN
            sbgn = lRes
            GRAPHIC ATTACH CB.HNDL, %IDC_IMAGE2 ' Background color
            GRAPHIC COLOR -1, sbgn
            GRAPHIC CLEAR
        END IF
        RefreshSplashScreen

    ELSEIF CB.MSG = %WM_COMMAND AND CB.CTL = %IDC_BUTTON1 AND _
      (CB.CTLMSG = %BN_CLICKED OR CB.CTLMSG = 1) THEN                 ' Default settings
        sdisp = 1
        sprog = 0
        stmr = 1500
        sbgn = VAL("&HFFFFFF")
        GRAPHIC ATTACH CB.HNDL, %IDC_IMAGE2 ' Background color
        GRAPHIC COLOR -1, sbgn
        GRAPHIC CLEAR
        simg = "" : GdipDisposeImage pImage : pImage = GdipLoadJpgFromResource("POWERED")
        CONTROL SET TEXT CB.HNDL, %IDC_TEXTBOX3, "Standby for initial file loading."
        CONTROL SET TEXT CB.HNDL, %IDC_TEXTBOX4, "."
        RefreshSplashScreen

    ELSEIF CB.MSG = %WM_COMMAND AND CB.CTL = %IDC_BUTTON2 AND _
      (CB.CTLMSG = %BN_CLICKED OR CB.CTLMSG = 1) THEN                ' OK button
        app.splashdisplay   = sdisp
        app.splashprogress  = sprog
        app.splashtimer     = stmr
        app.splashbgndcolor = HtmlColor(sbgn) + $NUL
        app.splashimg = simg + $NUL
        CONTROL GET TEXT CB.HNDL, %IDC_TEXTBOX3 TO e : app.startupmsg = e + $NUL
        CONTROL GET TEXT CB.HNDL, %IDC_TEXTBOX4 TO e : app.loadchr = e + $NUL
        DIALOG END CB.HNDL

    ELSEIF CB.MSG = %WM_COMMAND AND CB.CTL = %IDC_BUTTON3 AND _
      (CB.CTLMSG = %BN_CLICKED OR CB.CTLMSG = 1) THEN                ' Cancel button
        DIALOG END CB.HNDL

    ELSEIF CB.MSG = %WM_DESTROY THEN
        GdipDisposeImage pImage
        FONT END hFont
        FONT END hFont2

    END IF
END FUNCTION
'------------------------------------------------------------------------------
