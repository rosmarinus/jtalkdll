@echo off
set AMD64=0
if /I "%VSCMD_ARG_HOST_ARCH%" == "x64" set AMD64=1

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
