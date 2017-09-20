set jtalkcom=JTalkCOM64.dll
if "%VSCMD_ARG_HOST_ARCH%" == "x86" set jtalkcom=JTalkCOM32.dll
cl /c /clr /FU %jtalkcom% jtd_clim.cpp
link jtd_clim.obj /machine:%VSCMD_ARG_HOST_ARCH% /subsystem:"windows" /entry:"Main"
