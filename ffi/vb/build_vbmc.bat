set jtalkcom=JTalkCOM64.dll
if "%VSCMD_ARG_HOST_ARCH%" == "x86" set jtalkcom=JTalkCOM32.dll
vbc /platform:%VSCMD_ARG_HOST_ARCH% /target:exe /reference:%jtalkcom% jtd_vbmc.vb