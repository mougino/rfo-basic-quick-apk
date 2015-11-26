' List files or folders recursively from a root folder
' See macros below for use in a program:
'    RDIR_DeclareLocals  ' Local variables for recursive dir macros
'
'    RDIR_ListFilesIn(rootfolder, "*.bas")
'    FOR i = 0 to RDIR_NbOfFiles
'        e = RDIR_File(i)          ' full path + filename
'        e = RDIR_FileName(i)      ' single filename (without path)
'    NEXT i
'
'    RDIR_ListSubfoldersIn(rootfolder)
'    FOR i = 0 to RDIR_NbOfSubfolders
'        e = RDIR_Subfolder(i)     ' full path + subfolder name
'        e = RDIR_SubfolderName(i) ' single subfolder name, without path
'    NEXT i
'
'    RDIR_EraseAll(rootfolder)

'--------------------------------------------------------------------------------
MACRO RDIR_DeclareLocals = LOCAL FileCount, RDIR_NbOfSubfolders AS DWORD
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
MACRO RDIR_ListFilesIn(rootfolder, filter)
    REDIM RDIR_FileData(0 TO 0) AS WIN32_FIND_DATA
    FileCount = FileEnum(rootfolder, RDIR_FileData(), filter)
END MACRO
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
MACRO RDIR_NbOfFiles() = (FileCount - 1)
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
MACRO RDIR_File(i) = RDIR_FileData(i).cFilename
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
MACRO RDIR_FileName(i) = MID$(RDIR_File(i), INSTR(-1, RDIR_File(i), ANY ":/\") + 1)
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
MACRO RDIR_ListSubfoldersIn(rootfolder)
    REDIM RDIR_Subfolder(0 TO 0) AS STRING
    RDIR_NbOfSubfolders = SubfolderEnum(rootfolder, RDIR_Subfolder())
END MACRO
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
MACRO RDIR_SubfolderName(i) = MID$(RDIR_Subfolder(i), INSTR(-1, RDIR_Subfolder(i), ANY ":/\") + 1)
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
DECLARE FUNCTION PathMatchSpec LIB "SHLWAPI.DLL" ALIAS "PathMatchSpecA"(pszFile AS ASCIIZ, pszSpec AS ASCIIZ) AS LONG
'--------------------------------------------------------------------------------

'-----------------------------------------------------------------------------------------------
'   Calls Win32 API to create the folder and all its parents if not existing (e.g. "C:\D0\D1\D2\D3")
'-----------------------------------------------------------------------------------------------
' Use: MakeSureDirectoryPathExists folderName
DECLARE FUNCTION MakeSureDirectoryPathExists LIB "IMAGEHLP.DLL" ALIAS _
                "MakeSureDirectoryPathExists"(lpDirName AS ASCIIZ) AS LONG
'-----------------------------------------------------------------------------------------------

'-----------------------------------------------------------------------------------------------
FUNCTION RDIR_EraseAll(rootfolder AS STRING) AS LONG
    RDIR_DeclareLocals  ' Local variables for recursive dir macros
    LOCAL i AS LONG

    RDIR_ListFilesIn(rootfolder, "*.*")
    FOR i = 0 TO RDIR_NbOfFiles
        KILL RDIR_File(i)
    NEXT

    RDIR_ListSubfoldersIn(rootfolder)
    FOR i = RDIR_NbOfSubfolders TO 0 STEP -1
        RMDIR RDIR_Subfolder(i)
    NEXT
END FUNCTION
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
FUNCTION FileTimeToInternationalDate(File_Time AS FILETIME) AS STRING
 LOCAL Sys_Time AS SystemTime

 FileTimeToSystemTime File_Time, Sys_Time

 FUNCTION = _
   FORMAT$(Sys_Time.wyear, "0000") & "/" & FORMAT$(Sys_Time.wMonth,  "00") & "/" & _
   FORMAT$(Sys_Time.wDay,    "00") & " " & FORMAT$(Sys_Time.wHour,   "00") & ":" & _
   FORMAT$(Sys_Time.wMinute, "00") & ":" & FORMAT$(Sys_Time.wSecond, "00") & " " & _
   CHOOSE$(Sys_Time.wDayOfWeek + 1, "sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday")

END FUNCTION
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
FUNCTION FileEnum(rootFolder AS STRING, File() AS WIN32_FIND_DATA, Filter AS STRING) AS LONG
 LOCAL  FileData    AS WIN32_FIND_DATA
 LOCAL  Looper      AS LONG
 LOCAL  hDir        AS DWORD
 LOCAL  FolderCount AS DWORD
 STATIC FileCount   AS DWORD
 STATIC FolderDeep  AS DWORD
 STATIC LastTimeRun AS DOUBLE

 IF TIMER - LastTimeRun > 0.5 THEN ' Make the function thread safe! (if previous session was killed = last call is more than 1/10s ago)
  FolderDeep = 0
  FileCount  = 0
 END IF

 INCR FolderDeep
 DIM SubFolder(0 TO 0) AS STRING
 IF ASC(rootFolder, - 1) <> 92 THEN rootFolder = rootFolder & "\" 'Must end with a backlash
 SubFolder(0) = rootFolder

 hDir = FindFirstFile(SubFolder(0) & "*", FileData)
 IF hDir <> %INVALID_HANDLE_VALUE THEN
   DO
     IF (FileData.dwFileAttributes AND %FILE_ATTRIBUTE_DIRECTORY) THEN 'It's a folder
       IF FileData.cFilename <> "." AND FileData.cFilename <> ".." THEN 'Refuse "." and ".."
         INCR FolderCount
         REDIM PRESERVE SubFolder(0 TO FolderCount) AS STRING
         SubFolder(FolderCount) = SubFolder(0) & FileData.cFilename & "\"
       END IF
     ELSE 'Its a file ...
       IF PathMatchSpec(FileData.cFilename, BYVAL STRPTR(Filter)) THEN  'Check against Filter
         REDIM PRESERVE File(0 TO FileCount)
         File(FileCount) = FileData
         File(FileCount).cFilename = SubFolder(0) & File(FileCount).cFilename 'Set to full path
         INCR FileCount 'Zero based array
       END IF
     END IF
     DIALOG DOEVENTS
   LOOP WHILE FindNextFile(hDir, FileData)
 END IF

 FOR Looper = 1 TO FolderCount
   LastTimeRun = TIMER
   FileEnum SubFolder(Looper), File(), Filter 'Function call itself to get sub-folder files
 NEXT

 FindClose hDir

 DECR FolderDeep
 IF FolderDeep = 0 THEN 'Exiting from first call, function is not in recursive state
   FUNCTION = FileCount
   FileCount = 0  'Reset for subsequent call
 END IF

END FUNCTION
'--------------------------------------------------------------------------------

'--------------------------------------------------------------------------------
FUNCTION SubfolderEnum(rootFolder AS STRING, Folder() AS STRING) AS LONG

    LOCAL iPOS, FolderCount AS LONG, temp AS STRING

    Folder(FolderCount) = RTRIM$(rootFolder, "\")
    WHILE LEN(Folder(iPOS))
        temp = DIR$(BUILD$(Folder(iPOS),"\*.*"), ONLY %SUBDIR)  'subfolders only
        WHILE LEN(temp)
            INCR FolderCount
            IF FolderCount > UBOUND(Folder()) THEN
              REDIM PRESERVE Folder(FolderCount + 100)
            END IF
            Folder(FolderCount) =  BUILD$(Folder(iPos),"\",temp)
            temp =  DIR$(NEXT)
        WEND
        INCR iPOS
        IF UBOUND(Folder) < iPOS THEN REDIM PRESERVE Folder(iPOS)
    WEND

    REDIM PRESERVE Folder(FolderCount)
    DIR$ CLOSE

    FUNCTION = FolderCount

END FUNCTION
'--------------------------------------------------------------------------------
