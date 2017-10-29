set JTALKDIR=c:\open_jtalk
set QTDIR=C:\Qt\5.9.2\msvc2017_64
rem set QTDIR=C:\Qt\5.9.2\msvc2015_64
%QTDIR%\bin\qmake jtd_cppqt.pro
msbuild jtd_cppqt.vcxproj /p:Configuration=Release
copy release\jtd_cppqt.exe .