@echo off
setlocal
set BUILD32=
set BUILD64=ON
set CPPCLI=0
if not "%BUILD32%""%BUILD64%" == """" goto :skip
echo BUILD32、BUILD64 の少なくとも一方に何か値を入れて、有効にしてください
echo 終了します
pause
goto :eof
:skip
rem ****************************************
rem copy voices
rem ****************************************
set INSTALLDIR=c:\open_jtalk
set VOICEDIR=voice
if exist %INSTALLDIR%\%VOICEDIR% goto :skip_copy_voice
if not exist %INSTALLDIR%\%VOICEDIR% mkdir %INSTALLDIR%\%VOICEDIR%
xcopy %VOICEDIR% %INSTALLDIR%\%VOICEDIR% /e /q /y
:skip_copy_voice
rem ****************************************
rem set build ver
rem ****************************************
set REAL=%PROCESSOR_ARCHITECTURE%
if %PROCESSOR_ARCHITECTURE% == x86 set ProgramFiles(x86)=C:\Program Files
set BAT32="%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"
set BAT64="%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
if exist %BAT32% goto :build
set BAT32="%ProgramFiles(x86)%\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\vcvars32.bat"
set BAT64="%ProgramFiles(x86)%\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if not exist %BAT32% goto :error_batch
echo "Visual Studio Build Tools 2017でビルドします"
goto :build
:error_batch
echo "Visual Studio Community 2017 もしくは Visual Studio Build Tools 2017 をインストールしてから実行してください。"
echo 終了します
pause
goto :eof
:build
if "%BUILD32%" == "" goto :build64
rem ****************************************
rem 32bit build
rem ****************************************
call %BAT32%
cd /d %~dp0
set AMD64=0
set name=clrtest
echo using namespace System;void main(){} >%name%.cpp
cl /clr %name%.cpp >NUL 2>&1
if "%errorlevel%" == "0" (
echo C++/CLIのビルドに対応しているのでJTalkCOM32.dllもビルドします。
set CPPCLI=1
)
del/q %name%.*

cd portaudio\build\msvc
msbuild portaudio.sln /p:Configuration=Release;Platform=x86
if not exist ..\lib mkdir ..\..\lib
copy Win32\Release\portaudio_static_32.lib ..\..\lib\
cd ..\..\..

cd hts_engine_API-1.10
nmake -f makefile.mak clean
nmake -f makefile.mak
nmake -f makefile.mak install
cd ..

cd open_jtalk-1.10
nmake -f makefile.mak clean
nmake -f makefile.mak
nmake -f makefile.mak install
cd ..
:build64
if "%BUILD64%" == "" goto :eof
if "%PROCESSOR_ARCHITECTURE%" == "x86" goto :eof
rem ****************************************
rem 64bit build
rem ****************************************
call %BAT64%
cd /d %~dp0
set AMD64=1
set name=clrtest
echo using namespace System;void main(){} >%name%.cpp
cl /clr %name%.cpp >NUL 2>&1
if "%errorlevel%" == "0" (
echo C++/CLIのビルドに対応しているのでJTalkCOM64.dllもビルドします。
set CPPCLI=1
)
del/q %name%.*

cd portaudio\build\msvc
msbuild portaudio.sln /p:Configuration=Release;Platform=x64
if not exist ..\lib mkdir ..\..\lib
copy x64\Release\portaudio_static_64.lib ..\..\lib\
cd ..\..\..

cd hts_engine_API-1.10
rem nmake -f makefile.mak clean
nmake -f makefile.mak
nmake -f makefile.mak install
cd ..

cd open_jtalk-1.10
rem nmake -f makefile.mak clean
nmake -f makefile.mak
nmake -f makefile.mak install
cd ..
