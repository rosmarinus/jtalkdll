set jtalkcom=JTalkCOM%VSCMD_ARG_HOST_ARCH%.dll
copy %JTALKDIR%\bin\%jtalkcom% .
vbc /platform:%VSCMD_ARG_HOST_ARCH% /target:winexe /reference:%jtalkcom% jtd_vbm.vb

