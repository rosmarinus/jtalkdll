set JTALKDIR=c:\open_jtalk
set jtalkcom=JTalkCOM%VSCMD_ARG_HOST_ARCH%.dll
copy %JTALKDIR%\bin\%jtalkcom% .
cl /c /clr /FU %jtalkcom% jtd_climc.cpp
link jtd_climc.obj /machine:%VSCMD_ARG_HOST_ARCH% /subsystem:"console" /entry:"Main"
