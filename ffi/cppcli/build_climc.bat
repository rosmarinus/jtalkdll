set jtalkcom=JTalkCOM64.dll
if "%VSCMD_ARG_HOST_ARCH%" == "x86" set jtalkcom=JTalkCOM32.dll
cl /c /clr /FU %jtalkcom% jtd_climc.cpp
link jtd_climc.obj /machine:%VSCMD_ARG_HOST_ARCH% /subsystem:"console" /entry:"Main"
