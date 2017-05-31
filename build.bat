@echo off
set BUILD32=ON
set BUILD64=ON
if not "%BUILD32%""%BUILD64%" == """" goto :skip
echo BUILD32、BUILD64 の少なくとも一方に何か値を設定してください。
echo 終了します
pause
goto :eof
:skip
set BAT32="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"
set BAT64="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"
if exist %BAT32% goto :build
set B="C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"
if not exist %B% goto :error_batch
echo "Visual Studio 2015でビルドします"
set VS=2015
set BAT32=%B% x86
set BAT64=%B% amd64
goto :build
:error_batch
echo "Visual Studio 2017 Community をインストールしてから実行してください。"
echo 終了します
pause
goto :eof
:build
if "%BUILD32%" == "" goto :build64
rem ****************************************
rem *** 32bit build
rem ****************************************
call %BAT32%
cd /d %~dp0
set AMD64=0

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
rem *** 64bit build
rem ****************************************
call %BAT64%
cd /d %~dp0
set AMD64=1

cd portaudio\build\msvc
msbuild portaudio.sln /p:Configuration=Release;Platform=x64
if not exist ..\lib mkdir ..\..\lib
copy x64\Release\portaudio_static_64.lib ..\..\lib\
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
