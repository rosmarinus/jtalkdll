@echo off
setlocal
set VER=2017
set ARCH=x86
set BATNAME=vcvars32

rem ****************************************
rem call vcvars.bat
rem ****************************************
if %PROCESSOR_ARCHITECTURE% == x86 set ProgramFiles(x86)=C:\Program Files
set INSTANCENAME=Community
set BAT="%ProgramFiles(x86)%\Microsoft Visual Studio\%VER%\%INSTANCENAME%\VC\Auxiliary\Build\%BATNAME%.bat"
if exist %BAT% goto :call_batch
set INSTANCENAME=BuildTools
set BAT="%ProgramFiles(x86)%\Microsoft Visual Studio\%VER%\%INSTANCENAME%\VC\Auxiliary\Build\%BATNAME%.bat"
if exist %BAT% goto :call_batch
set INSTANCENAME=Professional
set BAT="%ProgramFiles(x86)%\Microsoft Visual Studio\%VER%\%INSTANCENAME%\VC\Auxiliary\Build\%BATNAME%.bat"
if exist %BAT% goto :call_batch
set INSTANCENAME=Enterprise
set BAT="%ProgramFiles(x86)%\Microsoft Visual Studio\%VER%\%INSTANCENAME%\VC\Auxiliary\Build\%BATNAME%.bat"
if not exist %BAT% goto :error_batch
goto :call_batch
:error_batch
echo "Visual Studio Community %VER% もしくは Build Tools for Visual Studio %VER% をインストールしてから実行してください。"
goto :exit
:call_batch
call %BAT%
cd /d %~dp0

rem ****************************************
rem check cli compilablity
rem ****************************************
set name=clrtest
set param=
echo using namespace System;void main(){} >%name%.cpp
cl /clr %name%.cpp >NUL 2>&1
if "%errorlevel%" == "0" (
    echo C++/CLIのビルドに対応しているのでJTalkCOM%ARCH%.dllもビルドします。
    set param=-Dbuild_jtalkcom=true
)
del/q %name%.*

rem ****************************************
rem build
rem ****************************************
set build_dir=build%ARCH%.dir
if exist ..\incbuild.bat call ..\incbuild.bat
if not exist "%build_dir%" mkdir %build_dir%
cd %build_dir%
cmake .. -G "NMake Makefiles" %param%
nmake clean
nmake
nmake install
cd ..

rem ****************************************
rem exit
rem ****************************************
:exit
set /p=キーを押すと終了します<NUL
pause >NUL
echo.
