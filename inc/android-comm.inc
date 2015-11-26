'##############################################################################
'#
'#  This is one of the include files for << RFO-BASIC! Quick APK >>
'#
'#  "android-comm.inc" handles all the communications with the Android device
'#      - by WiFi, via the Android app "RFO-BASIC! Quick APK (WiFi)"
'#      - by USB cable, via adb (android debug tool)
'#
'##############################################################################

%UDPPORT = 4643
GLOBAL tcpbuf AS STRING

'------------------------------------------------------------------------------
FUNCTION GetConnectedAndroidModelName() AS STRING
    LOCAL i, j, hSocket, servip AS LONG
    LOCAL e, manuf, myname AS STRING

    IF ConnectMode = %USB THEN          ' USB mode
    '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
        RUN_CMD "adb kill-server", EXE.PATH$ + "tools"
        PAUSE 500

        e = DUMP_CMD ("adb shell cat /system/build.prop", EXE.PATH$ + "tools")
        IF INSTR(LCASE$(e), "no android device detected") <> 0 _
        OR INSTR(LCASE$(e), "device unauthorized") <> 0 THEN
            PAUSE 500
            e = DUMP_CMD ("adb shell cat /system/build.prop", EXE.PATH$ + "tools")
        END IF

        i = INSTR(e, "ro.product.brand=")
        IF i = 0 THEN EXIT FUNCTION ' no connected device
        i += LEN("ro.product.brand=")
        j = INSTR(i, e, ANY $CRLF)
        manuf = MID$(e, i, j-i)

        i = INSTR(e, "ro.product.device=")
        IF i = 0 THEN EXIT FUNCTION ' no connected device
        i += LEN("ro.product.device=")
        j = INSTR(i, e, ANY $CRLF)

        FUNCTION = manuf + $SPC + MID$(e, i, j-i)

    ELSEIF ConnectMode = %WIFI THEN       ' WiFi mode
    '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
        HOST NAME 0 TO myname ' get name of this PC on the network
        servip = UDPdetect()
        IF servip = 0 THEN EXIT FUNCTION ' no connected device

        hSocket = FREEFILE ' initiate TCP communication
        TCP OPEN PORT %UDPPORT AT IP_STR(servip) AS #hSocket TIMEOUT 500 : IF ERR THEN EXIT FUNCTION ' no connected device
        TCP PRINT #hSocket, myname : IF ERR THEN TCP CLOSE #hSocket : EXIT FUNCTION ' no connected device
        TCP_RECV   hSocket, tcpbuf : IF ERR THEN TCP CLOSE #hSocket : EXIT FUNCTION ' no connected device
        TCP CLOSE #hSocket
        tcpbuf = REMOVE$(tcpbuf, $CR)
        tcpbuf = RTRIM$ (tcpbuf, $LF) + $LF
        i = INSTR(tcpbuf, $LF)
        IF i = 0 THEN EXIT FUNCTION ' no connected device
        FUNCTION = LEFT$(tcpbuf, i - 1)
        tcpbuf   = MID$ (tcpbuf, i + 1)
        PAUSE 1500

    END IF
END FUNCTION
'------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
MACRO LookForBASICin(p)
    e = DUMP_CMD ("adb shell ls " + p, EXE.PATH$ + "tools")
    IF INSTR(e, "rfo-basic") > 0 THEN sdpath = p
END MACRO
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB RetrieveSDpath
    LOCAL i AS LONG
    LOCAL e AS STRING

    sdpath = ""

    IF ConnectMode = %USB THEN          ' USB mode
    '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
        IF sdpath = "" THEN LookForBASICin("/mnt/sdcard/")
        IF sdpath = "" THEN LookForBASICin("/sdcard/")
        IF sdpath = "" THEN
            e = DUMP_CMD ("adb shell echo $EXTERNAL_STORAGE", EXE.PATH$ + "tools")
            e = RTRIM$(e, "/") + "/"
            LookForBASICin(e)
        END IF

    ELSEIF ConnectMode = %WIFI THEN       ' WiFi mode
    '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
        i = INSTR(tcpbuf, $LF)
        IF i = 0 THEN EXIT SUB
        sdpath = LEFT$(tcpbuf, i - 1)
        tcpbuf = MID$ (tcpbuf, i + 1)

    END IF
END SUB
'--------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB RetrieveFileList
    RetrieveSDpath()
    IF sdpath = "" THEN EXIT SUB ' RFO-BASIC! not found on device
    REDIM device_res(1 TO 1)
'local t0 as double : t0 = timer
    AddToDeviceRes(sdpath)
    REDIM PRESERVE device_res(1 TO UBOUND(device_res) - 1)
'open "timing"+StrReplace(time$,":","")+".txt" for binary as #6 : put$ #6, "Finished ADB listing in "+format$(timer-t0,"#.000")+"s" : close #6
END SUB
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB ParseLs(ls AS STRING) ' Parse the result of a "ls -l -R /sdcard/" command (via ADB or WiFi)
    LOCAL e, t AS STRING  ' And put the result into the array 'device_res()'
    LOCAL sd, rf AS STRING
    LOCAL i, j AS LONG

'i=FREEFILE
'OPEN EXE.PATH$+"ls_dump1.txt" FOR BINARY AS #i
'PUT$ #i, ls
'CLOSE #i

    e = REMOVE$(ls, $CR)
    e = TRIM$(e, $LF) + $LF

    i = INSTR(e, $LF)
    sd = RTRIM$(LEFT$(e, i-1), ":")
    IF sd="" THEN EXIT SUB ' malformed 'ls' result

    i = INSTR(e, sd+"/Android")
    DO WHILE i
        j = INSTR(i, e, $LF+$LF) + 2
        REPLACE MID$(e, i, j-i) WITH "" IN e
        i = INSTR(e, sd+"/Android")
    LOOP
    REPLACE "//" WITH "/" IN e
    REPLACE $LF+$LF WITH $LF IN e

'i=freefile
'open exe.path$+"ls_dump2.txt" for binary as #i
'put$ #i, e
'close #i

    FOR i = 1 TO TALLY(e, $LF) ' 1) List all subfolders
        t = PARSE$(e, $LF, i)
        IF LEFT$(t, 1) = "/" THEN ' root folder e.g. "/sdcard/Alarms:"
            rf = RTRIM$(LEFT$(t, -1), "/") + "/"   ' "/sdcard/Alarms/"
        ELSEIF LEFT$(t, 1) = "d" THEN ' subfolder from root folder
            j = INSTR(-1, t, ":")
            j = INSTR(j, t, $SPC) + 1
            IF MID$(t, j) <> "Android" THEN ' avoid system folder
                device_res(UBOUND(device_res)) = REMOVE$(rf + MID$(t, j) + "/", sdpath)
                REDIM PRESERVE device_res(1 TO UBOUND(device_res) + 1)
            END IF
        END IF
    NEXT
    FOR i = 1 TO TALLY(e, $LF) ' 2) List all files
        t = PARSE$(e, $LF, i)
        IF LEFT$(t, 1) = "/" THEN ' root folder e.g. "/sdcard/Alarms:"
            rf = RTRIM$(LEFT$(t, -1), "/") + "/"   ' "/sdcard/Alarms/"
        ELSEIF LEFT$(t, 1) = "-" THEN ' file from root folder
            j = INSTR(-1, t, ":")
            j = INSTR(j, t, $SPC) + 1
            device_res(UBOUND(device_res)) = REMOVE$(rf + MID$(t, j), sdpath)
            REDIM PRESERVE device_res(1 TO UBOUND(device_res) + 1)
        END IF
    NEXT
END SUB
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
SUB AddToDeviceRes(p AS STRING)
    LOCAL e, t AS STRING
    LOCAL i, j AS LONG

    IF ConnectMode = %USB THEN          ' USB mode
    '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
        e = DUMP_CMD ("adb shell ls -l -R " + $DQ + p + $DQ, EXE.PATH$ + "tools")
        ParseLs (e)

    ELSEIF ConnectMode = %WIFI THEN       ' WiFi mode
    '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
        i = INSTR(tcpbuf, "**full listing**")
        IF i>0 THEN ' Quick APK WiFi for Android v0.2 (uses super fast system call to 'ls -l -R')
            e = MID$(tcpbuf, LEN("**full listing**")+1)
            ParseLs (e)
        ELSE        ' Quick APK WiFi for Android v0.1 (uses slow native RFO-BASIC! 'Dir' function)
            e = tcpbuf
            REDIM device_res(1 TO TALLY(e, $LF) + 1)
            FOR i = 1 TO TALLY(e, $LF)
                t = PARSE$(e, $LF, i)
                ARRAY SCAN device_res(), =t, TO j
                IF j = 0 THEN
                    ARRAY SCAN device_res(), ="", TO j
                    device_res(j) = t
                END IF
            NEXT
            ARRAY SCAN device_res(), ="", TO j
            IF j <> 0 THEN
                REDIM PRESERVE device_res(1 TO j)
            END IF
        END IF

    END IF
END SUB
'------------------------------------------------------------------------------

'------------------------------------------------------------------------------
FUNCTION CopyAndroidFileTo(sourcefile AS STRING, targetpath AS STRING) AS LONG
    LOCAL e, src, folder AS STRING
    LOCAL hSocket, servip AS LONG

    IF TALLY(sourcefile, "/") > 0 THEN folder = LinuxPath(sourcefile)
    REPLACE "/" WITH "\" IN folder
    folder = targetpath + folder
    MakeSureDirectoryPathExists TRIM$(folder)

    src = sourcefile
    IF LEFT$(src,1) <> "/" THEN src = sdpath + src

    IF ConnectMode = %USB THEN          ' USB mode
    '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
        e = DUMP_CMD ("adb pull " + src + $SPC + $DQ + folder + LinuxName(sourcefile) + $DQ, EXE.PATH$ + "tools")

        IF INSTR(LCASE$(e), "bytes") > 0 THEN
            FUNCTION = %TRUE ' pulling the file from android device worked ok
        ELSE
            FUNCTION = %FALSE ' error copying the file
        END IF

    ELSEIF ConnectMode = %WIFI THEN       ' WiFi mode
    '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
        FUNCTION = %FALSE ' by default
        servip = UDPdetect()
        IF servip = 0 THEN EXIT FUNCTION ' nothing on LAN, return error

        hSocket = FREEFILE ' initiate TCP communication
        TCP OPEN PORT %UDPPORT AT IP_STR(servip) AS #hSocket TIMEOUT 500 : IF ERR THEN EXIT FUNCTION
        TCP PRINT #hSocket, "<<" + src : IF ERR THEN TCP CLOSE #hSocket : EXIT FUNCTION
        TCP_RECV   hSocket, tcpbuf     : IF ERR THEN TCP CLOSE #hSocket : EXIT FUNCTION
        IF ISFALSE LEN(tcpbuf) THEN
            TCP PRINT #hSocket, "<<" + src : IF ERR THEN TCP CLOSE #hSocket : EXIT FUNCTION
            TCP_RECV   hSocket, tcpbuf     : IF ERR THEN TCP CLOSE #hSocket : EXIT FUNCTION
        END IF
        TCP CLOSE #hSocket
        IF ISFALSE LEN(tcpbuf) THEN EXIT FUNCTION ' ?"Step 5: Err #"+TRIM$(ERR)+$CR+ERROR$(ERR):

        KILL folder + LinuxName(sourcefile)
        hSocket = FREEFILE
        OPEN folder + LinuxName(sourcefile) FOR BINARY AS #hSocket
            PUT$ #hSocket, tcpbuf
        CLOSE #hSocket
        FUNCTION = %TRUE

    END IF
END FUNCTION
'------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
FUNCTION IP_STR(ip AS LONG) AS STRING
    LOCAL p AS BYTE PTR
    p = VARPTR(ip)
    FUNCTION = USING$("#_.#_.#_.#", @p, @p[1], @p[2], @p[3])
END FUNCTION
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
FUNCTION IP_LNG(ip AS STRING) AS LONG
    FUNCTION = 256^3 * VAL(PARSE$(ip, ".", 4)) _
             + 256^2 * VAL(PARSE$(ip, ".", 3)) _
             + 256^1 * VAL(PARSE$(ip, ".", 2)) _
             + 256^0 * VAL(PARSE$(ip, ".", 1))
END FUNCTION
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
SUB TCP_RECV(pNum AS LONG, buf AS STRING)
    LOCAL e AS STRING
    RESET buf
    DO
        TCP RECV #pNum, 1024, e
        buf += e
    LOOP WHILE LEN(e) AND ISFALSE ERR
END SUB
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
FUNCTION UDPdetect() AS LONG
    LOCAL i, myip, bip, hSocket, servip, servport AS LONG
    STATIC oldservip AS LONG
    LOCAL e AS STRING

    HOST NAME 0 TO e : HOST ADDR e TO myip
    bip = (myip OR &HFF000000) ' broadcast IP

    FOR i = 1 TO 4
        PAUSE 500
        hSocket = FREEFILE ' send UDP-broadcast to detect server on LAN (if any)
        UDP OPEN AS #hSocket TIMEOUT 500
        UDP SEND #hSocket, AT bip, %UDPPORT, "Ping"
        UDP RECV #hSocket, FROM servip, servport, e
        UDP CLOSE #hSocket
        IF ISFALSE ERR AND ISTRUE servip THEN
            oldservip = servip
            FUNCTION = servip
            EXIT FUNCTION
        END IF
    NEXT

    FUNCTION = oldservip

END FUNCTION
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
FUNCTION InstallApkOverWiFi(apk AS STRING) AS LONG
    LOCAL i, myip, bip, servip, hSocket AS LONG
    LOCAL e, file AS STRING

    hSocket = FREEFILE ' get binary content of apk
    OPEN apk FOR BINARY AS #hSocket
        GET$ #hSocket, LOF(#hSocket), file
    CLOSE #hSocket

    servip = UDPdetect()
    IF servip = 0 THEN FUNCTION = %FALSE : EXIT FUNCTION ' nothing on LAN, return error

    hSocket = FREEFILE ' initiate TCP communication
    TCP OPEN PORT %UDPPORT AT IP_STR(servip) AS #hSocket TIMEOUT 500
    TCP PRINT #hSocket, ">>" + TRIM$(LEN(file))
    PAUSE 1000
    i = 1
    DO
        TCP SEND #hSocket, MID$(file, i, 1024)
        i += 1024
    LOOP UNTIL i > LEN(file) OR ISTRUE ERR
    TCP CLOSE #hSocket

    IF ERR THEN FUNCTION = %FALSE ELSE FUNCTION = %TRUE

END FUNCTION
'--------------------------------------------------------------------------------
