rem implib /system jtalk32d.lib jtalk.dll
rem dmd jsay.d jtalk.d jtalk32d.lib -m32
rem dmd jsay.d jtalk.d jtalk.lib -m32mscoff
rem dmd jsay.d jtalk.d -L=jtalk.lib -m32mscoff
dmd jsay.d jtalk.d jtalk.lib -m64