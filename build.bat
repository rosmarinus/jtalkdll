@echo off

rem command line option
rem %1: true | false
rem :jtalkcom build switch

set build_dir=build.dir
if exist ..\incbuild.bat call ..\incbuild.bat
if not exist "%build_dir%" mkdir %build_dir%
cd %build_dir%

rem *** vs2017win64 ***
rem cmake .. -G "Visual Studio 15 2017 Win64"
rem msbuild ALL_BUILD.vcxproj /p:Configuration=Release;Platform=x64

rem *** vs2017win32 ***
rem cmake .. -G "Visual Studio 15 2017"
rem msbuild ALL_BUILD.vcxproj /p:Configuration=Release;Platform=win32

rem *** nmake ***
set param=
if not "%1"=="" set param=-Djtalkcom=%1
cmake .. -G "NMake Makefiles" %param%
nmake
nmake install

cd ..

