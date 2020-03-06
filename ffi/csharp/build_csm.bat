set jtalkcom=JTalkCOM%VSCMD_ARG_HOST_ARCH%.dll
copy %JTALKDIR%\bin\%jtalkcom% .
csc /platform:%VSCMD_ARG_HOST_ARCH% /target:winexe /reference:%jtalkcom% jtd_csm.cs

