@echo off
if not exist "C:\temp\Release\MacroC++" mkdir "C:\temp\Release\MacroC++"
if not exist "C:\temp\R" mkdir "C:\temp\R"
set OUT_FILE="..\..\MacroC++.dll"
if exist %OUT_FILE% del /F %OUT_FILE%
set INCLUDE=Q:\@MsSDK\StdLib\vc6\Include;Q:\@MsSDK\WinLib\Include;Q:\MVS6\VC98\vc6\INCLUDE;Q:\MVS6\VC98\vc6\MFC\INCLUDE;Q:\MVS6\VC98\vc6\ATL\INCLUDE
..\Compiler\cl /nologo /G6 /Gr /MT /W3 /O2 /Fo"C:/temp/Release/MacroC++/" /c /D "for=if(0){}else for" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" MacroCPP.cpp
set LIB=Q:\@MsSDK\WinLib\Lib;Q:\@MsSDK\MsC\Lib;Q:\MVS6\VC98\vc6\LIB;Q:\MVS6\VC98\vc6\MFC\LIB
..\Compiler\link /nologo user32.lib oleaut32.lib /dll /pdb:none /out:%OUT_FILE% /implib:"C:/temp/Release/MacroC++/MacroC++.lib" C:\temp\Release\MacroC++\MacroCPP.obj /LARGEADDRESSAWARE /machine:I386

