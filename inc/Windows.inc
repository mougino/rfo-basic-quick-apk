'------------------------------------------------------------------------------
'
'  Windows.inc -- Master include file for Windows applications.
'  ** NOTE: this include file is largely obsolete. Use Win32Api.inc instead. **
'
'  Copyright (c) 2010-2011 PowerBASIC, Inc.
'  Portions Copyright (c) Microsoft Corporation
'  All Rights Reserved.
'
'  Last updated 31 Jan 2011
'
'------------------------------------------------------------------------------
'  from Windows.h build version 0001 of 14 Jul 2009
'------------------------------------------------------------------------------

#INCLUDE THIS ONCE

%WINDOWS = 1

#INCLUDE "SdkDdkVer.inc"

#IF NOT %DEF(%INC_WINDOWS)
%INC_WINDOWS = 1
#ENDIF


'  If def, the following flags inhibit definition
'     of the indicated items.
'
'  %NOGDICAPMASKS     - CC_*, LC_*, PC_*, CP_*, TC_*, RC_
'  %NOVIRTUALKEYCODES - VK_*
'  %NOWINMESSAGES     - WM_*, EM_*, LB_*, CB_*
'  %NOWINSTYLES       - WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
'  %NOSYSMETRICS      - SM_*
'  %NOMENUS           - MF_*
'  %NOICONS           - IDI_*
'  %NOKEYSTATES       - MK_*
'  %NOSYSCOMMANDS     - SC_*
'  %NORASTEROPS       - Binary and Tertiary raster ops
'  %NOSHOWWINDOW      - SW_*
'  %OEMRESOURCE       - OEM Resource values
'  %NOATOM            - Atom Manager routines
'  %NOCLIPBOARD       - Clipboard routines
'  %NOCOLOR           - Screen colors
'  %NOCTLMGR          - Control and Dialog routines
'  %NODRAWTEXT        - DrawText() and DT_*
'  %NOGDI             - All GDI defines and routines
'  %NOKERNEL          - All KERNEL defines and routines
'  %NOUSER            - All USER defines and routines
'  %NONLS             - All NLS defines and routines
'  %NOMB              - MB_* and MessageBox()
'  %NOMEMMGR          - GMEM_*, LMEM_*, GHND, LHND, associated routines
'  %NOMETAFILE        - typedef METAFILEPICT
'  %NOMINMAX          - Macros min(a,b) and max(a,b)
'  %NOMSG             - typedef MSG and associated routines
'  %NOOPENFILE        - OpenFile(), OemToAnsi, AnsiToOem, and OF_*
'  %NOSCROLL          - SB_* and scrolling routines
'  %NOSERVICE         - All Service Controller routines, SERVICE_ equates, etc.
'  %NOSOUND           - Sound driver routines
'  %NOTEXTMETRIC      - typedef TEXTMETRIC and associated routines
'  %NOWH              - SetWindowsHook and WH_*
'  %NOWINOFFSETS      - GWL_*, GCL_*, associated routines
'  %NOCOMM            - COMM driver routines
'  %NOKANJI           - Kanji support stuff.
'  %NOHELP            - Help engine interface.
'  %NOPROFILER        - Profiler interface.
'  %NODEFERWINDOWPOS  - DeferWindowPos routines
'  %NOMCX             - Modem Configuration Extensions
'

#IF NOT %DEF(%X86)
%X86 = 1
#ENDIF

'#Include "excpt.inc"
'#Include "stdarg.inc"

#INCLUDE "windef.inc"
#INCLUDE "ntstatus.inc"
#INCLUDE "winbase.inc"
#INCLUDE "wingdi.inc"
#INCLUDE "winuser.inc"

#IF NOT %DEF(%MAC) OR %DEF(%WIN32NLS)
#INCLUDE "winnls.inc"
#ENDIF

#IF NOT %DEF(%MAC)
#INCLUDE "wincon.inc"
#INCLUDE "winver.inc"
#ENDIF

#IF NOT %DEF(%MAC) OR %DEF(%WIN32REG)
#INCLUDE "winreg.inc"
#ENDIF

#IF NOT %DEF(%MAC)
#INCLUDE "winnetwk.inc"
#ENDIF

#IF NOT %DEF(%WIN32_LEAN_AND_MEAN)
#INCLUDE "cderr.inc"
#INCLUDE "dde.inc"
#INCLUDE "ddeml.inc"
#INCLUDE "dlgs.inc"

#IF NOT %DEF(%MAC)
#INCLUDE "lzexpand.inc"
#INCLUDE "mmsystem.inc"
#INCLUDE "nb30.inc"
#INCLUDE "rpc.inc"
#ENDIF
#INCLUDE "shellapi.inc"
#IF NOT %DEF(%MAC)
#INCLUDE "winperf.inc"
#INCLUDE "winsock.inc"
#ENDIF
#IF NOT %DEF(%NOCRYPT)
#INCLUDE "wincrypt.inc"
#INCLUDE "winefs.inc"
#INCLUDE "winscard.inc"
#ENDIF

#IF NOT %DEF(%NOGDI)
#IF NOT %DEF(%MAC)
#INCLUDE "winspool.inc"
#IF %DEF(%INC_OLE1)
#INCLUDE "ole.inc"
#ELSE
#INCLUDE "ole2.inc"
#ENDIF ' not %INC_OLE1
#ENDIF ' not %MAC
#IF NOT %DEF(%INC_COMMDLG)
#INCLUDE "commdlg.inc"
#ENDIF
#ENDIF ' not %NOGDI
#ENDIF ' %WIN32_LEAN_AND_MEAN

#INCLUDE "stralign.inc"

#IF %DEF(%MAC)
#INCLUDE "winwlm.inc"
#ENDIF

#IF %DEF(%INC_OLE2)
#INCLUDE "ole2.inc"
#ENDIF ' %INC_OLE2

#IF NOT %DEF(%MAC)
#IF NOT %DEF(%NOSERVICE)
#INCLUDE "winsvc.inc"
#ENDIF

#IF (%WINVER >= &H0400)
#IF NOT %DEF(%NOMCX)
#INCLUDE "mcx.inc"
#ENDIF ' %NOMCX

#IF NOT %DEF(%NOIME)
#INCLUDE "imm.inc"
#ENDIF
#ENDIF ' %WINVER >= &H0400

#ENDIF ' %INC_WINDOWS
