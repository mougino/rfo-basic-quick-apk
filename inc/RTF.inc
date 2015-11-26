'##############################################################################
'#
'#  "rtf.inc" contains functions to display Rich Text Format
'#
'##############################################################################
'
'   Syntax to specify Fonts
'   ===============================================================
'   [font:X,size] where 7 <= size <= 72 and X is a 1-letter code:
'   C = Courier New     Fixed Size
'   T = Times New Roman Variable Size
'   A = Andale Mono     Fixed Size
'   L = Lucida Console  Fixed Size
'   G = Georgia Console Fixed Size
'   O = Open Sans       Variable Size
'
'   Syntax to specify (foreground) RTF_Col
'   ===============================================================
'   [col] where col is a color name among:
'   Black, Maroon, Green, Olive, Navy, Purple, Teal, Grey,
'   Silver, Red, Lime, Yellow, Blue, Fuschia, Aqua, White
'
'   Use RTF_ADDCOL "CustomBlue", 80, 80, 120 to add a RGB color
'
'   Syntax to specify Format
'   ===============================================================
'   [b][/b]     = bold
'   [i][/i]     = italic
'   [u][/u]     = underline
'   [h:col][/h] = highlight with color "col" (specified above)
'
'   Syntax to specify Alignment
'   ===============================================================
'   [c] = centered
'   [r] = right-aligned
'   [l] = left-aligned
'   [j] = justified
'
'   Syntax to specify Delimiters
'   ===============================================================
'   [eol] = End Of Line
'   [eo§] = [eop] = End Of Paragraph
'   [np]  = New Page
'
'   IN ORDER TO PROCESS URLs IN YOUR CALLBACK:
'   ===============================================================
'   SELECT CASE CB.MSG
'       CASE %WM_NOTIFY
'           IF CB.NMID = RichEditCtl AND CB.NMCODE = %EN_LINK THEN RTF_hyperlink (CB.HNDL, RichEditCtl, CB.LPARAM)

#INCLUDE ONCE "RICHEDIT.INC"

GLOBAL RTF_ColNam() AS STRING
GLOBAL RTF_ColVal() AS STRING

'-----------------------------------------------------------------------------------------------------------------
FUNCTION RTF_ExampleOfDialog AS DWORD ' Returns handle to the newly created dialog
    LOCAL richtext AS STRING
    LOCAL hDlg AS DWORD

    LoadLibrary("RICHED32.DLL")

    DIALOG NEW PIXELS, 0, "RichEdit Dialog", , , 800, 600, %WS_SYSMENU TO hDlg
    DIALOG SET COLOR hDlg, -1, %WHITE
    CONTROL ADD "RichEdit", hDlg, 1001, "Test2", 7, 7, 785, 585, %WS_CHILD OR _
      %WS_VISIBLE OR %ES_MULTILINE OR %ES_READONLY OR %WS_VSCROLL

    richtext  = "[font:c,72][c][maroon]Sample File[eop]"
    richtext += "[l][black][font:a,12]This routine built by [h:aqua]Marty Francom[/h], "
    richtext += "use it, modify it, share it with[eol]"
    richtext += "others as you see fit. If you make improvements or add features, please[eol]"
    richtext += "share them with the http://powerbasic.com Forum.[eol][eop]"
    richtext += "[font:g,14][green]When building an RTF file with this routine alway start[eol]"
    richtext += "the file with RTF('Start') and end building the file[eol]"
    richtext += "with RTF('End')[b][fuchsia]    Everything ELSE is optional.[/b][eop]"
    richtext += "[eol]"   '+ "[np]"
    richtext += "[l][red][font:l,16][i]Some Italic Text in NewTimesRoman Font [/i]16 Point in Red.[eol][eop]"
    richtext += "[r][blue][font:c,12][b]Some BOLD Text in Courier Font 12 Point in Blue. Right Justified.[eol][eol]"
    richtext += "[l][green][/b][font:t,10]Some Text[blue][font:l,22] in different[font:a,16][maroon] font sizes "
    richtext += "[font:n,8][black][b]and RTF_Col.[/b][eol][eop]"
    richtext += "[font:t,16]The Command Line Syntax is  RTFPRINT Filename.RTF Action& NumCopy&[eol]"
    richtext += "[c][font:t,20][fuchsia]ie:  RTFPRINT Sample.Rtf 0 2[eol][eop]"

    RTF_SET hDlg, 1001, richtext
    FUNCTION = hDlg
END FUNCTION
'-----------------------------------------------------------------------------------------------------------------

'-----------------------------------------------------------------------------------------------------------------
SUB RTF_SET (hWnd AS DWORD, hCtl AS DWORD, rt AS STRING)
    SUBCLASS (hWnd, hCtl, RTF_SUBCLASS) ' Comment that if you want to be able to edit the text (and see a caret)
    SendMessage GetDlgItem(hWnd, hCtl), %EM_SETEVENTMASK, 0, %ENM_SELCHANGE OR %ENM_CHANGE OR %ENM_LINK
    CONTROL SEND hWnd, hCtl, %EM_AUTOURLDETECT, %True, 0
    CONTROL SET TEXT hWnd, hCtl, RTF(rt)
    CONTROL POST hWnd, hCtl, %WM_KILLFOCUS, 0, 0  ' Comment that if you want to be able to edit the text (and see a caret)
END SUB
'-----------------------------------------------------------------------------------------------------------------

'-----------------------------------------------------------------------------------------------------------------
SUB RTF_ADDCOL (colnam AS STRING, red AS LONG, green AS LONG, blue AS LONG)
    IF UBOUND(RTF_ColNam) < 0 THEN
        DIM RTF_ColNam(15)
        DIM RTF_ColVal(15)
        ARRAY ASSIGN RTF_ColNam() = _
            "Black", "Maroon", "Green", "Olive", _
            "Navy", "Purple", "Teal", "Grey", _
            "Silver", "Red", "Lime", "Yellow", _
            "Blue", "Fuchsia", "Aqua", "White"
        ARRAY ASSIGN RTF_ColVal() = _
            ";\red0\green0\blue0", _
            ";\red128\green0\blue0", _
            ";\red0\green128\blue0", _
            ";\red128\green128\blue0", _
            ";\red0\green0\blue128", _
            ";\red128\green0\blue128", _
            ";\red0\green128\blue128", _
            ";\red128\green128\blue128", _
            ";\red192\green192\blue192", _
            ";\red255\green0\blue0", _
            ";\red0\green255\blue0", _
            ";\red255\green255\blue0", _
            ";\red0\green0\blue255", _
            ";\red255\green0\blue255", _
            ";\red0\green255\blue255", _
            ";\red255\green255\blue255"
    END IF
    IF colnam <> "" THEN
        REDIM PRESERVE RTF_ColNam(UBOUND(RTF_ColNam) + 1)
        REDIM PRESERVE RTF_ColVal(UBOUND(RTF_ColNam))
        RTF_ColNam(UBOUND(RTF_ColNam)) = colnam
        RTF_ColVal(UBOUND(RTF_ColNam)) = ";\red"  + TRIM$(red) _
                                       + "\green" + TRIM$(green) _
                                       + "\blue"  + TRIM$(blue)
    END IF
END SUB
'-----------------------------------------------------------------------------------------------------------------

'-----------------------------------------------------------------------------------------------------------------
FUNCTION RTF (st AS STRING) AS STRING
    LOCAL fs, hl, i, j AS LONG
    LOCAL rt, bef, p, aft AS STRING

    IF UBOUND(RTF_ColNam) < 0 THEN RTF_AddCol ("",0,0,0)

    rt  = "{\rtf1\ansi\ansicpg1252\deff0\deflang1033\deflangfe1033"+ $CRLF
    rt += "{\fonttbl"
    rt += "{\f0\fmodern\fprq1\fcharset0 Courier New;}"+ $CRLF
    rt += "{\f1\fnil\fcharset0 Times New Roman;}"+ $CRLF
    rt += "{\f2\fmodern\fprq1\fcharset0 Andale Mono;}"+ $CRLF
    rt += "{\f3\fmodern\fprq1\fcharset0 Lucida CONSOLE;}"+ $CRLF
    rt += "{\f4\froman\fprq2\fcharset0 Georgia;}" + $CRLF
    rt += "{\f5\fnil\fcharset0 Open Sans;}}"+ $CRLF
    rt += "{\colortbl "+ $CRLF                 ' cf0  = black
    FOR i = 1 TO UBOUND(RTF_ColNam)
        rt += RTF_ColVal(i) + IIF$(i=UBOUND(RTF_ColNam), ";}", "") + $CRLF
    NEXT
    rt += "\cf1\f0\fs11 " + $CRLF
    rt += StrReplace(st,"\","\\") + "} " ' End of RTF File

    ' [*] for bullet lists
    '===============================================================
    REPLACE "[*]" WITH CHR$(7) + $SPC IN rt

    ' [font:X,size] where 7 <= size <= 72 and X is a 1-letter code
    '===============================================================
    i = INSTR(UCASE$(rt), "[FONT:")
    DO WHILE i
        j = INSTR(i, rt, "]")
        bef = MID$(rt, i, j-i+1)
        fs = VAL(MID$(bef, 9)) ' File Size
        IF fs >= 7 AND fs <= 72 THEN
            p = MID$(bef, 7, 1)
            SELECT CASE UCASE$(p)
                CASE "C"  : aft = "\f0 " ' Courier New     Fixed Size
                CASE "T"  : aft = "\f1 " ' Times New Roman Variable Size
                CASE "A"  : aft = "\f2 " ' Andale Mono     Fixed Size
                CASE "L"  : aft = "\f3 " ' Lucida Console  Fixed Size
                CASE "G"  : aft = "\f4 " ' Georgia Console Fixed Size
                CASE "O"  : aft = "\f5 " ' Open Sans       Variable Size
                CASE ELSE : aft = "\f1 " ' Times New Roman Variable Size
            END SELECT
            aft += "\fs" + TRIM$(2 * fs) + $SPC
            REPLACE bef WITH aft IN rt
        ELSE
            REPLACE bef WITH "" IN rt
        END IF
        i = INSTR(UCASE$(rt), "[FONT:")
    LOOP

    ' [col] where col is a color name belonging to RTF_Col()
    '===============================================================
    FOR j = 0 TO UBOUND(RTF_ColNam)
        i = INSTR(UCASE$(rt), "[" + UCASE$(RTF_ColNam(j)) + "]")
        DO WHILE i
            bef = MID$(rt, i, LEN("[" + RTF_ColNam(j) + "]"))
            p = TRIM$(bef, ANY "[]")
            aft = "\cf" + TRIM$(j) + $SPC
            REPLACE bef WITH aft IN rt
            i = INSTR(UCASE$(rt), "[" + UCASE$(RTF_ColNam(j)) + "]")
        LOOP
    NEXT

    ' [b][/b]     = bold
    '===============================================================
    i = INSTR(UCASE$(rt), "[B]")
    DO WHILE i
        bef = MID$(rt, i, 3)
        aft = "\b "
        REPLACE bef WITH aft IN rt
        i = INSTR(UCASE$(rt), "[B]")
    LOOP
    i = INSTR(UCASE$(rt), "[/B]")
    DO WHILE i
        bef = MID$(rt, i, 4)
        aft = "\b0 "
        REPLACE bef WITH aft IN rt
        i = INSTR(UCASE$(rt), "[/B]")
    LOOP

    ' [i][/i]     = italic
    '===============================================================
    i = INSTR(UCASE$(rt), "[I]")
    DO WHILE i
        bef = MID$(rt, i, 3)
        aft = "\i "
        REPLACE bef WITH aft IN rt
        i = INSTR(UCASE$(rt), "[I]")
    LOOP
    i = INSTR(UCASE$(rt), "[/I]")
    DO WHILE i
        bef = MID$(rt, i, 4)
        aft = "\i0 "
        REPLACE bef WITH aft IN rt
        i = INSTR(UCASE$(rt), "[/I]")
    LOOP

    ' [u][/u]     = underline
    '===============================================================
    i = INSTR(UCASE$(rt), "[U]")
    DO WHILE i
        bef = MID$(rt, i, 3)
        aft = "\ul "
        REPLACE bef WITH aft IN rt
        i = INSTR(UCASE$(rt), "[U]")
    LOOP
    i = INSTR(UCASE$(rt), "[/U]")
    DO WHILE i
        bef = MID$(rt, i, 4)
        aft = "\ul0 "
        REPLACE bef WITH aft IN rt
        i = INSTR(UCASE$(rt), "[/U]")
    LOOP

    ' [h:col][/h] = highlight with color "col" (specified above)
    '===============================================================
    i = INSTR(UCASE$(rt), "[H:")
    DO WHILE i
        j = INSTR(i, rt, "]")
        bef = MID$(rt, i, j-i+1)
        p = MID$(bef, 4) : p = RTRIM$(p, "]")
        fs = -1
        FOR j = 0 TO UBOUND(RTF_ColNam)
            IF UCASE$(p) = UCASE$(RTF_ColNam(j)) THEN
                fs = j
                EXIT FOR
            END IF
        NEXT
        IF fs >= 0 THEN
            aft = "{\highlight" + TRIM$(fs)
            REPLACE bef WITH aft IN rt
        ELSE
            REPLACE bef WITH "" IN rt
        END IF
        i = INSTR(UCASE$(rt), "[H:")
    LOOP
    i = INSTR(UCASE$(rt), "[/H]")
    DO WHILE i
        bef = MID$(rt, i, 4)
        aft = "}"
        REPLACE bef WITH aft IN rt
        i = INSTR(UCASE$(rt), "[/H]")
    LOOP

    ' [c] = centered
    '===============================================================
    i = INSTR(UCASE$(rt), "[C]")
    DO WHILE i
        bef = MID$(rt, i, 3)
        aft = "\pard\qc "
        REPLACE bef WITH aft IN rt
        i = INSTR(UCASE$(rt), "[C]")
    LOOP

    ' [r] = right-aligned
    '===============================================================
    i = INSTR(UCASE$(rt), "[R]")
    DO WHILE i
        bef = MID$(rt, i, 3)
        aft = "\pard\qr "
        REPLACE bef WITH aft IN rt
        i = INSTR(UCASE$(rt), "[R]")
    LOOP

    ' [l] = left-aligned
    '===============================================================
    i = INSTR(UCASE$(rt), "[L]")
    DO WHILE i
        bef = MID$(rt, i, 3)
        aft = "\pard "
        REPLACE bef WITH aft IN rt
        i = INSTR(UCASE$(rt), "[L]")
    LOOP

    ' [j] = justified
    '===============================================================
    i = INSTR(UCASE$(rt), "[J]")
    DO WHILE i
        bef = MID$(rt, i, 3)
        aft = "\pard\qj "
        REPLACE bef WITH aft IN rt
        i = INSTR(UCASE$(rt), "[J]")
    LOOP

    ' [eol] = End Of Line
    '===============================================================
    i = INSTR(UCASE$(rt), "[EOL]")
    DO WHILE i
        bef = MID$(rt, i, 5)
        aft = "\line "
        REPLACE bef WITH aft IN rt
        i = INSTR(UCASE$(rt), "[EOL]")
    LOOP

    ' [eo§] = End Of Paragraph
    '===============================================================
    i = INSTR(UCASE$(rt), "[EOP]")
    DO WHILE i
        bef = MID$(rt, i, 5)
        aft = "\par "
        REPLACE bef WITH aft IN rt
        i = INSTR(UCASE$(rt), "[EOP]")
    LOOP
    i = INSTR(UCASE$(rt), "[EO§]")
    DO WHILE i
        bef = MID$(rt, i, 5)
        aft = "\par "
        REPLACE bef WITH aft IN rt
        i = INSTR(UCASE$(rt), "[EO§]")
    LOOP

    ' [np]  = New Page
    '===============================================================
    i = INSTR(UCASE$(rt), "[NP]")
    DO WHILE i
        bef = MID$(rt, i, 4)
        aft = "\page "
        REPLACE bef WITH aft IN rt
        i = INSTR(UCASE$(rt), "[NP]")
    LOOP

    FUNCTION = rt
END FUNCTION
'-----------------------------------------------------------------------------------------------------------------

'-----------------------------------------------------------------------------------------------------------------
FUNCTION RTF_hyperlink(BYVAL hWnd AS DWORD, BYVAL hCtl AS DWORD, BYVAL lpLink AS DWORD) AS LONG
    LOCAL enlinkPtr AS ENLINK PTR
    LOCAL linkText AS STRING
    LOCAL tr AS TEXTRANGE

    enlinkPtr    = lpLink
    tr.chrg      = @enLinkPtr.chrg
    linkText     = SPACE$(tr.chrg.cpMax - tr.chrg.cpMin + 2)
    tr.lpstrText = STRPTR(linkText)

    CONTROL SEND hWnd, hCtl, %EM_GETTEXTRANGE, 0, VARPTR(tr)

    SELECT CASE @enLinkPtr.Msg
        CASE %WM_LBUTTONDOWN
            ShellExecute(%NULL, "open", BYCOPY linkText, "", "", %SW_SHOW)
            FUNCTION = %True                    ' Signal that we processed this
        CASE %WM_MOUSEMOVE
    END SELECT
END FUNCTION
'-----------------------------------------------------------------------------------------------------------------

'-----------------------------------------------------------------------------------------------------------------
MACRO SUBCLASS(hWnd, hCtl, Fn) = _
  SetProp GetDlgItem(hWnd, hCtl), "OldProc", SetWindowLong(GetDlgItem(hWnd, hCtl), %GWL_WNDPROC, CODEPTR(Fn))
'-----------------------------------------------------------------------------------------------------------------

'-----------------------------------------------------------------------------------------------------------------
FUNCTION RTF_SUBCLASS(BYVAL hWnd AS LONG, BYVAL wMsg AS LONG, BYVAL wParam AS LONG, BYVAL lParam AS LONG) AS LONG
    LOCAL tcr       AS CHARRANGE
    LOCAL OldProc   AS LONG
    LOCAL lRetVal   AS LONG

    OldProc=GetProp(hWnd, "OldProc")

    SELECT CASE wMsg

        CASE %WM_SETFOCUS
            FUNCTION=CallWindowProc(OldProc, hWnd, wMsg, wParam, lParam)
            DestroyCaret
            EXIT FUNCTION

        CASE %WM_KILLFOCUS
          ' The control destroys the caret

        CASE %WM_PAINT
          ' Remove caret the first time the control is painted
          IF GetFocus() = hWnd THEN
            IF GetProp(hWnd, "Initial_Paint") = 0 THEN
              SetProp hWnd, "Initial_Paint", 1
              CallWindowProc(OldProc, hWnd, wMsg, wParam, lParam)
              DestroyCaret
              EXIT FUNCTION
            END IF
          END IF

        CASE %WM_LBUTTONDOWN
          CallWindowProc(OldProc, hWnd, wMsg, wParam, lParam)
          SendMessage hWnd, %EM_EXGETSEL, 0, BYVAL VARPTR(tcr)
          IF tcr.cpMin = tcr.cpMax THEN
            DestroyCaret
          END IF
          EXIT FUNCTION

        CASE %WM_KEYDOWN
          IF GetFocus() = hWnd THEN
            lRetVal = CallWindowProc(OldProc, hWnd, wMsg, wParam, lParam)
            SendMessage hWnd, %EM_EXGETSEL, 0, BYVAL VARPTR(tcr)
            IF tcr.cpMin = tcr.cpMax THEN
              DestroyCaret
            END IF
            FUNCTION = lRetVal
            EXIT FUNCTION
          END IF

        CASE %WM_DESTROY
            RemoveProp hWnd, "Initial_Paint"
            RemoveProp hWnd, "OldProc"
            SetWindowLong hWnd, %GWL_WNDPROC, OldProc

    END SELECT

    FUNCTION=CallWindowProc(OldProc, hWnd, wMsg, wParam, lParam)

END FUNCTION
'-----------------------------------------------------------------------------------------------------------------
