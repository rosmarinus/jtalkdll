@echo off
setlocal
set VER=2019
:arch_top
if "%1"=="x86" (
	set ARCH=x86
	set BATNAME=vcvars32
	goto :arch_skip
)
if "%1"=="x64" (
	set ARCH=x64
	set BATNAME=vcvars64
	if %PROCESSOR_ARCHITECTURE% == x86 set BATNAME=vcvarsx86_amd64
	goto :arch_skip
)
if %PROCESSOR_ARCHITECTURE% == x86 (
	set ARCH=x86
	set BATNAME=vcvars32
) else (
	set ARCH=x64
	set BATNAME=vcvars64
)
:arch_skip
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
echo Visual Studio Community %VER% または Build Tools for Visual Studio %VER% をインストールしてから実行してください。
echo もしくは、build.batの三行目のset VER=%VER%の値を書き換えて下さい。
goto :exit
:call_batch
call %BAT%
cd /d %~dp0
rem ****************************************
rem search cmake.exe
rem ****************************************
(cmake 2>&1)>NUL
if errorlevel 9009 (
	echo エラー：このコンピュータにCMakeが見つかりません。インストールしてください。
	echo インストールしているのに見つからなければ、PATHを通してください。
	goto :exit
)
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
rem nmake clean
rem echo nmake
rem nmake
echo nmake install
nmake install
cd ..
rem ****************************************
rem exit
rem ****************************************
:exit
set /p=キーを押すと終了します<NUL
pause >NUL
echo.

