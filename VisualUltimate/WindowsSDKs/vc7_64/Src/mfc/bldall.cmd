set UNICODE=
call blddll RETAIL CLEAN
call blddll DEBUG CLEAN
nmake DEBUG=0 clean
nmake DEBUG=1 clean
call bldisapi CLEAN

set UNICODE=1
call blddll RETAIL CLEAN
call blddll DEBUG CLEAN
nmake DEBUG=0 clean
nmake DEBUG=1 clean

set UNICODE=

echo --Begin ANSI RETAIL DLL-- >log.txt
call blddll RETAIL >>log.txt
echo --End ANSI RETAIL DLL-- >>log.txt

echo --Begin ANSI DEBUG DLL-- >>log.txt
call blddll DEBUG >>log.txt
echo --End ANSI DEBUG DLL-- >>log.txt

echo --Begin ANSI RETAIL LIB-- >>log.txt
nmake DEBUG=0 >>log.txt
echo --End ANSI RETAIL LIB-- >>log.txt

echo --Begin ANSI DEBUG LIB-- >>log.txt
nmake DEBUG=1 >>log.txt
echo --End ANSI DEBUG LIB-- >>log.txt

echo --Begin ISAPI-- >>log.txt
call bldisapi >>log.txt
echo --End ISAPI-- >>log.txt

set UNICODE=1

echo --Begin UNICODE RETAIL DLL-- >>log.txt
call blddll RETAIL >>log.txt
echo --End UNICODE RETAIL DLL-- >>log.txt

echo --Begin UNICODE DEBUG DLL-- >>log.txt
call blddll DEBUG >>log.txt
echo --End UNICODE DEBUG DLL-- >>log.txt

echo --Begin UNICODE RETAIL LIB-- >>log.txt
nmake DEBUG=0 >>log.txt
echo --End UNICODE RETAIL LIB-- >>log.txt

echo --Begin UNICODE DEBUG LIB-- >>log.txt
nmake DEBUG=1 >>log.txt
echo --End UNICODE DEBUG LIB-- >>log.txt

