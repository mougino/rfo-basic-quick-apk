'##############################################################################
'#
'#  This is one of the include files for << RFO-BASIC! Quick APK >>
'#
'#  "rfo-ui.inc" contains some MACROs to generate the UI of the screens
'#
'##############################################################################

#IF NOT %DEF(%UPDATER)
#INCLUDE ONCE "INC\TREEVIEW.INC"
#ENDIF

#RESOURCE ICON   AICO,      "res\icon.ico"
#RESOURCE BITMAP SPLASH,    "res\splash.bmp"
#RESOURCE BITMAP WRONG,     "res\updater\wrong.bmp"
#RESOURCE BITMAP GOOD,      "res\updater\good.bmp"

#IF NOT %DEF(%UPDATER)
#RESOURCE BITMAP LNG,       "res\lang.bmp"
#RESOURCE BITMAP FONTINCR,  "res\a+.bmp"
#RESOURCE BITMAP FONTDECR,  "res\a-.bmp"
#RESOURCE BITMAP FOLDER,    "res\folder.bmp"
#RESOURCE BITMAP FILE,      "res\file.bmp"
#RESOURCE BITMAP PC,        "res\pc.bmp"
#RESOURCE BITMAP PHONE,     "res\phone.bmp"
#RESOURCE BITMAP USB_C1,    "res\usb_connect1.bmp"
#RESOURCE BITMAP USB_C2,    "res\usb_connect2.bmp"
#RESOURCE BITMAP WHAT,      "res\ghost.bmp"
#RESOURCE BITMAP APKOK,     "res\rainbow.bmp"
#RESOURCE BITMAP APKKO,     "res\fail.bmp"
#RESOURCE BITMAP GEAR1,     "res\gear1.bmp"
#RESOURCE BITMAP GEAR2,     "res\gear2.bmp"
#RESOURCE BITMAP GEAR3,     "res\gear3.bmp"
#RESOURCE BITMAP GEAR4,     "res\gear4.bmp"
#RESOURCE BITMAP GEAR5,     "res\gear5.bmp"
#RESOURCE BITMAP GEAR6,     "res\gear6.bmp"
#RESOURCE BITMAP ICO72,     "res\ico72.bmp"
#RESOURCE BITMAP INST0,     "res\install0.bmp"
#RESOURCE BITMAP INST1,     "res\install1.bmp"
#RESOURCE BITMAP INST2,     "res\install2.bmp"
#RESOURCE BITMAP INST3,     "res\install3.bmp"
#RESOURCE BITMAP WIF0,      "res\wifi_connect0.bmp"
#RESOURCE BITMAP WIF1,      "res\wifi_connect1.bmp"
#RESOURCE BITMAP WIF2,      "res\wifi_connect2.bmp"
#RESOURCE BITMAP WIF3,      "res\wifi_connect3.bmp"
#RESOURCE BITMAP WIF4,      "res\wifi_connect4.bmp"
#RESOURCE BITMAP WAV1,      "res\wave1.bmp"
#RESOURCE BITMAP WAV2,      "res\wave2.bmp"
#RESOURCE BITMAP WAV3,      "res\wave3.bmp"
#RESOURCE BITMAP WAV4,      "res\wave4.bmp"
#RESOURCE BITMAP WAV5,      "res\wave5.bmp"
#RESOURCE BITMAP WAV6,      "res\wave6.bmp"
#RESOURCE BITMAP WAV7,      "res\wave7.bmp"
#RESOURCE BITMAP DONATE,    "res\donate.bmp"
#RESOURCE RCDATA POWERED,   "res\powered.jpg"
#RESOURCE BITMAP NEWP,      "res\new.bmp"
#RESOURCE BITMAP DEL,       "res\delete.bmp"
#RESOURCE BITMAP TMR,       "res\timer.bmp"
#ENDIF

#IF NOT %DEF(%UPDATER)
%LOCAL                   = 1
%WIFI                    = 2
%USB                     = 3
%LINEHEIGHT              = 13

%ID_TIMER_SCROLL         = %WM_USER + 400
%ID_TIMER_THINK          = %WM_USER + 401
%ID_TIMER_LOG            = %WM_USER + 402
%ID_TIMER_TRANSFER       = %WM_USER + 403
%ID_TIMER_INSTALL        = %WM_USER + 404

%WM_START_ANIM_THINK     = %WM_USER + 900
%WM_START_ANIM_LOG       = %WM_USER + 901
%WM_WAIT_END_ANIM_LOG    = %WM_USER + 902
%WM_START_ANIM_TRANSFER  = %WM_USER + 903
%WM_TRANSFER_END         = %WM_USER + 904
%WM_DEVICE_FOUND         = %WM_USER + 905
%WM_SET_CTL_FOCUS        = %WM_USER + 984
%WM_SET_CTL_VISIBILITY   = %WM_USER + 985
%WM_INSTALL_START        = %WM_USER + 986
%WM_INSTALL_STOP         = %WM_USER + 987
%WM_NO_DEVICE_CONNECTED  = %WM_USER + 989
%WM_RFO_NOT_FOUND        = %WM_USER + 990
%WM_NO_LOCAL_BAS         = %WM_USER + 991
%WM_GOT_RES              = %WM_USER + 992
%WM_END_OF_BUILD         = %WM_USER + 993
%WM_DISPLAY_BAS_RES      = %WM_USER + 994
%WM_REFRESH_TREEVIEW_SEL = %WM_USER + 995
%WM_INIT_DIALOG          = %WM_USER + 996
%WM_REFRESH_DIALOG       = %WM_USER + 997
%WM_END_DIALOG           = %WM_USER + 998
%WM_KILL_THREAD          = %WM_USER + 999
#ENDIF

GLOBAL hFontLbl, hFontLnk, hFontTtl AS DWORD
GLOBAL font_size   AS LONG
GLOBAL hDlg        AS DWORD
#IF NOT %DEF(%UPDATER)
GLOBAL hAPKicon    AS LONG
GLOBAL ConnectMode AS LONG
GLOBAL hThreadMain AS DWORD
GLOBAL LngXml      AS STRING
GLOBAL ScrXml      AS STRING
GLOBAL ot          AS ColorTabsClass
#ENDIF

'------------------------------------------------------------------------------
FUNCTION INIT_DIALOG(w AS LONG, h AS LONG, title AS STRING) AS LONG
    LOCAL lRslt AS LONG

    DIALOG NEW PIXELS, %HWND_DESKTOP, title, , , w, h, _
      %DS_MODALFRAME OR %WS_CAPTION OR %WS_POPUP OR %WS_SYSMENU OR %WS_MINIMIZEBOX, TO hDlg
    DIALOG SET COLOR hDlg, %BLACK, %WHITE

    FONT NEW "Lucida Console", 10 + font_size, 0, %ANSI_CHARSET TO hFontLbl
    FONT NEW "Lucida Console", 14 + font_size, 3, %ANSI_CHARSET TO hFontTtl
    FONT NEW "Lucida Console", 10 + font_size, 4, %ANSI_CHARSET TO hFontLnk

#IF NOT %DEF(%UPDATER)
    RTF_ADDCOL "titleBlue", 80, 80, 120
#ENDIF

    DIALOG SET ICON hDlg, "AICO"
    DIALOG SHOW MODAL hDlg, CALL ProcMainDialog TO lRslt

    FUNCTION = lRslt

    FONT END hFontLbl
    FONT END hFontTtl
    FONT END hFontLnk
END FUNCTION
'------------------------------------------------------------------------------

#IF NOT %DEF(%UPDATER)
'------------------------------------------------------------------------------
MACRO CHANGE_FONT
    FONT END hFontLbl
    FONT END hFontTtl
    FONT END hFontLnk

    FONT NEW "Lucida Console", 10 + font_size, 0, %ANSI_CHARSET TO hFontLbl
    FONT NEW "Lucida Console", 14 + font_size, 3, %ANSI_CHARSET TO hFontTtl
    FONT NEW "Lucida Console", 10 + font_size, 4, %ANSI_CHARSET TO hFontLnk

    WritePrivateProfileString "Config", "font_size", TRIM$(font_size), INIFILE
    ShowScreen2projects
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO ADD_TITLE(x, y, w, h, tx)
    INCR ID
    CONTROL ADD LABEL, hDlg, 1000+ID, tx, x, y, w, h, %SS_NOTIFY OR %WS_TABSTOP
    CONTROL SET COLOR  hDlg, 1000+ID, RGB(80,80,120), %WHITE
    CONTROL SET FONT   hDlg, 1000+ID, hFontTtl
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO ADD_LABEL(x, y, w, h, tx)
    INCR ID
    CONTROL ADD LABEL, hDlg, 1000+ID, tx, x, y, w, h, %SS_NOTIFY OR %WS_TABSTOP
    CONTROL SET COLOR  hDlg, 1000+ID, %BLACK, %WHITE
    CONTROL SET FONT   hDlg, 1000+ID, hFontLbl
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO ADD_INPUT(x, y, w, h, tx)
    INCR ID
    CONTROL ADD TEXTBOX, hDlg, 1000+ID, tx, x, y, w, h
    CONTROL SET COLOR    hDlg, 1000+ID, %BLACK, %WHITE
    CONTROL SET FONT     hDlg, 1000+ID, hFontLbl
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO ADD_NUMINPUT(x, y, w, h, tx)
    INCR ID
    CONTROL ADD TEXTBOX, hDlg, 1000+ID, tx, x, y, w, h, %ES_NUMBER _
        OR %WS_TABSTOP OR %WS_BORDER OR %ES_LEFT OR %ES_AUTOHSCROLL, _
        %WS_EX_CLIENTEDGE OR %WS_EX_LEFT
    CONTROL SET COLOR    hDlg, 1000+ID, %BLACK, %WHITE
    CONTROL SET FONT     hDlg, 1000+ID, hFontLbl
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO ADD_LINK(x, y, w, h, tx)
    INCR ID
    CONTROL ADD LABEL, hDlg, 1000+ID, tx, x, y, w, h, %SS_NOTIFY OR %WS_TABSTOP
    CONTROL SET COLOR  hDlg, 1000+ID, %BLUE, %WHITE
    CONTROL SET FONT   hDlg, 1000+ID, hFontLnk
    linkable += "<" + TRIM$(1000+ID) + ">"
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO ADD_BUTTON(x, y, w, h, tx)
    INCR ID
    CONTROL ADD BUTTON, hDlg, 1000+ID, tx, x, y, w, h
    CONTROL SET COLOR   hDlg, 1000+ID, %BLACK, %WHITE
    CONTROL SET FONT    hDlg, 1000+ID, hFontLbl
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO ADD_IMG(x, y, w, h, img)
    INCR ID
    CONTROL ADD GRAPHIC, hDlg, 1000+ID, "", x, y, w, h, %SS_NOTIFY OR %WS_TABSTOP
    GRAPHIC ATTACH hDlg, 1000+ID
    GRAPHIC RENDER BITMAP img, (0, 0) - (w-1, h-1)
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO ADD_IMGBTN(x, y, w, h, img)
    INCR ID
    CONTROL ADD IMGBUTTON, hDlg, 1000+ID, img, x, y, w, h
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO ADD_CHECK(x, y, w, h, tx)
    INCR ID
    CONTROL ADD CHECKBOX, hDlg, 1000+ID, tx, x, y, w, h
    CONTROL SET COLOR     hDlg, 1000+ID, %BLACK, %WHITE
    CONTROL SET FONT      hDlg, 1000+ID, hFontLbl
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO ADD_ANIM
    INCR ID
    CONTROL ADD GRAPHIC, hDlg, 1000+ID, "", 8, -21*(SCREEN=3 OR SCREEN=4), 590, 150+8*(SCREEN=4)
    GRAPHIC ATTACH hDlg, 1000+ID, REDRAW
    GRAPHIC COLOR %BLACK, %WHITE
    GRAPHIC CLEAR

    GRAPHIC RENDER BITMAP "PC", (0, 0) - (150, 150)

    IF ConnectMode <> %LOCAL THEN
        GRAPHIC RENDER BITMAP "PHONE", (515, 0) - (515+75, 150)
    END IF

    GRAPHIC REDRAW
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO ADD_LISTBOX(x, y, w, h)
    INCR ID
    CONTROL ADD LISTBOX, hDlg, 1000+ID, , x, y, w, h
    CONTROL SET COLOR    hDlg, 1000+ID, %BLACK, %WHITE
    CONTROL SET FONT     hDlg, 1000+ID, hFontLbl
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO ADD_TREEVIEW(x, y, w, h)
    INCR ID
    CONTROL ADD TREEVIEW, hDlg, 1000+ID, "", x, y, w, h, _
      %WS_CHILD OR %WS_VISIBLE OR %WS_TABSTOP OR %TVS_HASBUTTONS OR _
      %TVS_HASLINES OR %TVS_LINESATROOT OR %TVS_DISABLEDRAGDROP OR _
      %TVS_SHOWSELALWAYS OR %TVS_CHECKBOXES OR %TVS_TRACKSELECT, _
      %WS_EX_LEFT OR %WS_EX_LTRREADING OR %WS_EX_RIGHTSCROLLBAR _
      OR %WS_EX_CLIENTEDGE
    CONTROL SET COLOR  hDlg, 1000+ID, %BLACK, %WHITE
    CONTROL SET FONT   hDlg, 1000+ID, hFontLbl

    LOCAL hImgList AS LONG
    IMAGELIST NEW BITMAP 16, 16, 32, 2 TO hImgList
    IMAGELIST ADD BITMAP hImgList, "FOLDER"
    IMAGELIST ADD BITMAP hImgList, "FILE"
    TREEVIEW SET IMAGELIST hDlg, 1000+ID, hImgList
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO ADD_DROPDOWN(x, y, w, h, items, selected)
    INCR ID
    CONTROL ADD COMBOBOX, hDlg, 1000+ID, items(), x, y, w, h, %CBS_DROPDOWNLIST OR %WS_TABSTOP
    CONTROL SET COLOR     hDlg, 1000+ID, %BLACK, %WHITE
    CONTROL SET FONT      hDlg, 1000+ID, hFontLbl
    COMBOBOX SELECT       hDlg, 1000+ID, selected
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO ADD_TABS(x, y, w, h)
    INCR ID
    ot = CLASS "ColorTabs"
    ot.StartTab (hDlg, 1000+ID, x, y, w, h, %BLACK, %WHITE)
    CONTROL SET FONT  hDlg, 1000+ID, hFontLbl
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO CLEAN_DIALOG()
    KillThread hThreadMain ' to avoid any conflict
    DIALOG POST hDlg, %WM_REFRESH_DIALOG, 0, 0
    LOCAL i AS LONG
    FOR i = 1 TO ID
        CONTROL KILL hDlg, 1000+i
    NEXT
    linkable = ""
    ID = 0
    ScrXml = XmlContent(LngXml, "screen id=" + $DQ + TRIM$(SCREEN) + $DQ)
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO SET_LABEL(tx) = IF tx <> "" THEN CONTROL SET TEXT hDlg, 1000+ID, tx
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO GET_LABEL(CtlId) = XmlContent(ScrXml, "label id=" + $DQ + TRIM$(CtlId) + $DQ)
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO GET_OLABEL(ScrId, CtlId) = XmlContent(XmlContent(LngXml, "screen id=" + $DQ + TRIM$(ScrId) + $DQ), "label id=" + $DQ + TRIM$(CtlId) + $DQ)
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO CUR_LABEL = GET_LABEL(ID)
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB SHOW_POPUP
    LOCAL lRes AS DWORD
    THREAD CREATE ThreadShowPopup(BYVAL 0) TO lRes
END SUB
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
THREAD FUNCTION ThreadShowPopup(BYVAL noVar AS DWORD)
    LOCAL hPp, i, x, y AS DWORD
    LOCAL richtext, tmpXml AS STRING

    DIALOG NEW PIXELS, 0, "",,, 627, 148, _
      %DS_MODALFRAME OR %WS_CAPTION OR %WS_POPUP OR %WS_SYSMENU OR %WS_MINIMIZEBOX, TO hPp
'      %DS_MODALFRAME OR %WS_CAPTION OR %WS_POPUP OR %WS_SYSMENU OR %WS_THICKFRAME, _
'      %WS_EX_TOOLWINDOW OR %WS_EX_LAYERED, TO hPp
    DIALOG SET COLOR   hPp, %BLACK, %WHITE

    DIALOG GET LOC hDlg TO x, y
    DIALOG SET LOC hPp, x+53, y+50

    CONTROL ADD "RichEdit", hPp, 1001, "", 2, 2, 607, 144, _
        %WS_CHILD OR %WS_VISIBLE OR %ES_MULTILINE OR %ES_READONLY _
        OR %WS_VSCROLL CALL ProcPopup()

    IF ConnectMode = %LOCAL THEN i = 201 ELSE IF ConnectMode = %WIFI THEN i = 211 ELSE i = 221
    tmpXml = XmlContent(LngXml, "screen id=" + $DQ + TRIM$(i) + $DQ)

    DIALOG SET TEXT hPp, XmlContent(tmpXml, "label id=" + $DQ + "1" + $DQ) ' Set popup window title

    ' Put content of tip in Richedit control
    richtext = "[font:l,8][black]" + XmlContent(tmpXml, "label id=" + $DQ + "2" + $DQ)
    IF ConnectMode = %WIFI THEN
        richtext += $CRLF + "[eop]"
        richtext += XmlContent(tmpXml, "label id=" + $DQ + "3" + $DQ) + $CRLF
        richtext += "https://play.google.com/store/apps/details?id=com.rfo.quickapk"
        richtext += "[eop][eol]"
    END IF
    REPLACE $CRLF WITH "[eol]" IN richtext
    REPLACE "XXX" WITH RTRIM$(APKfolder, "\") IN richtext
    RTF_SET hPp, 1001, richtext

    DIALOG SET ICON    hPp, "AICO"
    'SetLayeredWindowAttributes hPp, 0, 200, %LWA_ALPHA
    DIALOG SHOW MODAL  hPp, CALL ProcPopup()

    FUNCTION = 1
END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
CALLBACK FUNCTION ProcPopup()
    LOCAL w, h AS LONG

    IF CB.MSG = %WM_NOTIFY AND CB.NMCODE = %EN_LINK THEN
        RTF_hyperlink (CB.HNDL, CB.CTL, CB.LPARAM) ' Process RTF hyperlinks

    ELSEIF CB.MSG = %WM_SIZE THEN
        DIALOG GET SIZE  CB.HNDL TO w, h
        CONTROL SET SIZE CB.HNDL, 1001, w-20, h-20
        DIALOG REDRAW CB.HNDL

    END IF
END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO TRIGGER_TIMER(signal, ms)
    IF idEvent <> 0 THEN KillTimer CB.HNDL, idEvent
    idEvent = SetTimer(CB.HNDL, signal, ms, BYVAL %NULL)
    DIALOG POST CB.HNDL, %WM_TIMER, signal, 0
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO STOP_ANY_TIMER
    IF idEvent <> 0 THEN KillTimer CB.HNDL, idEvent
    idEvent = 0
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO ANIMATE_THINKING
    GRAPHIC ATTACH CB.HNDL, 1001, REDRAW
    anim = (anim MOD 6) + 1
    GRAPHIC RENDER BITMAP "GEAR" + TRIM$(anim), (260, 0) - (260+100, 62)
    GRAPHIC REDRAW
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO STOP_ANIM_THINKING
    STOP_ANY_TIMER ' Stop "thinking" animation
    GRAPHIC ATTACH CB.HNDL, 1001, REDRAW
    GRAPHIC BOX (260, 0)-(360, 62), 0, %WHITE, %WHITE
    GRAPHIC REDRAW
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO ANIMATE_SCROLLING
    GRAPHIC ATTACH hDlg, CTL, REDRAW
    GRAPHIC SET FONT hFontLbl : GRAPHIC COLOR %BLACK, %WHITE
    projLoc(CTL-1001) += 3 * IIF(dir=0, -1, +1)
    w = GRAPHIC(TEXT.SIZE.X, projNam(CTL-1001))
    IF projLoc(CTL-1001) < 69-w OR projLoc(CTL-1001) > 0 THEN
        dir = 1 - dir
    ELSE
        GRAPHIC SET POS (projLoc(CTL-1001), 73)
        GRAPHIC PRINT projNam(CTL-1001)
        GRAPHIC REDRAW
    END IF
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO STOP_ANIM_SCROLLING
    GRAPHIC ATTACH hDlg, CTL, REDRAW
    GRAPHIC SET FONT hFontLbl : GRAPHIC COLOR %BLACK, %WHITE
    GRAPHIC SET POS (0, 73)
    projLoc(CTL-1001) = 0
    GRAPHIC PRINT projNam(CTL-1001)
    GRAPHIC REDRAW
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO START_ANIM_LOG
    IF ConnectMode = %USB THEN ' Initialize USB connect animation
        alpha = 150
        anim = 1
        sens = 1 ' 1st part of animation: Android guy walks across the screen
        GRAPHIC ATTACH CB.HNDL, 1001, REDRAW
        GRAPHIC LINE (145, 124)-(150, 124), RGB(106,127,86)
        GRAPHIC LINE (145, 125)-(150, 125), RGB(0,13,0)
    ELSEIF ConnectMode = %WIFI THEN ' Initialize WiFi connect animation
        alpha = 0
        anim = 0
        sens = +1
    END IF
    TRIGGER_TIMER (%ID_TIMER_LOG, 100) ' Launch logging animation
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO ANIMATE_LOGGING
    GRAPHIC ATTACH CB.HNDL, 1001, REDRAW
    IF ConnectMode = %USB THEN
        IF sens = 1 THEN ' 1st part of animation: Android guy walks across the screen
            GRAPHIC RENDER BITMAP "USB_C" + TRIM$(anim), (alpha, 101) - (alpha+49, 101+49)
            GRAPHIC REDRAW
            anim = 3 - anim
            IF alpha < 450 THEN
                GRAPHIC BOX  (alpha, 101)-(alpha+10, 150), 0, %WHITE, %WHITE
                GRAPHIC LINE (alpha, 124)-(alpha+10, 124), RGB(106,127,86)
                GRAPHIC LINE (alpha, 125)-(alpha+10, 125), RGB(0,13,0)
                alpha += 10
            ELSE
                sens = 2 ' arrived! switch to 2nd part of animation
            END IF
        ELSEIF sens = 2 THEN ' 2nd part of animation: Android guy walks at same place
            GRAPHIC RENDER BITMAP "USB_C" + TRIM$(anim), (450, 101) - (499, 150)
            GRAPHIC REDRAW
            anim = 3 - anim
        END IF
    ELSEIF ConnectMode = %WIFI THEN
        alpha = (alpha + 1) MOD 20
        j = 55 + 30 * SIN(2 * 3.14159265359 * alpha / 20)
        GRAPHIC RENDER BITMAP "WIF" + TRIM$(anim), (262, j) - (262+58, j+32)
        IF alpha >= 11 AND alpha <= 17 THEN GRAPHIC RENDER BITMAP "WAV" + TRIM$(alpha-10), (322, j) - (322+19, j+32)
        GRAPHIC REDRAW
        anim += sens
        IF anim MOD 4 = 0 THEN sens *= -1
        GRAPHIC BOX (260, j-2) - (262+60, j+34), 0, %WHITE, %WHITE
        IF alpha >= 11 AND alpha <= 17 THEN GRAPHIC BOX (320, j-2) - (322+21, j+34), 0, %WHITE, %WHITE
    END IF
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO SHOW_LOGGED
    STOP_ANY_TIMER ' Stop "logging" animation
    GRAPHIC ATTACH    CB.HNDL, 1001, REDRAW
    GRAPHIC CLEAR
    GRAPHIC RENDER BITMAP "PC", (0, 0) - (150, 150) ' Show "logged" image
    IF ConnectMode = %USB THEN
        GRAPHIC LINE (145, 124)-(475, 124), RGB(106,127,86)
        GRAPHIC LINE (145, 125)-(475, 125), RGB(0,13,0)
        GRAPHIC RENDER BITMAP "USB_C1", (475, 101) - (475+49, 101+49)
    END IF
    GRAPHIC RENDER BITMAP "PHONE", (515, 0) - (515+75, 150)
    GRAPHIC REDRAW
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO START_ANIM_TRANSFER
    IF ConnectMode = %USB THEN ' Start "transfer" animation
        sens = 1 ' 1st part of animation: binary string coming from phone
        binstr = ""
        GRAPHIC ATTACH CB.HNDL, 1001, REDRAW
        GRAPHIC SET FONT hFontLbl
        TRIGGER_TIMER (%ID_TIMER_TRANSFER, 10) ' in USB
    ELSEIF ConnectMode = %WIFI THEN
        TRIGGER_TIMER (%ID_TIMER_TRANSFER, 80) ' or in WiFi
    END IF
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO ANIMATE_TRANSFER
    GRAPHIC ATTACH CB.HNDL, 1001, REDRAW
    IF ConnectMode = %USB THEN
        i = GRAPHIC(CHR.SIZE.X)
        IF sens = 1 THEN ' 1st part of animation: binary string coming from phone
            binstr += TRIM$(INT(RND+.5))
            IF i * LEN(binstr) > 380 THEN binstr = RIGHT$(binstr, -1)
            GRAPHIC SET POS (135, 101-GRAPHIC(CHR.SIZE.Y))
            GRAPHIC PRINT SPACE$(380\i - LEN(binstr)) + binstr
            GRAPHIC REDRAW
        ELSEIF sens = 2 THEN ' 2nd part of animation: binary string fading out
            binstr = MID$(binstr, 2)
            GRAPHIC SET POS (135, 101-GRAPHIC(CHR.SIZE.Y))
            GRAPHIC PRINT binstr + $SPC
            GRAPHIC REDRAW
            IF binstr = "" THEN
                STOP_ANY_TIMER ' finished 2nd part of animation
                IF SCREEN = 3 THEN
                    ShowScreen4setbasres
                ELSEIF SCREEN = 7 AND eots = +1 THEN ' End Of Transfer Status +1 -> go on with creating APK
                    TRIGGER_TIMER (%ID_TIMER_THINK, 50) ' Start "thinking" animation
                END IF
            END IF
        END IF
    ELSEIF ConnectMode = %WIFI THEN
        alpha = (alpha + 1) MOD 20
        wavimg = (wavimg MOD 7) + 1
        j = 55 + 30 * SIN(2 * 3.14159265359 * alpha / 20)
        GRAPHIC RENDER BITMAP "WIF" + TRIM$(anim),   (262, j) - (262+58, j+32)
        GRAPHIC RENDER BITMAP "WAV" + TRIM$(wavimg), (322, j) - (322+19, j+32)
        GRAPHIC REDRAW
        anim += sens
        IF anim MOD 4 = 0 THEN sens *= -1
        GRAPHIC BOX (260, j-2) - (262+60, j+34), 0, %WHITE, %WHITE
        GRAPHIC BOX (320, j-2) - (322+21, j+34), 0, %WHITE, %WHITE
    END IF
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO STOP_ANIM_TRANSFER
    anim = 0
    GRAPHIC ATTACH CB.HNDL, 1001, REDRAW
    IF ConnectMode = %USB THEN
        sens = 2 ' 2nd part of animation: binary string fading out
        i = GRAPHIC(CHR.SIZE.X)
        binstr = SPACE$(380\i - LEN(binstr)) + binstr
    ELSEIF ConnectMode = %WIFI THEN
        STOP_ANY_TIMER ' Stop "transfer" animation
        GRAPHIC CLEAR
        GRAPHIC RENDER BITMAP "PC", (0, 0) - (150, 150)
        GRAPHIC RENDER BITMAP "PHONE", (515, 0) - (515+75, 150)
        GRAPHIC REDRAW
        IF SCREEN = 7 AND eots = +1 THEN ' End Of Transfer Status +1 -> go on with creating APK
            TRIGGER_TIMER (%ID_TIMER_THINK, 50) ' Start "thinking" animation
        END IF
    END IF
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO SHOW_DISCONNECTED
    GRAPHIC ATTACH CB.HNDL, 1001, REDRAW ' Display ghost device image
    GRAPHIC RENDER BITMAP "WHAT", (515, 0) - (515+75, 150)
    IF CB.WPARAM = 0 THEN
        IF ConnectMode = %WIFI THEN
            GRAPHIC BOX (260, 23) - (322+21, 85+34), 0, %WHITE, %WHITE
        ELSE
            GRAPHIC RENDER BITMAP "USB_C1", (alpha, 101) - (alpha+49, 150)
        END IF
    END IF
    GRAPHIC REDRAW
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO SHOW_APK_OK
    STOP_ANY_TIMER ' Stop "thinking" or "install" animations + display image of successful APK
    GRAPHIC ATTACH CB.HNDL, 1001, REDRAW
    GRAPHIC RENDER BITMAP "APKOK", (250, 0) - (250+110, 110)
    GRAPHIC REDRAW
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO SHOW_APK_KO
    STOP_ANY_TIMER ' Stop "thinking" or "install" animations + display image of failed build
    GRAPHIC ATTACH CB.HNDL, 1001, REDRAW
    GRAPHIC RENDER BITMAP "APKKO", (250, 0) - (250+110, 110)
    GRAPHIC REDRAW
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO ANIMATE_INSTALL
    GRAPHIC ATTACH CB.HNDL, 1001, REDRAW
    GRAPHIC RENDER BITMAP "INST" + TRIM$(anim), (250, 0) - (250+110, 110)
    GRAPHIC REDRAW
    anim = (anim + 1) MOD 5
END MACRO
'------------------------------------------------------------------------------

#ENDIF
