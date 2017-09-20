@echo off
setlocal
rem set filename=%1
set filename=open_jtalk-1.10\lib\build.version
if "%filename%" == "" goto :eof
set c=0
if not exist %filename% echo BUILD=%c%>%filename%
for /F "tokens=1,2 delims==" %%A in (%filename%) do set /a c=%%B+1
if "%c%" == "" c=1
echo %c%
echo BUILD=%c%>%filename%
