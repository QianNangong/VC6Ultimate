# Microsoft Developer Studio Project File - Name="$$root$$" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=$$root$$ - Win32 Debug
!MESSAGE "$$root$$ - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "$$root$$ - Win32 Debug" (based on "Win32 (x86) Application")

# Begin Project
# PROP AllowPerConfigDependencies 0

!IF  "$(CFG)" == "$$root$$ - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "C:/temp/Release/$$root$$"
# PROP Intermediate_Dir "C:/temp/Release/$$root$$"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD CPP /nologo /W3 /O2 /I "Q:/@Projets/DevTools" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /c /G6 /Gr /MT
# SUBTRACT CPP /YX
MTL=midl.exe
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD RSC /l 0x409 /d "NDEBUG"
SDP=GLSLCompiler.exe
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
CPP=cl.exe
# ADD CPP /nologo /W3 /Z7 /Od /I "Q:/@Projets/DevTools" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /c /GZ /MTd
# SUBTRACT CPP /YX
MTL=midl.exe
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD RSC /l 0x409 /d "_DEBUG"
SDP=GLSLCompiler.exe
BSC32=bscmake.exe
# ADD BSC32 /nologo
LINK32=link.exe
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib /nologo /incremental:no /debug /machine:I386 /out:"c:/temp/D/$$root$$.exe" /pdbtype:sept /subsystem:windows

!ENDIF 

# Begin Target

# Name "$$root$$ - Win32 Release"
# Name "$$root$$ - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"

# Begin Source File
SOURCE="..\Code\main.cpp"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# End Target
# End Project
