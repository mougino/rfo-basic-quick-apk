' Get lines or raw-file from a Unix ($Lf delimiter) or a Windows ($Cr or $CrLf delimiter) text file
' See macros below for use in a program:
'    UW_DeclareLocals    ' Local variables for Unix/Windows file-access macros
'    UW_GetLinesFrom(filepath)
'    FOR i = 0 to UW_NbOfLines
'        e += UW_Line(i) + UW_NewLineChr
'    NEXT i
'
'    ' Replace a text file with an upper-case version of itself
'    UW_GetRawFile(filepath)
'    uwBuff = UCASE$(uwBuff)
'    UW_SaveRawFile(filepath)

%ADD_BUF      = 100

'--------------------------------------------------------------------------------
MACRO UW_DeclareLocals
    LOCAL uwBuff AS STRING
    LOCAL uwLine() AS LONG
    LOCAL pz AS ASCIIZ PTR
END MACRO
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
MACRO UW_GetLinesFrom(filepath)
    REDIM uwLine(0)
    GetUWLines filepath, uwBuff, uwLine()
END MACRO
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
MACRO UW_NbOfLines = UBOUND(uwLine)
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
MACRO FUNCTION UW_Line(i)
    pz = uwLine(i)
END MACRO = TRIM$(@pz)
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
MACRO UW_GetRawFile(file) = uwBuff = "" : OpenUWFile(file, uwBuff)
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
MACRO UW_SaveRawFile(file)
    MACROTEMP hf
    KILL file
    DIM hf AS LONG
    hf = FREEFILE
    OPEN file FOR BINARY AS #hf
        PUT$ #hf, uwBuff
    CLOSE #hf
END MACRO
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
MACRO UW_NewLineChr = IIF$(INSTR(uwBuff, $CR)=0, "", $CR) + $LF
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
FUNCTION OpenUWFile(BYVAL File AS STRING, Buffer AS STRING) AS LONG
' Opens a Unix ($Lf delimiter) or Windows ($Cr or $CrLf delimiter) file for use with GetUWLines

    LOCAL hFile AS LONG

    hFile = FREEFILE
    OPEN File FOR BINARY ACCESS READ LOCK SHARED AS hFile
    IF ERR THEN EXIT FUNCTION
    GET$ hFile, LOF(hFile), Buffer
    CLOSE hFile

    FUNCTION = 1

END FUNCTION
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
FUNCTION GetUWLines(BYVAL File AS STRING, BYREF Buffer AS STRING, pLine() AS LONG) AS LONG
' Get lines from Unix ($Lf delimiter) or Windows ($Cr or $CrLf delimiter) file

    LOCAL d AS LONG, p AS LONG, i AS LONG, n AS LONG
    LOCAL pBuf AS LONG

    IF OpenUWFile(File, Buffer) = 0 THEN EXIT FUNCTION

    REPLACE $CRLF WITH CHR$(0)IN Buffer
    REPLACE $LF WITH CHR$(0)IN Buffer
    REPLACE $CR WITH $LF IN Buffer

    d = %ADD_BUF : REDIM pLine(d)

    pBuf = STRPTR(Buffer)
    pLine(0) = pBuf

    DO
        n = (n + 1) MOD 100
        p = INSTR(p+1, Buffer, CHR$(0))
        IF p = 0 THEN EXIT LOOP
        INCR i
        IF i > d THEN
            d = d + %ADD_BUF : REDIM PRESERVE pLine(d)
        END IF
        pLine(i) = p + pBuf
    LOOP

    REDIM PRESERVE pLine(i)

    FUNCTION = 1

END FUNCTION
'--------------------------------------------------------------------------------
