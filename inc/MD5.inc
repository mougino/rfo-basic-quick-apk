'md5.inc
'based by Florent Heyworth
'
' Usage:    ' For a String
'           Local md5c As MD5_CTX
'           md5_Init VarPtr(md5c)
'           md5_Update VarPtr(md5c), StrPtr(myString), Len(myString) ' Beware : *STRPTR* and not VARPTR
'           MsgBox md5_Digest(VarPtr(md5c)),, "MD5"
' Or:
'           ' For a File
'           Local md5c As MD5_CTX
'           Local filename As Asciiz * %max_path
'           md5_Init VarPtr(md5c)
'           md5_readFile VarPtr(md5c), filename
'           MsgBox md5_Digest(VarPtr(md5c)),, "MD5"

#INCLUDE ONCE "WINDOWS.INC"


' Constants FOR md5_Transform routine.
%S11 = 7
%S12 = 12
%S13 = 17
%S14 = 22
%S21 = 5
%S22 = 9
%S23 = 14
%S24 = 20
%S31 = 4
%S32 = 11
%S33 = 16
%S34 = 23
%S41 = 6
%S42 = 10
%S43 = 15
%S44 = 21

TYPE MD5_Bytes
    b00      AS BYTE
    b01      AS BYTE
    b02      AS BYTE
    b03      AS BYTE
    b04      AS BYTE
    b05      AS BYTE
    b06      AS BYTE
    b07      AS BYTE
    b08      AS BYTE
    b09      AS BYTE
    b10      AS BYTE
    b11      AS BYTE
    b12      AS BYTE
    b13      AS BYTE
    b14      AS BYTE
    b15      AS BYTE
END TYPE

TYPE MD5_CTX
    lState(4) AS LONG 'state (ABCD)
    lCount(2) AS LONG 'number of bits modulo 2^64 (lsp first)
    bBuf AS ASCIIZ * 64 'input buffer
END TYPE

' PB really should have BUILT-IN function for shift instead of SHIFT LEFT|RIGHT semantics....
FUNCTION md5_shiftRight(BYVAL lThis AS LONG, BYVAL lBits AS LONG) AS LONG

    SHIFT RIGHT lThis, lBits
    FUNCTION = lThis

END FUNCTION

FUNCTION md5_shiftLeft(BYVAL lThis AS LONG, BYVAL lBits AS LONG) AS LONG

    SHIFT LEFT lThis, lBits
    FUNCTION = lThis

END FUNCTION

SUB md5_init( BYVAL tContext AS MD5_CTX PTR )
    'MD5 initialization. Begins an MD5 operation, writing a new context
    @tContext.lCount(0) = 0
    @tContext.lCount(1) = 0

    ' Load magic initialization constants.
    @tContext.lState(0) = &H67452301
    @tContext.lState(1) = &HEFCDAB89
    @tContext.lState(2) = &H98BADCFE
    @tContext.lState(3) = &H10325476

END SUB

SUB md5_Transform (BYVAL tContext AS MD5_CTX PTR, BYVAL dwBuf AS DWORD )

    LOCAL a AS LONG
    LOCAL b AS LONG
    LOCAL c AS LONG
    LOCAL d AS LONG
    LOCAL bBuf AS LONG PTR

    bBuf = dwBuf
    a = @tContext.lState(0)
    b = @tContext.lState(1)
    c = @tContext.lState(2)
    d = @tContext.lState(3)


    'ROUND 1
    a = a + ((((b)) AND ((c))) OR ((NOT (b)) AND ((d)))) + (@bBuf[ 0]) +&Hd76aa478
    ROTATE LEFT a, %S11: a = a + b
    d = d + ((((a)) AND ((b))) OR ((NOT (a)) AND ((c)))) + (@bBuf[ 1]) +&He8c7b756
    ROTATE LEFT d, %S12: d = d + a
    c = c + ((((d)) AND ((a))) OR ((NOT (d)) AND ((b)))) + (@bBuf[ 2]) +&H242070db
    ROTATE LEFT c, %S13: c = c + d
    b = b + ((((c)) AND ((d))) OR ((NOT (c)) AND ((a)))) + (@bBuf[ 3]) +&Hc1bdceee
    ROTATE LEFT b, %S14: b = b + c
    '
    a = a + ((((b)) AND ((c))) OR ((NOT (b)) AND ((d)))) + (@bBuf[ 4]) +&Hf57c0faf
    ROTATE LEFT a, %S11: a = a + b
    d = d + ((((a)) AND ((b))) OR ((NOT (a)) AND ((c)))) + (@bBuf[ 5]) +&H4787c62a
    ROTATE LEFT d, %S12: d = d + a
    c = c + ((((d)) AND ((a))) OR ((NOT (d)) AND ((b)))) + (@bBuf[ 6]) +&Ha8304613
    ROTATE LEFT c, %S13: c = c + d
    b = b + ((((c)) AND ((d))) OR ((NOT (c)) AND ((a)))) + (@bBuf[ 7]) +&Hfd469501
    ROTATE LEFT b, %S14: b = b + c
    '
    a = a + ((((b)) AND ((c))) OR ((NOT (b)) AND ((d)))) + (@bBuf[ 8]) +&H698098d8
    ROTATE LEFT a, %S11: a = a + b
    d = d + ((((a)) AND ((b))) OR ((NOT (a)) AND ((c)))) + (@bBuf[ 9]) +&H8b44f7af
    ROTATE LEFT d, %S12: d = d + a
    c = c + ((((d)) AND ((a))) OR ((NOT (d)) AND ((b)))) + (@bBuf[10]) +&Hffff5bb1
    ROTATE LEFT c, %S13: c = c + d
    b = b + ((((c)) AND ((d))) OR ((NOT (c)) AND ((a)))) + (@bBuf[11]) +&H895cd7be
    ROTATE LEFT b, %S14: b = b + c
    '
    a = a + ((((b)) AND ((c))) OR ((NOT (b)) AND ((d)))) + (@bBuf[12]) +&H6b901122
    ROTATE LEFT a, %S11: a = a + b
    d = d + ((((a)) AND ((b))) OR ((NOT (a)) AND ((c)))) + (@bBuf[13]) +&Hfd987193
    ROTATE LEFT d, %S12: d = d + a
    c = c + ((((d)) AND ((a))) OR ((NOT (d)) AND ((b)))) + (@bBuf[14]) +&Ha679438e
    ROTATE LEFT c, %S13: c = c + d
    b = b + ((((c)) AND ((d))) OR ((NOT (c)) AND ((a)))) + (@bBuf[15]) +&H49b40821
    ROTATE LEFT b, %S14: b = b + c

    'ROUND 2
    a = a + ((((b)) AND ((d))) OR (((c)) AND (NOT (d)))) + (@bBuf[ 1]) +&Hf61e2562
    ROTATE LEFT a, %S21: a = a + b
    d = d + ((((a)) AND ((c))) OR (((b)) AND (NOT (c)))) + (@bBuf[ 6]) +&Hc040b340
    ROTATE LEFT d, %S22: d = d + a
    c = c + ((((d)) AND ((b))) OR (((a)) AND (NOT (b)))) + (@bBuf[11]) +&H265e5a51
    ROTATE LEFT c, %S23: c = c + d
    b = b + ((((c)) AND ((a))) OR (((d)) AND (NOT (a)))) + (@bBuf[ 0]) +&He9b6c7aa
    ROTATE LEFT b, %S24: b = b + c
    '
    a = a + ((((b)) AND ((d))) OR (((c)) AND (NOT (d)))) + (@bBuf[ 5]) +&Hd62f105d
    ROTATE LEFT a, %S21: a = a + b
    d = d + ((((a)) AND ((c))) OR (((b)) AND (NOT (c)))) + (@bBuf[10]) +&H2441453
    ROTATE LEFT d, %S22: d = d + a
    c = c + ((((d)) AND ((b))) OR (((a)) AND (NOT (b)))) + (@bBuf[15]) +&Hd8a1e681
    ROTATE LEFT c, %S23: c = c + d
    b = b + ((((c)) AND ((a))) OR (((d)) AND (NOT (a)))) + (@bBuf[ 4]) +&He7d3fbc8
    ROTATE LEFT b, %S24: b = b + c
    '
    a = a + ((((b)) AND ((d))) OR (((c)) AND (NOT (d)))) + (@bBuf[ 9]) +&H21e1cde6
    ROTATE LEFT a, %S21: a = a + b
    d = d + ((((a)) AND ((c))) OR (((b)) AND (NOT (c)))) + (@bBuf[14]) +&Hc33707d6
    ROTATE LEFT d, %S22: d = d + a
    c = c + ((((d)) AND ((b))) OR (((a)) AND (NOT (b)))) + (@bBuf[ 3]) +&Hf4d50d87
    ROTATE LEFT c, %S23: c = c + d
    b = b + ((((c)) AND ((a))) OR (((d)) AND (NOT (a)))) + (@bBuf[ 8]) +&H455a14ed
    ROTATE LEFT b, %S24: b = b + c
    '
    a = a + ((((b)) AND ((d))) OR (((c)) AND (NOT (d)))) + (@bBuf[13]) +&Ha9e3e905
    ROTATE LEFT a, %S21: a = a + b
    d = d + ((((a)) AND ((c))) OR (((b)) AND (NOT (c)))) + (@bBuf[ 2]) +&Hfcefa3f8
    ROTATE LEFT d, %S22: d = d + a
    c = c + ((((d)) AND ((b))) OR (((a)) AND (NOT (b)))) + (@bBuf[ 7]) +&H676f02d9
    ROTATE LEFT c, %S23: c = c + d
    b = b + ((((c)) AND ((a))) OR (((d)) AND (NOT (a)))) + (@bBuf[12]) +&H8d2a4c8a
    ROTATE LEFT b, %S24: b = b + c


    'ROUND 3
    a = a + (((b)) XOR ((c)) XOR ((d))) + (@bBuf[ 5]) +&Hfffa3942
    ROTATE LEFT a, %S31: a = a + b
    d = d + (((a)) XOR ((b)) XOR ((c))) + (@bBuf[ 8]) +&H8771f681
    ROTATE LEFT d, %S32: d = d + a
    c = c + (((d)) XOR ((a)) XOR ((b))) + (@bBuf[11]) +&H6d9d6122
    ROTATE LEFT c, %S33: c = c + d
    b = b + (((c)) XOR ((d)) XOR ((a))) + (@bBuf[14]) +&Hfde5380c
    ROTATE LEFT b, %S34: b = b + c
    '
    a = a + (((b)) XOR ((c)) XOR ((d))) + (@bBuf[ 1]) +&Ha4beea44
    ROTATE LEFT a, %S31: a = a + b
    d = d + (((a)) XOR ((b)) XOR ((c))) + (@bBuf[ 4]) +&H4bdecfa9
    ROTATE LEFT d, %S32: d = d + a
    c = c + (((d)) XOR ((a)) XOR ((b))) + (@bBuf[ 7]) +&Hf6bb4b60
    ROTATE LEFT c, %S33: c = c + d
    b = b + (((c)) XOR ((d)) XOR ((a))) + (@bBuf[10]) +&Hbebfbc70
    ROTATE LEFT b, %S34: b = b + c
    '
    a = a + (((b)) XOR ((c)) XOR ((d))) + (@bBuf[13]) +&H289b7ec6
    ROTATE LEFT a, %S31: a = a + b
    d = d + (((a)) XOR ((b)) XOR ((c))) + (@bBuf[ 0]) +&Heaa127fa
    ROTATE LEFT d, %S32: d = d + a
    c = c + (((d)) XOR ((a)) XOR ((b))) + (@bBuf[ 3]) +&Hd4ef3085
    ROTATE LEFT c, %S33: c = c + d
    b = b + (((c)) XOR ((d)) XOR ((a))) + (@bBuf[ 6]) +&H4881d05
    ROTATE LEFT b, %S34: b = b + c
    '
    a = a + (((b)) XOR ((c)) XOR ((d))) + (@bBuf[ 9]) +&Hd9d4d039
    ROTATE LEFT a, %S31: a = a + b
    d = d + (((a)) XOR ((b)) XOR ((c))) + (@bBuf[12]) +&He6db99e5
    ROTATE LEFT d, %S32: d = d + a
    c = c + (((d)) XOR ((a)) XOR ((b))) + (@bBuf[15]) +&H1fa27cf8
    ROTATE LEFT c, %S33: c = c + d
    b = b + (((c)) XOR ((d)) XOR ((a))) + (@bBuf[ 2]) +&Hc4ac5665
    ROTATE LEFT b, %S34: b = b + c

    'ROUND 4
    a = a + (((c)) XOR (((b)) OR (NOT (d)))) + (@bBuf[ 0]) +&Hf4292244
    ROTATE LEFT a, %S41: a = a + b
    d = d + (((b)) XOR (((a)) OR (NOT (c)))) + (@bBuf[ 7]) +&H432aff97
    ROTATE LEFT d, %S42: d = d + a
    c = c + (((a)) XOR (((d)) OR (NOT (b)))) + (@bBuf[14]) +&Hab9423a7
    ROTATE LEFT c, %S43: c = c + d
    b = b + (((d)) XOR (((c)) OR (NOT (a)))) + (@bBuf[ 5]) +&Hfc93a039
    ROTATE LEFT b, %S44: b = b + c
    '
    a = a + (((c)) XOR (((b)) OR (NOT (d)))) + (@bBuf[12]) +&H655b59c3
    ROTATE LEFT a, %S41: a = a + b
    d = d + (((b)) XOR (((a)) OR (NOT (c)))) + (@bBuf[ 3]) +&H8f0ccc92
    ROTATE LEFT d, %S42: d = d + a
    c = c + (((a)) XOR (((d)) OR (NOT (b)))) + (@bBuf[10]) +&Hffeff47d
    ROTATE LEFT c, %S43: c = c + d
    b = b + (((d)) XOR (((c)) OR (NOT (a)))) + (@bBuf[ 1]) +&H85845dd1
    ROTATE LEFT b, %S44: b = b + c
    '
    a = a + (((c)) XOR (((b)) OR (NOT (d)))) + (@bBuf[ 8]) +&H6fa87e4f
    ROTATE LEFT a, %S41: a = a + b
    d = d + (((b)) XOR (((a)) OR (NOT (c)))) + (@bBuf[15]) +&Hfe2ce6e0
    ROTATE LEFT d, %S42: d = d + a
    c = c + (((a)) XOR (((d)) OR (NOT (b)))) + (@bBuf[ 6]) +&Ha3014314
    ROTATE LEFT c, %S43: c = c + d
    b = b + (((d)) XOR (((c)) OR (NOT (a)))) + (@bBuf[13]) +&H4e0811a1
    ROTATE LEFT b, %S44: b = b + c
    '
    a = a + (((c)) XOR (((b)) OR (NOT (d)))) + (@bBuf[ 4]) +&Hf7537e82
    ROTATE LEFT a, %S41: a = a + b
    d = d + (((b)) XOR (((a)) OR (NOT (c)))) + (@bBuf[11]) +&Hbd3af235
    ROTATE LEFT d, %S42: d = d + a
    c = c + (((a)) XOR (((d)) OR (NOT (b)))) + (@bBuf[ 2]) +&H2ad7d2bb
    ROTATE LEFT c, %S43: c = c + d
    b = b + (((d)) XOR (((c)) OR (NOT (a)))) + (@bBuf[ 9]) +&Heb86d391
    ROTATE LEFT b, %S44: b = b + c

    @tContext.lState(0) = @tContext.lState(0) + a
    @tContext.lState(1) = @tContext.lState(1) + b
    @tContext.lState(2) = @tContext.lState(2) + c
    @tContext.lState(3) = @tContext.lState(3) + d

END SUB

' Update context TO reflect the concatenation of another buffer full
' of bytes.
SUB md5_Update(BYVAL ctx AS MD5_CTX PTR, BYVAL bBuf AS BYTE PTR, BYVAL dwLen AS DWORD )

    LOCAL t AS DWORD

    ' Update bitcount
    t = @ctx.lCount(0)
    @ctx.lCount(0) = t + md5_shiftLeft( dwLen, 3 )
    IF @ctx.lCount(0) < t THEN
       INCR @ctx.lCount(1) 'carry from low to high
    END IF

    @ctx.lCount(1) = @ctx.lCount(1) + md5_shiftRight( dwLen, 29 )

    t = ( md5_shiftRight( t, 3) AND &H3F ) 'Bytes already IN shsInfo->DATA

    'handle any leading odd-sized chunks
    IF ISTRUE(t) THEN
        LOCAL p AS BYTE PTR

        p = VARPTR(@ctx.bBuf)
        p = p + t
        t = 64 - t
        IF dwLen < t THEN
            CALL MoveMemory(BYVAL p, BYVAL bBuf, dwLen )
            EXIT SUB
        END IF
        CALL MoveMemory(BYVAL p, BYVAL bBuf, t )
        CALL md5_Transform( ctx, VARPTR(@ctx.bBuf) )
        bBuf = bBuf + t
        dwLen = dwLen - t
    END IF

    'Process DATA IN 64-BYTE chunks
    DO WHILE (dwLen >= 64)
        CALL MoveMemory( BYVAL VARPTR(@ctx.bBuf), BYVAL bBuf, 64)
        CALL md5_Transform( ctx, VARPTR(@ctx.bBuf) )
        bBuf = bBuf + 64
        dwLen = dwLen - 64
    LOOP

    'handle any remaining bytes of data
    CALL MoveMemory(BYVAL VARPTR(@ctx.bBuf), BYVAL bBuf, dwLen )

END SUB

' Final wrapup - pad TO 64-BYTE boundary WITH the BIT pattern
' 1 0* (64-BIT count of bits processed, MSB-first)
SUB md5_Final( BYVAL dwDigest AS BYTE PTR, BYVAL ctx AS MD5_CTX PTR )
    LOCAL lCount AS DWORD
    LOCAL p AS BYTE PTR
    LOCAL bDigest AS BYTE PTR
    LOCAL bBuffer AS BYTE PTR

    bDigest = dwDigest
    bBuffer = VARPTR(@ctx.bBuf)
    'Compute number of bytes MOD 64
    lCount = (md5_shiftRight( @ctx.lCount(0), 3) AND &H3F )

    'set the first char of padding to &H80.  This is safe since there is
    'always AT least one BYTE free
    p = VARPTR(@ctx.bBuf)
    p = p + lCount
    @p = &H80
    INCR p

    ' Bytes of padding needed TO make 64 bytes
    lCount = 64 - 1 - lCount

    ' Pad out TO 56 MOD 64
    IF lCount < 8 THEN
        'two lots of padding: pad the first block to 64 bytes
        CALL FillMemory( BYVAL p, lCount, 0 )
        CALL md5_Transform( ctx, VARPTR(@ctx.bBuf) )
        CALL FillMemory( BYVAL VARPTR(@ctx.bBuf), 56, 0 )
    ELSE
        'pad block to 56 bytes
        CALL FillMemory( p, (lCount - 8), 0)
    END IF

    'append length IN bits and transform
    CALL MoveMemory( BYVAL VARPTR(@bBuffer[14 * SIZEOF(lCount)]), BYVAL VARPTR(@ctx.lCount(0)), SIZEOF(lCount))
    CALL MoveMemory( BYVAL VARPTR(@bBuffer[15 * SIZEOF(lCount)]), BYVAL VARPTR(@ctx.lCount(1)), SIZEOF(lCount))

    CALL md5_Transform( ctx, VARPTR(@ctx.bBuf) )
    CALL MoveMemory( BYVAL bDigest, BYVAL VARPTR(@ctx.lState(0)), 16)
    'Zeroise sensitive information
    CALL FillMemory( ctx, SIZEOF(@ctx), 0)

END SUB

FUNCTION md5_ByteType( BYVAL pContext AS MD5_CTX PTR, BYREF bt AS MD5_Bytes) AS LONG
   DIM szDigest AS ASCIIZ * 16
   DIM bDigest AS BYTE PTR
   DIM sCheck AS STRING
   DIM i AS LONG

   bDigest = VARPTR(szDigest)
   'pContext = dwContext

   CALL md5_Final( bDigest, pContext )

   IF bDigest <> %NULL THEN
        bt.b00 = @bDigest[0]
        bt.b01 = @bDigest[1]
        bt.b02 = @bDigest[2]
        bt.b03 = @bDigest[3]
        bt.b04 = @bDigest[4]
        bt.b05 = @bDigest[5]
        bt.b06 = @bDigest[6]
        bt.b07 = @bDigest[7]
        bt.b08 = @bDigest[8]
        bt.b09 = @bDigest[9]
        bt.b10 = @bDigest[10]
        bt.b11 = @bDigest[11]
        bt.b12 = @bDigest[12]
        bt.b13 = @bDigest[13]
        bt.b14 = @bDigest[14]
        bt.b15 = @bDigest[15]
   END IF
   FUNCTION = 0

END FUNCTION

FUNCTION md5_Digest( BYVAL pContext AS MD5_CTX PTR ) AS STRING
   'returns the MD5 digest as a hex string
   DIM sHex AS STRING
   'Dim pContext As MD5_CTX Ptr
   DIM szDigest AS ASCIIZ * 16
   DIM bDigest AS BYTE PTR
   DIM sCheck AS STRING
   DIM i AS LONG

   bDigest = VARPTR(szDigest)
   'pContext = dwContext

   CALL md5_Final( bDigest, pContext )

   IF bDigest <> %NULL THEN
      FOR i = 0 TO 15
        sHex = sHex + HEX$(@bDigest[i],2)
      NEXT
   END IF
   FUNCTION = sHex

END FUNCTION

FUNCTION md5_String( BYVAL pContext AS MD5_CTX PTR ) AS STRING
   'returns the MD5 digest as a hex string
   DIM s AS STRING
   'Dim pContext As MD5_CTX Ptr
   DIM szDigest AS ASCIIZ * 16
   DIM bDigest AS BYTE PTR
   DIM sCheck AS STRING
   DIM i AS LONG

   bDigest = VARPTR(szDigest)
   'pContext = dwContext

   CALL md5_Final( bDigest, pContext )

   IF bDigest <> %NULL THEN
      FOR i = 0 TO 15
        s = s + CHR$(@bDigest[i])
      NEXT
   END IF
   FUNCTION = s

END FUNCTION


SUB md5_readFile( BYVAL dwContext AS DWORD, szFile AS ASCIIZ )
   'reads a file and calls md5_Update on the buffer
   'md5_Init() MUST have been called prior to calling
   'this function - call md5_Digest() afterwards to
   'get the fingerprint
   LOCAL lHandle AS LONG
   LOCAL szBuffer AS ASCIIZ * 1024
   LOCAL lBufLen AS LONG
   LOCAL lLen AS LONG
   LOCAL lResult AS LONG
   LOCAL lToRead AS LONG
   LOCAL lToReadHigh AS LONG
   LOCAL pContext AS MD5_CTX PTR
   LOCAL lpBuffer AS LONG

   pContext = dwContext
   lBufLen = SIZEOF(szBuffer)

   lpBuffer = VARPTR(szBuffer)
   lHandle = CreateFile(szFile, %GENERIC_READ, %FILE_SHARE_READ, _
                          BYVAL %NULL, %OPEN_EXISTING, %FILE_ATTRIBUTE_NORMAL, 0&)

   IF lHandle <> %INVALID_HANDLE_VALUE THEN
      lToRead = GetFileSize(lHandle, lToReadHigh)
      DO WHILE lToRead > 0
         lResult = ReadFile(lHandle, BYVAL lpBuffer, lBufLen, lLen , BYVAL %NULL)
         CALL md5_Update(pContext, lpBuffer, lLen)
         ! cmp lResult, 0
         ! je md5_readFile_close
         ! mov eax, lToRead
         ! sub eax, lLen
         ! mov lToRead, eax
      LOOP
      md5_readFile_close:
      CALL CloseHandle(lHandle)
   END IF

END SUB
