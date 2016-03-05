# Microsoft Developer Studio Project File - Name="Hexzilla" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO EDIT **
# TARGTYPE "Win32 (x86) Console Application" 0x0103
# TARGTYPE "Win32 (x86) External Target" 0x0106
CFG=Hexzilla - Win32 Debug
!MESSAGE NMAKE /f "Hexzilla.mak".
!MESSAGE NMAKE /f "Hexzilla.mak" CFG="Hexzilla - Win32 Debug"
!MESSAGE "Hexzilla - Win32 Release" (based on  "Win32 (x86) Console Application")
!MESSAGE "Hexzilla - Win32 Debug" (based on  "Win32 (x86) Console Application")
!MESSAGE "Hexzilla - NACL Release" (based on  "Win32 (x86) External Target")
!MESSAGE "Hexzilla - NACL Debug" (based on  "Win32 (x86) External Target")
!MESSAGE "Hexzilla - Emscripten Release" (based on  "Win32 (x86) External Target")
!MESSAGE "Hexzilla - Emscripten Debug" (based on  "Win32 (x86) External Target")
!MESSAGE "Hexzilla - Android Release" (based on  "Win32 (x86) External Target")
!MESSAGE "Hexzilla - Android Debug" (based on  "Win32 (x86) External Target")
# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Hexzilla - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release-vc6"
# PROP Intermediate_Dir "Release-vc6"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /O2 /I "../ZillaLib/Include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x807 /d "NDEBUG"
# ADD RSC /l 0x807 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 "../ZillaLib/Release-vc6/ZillaLib.lib" /nologo /subsystem:windows /pdb:"Release-vc6/Hexzilla.pdb" /map:"Release-vc6/Hexzilla.map" /machine:I386 /out:"Release-vc6/Hexzilla.exe" /opt:ref /opt:nowin98
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Hexzilla - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug-vc6"
# PROP Intermediate_Dir "Debug-vc6"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /ZI /Od /I "../ZillaLib/Include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "ZILLALOG" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x807 /d "_DEBUG"
# ADD RSC /l 0x807 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 "../ZillaLib/Debug-vc6/ZillaLib.lib" /nologo /subsystem:console /incremental:yes /pdb:"Debug-vc6/Hexzilla.pdb" /debug /machine:I386 /out:"Debug-vc6/Hexzilla.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Hexzilla - NACL Release"

# PROP Output_Dir "Release-nacl"
# PROP Intermediate_Dir "Release-nacl"
# PROP Cmd_Line "python ../ZillaLib/NACL/ZillaLibNACL.py build -rel -vc Hexzilla"
# PROP Rebuild_Opt "-clean"
# PROP Target_File "Release-nacl/Hexzilla_x86_64.nexe.gz.exe"

!ELSEIF  "$(CFG)" == "Hexzilla - NACL Debug"

# PROP Output_Dir "Debug-nacl"
# PROP Intermediate_Dir "Debug-nacl"
# PROP Cmd_Line "python ../ZillaLib/NACL/ZillaLibNACL.py build -vc Hexzilla"
# PROP Rebuild_Opt "-clean"
# PROP Target_File "Debug-nacl/Hexzilla_x86_64.nexe.gz.exe"

!ELSEIF  "$(CFG)" == "Hexzilla - Emscripten Release"

# PROP Output_Dir "Release-emscripten"
# PROP Intermediate_Dir "Release-emscripten"
# PROP Cmd_Line "python ../ZillaLib/Emscripten/ZillaLibEmscripten.py build -rel -vc Hexzilla"
# PROP Rebuild_Opt "-clean"
# PROP Target_File "Release-emscripten/Hexzilla.js.exe"

!ELSEIF  "$(CFG)" == "Hexzilla - Emscripten Debug"

# PROP Output_Dir "Debug-emscripten"
# PROP Intermediate_Dir "Debug-emscripten"
# PROP Cmd_Line "python ../ZillaLib/Emscripten/ZillaLibEmscripten.py build -vc Hexzilla"
# PROP Rebuild_Opt "-clean"
# PROP Target_File "Debug-emscripten/Hexzilla.js.exe"

!ELSEIF  "$(CFG)" == "Hexzilla - Android Release"

# PROP Output_Dir "Android/bin"
# PROP Intermediate_Dir "Android/obj"
# PROP Cmd_Line "../ZillaLib/Tools/make.exe --no-print-directory -f ../ZillaLib/Android/ZillaLibAndroid.mk ZLDEBUG=0 ZillaApp=Hexzilla"
# PROP Rebuild_Opt "-B"
# PROP Target_File "Android/bin/Hexzilla.apk"

!ELSEIF  "$(CFG)" == "Hexzilla - Android Debug"

# PROP Output_Dir "Android/bin"
# PROP Intermediate_Dir "Android/obj"
# PROP Cmd_Line "../ZillaLib/Tools/make.exe --no-print-directory -f ../ZillaLib/Android/ZillaLibAndroid.mk ZLDEBUG=1 ZillaApp=Hexzilla"
# PROP Rebuild_Opt "-B"
# PROP Target_File "Android/bin/Hexzilla.apk"

!ENDIF

# Begin Target
# Name "Hexzilla - Win32 Release"
# Name "Hexzilla - Win32 Debug"
# Name "Hexzilla - NACL Release"
# Name "Hexzilla - NACL Debug"
# Name "Hexzilla - Emscripten Release"
# Name "Hexzilla - Emscripten Debug"
# Name "Hexzilla - Android Release"
# Name "Hexzilla - Android Debug"
# Begin Source File
SOURCE=./include.h
# End Source File
# Begin Source File
SOURCE=./main.cpp
# End Source File
# Begin Source File
SOURCE=./SceneGame.cpp
# End Source File
# Begin Source File
SOURCE=./SceneTitle.cpp
# End Source File
# Begin Source File
SOURCE=./sounds.cpp
# End Source File
# Begin Source File
SOURCE=./world.cpp
# End Source File
# Begin Source File
SOURCE=./world.h
# End Source File
# Begin Source File
SOURCE=./Hexzilla.rc
# End Source File
# End Target
# End Project
