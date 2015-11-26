'##############################################################################
'#
'#  This is one of the include files for << RFO-BASIC! Quick APK >>
'#
'#  "ColorTabs.inc" contains methods to display a Colored Tab Control
'#
'##############################################################################
'  Usage:
'  ------
'  GLOBAL ot AS ColorTabsClass
'  DIALOG NEW PIXELS, 0, "Test Color Tabs", 0, 0, 400, 300, %WS_SYSMENU OR %DS_CENTER TO hDlg
'  ot = CLASS "ColorTabs"
'  ot.StartTab (hDlg, %ID_TAB, 5, 10, 390, 280, %RED, %RGB_LIME)
'  hTab = ot.AddPage ("First Tab", RGB(120,120,0), %RGB_LIGHTPINK)
'
%TCM_FIRST        = &H1300      ' Tab control messages
%TCM_GETCURSEL    = %TCM_FIRST + 11
%TCM_GETITEM      = %TCM_FIRST + 5
'
TYPE TC_ITEM
    Mask          AS DWORD
    dwState       AS DWORD
    dwStateMask   AS DWORD
    pszText       AS ASCIIZ PTR
    cchTextMax    AS LONG
    iImage        AS LONG
    lParam        AS LONG
END TYPE
'
TYPE tpage
    pageno AS LONG    ' number of page within tab
    hpage AS DWORD    ' window handle
    txt AS ASCIZ * 50 ' tab text
    textcolor AS LONG
    bkcolor AS LONG
    brush AS DWORD    ' background color brush
END TYPE
'
FUNCTION TabCtrl_GetItem (BYVAL hwnd AS DWORD, BYVAL iItem AS DWORD, pItem AS TC_ITEM) AS LONG
    FUNCTION = SendMessage(hwnd, %TCM_GETITEM, iItem, VARPTR(pItem))
END FUNCTION
'
GLOBAL prevTabProc AS LONG
GLOBAL TabBgndCol  AS LONG
FUNCTION TabSubclassProc(BYVAL hwnd AS LONG, BYVAL msg AS LONG, BYVAL wparam AS LONG, BYVAL lparam AS LONG) AS LONG
    LOCAL hdc, hbrush AS LONG
    LOCAL rc AS rect
    SELECT CASE msg
        CASE %WM_ERASEBKGND
            hdc = wparam
            getclientrect hwnd,rc
            hbrush = createsolidbrush(TabBgndCol)
            fillrect hdc, rc, hbrush
            deleteobject hbrush
            FUNCTION=1
            EXIT FUNCTION
    END SELECT
    FUNCTION = callwindowproc(prevTabProc, hwnd, msg, wparam, lparam)
END FUNCTION
'
MACRO CB_COLORTABS
    SELECT CASE CB.MSG
        CASE %WM_DRAWITEM : IF ot.drawitem (BYVAL CB.HNDL, BYVAL CB.CTL, BYVAL CB.WPARAM, BYVAL CB.LPARAM) THEN EXIT SELECT
        CASE %WM_NOTIFY   : IF oT.notification_from_tab (BYVAL CB.HNDL, BYVAL CB.LPARAM) THEN EXIT SELECT
    END SELECT
END MACRO
'
CLASS ColorTabs
' credits: Borje Hagsten, Charles Dietz
    INSTANCE lTabId AS DWORD                ' handle of this tab
    INSTANCE hparent AS DWORD               ' handle of parent dialog
    INSTANCE X AS LONG: INSTANCE Y AS LONG: INSTANCE W AS LONG: INSTANCE H AS LONG    'coordinates
    INSTANCE PAGES() AS TPAGE               ' array of pages in this tab
    INSTANCE seltextcolor AS LONG           ' text color of a selected tab
    '-----------------------------
    CLASS METHOD CREATE
    END METHOD
    CLASS METHOD DESTROY
        SETWINDOWLONG GetDlgItem(hparent, ltabId), _ ' Remove subclass
            %GWL_WNDPROC, prevTabProc
        LOCAL i AS LONG
        FOR i = LBOUND(PAGES) TO UBOUND(PAGES)
            DeleteObject PAGES(i).brush
        NEXT
    END METHOD
    '-----------------------------
INTERFACE ColorTabsClass : INHERIT IUNKNOWN
    '-----------------------------
    ' parameters: parent dialog, TAB control id, coordinates XYWH, color of tab text for selected tab, color of background
    METHOD StartTab(hD AS DWORD, lctlid AS LONG, _
                    in_X AS LONG, in_Y AS LONG, in_W AS LONG, in_H AS LONG, _
                    in_seltextcolor AS LONG, in_bkgndcolor AS LONG)
        hparent = hD
        ltabId  = lCtlId
        X = in_X: Y  = in_Y: W = in_W: H = in_H
        seltextcolor = in_seltextcolor
        TabBgndCol   = in_bkgndcolor
        CONTROL ADD TAB,  hparent, ltabId, "", X, Y, W - 10, H-20, _
            %WS_CHILD OR %WS_VISIBLE OR %WS_TABSTOP OR %TCS_TOOLTIPS OR _
            %TCS_FOCUSONBUTTONDOWN OR %TCS_OWNERDRAWFIXED
        CONTROL SET COLOR hparent, ltabId, -1, TabBgndCol
        prevTabProc = SETWINDOWLONG (GetDlgItem(hparent, ltabId), _ ' Subclass the tab control
            %GWL_WNDPROC, CODEPTR(TabSubclassProc))
    END METHOD
    '------------------------------
    METHOD addpage(stext AS STRING, in_textcolor AS LONG, in_bkcolor AS LONG) AS LONG
        LOCAL n AS LONG

        n = UBOUND(PAGES) + 1
        REDIM PRESERVE PAGES(n)
        PAGES(n).pageno = n 'page
        PAGES(n).textcolor = in_textcolor
        PAGES(n).BKcolor = in_BKcolor
        PAGES(n).txt = LEFT$(stext, SIZEOF(PAGES(n).txt) - 1)
        PAGES(n).brush = createsolidbrush(in_BKcolor)
        TAB INSERT PAGE hparent, lTabId, n + 1, 0, $SPC+stext+$SPC TO PAGES(n).hpage
        DIALOG SET COLOR PAGES(n).hpage , -1, in_bkcolor
        METHOD = PAGES(n).hpage
    END METHOD
    '------------------------------
    METHOD notification_from_TAB( BYVAL hD AS DWORD, BYVAL lparam AS DWORD) AS LONG
        LOCAL pNMHDR AS NMHDR PTR
        LOCAL lresult AS LONG
        pNMHDR = lparam
        IF @pNMHDR.hWndFrom = GetDlgItem(hD, ltabId) THEN
            CONTROL SEND hD, ltabId, %TCM_GETCURSEL, 0, 0 TO lresult
            SELECT CASE @pNMHDR.Code
                CASE %TCN_SELCHANGING
                    DIALOG SHOW STATE PAGES(lresult).hpage, %SW_HIDE
                CASE %TCN_SELCHANGE
                    DIALOG SHOW STATE PAGES(lresult).hpage, %SW_SHOW
                    invalidaterect PAGES(lresult).hpage, BYVAL 0, %true
                    METHOD = 1
            END SELECT
        END IF
    END METHOD
    '-------------------------
    METHOD drawitem(BYVAL hD AS LONG, BYVAL lCtlID AS LONG, BYVAL wparam AS LONG, BYVAL lparam AS LONG) AS LONG
       LOCAL pDI AS DRAWITEMSTRUCT PTR
       LOCAL zCap AS ASCIIZ * 50
       LOCAL ti AS TC_ITEM
       LOCAL TextDrawFlag AS LONG

       IF lCtlId <> wparam THEN EXIT METHOD
       pDI = lparam
       inflaterect @pDI.rcItem, -getsystemmetrics(%Sm_CXframe), 0
       FillRect @pDI.hDc, @pDI.rcItem, PAGES(@pDI.itemId).brush
       @pDI.rcItem.nTop = @pDI.rcItem.nTop + 3
       IF @pDI.ItemState = %ODS_SELECTED THEN
           @pDI.rcitem.ntop+=4
           SetBkColor @pDI.hDc, PAGES(@pDI.itemId).BKcolor
           SetTextColor @pDI.hDc, Seltextcolor
       ELSE
           SetBkColor @pDI.hDc, PAGES(@pDI.itemId).BKcolor
           SetTextColor @pDI.hDc, PAGES(@pDI.itemId).textcolor
       END IF

       ti.mask = %TCIF_TEXT
       ti.pszText = VARPTR(zCap)
       ti.cchTextMax = SIZEOF(zCap)
       CALL TabCtrl_GetItem(getdlgitem(hD, lCtlId), @pDI.itemID, ti)
       DrawText @pDI.hDc, zCap, LEN(zCap), @pDI.rcItem, %DT_SINGLELINE OR %DT_CENTER
       METHOD = 1
    END METHOD
END INTERFACE
END CLASS
