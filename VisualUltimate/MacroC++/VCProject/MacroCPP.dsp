# Microsoft Developer Studio Project File - Name="MacroC++" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=MacroC++ - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MacroCPP.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MacroCPP.mak" CFG="MacroC++ - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MacroC++ - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MacroC++ - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MacroC++ - Win32 ReleaseDbg" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MacroC++ - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "C:/temp/Release/MacroC++"
# PROP Intermediate_Dir "C:/temp/Release/MacroC++"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /G6 /Gr /MT /W3 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /c
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 user32.lib oleaut32.lib /dll /pdb:none /machine:I386 /out:"C:/temp/R/MacroC++.dll"
# SUBTRACT LINK32 /nologo

!ELSEIF  "$(CFG)" == "MacroC++ - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "C:/temp/Debug/MacroC++"
# PROP Intermediate_Dir "C:/temp/Debug/MacroC++"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /MTd /W3 /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /GZ /c
# SUBTRACT CPP /YX
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /machine:IX86
# ADD LINK32 /nologo /dll /incremental:no /debug /machine:I386 /out:"C:/temp/D/MacroC++.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "MacroC++ - Win32 ReleaseDbg"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MacroC_____Win32_ReleaseDbg"
# PROP BASE Intermediate_Dir "MacroC_____Win32_ReleaseDbg"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "C:/temp/ReleaseDbg/MacroC++"
# PROP Intermediate_Dir "C:/temp/ReleaseDbg/MacroC++"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /Gr /MT /W3 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "MACROCPP_EXPORTS" /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /G6 /Gr /MT /W3 /Z7 /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /pdb:none /machine:I386
# ADD LINK32 user32.lib oleaut32.lib /nologo /dll /pdb:none /debug /machine:I386 /out:"Q:\MVS6\Common\MSDev98\Bun\MacroC++.dll"

!ENDIF 

# Begin Target

# Name "MacroC++ - Win32 Release"
# Name "MacroC++ - Win32 Debug"
# Name "MacroC++ - Win32 ReleaseDbg"

# Begin Source File
SOURCE="..\Code\MacroCPP.cpp"
# End Source File

# Begin Source File
SOURCE="..\Code\MacroCPP.h"
# End Source File
# End Target
# End Project
