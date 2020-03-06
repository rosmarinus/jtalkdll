rem implib /system jtalk32d.lib jtalk.dll
rem dmd jtd_d.d jtalk.d jtalk32d.lib -m32
rem dmd jtd_d.d jtalk.d jtalk.lib -m32mscoff
rem dmd jtd_d.d jtalk.d -L=jtalk.lib -m32mscoff
dmd jtd_d.d jtalk.d jtalk.lib -m64

