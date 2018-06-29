# Microsoft Developer Studio Project File - Name="$$root$$" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

$$<WIN64>$$
# TARGTYPE "Win64 (x86) Application" 0x0201
$$</WIN64>$$
$$<WIN32>$$
# TARGTYPE "Win32 (x86) Application" 0x0101
$$</WIN32>$$

$$<WIN32>$$
CFG=$$root$$ - Win32 Debug
!MESSAGE "$$root$$ - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "$$root$$ - Win32 Debug" (based on "Win32 (x86) Application")
$$</WIN32>$$
$$<!WIN32>$$
CFG=$$root$$ - Win64 Debug
$$</!WIN32>$$
$$<WIN64>$$
!MESSAGE "$$root$$ - Win64 Release" (based on "Win64 (x86) Application")
!MESSAGE "$$root$$ - Win64 Debug" (based on "Win64 (x86) Application")
$$</WIN64>$$

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

$$<WIN32>$$
!IF  "$(CFG)" == "$$root$$ - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "C:/temp/Release/$$root$$"
# PROP Intermediate_Dir "C:/temp/Release/$$root$$"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /GX- /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /c /G6 /Gr
# SUBTRACT CPP /YX
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib /nologo /pdb:none /machine:I386 /out:"c:/temp/R/$$root$$.exe" /subsystem:windows

!ELSEIF  "$(CFG)" == "$$root$$ - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "C:/temp/Debug/$$root$$"
# PROP Intermediate_Dir "C:/temp/Debug/$$root$$"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /GB /W3 /Gm /GX- /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /c /GZ /MLd
# SUBTRACT CPP /YX
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib /nologo /incremental:no /debug /machine:I386 /out:"c:/temp/D/$$root$$.exe" /pdbtype:sept /subsystem:windows

$$</WIN32>$$
$$<WIN64>$$
$$<!WIN32>$$
!IF  "$(CFG)" == "$$root$$ - Win64 Debug"
$$</!WIN32>$$
$$<WIN32>$$
!ELSEIF  "$(CFG)" == "$$root$$ - Win64 Debug"
$$</WIN32>$$

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "C:/temp/Debug64/$$root$$"
# PROP Intermediate_Dir "C:/temp/Debug64/$$root$$"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /GZ /c /MLd
# SUBTRACT CPP /YX
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib bufferoverflowu.lib /nologo /subsystem:windows /incremental:no /debug /machine:AMD64 /out:"c:/temp/D64/$$root$$.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "$$root$$ - Win64 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "C:/temp/Release64/$$root$$"
# PROP Intermediate_Dir "C:/temp/Release64/$$root$$"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /c /G6 /Gr
# SUBTRACT CPP /YX
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib bufferoverflowu.lib /nologo /subsystem:windows /pdb:none /machine:AMD64 /out:"c:/temp/R64/$$root$$.exe"

$$</WIN64>$$
!ENDIF 

# Begin Target

$$<WIN32>$$
# Name "$$root$$ - Win32 Release"
# Name "$$root$$ - Win32 Debug"
$$</WIN32>$$
$$<WIN64>$$
# Name "$$root$$ - Win64 Release"
# Name "$$root$$ - Win64 Debug"
$$</WIN64>$$
$$<GROUP>$$
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
$$</GROUP>$$

# Begin Source File
SOURCE=..\Code\main.cpp
# End Source File

$$<GROUP>$$
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
$$</GROUP>$$
# End Target
# End Project
