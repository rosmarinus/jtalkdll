set JTALKDIR=c:/open_jtalk
cl /c /clr jtd_clic.cpp /I %JTALKDIR%/include
link jtd_clic.obj jtalk.lib /machine:%VSCMD_ARG_HOST_ARCH% /subsystem:"console" /entry:"Main" /LIBPATH:%JTALKDIR%/lib

