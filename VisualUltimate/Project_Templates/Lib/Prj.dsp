# Microsoft Developer Studio Project File - Name="$$root$$" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

$$<WIN64>$$
# TARGTYPE "Win64 (x86) Static Library" 0x0204
$$</WIN64>$$
$$<WIN32>$$
# TARGTYPE "Win32 (x86) Static Library" 0x0104
$$</WIN32>$$

$$<WIN32>$$
CFG=$$root$$ - Win32 Debug
!MESSAGE "$$root$$ - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "$$root$$ - Win32 Debug" (based on "Win32 (x86) Static Library")
$$</WIN32>$$
$$<!WIN32>$$
CFG=$$root$$ - Win64 Debug
$$</!WIN32>$$
$$<WIN64>$$
!MESSAGE "$$root$$ - Win64 Release" (based on "Win64 (x86) Static Library")
!MESSAGE "$$root$$ - Win64 Debug" (based on "Win64 (x86) Static Library")
$$</WIN64>$$

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

$$<WIN32>$$
!IF  "$(CFG)" == "$$root$$ - Win32 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "C:/temp/Release/$$root$$"
# PROP Intermediate_Dir "C:/temp/Release/$$root$$"
# PROP Target_Dir ""
# ADD CPP /nologo /G6 /Gr /W3 /GX- /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB"  /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:"c:/temp/R/$$root$$.lib"

!ELSEIF  "$(CFG)" == "$$root$$ - Win32 Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "C:/temp/Debug/$$root$$"
# PROP Intermediate_Dir "C:/temp/Debug/$$root$$"
# PROP Target_Dir ""
# ADD CPP /nologo /W3 /Gm /GX- /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB"  /GZ /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:"c:/temp/D/$$root$$.lib"

$$</WIN32>$$
$$<WIN64>$$
$$<!WIN32>$$
!IF  "$(CFG)" == "$$root$$ - Win64 Debug"
$$</!WIN32>$$
$$<WIN32>$$
!ELSEIF  "$(CFG)" == "$$root$$ - Win64 Debug"
$$</WIN32>$$

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "C:/temp/Release64/$$root$$"
# PROP Intermediate_Dir "C:/temp/Release64/$$root$$"
# PROP Target_Dir ""
# ADD CPP /nologo /G6 /Gr /W3 /GX- /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB"  /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:"c:/temp/R/$$root$$.lib"

!ELSEIF  "$(CFG)" == "$$root$$ - Win64 Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "C:/temp/Release64/$$root$$"
# PROP Intermediate_Dir "C:/temp/Release64/$$root$$"
# PROP Target_Dir ""
# ADD CPP /nologo /G6 /Gr /W3 /GX- /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB"  /c
# SUBTRACT CPP /YX
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:"c:/temp/R64/$$root$$.lib"

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
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
$$</GROUP>$$
# End Target
# End Project
