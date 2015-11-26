
' Just call CB_SAVEPOS at the very beginning of your Dialog CALLBACK
' to save and restore position of your Dialog upon creation / closing

'------------------------------------------------------------------------------
MACRO POS_FILE = LocalAppdata() + "\" + EXE.NAME$ + ".pos"
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
MACRO CB_SAVEPOS
    SELECT CASE AS LONG CB.MSG
        CASE %WM_INITDIALOG
            LoadDialogPos CB.HNDL
        CASE %WM_SYSCOMMAND
            SaveDialogPos CB.HNDL
    END SELECT
END MACRO
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
%CSIDL_LOCAL_APPDATA  = &H001c ' <user name>\Local Settings\Application Data (non roaming)
DECLARE FUNCTION SHGetFolderPath LIB "Shell32.dll" _
    ALIAS "SHGetFolderPathA" (BYVAL hwnd AS DWORD, BYVAL csidl AS LONG, _
    BYVAL hToken AS DWORD, BYVAL dwFlags AS DWORD, pszPath AS ASCIIZ) AS LONG
FUNCTION LocalAppdata AS STRING
    LOCAL szBaseFolder AS ASCIIZ * %MAX_PATH
    ShGetFolderPath (BYVAL %NULL, _             ' hWnd, "typically null"
                     %CSIDL_LOCAL_APPDATA, _    ' (supported) CSIDL value
                     BYVAL %NULL, _             ' hToken for security, null  = Default
                     BYVAL 0, _                 ' above
                     szBaseFolder)
    FUNCTION = TRIM$(szBaseFolder)
END FUNCTION
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB SaveDialogPos(hDlg AS DWORD)
    LOCAL ff, x, y AS LONG
    IF IsIconic(hDlg) THEN EXIT SUB
    DIALOG GET LOC hDlg TO x, y
    ff = FREEFILE
    OPEN POS_FILE FOR OUTPUT AS #ff
        PRINT #ff, x
        PRINT #ff, y
    CLOSE #ff
END SUB
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB LoadDialogPos(hDlg AS DWORD)
    LOCAL ff, x, y AS LONG
    LOCAL e AS STRING
    IF NOT EXIST(POS_FILE) THEN EXIT SUB
    ff = FREEFILE
    OPEN POS_FILE FOR INPUT AS #ff
        LINE INPUT #ff, e : x = VAL(e)
        LINE INPUT #ff, e : y = VAL(e)
    CLOSE #ff
    DIALOG SET LOC hDlg, x, y
END SUB
'------------------------------------------------------------------------------
