set JTALKDIR=c:/open_jtalk
set jtalklib=jtalk64.lib
if "%VSCMD_ARG_HOST_ARCH%" == "x86" set jtalklib=jtalk32.lib
cl /c /clr jtd_cli.cpp /I %JTALKDIR%/include
link jtd_cli.obj %jtalklib% /machine:%VSCMD_ARG_HOST_ARCH% /subsystem:"windows" /entry:"Main" /LIBPATH:%JTALKDIR%/lib
