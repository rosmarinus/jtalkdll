rem implib /system jtalk32d.lib jtalk32.dll
rem dmd jtd_d.d jtalk.d jtalk32d.lib -m32
rem dmd jtd_d.d jtalk.d jtalk32.lib -m32mscoff
rem dmd jtd_d.d jtalk.d -L=jtalk32.lib -m32mscoff
dmd jtd_d.d jtalk.d jtalk64.lib -m64
