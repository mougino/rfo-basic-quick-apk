
' Change the 2 following constants when including this file, following your needs:
%ACTIVATE_DRAGNDROP_AT_DIALOG_CREATION = %FALSE
%ALLOW_ONLY_SINGLE_FILE_DRAGNDROP      = %TRUE
' Then change the 2 "Change me" places below
' And finally call CB_DRAGNDROP at the very beginning of your Dialog CALLBACK

GLOBAL oDropFile AS DropFileI

MACRO CB_DRAGNDROP
    MACROTEMP i, fileCount, szFile
    SELECT CASE AS LONG CB.MSG

        CASE %WM_INITDIALOG
            oDropFile = CLASS "DropFileC"
            IF %ACTIVATE_DRAGNDROP_AT_DIALOG_CREATION THEN DragAcceptFiles CB.HNDL, %TRUE

        CASE %WM_DROPFILES
            LOCAL i AS LONG
            LOCAL fileCount AS LONG
            LOCAL szFile AS ASCIIZ * %MAX_PATH
            fileCount = DragQueryFile(CB.WPARAM, -1, BYVAL 0, 0)
            IF %ALLOW_ONLY_SINGLE_FILE_DRAGNDROP AND fileCount > 1 THEN
                ' Change me! ***************************************************************
                CONTROL SET TEXT hDlg, 1012, GET_LABEL(13) ' Please drag and drop only 1 file
                CONTROL SHOW STATE hDlg, 1012, %SW_SHOW
                ' Change me! ***************************************************************
                EXIT FUNCTION
            END IF
            FOR i = 0 TO fileCount - 1
                DragQueryFile CB.WPARAM, i, szFile, SIZEOF(szFile)
                oDropFile.OnDropFile(szFile)
            NEXT i
            DragFinish CB.WPARAM

        CASE %WM_DESTROY
            IF %ACTIVATE_DRAGNDROP_AT_DIALOG_CREATION THEN DragAcceptFiles CB.HNDL, %FALSE
    END SELECT
END MACRO

CLASS DropFileC
    INTERFACE DropFileI : INHERIT IUNKNOWN
        METHOD OnDropFile(BYVAL file AS STRING)
            ' Change me! ***************************************************************
            CONTROL SHOW STATE hDlg, 1012, %SW_HIDE
            IF hAPKicon THEN GdipDisposeImage hAPKicon
            GdipLoadImageFromFile UCODE$(file), hAPKicon
            IF hAPKicon = 0 THEN
                CONTROL SET TEXT hDlg, 1012, GET_LABEL(14) ' Invalid image format
                CONTROL SHOW STATE hDlg, 1012, %SW_SHOW
            ELSE
                LOCAL i, j AS LONG
                GdipGetImageWidth hAPKicon, i
                GdipGetImageHeight hAPKicon, j
                IF i <> j THEN
                    CONTROL SET TEXT hDlg, 1012, GET_LABEL(12) ' Pic is not square
                    CONTROL SHOW STATE hDlg, 1012, %SW_SHOW
                    GdipDisposeImage hAPKicon
                ELSE
                    GRAPHIC ATTACH hDlg, 1013
                    GRAPHIC CLEAR %WHITE
                    GdipDrawImageRect hGdip(), hAPKicon, 0, 0, 72, 72
                    GRAPHIC REDRAW
                    app.icon = file + $NUL
                END IF
            END IF
            ' /Change me! **************************************************************
        END METHOD
    END INTERFACE
END CLASS
