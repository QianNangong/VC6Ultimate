@echo off
@for /f "usebackq tokens=2* delims==" %%a in (`call _und %1`) do if "%%b"=="" (echo %%a) else (echo %%a=%%b)
