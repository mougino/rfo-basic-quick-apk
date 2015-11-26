' Examples of use:
'  SetMyMsgBoxFont "Ms Sans Serif", 8, -1, -1 ' -1 = default color
'  SetMyMsgBoxFont "Lucida Console", 10, %BLACK, %WHITE
'  SetMyMsgBoxFont "comic sans ms", 12, %RGB_YELLOW, %RGB_FIREBRICK
'  SetMyMsgBoxFont "Arial", 10, -1, -1 ' -1 = default color
'  MyMsgBox CB.HNDL, "Simple MyMsgBox without any button defined"+$CR+"(called with only 1 parameter)"
'  MyMsgBox CB.HNDL, "Information", "", %MB_ICONINFORMATION
'  MyMsgBox CB.HNDL, "Warning"+$CR+"Retry?", "", %MB_ICONWARNING OR %MB_RETRYCANCEL
'  MyMsgBox CB.HNDL, "Critical error"+$CR+"What to do?", "", %MB_ICONERROR OR %MB_ABORTRETRYIGNORE
'  MyMsgBox CB.HNDL, "%MB_CANCELTRYCONTINUE", "", %MB_CANCELTRYCONTINUE
'  MyMsgBox CB.HNDL, "%MB_RETRYCANCEL", "", %MB_RETRYCANCEL
'  MyMsgBox CB.HNDL, "%MB_YESNO", "", %MB_YESNO
'  MyMsgBox CB.HNDL, "%MB_YESNOCANCEL", "", %MB_YESNOCANCEL
'  MyMsgBox CB.HNDL, "%MB_ABORTRETRYIGNORE", "", %MB_ABORTRETRYIGNORE
'  MyMsgBox CB.HNDL, "%MB_OKCANCEL", "", %MB_OKCANCEL
'  lRes = MyMsgBox(CB.HNDL, "Question?", "", %MB_ICONQUESTION OR %MB_YESNO)
'  lRes = MyMsgBox(CB.HNDL, "This just shows the versatility.", "Custom MSGBOX Four", %MB_ICONINFORMATION, "&Yes;&No;&What?")

#INCLUDE ONCE "windows.inc"

TYPE MyMsgBoxFont
    name AS ASCIIZ * %MAX_PATH
    size AS LONG
    fore AS LONG
    back AS LONG
END TYPE

GLOBAL msgfont      AS MyMsgBoxFont
GLOBAL btnfont      AS MyMsgBoxFont

GLOBAL LBL_CANCEL   AS STRING
GLOBAL LBL_RETRY    AS STRING
GLOBAL LBL_CONTINUE AS STRING
GLOBAL LBL_YES      AS STRING
GLOBAL LBL_NO       AS STRING
GLOBAL LBL_ABORT    AS STRING
GLOBAL LBL_IGNORE   AS STRING
GLOBAL LBL_OK       AS STRING

SUB SetMyMsgBoxFont(fntname AS STRING, fntsize AS LONG, fntfore AS LONG, fntback AS LONG)
    msgfont.name = TRIM$(fntname) + $NUL
    msgfont.size = fntsize
    msgfont.fore = fntfore
    msgfont.back = fntback
    ' By default, use same font for MyMsgBox buttons:
    SetMyMsgBoxBtnFont(fntname, fntsize) ' Overwrite this by calling SetMsgBoxBtnFont with another font as parameter
END SUB

SUB SetMyMsgBoxBtnFont(fntname AS STRING, fntsize AS LONG)
    btnfont.name = TRIM$(fntname) + $NUL
    btnfont.size = fntsize
END SUB

FUNCTION MyMsgBox(BYVAL hWin AS DWORD, BYVAL msgtxt AS STRING, OPTIONAL BYVAL msgtitle AS STRING, _
    OPTIONAL BYVAL style AS LONG, OPTIONAL BYVAL buttons AS STRING) AS LONG

    LOCAL hmsg AS DWORD
    LOCAL lRes, hMsgfont, hBtnfont, hIcon, ico, bip AS LONG
    LOCAL x, y, tbx, tbw, tbh, btnNb, btn, bx, by, bw AS LONG
    LOCAL blist() AS STRING

    ' Set some default values if absent
    IF msgfont.name = "" THEN SetMyMsgBoxFont "Ms Sans Serif", 8, -1, -1
    IF msgtitle = "" THEN msgtitle = EXE.NAME$
    REPLACE $CR WITH $CRLF IN msgtxt
    REPLACE $CR+$CR WITH $CR IN msgtxt

    ' Create dialog
    DIALOG NEW PIXELS , hWin, msgtitle, , , 400, 100 TO hmsg

    ' Define icon (if any) and sound, from style value
    bip = %MB_OK
    IF (STYLE AND %MB_ICONINFORMATION) = %MB_ICONINFORMATION THEN ico = 104 : bip = %MB_ICONASTERISK
    IF (STYLE AND %MB_ICONERROR) = %MB_ICONERROR THEN ico = 103 : bip = %MB_ICONHAND
    IF (STYLE AND %MB_ICONQUESTION) = %MB_ICONQUESTION THEN ico = 102 : bip = %MB_ICONQUESTION
    IF (STYLE AND %MB_ICONWARNING) = %MB_ICONWARNING THEN ico = 101 : bip = %MB_ICONEXCLAMATION
    tbx = 10
    IF ico THEN
        CONTROL ADD LABEL,   hmsg, 12299, "", 5, 5, 0, 0, %SS_ICON ' Control ID 12299 = icon
        CONTROL SET COLOR    hmsg, 12299, msgfont.fore, msgfont.back
        hIcon = LoadImage(BYVAL 0, BYVAL ico, %IMAGE_ICON, 32, 32, 0)
        SendDlgItemMessage   hmsg, 12299, %STM_SETIMAGE, %IMAGE_ICON, hIcon
        tbx = 42 ' 5 + 32 + 5
    END IF

    ' Add elements
    CONTROL ADD TEXTBOX, hmsg, 12300, msgtxt, tbx, 10, 10, 10, %ES_MULTILINE OR %ES_READONLY ' Control ID 12300 = textbox
    CONTROL ADD GRAPHIC, hmsg, 12301, "", -900, -900, 10, 10 ' Control ID 12301 = test graphic box

    ' Calculate graphic size of text with desired font
    GRAPHIC ATTACH hmsg, 12301
    FONT NEW msgfont.name, msgfont.size TO hMsgfont
    CONTROL SET FONT hmsg, 12300, hMsgfont
    GRAPHIC SET FONT hMsgfont
    x = GRAPHIC(TEXT.SIZE.X, msgtxt)
    y = GRAPHIC(TEXT.SIZE.Y, msgtxt)
    tbw = MAX(160, MIN(380, x))
    tbh = (x * y) * tbw / 100000 + (2 * msgfont.size)
    CONTROL SET SIZE hmsg, 12300, tbw, tbh ' set size of textbox
    CONTROL SET COLOR hmsg, 12300, msgfont.fore, msgfont.back
    DIALOG SET COLOR hmsg, msgfont.fore, msgfont.back

    ' Create buttons from style value (if any)
    IF LBL_CANCEL   = "" THEN LBL_CANCEL   = "&Cancel"
    IF LBL_RETRY    = "" THEN LBL_RETRY    = "&Retry"
    IF LBL_CONTINUE = "" THEN LBL_CONTINUE = "C&ontinue"
    IF LBL_YES      = "" THEN LBL_YES      = "&Yes"
    IF LBL_NO       = "" THEN LBL_NO       = "&No"
    IF LBL_ABORT    = "" THEN LBL_ABORT    = "&Abort"
    IF LBL_IGNORE   = "" THEN LBL_IGNORE   = "&Ignore"
    IF LBL_OK       = "" THEN LBL_OK       = "&OK"
    IF (STYLE AND %MB_YESNO) = %MB_YESNO THEN buttons = LBL_YES+";"+LBL_NO
    IF (STYLE AND %MB_ABORTRETRYIGNORE) = %MB_ABORTRETRYIGNORE THEN buttons = LBL_ABORT+";"+LBL_RETRY+";"+LBL_IGNORE
    IF (STYLE AND %MB_OKCANCEL) = %MB_OKCANCEL THEN buttons = LBL_OK+";"+LBL_CANCEL
    IF (STYLE AND %MB_YESNOCANCEL) = %MB_YESNOCANCEL THEN buttons = LBL_YES+";"+LBL_NO+";"+LBL_CANCEL
    IF (STYLE AND %MB_RETRYCANCEL) = %MB_RETRYCANCEL THEN buttons = LBL_RETRY+";"+LBL_CANCEL
    IF (STYLE AND %MB_CANCELTRYCONTINUE) = %MB_CANCELTRYCONTINUE THEN buttons = LBL_CANCEL+";"+LBL_RETRY+";"+LBL_CONTINUE
    IF buttons = "" THEN buttons = LBL_OK

    ' Define list of buttons
    DIM blist(1 TO TALLY(buttons, ";") + 1)
    FOR y = 1 TO PARSECOUNT(buttons, ";")
        blist(y) = PARSE$(buttons, ";", y)
    NEXT
    btnNb = UBOUND(blist)

    ' Draw buttons
    FONT NEW btnfont.name, btnfont.size TO hBtnfont
    GRAPHIC SET FONT hBtnfont
    bx = 10
    by = 5 + tbh + 5
    IF ico THEN by = MAX(5 + tbh + 5, 5 + 32 + 5) ' make it so that buttons don't overlap icon, if any
    FOR btn = 1 TO btnNb
        x = GRAPHIC(TEXT.SIZE.X, blist(btn)) + 20
        CONTROL ADD BUTTON, hmsg, 12301 + btn, blist(btn), bx, by, x, 24
        CONTROL SET FONT hmsg, 12301 + btn, hBtnfont
        bx += 10 + x
    NEXT
    x = MAX(bx, tbw + IIF(ico, 52, 20)) ' Handle when width of all buttons aligned horizontally > size of window
    DIALOG SET CLIENT hmsg, x, by + 29 ' set size of dialog

    ' Center buttons
    bw = bx - 20
    bx = (x - bw) \ 2
    FOR btn = 1 TO btnNb
        CONTROL SET LOC  hmsg, 12301 + btn, bx, by
        CONTROL GET SIZE hmsg, 12301 + btn TO x, y
        bx += 10 + x
    NEXT
    GRAPHIC DETACH

    ' Beep and display the message box
    MessageBeep bip
    DIALOG SET ICON   hmsg, "AICO"
    DIALOG SHOW MODAL hmsg, CALL MyMsgBoxproc() TO lRes
    FUNCTION = lRes
    FONT END hMsgfont
    FONT END hBtnfont
    DeleteObject hIcon

END FUNCTION


CALLBACK FUNCTION MyMsgBoxproc() AS LONG   'This callback function processes the MyMsgBox dialog events.
    STATIC activebutton AS LONG
    SELECT CASE CB.MSG
        CASE %WM_INITDIALOG
            CenterWindow CB.HNDL
            activebutton = 12302
            CONTROL SET FOCUS CB.HNDL, activebutton
        CASE %WM_COMMAND
        SELECT CASE CB.CTL
            CASE %IDOK 'Causes ENTER to post click for button with focus.
                DIALOG POST CB.HNDL, %WM_COMMAND, activebutton, %BN_CLICKED
            CASE 12302 TO 12320
                SELECT CASE CB.CTLMSG
                    CASE %BN_CLICKED
                        DIALOG END CB.HNDL, CB.CTL - 12301
                    CASE %BN_SETFOCUS
                        activebutton = CB.CTL 'This allows the ENTER key to activate button with focus.
                END SELECT
        END SELECT
    END SELECT
END FUNCTION

'--------------------------------------------------------------------------------
SUB CenterWindow(BYVAL hWnd AS DWORD)
   'This should be fairly straight forward to grasp.
   'First see if the parent is the desktop, if not
   'it is a child window.  Then find the size of the
   'client area of the parent and proceed to center
   'the child there.
   '
   LOCAL ncWidth, ncHeight, x, y   AS LONG
   LOCAL hParent                   AS DWORD
   WINDOW GET PARENT hWnd TO hParent
   IF hParent = 0 THEN
       DESKTOP GET CLIENT TO ncWidth&, ncHeight&
   ELSE
       DIALOG GET CLIENT hParent TO ncWidth&, ncHeight&
   END IF
   DIALOG GET SIZE hWnd TO  x&, y&
   x& = ncWidth& - x&
   x& \=2
   y& = ncHeight& - y&
   y& \=2
   DIALOG SET LOC hWnd,x&, y&
END SUB
'--------------------------------------------------------------------------------
