set JTALKDIR=c:/open_jtalk
cl /c /clr jtd_cli.cpp /I %JTALKDIR%/include
link jtd_cli.obj jtalk.lib /machine:%VSCMD_ARG_HOST_ARCH% /subsystem:"windows" /entry:"Main" /LIBPATH:%JTALKDIR%/lib
