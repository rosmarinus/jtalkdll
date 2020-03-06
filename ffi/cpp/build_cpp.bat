set JTALKDIR=c:\open_jtalk
cl /EHsc /I %JTALKDIR%\include /c jtalkcpp.cpp
cl /EHsc /I %JTALKDIR%\include jtd_cpp.cpp jtalkcpp.obj %JTALKDIR%\lib\jtalk.lib

